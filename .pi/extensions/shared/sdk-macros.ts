/** Standing approval for SDK definitions and their DMPSX-to-GAS shims.
 * Never trust a name, a directory, or a generated-file comment alone. Compare
 * the complete definition with the independent vendored SDK. For command
 * shims use the very same substitution as the encoding oracle; byte identity
 * is still the compiler/oracle's job, not a source-policy approval question. */
import { existsSync, readFileSync } from "node:fs";
import { resolve } from "node:path";
import { analyzeCSource, walkActiveC } from "../../../tools/agent/cSourceGuard.ts";
import { children, field, parseC, type Node } from "../../../tools/agent/residual-source-search/tree-sitter-c.ts";
import { extractMacroTemplates, sdkCommandSubstitution } from "../../../tools/diagnostics/macroTemplates.ts";

function syntax(node: Node): unknown {
  if (node.type === "string_literal" || node.type === "concatenated_string") {
    const pieces = node.type === "string_literal" ? [node] : node.descendantsOfType("string_literal");
    try { return ["string", pieces.map(p => JSON.parse(p.text)).join("")]; }
    catch { return [node.type, node.text]; }
  }
  const parts = children(node).filter(n => n.type !== "comment");
  return parts.length ? [node.type, parts.map(syntax)] : [node.type, node.text];
}

function definitionKey(node: Node): string | undefined {
  const name = field(node, "name"), parameters = field(node, "parameters"), value = field(node, "value");
  if (!name || !parameters || !value) return undefined;
  const body = `void sdk_policy(void) { ${value.text.replace(/\\\r?\n/g, "")}; }`;
  if (!analyzeCSource(body).parses) return undefined;
  const tree = parseC(body);
  try { return JSON.stringify([name.text, syntax(parameters), syntax(tree.rootNode)]); }
  finally { tree.delete(); }
}

function definitions(source: string): Array<{ start: number; key: string }> {
  const tree = parseC(source), result: Array<{ start: number; key: string }> = [];
  try {
    walkActiveC(tree.rootNode, node => {
      if (node.type !== "preproc_function_def") return true;
      const key = definitionKey(node);
      if (key) result.push({ start: node.startIndex, key });
      return false;
    });
  } finally { tree.delete(); }
  return result;
}

/** AST offsets of approved definitions, only in the SDK header namespace.
 * Local copies/redefinitions in game C are not SDK calls. Missing witnesses
 * grant nothing, and reading current contents (no path-only cache) keeps edits
 * to either the candidate or the SDK evidence visible on the next check. */
export function approvedSdkMacroDefinitions(projectRoot: string, file: string, source: string): ReadonlySet<number> {
  const header = file.match(/^include\/psyq\/([^/]+)\.h$/)?.[1];
  if (!header) return new Set();
  const original = resolve(projectRoot, "tools/vendor/psyq47/INCLUDE", `${header.replace(/_gas$/, "").toUpperCase()}.H`);
  if (!existsSync(original)) return new Set();
  const sdk = readFileSync(original, "utf8");
  const approved = new Set(definitions(sdk).map(d => d.key));
  if (header.endsWith("_gas")) {
    const library = extractMacroTemplates([{ path: original, source: sdk }]);
    for (const template of library.templates) {
      const command = sdkCommandSubstitution(template);
      if (!command) continue;
      for (const definition of definitions(`#define ${template.macro}() ${command.body}\n`)) approved.add(definition.key);
    }
  }
  return new Set(definitions(source).filter(d => approved.has(d.key)).map(d => d.start));
}
