#!/usr/bin/env npx tsx
/**
 * foldTranslationUnit.ts — fold contiguous, already-matching functions into
 * one reconstructed module translation unit.
 *
 * This is not a grouping oracle. Which functions belong together is evidence
 * the caller brings (callers, private state, link order, notes/file-groupings.md);
 * this tool only does the mechanical part, and refuses whenever the mechanics
 * cannot be done soundly:
 *
 *  - members are container-qualified original functions, contiguous in the
 *    original address order, and already implemented as compiled C — except a
 *    dead function's `INCLUDE_ASM` stub, which the module carries, pointed at
 *    the module's own extracted assembly;
 *  - a fold takes whole units: per-function sources, the module being
 *    extended, and any other module it absorbs whole (a merge);
 *  - the fold crosses no other configured segment — no non-member function,
 *    assembly, library object or data;
 *  - no multi-member unit carries a per-function compiler-flag override.
 *    A singleton may rename its existing override key without broadening its
 *    scope; --write first measures baseline, then checks the retained profile.
 *    Multi-member migrations require an exact configs/tu_flag_reviews.json
 *    approval; baseline is measured for every member before the linked check;
 *  - the members' jump tables form one run of the rodata window, in member
 *    order, with only verified zero padding for cc1's table alignment between
 *    them — the only layout a pooled `.rodata` section can reproduce;
 *  - merging the sources introduces no conflict: includes are unioned,
 *    byte-identical top-level declarations are kept once, and a macro or
 *    file-scope name defined differently, or a macro that would leak into a
 *    later member, refuses the fold.
 *
 * A dry run (the default) prints every consequence. `--write` applies them —
 * the module source, the deleted per-function files and the configuration —
 * then re-splits the container, builds and checks it, and compares every
 * compiled member with the per-function oracle. Any failure restores every file it
 * touched and re-splits, so the tree is never left half-folded. It records no
 * queue and no claim beyond the source and configuration themselves.
 *
 * Usage:
 *   npx tsx tools/build/foldTranslationUnit.ts --container ovl_31 --module memcard_format \
 *     --members ovl_31_func_800B82E8,ovl_31_func_800B8348 [--summary "<one paragraph>"] [--write]
 */

import { spawnSync } from "node:child_process";
import { createHash } from "node:crypto";
import { existsSync, mkdirSync, readFileSync, rmSync, statSync, writeFileSync } from "node:fs";
import { availableParallelism } from "node:os";
import { dirname, join, posix } from "node:path";
import { ROOT } from "../lib/psxExeInfo.js";
import { containerTargetPath, requireContainer, type Container } from "../lib/container.js";
import { elf32SectionSizes } from "../lib/elfSections.js";
import { failureLines } from "../lib/failureLines.js";
import { loadOriginalFunctionInventory, loadSubsegments, type OriginalFunctionSpan } from "../lib/symbolIndex.js";
import { flagOverrideKey, loadTranslationUnits, MODULE_DIR, moduleStemProblem, type TranslationUnit } from "../lib/translationUnits.js";
import { auditContainer, loadFlagOverrideKeys } from "../lib/translationUnitAudit.js";
import { literalFlagSetting, loadTuFlagReviews, originalIdentity, reviewedTuFlagOwner } from "../lib/tuFlagReviews.js";
import { field, namedChildren, parseC, type Node } from "../agent/residual-source-search/tree-sitter-c.js";
import { compareFunction, renderDiff } from "../lib/functionOracle.js";
import { computeLiveness } from "../lib/liveness.js";
import { compileSource } from "../agent/decompToolchain.js";
import { findWindow, mixedRodataAddress, parseSubsegments } from "./deriveRodataSplits.js";
import { parseFuncExtents } from "../lib/jumpTables.js";

export interface FoldRequest {
  container: string;
  module: string;
  members: string[];
  summary?: string | undefined;
}

export interface FoldPlan {
  container: Container;
  stem: string;
  modulePath: string;
  members: OriginalFunctionSpan[];
  /** Members the module still hands to the assembler: dead functions' stubs. */
  stubs: string[];
  /** Members whose embedded asm names cc1 labels, with the labels their own assembly defines, in order. */
  relabel: Array<{ member: string; labels: string[] }>;
  /** Units folded away, in address order (an existing module being extended or absorbed included). */
  units: TranslationUnit[];
  removedSources: string[];
  moduleText: string;
  configPath: string;
  configText: string;
  /** Existing setting migrated to a singleton or an explicitly reviewed whole TU. */
  flagConfig?: { path: string; text: string; owner: string; flags: string };
  notes: string[];
}

export class Refusal extends Error {}

function refuse(message: string): never {
  throw new Refusal(message);
}

function hex(value: number): string {
  return `0x${value.toString(16).toUpperCase()}`;
}

// --- source merging ---------------------------------------------------------

interface Piece { start: number; end: number }

const DEDUPED = new Set(["declaration", "preproc_def", "preproc_function_def", "type_definition", "struct_specifier", "union_specifier", "enum_specifier"]);

function normalized(text: string): string {
  return text.replace(/\/\*[\s\S]*?\*\//g, " ").replace(/\s+/g, " ").trim();
}

/** The identifier a top-level node defines, for conflict detection. */
function definedName(node: Node): string | undefined {
  if (node.type === "preproc_def" || node.type === "preproc_function_def") return field(node, "name")?.text;
  if (node.type === "function_definition") {
    let declarator = field(node, "declarator");
    while (declarator && declarator.type !== "identifier") declarator = field(declarator, "declarator") ?? undefined;
    return declarator?.text;
  }
  return undefined;
}

function isStatic(node: Node): boolean {
  return namedChildren(node).some((child) => child.type === "storage_class_specifier" && child.text === "static");
}

/** Identifiers a source spells, outside comments and strings. */
function identifiers(root: Node): Set<string> {
  const names = new Set<string>();
  const visit = (node: Node) => {
    if (node.type === "identifier" || node.type === "type_identifier" || node.type === "field_identifier") names.add(node.text);
    for (const child of namedChildren(node)) visit(child);
  };
  visit(root);
  return names;
}

/**
 * One module source from several per-function sources, in address order.
 *
 * Each file's text is kept as written except for what the merge owns: its
 * `#include` lines move to one shared preamble, and a top-level declaration
 * byte-identical (ignoring whitespace and comments) to one already emitted is
 * dropped. A file that holds nothing but includes and comments — a nested
 * function's carrier — contributes nothing.
 */
export function mergeSources(files: Array<{ path: string; text: string }>, header: string): { text: string; notes: string[] } {
  /* A one-unit move has nothing to union. Preserve preprocessing order:
     source-local header selectors must still precede their includes. */
  if (files.length === 1) {
    const file = files[0]!;
    const tree = parseC(file.text);
    try {
      if (tree.rootNode.hasError) refuse(`${file.path} does not parse as C`);
      return {
        text: file.path.includes(`/${MODULE_DIR}/`) ? file.text : `${header}\n\n${file.text.trim()}\n`,
        notes: [],
      };
    } finally {
      tree.delete();
    }
  }
  const includes: string[] = [];
  const seen = new Set<string>();
  const macros = new Map<string, { text: string; from: string }>();
  const statics = new Map<string, string>();
  const bodies: string[] = [];
  const notes: string[] = [];
  let leading: string | null = null;

  files.forEach((file, index) => {
    const tree = parseC(file.text);
    try {
      if (tree.rootNode.hasError) refuse(`${file.path} does not parse as C`);
      const cut: Piece[] = [];
      const nodes = namedChildren(tree.rootNode);
      /* Union the includes without reordering any file's own sequence: a new
         header goes just before the next one this file already shares. */
      const own = nodes.filter((node) => node.type === "preproc_include").map((node) => node.text.trim());
      own.forEach((line, i) => {
        if (includes.includes(line)) return;
        const successor = own.slice(i + 1).find((later) => includes.includes(later));
        if (successor === undefined) includes.push(line);
        else includes.splice(includes.indexOf(successor), 0, line);
      });
      let code = false;
      for (const node of nodes) {
        let end = node.endIndex;
        const after = file.text.slice(end).match(/^[ \t]*;/);
        if (after && node.type.endsWith("_specifier")) end += after[0].length;
        if (node.type === "preproc_include") {
          cut.push({ start: node.startIndex, end });
          continue;
        }
        if (node.type === "comment") continue;
        code = true;
        const name = definedName(node);
        if (name && (node.type === "preproc_def" || node.type === "preproc_function_def")) {
          const previous = macros.get(name);
          if (previous && normalized(previous.text) !== normalized(node.text)) refuse(`${file.path} defines macro ${name} differently from ${previous.from}`);
          if (!previous) macros.set(name, { text: node.text, from: file.path });
        }
        if ((node.type === "declaration" || node.type === "function_definition") && isStatic(node)) {
          const declared = node.type === "function_definition" ? name : node.descendantsOfType("identifier")[0]?.text;
          if (declared) {
            const owner = statics.get(declared);
            if (owner && owner !== file.path && node.type === "function_definition") refuse(`${file.path} and ${owner} both define static ${declared}`);
            statics.set(declared, file.path);
          }
        }
        if (DEDUPED.has(node.type)) {
          const key = normalized(file.text.slice(node.startIndex, end));
          if (seen.has(key)) {
            cut.push({ start: node.startIndex, end });
            notes.push(`kept one copy of \`${key.length > 60 ? `${key.slice(0, 57)}...` : key}\` (also in ${file.path})`);
            continue;
          }
          seen.add(key);
        }
      }
      /* A macro this file defines must not silently reach a later member. */
      for (const node of nodes) {
        if (node.type !== "preproc_def" && node.type !== "preproc_function_def") continue;
        const name = definedName(node);
        if (!name) continue;
        for (const later of files.slice(index + 1)) {
          const laterTree = parseC(later.text);
          try {
            const redefines = namedChildren(laterTree.rootNode).some((n) => (n.type === "preproc_def" || n.type === "preproc_function_def") && definedName(n) === name);
            if (!redefines && identifiers(laterTree.rootNode).has(name)) refuse(`macro ${name} from ${file.path} would apply to ${later.path}`);
          } finally { laterTree.delete(); }
        }
      }
      if (!code) {
        notes.push(`${file.path} holds only includes and comments (a nested function's carrier); nothing of it is kept`);
        return;
      }
      let body = "";
      let cursor = 0;
      for (const piece of cut.sort((a, b) => a.start - b.start)) {
        body += file.text.slice(cursor, piece.start);
        cursor = piece.end;
        if (file.text[cursor] === "\n") cursor++;
      }
      body += file.text.slice(cursor);
      body = body.replace(/\n{3,}/g, "\n\n").trim();
      /* An extended module keeps its own header comment at the top. */
      if (index === 0 && file.path.includes(`/${MODULE_DIR}/`)) {
        const first = nodes.find((node) => node.type !== "preproc_include");
        if (first?.type === "comment") {
          leading = first.text;
          body = body.startsWith(first.text) ? body.slice(first.text.length).trim() : body;
        }
      }
      if (body) bodies.push(body);
    } finally {
      tree.delete();
    }
  });

  const text = [leading ?? header, includes.join("\n"), ...bodies].filter(Boolean).join("\n\n") + "\n";
  return { text, notes };
}

/**
 * A splat config with some units folded into one module: their `c` lines
 * become one line at the first unit's address, and their jump-table
 * attributions one pooled `.rodata` line at the first table. Refuses when the
 * tables are out of member order or separated by another subsegment, except
 * the original-byte/compiler-witnessed alignment gaps supplied by planFold.
 */
export function foldConfigLines(lines: readonly string[], unitStems: readonly string[], stem: string, alignmentPadding: ReadonlySet<number> = new Set(), mixedPoolGaps: ReadonlySet<number> = new Set(), mixedPool?: { start: number; end: number }): { lines: string[]; notes: string[] } {
  const entry = (line: string) => line.match(/^(\s*)- \[(0x[0-9A-Fa-f]+), ([^,\]]+)(?:, ([^,\]]+))?/);
  const stems = new Set(unitStems);
  const rodata = lines.map((line, index) => ({ line, index, m: entry(line) }))
    .filter((row) => row.m && (row.m[3]!.trim() === ".rodata" || row.m[3]!.trim() === ".rdata") && stems.has(row.m[4]!.trim()));
  const notes: string[] = [];
  if (mixedPool) {
    /* A uniquely byte-proved pool can begin before its first switch table
       and end inside a generic residue. Preserve the unowned prefix/suffix;
       never absorb another unit's attributed section. */
    const order = rodata.map((row) => unitStems.indexOf(row.m![4]!.trim()));
    if (order.some((value, i) => i > 0 && value < order[i - 1]!)) refuse("the members' jump tables are not in member order in the rodata window");
    const rows = lines.flatMap((line, index) => {
      const m = entry(line);
      return m ? [{ index, m, addr: Number(m[2]) }] : [];
    });
    const overlap = rows.flatMap((row, i) => {
      const end = rows[i + 1]?.addr;
      return end !== undefined && row.addr < mixedPool.end && end > mixedPool.start ? [{ ...row, end }] : [];
    });
    if (mixedPool.end <= mixedPool.start || overlap.length === 0) refuse("empty mixed rodata pool");
    for (const row of overlap) {
      const kind = row.m[3]!.trim(), name = row.m[4]?.trim();
      if (kind === "rodata" && !name) continue;
      if ([".rodata", ".rdata"].includes(kind) && name && stems.has(name) &&
          row.addr >= mixedPool.start && row.end <= mixedPool.end) continue;
      refuse(`mixed rodata pool crosses an unowned subsegment: ${lines[row.index]!.trim()}`);
    }
    const first = overlap[0]!, last = overlap[overlap.length - 1]!;
    if (first.addr > mixedPool.start || last.end < mixedPool.end) refuse("mixed rodata pool is not covered by the configured window");
    const indent = first.m[1]!;
    const replacement: string[] = [];
    if (first.addr < mixedPool.start) replacement.push(`${indent}- [${hex(first.addr)}, rodata]`);
    replacement.push(`${indent}- [${hex(mixedPool.start)}, .rodata, ${stem}]`);
    if (last.end > mixedPool.end) replacement.push(`${indent}- [${hex(mixedPool.end)}, rodata]`);
    const rebuilt = [...lines.slice(0, first.index), ...replacement, ...lines.slice(last.index + 1)];
    /* Match bootstrapOverlay's input alignment derivation when the proved
       section leaves a sub-word original residue. Section transitions keep
       their explicit word alignment; no bytes may be inserted before a tail. */
    let subalign = 4;
    for (const row of findWindow(parseSubsegments(rebuilt)).members) {
      while (subalign > 1 && row.addr % subalign !== 0) subalign /= 2;
    }
    const alignmentIndex = rebuilt.findLastIndex((line, i) => i < first.index && /^    subalign: \d+\s*$/.test(line));
    if (alignmentIndex >= 0) rebuilt[alignmentIndex] = `    subalign: ${subalign}`;
    const folded = foldConfigLines(rebuilt, unitStems, stem);
    folded.notes.push(`unique relocated whole-pool proof establishes .rodata ${hex(mixedPool.start)}..${hex(mixedPool.end)}; unowned edge residues retained`);
    return folded;
  }
  if (rodata.length > 1) {
    const order = rodata.map((row) => unitStems.indexOf(row.m![4]!.trim()));
    if (order.some((value, i) => i > 0 && value < order[i - 1]!)) refuse("the members' jump tables are not in member order in the rodata window; one pooled .rodata cannot reproduce that");
    for (let i = 1; i < rodata.length; i++) {
      const between = lines.slice(rodata[i - 1]!.index + 1, rodata[i]!.index)
        .filter((line) => { const m = entry(line); return m && !alignmentPadding.has(Number(m[2])) && !mixedPoolGaps.has(Number(m[2])); });
      if (between.length > 0) refuse(`the members' jump tables are not contiguous: ${between[0]!.trim()} lies between them`);
    }
  }
  const out: string[] = [];
  let placedC = false;
  let placedRodata = false;
  for (const [index, line] of lines.entries()) {
    const m = entry(line);
    if (m && mixedPoolGaps.has(Number(m[2]))) {
      notes.push(`${m[2]}: literal/table gap absorbed by unique relocated whole-pool byte proof`);
      continue;
    }
    if (m && alignmentPadding.has(Number(m[2]))) {
      notes.push(`${m[2]}: verified zero alignment gap absorbed into the pooled .rodata`);
      continue;
    }
    if (m && m[3]!.trim() === "c" && stems.has(m[4]!.trim())) {
      if (!placedC) out.push(`${m[1]}- [${m[2]}, c, ${stem}]`);
      placedC = true;
      continue;
    }
    if (rodata.some((row) => row.index === index)) {
      if (!placedRodata) out.push(`${m![1]}- [${m![2]}, ${m![3]!.trim()}, ${stem}]`);
      placedRodata = true;
      continue;
    }
    out.push(line);
  }
  if (rodata.length > 0) notes.push(`${rodata.length} jump-table attribution(s) pooled into [${rodata[0]!.m![2]}, .rodata, ${stem}]; deriveRodataSplits re-derives its extent from the module object`);
  return { lines: out, notes };
}

// --- planning ---------------------------------------------------------------

function moduleHeader(name: string, summary?: string): string {
  const lines = [`/*`, ` * ${name} — reconstructed translation unit.`];
  if (summary) {
    lines.push(" *");
    const words = summary.trim().split(/\s+/);
    let line = " *";
    for (const word of words) {
      if ((line + " " + word).length > 78) { lines.push(line); line = " *"; }
      line += " " + word;
    }
    if (line !== " *") lines.push(line);
  }
  lines.push(" *", " * The evidence for its membership and boundaries is in", " * notes/file-groupings.md.", " */");
  return lines.join("\n");
}

// --- cc1 label references ----------------------------------------------------

/* cc1 numbers its internal labels (`$L<n>`) across the whole file, so embedded
   asm that branches to one names a number its file decided. In a module the
   same code gets the same labels in the same order, numbered after every
   earlier member's: the references are renumbered by position. */
const CC1_LABEL = /\$L\d+/g;

/** The `$L<n>` labels cc1 defines in one function of its assembly output, in order. */
export function labelDefinitions(asm: string, fn: string): string[] {
  const lines = asm.split("\n");
  const start = lines.indexOf(`${fn}:`);
  if (start < 0) return [];
  const labels: string[] = [];
  for (const line of lines.slice(start + 1)) {
    if (new RegExp(`^\\s*\\.end\\s+${fn}\\s*$`).test(line)) break;
    const label = line.match(/^(\$L\d+):/);
    if (label) labels.push(label[1]!);
  }
  return labels;
}

/** The string literals of the asm statements inside one function definition. */
function asmStrings(root: Node, fn: string): Node[] {
  const definition = root.descendantsOfType("function_definition").find((node) => {
    let declarator = node ? field(node, "declarator") : null;
    while (declarator && declarator.type !== "identifier") declarator = field(declarator, "declarator") ?? null;
    return declarator?.text === fn;
  });
  return definition ? definition.descendantsOfType("gnu_asm_expression").flatMap((asm) => asm ? asm.descendantsOfType("string_literal").filter((s): s is Node => !!s) : []) : [];
}

/** The cc1 labels one function's embedded asm names, from its source. */
export function asmLabelReferences(text: string, fn: string): string[] {
  const tree = parseC(text);
  try {
    return [...new Set(asmStrings(tree.rootNode, fn).flatMap((literal) => literal.text.match(CC1_LABEL) ?? []))];
  } finally {
    tree.delete();
  }
}

/** Rewrite one function's asm label references through `mapping`. */
export function renumberAsmLabels(text: string, fn: string, mapping: ReadonlyMap<string, string>): string {
  const tree = parseC(text);
  try {
    const edits = asmStrings(tree.rootNode, fn).map((literal) => ({
      start: literal.startIndex, end: literal.endIndex,
      text: literal.text.replace(CC1_LABEL, (label) => mapping.get(label) ?? refuse(`${fn}'s asm names ${label}, which its own assembly does not define`)),
    }));
    return edits.sort((a, b) => b.start - a.start).reduce((out, edit) => out.slice(0, edit.start) + edit.text + out.slice(edit.end), text);
  } finally {
    tree.delete();
  }
}

/**
 * Point a unit's `INCLUDE_ASM` stubs at the module's extracted assembly:
 * splat writes a C unit's undecompiled functions under the unit's stem, so a
 * stub that moves into the module moves from `<dir>/<from>` to `<dir>/<to>`.
 */
export function retargetStubs(text: string, from: string, to: string): string {
  const tree = parseC(text);
  try {
    const edits: Array<{ start: number; end: number; text: string }> = [];
    for (const call of tree.rootNode.descendantsOfType("call_expression")) {
      if (!call || field(call, "function")?.text !== "INCLUDE_ASM") continue;
      const args = field(call, "arguments");
      const folder = args ? namedChildren(args).find((child) => child.type === "string_literal") : undefined;
      const value = folder?.text.slice(1, -1);
      if (!folder || value === undefined || !value.endsWith(`/${from}`)) refuse(`${call.text} does not name the extracted assembly of ${from}`);
      edits.push({ start: folder.startIndex, end: folder.endIndex, text: `"${value.slice(0, -from.length)}${to}"` });
    }
    return edits.sort((a, b) => b.start - a.start).reduce((out, edit) => out.slice(0, edit.start) + edit.text + out.slice(edit.end), text);
  } finally {
    tree.delete();
  }
}

export function planFold(request: FoldRequest): FoldPlan {
  const container = requireContainer(request.container);
  const stem = `${MODULE_DIR}/${request.module}`;
  const problem = moduleStemProblem(stem);
  if (problem) refuse(problem);
  const inventory = loadOriginalFunctionInventory(container);
  if (inventory.findings.length > 0) refuse(`the ${container.id} inventory is undetermined: ${inventory.findings[0]}`);
  const game = inventory.functions.filter((fn) => fn.origin === "game");
  const wanted = [...new Set(request.members)];
  if (wanted.length === 0) refuse("--members names no function");
  const members = wanted.map((name) => game.find((fn) => fn.name === name) ?? refuse(`${name} is not an original game function of ${container.id}`))
    .sort((a, b) => a.vram - b.vram);

  /* Contiguous in original order: no game function between two members. */
  const first = game.indexOf(members[0]!);
  members.forEach((member, i) => {
    if (game[first + i] !== member) refuse(`members are not contiguous: ${game[first + i]?.name ?? "the end of text"} lies between ${members[i - 1]!.name} and ${member.name}`);
  });

  const units = loadTranslationUnits(container);
  const touched = units.filter((unit) => unit.members.some((m) => wanted.includes(m.name)));
  /* Whole units only; another module listed whole is absorbed (a merge). */
  for (const unit of touched) {
    const stray = unit.members.filter((m) => !wanted.includes(m.name));
    if (stray.length > 0) refuse(`${unit.sourcePath} also holds ${stray.map((m) => m.name).join(", ")}; a fold takes whole units — list them too`);
  }
  const existing = units.find((unit) => unit.segmentName === stem);
  if (existing && !touched.includes(existing)) refuse(`${stem} already exists elsewhere in ${container.id}; extend it by listing its members`);

  /* No configured segment other than the members' own units may lie inside the fold. */
  const startRom = Math.min(...touched.map((u) => u.startRom));
  const endRom = Math.max(...touched.map((u) => u.endRom));
  for (const segment of loadSubsegments(container)) {
    if (segment.rom <= startRom || segment.rom >= endRom) continue;
    if (segment.type === "c" && touched.some((u) => u.segmentName === segment.name)) continue;
    refuse(`the fold would cross [${hex(segment.rom)}, ${segment.type}${segment.name ? `, ${segment.name}` : ""}]`);
  }

  /* Implemented C, proved by the audit's source view — or a dead function's
     stub, which is backlog wherever it sits, so the module carries it. A live
     function's stub is unfinished decompilation and stays out. */
  const audit = auditContainer(container, { objects: false });
  const liveness = computeLiveness();
  const stubs: string[] = [];
  for (const unit of touched) {
    const entry = audit.units.find((u) => u.unit.segmentName === unit.segmentName)!;
    if (entry.status === "invalid") refuse(`${unit.sourcePath} is not a valid unit: ${entry.findings[0]}`);
    for (const member of unit.members) {
      if (entry.stubbed.has(member.name)) {
        const dead = container.kind === "exe" && liveness.referenced.size > 0 && !liveness.referenced.has(member.vram);
        if (!dead) refuse(`${member.name} is live and still an INCLUDE_ASM stub; decompile it before folding it`);
        stubs.push(member.name);
        continue;
      }
      if (!entry.defined.has(member.name)) refuse(`${unit.sourcePath} does not define ${member.name}`);
    }
  }
  if (stubs.length === members.length) refuse("every member is an INCLUDE_ASM stub; a module needs compiled C");

  /* One command line for the whole unit. */
  const keys = loadFlagOverrideKeys();
  const modulePath = posix.join(container.paths.srcDir, `${stem}.c`);
  let flagConfig: FoldPlan["flagConfig"];
  for (const member of members) {
    if (!keys.has(member.name)) continue;
    if (flagConfig) refuse("multiple flag owners require a separate review; this migration retains one existing exception");
    const path = "configs/flag_overrides.mk";
    const text = readFileSync(join(ROOT, path), "utf8");
    const key = member.name.replace(/[.*+?^${}()|[\]\\]/g, "\\$&");
    const pattern = new RegExp(`^CC1FLAGS_${key}\\s*:?=\\s*(.+)$`, "m");
    const flags = literalFlagSetting(text, member.name);
    if (!flags) refuse(`${member.name}: migration requires one existing literal flag list`);
    if (members.length !== 1 && (stubs.length > 0 || reviewedTuFlagOwner(loadTuFlagReviews(), modulePath,
      container.id, members.map(m => originalIdentity(container.id, m.vram)), flags) !== originalIdentity(container.id, member.vram))) {
      refuse(`${member.name} has a per-function flag override (CC1FLAGS_${member.name}); reconcile it per plans/flag-review-report.md and record the exact whole-TU approval before folding`);
    }
    flagConfig = {
      path, owner: member.name, flags,
      text: text.replace(pattern, (line) => line.replace(`CC1FLAGS_${member.name}`, `CC1FLAGS_${flagOverrideKey(modulePath)}`)),
    };
  }
  if (keys.has(flagOverrideKey(modulePath))) refuse(`configs/flag_overrides.mk already overrides ${flagOverrideKey(modulePath)}`);

  /* Rewrite the configuration: one `c` line, one pooled `.rodata` line. */
  const configPath = container.paths.splat;
  /* A per-function object's .rodata stops at its last emitted byte; the next
     function's .align 2/3 may leave zero bytes extracted as generic rodata.
     In a pooled object that SAME directive emits the gap internally. Admit
     only the exact round-up, witnessed by the prior object's extent, original
     bytes and the next compiler output's first rodata alignment. The checked
     write still proves every pooled position and the complete container. */
  const segments = loadSubsegments(container);
  /* .align is section-relative, not ROM-relative. Some overlays' pooled
     table run starts four bytes into the file (ovl_19 is one witness). */
  const pooledRodataBase = segments.find((segment) => segment.type === ".rodata" && touched.some((unit) => unit.segmentName === segment.name))?.rom;
  const alignmentPadding = new Set<number>();
  const original = readFileSync(containerTargetPath(container));
  segments.forEach((segment, i) => {
    const previous = segments[i - 1], next = segments[i + 1];
    if (segment.type !== "rodata" || segment.name || segment.size < 1 || segment.size > 7 || pooledRodataBase === undefined) return;
    if (previous?.type !== ".rodata" || next?.type !== ".rodata" || next.rom !== segment.rom + segment.size) return;
    const left = touched.find((u) => u.segmentName === previous.name);
    const right = touched.find((u) => u.segmentName === next.name);
    if (!left || !right || !existsSync(join(ROOT, left.objectPath))) return;
    const size = elf32SectionSizes(readFileSync(join(ROOT, left.objectPath))).get(".rodata");
    if (size === undefined || previous.rom + size !== segment.rom) return;
    const asm = join(ROOT, right.objectPath.replace(/\.c\.o$/, ".s"));
    if (!existsSync(asm)) return;
    const directive = readFileSync(asm, "utf8").match(/\.(?:rdata|section\s+\.rodata)\s*\n\s*\.align\s+(\d+)\b/);
    if (!directive || ![2, 3].includes(Number(directive[1]))) return;
    const alignment = 2 ** Number(directive[1]);
    const rounded = pooledRodataBase + Math.ceil((segment.rom - pooledRodataBase) / alignment) * alignment;
    if (rounded !== next.rom) return;
    const bytes = original.subarray(segment.rom, next.rom);
    if (bytes.length === segment.size && bytes.every((byte) => byte === 0)) alignmentPadding.add(segment.rom);
  });
  const configLines = readFileSync(join(ROOT, configPath), "utf8").split("\n");
  const files = touched.map((unit) => ({ path: unit.sourcePath, text: retargetStubs(readFileSync(join(ROOT, unit.sourcePath), "utf8"), unit.segmentName, stem) }));
  const merged = mergeSources(files, moduleHeader(request.module, request.summary));
  const mixedPoolGaps = new Set<number>();
  let mixedPool: { start: number; end: number } | undefined;
  const owned = segments.filter((s) => s.type === ".rodata" && touched.some((u) => u.segmentName === s.name));
  const gaps = owned.length > 1 ? segments.filter((s) => s.rom > owned[0]!.rom &&
    s.rom < owned[owned.length - 1]!.rom && s.type === "rodata" && !s.name && !alignmentPadding.has(s.rom)) : [];
  if (gaps.length > 0 && stubs.length === 0) {
    /* Mixed literals and tables must be reconstructed in C first. Prove the
       complete merged pool, including relocated table entries, before relaxing
       the gap guard. This never admits an unknown/nonmember attribution. */
    const dir = join(ROOT, "build/foldTranslationUnit", container.id, request.module);
    mkdirSync(dir, { recursive: true });
    const source = join(dir, "pool-proof.c");
    writeFileSync(source, merged.text);
    const compiled = compileSource(source, dir, "pool-proof", {
      assemble: true, useOverrides: false, containerKind: container.kind,
    });
    const object = readFileSync(compiled.object!);
    const extents = parseFuncExtents(container).filter((fn) => wanted.includes(fn.name));
    const base = mixedRodataAddress(original, object, extents, findWindow(parseSubsegments(configLines)));
    const size = elf32SectionSizes(object).get(".rodata") ?? 0;
    if (extents.length === members.length && base !== null && base <= owned[0]!.rom &&
        owned.every((segment) => segment.rom + segment.size <= base + size)) {
      mixedPool = { start: base, end: base + size };
      for (const gap of gaps) {
        if (gap.rom + gap.size <= base + size) mixedPoolGaps.add(gap.rom);
      }
    }
  }
  const rewritten = foldConfigLines(configLines, touched.map((u) => u.segmentName), stem, alignmentPadding, mixedPoolGaps, mixedPool);
  const notes = [...rewritten.notes];
  if (flagConfig) notes.push(`renames ${flagConfig.owner}'s existing flag key to ${flagOverrideKey(modulePath)} (${flagConfig.flags}); ${members.length === 1 ? "one function, unchanged exception scope" : "explicitly reviewed whole-TU compilation scope"}; --write measures every member at baseline first`);

  /* Embedded asm that names a cc1 label: read the labels its unit's own
     assembly defines now, to renumber them once the module is compiled. */
  const relabel: FoldPlan["relabel"] = [];
  for (const [index, unit] of touched.entries()) {
    for (const member of unit.members) {
      if (asmLabelReferences(files[index]!.text, member.name).length === 0) continue;
      const asmPath = join(ROOT, unit.objectPath.replace(/\.c\.o$/, ".s"));
      const labels = existsSync(asmPath) ? labelDefinitions(readFileSync(asmPath, "utf8"), member.name) : [];
      if (labels.length === 0) refuse(`${member.name}'s embedded asm names a cc1 label, and ${unit.objectPath.replace(/\.c\.o$/, ".s")} defines none for it — run make first`);
      relabel.push({ member: member.name, labels });
      notes.push(`${member.name}'s embedded asm names cc1 label(s) ${asmLabelReferences(files[index]!.text, member.name).join(", ")}; --write renumbers them to the module's own`);
    }
  }

  const absorbed = touched.filter((unit) => unit.foldedModule && unit.segmentName !== stem).map((unit) => unit.segmentName);
  if (absorbed.length > 0) notes.push(`absorbs ${absorbed.join(", ")}; their headers still publish the members they declare`);
  if (stubs.length > 0) notes.push(`carries the dead stub(s) ${stubs.join(", ")}, extracted under ${stem}`);
  return {
    container,
    stem,
    modulePath,
    members,
    stubs,
    relabel,
    units: touched,
    removedSources: touched.map((u) => u.sourcePath).filter((path) => path !== modulePath),
    moduleText: merged.text,
    configPath,
    configText: rewritten.lines.join("\n"),
    ...(flagConfig ? { flagConfig } : {}),
    notes: [...notes, ...merged.notes],
  };
}

// --- applying ---------------------------------------------------------------

function run(command: string, args: string[]): { code: number; output: string } {
  const result = spawnSync(command, args, { cwd: ROOT, encoding: "utf8", maxBuffer: 256 * 1024 * 1024, env: { ...process.env, MAKEFLAGS: "" } });
  return { code: result.status ?? 1, output: `${result.stdout ?? ""}${result.stderr ?? ""}` };
}

function splitTarget(container: Container): string {
  return container.kind === "exe" ? "split" : `split-${container.id}`;
}

function checkTarget(container: Container): string {
  return container.kind === "exe" ? "check" : `check-${container.id}`;
}

/**
 * Apply a plan, prove it, and roll back on any failure.
 *
 * The container is re-split (that is what regenerates its linker script from
 * the new configuration), built and byte-checked; then each member is compared
 * on its own through the module's object. That proves the fold where it
 * lands; `tuProgress.ts --verify` remains the all-container authority, run once
 * over a batch of folds.
 */
export function applyFold(plan: FoldPlan, log: (line: string) => void = console.log): { ok: boolean; detail: string } {
  const touched = [plan.modulePath, plan.configPath, ...plan.removedSources, ...(plan.flagConfig ? [plan.flagConfig.path] : [])];
  const saved = new Map(touched.map((path) => [path, existsSync(join(ROOT, path)) ? readFileSync(join(ROOT, path)) : null]));
  const rollback = (why: string): { ok: false; detail: string } => {
    for (const [path, content] of saved) {
      if (content === null) rmSync(join(ROOT, path), { force: true });
      else writeFileSync(join(ROOT, path), content);
    }
    log(`rolled back: ${why}`);
    const resplit = run("make", [splitTarget(plan.container)]);
    if (resplit.code !== 0) return { ok: false, detail: `${why}\nRe-split after rollback failed:\n${failureLines(resplit.output, 8).join("\n")}` };
    /* Restoring bytes rewrites source mtimes, and an extended module may
       have overwritten its old object. Restore the built container too. */
    log(`make ${checkTarget(plan.container)} (restore objects and linked bytes after rollback)`);
    const restored = run("make", [`-j${Math.max(1, availableParallelism() - 2)}`, checkTarget(plan.container)]);
    if (restored.code !== 0) return { ok: false, detail: `${why}\nBuild after rollback failed:\n${failureLines(restored.output, 8).join("\n")}` };
    return { ok: false, detail: why };
  };

  mkdirSync(dirname(join(ROOT, plan.modulePath)), { recursive: true });
  writeFileSync(join(ROOT, plan.modulePath), plan.moduleText);
  for (const path of plan.removedSources) rmSync(join(ROOT, path), { force: true });
  writeFileSync(join(ROOT, plan.configPath), plan.configText);
  if (plan.flagConfig) writeFileSync(join(ROOT, plan.flagConfig.path), plan.flagConfig.text);
  log(`wrote ${plan.modulePath}, ${plan.configPath}; removed ${plan.removedSources.join(", ")}`);

  log(`make ${splitTarget(plan.container)}`);
  const split = run("make", [splitTarget(plan.container)]);
  if (split.code !== 0) return rollback(`make ${splitTarget(plan.container)} failed:\n${failureLines(split.output, 8).join("\n")}`);
  const resplit = readFileSync(join(ROOT, plan.configPath), "utf8");
  if (resplit !== plan.configText) {
    const planned = new Set(plan.configText.split("\n")), produced = new Set(resplit.split("\n"));
    const changes = [
      ...plan.configText.split("\n").filter((line) => !produced.has(line)).map((line) => `  - ${line.trim()}`),
      ...resplit.split("\n").filter((line) => !planned.has(line)).map((line) => `  + ${line.trim()}`),
    ];
    return rollback(`the re-split rewrote ${plan.configPath}: the generator does not preserve the module\n${changes.slice(0, 12).join("\n")}`);
  }

  if (plan.flagConfig) {
    const dir = join(ROOT, "build/foldTranslationUnit", plan.container.id, plan.stem.slice(`${MODULE_DIR}/`.length), "baseline");
    try {
      const compiled = compileSource(plan.modulePath, dir, "baseline", {
        assemble: true, useOverrides: false, containerKind: plan.container.kind,
      });
      const results = plan.members.filter(m => !plan.stubs.includes(m.name)).map(member => {
        const result = compareFunction(member.name, { container: plan.container, objectPath: compiled.object! });
        writeFileSync(join(dir, `${member.name}.diff.txt`), renderDiff(result, false).join("\n") + "\n");
        return { member: member.name, verdict: result.verdict };
      });
      writeFileSync(join(dir, "report.json"), JSON.stringify({
        owner: plan.flagConfig.owner, sourceHash: createHash("sha256").update(readFileSync(join(ROOT, plan.modulePath))).digest("hex"),
        preprocessedHash: createHash("sha256").update(readFileSync(compiled.preprocessed)).digest("hex"),
        objectHash: createHash("sha256").update(readFileSync(compiled.object!)).digest("hex"),
        flags: compiled.cc1Flags, retainedFlags: plan.flagConfig.flags, members: results,
      }, null, 2) + "\n");
      log(`whole-unit baseline: ${results.filter(r => r.verdict === "match").length}/${results.length} match; ${posix.relative(ROOT, dir)}/report.json`);
      if (results.some(r => r.verdict !== "match" && r.verdict !== "mismatch")) return rollback("baseline is undetermined, so the existing setting cannot be assessed");
      if (results.every(r => r.verdict === "match")) return rollback("every member matches baseline: retire the unnecessary override before folding");
    } catch (error) {
      return rollback(`whole-unit baseline control failed: ${(error as Error).message}`);
    }
  }

  if (plan.relabel.length > 0) {
    const object = posix.join(plan.container.paths.objDir, `${plan.stem}.c.o`);
    log(`make ${object} (to read the module's cc1 labels)`);
    const compiled = run("make", [object]);
    if (compiled.code !== 0) return rollback(`make ${object} failed:\n${failureLines(compiled.output, 8).join("\n")}`);
    const asm = readFileSync(join(ROOT, object.replace(/\.c\.o$/, ".s")), "utf8");
    let text = plan.moduleText;
    try {
      for (const { member, labels } of plan.relabel) {
        const now = labelDefinitions(asm, member);
        if (now.length !== labels.length) return rollback(`${member} defines ${now.length} cc1 label(s) in the module but ${labels.length} on its own; its code changed`);
        const mapping = new Map(labels.map((label, i) => [label, now[i]!]));
        text = renumberAsmLabels(text, member, mapping);
        log(`${member}: asm labels ${[...mapping].filter(([a, b]) => a !== b).map(([a, b]) => `${a} -> ${b}`).join(", ") || "unchanged"}`);
      }
    } catch (error) {
      return rollback((error as Error).message);
    }
    writeFileSync(join(ROOT, plan.modulePath), text);
  }

  log(`make ${checkTarget(plan.container)}`);
  const check = run("make", [`-j${Math.max(1, availableParallelism() - 2)}`, checkTarget(plan.container)]);
  /* Preserve full diagnostics before rollback regenerates the failed link. */
  const reportDir = join(ROOT, "build/foldTranslationUnit", plan.container.id, plan.stem.slice(`${MODULE_DIR}/`.length));
  mkdirSync(reportDir, { recursive: true });
  writeFileSync(join(reportDir, "check.log"), check.output);
  /* The tools' own diagnostics name the cause; make's tail only names the target. */
  if (check.code !== 0) {
    /* A linked hash alone cannot identify the declaration/context change that
       broke a whole-unit fold. Preserve the existing member oracle's evidence
       before rollback removes the module and overwrites its object. This is
       diagnostic only: the failed container still unconditionally rolls back. */
    writeFileSync(join(reportDir, "failed-source.c"), readFileSync(join(ROOT, plan.modulePath)));
    const objectPath = posix.join(plan.container.paths.objDir, `${plan.stem}.c.o`);
    const memberReports: Array<{ member: string; verdict: string; detail?: string }> = [];
    if (existsSync(join(ROOT, objectPath)) &&
        statSync(join(ROOT, objectPath)).mtimeMs >= statSync(join(ROOT, plan.modulePath)).mtimeMs) {
      writeFileSync(join(reportDir, "failed-object.o"), readFileSync(join(ROOT, objectPath)));
      for (const member of plan.members.filter((m) => !plan.stubs.includes(m.name))) {
        try {
          const result = compareFunction(member.name, { container: plan.container });
          memberReports.push({ member: member.name, verdict: result.verdict });
          if (result.verdict !== "match") {
            writeFileSync(join(reportDir, `${member.name}.diff.txt`), renderDiff(result, false).join("\n") + "\n");
          }
        } catch (error) {
          memberReports.push({ member: member.name, verdict: "undetermined", detail: (error as Error).message });
        }
      }
      writeFileSync(join(reportDir, "members.json"), JSON.stringify(memberReports, null, 2) + "\n");
      const failed = memberReports.filter((member) => member.verdict !== "match");
      log(`Failed-link member oracle: ${memberReports.length - failed.length}/${memberReports.length} exact; ${failed.map((member) => `${member.member}: ${member.verdict}`).join(", ") || "member text is exact; inspect linked placement/data"}`);
      log(`Preserved failed source/object and member evidence in ${posix.relative(ROOT, reportDir)}`);
    }
    return rollback(`make ${checkTarget(plan.container)} failed:\n${failureLines(check.output).join("\n")}`);
  }

  /* A carried stub assembles the original disassembly; the container check covers it. */
  for (const member of plan.members.filter((m) => !plan.stubs.includes(m.name))) {
    try {
      const verdict = compareFunction(member.name, { container: plan.container }).verdict;
      if (verdict !== "match") return rollback(`${member.name}: the oracle says ${verdict} through the module object`);
    } catch (error) {
      return rollback(`${member.name}: ${(error as Error).message}`);
    }
  }
  const carried = plan.stubs.length > 0 ? `, ${plan.stubs.length} dead stub(s) carried` : "";
  return { ok: true, detail: `${plan.stem}: ${plan.members.length - plan.stubs.length} member(s) byte-identical${carried}; ${plan.container.id} matches` };
}

function main(): void {
  const args = process.argv.slice(2);
  const value = (flag: string) => {
    const index = args.indexOf(flag);
    return index >= 0 ? args[index + 1] : undefined;
  };
  const container = value("--container"), module = value("--module"), members = value("--members");
  if (!container || !module || !members) {
    console.error("Usage: npx tsx tools/build/foldTranslationUnit.ts --container <id> --module <name> --members A,B,... [--summary <text>] [--write]");
    process.exit(2);
  }
  let plan: FoldPlan;
  try {
    plan = planFold({ container, module, members: members.split(",").map((m) => m.trim()).filter(Boolean), summary: value("--summary") });
  } catch (error) {
    if (error instanceof Refusal) {
      console.error(`REFUSED: ${error.message}`);
      process.exit(1);
    }
    throw error;
  }
  console.log(`Fold ${plan.members.length} function(s) into ${plan.modulePath}:`);
  for (const member of plan.members) console.log(`  ${hex(member.vram)}  ${member.name}${plan.stubs.includes(member.name) ? "  (dead stub, carried)" : ""}`);
  console.log(`Removes: ${plan.removedSources.join(", ") || "nothing"}`);
  console.log(`Config: ${plan.configPath} gets one [${hex(plan.units[0]!.startRom)}, c, ${plan.stem}] line`);
  for (const note of plan.notes) console.log(`  note: ${note}`);
  if (!args.includes("--write")) {
    console.log("\n--- module source ---");
    console.log(plan.moduleText);
    console.log("(dry run — pass --write to apply, re-split, build and verify)");
    return;
  }
  const result = applyFold(plan);
  console.log(result.ok ? `OK: ${result.detail}` : `FAILED: ${result.detail}`);
  process.exitCode = result.ok ? 0 : 1;
}

if (process.argv[1]?.endsWith("foldTranslationUnit.ts")) main();
