import assert from "node:assert/strict";
import test from "node:test";
import { execFileSync } from "node:child_process";
import { mkdtempSync, mkdirSync, readFileSync, rmSync, writeFileSync } from "node:fs";
import { tmpdir } from "node:os";
import { join } from "node:path";
import { ROOT } from "../lib/psxExeInfo.js";
import { configuredCSources } from "./buildInputs.js";

/** The Makefile's shared source listing and overlay rules, with nothing else. */
function fixtureMakefile(root: string): void {
  const original = readFileSync(join(ROOT, "Makefile"), "utf8");
  const listing = original.slice(original.indexOf("ConfiguredCSources ="), original.indexOf("\n", original.indexOf("ConfiguredCSources =")));
  const start = original.indexOf("define OverlayRules\n"), end = original.indexOf("\nendef", start) + "\nendef".length;
  writeFileSync(join(root, "Makefile"), `BUILD_DIR := build\n${listing}\nC_SRCS := $(call ConfiguredCSources,configs/splat/exe.yaml,src)\nOVERLAYS := ovl_test\n` +
    original.slice(start, end) + "\n$(eval $(call OverlayRules,ovl_test))\n.PHONY: selected\nselected:\n\t@echo $(ovl_test_C_SRCS)\n");
}

test("every container builds exactly its configured c subsegments, nested module stems included", (t) => {
  const root = mkdtempSync(join(tmpdir(), "overlay-sources-"));
  t.after(() => rmSync(root, { recursive: true, force: true }));
  for (const dir of ["configs/splat", "src/overlays/ovl_test/modules", "src/modules"]) mkdirSync(join(root, dir), { recursive: true });
  fixtureMakefile(root);
  const live = "src/overlays/ovl_test/head.c", retired = "src/overlays/ovl_test/case.c", module = "src/overlays/ovl_test/modules/widget.c";
  writeFileSync(join(root, live), "int head(void) {return 1;}\n");
  const stub = 'INCLUDE_ASM("build/nonexistent/case", case);\n';
  writeFileSync(join(root, retired), stub);
  writeFileSync(join(root, module), "int a(void) {return 1;}\nint b(void) {return 2;}\n");
  const config = join(root, "configs/splat/ovl_test.yaml");
  const selected = () => execFileSync("make", ["--no-print-directory", "selected"], { cwd: root, encoding: "utf8" }).trim().split(/\s+/);
  writeFileSync(config, "subsegments:\n  - [0x0, .rodata, head]\n  - [0x10, c, head]\n  - [0x18, c, case]\n  - [0x20, c, modules/widget]\n  - [0x30, data]\n");
  assert.deepEqual(selected(), [live, retired, module], "link order, nested stem preserved");

  /* The executable lists its configured units too, not a glob of src/. */
  writeFileSync(join(root, "configs/splat/exe.yaml"), "subsegments:\n  - [0x0, c, exe]\n  - [0x8, c, modules/loader]  # 0x80010008 loader\n  - [0x10, data]\n");
  writeFileSync(join(root, "src/exe.c"), "void exe(void) {}\n");
  writeFileSync(join(root, "src/modules/loader.c"), "void loader(void) {}\n");
  writeFileSync(join(root, "src/stray.c"), "void stray(void) {}\n");
  writeFileSync(join(root, "src/overlays/misplaced.c"), "void misplaced(void) {}\n");
  assert.deepEqual(configuredCSources(root), ["src/exe.c", "src/modules/loader.c", retired, live, module].sort(),
    "an unconfigured file under src/ is never a build input");

  writeFileSync(config, "subsegments:\n  - [0x0, .rodata, head]\n  - [0x10, c, head]\n  - [0x30, data]\n");
  assert.deepEqual(selected(), [live]);
  assert.deepEqual(configuredCSources(root), ["src/exe.c", "src/modules/loader.c", live]);
  assert.equal(readFileSync(join(root, retired), "utf8"), stub, "retired source remains untouched, with no standalone asm prerequisite");
});

test("a missing configuration selects nothing rather than failing the listing", (t) => {
  const root = mkdtempSync(join(tmpdir(), "overlay-sources-"));
  t.after(() => rmSync(root, { recursive: true, force: true }));
  mkdirSync(join(root, "src"), { recursive: true });
  fixtureMakefile(root);
  assert.deepEqual(configuredCSources(root), []);
});
