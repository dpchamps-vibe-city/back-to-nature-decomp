import assert from "node:assert/strict";
import test from "node:test";
import { foldConfigLines, mergeSources, planFold, Refusal } from "./foldTranslationUnit.js";

test("merging keeps each file's include order, and one copy of a shared tentative definition", () => {
  const merged = mergeSources([
    { path: "a.c", text: '#include "common.h"\n#include "psyq/libmcrd.h"\n\nu8 D_flag;\n\nvoid Set(u8 v) { D_flag = v; }\n' },
    { path: "b.c", text: '#include "common.h"\n#include "psyq/kernel.h"\n#include "psyq/libmcrd.h"\n\nu8 D_flag;\n\nu8 Get(void) { return D_flag; }\n' },
  ], "/* header */");
  assert.equal(merged.text, '/* header */\n\n#include "common.h"\n#include "psyq/kernel.h"\n#include "psyq/libmcrd.h"\n\nu8 D_flag;\n\nvoid Set(u8 v) { D_flag = v; }\n\nu8 Get(void) { return D_flag; }\n');
  assert.ok(merged.notes.some((note) => /kept one copy of `u8 D_flag;`/.test(note)));
});

test("merging refuses a macro defined twice differently, or one that would reach a later member", () => {
  assert.throws(() => mergeSources([
    { path: "a.c", text: "#define LIMIT 4\nint A(void) { return LIMIT; }\n" },
    { path: "b.c", text: "#define LIMIT 5\nint B(void) { return LIMIT; }\n" },
  ], ""), /defines macro LIMIT differently/);
  assert.throws(() => mergeSources([
    { path: "a.c", text: "#define count 4\nint A(void) { return count; }\n" },
    { path: "b.c", text: "int B(int count) { return count; }\n" },
  ], ""), (error) => error instanceof Refusal && /macro count from a\.c would apply to b\.c/.test(error.message));
  assert.throws(() => mergeSources([
    { path: "a.c", text: "static int helper(void) { return 1; }\nint A(void) { return helper(); }\n" },
    { path: "b.c", text: "static int helper(void) { return 2; }\nint B(void) { return helper(); }\n" },
  ], ""), /both define static helper/);
});

test("a nested function's carrier contributes nothing", () => {
  const merged = mergeSources([
    { path: "carrier.c", text: '#include "common.h"\n\n/* defined inside parent */\n' },
    { path: "parent.c", text: '#include "common.h"\n\nint parent(void) { int inner(void) { return 1; } return inner(); }\n' },
  ], "/* h */");
  assert.doesNotMatch(merged.text, /defined inside parent/);
  assert.ok(merged.notes.some((note) => /carrier\.c holds only includes and comments/.test(note)));
});

test("config folding pools contiguous jump tables and refuses an interleaved one", () => {
  const lines = [
    "      - [0x0, rodata]",
    "      - [0x10, .rodata, f_a]",
    "      - [0x20, .rodata, f_b]",
    "      - [0x30, rodata]",
    "      - [0x100, c, f_a]       # 0x80100100 f_a",
    "      - [0x140, c, f_b]",
    "      - [0x180, c, f_c]",
  ];
  const folded = foldConfigLines(lines, ["f_a", "f_b"], "modules/pair");
  assert.deepEqual(folded.lines, [
    "      - [0x0, rodata]",
    "      - [0x10, .rodata, modules/pair]",
    "      - [0x30, rodata]",
    "      - [0x100, c, modules/pair]",
    "      - [0x180, c, f_c]",
  ]);
  const interleaved = [...lines.slice(0, 2), "      - [0x18, rodata]", ...lines.slice(2)];
  assert.throws(() => foldConfigLines(interleaved, ["f_a", "f_b"], "modules/pair"), /not contiguous: - \[0x18, rodata\]/);
  const reversed = ["      - [0x10, .rodata, f_b]", "      - [0x20, .rodata, f_a]", ...lines.slice(3)];
  assert.throws(() => foldConfigLines(reversed, ["f_a", "f_b"], "modules/pair"), /not in member order/);
});

test("planning refuses what the mechanics cannot do soundly", () => {
  const refusal = (members: string[], module = "memcard_service") => {
    try {
      planFold({ container: "ovl_31", module, members });
      return "planned";
    } catch (error) {
      assert.ok(error instanceof Refusal, String(error));
      return error.message;
    }
  };
  assert.match(refusal(["ovl_31_func_800B82E8", "ovl_31_func_800B83B8"]), /not contiguous/);
  assert.match(refusal(["ovl_31_func_800B82E8"], "func_800B82E8"), /encodes an address/);
  assert.match(refusal(["func_80011270"]), /not an original game function of ovl_31/);
  assert.match(planFold({ container: "exe", module: "flag_accessors", members: ["SetFlag8005E274", "GetFlag8005E274"] }).moduleText, /^u8 D_8005E274;$/m);
  assert.match((() => { try { planFold({ container: "exe", module: "entry", members: ["func_80011278"] }); return ""; } catch (error) { return (error as Error).message; } })(),
    /INCLUDE_ASM stub/);
});
