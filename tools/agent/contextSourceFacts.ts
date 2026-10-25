/** Immutable raw-C facts for context publication. Cache identities include the
 * source bytes and AST implementation, never mtimes or generated prototypes.
 * Preprocessed type facts still require the real cpp's dependency/search proof. */
import { readFileSync } from "node:fs";
import { join, resolve } from "node:path";
import { ROOT } from "./decompToolchain.js";
import { analyzeCSource } from "./cSourceGuard.js";
import { collectTypedefs, extractPrototypesFromSource, extractSignaturesFromSource, type ExtractedSignature } from "./sdkTypes.js";
import { C_FRONTEND_IDENTITY, parseC } from "./residual-source-search/tree-sitter-c.js";
import { digest, fileDigest, readCache, writeCache } from "../lib/contentCache.js";

export function contextImplementation(files: string[] = []): string {
  return digest(JSON.stringify([C_FRONTEND_IDENTITY, ...[
    "tools/agent/contextSourceFacts.ts", "tools/agent/sdkTypes.ts", "tools/agent/cSourceGuard.ts",
    "tools/agent/declarationContext.ts", "tools/agent/residual-source-search/tree-sitter-c.ts",
    "tools/lib/contentCache.ts", "tools/vendor/tree-sitter-c/tree-sitter-c.wasm", "package-lock.json", ...files,
  ].map((p) => fileDigest(join(ROOT, p)))]));
}
export interface ContextSourceFacts {
  signatures: ExtractedSignature[];
  prototypes: ExtractedSignature[];
  includes: Array<{ path: string; line: number }>;
  hasIncludeAsmText: boolean;
  stubSymbols: string[];
  hasLocalTypes: boolean;
}
export function contextSourceReader(root: string) {
  root = resolve(root);
  const implementation = contextImplementation(), stats = { hits: 0, misses: 0 };
  return Object.assign((path: string): ContextSourceFacts => {
    path = resolve(path);
    const source = readFileSync(path, "utf8"), key = digest(JSON.stringify([digest(source), implementation]));
    const cache = join(root, "build/cache/context-source", digest(path) + ".json");
    const hit = readCache<ContextSourceFacts>(cache, key);
    if (hit) { stats.hits++; return hit; }
    stats.misses++;
    const tree = parseC(source);
    let includes: ContextSourceFacts["includes"];
    try {
      includes = tree.rootNode.descendantsOfType("preproc_include").map((n) => ({
        path: n.childForFieldName("path")?.text.slice(1, -1) ?? "", line: n.startPosition.row + 1,
      }));
    } finally { tree.delete(); }
    /* A stub's own function is never published, nor a disabled parked body
       beside it; actual symbols are read by the AST guard. A module that
       carries a dead function's stub still publishes its other members. */
    const hasIncludeAsmText = source.includes("INCLUDE_ASM(");
    const stubSymbols = hasIncludeAsmText ? analyzeCSource(source).includeAsm.map((s) => s.symbol) : [];
    const signatures = extractSignaturesFromSource(source).filter((s) => !stubSymbols.includes(s.name));
    const typedefs = new Map<string, string>();
    if (signatures.length) collectTypedefs(source, typedefs);
    const facts: ContextSourceFacts = { signatures, prototypes: extractPrototypesFromSource(source), includes,
      hasIncludeAsmText, stubSymbols,
      hasLocalTypes: typedefs.size > 0 || source.includes("struct ") || source.includes("union ") || source.includes("enum ") };
    writeCache(cache, key, facts);
    return facts;
  }, { stats });
}
