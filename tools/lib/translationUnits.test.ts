import assert from "node:assert/strict";
import test from "node:test";
import { mkdtempSync, rmSync, writeFileSync } from "node:fs";
import { tmpdir } from "node:os";
import { join } from "node:path";
import { ROOT } from "./psxExeInfo.js";
import { buildOriginalFunctionInventory, loadOriginalFunctionInventory, loadSubsegments, type SplatSubsegment } from "./symbolIndex.js";
import {
  buildTranslationUnits,
  flagOverrideKey,
  functionObjectPath,
  functionSourcePath,
  loadTranslationUnits,
  moduleCSegments,
  moduleStemProblem,
  translationUnitForFunction,
  translationUnitOf,
} from "./translationUnits.js";
import { auditUnits, enclosingDefinition, sourceFacts, type ObjectFacts, type UnitAuditInput } from "./translationUnitAudit.js";
import { loadContainers } from "./container.js";

/* --- synthetic layouts --------------------------------------------------- */

const VRAM = 0x80100000;
function seg(rom: number, type: string, name: string | null, next: number): SplatSubsegment {
  return { rom, vram: VRAM + rom, type, name, size: next - rom };
}

/** rodata, three game functions, a library object, data. */
function layout(cSegments: Array<[number, string]>) {
  const starts = [0x0, ...cSegments.map(([rom]) => rom), 0x60, 0x80, 0x100];
  const segments: SplatSubsegment[] = [seg(0x0, "rodata", null, starts[1]!)];
  cSegments.forEach(([rom, name], i) => segments.push(seg(rom, "c", name, cSegments[i + 1]?.[0] ?? 0x60)));
  segments.push(seg(0x60, "o", "../lib/libx/obj", 0x80), seg(0x80, "data", null, 0x100), seg(0x100, "", null, 0x100));
  const rows = [
    { rom: 0x10, vram: VRAM + 0x10, name: "f_a", size: 0x10 },
    { rom: 0x20, vram: VRAM + 0x20, name: "f_b", size: 0x20 },
    { rom: 0x40, vram: VRAM + 0x40, name: "f_c", size: 0x20 },
    { rom: 0x60, vram: VRAM + 0x60, name: "LibCall", size: 0x20 },
    { rom: 0x84, vram: VRAM + 0x84, name: "D_phantom", size: 0x10 },
  ];
  const symbols = new Map(rows.slice(0, 4).map((row) => [row.vram, { name: row.name }]));
  const inventory = buildOriginalFunctionInventory({
    container: "ovl_t", toRom: (vram) => vram - VRAM, segments, rows, symbols,
    labels: { splat: "configs/splat/ovl_t.yaml", table: "functions.csv", symbols: "symbols.txt" },
  });
  const units = buildTranslationUnits({ id: "ovl_t", srcDir: "src/overlays/ovl_t", objDir: "build/src/overlays/ovl_t" }, segments, inventory.functions);
  return { segments, inventory, units };
}

const PER_FUNCTION: Array<[number, string]> = [[0x10, "f_a"], [0x20, "f_b"], [0x40, "f_c"]];
const MODULE: Array<[number, string]> = [[0x10, "f_a"], [0x20, "modules/widget"]];

function object(functions: Array<[string, number, number]>, textSize: number, rodataSize = 0): ObjectFacts {
  return { textSize, rodataSize, stubs: new Set(), functions: functions.map(([name, offset, size]) => ({ name: name.replace(/\.\d+$/, ""), symbol: name, offset, size, nested: /\.\d+$/.test(name) })) };
}

function audit(cSegments: Array<[number, string]>, sources: Record<string, string | null>, objects?: Record<string, ObjectFacts | null>, extra: Partial<UnitAuditInput> = {}) {
  const { segments, inventory, units } = layout(cSegments);
  return auditUnits({
    container: "ovl_t", srcDir: "src/overlays/ovl_t", splatPath: "configs/splat/ovl_t.yaml",
    inventory, units, segments,
    sources: new Map(Object.entries(sources)),
    ...(objects ? { objects: new Map(Object.entries(objects)) } : {}),
    exists: (path) => path in sources && sources[path] !== null && !units.some((u) => u.sourcePath === path),
    moduleSourcesOnDisk: Object.keys(sources).filter((path) => path.includes("/modules/")),
    flagOverrideKeys: new Set(), dataSymbols: new Set(["D_shared"]),
    ...extra,
  });
}

const A = "s32 f_a(void) { return 1; }\n";
const B = "s32 f_b(void) { return D_shared; }\n";
const C = "s32 f_c(void) { return D_shared + f_b(); }\n";

test("original functions are inventoried independently of C subsegments", () => {
  const perFunction = layout(PER_FUNCTION).inventory, folded = layout(MODULE).inventory;
  assert.deepEqual(perFunction.findings, []);
  assert.deepEqual(folded.functions.map((fn) => [fn.name, fn.origin]), perFunction.functions.map((fn) => [fn.name, fn.origin]));
  assert.deepEqual(folded.functions.map((fn) => [fn.name, fn.origin]), [["f_a", "game"], ["f_b", "game"], ["f_c", "game"], ["LibCall", "library"]]);
  assert.equal(folded.outsideText, 1, "a disassembler row in data is a phantom, not a function");
});

test("conflicting names, extents and uncovered text leave the inventory undetermined", () => {
  const { segments } = layout(PER_FUNCTION);
  const base = { container: "ovl_t", toRom: (v: number) => v - VRAM, segments, labels: { splat: "s", table: "t", symbols: "y" } };
  const rows = [{ rom: 0x10, vram: VRAM + 0x10, name: "f_a", size: 0x10 }, { rom: 0x20, vram: VRAM + 0x20, name: "f_b", size: 0x18 }, { rom: 0x40, vram: VRAM + 0x40, name: "f_c", size: 0x20 }];
  const renamed = buildOriginalFunctionInventory({ ...base, rows, symbols: new Map([[VRAM + 0x10, { name: "other" }]]) });
  assert.ok(renamed.findings.some((f) => /names other/.test(f)));
  assert.ok(renamed.findings.some((f) => /f_b: functions end at ROM 0x38/.test(f)), "a gap inside a unit is uncovered text");
  assert.ok(buildOriginalFunctionInventory({ ...base, rows: null, symbols: new Map() }).findings.some((f) => /missing/.test(f)));
});

test("a module's interval holds several original functions; members keep their identity", () => {
  const { units } = layout(MODULE);
  assert.deepEqual(units.map((u) => [u.segmentName, u.members.map((m) => m.name), u.foldedModule]),
    [["f_a", ["f_a"], false], ["modules/widget", ["f_b", "f_c"], true]]);
  const module = units[1]!;
  assert.equal(module.sourcePath, "src/overlays/ovl_t/modules/widget.c");
  assert.equal(module.objectPath, "build/src/overlays/ovl_t/modules/widget.c.o");
  assert.equal(module.endVram, VRAM + 0x60, "the interval ends at the next configured subsegment, the library object");
});

test("overlay functions at the same VRAM remain distinct", () => {
  const overlays = loadContainers().filter((container) => container.kind === "overlay");
  let checked = 0;
  for (const left of overlays) for (const right of overlays) {
    if (left.id >= right.id || left.loadAddr !== right.loadAddr) continue;
    const theirs = new Map(loadOriginalFunctionInventory(right).functions.map((fn) => [fn.vram, fn]));
    for (const mine of loadOriginalFunctionInventory(left).functions) {
      const other = theirs.get(mine.vram);
      if (!other) continue;
      checked++;
      assert.notEqual(mine.name, other.name, "the container prefix keeps the names apart");
      assert.equal(translationUnitForFunction(left, mine.vram)?.members.some((m) => m.name === mine.name), true);
      assert.equal(translationUnitForFunction(right, mine.vram)?.members.some((m) => m.name === other.name), true);
      assert.equal(translationUnitOf(right, mine.name), null, "a name is looked up within its own container");
    }
  }
  assert.ok(checked > 0, "some address is used by two overlays that share a slot");
});

test("module stems must be semantic", () => {
  assert.equal(moduleStemProblem("modules/memcard_service"), null);
  assert.equal(moduleStemProblem("modules/x"), null, "a one-word singleton name is a name");
  for (const stem of ["modules/func_80012345", "modules/ovl_11_func_800ABCDE", "modules/D_80012345", "modules/module_80012345", "modules/mod_1", "modules/a/b", "func_80012345"]) {
    assert.notEqual(moduleStemProblem(stem), null, stem);
  }
});

test("flag keys mirror the Makefile: basename for a function file, a path key for a module", () => {
  assert.equal(flagOverrideKey("src/func_80012345.c"), "func_80012345");
  assert.equal(flagOverrideKey("src/overlays/ovl_11/ovl_11_func_800D1CFC.c"), "ovl_11_func_800D1CFC");
  assert.equal(flagOverrideKey("src/modules/cd_loader.c"), "modules__cd_loader");
  assert.equal(flagOverrideKey("/abs/repo/src/overlays/ovl_11/modules/shop.c"), "overlays__ovl_11__modules__shop");
});

test("source and object views agree for per-function units and for a valid module", () => {
  const perFunction = audit(PER_FUNCTION, { "src/overlays/ovl_t/f_a.c": A, "src/overlays/ovl_t/f_b.c": B, "src/overlays/ovl_t/f_c.c": C },
    { "build/src/overlays/ovl_t/f_a.c.o": object([["f_a", 0, 0x10]], 0x10), "build/src/overlays/ovl_t/f_b.c.o": object([["f_b", 0, 0x20]], 0x20), "build/src/overlays/ovl_t/f_c.c.o": object([["f_c", 0, 0x20]], 0x20) });
  assert.deepEqual(perFunction.units.map((u) => u.status), ["valid", "valid", "valid"]);
  const module = audit(MODULE, { "src/overlays/ovl_t/f_a.c": A, "src/overlays/ovl_t/modules/widget.c": B + C },
    { "build/src/overlays/ovl_t/f_a.c.o": object([["f_a", 0, 0x10]], 0x10), "build/src/overlays/ovl_t/modules/widget.c.o": object([["f_b", 0, 0x20], ["f_c", 0x20, 0x20]], 0x40) });
  assert.deepEqual(module.units.map((u) => [u.status, u.findings]), [["valid", []], ["valid", []]]);
  assert.deepEqual(module.units[1]!.warnings, [], "members share a global and one calls the other");
});

test("a semantic singleton module passes", () => {
  const result = audit([[0x10, "f_a"], [0x20, "modules/reader"], [0x40, "f_c"]],
    { "src/overlays/ovl_t/f_a.c": A, "src/overlays/ovl_t/modules/reader.c": B, "src/overlays/ovl_t/f_c.c": C });
  assert.equal(result.units[1]!.status, "valid");
  assert.deepEqual(result.units[1]!.unit.members.map((m) => m.name), ["f_b"]);
});

test("an address-derived module path fails", () => {
  const result = audit([[0x10, "f_a"], [0x20, "modules/func_80100020"]], { "src/overlays/ovl_t/f_a.c": A, "src/overlays/ovl_t/modules/func_80100020.c": B + C });
  assert.equal(result.units[1]!.status, "invalid");
  assert.match(result.units[1]!.findings.join("\n"), /encodes an address/);
});

test("duplicate ownership, a missing member, an extra symbol and wrong source order fail", () => {
  const duplicate = audit(MODULE, { "src/overlays/ovl_t/f_a.c": A + B, "src/overlays/ovl_t/modules/widget.c": B + C });
  assert.match(duplicate.findings.join("\n"), /f_b is defined by 2 configured sources/);
  assert.match(duplicate.units[0]!.findings.join("\n"), /defines f_b, an original function outside its interval/);

  const missing = audit(MODULE, { "src/overlays/ovl_t/f_a.c": A, "src/overlays/ovl_t/modules/widget.c": C });
  assert.match(missing.units[1]!.findings.join("\n"), /does not define f_b/);
  assert.equal(missing.units[1]!.defined.has("f_b"), false, "a removed definition stops counting as decompiled");

  const reordered = audit(MODULE, { "src/overlays/ovl_t/f_a.c": A, "src/overlays/ovl_t/modules/widget.c": C + B });
  assert.match(reordered.units[1]!.findings.join("\n"), /defines f_c before f_b/);

  const extra = audit(MODULE, { "src/overlays/ovl_t/f_a.c": A, "src/overlays/ovl_t/modules/widget.c": B + C },
    { "build/src/overlays/ovl_t/f_a.c.o": object([["f_a", 0, 0x10]], 0x10), "build/src/overlays/ovl_t/modules/widget.c.o": object([["f_b", 0, 0x20], ["helper", 0x20, 0x8], ["f_c", 0x28, 0x20]], 0x48) });
  assert.match(extra.units[1]!.findings.join("\n"), /emits helper, which is not a member/);
  assert.match(extra.units[1]!.findings.join("\n"), /places f_c at \.text\+0x28/);
});

test("a definition past an intervening segment is not silently swallowed", () => {
  /* f_c sits after the library object in this layout, so a module that ends at
     the object cannot own it even if its source defines it. */
  const { segments, inventory } = layout(MODULE);
  const moved = inventory.functions.map((fn) => fn.name === "f_c" ? { ...fn, rom: 0x90, vram: VRAM + 0x90 } : fn);
  const units = buildTranslationUnits({ id: "ovl_t", srcDir: "src/overlays/ovl_t", objDir: "build/src/overlays/ovl_t" }, segments, moved);
  const result = auditUnits({
    container: "ovl_t", srcDir: "src/overlays/ovl_t", splatPath: "s", inventory: { functions: moved, findings: [] }, units, segments,
    sources: new Map([["src/overlays/ovl_t/f_a.c", A], ["src/overlays/ovl_t/modules/widget.c", B + C]]),
    exists: () => false, moduleSourcesOnDisk: ["src/overlays/ovl_t/modules/widget.c"], flagOverrideKeys: new Set(), dataSymbols: new Set(),
  });
  assert.deepEqual(units[1]!.members.map((m) => m.name), ["f_b"]);
  assert.match(result.units[1]!.findings.join("\n"), /defines f_c, an original function outside its interval/);
});

test("orphan modules and stale per-function sources fail", () => {
  const orphan = audit(PER_FUNCTION, { "src/overlays/ovl_t/f_a.c": A, "src/overlays/ovl_t/f_b.c": B, "src/overlays/ovl_t/f_c.c": C, "src/overlays/ovl_t/modules/loose.c": B });
  assert.match(orphan.findings.join("\n"), /modules\/loose\.c is an orphan module/);
  const stale = audit(MODULE, { "src/overlays/ovl_t/f_a.c": A, "src/overlays/ovl_t/modules/widget.c": B + C, "src/overlays/ovl_t/f_b.c": B });
  assert.match(stale.units[1]!.findings.join("\n"), /f_b\.c remains beside .*widget\.c: a stale per-function source/);
});

test("a module that still hands a member to the assembler is a partial fold", () => {
  const partial = audit(MODULE, { "src/overlays/ovl_t/f_a.c": A, "src/overlays/ovl_t/modules/widget.c": 'INCLUDE_ASM("asm", f_b);\n' + C });
  assert.equal(partial.units[1]!.stubbed.has("f_b"), true);
  assert.match(partial.units[1]!.findings.join("\n"), /partial fold/);
});

test("a member's flag override blocks the module instead of silently not applying", () => {
  const result = audit(MODULE, { "src/overlays/ovl_t/f_a.c": A, "src/overlays/ovl_t/modules/widget.c": B + C }, undefined, { flagOverrideKeys: new Set(["f_c"]) });
  assert.match(result.units[1]!.findings.join("\n"), /still overrides f_c/);
});

test("a nested function is attributed to its own identity without stealing its parent's", () => {
  const carrier = "/* f_b is nested in f_c */\n";
  const parent = "s32 f_c(void) {\n  auto s32 inner(void) __asm__(\"f_b\");\n  s32 inner(void) { return 2; }\n  return inner();\n}\n";
  const result = audit(PER_FUNCTION, { "src/overlays/ovl_t/f_a.c": A, "src/overlays/ovl_t/f_b.c": carrier, "src/overlays/ovl_t/f_c.c": parent },
    { "build/src/overlays/ovl_t/f_a.c.o": object([["f_a", 0, 0x10]], 0x10), "build/src/overlays/ovl_t/f_b.c.o": object([], 0), "build/src/overlays/ovl_t/f_c.c.o": object([["f_b.3", 0, 0x20], ["f_c", 0x20, 0x20]], 0x40) });
  assert.deepEqual(result.units.map((u) => u.status), ["valid", "valid", "valid"]);
  assert.equal(result.units[1]!.defined.has("f_b"), true);
  const facts = sourceFacts(parent);
  assert.deepEqual(facts.definitions.map((d) => [d.name, d.nested, d.parent]), [["f_c", false, null], ["f_b", true, "f_c"]]);
  assert.equal(enclosingDefinition(facts.definitions, 3)?.name, "f_b");
  assert.equal(enclosingDefinition(facts.definitions, 4)?.name, "f_c");
});

test("generators preserve a module and reach a fixed point", () => {
  const functions = [{ name: "f_a", address: 1 }, { name: "f_b", address: 2 }, { name: "f_c", address: 3 }, { name: "f_d", address: 4 }];
  const modules = [{ stem: "modules/widget", names: new Set(["f_b", "f_c", "helper"]) }];
  const once = moduleCSegments(functions, modules).map((s) => [s.address, s.name]);
  assert.deepEqual(once, [[1, "f_a"], [2, "modules/widget"], [4, "f_d"]]);
  assert.deepEqual(moduleCSegments(functions, modules).map((s) => [s.address, s.name]), once);
  assert.throws(() => moduleCSegments(functions, [{ stem: "modules/w", names: new Set(["f_a", "f_c"]) }]), /not contiguous/);
  assert.throws(() => moduleCSegments(functions, [{ stem: "modules/w", names: new Set(["f_a"]) }, { stem: "modules/v", names: new Set(["f_a"]) }]), /both/);
  assert.throws(() => moduleCSegments(functions, [{ stem: "modules/w", names: new Set(["nothing"]) }]), /defines no function/);
});

test("the current tree maps every configured function to its unit, and every unit audits valid", () => {
  for (const container of loadContainers()) {
    const inventory = loadOriginalFunctionInventory(container);
    assert.deepEqual(inventory.findings, [], container.id);
    const units = loadTranslationUnits(container);
    const owned = units.flatMap((unit) => unit.members.map((member) => member.name));
    assert.equal(new Set(owned).size, owned.length, `${container.id}: no function in two units`);
    const game = inventory.functions.filter((fn) => fn.origin === "game").map((fn) => fn.name);
    assert.deepEqual([...owned].sort(), [...game].sort(), `${container.id}: every game function has a unit`);
    assert.equal(units.length, loadSubsegments(container).filter((s) => s.type === "c").length);
  }
});

test("frozen inventory: container-qualified original-function counts and representative identities", () => {
  /* Counts of the original program, whatever the source organization. A fold
     never changes them; a change here is an inventory regression or a
     deliberate reclassification. */
  const expected: Record<string, [game: number, library: number]> = {
    exe: [462, 620], ovl_08: [5, 0], ovl_10: [22, 0], ovl_11: [1606, 0], ovl_15: [135, 0], ovl_17: [61, 0], ovl_19: [76, 0],
    ovl_21: [67, 0], ovl_23: [55, 0], ovl_25: [65, 0], ovl_27: [53, 0], ovl_28: [27, 0], ovl_30: [34, 0], ovl_31: [6, 0],
  };
  for (const container of loadContainers()) {
    const functions = loadOriginalFunctionInventory(container).functions;
    assert.deepEqual([functions.filter((fn) => fn.origin === "game").length, functions.filter((fn) => fn.origin === "library").length],
      expected[container.id], container.id);
  }
  const entry = loadOriginalFunctionInventory("exe").functions.find((fn) => fn.vram === 0x80011278)!;
  assert.equal(entry.name, "func_80011278", "the entry point is named by its symbol");
  assert.equal(translationUnitOf("exe", "func_80011278")?.segmentName, "__start", "and owned by its differently named unit");
  assert.equal(translationUnitOf("ovl_11", "ovl_11_func_800D1CD0")?.segmentName, "ovl_11_func_800D1CD0", "a nested function's carrier is its own unit");
  assert.equal(translationUnitOf("ovl_31", "ovl_31_func_800B7FCC")?.container, "ovl_31");
});

test("every member of a module shares its unit's source and object and keeps its own identity", () => {
  for (const container of loadContainers()) {
    for (const unit of loadTranslationUnits(container).filter((u) => u.foldedModule)) {
      const names = unit.members.map((member) => member.name);
      assert.equal(new Set(names).size, names.length);
      for (const name of names) {
        assert.equal(functionSourcePath(container, name), unit.sourcePath);
        assert.equal(functionObjectPath(container, name), unit.objectPath);
        assert.equal(translationUnitOf(container, name)?.segmentName, unit.segmentName);
      }
    }
  }
  const { units } = layout(MODULE);
  assert.equal(units[1]!.members.length, 2);
});

test("context export reads every definition inside a module", async (t) => {
  const { extractSignaturePairs } = await import("../agent/contextExport.js");
  const dir = mkdtempSync(join(tmpdir(), "module-context-"));
  t.after(() => rmSync(dir, { recursive: true, force: true }));
  const path = join(dir, "widget.c");
  writeFileSync(path, '#include "common.h"\n\ns32 f_b(void) { return 1; }\n\nvoid f_c(s32 value) { (void)value; }\n');
  assert.deepEqual(extractSignaturePairs(path).map((pair) => pair.name), ["f_b", "f_c"]);
  for (const container of loadContainers()) {
    for (const unit of loadTranslationUnits(container).filter((u) => u.foldedModule)) {
      const published = new Set(extractSignaturePairs(join(ROOT, unit.sourcePath)).map((pair) => pair.name));
      for (const member of unit.members) assert.ok(published.has(member.name), `${member.name} in ${unit.sourcePath}`);
    }
  }
});
