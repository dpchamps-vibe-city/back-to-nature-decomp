import assert from "node:assert/strict";
import test from "node:test";
import type { Container } from "../lib/container.js";
import {
  aliasReferences,
  planHeaderRelocations,
  resolveAlias,
  rewriteAliases,
  type AliasReference,
  type NameTable,
} from "./canonicalNames.js";

const container = (id: string, kind: "exe" | "overlay", loadAddr: number, payloadSize: number): Container =>
  ({ id, kind, loadAddr, payloadSize, paths: {} }) as unknown as Container;

/* Two overlays share one slot and put different functions at one address. */
const table: NameTable = {
  containers: [container("exe", "exe", 0x80010000, 0x4e800), container("ovl_10", "overlay", 0x800b8000, 0x10000), container("ovl_11", "overlay", 0x800b8000, 0x10000)],
  functions: new Map([
    ["exe", new Map([[0x80012a34, "Rand"], [0x80015814, "func_80015814"]])],
    ["ovl_10", new Map([[0x800b8100, "ovl_10_func_800B8100"]])],
    ["ovl_11", new Map([[0x800b8100, "Named"]])],
  ]),
  defined: new Map([["exe", new Set(["Rand", "func_80015814"])], ["ovl_10", new Set()], ["ovl_11", new Set(["Named"])]]),
  sourceDefinitions: new Map([["exe", new Set(["Rand"])], ["ovl_10", new Set()], ["ovl_11", new Set()]]),
  headerDeclarations: new Map(),
};

test("identity is the defining container plus the entry address", () => {
  assert.deepEqual(resolveAlias("func_80012A34", "ovl_10", table), { kind: "obsolete", container: "exe", address: 0x80012a34, canonical: "Rand" });
  assert.equal(resolveAlias("func_800B8100", "ovl_10", table)?.kind, "ambiguous");
  assert.deepEqual(resolveAlias("ovl_11_func_800B8100", "ovl_10", table), { kind: "obsolete", container: "ovl_11", address: 0x800b8100, canonical: "Named" });
  assert.deepEqual(resolveAlias("ovl_10_func_800B8100", "ovl_11", table), { kind: "current" });
  assert.deepEqual(resolveAlias("D_80015814", "ovl_10", table), { kind: "obsolete", container: "exe", address: 0x80015814, canonical: "func_80015814" });
  assert.equal(resolveAlias("D_80012000", "ovl_10", table), null, "data at no function entry is not an alias");
  assert.equal(resolveAlias("Rand", "ovl_10", table), null);
});

const SOURCE = `#include "common.h"

/* func_80012A34 is the engine's generator. */
s32 func_80012A34(s32 arg0);
static s32 (*pick)(s32) = func_80012A34;

s32 first(void) {
    return func_80012A34(3) + pick(1);
}

s32 second(void) {
    s32 func_80012A34;
    func_80012A34 = 2;
    return func_80012A34 + (s32)"func_80012A34";
}
`;

test("calls, declarations and callbacks migrate; shadowed locals and prose do not", () => {
  const references = aliasReferences(SOURCE, "ovl_10", table);
  const rewritten = rewriteAliases(SOURCE, references);
  assert.match(rewritten, /^s32 Rand\(s32 arg0\);$/m);
  assert.match(rewritten, /= Rand;/);
  assert.match(rewritten, /return Rand\(3\) \+ pick\(1\);/);
  assert.match(rewritten, /\/\* func_80012A34 is the engine's generator\. \*\//);
  assert.match(rewritten, /s32 func_80012A34;\n    func_80012A34 = 2;\n    return func_80012A34 \+ \(s32\)"func_80012A34";/);
  assert.equal(rewriteAliases(rewritten, aliasReferences(rewritten, "ovl_10", table)), rewritten, "a second run changes nothing");
});

test("macro bodies, assembly strings and colliding declarations are reported, not rewritten", () => {
  const source = `#define ROLL() func_80012A34(4)
u32 Rand(s32 arg0);
s32 func_80012A34(s32 arg0);
void f(void) { __asm__("jal func_80012A34"); func_80012A34(1); }
`;
  const references = aliasReferences(source, "ovl_10", table);
  const contexts = references.map((r) => `${r.context}${r.blocked ? ":blocked" : ""}`);
  assert.deepEqual(contexts, ["macro", "code:blocked", "assembly", "code:blocked"]);
  assert.equal(rewriteAliases(source, references), source);
});

test("a header-bound alias moves its declaration into each user and leaves the header", () => {
  const header = `#ifndef H
#define H
extern int D_800BD07C;

/* D_80015814 - the engine helper under the name the overlay link knew. */
extern void D_80015814(u16 *arg0, s32 arg1);

extern int D_800BD080;
#endif
`;
  const user = `#include "common.h"

void f(s32 arg0) {
    void (*cb)(u16 *, s32) = D_80015814;
}
`;
  const relocating: NameTable = { ...table, headerDeclarations: new Map([["D_80015814", "include/h.h"]]) };
  const sources = new Map([["include/h.h", header], ["src/overlays/ovl_10/user.c", user]]);
  const references = new Map<string, AliasReference[]>([
    ["include/h.h", aliasReferences(header, "exe", relocating, { header: true })],
    ["src/overlays/ovl_10/user.c", aliasReferences(user, "ovl_10", relocating)],
  ]);
  const edits = planHeaderRelocations(sources, references);
  const newHeader = rewriteAliases(header, [], edits.get("include/h.h"));
  const newUser = rewriteAliases(user, references.get("src/overlays/ovl_10/user.c")!, edits.get("src/overlays/ovl_10/user.c"));
  assert.equal(newHeader, "#ifndef H\n#define H\nextern int D_800BD07C;\n\nextern int D_800BD080;\n#endif\n");
  assert.equal(newUser, `#include "common.h"

extern void func_80015814(u16 *arg0, s32 arg1);

void f(s32 arg0) {
    void (*cb)(u16 *, s32) = func_80015814;
}
`);
});
