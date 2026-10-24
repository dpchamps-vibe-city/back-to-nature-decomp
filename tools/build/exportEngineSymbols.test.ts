import assert from "node:assert/strict";
import test from "node:test";
import { selectEngineExports, type ExportedSymbol } from "./exportEngineSymbols.js";

const symbol = (name: string, address: number, kind = "T"): ExportedSymbol => ({ name, address, kind, source: "elf" });
const noSlots = (): boolean => false;

test("a function keeps its name at an address it shares with a compiler marker", () => {
  const { exported, ambiguousNames } = selectEngineExports([
    symbol("__gnu_compiled_c", 0x80012a34, "t"),
    symbol("Rand", 0x80012a34),
    symbol("__gnu_compiled_c", 0x80012a68, "t"),
    symbol("func_80012A68", 0x80012a68),
  ], noSlots);
  assert.deepEqual(exported.map((s) => s.name), ["Rand", "func_80012A68"]);
  assert.deepEqual(ambiguousNames, ["__gnu_compiled_c"]);
});

test("data, section markers and overlay slots keep their existing treatment", () => {
  const { exported } = selectEngineExports([
    symbol("main_RODATA_START", 0x80010000),
    symbol("D_80010000", 0x80010000),
    symbol("main_BSS_END", 0x8005e800),
    symbol("GsIDMATRIX", 0x800a4008, "D"),
    symbol("ovl_func", 0x800b8000),
    symbol("header_VRAM", 0x00000000, "D"),
  ], (address) => address >= 0x800b8000 && address < 0x800c0000);
  assert.deepEqual(exported.map((s) => s.name), ["D_80010000", "main_BSS_END", "GsIDMATRIX"]);
});
