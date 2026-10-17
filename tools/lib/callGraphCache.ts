/** A receipt for the full ranked graph. Source status, overlay membership and
 * diagnostic inputs are all content-checked; a timestamp is never freshness. */
import { join } from "node:path";
import { digest, fileDigest, readCache, snapshot, writeCache } from "./contentCache.js";
import { buildToolInputs } from "../build/buildInputs.js";

interface GraphReceipt { paths: string[]; inputKey: string; outputHash: string }
const receiptPath = (root: string) => join(root, "build/callGraph.receipt.json");

export function callGraphInputKey(root: string, paths: string[]): string {
  const environment = Object.fromEntries(["CPATH", "C_INCLUDE_PATH", "GCC_EXEC_PREFIX", "COMPILER_PATH", "SOURCE_DATE_EPOCH", "PYTHONPATH"]
    .map((name) => [name, process.env[name] ?? null]));
  /* Resolve tools, not PATH text: npx prepends its own bin directories. */
  return digest(JSON.stringify({ environment, inputs: snapshot(root, [...paths, ...buildToolInputs(root)],
    (p) => !p.endsWith(".test.ts") && !p.endsWith(".pyc")) }));
}

export function callGraphIsFresh(root: string): boolean {
  try {
    const receipt = readCache<GraphReceipt>(receiptPath(root), "full-call-graph-v1");
    return !!receipt && receipt.outputHash === fileDigest(join(root, "build/callGraph.json")) &&
      receipt.inputKey === callGraphInputKey(root, receipt.paths);
  } catch { return false; }
}

export function recordCallGraph(root: string, paths: string[], inputKey: string): void {
  /* Do not certify output generated while its inputs were changing. Scoped
     graph runs never call this; their output hash also invalidates old receipts. */
  if (callGraphInputKey(root, paths) !== inputKey) return;
  writeCache(receiptPath(root), "full-call-graph-v1", {
    paths, inputKey, outputHash: fileDigest(join(root, "build/callGraph.json")),
  } satisfies GraphReceipt);
}
