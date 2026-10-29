#!/usr/bin/env npx tsx
/**
 * Read-only INCLUDE_ASM completion gate. Scan every C source/header under src/
 * and include/, including overlays and unconfigured files. Only src/__start.c
 * is exempt: its handwritten runtime startup must remain assembly.
 *
 * Usage: make check-asm (or npx tsx tools/diagnostics/checkAsm.ts)
 */
import { readdirSync, readFileSync } from "node:fs";
import { join, relative, resolve } from "node:path";
import { fileURLToPath } from "node:url";
import { field, parseC } from "../agent/residual-source-search/tree-sitter-c.js";

export interface IncludeAsmUse {
  line: number;
  symbol: string;
}

/** Ignore comments, string literals and macro definitions, not disabled arms. */
export function findIncludeAsm(source: string): IncludeAsmUse[] {
  if (!source.includes("INCLUDE_ASM")) return [];
  const tree = parseC(source);
  try {
    return tree.rootNode.descendantsOfType("call_expression")
      .filter((node) => field(node, "function")?.text === "INCLUDE_ASM")
      .map((node) => ({
        line: node.startPosition.row + 1,
        symbol: field(node, "arguments")?.namedChildren.find((child) => child?.type === "identifier")?.text ?? "<unknown>",
      }));
  } finally {
    tree.delete();
  }
}

export function checkAsm(root: string): Array<IncludeAsmUse & { file: string }> {
  const findings: Array<IncludeAsmUse & { file: string }> = [];
  function scan(directory: string): void {
    for (const entry of readdirSync(directory, { withFileTypes: true }).sort((a, b) => a.name.localeCompare(b.name))) {
      const absolute = join(directory, entry.name);
      if (entry.isDirectory()) {
        scan(absolute);
      } else if (entry.isFile() && /\.(c|h)$/.test(entry.name)) {
        const file = relative(root, absolute).split("\\").join("/");
        if (file === "src/__start.c") continue;
        for (const use of findIncludeAsm(readFileSync(absolute, "utf8"))) findings.push({ file, ...use });
      }
    }
  }
  scan(join(root, "src"));
  scan(join(root, "include"));
  return findings;
}

function main(): void {
  const findings = checkAsm(fileURLToPath(new URL("../../", import.meta.url)));
  if (findings.length === 0) {
    console.log("check-asm: OK — no INCLUDE_ASM outside src/__start.c");
  } else {
    console.error(`check-asm: FAIL — ${findings.length} INCLUDE_ASM invocation(s) outside src/__start.c`);
    for (const finding of findings) console.error(`  ${finding.file}:${finding.line}: ${finding.symbol}`);
    process.exitCode = 1;
  }
}

if (process.argv[1] && resolve(process.argv[1]) === fileURLToPath(import.meta.url)) main();
