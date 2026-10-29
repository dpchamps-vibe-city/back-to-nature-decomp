import assert from "node:assert/strict";
import { mkdirSync, mkdtempSync, rmSync, writeFileSync } from "node:fs";
import { tmpdir } from "node:os";
import { dirname, join } from "node:path";
import test from "node:test";
import { checkAsm, findIncludeAsm } from "./checkAsm.js";

const STUB = 'INCLUDE_ASM("asm", unfinished);';

function fixture(t: test.TestContext, files: Record<string, string>): string {
  const root = mkdtempSync(join(tmpdir(), "check-asm-"));
  t.after(() => rmSync(root, { recursive: true, force: true }));
  for (const directory of ["src", "include"]) mkdirSync(join(root, directory));
  for (const [file, content] of Object.entries(files)) {
    const absolute = join(root, file);
    mkdirSync(dirname(absolute), { recursive: true });
    writeFileSync(absolute, content);
  }
  return root;
}

test("reports every invocation with its symbol and line, including multiline calls", () => {
  assert.deepEqual(findIncludeAsm(`/* heading */\n${STUB}\nINCLUDE_ASM /* comment */ (\n    "asm", /* symbol */ other\n);`), [
    { line: 2, symbol: "unfinished" },
    { line: 3, symbol: "other" },
  ]);
});

test("comments, strings, macro definitions and similar identifiers are not invocations", () => {
  assert.deepEqual(findIncludeAsm(`
/* ${STUB} */
// ${STUB}
const char *text = "INCLUDE_ASM(\\\"asm\\\", fake)";
#define INCLUDE_ASM(FOLDER, NAME) ignored
INCLUDE_ASM_EXTRA("asm", other);
`), []);
});

test("disabled and conditional stubs still count as leftover INCLUDE_ASM", () => {
  assert.deepEqual(findIncludeAsm(`#if 0\n${STUB}\n#endif\n#ifdef OTHER\n${STUB}\n#endif\n`), [
    { line: 2, symbol: "unfinished" },
    { line: 5, symbol: "unfinished" },
  ]);
});

test("clean sources pass and only the exact startup path is exempt", (t) => {
  const root = fixture(t, {
    "src/__start.c": STUB,
    "src/modules/complete.c": "void complete(void) {}\n",
    "include/include_asm.h": "#define INCLUDE_ASM(FOLDER, NAME) ignored\n",
  });
  assert.deepEqual(checkAsm(root), []);
});

test("scans standalone files, nested modules, overlays and headers, not build artifacts", (t) => {
  const root = fixture(t, {
    "src/__start.c": STUB,
    "src/unfinished.c": STUB,
    "src/modules/nested/unfinished.c": STUB,
    "src/overlays/ovl_test/modules/__start.c": STUB,
    "include/nested/unfinished.h": STUB,
    "src/ignored.txt": STUB,
    "build/candidate.c": STUB,
  });
  assert.deepEqual(checkAsm(root).map((finding) => finding.file).sort(), [
    "include/nested/unfinished.h",
    "src/modules/nested/unfinished.c",
    "src/overlays/ovl_test/modules/__start.c",
    "src/unfinished.c",
  ]);
});

test("a missing source tree fails rather than silently passing", (t) => {
  const root = fixture(t, {});
  rmSync(join(root, "src"), { recursive: true });
  assert.throws(() => checkAsm(root), /ENOENT/);
});
