/**
 * exportEngineSymbols.ts — the PS-X EXE's symbol table, as absolute definitions.
 *
 * Deliverable 8 of plans/overlay-decompilation-enablement.md. When an overlay
 * links, its calls to `0x8001FABC` must resolve, but the EXE is not in that
 * link. Engine symbols enter it as absolute address definitions instead — the
 * same mechanism splat's `undefined_syms_auto.txt` uses — populated from the
 * EXE's own symbol table.
 *
 * Data symbols are not optional. Overlays resolve 251 distinct PS-X EXE
 * `.data`/`.sdata` addresses across 1,725 sites; an export list carrying only
 * functions links cleanly and leaves every global reference wrong.
 *
 * The list is generated, never curated. The measured 246 function entry points
 * and 251 data globals are floors, not ceilings, so the whole symbol table is
 * exported and the build depends on it: rename a function in `src/` and every
 * overlay relinks. The failure mode being designed out is quiet — a stale
 * export produces an overlay that links cleanly and calls the wrong function.
 *
 * Usage:
 *   npx tsx tools/build/exportEngineSymbols.ts --write
 */

import { execFileSync } from "child_process";
import { existsSync, mkdirSync, readFileSync, writeFileSync } from "fs";
import { dirname, join, resolve } from "path";
import { fileURLToPath } from "url";
import { ROOT } from "../lib/psxExeInfo.js";
import {
  EXE_CONTAINER_ID,
  containerPath,
  containsVram,
  loadContainers,
  requireContainer,
  type Container,
} from "../lib/container.js";
import { ENGINE_EXPORT_PATH, loadSymbolAddresses } from "../lib/symbolIndex.js";

const NM = "mips-linux-gnu-nm";

export interface ExportedSymbol {
  name: string;
  address: number;
  kind: string;
  source: "elf" | "symbol-table";
}

/**
 * Symbols from the linked ELF — the authoritative post-link answer, because it
 * is what the EXE's own `make check` verified.
 */
function fromElf(elf: string): ExportedSymbol[] {
  if (!existsSync(elf)) return [];
  const output = execFileSync(NM, ["--defined-only", elf], { encoding: "utf-8", maxBuffer: 64 * 1024 * 1024 });
  const symbols: ExportedSymbol[] = [];
  for (const line of output.split("\n")) {
    const match = line.match(/^([0-9a-fA-F]{8})\s+(\S)\s+(\S+)$/);
    if (!match) continue;
    const [, hex, kind, name] = match;
    /* Assembler-local labels, section symbols and splat's variant suffixes are
       not an interface — and a name with a dot in it is not a linker-script
       identifier at all. */
    if (!/^[A-Za-z_][A-Za-z0-9_]*$/.test(name!)) continue;
    if (name!.startsWith(".L") || name!.startsWith("_gp_disp")) continue;
    symbols.push({ name: name!, address: Number(`0x${hex}`), kind: kind!, source: "elf" });
  }
  return symbols;
}

/**
 * Fallback for a tree that has not linked yet: the project's own symbol tables.
 *
 * This is strictly weaker — it cannot see a symbol only the link defines — so
 * it is reported as a fallback rather than used silently.
 */
function fromSymbolTables(exe: Container): ExportedSymbol[] {
  return [...loadSymbolAddresses(exe)].map(([name, address]) => ({
    name,
    address,
    kind: "?",
    source: "symbol-table" as const,
  }));
}

/** splat names an anonymous symbol after its address; a real name outranks that. */
const GENERATED_NAME = /^(?:D|B|func|jtbl|T|_)_?[0-9A-Fa-f]{8}$/;
/** splat's linker script defines section markers; they are geometry, not an interface. */
const SECTION_MARKER = /_(?:RODATA|TEXT|DATA|SDATA|SBSS|BSS)_(?:START|END)$|_VRAM(?:_END)?$|_ROM_(?:START|END)$/;

/** nm's lowercase kinds are local bindings; the symbol-table fallback has no binding. */
function isLocal(symbol: ExportedSymbol): boolean {
  return /^[a-z]$/.test(symbol.kind);
}

function better(a: ExportedSymbol, b: ExportedSymbol): ExportedSymbol {
  /* Geometry first: a section boundary that coincides with a function or a
     data label is still not what a reference to that address means. */
  const aMarker = SECTION_MARKER.test(a.name);
  const bMarker = SECTION_MARKER.test(b.name);
  if (aMarker !== bMarker) return aMarker ? b : a;
  const aGenerated = GENERATED_NAME.test(a.name);
  const bGenerated = GENERATED_NAME.test(b.name);
  if (aGenerated !== bGenerated) return aGenerated ? b : a;
  /* A file-local name cannot be what another link means by this address. */
  if (isLocal(a) !== isLocal(b)) return isLocal(a) ? b : a;
  return a.name.localeCompare(b.name) <= 0 ? a : b;
}

export interface EngineExportSelection {
  exported: ExportedSymbol[];
  /** Names withheld because they are defined at more than one address. */
  ambiguousNames: string[];
}

/**
 * One name per address, from every symbol the executable defines.
 *
 * Only addresses inside PS1 RAM are an interface an overlay can call, and only
 * addresses outside every overlay slot belong in this list: a slot address is
 * defined by whichever overlay is resident, so exporting the EXE's name for it
 * would give an overlay link two definitions of the same address — one of them
 * the wrong one.
 */
export function selectEngineExports(
  symbols: readonly ExportedSymbol[],
  inOverlaySlot: (address: number) => boolean
): EngineExportSelection {
  const inScope = symbols.filter(
    (symbol) => symbol.address >= 0x80000000 && symbol.address < 0x80200000 && !inOverlaySlot(symbol.address)
  );

  /*
   * A name defined at more than one address is not an interface.
   *
   * `__gnu_compiled_c` and friends are per-translation-unit assembler markers
   * that recur at every object's first symbol. They have to leave before any
   * address chooses its name: a marker and the function it shares an address
   * with are both hand-written names, so a later tie-break can pick the marker,
   * and dropping the marker afterwards leaves that address with no name at all.
   */
  const addressesByName = new Map<string, Set<number>>();
  for (const symbol of inScope) {
    const addresses = addressesByName.get(symbol.name) ?? new Set<number>();
    addresses.add(symbol.address);
    addressesByName.set(symbol.name, addresses);
  }
  const ambiguousNames = [...addressesByName.entries()]
    .filter(([, addresses]) => addresses.size > 1)
    .map(([name]) => name)
    .sort();
  const ambiguous = new Set(ambiguousNames);

  /*
   * splat refuses a symbol table that gives one address two names, and the
   * overlay assembly it generates is written against whichever name is here — so
   * picking the most informative one is the whole decision. Aliases are dropped
   * rather than renamed.
   */
  const byAddress = new Map<number, ExportedSymbol>();
  for (const symbol of inScope) {
    if (ambiguous.has(symbol.name)) continue;
    const existing = byAddress.get(symbol.address);
    byAddress.set(symbol.address, existing ? better(existing, symbol) : symbol);
  }

  return {
    exported: [...byAddress.values()].sort((a, b) => a.address - b.address),
    ambiguousNames,
  };
}

function main(): void {
  const args = process.argv.slice(2);
  const write = args.includes("--write");
  const check = args.includes("--check");

  const exe = requireContainer(EXE_CONTAINER_ID);
  const elfSymbols = fromElf(containerPath(exe, "builtElf"));
  const symbols = elfSymbols.length > 0 ? elfSymbols : fromSymbolTables(exe);
  const source = elfSymbols.length > 0 ? `${exe.paths.builtElf} (linked)` : "the project symbol tables (no linked ELF yet)";

  if (symbols.length === 0) {
    console.error("No engine symbols found. Build the PS-X EXE first: make");
    process.exit(1);
  }

  const overlays = loadContainers().filter((c) => c.kind === "overlay");
  const { exported, ambiguousNames } = selectEngineExports(symbols, (address) =>
    overlays.some((container) => containsVram(container, address))
  );
  const aliasesDropped = symbols.length - exported.length;

  /*
   * Bare assignments, no comment header. This one file is read by two parsers
   * with incompatible comment syntax — splat's symbol_addrs reader (`//`) and
   * GNU ld's linker script reader (`/* *' + '/`) — so it carries no comment at all,
   * and its provenance is written beside it.
   */
  const lines =
    exported.map((s) => `${s.name} = 0x${s.address.toString(16).toUpperCase().padStart(8, "0")};`).join("\n") + "\n";

  const provenance = [
    "Generated by tools/build/exportEngineSymbols.ts — do not edit engine_syms.txt by hand.",
    `Source: ${source}`,
    `Symbols: ${exported.length}`,
    "",
  ].join("\n");

  const outPath = join(ROOT, ENGINE_EXPORT_PATH);

  if (check) {
    /* A stale export links cleanly and calls the wrong function, so staleness has
       to fail the build rather than be reported. */
    const current = existsSync(outPath) ? readFileSync(outPath, "utf-8") : null;
    if (current === lines) {
      console.log(`${ENGINE_EXPORT_PATH} is current (${exported.length} symbols)`);
      process.exit(0);
    }
    console.error(
      current === null
        ? `${ENGINE_EXPORT_PATH} does not exist; every overlay link would resolve engine calls to nothing.`
        : `${ENGINE_EXPORT_PATH} is stale against ${source}. An overlay linked against it would call the wrong function.`
    );
    console.error("  Regenerate: npx tsx tools/build/exportEngineSymbols.ts --write");
    process.exit(1);
  }

  if (write) {
    mkdirSync(dirname(outPath), { recursive: true });
    writeFileSync(outPath, lines);
    writeFileSync(`${outPath.replace(/\.txt$/, "")}.provenance.txt`, provenance);
    console.log(
      `Wrote ${ENGINE_EXPORT_PATH}: ${exported.length} engine symbols from ${source} ` +
        `(${aliasesDropped} alias or out-of-scope symbol(s) dropped)`
    );
    if (ambiguousNames.length > 0) {
      console.log(
        `  ${ambiguousNames.length} name(s) defined at more than one address are not exported: ` +
          `${ambiguousNames.slice(0, 5).join(", ")}${ambiguousNames.length > 5 ? ", …" : ""}`
      );
    }
  } else {
    console.log(`Would write ${exported.length} engine symbols from ${source}`);
  }
}

if (process.argv[1] && resolve(process.argv[1]) === fileURLToPath(import.meta.url)) main();
