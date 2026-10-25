#!/usr/bin/env npx tsx
/**
 * interfaceAudit.ts — what each translation unit tells the compiler about the
 * functions it calls, against the evidence that can refute it.
 *
 * Every source declares its own callees, so one function can be described
 * differently in every file that calls it. Nothing fails when they disagree:
 * each object compiles against its own prototype, and a caller told `s32` where
 * the callee returns `u32` simply compiles different comparisons. This audit
 * finds those disagreements project-wide; `calleeTruth.ts` answers the same
 * question in depth for one function.
 *
 * Witnesses, as in the callee audit: the callee's matched definition in an
 * active source, or the vendored SDK header. `include/functions.h` is never one
 * — it is generated from the definitions, so it cannot corroborate them. A
 * callee with neither witness is an unknown contract and is listed as such; no
 * signature is invented for it.
 *
 * Types are compared by their ABI class — width and signedness of an integer,
 * pointer, floating, void — because that is what changes a caller's code. A
 * pointer to one struct against a pointer to another is not a disagreement
 * here; a typedef the audit cannot see through is undetermined, not a conflict.
 *
 * A function a hand-maintained header declares is published: a module's
 * interface. Published interfaces are the completed scope, and `--check`
 * fails when one disagrees with its definition or is restated by a unit that
 * includes its header.
 *
 * `--write` reconciles callers with a callee's matched definition. A unit that
 * includes the callee's module header loses its own declaration; any other
 * unit declares exactly the definition's prototype, gaining one if it called
 * the callee undeclared. Each call keeps meaning what it meant, through a cast
 * to the type the caller used to declare. Each unit is compiled before and
 * after: a cast survives only where removing it changes the unit's object, and
 * a unit whose object would change is left untouched and reported.
 *
 * Usage:
 *   npx tsx tools/diagnostics/interfaceAudit.ts                        # summary
 *   npx tsx tools/diagnostics/interfaceAudit.ts --callee Rand          # every site for one callee
 *   npx tsx tools/diagnostics/interfaceAudit.ts --detail               # every site for every callee
 *   npx tsx tools/diagnostics/interfaceAudit.ts --check                # exit 1 when a published interface fails
 *   npx tsx tools/diagnostics/interfaceAudit.ts --write --callee Rand  # reconcile Rand's callers with its definition,
 *                                                                      # flagged or not: publishes a new module header
 *   npx tsx tools/diagnostics/interfaceAudit.ts --write                # reconcile every published interface's callers
 */

import { execFileSync, spawnSync } from "node:child_process";
import { existsSync, readFileSync, writeFileSync } from "node:fs";
import { basename, dirname, join, resolve } from "node:path";
import { fileURLToPath } from "node:url";
import { ROOT } from "../lib/psxExeInfo.js";
import { prototypesIn, scopeFromPreprocessed, sdkPrototypes, type Prototype } from "../agent/calleeTruth.js";
import { applyCSourceEdits, walkActiveC, type CSourceEdit } from "../agent/cSourceGuard.js";
import { CPP_FLAGS, compileSource } from "../agent/decompToolchain.js";
import { preprocessingTools } from "../agent/preprocessedCache.js";
import { declaratorName, field, namedChildren, parseC, type Node } from "../agent/residual-source-search/tree-sitter-c.js";
import {
  COMMON_TYPES,
  activeSources,
  bindingAt,
  declarationRemovals,
  declarators,
  declaresFunction,
  handHeaders,
  misparsedDeclaration,
  sourceContainer,
} from "./canonicalNames.js";

/* ------------------------------------------------------------------ */
/* ABI classes                                                         */
/* ------------------------------------------------------------------ */

const SCALARS: Record<string, string> = {
  "int": "s32", "signed int": "s32", "signed": "s32", "long": "s32", "long int": "s32", "signed long": "s32",
  "s32": "s32", "vs32": "s32",
  "unsigned int": "u32", "unsigned": "u32", "unsigned long": "u32", "unsigned long int": "u32",
  "u32": "u32", "vu32": "u32", "u_long": "u32", "u_int": "u32",
  "short": "s16", "short int": "s16", "signed short": "s16", "s16": "s16", "vs16": "s16",
  "unsigned short": "u16", "unsigned short int": "u16", "u16": "u16", "vu16": "u16", "u_short": "u16",
  "signed char": "s8", "s8": "s8", "vs8": "s8",
  /* -funsigned-char: plain char is the unsigned byte. */
  "char": "u8", "unsigned char": "u8", "u8": "u8", "vu8": "u8", "u_char": "u8",
  "long long": "s64", "unsigned long long": "u64",
  "float": "float", "double": "double", "void": "void",
};

/**
 * What a declared type means to the calling convention: `s32`, `u16`, `ptr`,
 * `void`, `struct:Name`, or `named:T` for a typedef the audit cannot classify.
 */
export function abiClass(type: string): string {
  const text = type.replace(/\b(?:const|volatile|register|extern|static|inline|__inline__)\b/g, " ").replace(/\s+/g, " ").trim();
  if (/[*[(]/.test(text)) return "ptr";
  if (/^enum\b/.test(text)) return "s32";
  const record = text.match(/^(struct|union)\s+(\w+)$/);
  if (record) return `struct:${record[2]}`;
  return SCALARS[text] ?? `named:${text}`;
}

export interface Shape {
  result: string;
  /** Parameter classes, or `null` for an unprototyped `()` list. */
  parameters: string[] | null;
  variadic: boolean;
}

export function shapeOf(prototype: Prototype): Shape {
  return {
    result: abiClass(prototype.returnType ?? "int"),
    parameters: prototype.parameters === null ? null : (prototype.paramTypes ?? []).map(abiClass),
    variadic: prototype.variadic,
  };
}

/**
 * How two descriptions of one function differ: `[]` when they agree, a list of
 * differences when they provably do not, `null` when a difference rests on a
 * typedef this audit cannot see through.
 */
export function compareShapes(declared: Shape, witness: Shape): string[] | null {
  const differences: string[] = [];
  let undetermined = false;
  const compare = (what: string, a: string, b: string): void => {
    if (a === b) return;
    if (a.startsWith("named:") || b.startsWith("named:")) undetermined = true;
    else differences.push(`${what} ${a}≠${b}`);
  };
  compare("return", declared.result, witness.result);
  /* `()` claims nothing about parameters, and the SDK's C++-only `(...)`
     claims nothing about the fixed ones. */
  const open = (shape: Shape): boolean => shape.parameters === null || (shape.variadic && shape.parameters.length === 0);
  if (!open(declared) && !open(witness)) {
    if (declared.parameters!.length !== witness.parameters!.length) {
      differences.push(`arity ${declared.parameters!.length}≠${witness.parameters!.length}`);
    } else {
      declared.parameters!.forEach((p, i) => compare(`arg${i}`, p, witness.parameters![i]!));
    }
  }
  if (declared.parameters !== null && witness.parameters !== null && declared.variadic !== witness.variadic) {
    differences.push(declared.variadic ? "variadic≠fixed" : "fixed≠variadic");
  }
  if (differences.length > 0) return differences;
  return undetermined ? null : [];
}

/* ------------------------------------------------------------------ */
/* Translation units                                                   */
/* ------------------------------------------------------------------ */

export interface Declared {
  prototype: Prototype;
  line: number;
}

export interface CallSite {
  callee: string;
  line: number;
  arguments: number;
  /** No declaration of the callee in this file; a header may still supply one. */
  unbound: boolean;
}

export interface HeaderFacts {
  path: string;
  macros: Set<string>;
  functions: Set<string>;
  objects: Set<string>;
  types: Set<string>;
  /** struct/union/enum tags the header defines. */
  tags: Set<string>;
  includes: string[];
  prototypes: Prototype[];
}

export interface UnitFacts {
  headers: HeaderFacts[];
  macros: Set<string>;
  declarations: Declared[];
  /** Every function this unit defines, nested GNU definitions included. */
  definitions: Prototype[];
  /** C name -> the symbol an `__asm__("label")` declaration binds it to. */
  labels: Map<string, string>;
  calls: CallSite[];
  /** Identifiers used as values rather than called; the declared functions among them are callbacks. */
  values: Set<string>;
}

const headerCache = new Map<string, HeaderFacts>();

function resolveInclude(spec: string, quoted: boolean, from: string): string | undefined {
  const dirs = [...(quoted ? [dirname(from)] : []), "include", "include/psyq"];
  for (const dir of dirs) {
    const path = join(dir, spec);
    if (existsSync(join(ROOT, path))) return path;
  }
  return undefined;
}

function includesOf(root: Node, from: string): string[] {
  const found: string[] = [];
  walkActiveC(root, (node) => {
    if (node.type !== "preproc_include") return node.type !== "comment";
    const path = field(node, "path");
    if (path) {
      const resolved = resolveInclude(path.text.slice(1, -1), path.type === "string_literal", from);
      if (resolved) found.push(resolved);
    }
    return false;
  });
  return found;
}

/** Names a `typedef` introduces. */
function typedefNames(node: Node): string[] {
  return node.childrenForFieldName("declarator").flatMap((d) => {
    let current: Node | null | undefined = d;
    while (current && current.type !== "type_identifier" && current.type !== "identifier") {
      current = field(current, "declarator") ?? namedChildren(current)[0];
    }
    return current ? [current.text] : [];
  });
}

/** The tag a struct/union/enum specifier with a body defines. */
function definedTag(node: Node): string | undefined {
  if (!["struct_specifier", "union_specifier", "enum_specifier"].includes(node.type) || !field(node, "body")) return undefined;
  return field(node, "name")?.text;
}

/** What a header declares, defines as a macro, and includes. */
export function headerFacts(path: string): HeaderFacts {
  const cached = headerCache.get(path);
  if (cached) return cached;
  const text = readFileSync(join(ROOT, path), "utf-8");
  const tree = parseC(text);
  const facts: HeaderFacts = { path, macros: new Set(), functions: new Set(), objects: new Set(), types: new Set(), tags: new Set(), includes: [], prototypes: [] };
  try {
    walkActiveC(tree.rootNode, (node) => {
      if (node.type === "comment") return false;
      if (node.type === "preproc_def" || node.type === "preproc_function_def") {
        const name = field(node, "name")?.text;
        if (name) facts.macros.add(name);
        return false;
      }
      if (node.type === "type_definition") {
        for (const name of typedefNames(node)) facts.types.add(name);
        const tag = definedTag(field(node, "type") ?? node);
        if (tag) facts.tags.add(tag);
        return false;
      }
      if (node.type === "declaration") {
        const line = node.startPosition.row + 1;
        const prototypes = prototypesIn(node.text, path, () => ({ file: path, line }));
        facts.prototypes.push(...prototypes);
        const functions = new Set(prototypes.map((p) => p.name));
        for (const declarator of node.childrenForFieldName("declarator")) {
          const name = declaratorName(declarator ?? undefined)?.text;
          if (name) (functions.has(name) ? facts.functions : facts.objects).add(name);
        }
        return false;
      }
      if (node.type === "enumerator") {
        const name = field(node, "name")?.text;
        if (name) facts.objects.add(name);
      }
      const tag = definedTag(node);
      if (tag) facts.tags.add(tag);
      return true;
    });
    facts.includes = includesOf(tree.rootNode, path);
    headerCache.set(path, facts);
    return facts;
  } finally {
    tree.delete();
  }
}

/** Every header a unit reaches, through the headers' own includes. */
export function visibleHeaders(direct: readonly string[]): string[] {
  const seen = new Set<string>();
  const pending = [...direct];
  while (pending.length > 0) {
    const path = pending.shift()!;
    if (seen.has(path)) continue;
    seen.add(path);
    pending.push(...headerFacts(path).includes);
  }
  return [...seen];
}

function enclosingCall(node: Node): boolean {
  const parent = node.parent;
  return parent?.type === "call_expression" && field(parent, "function")?.id === node.id;
}

const ASM_LABEL = /__asm__\s*\(\s*"\s*([A-Za-z_][A-Za-z0-9_]*)\s*"\s*\)/;

/** Declarations, definitions, calls and values of one unit, from its own text. */
export function unitFacts(source: string, path: string): UnitFacts {
  const tree = parseC(source);
  const declarations: Declared[] = [];
  const definitions: Prototype[] = [];
  const labels = new Map<string, string>();
  const calls: CallSite[] = [];
  const values = new Set<string>();
  const macros = new Set<string>();
  let headers: HeaderFacts[] = [];
  try {
    headers = visibleHeaders(includesOf(tree.rootNode, path)).map(headerFacts);
    const types = new Set([...COMMON_TYPES, ...headers.flatMap((h) => [...h.types])]);
    walkActiveC(tree.rootNode, (node) => {
      if (node.type === "type_definition") for (const name of typedefNames(node)) types.add(name);
      return node.type !== "comment";
    });
    const isType = (name: string): boolean => types.has(name);

    walkActiveC(tree.rootNode, (node) => {
      if (node.type === "comment" || node.type === "preproc_arg" || node.type === "attribute_specifier") return false;
      if (node.type === "preproc_def" || node.type === "preproc_function_def") {
        const name = field(node, "name")?.text;
        if (name) macros.add(name);
        return false;
      }
      if (node.type === "function_definition") {
        const line = node.startPosition.row + 1;
        const [prototype] = prototypesIn(node.text, path, () => ({ file: path, line }));
        if (prototype?.kind === "definition") definitions.push(prototype);
        return true;
      }
      if (node.type === "declaration") {
        const line = node.startPosition.row + 1;
        for (const prototype of prototypesIn(node.text, path, () => ({ file: path, line }))) {
          if (prototype.kind !== "declaration") continue;
          declarations.push({ prototype, line });
          const label = prototype.signature.match(ASM_LABEL)?.[1];
          if (label) labels.set(prototype.name, label);
        }
        return true;
      }
      if (node.type === "expression_statement" && misparsedDeclaration(node, isType)) return false;
      if (node.type === "call_expression") {
        const callee = field(node, "function");
        if (callee?.type === "identifier" && !isType(callee.text)) {
          const binding = bindingAt(callee, callee.text, isType);
          if (binding !== "object") {
            const args = field(node, "arguments");
            calls.push({
              callee: callee.text,
              line: node.startPosition.row + 1,
              arguments: args ? namedChildren(args).filter((n) => n.type !== "comment").length : 0,
              unbound: binding === null,
            });
          }
        }
        return true;
      }
      if (node.type === "identifier" && !enclosingCall(node)) {
        const parent = node.parent?.type ?? "";
        if (!/declarator$/.test(parent) && parent !== "parameter_declaration") values.add(node.text);
      }
      return true;
    });
  } finally {
    tree.delete();
  }
  return { headers, macros, declarations, definitions, labels, calls, values };
}

/* ------------------------------------------------------------------ */
/* Project audit                                                       */
/* ------------------------------------------------------------------ */

export type FindingKind = "conflict" | "undetermined" | "arity" | "implicit" | "restated" | "unknown";

export interface Finding {
  kind: FindingKind;
  callee: string;
  file: string;
  line: number;
  declared?: string;
  witness?: string;
  differences?: string[];
  callback?: boolean;
}

export interface Witnessed {
  prototype: Prototype;
  source: "definition" | "sdk";
}

/** A function a hand-maintained header declares: a module's interface. */
export interface Published {
  prototype: Prototype;
  header: string;
}

export interface AuditInput {
  sources: ReadonlyMap<string, string>;
  sdk: ReadonlyMap<string, Prototype>;
  published: ReadonlyMap<string, Published>;
}

export interface AuditResult {
  findings: Finding[];
  /** Every finding about a published interface, for --check. */
  publishedFailures: Finding[];
  units: number;
}

export function auditInterfaces(input: AuditInput): AuditResult {
  const units = new Map<string, UnitFacts>();
  const definitions = new Map<string, Map<string, Prototype>>();
  const define = (name: string, container: string, definition: Prototype): void => {
    const byContainer = definitions.get(name) ?? new Map<string, Prototype>();
    byContainer.set(container, definition);
    definitions.set(name, byContainer);
  };
  for (const [path, text] of input.sources) {
    if (!path.endsWith(".c")) continue;
    const facts = unitFacts(text, path);
    units.set(path, facts);
    const container = sourceContainer(path);
    for (const definition of facts.definitions) {
      define(definition.name, container, definition);
      /* A nested definition is the symbol its label declaration names. */
      const label = facts.labels.get(definition.name);
      if (label) define(label, container, definition);
    }
  }

  /* The callee a unit means: its own container's definition, else the engine's,
     else the one other container that defines the name. */
  const witnessFor = (name: string, container: string): Witnessed | undefined => {
    const defined = definitions.get(name);
    if (defined) {
      const chosen = defined.get(container) ?? defined.get("exe") ?? (defined.size === 1 ? [...defined.values()][0] : undefined);
      if (chosen) return { prototype: chosen, source: "definition" };
    }
    const sdk = input.sdk.get(name);
    return sdk ? { prototype: sdk, source: "sdk" } : undefined;
  };
  const render = (p: Prototype): string => p.signature.replace(/\s+\(/, "(").replace(/;$/, "");

  const findings: Finding[] = [];
  const unknown = new Map<string, Finding>();
  for (const [path, facts] of units) {
    const container = sourceContainer(path);
    const ownDefinitions = new Set(facts.definitions.map((d) => d.name));
    const witnessIn = (name: string): Witnessed | undefined => witnessFor(facts.labels.get(name) ?? name, container);
    const declaredHere = new Map<string, Declared>();
    for (const declared of facts.declarations) declaredHere.set(declared.prototype.name, declared);
    const headerMacros = new Set(facts.headers.flatMap((h) => [...h.macros]));
    const headerFunctions = new Set(facts.headers.flatMap((h) => [...h.functions]));
    const headerObjects = new Set(facts.headers.flatMap((h) => [...h.objects]));

    for (const { prototype, line } of facts.declarations) {
      if (ownDefinitions.has(prototype.name)) continue;
      const published = input.published.get(prototype.name);
      if (published && facts.headers.some((h) => h.path === published.header)) {
        findings.push({ kind: "restated", callee: prototype.name, file: path, line, declared: render(prototype), witness: published.header });
      }
      const witness = witnessIn(prototype.name);
      if (!witness) {
        if (!unknown.has(prototype.name)) unknown.set(prototype.name, { kind: "unknown", callee: prototype.name, file: path, line, declared: render(prototype) });
        continue;
      }
      const differences = compareShapes(shapeOf(prototype), shapeOf(witness.prototype));
      if (differences !== null && differences.length === 0) continue;
      findings.push({
        kind: differences === null ? "undetermined" : "conflict",
        callee: prototype.name, file: path, line,
        declared: render(prototype),
        witness: `${render(witness.prototype)} (${witness.prototype.where}:${witness.prototype.line})`,
        ...(differences ? { differences } : {}),
        ...(facts.values.has(prototype.name) ? { callback: true } : {}),
      });
    }

    for (const call of facts.calls) {
      if (ownDefinitions.has(call.callee) || facts.macros.has(call.callee) || headerMacros.has(call.callee)) continue;
      if (call.callee.startsWith("__builtin_")) continue;
      /* A call through a pointer object a header declares names no function. */
      if (call.unbound && headerObjects.has(call.callee) && !headerFunctions.has(call.callee)) continue;
      const implicit = call.unbound && !headerFunctions.has(call.callee);
      const witness = witnessIn(call.callee);
      if (!witness && !unknown.has(call.callee)) unknown.set(call.callee, { kind: "unknown", callee: call.callee, file: path, line: call.line });
      if (implicit) {
        findings.push({
          kind: "implicit", callee: call.callee, file: path, line: call.line,
          ...(witness ? { witness: `${render(witness.prototype)} (${witness.prototype.where}:${witness.prototype.line})` } : {}),
        });
      }
      const declared = declaredHere.get(call.callee)?.prototype;
      const prototyped = declared ? declared.parameters !== null : !implicit;
      if (prototyped || !witness || witness.prototype.parameters === null) continue;
      const expected = witness.prototype.parameters;
      if (witness.prototype.variadic ? call.arguments < expected : call.arguments !== expected) {
        findings.push({
          kind: "arity", callee: call.callee, file: path, line: call.line,
          differences: [`${call.arguments} argument(s), ${witness.prototype.variadic ? "at least " : ""}${expected} expected`],
          witness: `${render(witness.prototype)} (${witness.prototype.where}:${witness.prototype.line})`,
        });
      }
    }
  }

  /* A module header must itself agree with the definition it publishes. */
  for (const [name, { prototype, header }] of input.published) {
    const witness = witnessFor(name, "exe");
    if (!witness) continue;
    const differences = compareShapes(shapeOf(prototype), shapeOf(witness.prototype));
    if (differences === null || differences.length > 0) {
      findings.push({
        kind: differences === null ? "undetermined" : "conflict", callee: name, file: header, line: prototype.line,
        declared: render(prototype), witness: `${render(witness.prototype)} (${witness.prototype.where}:${witness.prototype.line})`,
        ...(differences ? { differences } : {}),
      });
    }
  }

  findings.push(...unknown.values());
  const publishedFailures = findings.filter((f) => input.published.has(f.callee));
  return { findings, publishedFailures, units: units.size };
}

/** Functions an active source defines, as hand-maintained headers declare them. */
export function publishedInterfaces(sources: ReadonlyMap<string, string>): Map<string, Published> {
  const defined = new Set<string>();
  for (const [path, text] of sources) {
    if (path.endsWith(".c")) for (const p of prototypesIn(text, path)) if (p.kind === "definition") defined.add(p.name);
  }
  const result = new Map<string, Published>();
  for (const header of handHeaders()) {
    for (const prototype of headerFacts(header).prototypes) {
      if (defined.has(prototype.name) && !result.has(prototype.name)) result.set(prototype.name, { prototype, header });
    }
  }
  return result;
}

/* ------------------------------------------------------------------ */
/* Propagation                                                         */
/* ------------------------------------------------------------------ */

const WORK_DIR = "build/interface-audit";

/** What a reconciliation carries to the callers of one function. */
export interface Contract {
  name: string;
  source: "definition" | "sdk";
  prototype: Prototype;
  /** The definition's own declarator, as a declaration. */
  text: string;
  /** Parameter declarations as the witness writes them; `null` when unprototyped. */
  parameters: string[] | null;
  /**
   * The header that declares it — a module header or the vendored SDK header —
   * its lines there, and the headers the project's own sources include before
   * it (the PSY-Q headers are not self-contained).
   */
  published?: { header: string; lines: Set<number>; prerequisites: string[] };
}

/** The function declarator a declaration introduces `name` with. */
function functionDeclarator(declaration: Node, name: string): Node | undefined {
  for (const declarator of declarators(declaration)) {
    if (declaratorName(declarator)?.text !== name || !declaresFunction(declarator)) continue;
    let node: Node | undefined = declaratorName(declarator)!.parent ?? undefined;
    while (node && node.type !== "function_declarator") node = node.parent ?? undefined;
    return node;
  }
  return undefined;
}

/** Parameter declarations as written: `[]` for `(void)`, `null` for `()`. Variadic `...` excluded. */
function parameterTexts(declarator: Node): string[] | null {
  const list = field(declarator, "parameters");
  const params = list ? namedChildren(list).filter((n) => n.type === "parameter_declaration") : [];
  const variadic = list ? namedChildren(list).some((n) => n.type === "variadic_parameter") || /\.\.\./.test(list.text) : false;
  if (params.length === 0) return variadic ? [] : null;
  if (params.length === 1 && params[0]!.text.trim() === "void") return [];
  return params.map((p) => p.text.replace(/\s+/g, " ").trim());
}

/** Where a declaration of `name` sits in a header, if it has one. */
function declarationIn(header: string, name: string): { node: { text: string; parameters: string[] | null }; lines: Set<number> } | undefined {
  const tree = parseC(readFileSync(join(ROOT, header), "utf-8"));
  try {
    let found: { node: { text: string; parameters: string[] | null }; lines: Set<number> } | undefined;
    walkActiveC(tree.rootNode, (node) => {
      if (found || node.type === "comment" || node.type === "function_definition") return false;
      if (node.type !== "declaration") return true;
      const declarator = functionDeclarator(node, name);
      if (declarator) {
        const lines = new Set<number>();
        for (let line = node.startPosition.row + 1; line <= node.endPosition.row + 1; line++) lines.add(line);
        found = { node: { text: node.text, parameters: parameterTexts(declarator) }, lines };
      }
      return false;
    });
    return found;
  } finally {
    tree.delete();
  }
}

/**
 * The headers the project's own sources include before `header`, by their most
 * common ordering. Read from top-level `#include` directives, so the answer is
 * the project's established practice rather than a guess at what the header
 * needs.
 */
function prerequisitesOf(header: string, sources: ReadonlyMap<string, string>): string[] {
  const spec = header.replace(/^include\//, "");
  const orderings = new Map<string, number>();
  for (const [path, text] of sources) {
    if (!path.endsWith(".c") || !text.includes(spec)) continue;
    const tree = parseC(text);
    try {
      const includes = namedChildren(tree.rootNode).filter((n) => n.type === "preproc_include")
        .map((n) => field(n, "path")?.text.slice(1, -1) ?? "");
      const at = includes.indexOf(spec);
      if (at < 0) continue;
      const before = includes.slice(0, at).filter((i) => i.startsWith("psyq/")).map((i) => `include/${i}`).join("\n");
      orderings.set(before, (orderings.get(before) ?? 0) + 1);
    } finally {
      tree.delete();
    }
  }
  const best = [...orderings].sort((a, b) => b[1] - a[1] || a[0].localeCompare(b[0]))[0]?.[0];
  return best ? best.split("\n") : [];
}

const TYPE_KEYWORDS = new Set(["const", "volatile", "unsigned", "signed", "int", "char", "short", "long", "void", "float", "double",
  "struct", "union", "enum", "register", "extern", "static", "inline", "__inline__"]);

/** Spells a type using only names one unit can see; `undefined` when it cannot. */
export type Speller = (type: string) => string | undefined;

/**
 * A type exactly as written when the unit can see every name in it. Otherwise
 * the same calling convention in the unit's own words: a scalar typedef as its
 * project type (`u_long` → `u32`), a pointer to an unseen type as a pointer to
 * `void`. Function pointer types the unit cannot see are not spelled.
 */
export function spellerFor(types: ReadonlySet<string>, tags: ReadonlySet<string>): Speller {
  return (type) => {
    const tag = type.match(/\b(?:struct|union|enum)\s+([A-Za-z_]\w*)/)?.[1];
    const names = [...type.matchAll(/[A-Za-z_]\w*/g)].map((m) => m[0]).filter((n) => !TYPE_KEYWORDS.has(n) && n !== tag);
    if ((tag === undefined || tags.has(tag)) && names.every((n) => types.has(n))) return type;
    if (type.includes("(")) return undefined;
    const stars = (type.match(/\*/g) ?? []).length;
    const base = abiClass(type.replace(/\*/g, " "));
    if (/^[su](?:8|16|32)$/.test(base)) return stars > 0 ? `${base} ${"*".repeat(stars)}` : base;
    return stars > 0 ? `void ${"*".repeat(stars)}` : undefined;
  };
}

/**
 * The speller for one point in a unit: common.h's scalars, its visible
 * headers' typedefs and tags, and its own — those it defines before `before`.
 */
function unitSpeller(root: Node, visible: readonly string[], before = Infinity): Speller {
  const types = new Set([...COMMON_TYPES, ...visible.flatMap((h) => [...headerFacts(h).types])]);
  const tags = new Set(visible.flatMap((h) => [...headerFacts(h).tags]));
  walkActiveC(root, (node) => {
    if (node.startIndex >= before) return false;
    if (node.type === "type_definition" && node.endIndex <= before) for (const name of typedefNames(node)) types.add(name);
    const tag = node.endIndex <= before ? definedTag(node) : undefined;
    if (tag) tags.add(tag);
    return node.type !== "comment";
  });
  return spellerFor(types, tags);
}

/**
 * The contract for `name`: its one matched definition, or else its vendored SDK
 * declaration; and the header that publishes it, if one does.
 */
export function contractFor(
  name: string,
  sources: ReadonlyMap<string, string>,
  sdk: ReadonlyMap<string, Prototype> = new Map()
): Contract | string {
  const found: Contract[] = [];
  for (const [path, text] of sources) {
    if (!path.endsWith(".c") || !text.includes(name)) continue;
    const tree = parseC(text);
    try {
      for (const node of namedChildren(tree.rootNode)) {
        if (node.type !== "function_definition" || declaratorName(field(node, "declarator"))?.text !== name) continue;
        const body = field(node, "body")!;
        const head = text.slice(node.startIndex, body.startIndex).trim();
        if (/^static\b/.test(head)) return `${name} is static to ${path}`;
        if (text.slice(field(node, "declarator")!.endIndex, body.startIndex).trim() !== "") return `${name} has an old-style parameter list`;
        const declaration = `${head};`;
        const parsed = parseC(declaration);
        try {
          const decl = namedChildren(parsed.rootNode).find((n) => n.type === "declaration");
          const declarator = decl ? functionDeclarator(decl, name) : undefined;
          const [prototype] = prototypesIn(declaration, path, () => ({ file: path, line: node.startPosition.row + 1 }));
          if (prototype && declarator) found.push({ name, source: "definition", prototype, text: declaration, parameters: parameterTexts(declarator) });
        } finally {
          parsed.delete();
        }
      }
    } finally {
      tree.delete();
    }
  }
  if (found.length > 1) return `${found.length} active sources define ${name}`;
  if (found.length === 1) {
    for (const header of handHeaders()) {
      const declared = declarationIn(header, name);
      if (declared) {
        found[0]!.published = { header, lines: declared.lines, prerequisites: prerequisitesOf(header, sources) };
        break;
      }
    }
    return found[0]!;
  }
  const vendor = sdk.get(name);
  if (!vendor) return `${name} has no matched definition and no SDK declaration`;
  const declared = declarationIn(vendor.where, name);
  if (!declared) return `${vendor.where} does not declare ${name} where the audit can read it`;
  return {
    name, source: "sdk", prototype: vendor, text: declared.node.text, parameters: declared.node.parameters,
    published: { header: vendor.where, lines: declared.lines, prerequisites: prerequisitesOf(vendor.where, sources) },
  };
}

/**
 * A caller's declaration brought to the contract. Only what the calling
 * convention reads changes: a parameter or result whose ABI class already
 * agrees keeps the caller's own spelling, so a caller is never handed a type it
 * cannot see merely because the definition names it differently.
 */
export function reconciledDeclaration(
  contract: Contract,
  old?: { prototype: Prototype; parameters: string[] | null },
  spell: Speller = (type) => type,
  as = contract.name
): string | undefined {
  const theirs = contract.prototype.paramTypes ?? [];
  const mine = old?.prototype.paramTypes ?? [];
  const contractReturn = contract.prototype.returnType ?? "int";
  const result = old && abiClass(old.prototype.returnType ?? "int") === abiClass(contractReturn)
    ? old.prototype.returnType ?? "int"
    : spell(contractReturn);
  if (result === undefined) return undefined;
  let parameters: string[] | null = null;
  if (contract.parameters) {
    const aligned = old?.parameters && old.parameters.length === contract.parameters.length;
    parameters = [];
    for (const [i, text] of contract.parameters.entries()) {
      if (aligned && abiClass(mine[i] ?? "") === abiClass(theirs[i] ?? "")) {
        parameters.push(old!.parameters![i]!);
        continue;
      }
      const spelled = spell(theirs[i] ?? "");
      if (spelled === undefined) return undefined;
      parameters.push(spelled === theirs[i] ? text : spelled);
    }
  }
  const list = parameters === null ? "" : [...(parameters.length === 0 && !contract.prototype.variadic ? ["void"] : parameters),
    ...(contract.prototype.variadic ? ["..."] : [])].join(", ");
  return `${result.endsWith("*") ? result : `${result} `}${as}(${list});`;
}

/**
 * The unit as cpp delivers it. With `without`, the header's lines for that
 * contract are blanked: the unit as it compiled before the header published
 * the declaration, everything else in place.
 */
function preprocess(text: string, without?: Contract["published"]): string {
  const result = spawnSync(preprocessingTools().tools.cpp, [...CPP_FLAGS, "-"], {
    cwd: ROOT, input: text, encoding: "utf-8", maxBuffer: 64 * 1024 * 1024,
  });
  if (result.status !== 0) throw new Error(`cpp failed: ${result.stderr}`);
  if (!without) return result.stdout;
  const { lineOf } = scopeFromPreprocessed(result.stdout);
  return result.stdout.split("\n").map((line, row) => {
    if (line.startsWith("#")) return line;
    const origin = lineOf(row);
    return origin.file === without.header && without.lines.has(origin.line) ? "" : line;
  }).join("\n");
}

/** Every section's bytes and every relocation of the unit's object, or the compile error. */
function objectSignature(text: string, path: string, without?: Contract["published"]): { signature?: string; error?: string } {
  const container = sourceContainer(path);
  const stem = basename(path, ".c");
  try {
    const artifacts = compileSource(join(ROOT, path), join(ROOT, WORK_DIR, container), stem, {
      assemble: true, preprocessedText: preprocess(text, without), containerKind: container === "exe" ? "exe" : "overlay",
    });
    const dump = execFileSync("mips-linux-gnu-objdump", ["-s", "-r", artifacts.object!], { encoding: "utf-8", maxBuffer: 64 * 1024 * 1024 });
    return { signature: dump.split("\n").filter((line) => !line.includes(artifacts.object!)).join("\n") };
  } catch (error) {
    /* The first diagnostic, not the "In function"/"In file included from" context lines. */
    const lines = (error as Error).message.split("\n").map((l) => l.replace(/^.*cc1 failed: /, "").trim());
    const diagnostic = lines.find((l) => l && !/In function|In file included from|^from /.test(l));
    return { error: diagnostic?.replace(/^<stdin>:/, "line ") ?? "compile failed" };
  }
}

/** Arguments of a call, comments excluded. */
function callArguments(call: Node): Node[] {
  const args = field(call, "arguments");
  return args ? namedChildren(args).filter((n) => n.type !== "comment") : [];
}

/** `#include` lines for the header and its missing prerequisites, after the unit's last top-level include. */
function includeInsertion(root: Node, published: NonNullable<Contract["published"]>, visible: readonly string[]): CSourceEdit | undefined {
  const headers = [...published.prerequisites.filter((h) => !visible.includes(h)), published.header];
  if (headers.some((h) => !h.startsWith("include/"))) return undefined;
  const lines = headers.map((h) => `#include "${h.slice("include/".length)}"\n`).join("");
  const includes = namedChildren(root).filter((n) => n.type === "preproc_include");
  const at = includes.length > 0 ? includes[includes.length - 1]!.endIndex : 0;
  return { start: at, end: at, text: includes.length > 0 ? lines : `${lines}\n` };
}

const PRIMARY = new Set(["identifier", "number_literal", "call_expression", "parenthesized_expression", "field_expression", "subscript_expression"]);

export interface ReconcileResult {
  file: string;
  callee: string;
  outcome: "written" | "refused";
  /** What the unit now relies on: its corrected or new declaration, or the header it includes. */
  change: "replaced" | "added" | "included" | "removed";
  /** Casts the unit's object requires, as `line: (type)`. */
  casts: string[];
  /** The unit's own declaration before reconciliation, when it had one. */
  declared?: Prototype;
  detail?: string;
}

/**
 * Reconcile every unit that declares or calls the contract's function.
 *
 * A unit that sees a header declaring the callee drops its own copies. A unit
 * that does not, where a header publishes the callee, includes that header
 * instead. Any other unit declares the reconciled prototype, gaining one if it
 * called the callee undeclared. Pure over `sources`: the new text of every
 * changed unit is returned.
 */
type Cast = { line: number; type: string; edits: CSourceEdit[] };

/**
 * Casts that keep each call meaning what its old binding gave it. An undeclared
 * call was C89 implicit int; a discarded result, or a void callee, needs no
 * cast. An argument keeps the value its old parameter type gave it; between
 * word-sized classes (int and pointer) the conversion is a reinterpretation, so
 * the argument is cast to the new type instead, which also keeps the compiler
 * from warning about an implicit int/pointer conversion. With no old parameter
 * types, an argument whose parameter is now a pointer is cast to it.
 */
function castsFor(calls: readonly Node[], old: Prototype | undefined, implicit: boolean, target: Prototype, spell: Speller): Cast[] {
  const casts: Cast[] = [];
  const shape = shapeOf(target);
  const oldResult = old ? old.returnType ?? "int" : implicit ? "s32" : undefined;
  if (oldResult && shape.result !== "void" && abiClass(oldResult) !== shape.result && abiClass(oldResult) !== "void" && !oldResult.includes("(")) {
    for (const call of calls) {
      if (call.parent?.type === "expression_statement") continue;
      casts.push({ line: call.startPosition.row + 1, type: oldResult, edits: [{ start: call.startIndex, end: call.startIndex, text: `(${oldResult})` }] });
    }
  }
  const word = (c: string): boolean => c === "s32" || c === "u32" || c === "ptr";
  const wrap = (arg: Node, type: string): Cast => ({
    line: arg.startPosition.row + 1, type,
    edits: PRIMARY.has(arg.type)
      ? [{ start: arg.startIndex, end: arg.startIndex, text: `(${type})` }]
      : [{ start: arg.startIndex, end: arg.startIndex, text: `(${type})(` }, { start: arg.endIndex, end: arg.endIndex, text: ")" }],
  });
  if (!shape.parameters) return casts;
  const oldParameters = old && old.parameters !== null ? old.paramTypes : undefined;
  shape.parameters.forEach((targetClass, index) => {
    const oldType = oldParameters?.[index];
    let cast: string | undefined;
    if (oldParameters && oldParameters.length === shape.parameters!.length) {
      if (!oldType || abiClass(oldType) === targetClass || oldType.includes("(")) return;
      cast = word(abiClass(oldType)) && word(targetClass) ? spell(target.paramTypes?.[index] ?? "") : oldType;
    } else if (!oldParameters && targetClass === "ptr") {
      cast = spell(target.paramTypes?.[index] ?? "");
    }
    if (!cast) return;
    for (const call of calls) {
      const arg = callArguments(call)[index];
      if (arg) casts.push(wrap(arg, cast));
    }
  });
  return casts;
}

/** The `__asm__("label")` a declaration binds its declarator to, if any. */
function asmLabel(declaration: Node): string | undefined {
  return declaration.text.match(/__asm__\s*\(\s*"\s*([A-Za-z_][A-Za-z0-9_]*)\s*"\s*\)/)?.[1];
}

/** Where a declaration the unit gains goes: before its first function, and that function's leading comments. */
function beforeFirstFunction(root: Node, text: string, declaration: string): CSourceEdit {
  const first = namedChildren(root).find((n) => n.type === "function_definition");
  if (!first) return { start: text.length, end: text.length, text: `\n${declaration}\n` };
  let at: Node = first;
  while (at.previousNamedSibling?.type === "comment" && at.previousNamedSibling.endPosition.row + 1 >= at.startPosition.row) at = at.previousNamedSibling;
  const start = text.lastIndexOf("\n", at.startIndex - 1) + 1;
  return { start, end: start, text: `${declaration}\n\n` };
}

/**
 * Reconcile every unit that declares or calls the contract's function.
 *
 * A unit that sees a header declaring the callee drops its own copies. A unit
 * that does not, where a header publishes the callee, includes that header
 * instead — and gives up its own copies of anything else that header declares,
 * which would otherwise collide with it. Any other unit declares the reconciled
 * prototype, gaining one if it called the callee undeclared; a declaration that
 * binds a local name to the callee through an `__asm__` label keeps its name and
 * label. Pure over `sources`: the new text of every changed unit is returned.
 */
export function reconcileCallers(
  sources: ReadonlyMap<string, string>,
  contract: Contract
): { results: ReconcileResult[]; texts: Map<string, string> } {
  const { name } = contract;
  const texts = new Map<string, string>();
  const results: ReconcileResult[] = [];
  const shape = shapeOf(contract.prototype);

  for (const path of [...sources.keys()].filter((p) => p.endsWith(".c")).sort()) {
    const text = sources.get(path)!;
    if (!text.includes(name)) continue;
    const tree = parseC(text);
    let edits: CSourceEdit[] = [];
    let change: ReconcileResult["change"] = "replaced";
    let casts: Cast[] = [];
    let refusal: string | undefined;
    let blank: Contract["published"];
    const result0: { declared?: Prototype } = {};
    try {
      /* The local names bound to the callee: its own, and any an asm label binds. */
      const labelled = new Map<string, string>();
      let defines = false;
      walkActiveC(tree.rootNode, (node) => {
        if (node.type === "comment" || node.type === "preproc_arg") return false;
        if (node.type === "function_definition" && declaratorName(field(node, "declarator"))?.text === name) defines = true;
        if (node.type === "declaration" && asmLabel(node) === name) {
          for (const d of declarators(node)) {
            const local = declaratorName(d)?.text;
            if (local && local !== name && declaresFunction(d)) labelled.set(local, name);
          }
        }
        return true;
      });
      if (defines) continue;
      const bound = new Set([name, ...labelled.keys()]);
      const declarations: Array<{ node: Node; local: string }> = [];
      const calls: Node[] = [];
      walkActiveC(tree.rootNode, (node) => {
        if (node.type === "comment" || node.type === "preproc_arg") return false;
        if (node.type === "declaration") {
          for (const local of bound) if (functionDeclarator(node, local)) declarations.push({ node, local });
        }
        if (node.type === "call_expression") {
          const callee = field(node, "function");
          if (callee?.type === "identifier" && bound.has(callee.text) && bindingAt(callee, callee.text) !== "object") calls.push(node);
        }
        return true;
      });
      if (declarations.length === 0 && calls.length === 0) continue;

      const visible = visibleHeaders(includesOf(tree.rootNode, path));
      const headerDeclares = visible.some((h) => headerFacts(h).functions.has(name));
      const olds = declarations.map(({ node, local }) => ({
        prototype: prototypesIn(node.text, path).find((p) => p.name === local)!,
        parameters: parameterTexts(functionDeclarator(node, local)!),
      }));
      const shapes = new Set(olds.map((o) => JSON.stringify(shapeOf(o.prototype))));
      if (declarations.some(({ node }) => declarators(node).length !== 1)) refusal = `declares ${name} together with other names`;
      else if (shapes.size > 1) refusal = `declares ${name} with ${shapes.size} different prototypes`;
      const old = olds[0];
      if (old) result0.declared = old.prototype;
      const agrees = (o: (typeof olds)[number]): boolean => {
        const differences = compareShapes(shapeOf(o.prototype), shape);
        return differences !== null && differences.length === 0;
      };
      const plain = declarations.filter(({ local }) => local === name).map(({ node }) => node);

      if (labelled.size > 0) {
        if (olds.every(agrees)) continue;
        const rendered = declarations.map(({ node, local }, i) =>
          reconciledDeclaration(contract, olds[i], unitSpeller(tree.rootNode, visible, node.startIndex), local));
        if (rendered.some((r) => r === undefined)) refusal = `the unit cannot spell ${contract.text}`;
        else {
          edits = declarations.map(({ node, local }, i) => {
            const declarator = functionDeclarator(node, local)!;
            const prefix = node.text.match(/^(?:(?:auto|extern|static)\s+)*/)?.[0] ?? "";
            const suffix = text.slice(declarator.endIndex, node.endIndex).replace(/;\s*$/, "");
            return { start: node.startIndex, end: node.endIndex, text: `${prefix}${rendered[i]!.replace(/;$/, "")}${suffix};` };
          });
        }
        change = "replaced";
        casts = castsFor(calls, old?.prototype, false, contract.prototype, unitSpeller(tree.rootNode, visible));
      } else if (headerDeclares) {
        if (plain.length === 0) continue;
        edits = declarationRemovals(text, plain);
        change = "removed";
        if (contract.published && visible.includes(contract.published.header)) blank = contract.published;
        casts = castsFor(calls, old?.prototype, false, contract.prototype, unitSpeller(tree.rootNode, visible));
      } else if (contract.published) {
        const include = includeInsertion(tree.rootNode, contract.published, visible);
        const spell = unitSpeller(tree.rootNode, visible);
        if (!include) refusal = `${contract.published.header} is not on the include path`;
        else {
          /* Everything the newly included headers declare that the unit
             declares itself goes too, each call keeping its old meaning. */
          const gained = [...contract.published.prerequisites.filter((h) => !visible.includes(h)), contract.published.header]
            .flatMap((h) => visibleHeaders([h])).filter((h) => !visible.includes(h));
          const published = new Map(gained.flatMap((h) => headerFacts(h).prototypes.map((p) => [p.name, p] as const)));
          const others: Node[] = [];
          walkActiveC(tree.rootNode, (node) => {
            if (node.type === "comment" || node.type === "preproc_arg" || node.type === "function_definition") return node.type === "function_definition";
            if (node.type !== "declaration") return true;
            for (const other of published.keys()) {
              if (other === name || !functionDeclarator(node, other)) continue;
              if (declarators(node).length !== 1) refusal = `declares ${other} together with other names`;
              others.push(node);
              const prior = prototypesIn(node.text, path).find((p) => p.name === other);
              const otherCalls: Node[] = [];
              walkActiveC(tree.rootNode, (call) => {
                if (call.type !== "call_expression") return call.type !== "comment";
                const callee = field(call, "function");
                if (callee?.type === "identifier" && callee.text === other && bindingAt(callee, other) !== "object") otherCalls.push(call);
                return true;
              });
              casts.push(...castsFor(otherCalls, prior, false, published.get(other)!, spell));
            }
            return false;
          });
          edits = [include, ...declarationRemovals(text, [...plain, ...others])];
        }
        change = "included";
        casts.push(...castsFor(calls, old?.prototype, !old, contract.prototype, spell));
      } else if (declarations.length === 0) {
        const insertion = beforeFirstFunction(tree.rootNode, text, "");
        const spell = unitSpeller(tree.rootNode, visible, insertion.start);
        const declaration = reconciledDeclaration(contract, undefined, spell);
        if (declaration === undefined) refusal = `the unit cannot spell ${contract.text}`;
        else edits = [beforeFirstFunction(tree.rootNode, text, declaration)];
        change = "added";
        casts = castsFor(calls, undefined, true, contract.prototype, spell);
      } else {
        if (olds.every(agrees)) continue;
        const rendered = plain.map((node, i) => reconciledDeclaration(contract, olds[i], unitSpeller(tree.rootNode, visible, node.startIndex)));
        if (rendered.some((r) => r === undefined)) refusal = `the unit cannot spell ${contract.text}`;
        else edits = plain.map((d, i) => ({ start: d.startIndex, end: d.endIndex, text: `${/^extern\b/.test(d.text) ? "extern " : ""}${rendered[i]}` }));
        change = "replaced";
        casts = castsFor(calls, old?.prototype, false, contract.prototype, unitSpeller(tree.rootNode, visible));
      }
    } finally {
      tree.delete();
    }

    const result: ReconcileResult = { file: path, callee: name, outcome: "refused", change, casts: [], ...result0 };
    results.push(result);
    if (refusal) {
      result.detail = refusal;
      continue;
    }
    let render: (kept: Cast[]) => string;
    try {
      render = (kept) => applyCSourceEdits(text, [...edits, ...kept.flatMap((c) => c.edits)]);
      render(casts);
    } catch (error) {
      result.detail = (error as Error).message;
      continue;
    }
    const baseline = objectSignature(text, path, blank);
    if (!baseline.signature) {
      result.detail = `does not compile as it stands: ${baseline.error}`;
      continue;
    }
    const attempt = objectSignature(render(casts), path);
    if (attempt.signature !== baseline.signature) {
      result.detail = attempt.signature ? "the object changes even with every cast in place" : `does not compile reconciled: ${attempt.error}`;
      continue;
    }
    let kept = casts;
    for (const cast of casts) {
      const without = kept.filter((c) => c !== cast);
      if (objectSignature(render(without), path).signature === baseline.signature) kept = without;
    }
    texts.set(path, render(kept));
    result.outcome = "written";
    result.casts = kept.map((c) => `${c.line}: (${c.type})`);
  }
  return { results, texts };
}

/**
 * The refused callers' declared signatures, most common first: candidates for
 * the definition to adopt.
 */
export function callerSignatures(refused: readonly ReconcileResult[]): Prototype[] {
  const byShape = new Map<string, Prototype[]>();
  for (const declared of refused.map((r) => r.declared)) {
    if (!declared || declared.parameters === null) continue;
    const key = JSON.stringify(shapeOf(declared));
    byShape.set(key, [...(byShape.get(key) ?? []), declared]);
  }
  return [...byShape].sort((a, b) => b[1].length - a[1].length || a[0].localeCompare(b[0])).map(([, group]) => group[0]!);
}

/**
 * Try a caller's signature on the definition.
 *
 * A definition is a reconstruction; its callers' code is evidence too, and a
 * parameter the definition never reads, or a width it re-extends anyway, can
 * be written either way and still match. `signature` takes the caller's
 * parameter and result classes; `arity` keeps the definition's own parameters
 * and only appends the further ones the caller passes. The definition adopts
 * either only where its own object is unchanged. Returns the definition's new
 * source text, or nothing.
 */
export function adoptCallerSignature(
  sources: ReadonlyMap<string, string>,
  contract: Contract,
  wanted: Prototype,
  mode: "signature" | "arity" = "signature"
): { path: string; text: string } | undefined {
  if (contract.source !== "definition") return undefined;
  const mine = contract.prototype;
  if (wanted.variadic !== mine.variadic) return undefined;
  const path = mine.where;
  const text = sources.get(path);
  if (!text) return undefined;

  const tree = parseC(text);
  let edits: CSourceEdit[] = [];
  try {
    const definition = namedChildren(tree.rootNode).find((n) =>
      n.type === "function_definition" && declaratorName(field(n, "declarator"))?.text === contract.name);
    const declarator = definition && field(definition, "declarator");
    const parameters = declarator?.type === "function_declarator" ? field(declarator, "parameters") : undefined;
    if (!definition || !declarator || !parameters) return undefined;
    const spell = unitSpeller(tree.rootNode, visibleHeaders(includesOf(tree.rootNode, path)));
    const used = new Set<string>();
    walkActiveC(field(definition, "body")!, (node) => {
      if (node.type === "identifier") used.add(node.text);
      return true;
    });
    const declarations = namedChildren(parameters).filter((n) => n.type === "parameter_declaration");
    const mineTypes = mine.paramTypes ?? [], theirTypes = wanted.paramTypes ?? [], mineTexts = contract.parameters ?? [];
    if (mode === "arity" && theirTypes.length <= mineTypes.length) return undefined;
    const list: string[] = [];
    for (const [i, type] of theirTypes.entries()) {
      if (i < mineTypes.length && (mode === "arity" || abiClass(mineTypes[i]!) === abiClass(type))) {
        list.push(mineTexts[i]!);
        continue;
      }
      const spelled = spell(type);
      if (!spelled) return undefined;
      let name = i < declarations.length ? declaratorName(field(declarations[i]!, "declarator"))?.text : undefined;
      if (!name) for (name = `arg${i}`; used.has(name); name += "_");
      list.push(spelled.endsWith("*") ? `${spelled}${name}` : `${spelled} ${name}`);
    }
    edits = [{ start: parameters.startIndex, end: parameters.endIndex, text: `(${list.length > 0 ? list.join(", ") : "void"})` }];
    /* A scalar result can be re-declared; a void or pointer one stays the
       definition's, and callers that ignore it are reconciled to it. */
    const classes = [abiClass(wanted.returnType ?? "int"), abiClass(mine.returnType ?? "int")];
    const type = field(definition, "type");
    if (mode === "signature" && classes[0] !== classes[1] && type && !classes.some((c) => c === "void" || c === "ptr")) {
      const spelled = spell(wanted.returnType ?? "int");
      if (spelled) edits.push({ start: type.startIndex, end: type.endIndex, text: spelled });
    }
  } finally {
    tree.delete();
  }
  const adopted = applyCSourceEdits(text, edits);
  const before = objectSignature(text, path), after = objectSignature(adopted, path);
  return before.signature && before.signature === after.signature ? { path, text: adopted } : undefined;
}

/** The symbol an `__asm__` label binds `local` to, in any unit that declares it so. */
function labelTarget(local: string, sources: ReadonlyMap<string, string>): string | undefined {
  for (const [path, text] of sources) {
    if (!path.endsWith(".c") || !text.includes(local) || !text.includes("__asm__")) continue;
    const tree = parseC(text);
    try {
      let target: string | undefined;
      walkActiveC(tree.rootNode, (node) => {
        if (target || node.type !== "declaration") return node.type !== "comment" && !target;
        if (functionDeclarator(node, local)) target = asmLabel(node);
        return false;
      });
      if (target && target !== local) return target;
    } finally {
      tree.delete();
    }
  }
  return undefined;
}

export interface ProjectReconciliation {
  /** New text of every unit reconciliation changes. */
  texts: Map<string, string>;
  /** Units no interface reaches without changing their bytes, by callee. */
  disputes: Map<string, ReconcileResult[]>;
}

/**
 * Reconcile every flagged callee across the project, until a pass changes
 * nothing. A unit refused for one callee can become reconcilable once another
 * is settled (a header include that used to collide), hence the passes. Pure
 * over `sources`.
 */
export function reconcileProject(
  sources: ReadonlyMap<string, string>,
  sdk: ReadonlyMap<string, Prototype>,
  only: readonly string[] = [],
  log: (line: string) => void = () => undefined,
  maxPasses = Infinity
): ProjectReconciliation {
  const current = new Map(sources);
  const texts = new Map<string, string>();
  const disputes = new Map<string, ReconcileResult[]>();
  for (let pass = 1; pass <= maxPasses; pass++) {
    const flagged = new Set(auditInterfaces({ sources: current, sdk, published: publishedInterfaces(current) }).findings
      .filter((f) => f.kind === "conflict" || f.kind === "implicit" || f.kind === "arity").map((f) => f.callee));
    /* A callee named explicitly is reconciled even when nothing flags it:
       that is how a new module header reaches callers that declared the
       function themselves. */
    const names = (only.length > 0 ? [...only] : [...flagged].filter((n) => flagged.has(n))).sort();
    let progress = 0;
    disputes.clear();
    for (const flaggedName of names) {
      /* A local name an __asm__ label binds is reconciled as the symbol it names. */
      const name = labelTarget(flaggedName, current) ?? flaggedName;
      const contract = contractFor(name, current, sdk);
      if (typeof contract === "string") {
        disputes.set(name, [{ file: "-", callee: name, outcome: "refused", change: "replaced", casts: [], detail: contract }]);
        continue;
      }
      let { results, texts: changed } = reconcileCallers(current, contract);
      const disputed = results.filter((r) => r.outcome === "refused");
      /* The first caller signature, most common first, that the definition
         accepts unchanged and that leaves fewer units refused. */
      const candidates = (disputed.length > 0 ? callerSignatures(disputed) : [])
        .flatMap((wanted) => (["signature", "arity"] as const).map((mode) => ({ wanted, mode })));
      for (const { wanted, mode } of candidates) {
        const adopted = adoptCallerSignature(current, contract, wanted, mode);
        if (!adopted) continue;
        const trial = new Map(current).set(adopted.path, adopted.text);
        const revised = contractFor(name, trial, sdk);
        const retry = typeof revised === "string" ? undefined : reconcileCallers(trial, revised);
        if (!retry || retry.results.filter((r) => r.outcome === "refused").length >= disputed.length) continue;
        log(`  ${adopted.path}  ${name}: definition adopts its callers' ${(revised as Contract).text}`);
        current.set(adopted.path, adopted.text);
        texts.set(adopted.path, adopted.text);
        progress++;
        ({ results, texts: changed } = retry);
        break;
      }
      for (const [path, text] of changed) {
        current.set(path, text);
        texts.set(path, text);
      }
      progress += changed.size;
      const refused = results.filter((r) => r.outcome === "refused");
      if (refused.length > 0) disputes.set(name, refused);
      for (const r of results.filter((r) => r.outcome === "written")) {
        log(`  ${r.file}  ${name}: ${r.change}${r.casts.length > 0 ? `, cast ${r.casts.join(", ")}` : ""}`);
      }
    }
    log(`pass ${pass}: ${progress} unit change(s) across ${names.length} callee(s)`);
    if (progress === 0) break;
  }
  return { texts, disputes };
}

function reportDisputes(disputes: ReadonlyMap<string, ReconcileResult[]>): void {
  if (disputes.size === 0) return;
  console.log(`Not reconcilable without changing bytes — disputes for review: ${[...disputes.values()].flat().length} unit(s), ${disputes.size} callee(s)`);
  for (const [name, results] of [...disputes].sort()) {
    for (const r of results) console.log(`    ${name}  ${r.file}: ${r.detail}`);
  }
}

/* ------------------------------------------------------------------ */
/* CLI                                                                 */
/* ------------------------------------------------------------------ */

const HEADINGS: Record<FindingKind, string> = {
  conflict: "Prototype conflicts (declared type disagrees with the definition or SDK)",
  arity: "Call arity conflicts (unprototyped call, wrong argument count)",
  implicit: "Implicit declarations (call with no declaration in scope)",
  restated: "Published interfaces restated by a unit that includes their module header",
  undetermined: "Undetermined (a typedef the audit cannot classify)",
  unknown: "Unknown contracts (no matched definition and no SDK declaration)",
};

function main(): void {
  const args = process.argv.slice(2);
  const check = args.includes("--check");
  const detail = args.includes("--detail");
  const callees = args.flatMap((arg, i) => (arg === "--callee" && args[i + 1] ? [args[i + 1]!] : []));
  const only = callees.length === 1 ? callees[0] : undefined;

  if (args.includes("--write")) {
    const outcome = reconcileProject(activeSources(), sdkPrototypes(), callees, (line) => console.log(line));
    for (const [path, text] of outcome.texts) writeFileSync(join(ROOT, path), text);
    console.log(`Reconciled ${outcome.texts.size} unit(s).`);
    reportDisputes(outcome.disputes);
    headerCache.clear();
  }

  const sources = activeSources();
  const published = publishedInterfaces(sources);
  const result = auditInterfaces({ sources, sdk: sdkPrototypes(), published });
  const findings = only ? result.findings.filter((f) => f.callee === only) : result.findings;

  /* --check is a gate: the summary and the failures, not the backlog. */
  const listing = !check || detail || only !== undefined;
  for (const kind of Object.keys(HEADINGS) as FindingKind[]) {
    const ofKind = findings.filter((f) => f.kind === kind);
    if (!listing || ofKind.length === 0) continue;
    const byCallee = new Map<string, Finding[]>();
    for (const finding of ofKind) byCallee.set(finding.callee, [...(byCallee.get(finding.callee) ?? []), finding]);
    console.log(`${HEADINGS[kind]}: ${ofKind.length} site(s), ${byCallee.size} callee(s)`);
    for (const [callee, sites] of [...byCallee].sort((a, b) => b[1].length - a[1].length || a[0].localeCompare(b[0]))) {
      const first = sites[0]!;
      const files = new Set(sites.map((s) => s.file)).size;
      if (kind === "unknown") {
        console.log(`  ${callee}  (first seen ${first.file}:${first.line}${first.declared ? `, declared ${first.declared}` : ""})`);
        continue;
      }
      const differences = [...new Set(sites.flatMap((s) => s.differences ?? []))].join(", ");
      console.log(`  ${callee}  ${sites.length} site(s) in ${files} file(s)${differences ? `  [${differences}]` : ""}${first.witness ? `  vs ${first.witness}` : ""}`);
      if (detail || only) {
        for (const site of sites) {
          const extra = [site.declared, site.differences?.join(", "), site.callback ? "callback" : ""].filter(Boolean).join("  ");
          console.log(`      ${site.file}:${site.line}${extra ? `  ${extra}` : ""}`);
        }
      }
    }
  }

  const counts = (Object.keys(HEADINGS) as FindingKind[]).map((k) => `${result.findings.filter((f) => f.kind === k).length} ${k}`).join(", ");
  console.log(`interfaces: ${counts} (${result.units} units; ${published.size} published by module headers, ` +
    `${result.publishedFailures.length} failing)`);
  if (check) {
    for (const failure of result.publishedFailures) {
      console.error(`  ${failure.kind}: ${failure.callee} at ${failure.file}:${failure.line}${failure.differences ? ` [${failure.differences.join(", ")}]` : ""}`);
    }
    /* Interfaces are at their fixed point when reconciling would change nothing. */
    const pending = reconcileProject(sources, sdkPrototypes(), callees, () => undefined, 1);
    for (const path of [...pending.texts.keys()].sort()) console.error(`  not reconciled: ${path}`);
    if (pending.texts.size > 0) console.error(`interfaces: ${pending.texts.size} unit(s) are not reconciled — run: npx tsx tools/diagnostics/interfaceAudit.ts --write`);
    console.log(`interfaces: ${[...pending.disputes.values()].flat().length} disputed unit(s) across ${pending.disputes.size} callee(s) (--write lists them)`);
    if (result.publishedFailures.length > 0 || pending.texts.size > 0) process.exit(1);
  }
}

if (process.argv[1] && resolve(process.argv[1]) === fileURLToPath(import.meta.url)) main();
