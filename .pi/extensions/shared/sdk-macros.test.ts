import assert from "node:assert/strict";
import { mkdirSync, mkdtempSync, readFileSync, rmSync, writeFileSync } from "node:fs";
import { tmpdir } from "node:os";
import { dirname, join } from "node:path";
import test from "node:test";
import { DEFAULT_CONFIG } from "./config.ts";
import { checkSourcePolicy } from "./source-policy.ts";

const sdk = readFileSync(new URL("../../../tools/vendor/psyq47/INCLUDE/INLINE_C.H", import.meta.url), "utf8");
const compatibility = readFileSync(new URL("../../../include/psyq/inline_c_gas.h", import.meta.url), "utf8");
const header = "include/psyq/inline_c_gas.h";

function fixture(t: test.TestContext) {
  const root = mkdtempSync(join(tmpdir(), "sdk-policy-"));
  t.after(() => rmSync(root, { recursive: true, force: true }));
  const put = (file: string, source: string) => {
    mkdirSync(dirname(join(root, file)), { recursive: true });
    writeFileSync(join(root, file), source);
  };
  put("tools/vendor/psyq47/INCLUDE/INLINE_C.H", sdk);
  put("src/target.c", '#include "psyq/inline_c.h"\n#include "psyq/inline_c_gas.h"\nvoid target(void) { gte_rtps(); gte_avsz4(); gte_rtpt_b(); gte_nclip_b(); gte_avsz3_b(); gte_avsz4_b(); }\n');
  const config = structuredClone(DEFAULT_CONFIG);
  config.sourcePolicy.allowlist = {};
  const check = (source: string, file = header, patch?: string) => {
    put(file, source);
    return checkSourcePolicy({ projectRoot: root, config, functionName: "target", changedFiles: [file],
      patch: patch ?? [`+++ b/${file}`, `@@ -0,0 +1,${source.split("\n").length} @@`,
        ...source.split("\n").map(line => `+${line}`)].join("\n") });
  };
  return { root, config, put, check };
}

test("func_8001C37C's six SDK GAS macros pass as a newly added header with no approval", t => {
  const { check, config } = fixture(t);
  const result = check(compatibility);
  assert.equal(result.pass, true, JSON.stringify(result.hardFailures));
  assert.deepEqual(result.newlyAddedForbiddenConstructs, []);
  assert.deepEqual(config.sourcePolicy.allowlist, {});
});

test("vendored SDK definitions and multiline/concatenated GAS spellings retain standing approval", t => {
  const { check } = fixture(t);
  assert.equal(check(sdk, "include/psyq/inline_c.h").pass, true);
  const result = check([
    "#undef gte_rtps",
    "#define gte_rtps() \\",
    '  __asm__ volatile( "nop;" \\',
    '      "nop;" "rtps")',
    "",
  ].join("\n"));
  assert.equal(result.pass, true, JSON.stringify(result.hardFailures));
});

test("a blessed macro name/directory/comment never blesses changed instructions or constraints", t => {
  const { check } = fixture(t);
  for (const body of [
    '__asm__ volatile("nop;nop;rtps;mult $8,$9")',
    '__asm__ volatile("nop;nop;avsz4")',
    '__asm__ volatile("rtps")',
    '__asm__("nop;nop;rtps")',
    '__asm__ volatile("nop;nop;rtps" : "=r"(x))',
    '__asm__ volatile("nop;nop;rtps" : : : "memory")',
    '__asm__ volatile("nop;nop;rtps"); __asm__("nop")',
  ]) {
    const result = check(`/* Generated diagnostic GAS substitutions. */\n#define gte_rtps() ${body}\n`);
    assert.equal(result.pass, false, body);
    assert(result.hardFailures.some(f => f.kind === "embedded-asm"), body);
  }
  assert.equal(check('#define MADE_UP() __asm__ volatile("nop;nop;rtps")\n').pass, false);
  assert.equal(check(compatibility + '\n__asm__("nop");\n').pass, false);
});

test("changing only a continuation audits the whole SDK macro; unrelated legacy asm is not new", t => {
  const { check } = fixture(t);
  const source = '#define gte_rtps() \\\n  __asm__ volatile("nop;nop;rtps;mult $8,$9")\n';
  const patch = [`+++ b/${header}`, "@@ -2 +2 @@", '-  __asm__ volatile("nop;nop;rtps")',
    '+  __asm__ volatile("nop;nop;rtps;mult $8,$9")'].join("\n");
  const result = check(source, header, patch);
  assert.equal(result.pass, false);
  assert.equal(result.hardFailures[0]!.line, 1);

  const unchanged = '__asm__("nop");\n/* new comment */\n';
  assert.equal(check(unchanged, "include/legacy.h", "+++ b/include/legacy.h\n@@ -1,0 +2 @@\n+/* new comment */").pass, true);
});

test("copies in game C, arbitrary header wrappers, and missing SDK witnesses remain forbidden", t => {
  const { root, check } = fixture(t);
  assert.equal(check(compatibility + '\nvoid target(void) { gte_rtps(); }\n', "src/target.c").pass, false);
  assert.equal(check(compatibility, "include/arbitrary.h").pass, false);
  rmSync(join(root, "tools/vendor/psyq47/INCLUDE/INLINE_C.H"));
  assert.equal(check(compatibility).pass, false);
});

test("header audits still reject ordinary asm, pins and disabled/comment false positives correctly", t => {
  const { check } = fixture(t);
  assert.equal(check('#define WRAP() __asm__("nop")\n', "include/wrapper.h").pass, false);
  assert.equal(check('register int pinned __asm__("v0");\n', "include/wrapper.h").pass, false);
  assert.equal(check('/* __asm__("nop"); */\n#if 0\n#define WRAP() __asm__("nop")\n#endif\n', "include/wrapper.h").pass, true);
  assert.equal(check('extern int field[] __asm__("record+2");\n', "include/wrapper.h").pass, true);
});
