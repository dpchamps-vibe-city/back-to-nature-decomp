/**
 * Real-toolchain checks for multi-function translation units: a module object
 * holds several members, each is still byte-identical through the per-function
 * oracle, and a container linked with a module — pooled jump tables included —
 * reproduces the original image exactly.
 *
 * Everything is built out of tree; the source tree and configuration are only
 * read.
 */
import assert from "node:assert/strict";
import test from "node:test";
import { spawnSync } from "node:child_process";
import { copyFileSync, existsSync, mkdirSync, mkdtempSync, readFileSync, readdirSync, rmSync, symlinkSync, writeFileSync } from "node:fs";
import { createHash } from "node:crypto";
import { tmpdir } from "node:os";
import { dirname, join, posix } from "node:path";
import { ROOT } from "../lib/psxExeInfo.js";
import { requireContainer } from "../lib/container.js";
import { compareFunction } from "../lib/functionOracle.js";
import { loadTranslationUnits, translationUnitOf } from "../lib/translationUnits.js";
import { objectFacts } from "../lib/translationUnitAudit.js";
import { compileSource, configuredAsFlagsForContainer } from "../agent/decompToolchain.js";
import { objectGprelSymbols } from "./deriveTuOwnedGlobals.js";
import { foldConfigLines, mergeSources } from "./foldTranslationUnit.js";

const available = (command: string) => spawnSync("sh", ["-c", `command -v ${command}`]).status === 0;
const toolchain = available("mips-linux-gnu-as") && existsSync(join(ROOT, "build/functions.csv"));
const splat = toolchain && available("splat");

function sources(container: string, members: string[]): Array<{ path: string; text: string }> {
  const seen = new Set<string>();
  return members.map((name) => translationUnitOf(container, name)!.sourcePath).filter((path) => !seen.has(path) && seen.add(path))
    .map((path) => ({ path, text: readFileSync(join(ROOT, path), "utf8") }));
}

(toolchain ? test : test.skip)("two EXE functions in one module object keep their TU-owned small data and match independently", (t) => {
  const dir = mkdtempSync(join(tmpdir(), "module-exe-"));
  t.after(() => rmSync(dir, { recursive: true, force: true }));
  const members = ["SetFlag8005E274", "GetFlag8005E274"];
  const merged = mergeSources(sources("exe", members), "/* flag accessors */");
  assert.equal(merged.text.match(/^u8 D_8005E274;$/gm)?.length, 1, "one tentative definition survives the merge");
  const module = join(dir, "flag_accessors.c");
  writeFileSync(module, merged.text);
  const compiled = compileSource(module, join(dir, "out"), "flag_accessors", { assemble: true, containerKind: "exe" });
  const facts = objectFacts(compiled.object!)!;
  assert.deepEqual(facts.functions.map((fn) => fn.name), members, "one object emits both members in address order");
  for (const name of members) {
    assert.equal(compareFunction(name, { objectPath: compiled.object!, container: "exe" }).verdict, "match", name);
    assert.ok(objectGprelSymbols(compiled.object!, name)?.has("D_8005E274"), `${name} still reaches its global through $gp`);
  }
});

/**
 * A container rebuilt in a scratch project with some units folded into a
 * module: split by splat, compiled and assembled by the configured toolchain,
 * linked by its generated script, and compared with the original image.
 */
function linkWithModule(t: test.TestContext, containerId: string, members: string[], name: string): { original: string; built: string; ld: string } {
  const container = requireContainer(containerId);
  const root = mkdtempSync(join(tmpdir(), `module-${containerId}-`));
  t.after(() => rmSync(root, { recursive: true, force: true }));
  const place = (relative: string) => { mkdirSync(dirname(join(root, relative)), { recursive: true }); return join(root, relative); };
  symlinkSync(join(ROOT, container.targetPath), place(container.targetPath));
  symlinkSync(join(ROOT, "include"), join(root, "include"));
  symlinkSync(join(ROOT, "tools/vendor/splat_ext"), place("tools/vendor/splat_ext"));
  for (const file of readdirSync(join(ROOT, "configs/symbols"))) copyFileSync(join(ROOT, "configs/symbols", file), place(`configs/symbols/${file}`));
  copyFileSync(join(ROOT, "build/engine_syms.txt"), place("build/engine_syms.txt"));

  const units = loadTranslationUnits(container);
  const folded = units.filter((unit) => unit.members.some((member) => members.includes(member.name)));
  const stem = `modules/${name}`;
  const config = foldConfigLines(readFileSync(join(ROOT, container.paths.splat), "utf8").split("\n"), folded.map((u) => u.segmentName), stem);
  writeFileSync(place(container.paths.splat), config.lines.join("\n"));
  for (const unit of units) if (!folded.includes(unit)) copyFileSync(join(ROOT, unit.sourcePath), place(unit.sourcePath));
  writeFileSync(place(posix.join(container.paths.srcDir, `${stem}.c`)), mergeSources(sources(containerId, members), `/* ${name} */`).text);

  const split = spawnSync("splat", ["split", container.paths.splat], { cwd: root, encoding: "utf8", env: { ...process.env, SPIMDISASM_ARCHLEVEL: "1" } });
  assert.equal(split.status, 0, split.stderr);
  const ld = join(root, container.paths.ldScript);
  writeFileSync(ld, readFileSync(ld, "utf8") + readFileSync(join(ROOT, container.paths.disasmDir, "ld_includes.txt"), "utf8"));

  const configured = [...config.lines.join("\n").matchAll(/^\s*- \[0x[0-9A-Fa-f]+, c, ([^\]\s]+)\]/gm)].map((m) => m[1]!);
  for (const unit of configured) {
    const out = place(posix.join(container.paths.objDir, `${unit}.c.o`));
    compileSource(join(root, container.paths.srcDir, `${unit}.c`), dirname(out), posix.basename(unit), { assemble: true, containerKind: "overlay" });
  }
  const asFlags = configuredAsFlagsForContainer("overlay");
  const assemble = (directory: string) => {
    for (const entry of readdirSync(directory, { withFileTypes: true })) {
      const path = join(directory, entry.name);
      if (entry.isDirectory()) assemble(path);
      else if (entry.name.endsWith(".s")) assert.equal(spawnSync("mips-linux-gnu-as", [...asFlags, path, "-o", `${path}.o`], { cwd: root }).status, 0, path);
    }
  };
  assemble(join(root, container.paths.asmDir, "data"));
  const elf = join(root, container.paths.builtElf), bin = join(root, container.paths.builtBin);
  const link = spawnSync("mips-linux-gnu-ld", ["-EL", "-T", container.paths.ldScript, "-o", container.paths.builtElf], { cwd: root, encoding: "utf8" });
  assert.equal(link.status, 0, link.stderr);
  assert.equal(spawnSync("mips-linux-gnu-objcopy", ["-O", "binary", elf, bin]).status, 0);
  const hash = (path: string) => createHash("sha256").update(readFileSync(path)).digest("hex");
  return { original: hash(join(ROOT, container.targetPath)), built: hash(bin), ld: readFileSync(ld, "utf8") };
}

(splat ? test : test.skip)("an overlay module with two pooled jump tables links byte-identically at -G0", (t) => {
  const members = ["ovl_19_func_800BA834", "ovl_19_func_800BAB4C"];
  if (members.some((name) => translationUnitOf("ovl_19", name)?.foldedModule)) return t.skip("the pair is already folded in the tree");
  const result = linkWithModule(t, "ovl_19", members, "object_readers");
  assert.match(result.ld, /build\/src\/overlays\/ovl_19\/modules\/object_readers\.c\.o\(\.rodata\)/, "one pooled .rodata, attributed to the module");
  assert.match(result.ld, /build\/src\/overlays\/ovl_19\/modules\/object_readers\.c\.o\(\.text\)/);
  assert.equal(result.built, result.original);
});
