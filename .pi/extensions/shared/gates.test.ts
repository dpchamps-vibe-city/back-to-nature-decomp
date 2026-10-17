import assert from "node:assert/strict";
import test from "node:test";
import { parseFunctionDiffSummary, runBuildCheck } from "./gates.ts";
import { mkdtempSync, mkdirSync, readFileSync, writeFileSync, rmSync } from "node:fs";
import { tmpdir } from "node:os";
import { join } from "node:path";

test("full build checks retain check-all with bounded parallelism", async (t) => {
  const root = mkdtempSync(join(tmpdir(), "build-check-jobs-")), oldPath = process.env.PATH;
  mkdirSync(join(root, "bin"));
  writeFileSync(join(root, "bin/make"), `#!${process.execPath}\nrequire('node:fs').writeFileSync('args.json', JSON.stringify(process.argv.slice(2)));\n`, { mode: 0o755 });
  process.env.PATH = join(root, "bin") + ":" + oldPath;
  t.after(() => { process.env.PATH = oldPath; rmSync(root, { recursive: true, force: true }); });
  assert.equal((await runBuildCheck(root)).code, 0);
  const args = JSON.parse(readFileSync(join(root, "args.json"), "utf8"));
  assert.equal(args.length, 2); assert.equal(args[1], "check-all");
  assert.match(args[0], /^-j[1-8]$/);
});

test("parses the oracle's word count and verdict", () => {
  const result = parseFunctionDiffSummary(
    "Match: 214/214 words (100.0%)\nVERDICT: MATCH — every word is byte-identical to the original after relocation.\n",
  );

  assert.deepEqual(result, {
    matchedInstructions: 214,
    totalInstructions: 214,
    matchPercent: 100,
    verdict: "match",
  });
});

test("a mismatch is reported as such even at a high word count", () => {
  const result = parseFunctionDiffSummary(
    "Match: 80/81 words (98.8%)\nVERDICT: MISMATCH — 1 word(s) differ.\n",
  );

  assert.deepEqual(result, {
    matchedInstructions: 80,
    totalInstructions: 81,
    matchPercent: 98.8,
    verdict: "mismatch",
  });
});

test("an undetermined verdict is kept distinct from a match, even at a full count", () => {
  const result = parseFunctionDiffSummary(
    "Match: 9/9 words (100.0%)\nVERDICT: UNDETERMINED — 1 word(s) could not be resolved; nothing else differs.\n",
  );

  assert.equal(result.verdict, "undetermined");
  assert.equal(result.matchedInstructions, result.totalInstructions);
});

test("uses the last summary and reports nothing rather than a pass when there is none", () => {
  assert.deepEqual(
    parseFunctionDiffSummary("Match: 1/2 words (50.0%)\nVERDICT: MISMATCH\nMatch: 2/2 words (100.0%)\nVERDICT: MATCH\n"),
    { matchedInstructions: 2, totalInstructions: 2, matchPercent: 100, verdict: "match" },
  );
  assert.deepEqual(
    parseFunctionDiffSummary("diff command failed before producing a summary\n"),
    { matchedInstructions: 0, totalInstructions: 0, matchPercent: 0, verdict: "unknown" },
  );
});
