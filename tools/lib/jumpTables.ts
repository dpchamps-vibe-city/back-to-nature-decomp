/**
 * jumpTables.ts — which jump table a function's original code indexes.
 *
 * Shared by the rodata derivation, which attributes each compiled unit's
 * pooled `.rodata` to the configuration, and by the per-function oracle, which
 * needs one member's own table base when it compares that member compiled on
 * its own rather than as part of its whole unit.
 */

import { existsSync, readFileSync } from "fs";
import { containerPath, containerTargetPath, vramToRom, type Container } from "./container.js";
import { loadSubsegments } from "./symbolIndex.js";

export interface FuncExtent {
  name: string;
  start: number;
  end: number;
}

export function parseFuncExtents(container: Container): FuncExtent[] {
  const entries: { name: string; addr: number; size?: number }[] = [];
  const path = containerPath(container, "symbolAddrs");
  if (!existsSync(path)) return [];
  for (const line of readFileSync(path, "utf-8").split("\n")) {
    const m = line.trim().match(/^(\w+)\s*=\s*(0x[0-9A-Fa-f]+).*type:func/i);
    if (!m) continue;
    const size = line.match(/size:(0x[0-9A-Fa-f]+|\d+)/i);
    entries.push({
      name: m[1],
      addr: parseInt(m[2], 16),
      size: size ? Number(size[1]) : undefined,
    });
  }
  entries.sort((a, b) => a.addr - b.addr);
  /* The last function ends where the container's image does. Without the
     clamp the scan runs off the end of a 16 KiB overlay member. */
  const imageEnd = container.loadAddr + container.payloadSize;
  return entries.map((e, i) => ({
    name: e.name,
    start: e.addr,
    /* A declared `size:` wins over the next symbol's address. The two disagree
       exactly where the disassembler named a branch target inside a function
       as a function of its own, and a jump table's entries are precisely those
       addresses — so trusting the next symbol truncates the extent to the
       first table entry and makes the table unrecognisable. */
    end: e.size
      ? e.addr + e.size
      : i + 1 < entries.length
        ? entries[i + 1].addr
        : Math.min(e.addr + 0x10000, imageEnd),
  }));
}

/**
 * The lowest jump-table base this function references.
 *
 * Two conditions, both necessary. The address must be formed by a lui/addiu
 * pair inside the function and land in the rodata window — that is the
 * reference. And the words at that address must read as a table of this
 * function's own code labels — that is what makes it a *jump table* rather
 * than a string or a constant pool entry.
 *
 * The content test is not a refinement, it is the whole difference between a
 * container whose rodata window holds only jump tables and one whose window is
 * the entire read-only section. In the PS-X EXE the game-rodata window happens
 * to contain nothing else, so "the lowest referenced constant" was right by
 * accident; in an overlay member the same rule picks the function's first
 * format string and attributes 20 bytes of somebody else's data to it.
 */
export function scanTableAddr(
  buf: Buffer,
  container: Container,
  fn: FuncExtent,
  windowLo: number,
  windowHi: number,
): number | null {
  const luiVal: (number | null)[] = new Array(32).fill(null);
  let sawJr = false;
  const candidates: number[] = [];
  const loVa = romToVramLocal(windowLo);
  const hiVa = romToVramLocal(windowHi);
  for (let va = fn.start; va < fn.end; va += 4) {
    const rom = vramToRom(container, va);
    if (rom < 0 || rom + 4 > buf.length) break;
    const w = buf.readUInt32LE(rom);
    const op = w >>> 26;
    const rs = (w >>> 21) & 31;
    const rt = (w >>> 16) & 31;
    const simm = ((w & 0xffff) << 16) >> 16;
    if (op === 0x0f && rs === 0) {
      luiVal[rt] = (w & 0xffff) << 16;
      continue;
    }
    if (op === 0x09) {
      if (luiVal[rs] !== null) {
        const c = (luiVal[rs]! + simm) >>> 0;
        if (c >= loVa && c < hiVa) candidates.push(c);
      }
      if (rt !== rs) luiVal[rt] = null;
      continue;
    }
    if (op === 0 && (w & 0x3f) === 0x08) sawJr = true;
    // Any other write to a register invalidates its tracked lui; keep the
    // tracker conservative rather than exhaustive.
    if (op === 0 && ((w >>> 11) & 31) !== 0) luiVal[(w >>> 11) & 31] = null;
    else if (op !== 0 && op !== 0x2b && op !== 0x29 && op !== 0x28 && rt !== 0) luiVal[rt] = null;
  }
  if (!sawJr) return null;
  const tables = candidates.filter((addr) => jumpTableLength(buf, container, addr, hiVa, fn) >= 2);
  return tables.length === 0 ? null : Math.min(...tables);

  function romToVramLocal(rom: number): number {
    return rom - container.payloadOffset + container.loadAddr;
  }
}

/** How many consecutive words at `addr` read as code labels inside `fn`. */
function jumpTableLength(
  buf: Buffer,
  container: Container,
  addr: number,
  windowHiVa: number,
  fn: FuncExtent,
): number {
  let n = 0;
  for (let va = addr; va < windowHiVa; va += 4) {
    const rom = vramToRom(container, va);
    if (rom < 0 || rom + 4 > buf.length) break;
    const target = buf.readUInt32LE(rom);
    if ((target & 3) !== 0) break;
    if (target < fn.start || target >= fn.end) break;
    n++;
  }
  return n;
}

/**
 * The jump table one function's original code indexes, or null.
 *
 * The rodata window is the configured run of rodata/.rodata subsegments; the
 * function's extent comes from the symbol table, a declared `size:` winning.
 */
export function memberJumpTableAddress(container: Container, functionName: string): number | null {
  const fn = parseFuncExtents(container).find((entry) => entry.name === functionName);
  if (!fn) return null;
  const segments = loadSubsegments(container);
  const first = segments.findIndex((segment) => segment.type === "rodata" || segment.type === ".rodata");
  if (first < 0) return null;
  let end = first;
  while (end < segments.length && (segments[end]!.type === "rodata" || segments[end]!.type === ".rodata")) end++;
  if (end >= segments.length) return null;
  return scanTableAddr(readFileSync(containerTargetPath(container)), container, fn, segments[first]!.rom, segments[end]!.rom);
}
