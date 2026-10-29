import { strict as assert } from "node:assert";
import { readFileSync, mkdtempSync, rmSync, writeFileSync } from "node:fs";
import { tmpdir } from "node:os";
import { join } from "node:path";
import { after, before, test } from "node:test";
import { targetWordsOf } from "../lib/functionOracle.js";
import { functionSourcePath } from "../lib/translationUnits.js";
import { declaratorName, field, parseC } from "./residual-source-search/tree-sitter-c.js";
import { liftWords } from "./pipeline-reversal/lift.js";
import { branchOrientations } from "./branch-orientation.js";
import { detectBranchOrientation } from "./triage.js";
import { controlShapeSweep } from "./controlShapeSweep.js";

const fixture = join(import.meta.dirname, "test-fixtures/branch-orientation");
const fn = "Ovl11LookupMode6GridPosition";
let sources: string;

before(() => {
  sources = mkdtempSync(join(tmpdir(), "branch-orientation-"));
  for (const name of ["parked.c", "address-corrected-nested.c", "final.c"]) {
    const source = readFileSync(join(fixture, name), "utf8"), tree = parseC(source);
    try {
      /* Keep historical evidence intact; bind only its definition to today's name. */
      const names = tree.rootNode.descendantsOfType("function_definition")
        .map(node => declaratorName(field(node, "declarator")))
        .filter(node => node?.text === "ovl_11_func_8011E090");
      assert.equal(names.length, 1);
      const identifier = names[0]!;
      writeFileSync(join(sources, name), source.slice(0, identifier.startIndex) + fn + source.slice(identifier.endIndex));
    } finally { tree.delete(); }
  }
});
after(() => { if (sources) rmSync(sources, { recursive: true, force: true }); });

test("production compiler: parked fires, nested hoist is attributed, committed and target-folded siblings do not", () => {
  const parked = detectBranchOrientation(fn, join(sources, "parked.c"));
  assert.equal(parked.length, 1);
  assert.match(parked[0]!.summary, /Block 8: opposite-sense/);
  assert.match(parked[0]!.evidence.join("\n"), /NOT an assignment-hoist witness/);
  const nested = detectBranchOrientation(fn, join(sources, "address-corrected-nested.c"));
  assert.equal(nested.length, 1);
  assert.match(nested[0]!.summary, /assignment-hoist.*jump.c:596/);
  assert.deepEqual(detectBranchOrientation(fn, join(sources, "final.c")), []);
  const control = "Ovl11AreaGridCellFromPosition";
  assert.deepEqual(detectBranchOrientation(control, functionSourcePath("ovl_11", control)), []);
});
test("original inverted sibling words detect the opposite direction against a non-inverted MIR tail", () => {
  const sibling = "Ovl11LookupMode1GridPosition", words = targetWordsOf(sibling, { container: "ovl_11" });
  const tail = words.findLastIndex(w => w.text.startsWith("beqz "));
  assert.ok(tail >= 0);
  const candidate = words.map((w, i) => ({ ...w, text: i === tail ? w.text.replace("beqz", "bnez") : i === tail + 1 ? "move v0,zero" : i === tail + 2 ? "li v0,1" : w.text }));
  const findings = branchOrientations(liftWords({ functionName: sibling, words }), liftWords({ functionName: sibling, words: candidate }));
  assert.equal(findings.length, 1);
  assert.match(findings[0]!.target, /^beqz/);
  assert.match(findings[0]!.candidate, /^bnez/);
});
test("complete-source sweep fixes a genuine nested-tail residual and preserves the exact outer duplicate else", () => {
  const path = join(sources, "address-corrected-nested.c"), original = readFileSync(path, "utf8");
  const report = controlShapeSweep(fn, path);
  assert.equal(report.baseline.exact, false);
  assert.equal(report.coverage.exhaustive, true);
  assert.equal(report.coverage.failed, 0);
  const duplicate = report.variants.find(v => v.form === "nested-duplicate-1")!;
  assert.equal(duplicate.objective?.exact, true);
  assert.match(readFileSync(duplicate.source, "utf8"), /else \{\nreturn 0;\n\}\nreturn 0;/);
  for (const form of ["and", "nested-duplicate-0"]) assert.ok(report.variants.find(v => v.form === form)?.trace?.passes[0]?.changes.some(c => c.classification === "assignment-hoist"));
  assert.ok(report.variants.find(v => v.form === "early-returns")?.trace?.passes[0]?.changes.some(c => c.classification === "store-flag-fold"));
  assert.ok(report.variants.find(v => v.form === "or-inverted")?.trace?.passes[0]?.changes.some(c => c.classification === "undetermined"));
  assert.equal(readFileSync(path, "utf8"), original);
  assert.ok(report.exact.every(p => p.startsWith("build/")));
  assert.equal(report.targetHash.length, 64);
});
test("sweep refuses a result typedef hiding narrowing or volatile storage", () => {
  const directory = mkdtempSync(join(tmpdir(), "control-result-"));
  try {
    const path = join(directory, "input.c");
    for (const type of ["short", "volatile int"]) {
      writeFileSync(path, `typedef ${type} u32;\nint ${fn}(int a) { u32 r; r=0; if (a) r=65536; return r; }\n`);
      assert.throws(() => controlShapeSweep(fn, path), /expanded result-local type/);
    }
  } finally { rmSync(directory, { recursive: true, force: true }); }
});

test("sweep refuses an expression macro hiding a call rather than treating it as a read", () => {
  const directory = mkdtempSync(join(tmpdir(), "control-shape-"));
  try {
    const path = join(directory, "input.c");
    writeFileSync(path, `#define C g()\nint g(void);\nint ${fn}(void) { if (C) return 1; return 0; }\n`);
    assert.throws(() => controlShapeSweep(fn, path), /raw\/preprocessed tail tokens differ/);
  } finally { rmSync(directory, { recursive: true, force: true }); }
});
