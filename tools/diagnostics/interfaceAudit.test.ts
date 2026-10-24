import assert from "node:assert/strict";
import test from "node:test";
import type { Prototype } from "../agent/calleeTruth.js";
import type { Container } from "../lib/container.js";
import { aliasReferences, rewriteAliases, type NameTable } from "./canonicalNames.js";
import { auditInterfaces, contractFor, reconcileCallers, type Finding, type Published } from "./interfaceAudit.js";

const DEFINITION = "#include \"common.h\"\n\nu32 Rand(s32 arg0) {\n    return arg0 & 0xFFFF;\n}\n";
const audit = (sources: Record<string, string>): Finding[] =>
  auditInterfaces({ sources: new Map(Object.entries({ "src/Rand.c": DEFINITION, ...sources })), sdk: new Map(), published: new Map() }).findings;
const brief = (findings: Finding[]): string[] => findings.map((f) => `${f.kind} ${f.callee} ${f.file}${f.differences ? ` ${f.differences.join(",")}` : ""}`);

test("a conflicting prototype and an implicit declaration are reported; a corroborated one is not", () => {
  const findings = audit({
    "src/Callee.c": "#include \"common.h\"\n\nu16 Callee(s32 arg0) {\n    return arg0;\n}\n",
    "src/overlays/ovl_10/signed.c": "#include \"common.h\"\ns16 Callee(s32 arg0);\ns32 a(void) { return Callee(1); }\n",
    "src/overlays/ovl_10/agreeing.c": "#include \"common.h\"\nu16 Callee(s32);\nu32 b(void) { return Callee(2); }\n",
    "src/overlays/ovl_10/undeclared.c": "#include \"common.h\"\ns32 c(void) { return Callee(3); }\n",
    "src/overlays/ovl_10/pointer.c": "#include \"common.h\"\nvoid d(void) {\n    s32 (*fn)(void);\n    fn = 0;\n    fn();\n}\n",
  });
  assert.deepEqual(brief(findings), [
    "conflict Callee src/overlays/ovl_10/signed.c return s16≠u16",
    "implicit Callee src/overlays/ovl_10/undeclared.c",
  ]);
});

test("a published interface fails when a unit that includes its module header restates it", () => {
  const prototype = { name: "Rand", signature: "u32 Rand (s32 arg0);", parameters: 1, variadic: false, returnsVoid: false,
    returnType: "u32", paramTypes: ["s32"], kind: "declaration", where: "include/common.h", line: 1 } as Prototype;
  const published = new Map<string, Published>([["Rand", { prototype, header: "include/common.h" }]]);
  const result = auditInterfaces({
    sources: new Map([["src/Rand.c", DEFINITION], ["src/overlays/ovl_10/agreeing.c", "#include \"common.h\"\nu32 Rand(s32);\n"]]),
    sdk: new Map(), published,
  });
  assert.deepEqual(brief(result.publishedFailures), ["restated Rand src/overlays/ovl_10/agreeing.c"]);
});

test("a name-only rewrite leaves the caller's contract exactly as it was", () => {
  const table: NameTable = {
    containers: [{ id: "exe", kind: "exe", loadAddr: 0x80010000, payloadSize: 0x4e800, paths: {} } as unknown as Container,
      { id: "ovl_10", kind: "overlay", loadAddr: 0x800b8000, payloadSize: 0x10000, paths: {} } as unknown as Container],
    functions: new Map([["exe", new Map([[0x80012a34, "Rand"]])], ["ovl_10", new Map()]]),
    defined: new Map([["exe", new Set(["Rand"])], ["ovl_10", new Set()]]),
    sourceDefinitions: new Map([["exe", new Set(["Rand"])], ["ovl_10", new Set()]]),
    headerDeclarations: new Map(),
  };
  const before = "#include \"common.h\"\ns32 func_80012A34(s32 arg0);\ns32 a(void) { return func_80012A34(1); }\n";
  const after = rewriteAliases(before, aliasReferences(before, "ovl_10", table));
  assert.equal(after, "#include \"common.h\"\ns32 Rand(s32 arg0);\ns32 a(void) { return Rand(1); }\n");
  assert.deepEqual(brief(audit({ "src/overlays/ovl_10/a.c": after })), ["conflict Rand src/overlays/ovl_10/a.c return s32≠u32"]);
});

test("reconciliation declares the definition's prototype and keeps a cast only where the object needs it", () => {
  const path = "src/overlays/ovl_10/propagation_fixture.c";
  const unit = `#include "common.h"

/* Overlay import of Rand. */
s32 Rand(s32 arg0);

s32 roll(s32 limit) {
    s32 kept;
    kept = Rand(4);
    if (Rand(100) < limit) {
        return kept;
    }
    return 0;
}
`;
  const sources = new Map([["src/Rand.c", DEFINITION], [path, unit]]);
  const contract = contractFor("Rand", sources);
  assert.ok(typeof contract !== "string", String(contract));
  const { results, texts } = reconcileCallers(sources, contract);
  assert.equal(results[0]?.outcome, "written", results[0]?.detail);
  assert.equal(texts.get(path), `#include "common.h"

/* Overlay import of Rand. */
u32 Rand(s32 arg0);

s32 roll(s32 limit) {
    s32 kept;
    kept = Rand(4);
    if ((s32)Rand(100) < limit) {
        return kept;
    }
    return 0;
}
`);
  const again = reconcileCallers(new Map([["src/Rand.c", DEFINITION], [path, texts.get(path)!]]), contract);
  assert.equal(again.texts.size, 0, "a second run changes nothing");
});
