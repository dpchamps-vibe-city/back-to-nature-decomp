/**
 * deriveRodataSplits.ts — Derive the game-rodata subsegment block of a
 * container's splat config from first principles, so it is never hand-edited.
 *
 * A switch function's jump table lives in the rodata window. While the
 * function is an INCLUDE_ASM stub, the table must be extracted as asm data
 * (a generic `rodata` subsegment); once the function is compiled as C, its
 * translation unit emits the table itself and the range must be attributed
 * to the TU (`.rodata` subsegment). The attribution extent must equal the
 * .rodata section size of the TU's object file — the original may carry
 * alignment pad words after the table (e.g. 4 zero bytes) that the compiled
 * TU does not emit, and those must remain generic asm data or the whole
 * binary shifts.
 *
 * Derivation, per compiled C translation unit with a non-empty .o .rodata
 * section (a per-function file and a module alike):
 *   - table address A: the smallest lui/addiu-formed constant inside the
 *     rodata window referenced by any compiled member's original code (the
 *     function must also contain a `jr` — the tablejump);
 *   - extent S: the .rodata section size of the unit's object file, and every
 *     member's table must lie inside [A, A+S);
 *   - attribution line `[A, .rodata, <unit stem>]`, and a generic `[A+S, rodata]`
 *     residue line when A+S does not coincide with the next subsegment.
 * A relocation-free literal-only section may instead be placed at a unique
 * original lui/addiu reference whose bytes equal the entire emitted section.
 * This bounded fallback never treats an unverified string address as a table.
 * Stub functions get no attribution: their tables stay in generic rodata.
 *
 * This is per *container*, not per project. Every container the project
 * builds — the PS-X EXE and each overlay member — has its own rodata window,
 * its own symbol table and its own object tree, and a container whose
 * attribution is missing cannot link a single jump-table function as C.
 *
 * Usage:
 *   npx tsx tools/build/deriveRodataSplits.ts                    # check exe
 *   npx tsx tools/build/deriveRodataSplits.ts --container ovl_10
 *   npx tsx tools/build/deriveRodataSplits.ts --all
 *   npx tsx tools/build/deriveRodataSplits.ts --all --write
 */

import { readFileSync, writeFileSync, existsSync } from "fs";
import { join } from "path";
import { elf32Sections } from "../lib/elfSections.js";
import { ROOT } from "../lib/psxExeInfo.ts";
import {
  EXE_CONTAINER_ID,
  containerPath,
  containerTargetPath,
  loadContainers,
  requireContainer,
  vramToRom,
  type Container,
} from "../lib/container.ts";
import { loadTranslationUnits, type TranslationUnit } from "../lib/translationUnits.ts";
import { parseFuncExtents, scanTableAddr, type FuncExtent } from "../lib/jumpTables.ts";


interface SubsegLine {
  index: number;
  raw: string;
  addr: number;
  kind: string;
  name?: string;
}

interface Attribution {
  addr: number;
  end: number;
  name: string;
}

/** The contiguous run of rodata/.rodata subsegments a container's config owns. */
export interface RodataWindow {
  start: number;
  endAddr: number;
  members: SubsegLine[];
}


export function parseSubsegments(lines: string[]): SubsegLine[] {
  const out: SubsegLine[] = [];
  for (let i = 0; i < lines.length; i++) {
    const m = lines[i].match(/^(\s*)- \[(0x[0-9A-Fa-f]+), ([^,\]]+)(?:, ([^,\]]+))?/);
    if (!m) continue;
    out.push({
      index: i,
      raw: lines[i],
      addr: parseInt(m[2], 16),
      kind: m[3].trim(),
      name: m[4]?.trim(),
    });
  }
  return out;
}

/** The managed window: the contiguous run of rodata/.rodata subsegments
 *  starting at the first one, ending at the first entry of another kind. */
export function findWindow(subsegs: SubsegLine[]): RodataWindow {
  const first = subsegs.findIndex((s) => s.kind === "rodata" || s.kind === ".rodata");
  if (first < 0) throw new Error("no rodata subsegments found");
  const members: SubsegLine[] = [];
  let i = first;
  for (; i < subsegs.length; i++) {
    const k = subsegs[i].kind;
    if (k !== "rodata" && k !== ".rodata") break;
    members.push(subsegs[i]);
  }
  if (i >= subsegs.length) throw new Error("rodata window has no terminating subsegment");
  return { start: members[0].addr, endAddr: subsegs[i].addr, members };
}

/**
 * The rodata block of a container's config as it stands today.
 *
 * `bootstrapOverlay.ts` regenerates an overlay's whole config, and this block
 * is the one part of it that this module owns. Handing the block back lets the
 * bootstrap carry the derived attributions across a re-split instead of
 * silently reverting them, which would deadlock the self-healing relink.
 */
export function existingRodataBlock(container: Container): string[] | null {
  const path = containerPath(container, "splat");
  if (!existsSync(path)) return null;
  try {
    const window = findWindow(parseSubsegments(readFileSync(path, "utf-8").split("\n")));
    return window.members.map((m) => m.raw);
  } catch {
    return null;
  }
}

/** Only a bounded straight-line address formation is accepted for literal
 * ownership: lui, at most four addiu/addu/or instructions, then addiu using
 * the still-unmodified high register. Calls, branches and unknown instructions
 * stop the proof. The entire relocation-free section must match at exactly
 * one referenced original address; offsets into a pool remain undetermined. */
function literalRodataAddress(
  buf: Buffer, container: Container, members: FuncExtent[],
  window: RodataWindow, data: Buffer,
): number | null {
  const matches = new Set<number>();
  for (const fn of members) {
    for (let va = fn.start; va + 4 < fn.end; va += 4) {
      const rom = vramToRom(container, va);
      if (rom < 0 || rom + 4 > buf.length) break;
      const hi = buf.readUInt32LE(rom);
      if (hi >>> 26 !== 0x0f || ((hi >>> 21) & 31) !== 0) continue;
      const base = (hi >>> 16) & 31;
      if (base === 0) continue;
      for (let step = 1; step <= 5 && va + step * 4 < fn.end; step++) {
        const at = vramToRom(container, va + step * 4);
        if (at < 0 || at + 4 > buf.length) break;
        const word = buf.readUInt32LE(at), op = word >>> 26;
        const rs = (word >>> 21) & 31, rt = (word >>> 16) & 31;
        if (op === 0x09 && rs === base) {
          const address = (((hi & 0xffff) << 16) + ((word << 16) >> 16)) >>> 0;
          const offset = vramToRom(container, address);
          if (offset >= window.start && offset + data.length <= window.endAddr &&
              buf.subarray(offset, offset + data.length).equals(data)) matches.add(offset);
        }
        const alu = op === 0 && [0x21, 0x25].includes(word & 63);
        if (op !== 0x09 && !alu) break;
        const destination = alu ? (word >>> 11) & 31 : rt;
        if (destination === base) break;
      }
    }
  }
  return matches.size === 1 ? [...matches][0]! : null;
}

/** Members of a unit its source compiles rather than hands to the assembler. */
function compiledMembers(unit: TranslationUnit): string[] {
  const src = join(ROOT, unit.sourcePath);
  if (!existsSync(src)) return [];
  const text = readFileSync(src, "utf-8");
  return unit.members.map((member) => member.name).filter((fn) =>
    !new RegExp(`INCLUDE_ASM\\([^)]*,\\s*${fn}\\s*\\)`).test(text));
}

/** The derived rodata block for one container, and whether the config agrees. */
export interface Derivation {
  container: Container;
  window: RodataWindow;
  attributions: Attribution[];
  current: string[];
  derived: string[];
  consistent: boolean;
}

export function deriveContainer(container: Container): Derivation {
  const buf = readFileSync(containerTargetPath(container));
  const yamlLines = readFileSync(containerPath(container, "splat"), "utf-8").split("\n");
  const subsegs = parseSubsegments(yamlLines);
  const window = findWindow(subsegs);
  const funcs = new Map(parseFuncExtents(container).map((fn) => [fn.name, fn]));

  /* Ownership is the translation unit's. A module object pools every member's
     tables into one .rodata section, in emission order, so the attribution is
     one line named for the unit: it starts at the lowest table any compiled
     member references and runs for the object's .rodata size. Every member's
     table has to fall inside that extent — a table outside it means the pooled
     section is not one contiguous block of the original, and assigning it to
     the first member would hide that. */
  const attributions: Attribution[] = [];
  for (const unit of loadTranslationUnits(container)) {
    const compiled = compiledMembers(unit);
    if (compiled.length === 0) continue;
    const obj = join(ROOT, unit.objectPath);
    if (!existsSync(obj)) {
      throw new Error(`${unit.segmentName}: compiled C but ${unit.objectPath} is missing — run make first`);
    }
    const sections = elf32Sections(readFileSync(obj));
    const rodata = sections.get(".rodata");
    const size = rodata?.size ?? 0;
    if (!rodata || size === 0) continue;
    const tables: Array<{ name: string; vram: number }> = [];
    for (const name of compiled) {
      const fn = funcs.get(name);
      const vramAddr = fn ? scanTableAddr(buf, container, fn, window.start, window.endAddr) : null;
      if (vramAddr !== null) tables.push({ name, vram: vramAddr });
    }
    let addr: number;
    if (tables.length === 0) {
      const relocated = [...sections.values()].some((section) =>
        [4, 9].includes(section.type) && section.info === rodata.index && section.size > 0);
      const literal = relocated ? null : literalRodataAddress(buf, container,
        compiled.flatMap((name) => funcs.has(name) ? [funcs.get(name)!] : []), window, rodata.data);
      if (literal === null) {
        throw new Error(
          `${unit.segmentName}: object emits 0x${size.toString(16)} bytes of .rodata but no ` +
            `jump table or unique referenced relocation-free byte match establishes its base — undetermined`,
        );
      }
      addr = literal;
    } else {
      addr = vramToRom(container, Math.min(...tables.map((table) => table.vram)));
    }
    for (const table of tables) {
      const at = vramToRom(container, table.vram);
      if (at >= addr + size) {
        throw new Error(
          `${unit.segmentName}: ${table.name}'s table at 0x${hex(at)} lies outside the unit's pooled .rodata ` +
            `0x${hex(addr)}..0x${hex(addr + size)} — ownership undetermined`,
        );
      }
    }
    attributions.push({ addr, end: addr + size, name: unit.segmentName });
  }
  attributions.sort((a, b) => a.addr - b.addr);

  for (let i = 0; i + 1 < attributions.length; i++) {
    if (attributions[i].end > attributions[i + 1].addr) {
      throw new Error(
        `overlap: ${attributions[i].name} ends 0x${attributions[i].end.toString(16)} past ` +
          `${attributions[i + 1].name} at 0x${attributions[i + 1].addr.toString(16)}`,
      );
    }
  }

  // Build the derived window: generic head, attributions, generic residues.
  const indent = window.members[0].raw.match(/^(\s*)/)![1];
  const derived: string[] = [];
  let cursor = window.start;
  for (const a of attributions) {
    if (a.addr > cursor) derived.push(`${indent}- [0x${hex(cursor)}, rodata]`);
    derived.push(`${indent}- [0x${hex(a.addr)}, .rodata, ${a.name}]`);
    cursor = a.end;
  }
  if (cursor < window.endAddr) derived.push(`${indent}- [0x${hex(cursor)}, rodata]`);
  /* A window that starts where it ends still needs one line to exist at all. */
  if (derived.length === 0) derived.push(`${indent}- [0x${hex(window.start)}, rodata]`);

  const current = window.members.map((m) => m.raw);
  const consistent = current.length === derived.length && current.every((l, i) => l.trim() === derived[i].trim());

  return { container, window, attributions, current, derived, consistent };
}

function applyDerivation(d: Derivation): void {
  const path = containerPath(d.container, "splat");
  const yamlLines = readFileSync(path, "utf-8").split("\n");
  const before = yamlLines.slice(0, d.window.members[0].index);
  const after = yamlLines.slice(d.window.members[d.window.members.length - 1].index + 1);
  writeFileSync(path, [...before, ...d.derived, ...after].join("\n"));
}

function main(): void {
  const argv = process.argv.slice(2);
  const write = argv.includes("--write");
  const all = argv.includes("--all");
  const idx = argv.indexOf("--container");
  const ids = all
    ? loadContainers().map((c) => c.id)
    : [idx >= 0 ? argv[idx + 1] : EXE_CONTAINER_ID];

  let drift = false;
  for (const id of ids) {
    const container = requireContainer(id);
    const d = deriveContainer(container);
    const label = `${container.id} rodata window (0x${hex(d.window.start)}..0x${hex(d.window.endAddr)})`;

    if (d.consistent) {
      console.log(
        `${label}: ${d.attributions.length} attribution(s) — ${container.paths.splat} is consistent with the derivation`,
      );
      continue;
    }

    drift = true;
    console.log(`derived rodata window differs from ${container.paths.splat}:`);
    console.log("  current:");
    for (const l of d.current) console.log(`    ${l.trim()}`);
    console.log("  derived:");
    for (const l of d.derived) console.log(`    ${l.trim()}`);

    if (write) {
      applyDerivation(d);
      console.log(`wrote ${container.paths.splat} — re-run \`make split\` and \`make check\``);
    }
  }

  if (drift && !write) {
    console.log("run with --write to apply the derived block");
    process.exitCode = 1;
  }
}

function hex(n: number): string {
  return n.toString(16).toUpperCase().replace(/^0X/, "");
}

if (process.argv[1]?.endsWith("deriveRodataSplits.ts")) main();
