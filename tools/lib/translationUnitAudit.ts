/**
 * translationUnitAudit.ts — prove each translation unit's membership twice.
 *
 * `translationUnits.ts` says which original functions a configured `c`
 * subsegment's interval holds. That is the configuration's claim. Two
 * independent views check it:
 *
 *  - the **source view**: the pinned C AST lists the function definitions the
 *    file actually contains, top-level and nested, and the `INCLUDE_ASM` stubs
 *    it hands to the assembler;
 *  - the **object view**: the built object's symbol table lists the `.text`
 *    function symbols it emits, at which offsets.
 *
 * A unit is valid when the two agree with the configuration: every member is
 * defined and emitted exactly once, in original address order, at the offset
 * its original address implies, and nothing else is emitted. GCC nested
 * functions are the sanctioned exception: their parent's object emits them
 * immediately before the parent. Their consecutive per-function carrier files
 * define nothing; the parent's object starts at the first carried function. That
 * pairing is recognised explicitly rather than tolerated in general.
 *
 * An artifact that is missing or older than its source makes the object view
 * `undetermined`, never valid and never invalid.
 */

import { existsSync, readFileSync, statSync } from "fs";
import { createHash } from "crypto";
import { join, posix } from "path";
import { ROOT } from "./psxExeInfo.js";
import { elf32Sections, elf32Symbols } from "./elfSections.js";
import { requireContainer, type Container } from "./container.js";
import { loadOriginalFunctionInventory, loadSubsegments, loadSymbolAddresses, type OriginalFunctionSpan, type SplatSubsegment } from "./symbolIndex.js";
import {
  flagOverrideKey,
  isModuleStem,
  loadTranslationUnits,
  moduleNameIsGeneric,
  moduleSourcesOnDisk,
  moduleStemProblem,
  type ModuleDeclaration,
  type TranslationUnit,
} from "./translationUnits.js";
import { declaratorName, field, parseC, type Node } from "../agent/residual-source-search/tree-sitter-c.js";
import { walkActiveC } from "../agent/cSourceGuard.js";

// --- source view ---------------------------------------------------------------

export interface SourceDefinition {
  /** The symbol the definition emits: its C name, or the asm label a declaration gives it. */
  name: string;
  /** The C identifier, when an asm label renames it. */
  cName?: string;
  /** 1-based first and last lines of the definition. */
  line: number;
  endLine: number;
  startIndex: number;
  endIndex: number;
  /** A GNU nested function, defined inside `parent`. */
  nested: boolean;
  parent: string | null;
}

export interface SourceFacts {
  parses: boolean;
  definitions: SourceDefinition[];
  /** Symbols `INCLUDE_ASM` hands to the assembler. */
  includeAsm: string[];
  /** Identifiers each top-level definition references, for cohesion warnings. */
  references: Map<string, Set<string>>;
}

const sourceFactCache = new Map<string, SourceFacts>();

/** Function definitions and assembler stubs in one C file, from the pinned AST. */
export function sourceFacts(source: string): SourceFacts {
  const key = createHash("sha256").update(source).digest("hex");
  const cached = sourceFactCache.get(key);
  if (cached) return cached;
  const tree = parseC(source);
  const definitions: SourceDefinition[] = [];
  const includeAsm: string[] = [];
  const references = new Map<string, Set<string>>();
  /* `auto T f(...) __asm__("SYM");` renames what a nested definition emits;
     the definition's identity is the symbol, not the C spelling. */
  const labels = new Map<string, string>();
  try {
    walkActiveC(tree.rootNode, (node) => {
      if (node.type === "gnu_asm_expression") {
        const declaration = node.parent?.type === "function_declarator" ? node.parent.parent : node.parent;
        const literal = node.namedChildren.find((child) => child?.type === "string_literal");
        const declarator = declaration?.type === "declaration" ? field(declaration, "declarator") : undefined;
        const declared = declaratorName(declarator)?.text;
        if (declared && literal && /^"[A-Za-z_][A-Za-z0-9_.]*"$/.test(literal.text)) labels.set(declared, literal.text.slice(1, -1));
      }
      return true;
    });
    walkActiveC(tree.rootNode, (node) => {
      if (node.type === "function_definition") {
        const name = declaratorName(field(node, "declarator"))?.text;
        if (name) {
          let parent: Node | null = node.parent;
          while (parent && parent.type !== "function_definition") parent = parent.parent;
          const parentC = parent ? declaratorName(field(parent, "declarator"))?.text ?? null : null;
          const parentName = parentC ? labels.get(parentC) ?? parentC : null;
          const symbol = labels.get(name);
          definitions.push({
            name: symbol ?? name, ...(symbol ? { cName: name } : {}), line: node.startPosition.row + 1, endLine: node.endPosition.row + 1,
            startIndex: node.startIndex, endIndex: node.endIndex,
            nested: parent !== null, parent: parentName,
          });
          if (!parent) {
            const used = new Set<string>();
            for (const identifier of node.descendantsOfType("identifier")) if (identifier.text !== name) used.add(identifier.text);
            references.set(name, used);
          }
        }
      }
      if (node.type === "call_expression" && field(node, "function")?.text === "INCLUDE_ASM") {
        const args = field(node, "arguments");
        const symbol = args?.namedChildren.find((child) => child?.type === "identifier");
        if (symbol) includeAsm.push(symbol.text);
      }
      return true;
    });
    const facts = { parses: !tree.rootNode.hasError, definitions, includeAsm, references };
    sourceFactCache.set(key, facts);
    return facts;
  } finally {
    tree.delete();
  }
}

/** The definition a 1-based line falls in: the innermost, so a nested function keeps its own identity. */
export function enclosingDefinition(definitions: readonly SourceDefinition[], line: number): SourceDefinition | null {
  let found: SourceDefinition | null = null;
  for (const definition of definitions) {
    if (definition.line <= line && line <= definition.endLine) {
      if (!found || definition.startIndex >= found.startIndex) found = definition;
    }
  }
  return found;
}

// --- object view ---------------------------------------------------------------

export interface ObjectFunction {
  /** The function's own name: a nested function's `.N` suffix removed. */
  name: string;
  symbol: string;
  offset: number;
  size: number;
  nested: boolean;
}

export interface ObjectFacts {
  textSize: number;
  rodataSize: number;
  functions: ObjectFunction[];
  /** `<name>.NON_MATCHING` markers: symbols an `INCLUDE_ASM` stub assembled. */
  stubs: Set<string>;
}

/** `.text` function symbols of a built object, read in-process. */
export function objectFacts(objectPath: string): ObjectFacts | null {
  if (!existsSync(objectPath)) return null;
  const bytes = readFileSync(objectPath);
  const sections = elf32Sections(bytes);
  const functions: ObjectFunction[] = [];
  const stubs = new Set<string>();
  for (const symbol of elf32Symbols(bytes)) {
    if (symbol.name.endsWith(".NON_MATCHING")) {
      stubs.add(symbol.name.slice(0, -".NON_MATCHING".length));
      continue;
    }
    if (symbol.type !== 2 || symbol.section !== ".text" || symbol.size === 0) continue;
    const nested = symbol.name.match(/^(.+)\.\d+$/);
    functions.push({ name: nested ? nested[1]! : symbol.name, symbol: symbol.name, offset: symbol.value, size: symbol.size, nested: !!nested });
  }
  functions.sort((a, b) => a.offset - b.offset);
  return {
    textSize: sections.get(".text")?.size ?? 0,
    rodataSize: (sections.get(".rodata")?.size ?? 0) + (sections.get(".rdata")?.size ?? 0),
    functions,
    stubs,
  };
}

// --- audit -----------------------------------------------------------------------

export type UnitStatus = "valid" | "invalid" | "undetermined";

export interface UnitAudit {
  unit: TranslationUnit;
  status: UnitStatus;
  /** Why the unit is invalid. */
  findings: string[];
  /** Why a view could not be read; never collapsed into valid or invalid. */
  undetermined: string[];
  /** Review prompts that never fail a unit. */
  warnings: string[];
  /** Members the source defines (or a neighbour's nested definition carries). */
  defined: Set<string>;
  /** Members the source hands to the assembler. */
  stubbed: Set<string>;
}

export interface ContainerAudit {
  container: string;
  inventoryFindings: string[];
  units: UnitAudit[];
  /** Orphan module sources, duplicate definitions, stale per-function files. */
  findings: string[];
  warnings: string[];
}

export interface AuditOptions {
  /** Read built objects. Off for a purely source/config check. */
  objects?: boolean;
  /** Extra findings for a module, e.g. module-aware source policy. */
  moduleFindings?: (unit: TranslationUnit, source: string, facts: SourceFacts) => string[];
  /** Flag-override keys from configs/flag_overrides.mk. */
  flagOverrideKeys?: ReadonlySet<string>;
  /** Members count above which a module draws a review warning. */
  largeModuleWarning?: number;
}

/** `CC1FLAGS_<key>` keys of configs/flag_overrides.mk. */
export function loadFlagOverrideKeys(root = ROOT): Set<string> {
  const path = join(root, "configs/flag_overrides.mk");
  const keys = new Set<string>();
  if (!existsSync(path)) return keys;
  for (const line of readFileSync(path, "utf8").split("\n")) {
    const match = line.match(/^CC1FLAGS_(\S+?)\s*:?=/);
    if (match) keys.add(match[1]!);
  }
  return keys;
}

function hex(value: number): string {
  return `0x${value.toString(16).toUpperCase()}`;
}

function readSource(path: string): string | null {
  const absolute = join(ROOT, path);
  return existsSync(absolute) ? readFileSync(absolute, "utf8") : null;
}

/**
 * Audit every configured translation unit of one container, reading its
 * sources, objects and configuration from the tree.
 */
export function auditContainer(container: Container | string, options: AuditOptions = {}): ContainerAudit {
  const target = typeof container === "string" ? requireContainer(container) : container;
  const inventory = loadOriginalFunctionInventory(target);
  const units = loadTranslationUnits(target);
  const originalNames = new Set(inventory.functions.map((fn) => fn.name));
  const sources = new Map<string, string | null>();
  for (const unit of units) sources.set(unit.sourcePath, readSource(unit.sourcePath));
  let objects: Map<string, ObjectFacts | null> | undefined;
  const staleObjects = new Set<string>();
  if (options.objects !== false) {
    objects = new Map();
    for (const unit of units) {
      const absolute = join(ROOT, unit.objectPath);
      objects.set(unit.objectPath, objectFacts(absolute));
      const source = join(ROOT, unit.sourcePath);
      if (existsSync(absolute) && existsSync(source) && statSync(absolute).mtimeMs < statSync(source).mtimeMs) staleObjects.add(unit.objectPath);
    }
  }
  return auditUnits({
    container: target.id,
    srcDir: target.paths.srcDir,
    splatPath: target.paths.splat,
    inventory,
    units,
    segments: loadSubsegments(target),
    sources,
    ...(objects ? { objects } : {}),
    staleObjects,
    exists: (path) => existsSync(join(ROOT, path)),
    moduleSourcesOnDisk: moduleSourcesOnDisk(target),
    flagOverrideKeys: options.flagOverrideKeys ?? loadFlagOverrideKeys(),
    dataSymbols: new Set([...loadSymbolAddresses(target).keys()].filter((name) => !originalNames.has(name))),
    ...(options.moduleFindings ? { moduleFindings: options.moduleFindings } : {}),
    ...(options.largeModuleWarning !== undefined ? { largeModuleWarning: options.largeModuleWarning } : {}),
  });
}

/** Everything one container's audit reads, so the rules can run on synthetic layouts. */
export interface UnitAuditInput {
  container: string;
  /** Project-relative source root and splat config, for paths named in findings. */
  srcDir: string;
  splatPath: string;
  inventory: { functions: readonly OriginalFunctionSpan[]; findings: readonly string[] };
  units: readonly TranslationUnit[];
  segments: readonly SplatSubsegment[];
  /** Source text by project-relative path; null for a configured file that is absent. */
  sources: ReadonlyMap<string, string | null>;
  /** Object facts by project-relative path. Omitted: the object view is not read. */
  objects?: ReadonlyMap<string, ObjectFacts | null>;
  staleObjects?: ReadonlySet<string>;
  /** Whether a project-relative file exists (a leftover per-function source). */
  exists(path: string): boolean;
  moduleSourcesOnDisk: readonly string[];
  flagOverrideKeys: ReadonlySet<string>;
  /** Data symbol names, for the "shares a global" review warning. */
  dataSymbols: ReadonlySet<string>;
  moduleFindings?: AuditOptions["moduleFindings"];
  largeModuleWarning?: number;
}

/** The audit rules, over their inputs alone. */
export function auditUnits(input: UnitAuditInput): ContainerAudit {
  const { units, segments } = input;
  const flagKeys = input.flagOverrideKeys;
  const originalNames = new Set(input.inventory.functions.map((fn) => fn.name));
  const dataSymbols = input.dataSymbols;
  const readObjects = input.objects !== undefined;
  const findings: string[] = [];
  const warnings: string[] = [];

  const sources = new Map<string, string | null>();
  const facts = new Map<string, SourceFacts | null>();
  for (const unit of units) {
    const text = input.sources.get(unit.sourcePath) ?? null;
    sources.set(unit.segmentName, text);
    facts.set(unit.segmentName, text === null ? null : sourceFacts(text));
  }
  const objects = new Map<string, ObjectFacts | null>();
  const stale = new Map<string, boolean>();
  if (readObjects) {
    for (const unit of units) {
      objects.set(unit.segmentName, input.objects!.get(unit.objectPath) ?? null);
      stale.set(unit.segmentName, input.staleObjects?.has(unit.objectPath) ?? false);
    }
  }

  /* Consecutive empty carriers may precede one parent with several nested
     helpers. Admit only an unbroken run of single-member, code-free sources,
     each defined directly inside a member of that parent's actual source.
     The object check below still proves every emitted offset and the order. */
  const carriedBy = new Map<string, string>();
  const carriersOf = new Map<string, TranslationUnit[]>();
  units.forEach((parent, index) => {
    const parentFacts = facts.get(parent.segmentName);
    const carriers: TranslationUnit[] = [];
    let start = parent.startVram;
    for (let i = index - 1; i >= 0; i--) {
      const unit = units[i]!;
      const own = facts.get(unit.segmentName);
      if (unit.endVram !== start || !own || unit.members.length !== 1 ||
          own.definitions.length !== 0 || own.includeAsm.length !== 0) break;
      const member = unit.members[0]!;
      const nested = parentFacts?.definitions.find((d) => d.name === member.name && d.nested);
      if (!nested || !parent.members.some((m) => m.name === nested.parent)) break;
      carriedBy.set(unit.segmentName, parent.segmentName);
      carriers.unshift(unit);
      start = unit.startVram;
    }
    if (carriers.length > 0) carriersOf.set(parent.segmentName, carriers);
  });

  /* Top-level definitions across the container, for duplicate ownership. */
  const definers = new Map<string, string[]>();
  for (const unit of units) {
    for (const definition of facts.get(unit.segmentName)?.definitions ?? []) {
      if (definition.nested) continue;
      definers.set(definition.name, [...(definers.get(definition.name) ?? []), unit.sourcePath]);
    }
  }

  const audits: UnitAudit[] = [];
  units.forEach((unit, index) => {
    const unitFindings: string[] = [];
    const undetermined: string[] = [];
    const unitWarnings: string[] = [];
    const defined = new Set<string>();
    const stubbed = new Set<string>();
    const source = sources.get(unit.segmentName) ?? null;
    const view = facts.get(unit.segmentName) ?? null;
    const memberNames = new Set(unit.members.map((m) => m.name));
    const carrierUnits = carriersOf.get(unit.segmentName) ?? [];
    /* Functions this unit's object is expected to emit, in address order. */
    const carriedMembers = carrierUnits.flatMap((carrier) => carrier.members);
    const emitted = [...carriedMembers, ...unit.members];

    if (unit.members.length === 0) unitFindings.push(`${unit.segmentName}: the interval ${hex(unit.startVram)}..${hex(unit.endVram)} holds no original function`);
    for (const member of unit.members) {
      if (member.vram + member.size > unit.endVram) unitFindings.push(`${member.name} runs past the end of ${unit.segmentName} (${hex(unit.endVram)})`);
    }

    if (source === null || view === null) {
      unitFindings.push(`${unit.sourcePath} is configured but does not exist`);
    } else {
      if (!view.parses) unitFindings.push(`${unit.sourcePath} does not parse as C`);
      for (const member of unit.members) {
        const count = view.definitions.filter((d) => d.name === member.name).length;
        const stubs = view.includeAsm.filter((name) => name === member.name).length;
        if (stubs > 0) stubbed.add(member.name);
        if (count + stubs === 1) {
          if (count === 1) defined.add(member.name);
        } else if (count + stubs === 0 && carriedBy.has(unit.segmentName)) {
          defined.add(member.name);
        } else if (count + stubs === 0) {
          unitFindings.push(`${unit.sourcePath} does not define ${member.name}`);
        } else {
          unitFindings.push(`${unit.sourcePath} defines ${member.name} ${count + stubs} times`);
        }
      }
      const carried = new Set(carriedMembers.map((m) => m.name));
      for (const definition of view.definitions) {
        if (memberNames.has(definition.name) || (definition.nested && carried.has(definition.name))) continue;
        if (originalNames.has(definition.name)) unitFindings.push(`${unit.sourcePath} defines ${definition.name}, an original function outside its interval`);
        /* Anything else is a helper the object must not emit; checked below. */
      }
      for (const name of view.includeAsm) {
        if (!memberNames.has(name)) unitFindings.push(`${unit.sourcePath} hands ${name} to the assembler but its interval does not hold it`);
      }

      /* Source emission order: a nested definition is emitted just before its parent. */
      const position = (name: string): number | undefined => {
        const definition = view.definitions.find((d) => d.name === name);
        if (!definition) return undefined;
        if (!definition.nested) return definition.startIndex * 2 + 1;
        const parent = view.definitions.find((d) => d.name === definition.parent && !d.nested);
        return parent ? parent.startIndex * 2 : definition.startIndex * 2;
      };
      const ordered = emitted.map((fn) => ({ fn, at: position(fn.name) })).filter((entry) => entry.at !== undefined);
      for (let i = 1; i < ordered.length; i++) {
        if (ordered[i]!.at! < ordered[i - 1]!.at!) {
          unitFindings.push(`${unit.sourcePath} defines ${ordered[i]!.fn.name} before ${ordered[i - 1]!.fn.name}, against original address order`);
        }
      }
    }

    if (readObjects) {
      const object = objects.get(unit.segmentName) ?? null;
      if (!object) undetermined.push(`${unit.objectPath} is not built — run make`);
      else if (stale.get(unit.segmentName)) undetermined.push(`${unit.objectPath} is older than ${unit.sourcePath} — run make`);
      else if (carriedBy.has(unit.segmentName)) {
        if (object.textSize !== 0 || object.functions.length !== 0) unitFindings.push(`${unit.objectPath} emits code although its function is nested in ${carriedBy.get(unit.segmentName)}`);
      } else {
        const expected = new Map(emitted.map((fn) => [fn.name, fn]));
        const base = emitted[0]?.vram ?? unit.startVram;
        const seen = new Map<string, number>();
        for (const symbol of object.functions) {
          const fn = expected.get(symbol.name);
          if (!fn) {
            unitFindings.push(`${unit.objectPath} emits ${symbol.symbol}, which is not a member of ${unit.segmentName}`);
            continue;
          }
          seen.set(fn.name, (seen.get(fn.name) ?? 0) + 1);
          if (symbol.offset !== fn.vram - base) {
            unitFindings.push(`${unit.objectPath} places ${fn.name} at .text+${hex(symbol.offset)}; its original address implies +${hex(fn.vram - base)}`);
          }
        }
        for (const fn of emitted) {
          const count = seen.get(fn.name) ?? 0;
          if (count !== 1) unitFindings.push(`${unit.objectPath} emits ${fn.name} ${count} times`);
        }
        const extent = unit.endVram - base;
        if (object.textSize !== extent) unitFindings.push(`${unit.objectPath} has ${hex(object.textSize)} bytes of .text; its interval needs ${hex(extent)}`);
        for (const name of object.stubs) if (!expected.has(name)) unitFindings.push(`${unit.objectPath} assembles ${name}, which is not a member`);
      }
    }

    if (unit.foldedModule) {
      const problem = moduleStemProblem(unit.segmentName);
      if (problem) unitFindings.push(problem);
      if (moduleNameIsGeneric(unit.segmentName)) unitWarnings.push(`${unit.segmentName}: generic module name — say what the unit is responsible for`);
      /* A member the module still hands to the assembler is undecompiled, not
         folded: callers exclude it as they exclude any pending stub, and a
         live one fails the completion checks. It is no boundary. */
      const moduleKey = flagOverrideKey(unit.sourcePath);
      if (flagKeys.has(moduleKey)) {
        unitFindings.push(`configs/flag_overrides.mk overrides ${moduleKey}: a module flag override needs the evidence review in plans/flag-review-report.md`);
      }
      for (const member of unit.members) {
        if (flagKeys.has(member.name)) unitFindings.push(`configs/flag_overrides.mk still overrides ${member.name}, which no longer applies inside ${unit.segmentName}; reconcile it before folding`);
        const legacy = posix.join(input.srcDir, `${member.name}.c`);
        if (input.exists(legacy)) unitFindings.push(`${legacy} remains beside ${unit.sourcePath}: a stale per-function source`);
        for (const segment of segments) {
          if (segment.name === member.name) unitFindings.push(`${input.splatPath} still has a [${hex(segment.rom)}, ${segment.type}, ${member.name}] entry for a module member`);
        }
      }
      if (readObjects) {
        const object = objects.get(unit.segmentName);
        const attributed = segments.some((segment) => segment.name === unit.segmentName && (segment.type === ".rodata" || segment.type === ".rdata"));
        if (object && object.rodataSize > 0 && !attributed) unitFindings.push(`${unit.objectPath} emits ${hex(object.rodataSize)} bytes of .rodata with no [.rodata, ${unit.segmentName}] attribution — run deriveRodataSplits`);
        if (object && object.rodataSize === 0 && attributed) unitFindings.push(`${unit.segmentName} has a .rodata attribution but its object emits none`);
      }
      if (source !== null && view !== null && input.moduleFindings) unitFindings.push(...input.moduleFindings(unit, source, view));
      if (input.largeModuleWarning !== undefined && unit.members.length > input.largeModuleWarning) {
        unitWarnings.push(`${unit.segmentName} has ${unit.members.length} members — check that one original file really held them all`);
      }
      if (view && unit.members.length > 1) {
        const names = new Set(unit.members.map((m) => m.name));
        const uses = unit.members.map((member) => view.references.get(member.name) ?? new Set<string>());
        const calls = uses.some((used) => [...used].some((id) => names.has(id)));
        const shares = uses.some((used, i) => uses.some((other, j) => j > i && [...used].some((id) => dataSymbols.has(id) && other.has(id))));
        if (!calls && !shares) unitWarnings.push(`${unit.segmentName}: no member calls another or shares a global with one — check the boundary evidence`);
      }
    }

    const status: UnitStatus = unitFindings.length > 0 ? "invalid" : undetermined.length > 0 ? "undetermined" : "valid";
    audits.push({ unit, status, findings: unitFindings, undetermined, warnings: unitWarnings, defined, stubbed });
  });

  /* Duplicate definitions: one function defined by two configured sources. */
  for (const [name, paths] of definers) {
    if (paths.length > 1) findings.push(`${name} is defined by ${paths.length} configured sources: ${paths.join(", ")}`);
  }

  /* A module source no subsegment configures is not part of the build. */
  const configured = new Set(units.map((unit) => unit.sourcePath));
  for (const path of input.moduleSourcesOnDisk) {
    if (!configured.has(path)) findings.push(`${path} is an orphan module: no c subsegment configures it`);
  }
  for (const unit of units) {
    if (isModuleStem(unit.segmentName) && unit.segmentName.split("/").length !== 2) findings.push(`${unit.segmentName}: modules are one level deep`);
  }

  return { container: input.container, inventoryFindings: [...input.inventory.findings], units: audits, findings, warnings };
}

/** Members of one container whose translation unit hands them to the assembler. */
export function pendingStubs(audit: ContainerAudit): Set<string> {
  return new Set(audit.units.flatMap((unit) => [...unit.stubbed]));
}

export type { OriginalFunctionSpan };

/**
 * The modules a container's source tree declares, read from their ASTs.
 *
 * Every function symbol a module defines — top-level, nested, or handed to the
 * assembler by a leftover stub — is a member; a helper the original inventory
 * does not know (a `static inline` the compiler never emits) is not.
 */
export function moduleDeclarationsFromSources(container: Container | string): ModuleDeclaration[] {
  const target = typeof container === "string" ? requireContainer(container) : container;
  return moduleSourcesOnDisk(target).map((path) => {
    const facts = sourceFacts(readFileSync(join(ROOT, path), "utf8"));
    const stem = path.slice(target.paths.srcDir.length + 1).replace(/\.c$/, "");
    return { stem, names: new Set([...facts.definitions.map((d) => d.name), ...facts.includeAsm]) };
  });
}
