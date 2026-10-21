import assert from "node:assert/strict";
import test from "node:test";
import { spawnSync } from "node:child_process";
import { mkdtempSync, mkdirSync, readFileSync, rmSync, writeFileSync } from "node:fs";
import { tmpdir } from "node:os";
import { join } from "node:path";
import { ROOT } from "../lib/psxExeInfo.js";

function fixture(t: test.TestContext, status: number): string {
  const root = mkdtempSync(join(tmpdir(), "make-complete-"));
  t.after(() => rmSync(root, { recursive: true, force: true }));
  const original = readFileSync(join(ROOT, "Makefile"), "utf8");
  const start = original.indexOf("# Read-only completion query.");
  const end = original.indexOf("# Config convergence guard:", start);
  assert.ok(start >= 0 && end > start);
  writeFileSync(join(root, "Makefile"), original.slice(start, end) +
    '\n.PHONY: complete check\ncheck:\n\t@echo built > built\n');
  mkdirSync(join(root, "bin"));
  writeFileSync(join(root, "bin/npx"),
    '#!/bin/sh\nprintf "%s\\n" "$*" >> invoked\nexit ' + status + '\n', { mode: 0o755 });
  // A file named after the target must not suppress the query.
  writeFileSync(join(root, "complete"), "");
  return root;
}

function make(root: string, ...goals: string[]) {
  const env = { ...process.env, PATH: `${join(root, "bin")}:${process.env.PATH}` };
  delete env.MAKEFLAGS;
  delete env.MFLAGS;
  delete env.MAKELEVEL;
  return spawnSync("make", ["--no-print-directory", ...goals], { cwd: root, env, encoding: "utf8" });
}

for (const status of [0, 1, 2, 127]) {
  test(`make complete maps checker exit ${status} to ${status === 0 ? 0 : 1}`, (t) => {
    const root = fixture(t, status);
    const result = make(root, "complete");
    assert.equal(result.status, status === 0 ? 0 : 1, result.stderr);
    assert.equal(result.stdout, "");
    assert.equal(result.stderr, "");
    assert.equal(readFileSync(join(root, "invoked"), "utf8"),
      "tsx tools/diagnostics/progress.ts --check-complete\n");
  });
}

test("completion query mode does not leak into other goals", (t) => {
  const root = fixture(t, 0);
  assert.equal(make(root, "check").status, 0);
  assert.equal(readFileSync(join(root, "built"), "utf8"), "built\n");
  rmSync(join(root, "built"));
  assert.equal(make(root, "complete", "check").status, 0);
  assert.equal(readFileSync(join(root, "built"), "utf8"), "built\n");
});
