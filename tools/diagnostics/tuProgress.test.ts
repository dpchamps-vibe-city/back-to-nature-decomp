import assert from "node:assert/strict";
import test from "node:test";
import { spawnSync } from "node:child_process";
import { mkdtempSync, rmSync } from "node:fs";
import { join } from "node:path";
import { ROOT } from "../lib/psxExeInfo.js";
import { DEFAULT_CONFIG } from "../../.pi/extensions/shared/config.ts";
import { exitCode, type TuProgressReport } from "./tuProgress.js";
import { exceptionGrowth, exceptionSets, type ExceptionSets, type TreeView } from "./tuExceptions.js";

function report(overrides: Partial<TuProgressReport> & { inventory?: Partial<TuProgressReport["inventory"]> } = {}): TuProgressReport {
  const base: TuProgressReport = {
    schemaVersion: 1, complete: false,
    inventory: { eligible: 3, folded: 1, remaining: 2, undetermined: 0, invalid: 0, excluded: {} },
    modules: [], remaining: [], remainingTruncated: false, failures: [],
    integrity: { inventory: "pass", decompilation: "not-run", policy: "not-run", linkedBytes: "not-run", convergence: "not-run", whitespace: "not-run", stability: "not-run", exceptions: "not-run" },
    gates: [], exceptionGrowth: [], baseline: null,
  };
  return { ...base, ...overrides, inventory: { ...base.inventory, ...overrides.inventory } };
}

test("exit status: 0 complete, 1 ordinary module work, 2 any broken or undetermined invariant", () => {
  assert.equal(exitCode(report({ complete: true, inventory: { folded: 3, remaining: 0 } })), 0);
  assert.equal(exitCode(report()), 1);
  assert.equal(exitCode(report({ inventory: { undetermined: 1 } })), 2, "undetermined is never backlog");
  assert.equal(exitCode(report({ inventory: { invalid: 1 } })), 2);
  assert.equal(exitCode(report({ failures: ["inventory conflict"] })), 2);
  assert.equal(exitCode(report({ integrity: { ...report().integrity, inventory: "undetermined" } })), 2);
  assert.equal(exitCode(report({ complete: true, inventory: { remaining: 0 }, gates: [{ gate: "make check-all", state: "fail", detail: "" }] })), 2);
});

test("the current tree reports ordinary module work, deterministically", () => {
  const run = (...args: string[]) => spawnSync(process.execPath, ["--import", "tsx", "tools/diagnostics/tuProgress.ts", ...args], { cwd: ROOT, encoding: "utf8", maxBuffer: 64 * 1024 * 1024 });
  const first = run("--json", "--limit", "5"), second = run("--json", "--limit", "5");
  assert.equal(first.status, 1, first.stderr);
  assert.equal(first.stdout, second.stdout, "two reads of one tree give one report");
  const parsed = JSON.parse(first.stdout) as TuProgressReport;
  assert.equal(parsed.schemaVersion, 1);
  assert.equal(parsed.integrity.inventory, "pass");
  assert.equal(parsed.integrity.linkedBytes, "not-run", "a gate that did not run says so");
  assert.equal(parsed.inventory.eligible, parsed.inventory.folded + parsed.inventory.remaining);
  assert.ok(parsed.inventory.eligible > 2000);
  assert.ok((parsed.inventory.excluded["handwritten-asm"] ?? 0) >= 1, "the entry point is a visible exclusion");
  assert.ok((parsed.inventory.excluded["pending-dead-stub"] ?? 0) > 0, "dead stubs are a visible exclusion");
  assert.ok((parsed.inventory.excluded["library"] ?? 0) > 0);
  assert.equal(parsed.remaining.length, 5);
  assert.equal(parsed.remainingTruncated, true);
  const check = run("--check-complete");
  assert.equal(check.status, 1);
  assert.equal(check.stdout, "", "--check-complete is quiet");
});

test("a campaign receipt and a git baseline both read as no exception growth on an unchanged tree", (t) => {
  const dir = mkdtempSync(join(ROOT, "build", "tu-receipt-test-"));
  t.after(() => rmSync(dir, { recursive: true, force: true }));
  const receipt = join(dir.slice(ROOT.length + 1), "baseline.json");
  const run = (...args: string[]) => spawnSync(process.execPath, ["--import", "tsx", "tools/diagnostics/tuProgress.ts", ...args], { cwd: ROOT, encoding: "utf8", maxBuffer: 64 * 1024 * 1024 });
  assert.equal(run("--write-baseline", receipt).status, 0);
  for (const baseline of [receipt, "HEAD"]) {
    const result = run("--json", "--limit", "0", "--baseline", baseline);
    const parsed = JSON.parse(result.stdout) as TuProgressReport;
    assert.equal(parsed.integrity.exceptions, "pass", `${baseline}: ${JSON.stringify(parsed.exceptionGrowth)}`);
    assert.equal(result.status, 1);
  }
});

const EMPTY: ExceptionSets = { allowlist: [], flagOverrides: [], constructs: [], includeAsm: [], moduleScope: [] };

test("exception growth: any added entry fails, removals never do", () => {
  const before: ExceptionSets = { ...EMPTY, allowlist: ["func_a:embedded-asm"], flagOverrides: ["func_b=-fno-gcse"], constructs: ["exe:func_a:embedded-asm"] };
  assert.deepEqual(exceptionGrowth(before, EMPTY), [], "removing every exception is allowed");
  assert.deepEqual(exceptionGrowth(before, { ...before, allowlist: [...before.allowlist, "func_c:register-asm"] }).map((g) => g.category), ["allowlist"]);
  assert.deepEqual(exceptionGrowth(before, { ...before, flagOverrides: ["func_b=-fno-gcse -fno-cse"] }).map((g) => g.category), ["flagOverrides"], "a changed flag set is a new override");
  assert.deepEqual(exceptionGrowth(before, { ...before, constructs: [...before.constructs, "exe:func_d:register-asm"] }).map((g) => g.category), ["constructs"]);
  assert.deepEqual(exceptionGrowth(before, { ...before, includeAsm: ["exe:func_e"] }).map((g) => g.category), ["includeAsm"]);
  assert.deepEqual(exceptionGrowth(before, { ...before, moduleScope: ["exe:src/modules/m.c:embedded-asm"] }).map((g) => g.category), ["moduleScope"]);
  assert.ok(exceptionGrowth(EMPTY, { ...EMPTY, allowlist: ["modules__m:flag-override"] }).some((g) => g.category === "moduleScope"),
    "an allowlist key naming a module broadens a function exception");
});

test("reviewed TU flag migration retains its origin in the old receipt, but unreviewed growth fails", () => {
  const sourcePath = "src/modules/pair.c", members = ["func_a", "func_b"];
  const review = { sourcePath, container: "exe", members: ["exe:0x80010000", "exe:0x80010010"],
    owner: "exe:0x80010010", flags: "-fno-cse-skip-blocks", evidence: "plans/flag-review-report.md#pair" };
  const base = { "configs/symbols/exe.txt": "func_a = 0x80010000; // type:func\nfunc_b = 0x80010010; // type:func\n" };
  const files: Record<string, string> = { ...base,
    [sourcePath]: "void func_a(void) {}\nvoid func_b(void) {}\n",
    "configs/flag_overrides.mk": `CC1FLAGS_modules__pair := ${review.flags}`,
    "configs/tu_flag_reviews.json": JSON.stringify({ schemaVersion: 1, reviews: [review] }),
  };
  const view = (files: Record<string, string>): TreeView => ({ read: p => files[p] ?? null,
    units: [{ container: "exe", sourcePath, segmentName: "modules/pair", members }] });
  const before = exceptionSets({ read: p => ({ ...base, "configs/flag_overrides.mk": `CC1FLAGS_func_b := ${review.flags}` })[p] ?? null, units: [] }, DEFAULT_CONFIG);
  assert.deepEqual(exceptionGrowth(before, exceptionSets(view(files), DEFAULT_CONFIG)), []);
  for (const changed of [
    { ...files, "configs/tu_flag_reviews.json": '{"schemaVersion":1,"reviews":[]}' },
    { ...files, "configs/flag_overrides.mk": "CC1FLAGS_modules__pair := -fno-gcse" },
    { ...files, [sourcePath]: files[sourcePath] + "void helper(void) {}\n" },
    { ...files, [sourcePath]: "void func_b(void) {}\n" },
  ]) assert.ok(exceptionGrowth(before, exceptionSets(view(changed), DEFAULT_CONFIG)).some(g => g.category === "flagOverrides"));
  const addedAsm = { ...files, [sourcePath]: 'void func_a(void) { __asm__("nop"); }\nvoid func_b(void) {}\n' };
  assert.ok(exceptionGrowth(before, exceptionSets(view(addedAsm), DEFAULT_CONFIG)).some(g => g.category === "constructs"));
});

test("folding a function into a module keeps its exception identity; module scope is new", () => {
  const view = (units: TreeView["units"], files: Record<string, string>): TreeView => ({ read: (path) => files[path] ?? null, units });
  const pinned = "void func_a(void) {\n    __asm__(\"nop\");\n}\n";
  const plain = "void func_b(void) {}\n";
  const perFunction = exceptionSets(view([
    { container: "exe", sourcePath: "src/func_a.c", segmentName: "func_a", members: ["func_a"] },
    { container: "exe", sourcePath: "src/func_b.c", segmentName: "func_b", members: ["func_b"] },
  ], { "src/func_a.c": pinned, "src/func_b.c": plain }), DEFAULT_CONFIG);
  const module = exceptionSets(view([
    { container: "exe", sourcePath: "src/modules/pair.c", segmentName: "modules/pair", members: ["func_a", "func_b"] },
  ], { "src/modules/pair.c": pinned + plain }), DEFAULT_CONFIG);
  assert.deepEqual(module.constructs, perFunction.constructs);
  assert.deepEqual(exceptionGrowth(perFunction, module), []);
  const hoisted = exceptionSets(view([
    { container: "exe", sourcePath: "src/modules/pair.c", segmentName: "modules/pair", members: ["func_a", "func_b"] },
  ], { "src/modules/pair.c": "register int r __asm__(\"s0\");\n" + plain }), DEFAULT_CONFIG);
  assert.deepEqual(hoisted.moduleScope, ["exe:src/modules/pair.c:register-asm"]);
  assert.ok(exceptionGrowth(perFunction, hoisted).some((g) => g.category === "moduleScope"));
});
