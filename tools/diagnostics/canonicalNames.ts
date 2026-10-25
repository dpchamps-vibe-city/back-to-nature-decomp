#!/usr/bin/env npx tsx
/**
 * canonicalNames.ts — carry each established function or data name to every reference.
 *
 * A function keeps the address-derived name splat gave it until someone names
 * it. Naming it updates the defining container's symbol table; its own
 * definition, in the unit that owns its address, is renamed with its uses.
 * A reference written before the name existed still spells the
 * alias — and an overlay is a separate link, so its alias resolves through
 * splat's generated tables for exactly as long as the engine export fails to
 * carry the real name. Nothing fails while that holds; one regeneration later
 * the alias stops resolving.
 *
 * Identity is the defining container plus the exact entry address, never the
 * address alone: overlays that share a RAM slot put different functions at one
 * address. An unprefixed alias in an overlay source can only mean the PS-X EXE;
 * a prefixed one names its container. Anything else is reported, not guessed.
 *
 * A global is named the same way: a row in the defining container's symbol
 * configuration (`configs/symbols/exe.txt`, or an overlay's `.names.txt` with
 * `type:data`) establishes the name, and every `D_<address>` spelling of it —
 * the tentative definition, the `globals_override.h` extern, each use — is
 * rewritten. The extern stays in its header, renamed; only a function alias a
 * header declares as a workaround is moved into the files that use it.
 *
 * The rewrite is name-only and reads the tree-sitter parse, so comments, string
 * literals, disabled preprocessor arms and same-named locals are never touched.
 * A reference inside a macro body or an assembly string is reported and left
 * alone, as is a rename that would collide with a declaration already in scope.
 *
 * Usage:
 *   npx tsx tools/diagnostics/canonicalNames.ts            # report
 *   npx tsx tools/diagnostics/canonicalNames.ts --check    # exit 1 on an obsolete alias or stale generated name
 *   npx tsx tools/diagnostics/canonicalNames.ts --write    # rewrite obsolete aliases in active sources
 *
 * To name a global: add `Name = 0xADDRESS; // size:0xN` to configs/symbols/exe.txt
 * (EXE) or `Name = 0xADDRESS; // type:data size:0xN` to configs/symbols/<id>.names.txt
 * (overlay), then run --write and re-split the container.
 */

import { existsSync, readFileSync, readdirSync, statSync, writeFileSync } from "node:fs";
import { join, resolve } from "node:path";
import { fileURLToPath } from "node:url";
import { ROOT } from "../lib/psxExeInfo.js";
import { containsVram, loadContainers, type Container } from "../lib/container.js";
import { ENGINE_EXPORT_PATH } from "../lib/symbolIndex.js";
import { loadTranslationUnits } from "../lib/translationUnits.js";
import { sourceFacts } from "../lib/translationUnitAudit.js";
import { configuredCSources } from "../build/buildInputs.js";
import { analyzeCSource, applyCSourceEdits, walkActiveC, type CSourceEdit } from "../agent/cSourceGuard.js";
import { prototypesIn } from "../agent/calleeTruth.js";
import { declaratorName, field, namedChildren, parseC, type Node } from "../agent/residual-source-search/tree-sitter-c.js";

/* ------------------------------------------------------------------ */
/* Identity                                                            */
/* ------------------------------------------------------------------ */

const ALIAS = /^(?:(?:(ovl_\d+)_)?func|(D))_([0-9A-F]{8})$/;
const ALIAS_TOKEN = /\b(?:(?:ovl_\d+_)?func|D)_[0-9A-F]{8}\b/g;

/**
 * The container prefix and address splat encodes in an anonymous name.
 *
 * A data-shaped `D_` name is decoded too: splat gives one to an address it sees
 * taken rather than called, so a callback into a function the engine export did
 * not name arrives as `D_<entry>`. It is a function alias where a function entry
 * is established, a data alias where the container's configuration names that
 * address as data, and otherwise an anonymous global this pass leaves alone.
 */
export function decodeAlias(name: string): { prefix: string | null; data: boolean; address: number } | null {
  const match = name.match(ALIAS);
  return match ? { prefix: match[1] ?? null, data: match[2] !== undefined, address: Number(`0x${match[3]}`) } : null;
}

export interface NameTable {
  containers: Container[];
  /** Container id -> entry address -> established function name. */
  functions: Map<string, Map<number, string>>;
  /** Container id -> address -> established data name; absent, no global is named. */
  data?: Map<string, Map<number, string>>;
  /** Container id -> every name the container itself defines, for genuine public aliases. */
  defined: Map<string, Set<string>>;
  /** Container id -> function names an active source defines. */
  sourceDefinitions: Map<string, Set<string>>;
  /** Container id -> aliases defined, or stubbed, by the translation unit that owns their address: the function itself. */
  ownDefinitions?: Map<string, Set<string>>;
  /** Alias-shaped name -> the hand-maintained header that declares it. */
  headerDeclarations: Map<string, string>;
}

export type Resolution =
  | { kind: "current" }
  | { kind: "obsolete"; container: string; address: number; canonical: string; data?: true }
  | { kind: "ambiguous"; reason: string };

function hex(address: number): string {
  return `0x${address.toString(16).toUpperCase().padStart(8, "0")}`;
}

/**
 * What an alias-shaped name written in `from` refers to, and whether that
 * function has a name of its own. `null` when the name is not alias-shaped.
 */
export function resolveAlias(name: string, from: string, table: NameTable): Resolution | null {
  const decoded = decodeAlias(name);
  if (!decoded) return null;
  if (decoded.data) {
    const exe = table.containers.find((c) => c.kind === "exe");
    const local = table.containers.find((c) => c.id === from && c.kind === "overlay" && containsVram(c, decoded.address));
    const owner = local ?? (exe && containsVram(exe, decoded.address) ? exe : undefined);
    if (!owner || !table.functions.get(owner.id)?.has(decoded.address)) {
      /* A global: its own overlay's name for it, else the EXE's, whose data
         reaches past its image into BSS. */
      const named = [local, exe].find((c) => c && table.data?.get(c.id)?.has(decoded.address));
      if (!named) return null;
      const canonical = table.data!.get(named.id)!.get(decoded.address)!;
      if (canonical === name || table.defined.get(named.id)?.has(name)) return { kind: "current" };
      return { kind: "obsolete", container: named.id, address: decoded.address, canonical, data: true };
    }
  }
  const source = table.containers.find((c) => c.id === from);
  if (!source) return { kind: "ambiguous", reason: `no container ${from}` };

  let target: Container | undefined;
  if (decoded.prefix) {
    target = table.containers.find((c) => c.id === decoded.prefix && c.kind === "overlay");
    if (!target) return { kind: "ambiguous", reason: `no overlay container ${decoded.prefix}` };
  } else if (decoded.data && source.kind === "overlay" && containsVram(source, decoded.address)) {
    target = source;
  } else {
    const exe = table.containers.find((c) => c.kind === "exe");
    if (exe && containsVram(exe, decoded.address)) target = exe;
    else if (table.containers.some((c) => c.kind === "overlay" && containsVram(c, decoded.address))) {
      return { kind: "ambiguous", reason: `${hex(decoded.address)} is in an overlay slot; an unprefixed name cannot say which member` };
    } else return { kind: "ambiguous", reason: `${hex(decoded.address)} is in no container` };
  }

  const canonical = table.functions.get(target.id)?.get(decoded.address);
  if (!canonical || canonical === name) return { kind: "current" };
  /* Both names are defined: a genuine public alias, not a stale spelling. */
  if (table.defined.get(target.id)?.has(name)) return { kind: "current" };
  /* The unit that owns the address defines it under the alias: that is the
     function itself, named since, and its definition is renamed with its uses. */
  if (table.ownDefinitions?.get(target.id)?.has(name)) return { kind: "obsolete", container: target.id, address: decoded.address, canonical };
  if (table.sourceDefinitions.get(target.id)?.has(name)) {
    return { kind: "ambiguous", reason: `an active ${target.id} source defines ${name}, but its symbol table names ${hex(decoded.address)} ${canonical}` };
  }
  /* A data-shaped alias still wants the function's own name, anonymous or not. */
  if (decodeAlias(canonical) && !decoded.data) {
    return { kind: "ambiguous", reason: `${target.id} names ${hex(decoded.address)} ${canonical}, another anonymous spelling` };
  }
  return { kind: "obsolete", container: target.id, address: decoded.address, canonical };
}

/** `type:func` rows of a container's symbol table: the record of established names. */
function establishedFunctions(container: Container): Map<number, string> {
  const result = new Map<number, string>();
  const path = join(ROOT, container.paths.symbolAddrs);
  if (!existsSync(path)) return result;
  for (const line of readFileSync(path, "utf-8").split("\n")) {
    const match = line.match(/^\s*([A-Za-z_][A-Za-z0-9_]*)\s*=\s*(0x[0-9A-Fa-f]+)\s*;.*\btype:func\b/);
    if (match) result.set(Number(match[2]), match[1]!);
  }
  return result;
}

/**
 * Named globals: the symbol configuration's rows that are neither functions
 * nor code labels, and an overlay's `type:data` labels, which its generated
 * table carries only after a re-split.
 */
function establishedData(container: Container, functions: ReadonlyMap<number, string>): Map<number, string> {
  const result = new Map<number, string>();
  const paths = [container.paths.symbolAddrs, ...(container.kind === "overlay" ? [container.paths.symbolAddrs.replace(/\.txt$/, ".names.txt")] : [])];
  for (const path of paths) {
    if (!existsSync(join(ROOT, path))) continue;
    for (const line of readFileSync(join(ROOT, path), "utf-8").split("\n")) {
      const match = line.match(/^\s*([A-Za-z_][A-Za-z0-9_]*)\s*=\s*(0x[0-9A-Fa-f]+)\s*;(.*)$/);
      if (!match || decodeAlias(match[1]!)) continue;
      const type = match[3]!.match(/\btype:(\w+)/)?.[1];
      if (type === "func" || type === "label" || (path.endsWith(".names.txt") && type !== "data")) continue;
      const address = Number(match[2]);
      if (functions.has(address) || (container.kind === "overlay" && !containsVram(container, address))) continue;
      if (!result.has(address)) result.set(address, match[1]!);
    }
  }
  return result;
}

/** Container id of a configured source path. */
export function sourceContainer(path: string): string {
  return path.match(/^src\/overlays\/([^/]+)\//)?.[1] ?? "exe";
}

/** Hand-maintained headers; generated and vendored ones are outputs, not sources. */
export function handHeaders(): string[] {
  const generated = /^include\/(?:globals\.h|functions\.h|sdk_types\.h|overlays\/|psyq\/)/;
  const found: string[] = [];
  const visit = (dir: string): void => {
    for (const entry of readdirSync(join(ROOT, dir))) {
      const path = `${dir}/${entry}`;
      if (statSync(join(ROOT, path)).isDirectory()) visit(path);
      else if (path.endsWith(".h") && !generated.test(path)) found.push(path);
    }
  };
  visit("include");
  return found.sort();
}

export function loadNameTable(sources: ReadonlyMap<string, string>): NameTable {
  const containers = loadContainers();
  const functions = new Map(containers.map((c) => [c.id, establishedFunctions(c)]));
  const data = new Map(containers.map((c) => [c.id, establishedData(c, functions.get(c.id)!)]));
  const defined = new Map<string, Set<string>>();
  for (const container of containers) {
    /* Only the defining symbol table establishes public aliases. The broad
       relocation index also contains old, inactive nonmatchings/*.s labels
       and undefined references; neither is evidence of a second definition. */
    const own = assignments(container.paths.symbolAddrs)
      .filter(({ address }) => container.kind === "exe" || containsVram(container, address));
    defined.set(container.id, new Set(own.map(({ name }) => name)));
  }
  const sourceDefinitions = new Map<string, Set<string>>(containers.map((c) => [c.id, new Set<string>()]));
  const definitionsIn = new Map<string, Set<string>>();
  const headerDeclarations = new Map<string, string>();
  for (const [path, text] of sources) {
    if (path.endsWith(".h")) {
      for (const name of declaredNames(text)) if (decodeAlias(name)) headerDeclarations.set(name, path);
      continue;
    }
    const into = sourceDefinitions.get(sourceContainer(path));
    if (!into) continue;
    const here = new Set<string>(sourceFacts(text).includeAsm);
    for (const prototype of prototypesIn(text, path)) if (prototype.kind === "definition") { into.add(prototype.name); here.add(prototype.name); }
    definitionsIn.set(path, here);
  }
  const ownDefinitions = new Map<string, Set<string>>();
  for (const container of containers) {
    const own = new Set<string>();
    for (const unit of loadTranslationUnits(container)) {
      const defined = definitionsIn.get(unit.sourcePath);
      if (!defined) continue;
      for (const member of unit.members) {
        const alias = `${container.kind === "exe" ? "" : `${container.id}_`}func_${member.vram.toString(16).toUpperCase().padStart(8, "0")}`;
        if (defined.has(alias)) own.add(alias);
      }
    }
    ownDefinitions.set(container.id, own);
  }
  return { containers, functions, data, defined, sourceDefinitions, ownDefinitions, headerDeclarations };
}

/* ------------------------------------------------------------------ */
/* References                                                          */
/* ------------------------------------------------------------------ */

export type ReferenceContext = "code" | "macro" | "assembly";

export interface AliasReference {
  name: string;
  line: number;
  start: number;
  end: number;
  context: ReferenceContext;
  resolution: Resolution;
  /** Why an obsolete code reference is still not rewritten. */
  blocked?: string;
  /** The hand header whose declaration this reference binds to; renaming it moves that declaration. */
  via?: string;
}

const ASM_MACROS = new Set(["INCLUDE_ASM", "INCLUDE_RODATA"]);

/** True when the declarator's identifier names a function rather than an object. */
export function declaresFunction(declarator: Node): boolean {
  const name = declaratorName(declarator);
  let parent = name?.parent;
  while (parent?.type === "parenthesized_declarator") parent = parent.parent;
  return parent?.type === "function_declarator";
}

export function declarators(declaration: Node): Node[] {
  return declaration.childrenForFieldName("declarator").filter((d): d is Node => d !== null);
}

/** The scalar typedefs every source receives from `common.h`. */
export const COMMON_TYPES: ReadonlySet<string> = new Set(
  ["u8", "u16", "u32", "s8", "s16", "s32", "vu8", "vu16", "vu32", "vs8", "vs16", "vs32"]
);

/**
 * The object a statement declares when tree-sitter, which cannot know which
 * identifiers are typedefs, has read `T (*name)(args);` or `T (*name)[n];` as
 * a call of `T`. Only a known type name makes it a declaration.
 */
export function misparsedDeclaration(statement: Node, isType: (name: string) => boolean): string | undefined {
  if (statement.type !== "expression_statement") return undefined;
  let expression = namedChildren(statement)[0];
  while (expression && (expression.type === "call_expression" || expression.type === "subscript_expression")) {
    const inner = field(expression, expression.type === "call_expression" ? "function" : "argument");
    if (expression.type === "call_expression" && inner?.type === "identifier") {
      if (!isType(inner.text)) return undefined;
      const args = namedChildren(field(expression, "arguments")!);
      let target = args.length === 1 ? args[0] : undefined;
      while (target?.type === "pointer_expression") target = field(target, "argument");
      return target?.type === "identifier" && args[0]?.type === "pointer_expression" ? target.text : undefined;
    }
    expression = inner;
  }
  return undefined;
}

/**
 * How `name` is bound at `node`: by the nearest enclosing scope that declares
 * it before this point. `null` means no declaration in this file — a header's,
 * or an implicit one. With `fileScope` false only a block or a parameter
 * counts: a file-scope object of a global's name is that global.
 */
export function bindingAt(
  node: Node,
  name: string,
  isType: (name: string) => boolean = (n) => COMMON_TYPES.has(n),
  fileScope = true
): "function" | "object" | null {
  for (let scope = node.parent; scope; scope = scope.parent) {
    let found: "function" | "object" | null = null;
    if (scope.type === "translation_unit" && !fileScope) break;
    if (scope.type === "compound_statement" || scope.type === "translation_unit" || scope.type === "for_statement") {
      for (const child of namedChildren(scope)) {
        if (child.startIndex > node.startIndex) break;
        if (child.type === "expression_statement" && misparsedDeclaration(child, isType) === name) found = "object";
        if (child.type === "declaration") {
          for (const declarator of declarators(child)) {
            if (declaratorName(declarator)?.text === name) found = declaresFunction(declarator) ? "function" : "object";
          }
        } else if (child.type === "function_definition" && declaratorName(field(child, "declarator"))?.text === name) {
          found = "function";
        }
      }
    } else if (scope.type === "function_definition") {
      const parameters = findParameters(field(scope, "declarator"));
      for (const parameter of parameters ? namedChildren(parameters) : []) {
        if (parameter.type === "parameter_declaration" && declaratorName(field(parameter, "declarator"))?.text === name) found = "object";
      }
    }
    if (found) return found;
  }
  return null;
}

function findParameters(declarator: Node | undefined): Node | undefined {
  for (let node = declarator; node; node = field(node, "declarator")) {
    if (node.type === "function_declarator") return field(node, "parameters");
  }
  return undefined;
}

function insideAsmMacro(node: Node): boolean {
  for (let parent = node.parent; parent; parent = parent.parent) {
    if (parent.type === "call_expression" && ASM_MACROS.has(field(parent, "function")?.text ?? "")) return true;
  }
  return false;
}

/** The function an `INCLUDE_ASM` stub hands to the assembler: splat extracts its assembly under its configured name. */
function stubbedFunction(node: Node): boolean {
  const call = node.parent?.type === "argument_list" ? node.parent.parent : null;
  return call?.type === "call_expression" && field(call, "function")?.text === "INCLUDE_ASM";
}

function insideAsmStatement(node: Node): boolean {
  for (let parent = node.parent; parent; parent = parent.parent) if (parent.type === "gnu_asm_expression") return true;
  return false;
}

/** Every name a declaration in this text introduces, at any scope. */
function declaredNames(source: string): string[] {
  const tree = parseC(source);
  const names: string[] = [];
  try {
    walkActiveC(tree.rootNode, (node) => {
      if (node.type !== "declaration") return node.type !== "comment";
      for (const declarator of declarators(node)) {
        const name = declaratorName(declarator)?.text;
        if (name) names.push(name);
      }
      return false;
    });
  } finally {
    tree.delete();
  }
  return names;
}

/** Normalized prototype per function name this source declares or defines. */
function declaredShapes(source: string): Map<string, Set<string>> {
  const shapes = new Map<string, Set<string>>();
  for (const p of prototypesIn(source, "source")) {
    const shape = `${p.returnType ?? ""}(${p.parameters === null ? "?" : (p.paramTypes ?? []).join(",")}${p.variadic ? ",..." : ""})`.replace(/\s+/g, " ");
    const set = shapes.get(p.name) ?? new Set<string>();
    set.add(shape);
    shapes.set(p.name, set);
  }
  return shapes;
}

/**
 * Every alias-shaped reference in one source, with what it refers to.
 *
 * A header's declaration is everyone's: renaming it changes what every
 * includer is told, so in a header, and wherever a header's declaration is the
 * one a reference binds to, the rename is reported as an interface change.
 */
export function aliasReferences(source: string, from: string, table: NameTable, options: { header?: boolean } = {}): AliasReference[] {
  const tree = parseC(source);
  const found: AliasReference[] = [];
  const tokens = (node: Node, context: ReferenceContext): void => {
    for (const match of node.text.matchAll(ALIAS_TOKEN)) {
      const resolution = resolveAlias(match[0], from, table);
      if (!resolution || resolution.kind === "current") continue;
      const start = node.startIndex + match.index!;
      found.push({
        name: match[0], start, end: start + match[0].length, context, resolution,
        line: source.slice(0, start).split("\n").length,
      });
    }
  };
  try {
    walkActiveC(tree.rootNode, (node) => {
      if (node.type === "comment") return false;
      if (node.type === "preproc_arg") {
        tokens(node, "macro");
        return false;
      }
      if (node.type === "string_literal") {
        if (insideAsmStatement(node)) tokens(node, "assembly");
        return false;
      }
      if (node.type !== "identifier") return true;
      const resolution = resolveAlias(node.text, from, table);
      if (!resolution || resolution.kind === "current") return true;
      const parent = node.parent?.type ?? "";
      /* A stub of the function itself is renamed with it; the next split
         extracts its assembly under the new name. */
      const ownStub = resolution.kind === "obsolete" && stubbedFunction(node) && !!table.ownDefinitions?.get(resolution.container)?.has(node.text);
      const context: ReferenceContext = insideAsmMacro(node) && !ownStub ? "assembly"
        : parent === "preproc_def" || parent === "preproc_function_def" ? "macro" : "code";
      /* A same-named local is not the function, nor the global. */
      const data = (resolution as { data?: true }).data === true;
      if (context === "code" && bindingAt(node, node.text, undefined, !data) === "object") return true;
      found.push({
        name: node.text, start: node.startIndex, end: node.endIndex, context, resolution,
        line: node.startPosition.row + 1,
      });
      return true;
    });

    /* A rename must not change which declaration a reference binds to. */
    const obsolete = found.filter((r) => r.context === "code" && r.resolution.kind === "obsolete");
    for (const reference of obsolete) {
      /* A global's extern belongs in its header; it is renamed there, in place. */
      if ((reference.resolution as { data?: true }).data) continue;
      const header = table.headerDeclarations.get(reference.name);
      const node = tree.rootNode.descendantForIndex(reference.start, reference.end)!;
      if (options.header) {
        /* Only the declaration itself can move; any other use stays put. */
        const declaration = node.parent;
        if (header && /declarator$/.test(declaration?.type ?? "")) reference.via = header;
        else reference.blocked = "used inside a shared header; renaming it is an interface change for every includer";
      } else if (header && bindingAt(node, reference.name) === null) {
        reference.via = header;
      }
    }
    if (obsolete.some((r) => !r.blocked)) {
      const shapes = declaredShapes(source);
      for (const reference of obsolete.filter((r) => !r.blocked)) {
        const canonical = (reference.resolution as { canonical: string }).canonical;
        const node = tree.rootNode.descendantForIndex(reference.start, reference.end)!;
        if (bindingAt(node, canonical, undefined, !(reference.resolution as { data?: true }).data) === "object") {
          reference.blocked = `a local object named ${canonical} is in scope`;
          continue;
        }
        const mine = reference.via ? undefined : shapes.get(reference.name), theirs = shapes.get(canonical);
        if (reference.via && theirs) reference.blocked = `the file already declares ${canonical}; moving ${reference.via}'s declaration here would restate it`;
        else if (mine && theirs && [...mine, ...theirs].some((shape) => !theirs.has(shape) || !mine.has(shape))) {
          reference.blocked = `the file also declares ${canonical} with a different prototype`;
        }
      }
    }
  } finally {
    tree.delete();
  }
  return found.sort((a, b) => a.start - b.start);
}

/**
 * Edits that delete these declarations, each with the lines it owns.
 *
 * A declaration alone on its lines takes them with it, along with a comment
 * trailing it on its last line. The comment directly above a run of adjacent
 * declarations documents that run: it goes when every declaration in the run
 * does, or when it heads the removed declaration and names it but no other
 * declaration in the run. A deletion that would leave two blank lines leaves
 * one.
 */
export function declarationRemovals(source: string, targets: readonly Node[]): CSourceEdit[] {
  const lineStart = (index: number): number => source.lastIndexOf("\n", index - 1) + 1;
  const lineEnd = (index: number): number => {
    const next = source.indexOf("\n", index);
    return next < 0 ? source.length : next + 1;
  };
  const row = (index: number): number => source.slice(0, index).split("\n").length;
  const removed = new Set(targets.map((t) => t.id));
  const ranges: Array<[number, number]> = [];

  for (const target of targets) {
    let end = target.endIndex;
    for (let next = target.nextNamedSibling; next?.type === "comment" && row(next.startIndex) === row(target.endIndex); next = next.nextNamedSibling) {
      end = next.endIndex;
    }
    const start = lineStart(target.startIndex);
    const isolated = source.slice(start, target.startIndex).trim() === "" && source.slice(end, lineEnd(end)).trim() === "";
    if (!isolated) {
      ranges.push([target.startIndex, target.endIndex]);
      continue;
    }
    ranges.push([start, lineEnd(end)]);

    const adjacent = (a: Node, b: Node): boolean => row(b.startIndex) === row(a.endIndex) + 1;
    let first = target;
    while (first.previousNamedSibling?.type === "declaration" && adjacent(first.previousNamedSibling, first)) first = first.previousNamedSibling;
    let last = target;
    while (last.nextNamedSibling?.type === "declaration" && adjacent(last, last.nextNamedSibling)) last = last.nextNamedSibling;
    const run: Node[] = [];
    for (let node: Node | null = first; node; node = node.nextNamedSibling) {
      run.push(node);
      if (node.id === last.id) break;
    }
    const comment = first.previousNamedSibling;
    const names = (declaration: Node): string[] => declarators(declaration).flatMap((d) => declaratorName(d)?.text ?? []);
    const mentions = (name: string): boolean => new RegExp(`\\b${name}\\b`).test(comment!.text);
    const owned = (): boolean => run.every((d) => removed.has(d.id)) || (
      first.id === target.id && names(target).some(mentions) &&
      !run.some((d) => d.id !== target.id && names(d).some(mentions))
    );
    if (comment?.type === "comment" && adjacent(comment, first) && owned() &&
        source.slice(lineStart(comment.startIndex), comment.startIndex).trim() === "") {
      ranges.push([lineStart(comment.startIndex), lineEnd(comment.endIndex)]);
    }
  }

  ranges.sort((a, b) => a[0] - b[0]);
  const merged: Array<[number, number]> = [];
  for (const range of ranges) {
    const previous = merged[merged.length - 1];
    if (previous && range[0] <= previous[1]) previous[1] = Math.max(previous[1], range[1]);
    else merged.push([...range]);
  }
  return merged.map(([start, end]) => {
    const blankBefore = start === 0 || (source[start - 1] === "\n" && (start === 1 || source[start - 2] === "\n"));
    const blankAfter = source[end] === "\n";
    return { start, end: blankBefore && blankAfter && start === lineStart(start) ? end + 1 : end, text: "" };
  });
}

/** File-scope declarations, including those inside include guards and other conditionals. */
function fileScopeDeclarations(root: Node, names: ReadonlySet<string>): Node[] {
  const found: Node[] = [];
  walkActiveC(root, (node) => {
    if (node.type === "function_definition" || node.type === "comment") return false;
    if (node.type !== "declaration") return true;
    if (declarators(node).some((d) => names.has(declaratorName(d)?.text ?? ""))) found.push(node);
    return false;
  });
  return found;
}

/** Where a file-scope declaration goes: after the last top-level `#include`. */
export function declarationInsertion(source: string, root: Node, lines: readonly string[]): CSourceEdit {
  const includes = namedChildren(root).filter((n) => n.type === "preproc_include");
  const at = includes.length > 0 ? includes[includes.length - 1]!.endIndex : 0;
  const body = lines.join("\n");
  return at === 0 ? { start: 0, end: 0, text: `${body}\n\n` } : { start: at, end: at, text: `\n${body}\n` };
}

/** The source with every unblocked obsolete code reference renamed, plus any planned edits. */
export function rewriteAliases(source: string, references: readonly AliasReference[], extra: readonly CSourceEdit[] = []): string {
  const edits: CSourceEdit[] = references
    .filter((r) => r.context === "code" && !r.blocked && r.resolution.kind === "obsolete")
    .map((r) => ({ start: r.start, end: r.end, text: (r.resolution as { canonical: string }).canonical }));
  return edits.length + extra.length === 0 ? source : applyCSourceEdits(source, [...edits, ...extra]);
}

/**
 * Carry header-bound aliases into the files that use them.
 *
 * A hand header that declares a function under its alias is a workaround for a
 * name the build could not supply. Renaming it there would change what every
 * includer is told, and other files may describe the same function
 * differently. So each file that binds to it receives that declaration,
 * renamed, as its own, and the header entry goes. Every file keeps exactly the
 * prototype it compiled against. An alias whose move is unsafe anywhere moves
 * nowhere.
 */
export function planHeaderRelocations(
  sources: ReadonlyMap<string, string>,
  references: ReadonlyMap<string, AliasReference[]>
): Map<string, CSourceEdit[]> {
  const edits = new Map<string, CSourceEdit[]>();
  const add = (path: string, edit: CSourceEdit): void => {
    edits.set(path, [...(edits.get(path) ?? []), edit]);
  };
  const byAlias = new Map<string, Array<[string, AliasReference]>>();
  for (const [path, refs] of references) {
    for (const ref of refs) if (ref.via) byAlias.set(ref.name, [...(byAlias.get(ref.name) ?? []), [path, ref]]);
  }

  const inserts = new Map<string, string[]>();
  const moved = new Map<string, Set<string>>();
  for (const [alias, sites] of [...byAlias].sort((a, b) => a[0].localeCompare(b[0]))) {
    const header = sites[0]![1].via!;
    const canonical = (sites[0]![1].resolution as { canonical: string }).canonical;
    const block = (why: string): void => {
      for (const [, ref] of sites) ref.blocked = why;
    };
    const tree = parseC(sources.get(header)!);
    try {
      const declarations = fileScopeDeclarations(tree.rootNode, new Set([alias]));
      const declaration = declarations[0];
      if (declarations.length !== 1 || declarators(declaration!).length !== 1) {
        block(`${header} does not declare ${alias} in exactly one single-declarator declaration`);
        continue;
      }
      if (sites.some(([, ref]) => ref.blocked)) {
        block(`a use of ${alias} cannot be renamed, so its ${header} declaration stays`);
        continue;
      }
      const name = declaratorName(declarators(declaration!)[0]!)!;
      const renamed = declaration!.text.slice(0, name.startIndex - declaration!.startIndex) + canonical +
        declaration!.text.slice(name.endIndex - declaration!.startIndex);
      moved.set(header, (moved.get(header) ?? new Set<string>()).add(alias));
      for (const path of new Set(sites.map(([p]) => p).filter((p) => p !== header))) {
        inserts.set(path, [...(inserts.get(path) ?? []), renamed]);
      }
    } finally {
      tree.delete();
    }
  }
  for (const [header, aliases] of moved) {
    const tree = parseC(sources.get(header)!);
    try {
      for (const edit of declarationRemovals(sources.get(header)!, fileScopeDeclarations(tree.rootNode, aliases))) add(header, edit);
    } finally {
      tree.delete();
    }
  }
  for (const [path, lines] of inserts) {
    const tree = parseC(sources.get(path)!);
    try {
      add(path, declarationInsertion(sources.get(path)!, tree.rootNode, [...lines].sort()));
    } finally {
      tree.delete();
    }
  }
  return edits;
}

/* ------------------------------------------------------------------ */
/* Generated output                                                    */
/* ------------------------------------------------------------------ */

export interface StaleName {
  file: string;
  line: number;
  name: string;
  detail: string;
}

function assignments(path: string): Array<{ name: string; address: number; line: number }> {
  if (!existsSync(join(ROOT, path))) return [];
  return readFileSync(join(ROOT, path), "utf-8").split("\n").flatMap((text, index) => {
    const match = text.match(/^\s*([A-Za-z_][A-Za-z0-9_]*)\s*=\s*(0x[0-9A-Fa-f]+)\s*;/);
    return match ? [{ name: match[1]!, address: Number(match[2]), line: index + 1 }] : [];
  });
}

function filesUnder(dir: string, suffix: string): string[] {
  if (!existsSync(join(ROOT, dir))) return [];
  const found: string[] = [];
  for (const entry of readdirSync(join(ROOT, dir), { withFileTypes: true })) {
    const path = `${dir}/${entry.name}`;
    if (entry.isDirectory()) found.push(...filesUnder(path, suffix));
    else if (entry.name.endsWith(suffix)) found.push(path);
  }
  return found;
}

/**
 * Anonymous aliases that regeneration wrote where a name is established: the
 * engine export, every container's undefined tables, the assembly its link
 * consumes, and the generated function headers. A missing build artifact is
 * skipped, not failed.
 *
 * Only assembly the build reads is scanned. splat writes `nonmatchings/` only
 * for an `INCLUDE_ASM` stub and never deletes one, so a function that has since
 * become C leaves its old file behind under whatever names it had then.
 */
export function staleGeneratedNames(table: NameTable, sources: ReadonlyMap<string, string>): StaleName[] {
  const stale: StaleName[] = [];
  const exe = table.containers.find((c) => c.kind === "exe")!;
  const overlays = table.containers.filter((c) => c.kind === "overlay");

  const exported = assignments(ENGINE_EXPORT_PATH);
  if (exported.length > 0) {
    const byAddress = new Map(exported.map((e) => [e.address, e]));
    for (const [address, canonical] of table.functions.get(exe.id) ?? []) {
      if (decodeAlias(canonical) || overlays.some((c) => containsVram(c, address))) continue;
      const entry = byAddress.get(address);
      if (!entry) stale.push({ file: ENGINE_EXPORT_PATH, line: 0, name: canonical, detail: `${hex(address)} is not exported` });
      else if (entry.name !== canonical) stale.push({ file: ENGINE_EXPORT_PATH, line: entry.line, name: entry.name, detail: `exported for ${canonical}` });
    }
  }

  const scan = (file: string, from: string): void => {
    const text = readFileSync(join(ROOT, file), "utf-8");
    const seen = new Set<string>();
    for (const match of text.matchAll(ALIAS_TOKEN)) {
      if (seen.has(match[0])) continue;
      seen.add(match[0]);
      const resolution = resolveAlias(match[0], from, table);
      if (resolution?.kind !== "obsolete") continue;
      const line = text.slice(0, match.index).split("\n").length;
      stale.push({ file, line, name: match[0], detail: `${resolution.container} names it ${resolution.canonical}` });
    }
  };
  const included = new Map<string, string[]>();
  for (const [path, text] of sources) {
    if (!path.endsWith(".c") || !text.includes("INCLUDE_")) continue;
    const files = included.get(sourceContainer(path)) ?? [];
    for (const site of analyzeCSource(text).includeAsm) files.push(`${site.folder}/${site.symbol}.s`);
    included.set(sourceContainer(path), files);
  }
  for (const container of table.containers) {
    const files = [
      container.paths.undefinedFuncs,
      container.paths.undefinedSyms,
      ...filesUnder(container.paths.asmDir, ".s").filter((file) => !file.includes("/nonmatchings/")),
      ...(included.get(container.id) ?? []),
    ];
    for (const file of files) if (existsSync(join(ROOT, file))) scan(file, container.id);
  }
  for (const file of ["include/functions.h", ...filesUnder("include/overlays", ".h")]) {
    if (existsSync(join(ROOT, file))) scan(file, file.match(/overlays\/([^/.]+)\.h$/)?.[1] ?? exe.id);
  }
  return stale;
}

/* ------------------------------------------------------------------ */
/* Source-policy allowlist                                             */
/* ------------------------------------------------------------------ */

const POLICY_CONFIG = ".pi/autoloop.json";

/**
 * Allowlist keys that still spell a renamed function's alias. A grant is
 * keyed by the function's (lowercased) name, so a rename must carry the key,
 * or the function's exception silently stops applying and every gate reads
 * the same construct as a new one.
 */
export function staleAllowlistKeys(table: NameTable): Array<{ key: string; canonical: string }> {
  const path = join(ROOT, POLICY_CONFIG);
  if (!existsSync(path)) return [];
  const config = JSON.parse(readFileSync(path, "utf-8")) as { sourcePolicy?: { allowlist?: Record<string, unknown> } };
  const exe = table.containers.find((c) => c.kind === "exe")?.id ?? "exe";
  const stale: Array<{ key: string; canonical: string }> = [];
  for (const key of Object.keys(config.sourcePolicy?.allowlist ?? {})) {
    const match = key.match(/^(?:(ovl_\d+)_)?func_([0-9a-f]{8})$/i);
    if (!match) continue;
    const alias = `${match[1] ? `${match[1]}_` : ""}func_${match[2]!.toUpperCase()}`;
    const resolution = resolveAlias(alias, match[1] ?? exe, table);
    if (resolution?.kind === "obsolete") stale.push({ key, canonical: resolution.canonical.toLowerCase() });
  }
  return stale;
}

/** Rename stale allowlist keys in place, leaving the file's layout as it is. */
function renameAllowlistKeys(stale: ReadonlyArray<{ key: string; canonical: string }>): void {
  if (stale.length === 0) return;
  const path = join(ROOT, POLICY_CONFIG);
  let text = readFileSync(path, "utf-8");
  const start = text.indexOf('"allowlist"');
  for (const { key, canonical } of stale) {
    const at = text.indexOf(`"${key}":`, start);
    if (at >= 0) text = `${text.slice(0, at)}"${canonical}":${text.slice(at + key.length + 3)}`;
  }
  writeFileSync(path, text);
}

/* ------------------------------------------------------------------ */
/* CLI                                                                 */
/* ------------------------------------------------------------------ */

export function activeSources(): Map<string, string> {
  const paths = [...configuredCSources(ROOT), ...handHeaders()];
  return new Map(paths.map((path) => [path, readFileSync(join(ROOT, path), "utf-8")]));
}

function describe(reference: AliasReference): string {
  const r = reference.resolution;
  if (r.kind === "obsolete") return `${reference.name} -> ${r.canonical} (${r.container} ${hex(r.address)})`;
  return `${reference.name}: ${r.kind === "ambiguous" ? r.reason : r.kind}`;
}

function main(): void {
  const args = process.argv.slice(2);
  const write = args.includes("--write");
  const check = args.includes("--check");

  const sources = activeSources();
  const table = loadNameTable(sources);

  const references = new Map<string, AliasReference[]>();
  for (const [path, text] of sources) {
    references.set(path, aliasReferences(text, sourceContainer(path), table, { header: path.endsWith(".h") }));
  }
  const relocations = planHeaderRelocations(sources, references);

  const rewritable: Array<[string, AliasReference]> = [];
  const outstanding: Array<[string, AliasReference]> = [];
  let filesWritten = 0;
  for (const [path, text] of sources) {
    const refs = references.get(path)!;
    for (const reference of refs) {
      if (reference.context === "code" && !reference.blocked && reference.resolution.kind === "obsolete") rewritable.push([path, reference]);
      else outstanding.push([path, reference]);
    }
    /* A header's moved declaration is deleted, not renamed. */
    const renames = path.endsWith(".h") ? refs.filter((r) => !r.via) : refs;
    const extra = relocations.get(path) ?? [];
    const changed = extra.length > 0 || renames.some((r) => r.context === "code" && !r.blocked && r.resolution.kind === "obsolete");
    if (write && changed) {
      writeFileSync(join(ROOT, path), rewriteAliases(text, renames, extra));
      filesWritten++;
    }
  }

  const stale = write ? [] : staleGeneratedNames(table, sources);
  const renames = new Map<string, number>();
  for (const [, reference] of rewritable) renames.set(describe(reference), (renames.get(describe(reference)) ?? 0) + 1);
  const grants = staleAllowlistKeys(table);
  if (write) renameAllowlistKeys(grants);

  if (write) {
    console.log(`Rewrote ${rewritable.length} reference(s) in ${filesWritten} file(s):`);
    for (const [rename, count] of [...renames].sort()) console.log(`  ${rename}  ×${count}`);
  } else if (rewritable.length > 0) {
    console.log(`Obsolete aliases in active sources: ${rewritable.length}`);
    for (const [path, reference] of rewritable) console.log(`  ${path}:${reference.line}  ${describe(reference)}`);
  }
  if (grants.length > 0) {
    console.log(`${write ? "Renamed" : "Obsolete"} allowlist key(s) in ${POLICY_CONFIG}: ${grants.length}`);
    for (const { key, canonical } of grants) console.log(`  ${key} -> ${canonical}`);
  }
  if (outstanding.length > 0) {
    console.log(`Outstanding — not rewritten automatically: ${outstanding.length}`);
    for (const [path, reference] of outstanding) {
      const why = reference.blocked ?? (reference.context !== "code" ? `${reference.context} reference` : "");
      console.log(`  ${path}:${reference.line}  ${describe(reference)}${why ? `  [${why}]` : ""}`);
    }
  }
  if (stale.length > 0) {
    console.log(`Stale generated names: ${stale.length} (regenerate: make split-all, then rebuild)`);
    for (const item of stale) console.log(`  ${item.file}${item.line ? `:${item.line}` : ""}  ${item.name}: ${item.detail}`);
  }

  const failed = !write && (rewritable.length > 0 || stale.length > 0 || grants.length > 0);
  const summary = `names: ${write ? "rewritten" : `${rewritable.length + grants.length} obsolete`}, ${outstanding.length} outstanding, ` +
    `${write ? "generated output not checked" : `${stale.length} stale generated`} (${sources.size} sources)`;
  console.log(failed || outstanding.length > 0 ? summary : `names: clean (${sources.size} sources)`);
  if (check && failed) process.exit(1);
}

if (process.argv[1] && resolve(process.argv[1]) === fileURLToPath(import.meta.url)) main();
