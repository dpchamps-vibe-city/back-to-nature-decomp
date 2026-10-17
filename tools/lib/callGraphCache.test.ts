import test from "node:test";
import assert from "node:assert/strict";
import { mkdirSync, mkdtempSync, readFileSync, rmSync, symlinkSync, writeFileSync } from "node:fs";
import { tmpdir } from "node:os";
import { join } from "node:path";
import { ROOT } from "./psxExeInfo.js";
import { callGraphInputKey, callGraphIsFresh, recordCallGraph } from "./callGraphCache.js";
import { nextTarget } from "../../.pi/extensions/psx-decomp/autoloop/oracles.ts";

test("target selection reuses the graph, but refreshes changed overlay status and rejects stale output", async (t) => {
  const root = mkdtempSync(join(tmpdir(), "graph-reuse-"));
  t.after(() => rmSync(root, { recursive: true, force: true }));
  for (const dir of ["src/overlays/ovl_11", "configs", "build", "tools/agent"])
    mkdirSync(join(root, dir), { recursive: true });
  symlinkSync(join(ROOT, "node_modules"), join(root, "node_modules"));
  writeFileSync(join(root, "Makefile"), "GCC_VERSION := fixture\nCROSS := mips-linux-gnu-\n");
  const paths = ["src", "configs", "tools/agent"];
  const functions = ["first", "second"].map((name, i) => ({ name, container: "ovl_11", vram: `0x8001000${i * 4}`,
    dead: false, handwritten: false, decompiled: false }));
  const source = join(root, "src/overlays/ovl_11/first.c");
  writeFileSync(source, "void first(void) {}\n");
  writeFileSync(join(root, "configs/graph.json"), JSON.stringify({ functions }));
  writeFileSync(join(root, "build/callGraph.json"), JSON.stringify({ functions }));
  writeFileSync(join(root, "tools/agent/callGraph.ts"), `
import { readFileSync, writeFileSync } from 'node:fs';
import { callGraphInputKey, recordCallGraph } from ${JSON.stringify(join(ROOT, "tools/lib/callGraphCache.ts"))};
const root = process.cwd(), paths = ${JSON.stringify(paths)};
const key = callGraphInputKey(root, paths);
writeFileSync('build/rebuilt', 'yes');
writeFileSync('build/callGraph.json', readFileSync('configs/graph.json'));
recordCallGraph(root, paths, key);
`);
  recordCallGraph(root, paths, callGraphInputKey(root, paths));
  assert.equal(callGraphIsFresh(root), true);
  assert.equal(await nextTarget(root, new Set()), "first");
  assert.equal(await nextTarget(root, new Set(["first"])), "second");
  assert.equal(await nextTarget(root, new Set(), new Set(["first"])), "second");
  assert.throws(() => readFileSync(join(root, "build/rebuilt")), /ENOENT/);

  writeFileSync(source, "void first(void) { return; }\n");
  assert.equal(callGraphIsFresh(root), false, "nested overlay edits invalidate the receipt");
  functions[0]!.decompiled = true;
  writeFileSync(join(root, "configs/graph.json"), JSON.stringify({ functions }));
  assert.equal(await nextTarget(root, new Set()), "second");
  assert.equal(readFileSync(join(root, "build/rebuilt"), "utf8"), "yes");
  assert.equal(callGraphIsFresh(root), true);

  const added = join(root, "src/overlays/ovl_11/added.c");
  writeFileSync(added, "void added(void) {}\n");
  assert.equal(callGraphIsFresh(root), false, "new source membership invalidates the receipt");
  rmSync(added);
  assert.equal(callGraphIsFresh(root), true);
  writeFileSync(join(root, "build/callGraph.json"), "{}");
  assert.equal(callGraphIsFresh(root), false, "partial/scoped/corrupt output is not the certified graph");
});
