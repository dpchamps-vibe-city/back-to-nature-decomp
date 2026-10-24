import assert from "node:assert/strict";
import { mkdtempSync, mkdirSync, writeFileSync } from "node:fs";
import { tmpdir } from "node:os";
import { join } from "node:path";
import { fileURLToPath } from "node:url";
import test from "node:test";
import { DEFAULT_CONFIG, loadConfig } from "./config.ts";
import { checkSourcePolicy, isPendingStub } from "./source-policy.ts";

function fixture(source: string) {
  const root = mkdtempSync(join(tmpdir(), "autodecomp-policy-"));
  mkdirSync(join(root, "src"));
  mkdirSync(join(root, "configs"));
  writeFileSync(join(root, "src", "target.c"), source);
  writeFileSync(join(root, "configs", "flag_overrides.mk"), "");
  const config = structuredClone(DEFAULT_CONFIG);
  config.runtimeDir = join(root, "run_output");
  return { root, config };
}

test("allows comments and the narrow empty memory scheduling barrier", () => {
  const { root, config } = fixture(`/* __asm__(\"not code\") */\nvoid target(void) {\n    __asm__ volatile(\"\" ::: \"memory\");\n}\n`);
  const result = checkSourcePolicy({ projectRoot: root, config, functionName: "target", scanFunctions: ["target"] });
  assert.equal(result.pass, true);
});

test("rejects register pinning, embedded assembly, and stubs", () => {
  const { root, config } = fixture(`void target(void) {\n    register int x __asm__(\"v0\");\n    __asm__(\"nop\");\n    INCLUDE_ASM(\"x\", target);\n}\n`);
  const result = checkSourcePolicy({ projectRoot: root, config, functionName: "target", scanFunctions: ["target"] });
  assert.equal(result.pass, false);
  assert.deepEqual(new Set(result.hardFailures.map((finding) => finding.kind)), new Set(["register-asm", "embedded-asm", "include-asm"]));
});

test("catches embedded asm written with the reserved __volatile__ spelling", () => {
  const { root, config } = fixture(`void target(void) {\n    __asm__ __volatile__(\"nop\");\n}\n`);
  const result = checkSourcePolicy({ projectRoot: root, config, functionName: "target", scanFunctions: ["target"] });
  assert.equal(result.pass, false);
  assert(result.hardFailures.some((finding) => finding.kind === "embedded-asm"));
});

test("a patch-added line is judged against its own file's allowlist entry", () => {
  const { root, config } = fixture("void target(void) {\n    __asm__(\"nop\");\n}\n");
  config.sourcePolicy.allowlist["target"] = ["embedded-asm"];
  const patch = [
    "--- a/src/target.c",
    "+++ b/src/target.c",
    "@@ -1,0 +2,1 @@",
    '+    __asm__("nop");',
  ].join("\n");
  /* Repo-wide sweep: no top-level functionName, so the allowlist can only be
   * reached by attributing the line to src/target.c. */
  const allowlisted = checkSourcePolicy({ projectRoot: root, config, scanFunctions: ["target"], patch });
  assert.equal(allowlisted.pass, true);

  delete config.sourcePolicy.allowlist["target"];
  const bare = checkSourcePolicy({ projectRoot: root, config, scanFunctions: ["target"], patch });
  assert(bare.hardFailures.some((finding) => finding.kind === "embedded-asm"));
});

test("a note quoting embedded asm is documentation, not a violation", () => {
  const { root, config } = fixture("void target(void) {}\n");
  const patch = [
    "--- a/notes/retros/example.md",
    "+++ b/notes/retros/example.md",
    "@@ -1,0 +2,1 @@",
    '+    __asm__ __volatile__("nop");',
  ].join("\n");
  const result = checkSourcePolicy({ projectRoot: root, config, scanFunctions: ["target"], patch });
  assert.equal(result.pass, true);
});

test("project scope permits plans without widening unrelated roots or source exceptions", () => {
  const { root, config } = fixture("void target(void) {}\n");
  /* Finalization reads the project's integration roots, not DEFAULT_CONFIG. */
  const projectRoot = fileURLToPath(new URL("../../../", import.meta.url));
  config.integration = loadConfig(projectRoot).integration;
  const changedFiles = [
    "plans/retired-overview.md",
    "./plans/static-recompilation-project/overview.md",
    "plans/static-recompilation-project/phase-01-end-to-end-slice.md",
  ];
  const patch = [
    "+++ b/plans/static-recompilation-project/overview.md",
    "@@ -0,0 +1 @@",
    '+__asm__("nop");',
  ].join("\n");
  const result = checkSourcePolicy({ projectRoot: root, config, functionName: "target", changedFiles, patch });
  assert.equal(result.pass, true);
  assert.deepEqual(result.outOfScopeFiles, []);

  const unrelated = checkSourcePolicy({
    projectRoot: root,
    config,
    changedFiles: [...changedFiles, "README.md", "plans-other/overview.md"],
  });
  assert.equal(unrelated.pass, false);
  assert.deepEqual(unrelated.outOfScopeFiles, ["README.md", "plans-other/overview.md"]);

  const sourcePatch = [
    patch,
    "+++ b/src/target.c",
    "@@ -0,0 +1 @@",
    '+void target(void) { __asm__("nop"); }',
  ].join("\n");
  const forbidden = checkSourcePolicy({
    projectRoot: root,
    config,
    functionName: "target",
    changedFiles: [...changedFiles, "src/target.c"],
    patch: sourcePatch,
  });
  assert.equal(forbidden.pass, false);
  assert(forbidden.hardFailures.some((finding) => finding.kind === "embedded-asm"));
});

test("classifies a pin written without underscores as register pinning, not embedded asm", () => {
  const { root, config } = fixture(`void target(void) {\n    register int x asm(\"v0\");\n}\n`);
  const result = checkSourcePolicy({ projectRoot: root, config, functionName: "target", scanFunctions: ["target"] });
  assert.equal(result.pass, false);
  assert(result.hardFailures.some((finding) => finding.kind === "register-asm"));
});

test("an undecompiled backlog stub is a pending stub; a partial fold is not", () => {
  assert.equal(isPendingStub(`#include "common.h"\n#include "include_asm.h"\n\nINCLUDE_ASM("build/asm/x", target);\n`), true);
  assert.equal(isPendingStub(`/* INCLUDE_ASM is only mentioned here */\n#include "common.h"\n\nINCLUDE_ASM("build/asm/x", target);\n`), true);
  assert.equal(isPendingStub(`#include "common.h"\n\nint helper(void) { return 1; }\nINCLUDE_ASM("build/asm/x", target);\n`), false);
  assert.equal(isPendingStub(`void target(void) {}\n`), false);
});

test("an asm label on a declaration is a symbol rename, not embedded assembly", () => {
  const { root, config } = fixture("void target(void) {}\n");
  const added = (line: string) => [
    "--- a/include/globals_override.h",
    "+++ b/include/globals_override.h",
    "@@ -1,0 +2,1 @@",
    `+${line}`,
  ].join("\n");

  /* The documented override for an absolutely-addressed generated symbol. */
  for (const line of [
    'extern struct_80070000 _D_80070000[1] __asm__("D_80070000");',
    'extern s32 _D_8004B1A4[3] __asm__("D_8004B1A4");',
    'extern u16 D_80076226[] __asm__("D_80076224+2");',
    'extern u16 D_8007622A[] __asm__("D_80076228+2");',
    'extern u16 D_80076236[] __asm__("D_80076234+2");',
    'extern u16 D_8007623E[] __asm__("D_8007623C+2");',
    'extern u16 field[] __asm__("record + 0x2");',
    'extern u16 field[] __asm__("record.end-16");',
  ]) {
    const result = checkSourcePolicy({ projectRoot: root, config, functionName: "target", patch: added(line) });
    assert.equal(result.pass, true, `expected an asm label to pass: ${line}`);
  }

  /* Instructions must not enter through it, however the line is arranged. */
  for (const line of [
    '__asm__("nop");',
    'int x = 0; __asm__("nop");',
    'void f(void) { __asm__("nop"); }',
    'extern int x __asm__("lw %0, 0(%1)" : "=r"(a));',
    '__asm__("D_80076224+2");',
    'int x; __asm__("D_80076224+2");',
    'void f(void) { __asm__("D_80076224+2"); }',
    'extern int x __asm__("record+other");',
    'extern int x __asm__("record+2; nop");',
    'extern int x __asm__("record+2\\nnop");',
    'extern int x __asm__("record+2" "nop");',
    'extern int x __asm__("record+2" : "=r"(a));',
    'extern int x = ({ __asm__("record+2"); 0; });',
    'register int x __asm__("$2");',
  ]) {
    const result = checkSourcePolicy({ projectRoot: root, config, functionName: "target", patch: added(line) });
    assert.equal(result.pass, false, `expected an asm statement to fail: ${line}`);
  }
});

test("symbol-offset labels pass the complete-source audit without exempting instructions or pins", () => {
  const { root, config } = fixture('extern int field[] __asm__("record+2");\nvoid target(void) {}\n');
  const options = { projectRoot: root, config, functionName: "target" };
  assert.equal(checkSourcePolicy(options).pass, true);
  for (const body of ['__asm__("record+2");', 'register int x __asm__("$2");']) {
    writeFileSync(join(root, "src/target.c"), `extern int field[] __asm__("record+2");\nvoid target(void) { ${body} }\n`);
    assert.equal(checkSourcePolicy(options).pass, false, body);
  }
});

test("rejects out-of-scope files and added flag overrides", () => {
  const { root, config } = fixture("void target(void) {}\n");
  const patch = `diff --git a/configs/flag_overrides.mk b/configs/flag_overrides.mk\n--- a/configs/flag_overrides.mk\n+++ b/configs/flag_overrides.mk\n@@ -0,0 +1 @@\n+CC1FLAGS_target := -fno-schedule-insns\n`;
  const result = checkSourcePolicy({
    projectRoot: root,
    config,
    functionName: "target",
    changedFiles: ["README.md", "configs/flag_overrides.mk"],
    patch,
  });
  assert.equal(result.pass, false);
  assert(result.hardFailures.some((finding) => finding.kind === "out-of-scope"));
  assert(result.hardFailures.some((finding) => finding.kind === "flag-override"));
});

test("an overlay's translation unit is scanned where it actually lives", () => {
  const root = mkdtempSync(join(tmpdir(), "autodecomp-policy-overlay-"));
  mkdirSync(join(root, "src", "overlays", "ovl_31"), { recursive: true });
  mkdirSync(join(root, "configs"), { recursive: true });
  writeFileSync(join(root, "configs", "flag_overrides.mk"), "");
  writeFileSync(
    join(root, "src", "overlays", "ovl_31", "ovl_31_target.c"),
    `void ovl_31_target(void) {\n    __asm__(\"nop\");\n}\n`,
  );
  const config = structuredClone(DEFAULT_CONFIG);
  config.runtimeDir = join(root, "run_output");

  /* Without the source map the scan looks under `src/`, finds nothing, and
     reports a clean function it never opened — a pass, not an error. */
  const blind = checkSourcePolicy({ projectRoot: root, config, functionName: "ovl_31_target", scanFunctions: ["ovl_31_target"] });
  assert.equal(blind.pass, true);

  const placed = checkSourcePolicy({
    projectRoot: root,
    config,
    functionName: "ovl_31_target",
    functionContainer: "ovl_31",
    functionSources: { ovl_31_target: "src/overlays/ovl_31/ovl_31_target.c" },
    scanFunctions: ["ovl_31_target"],
  });
  assert.equal(placed.pass, false);
  assert(placed.hardFailures.some((finding) => finding.kind === "embedded-asm"));
});

test("a patch to an overlay source is attributed to the function, not to the path tail", () => {
  const root = mkdtempSync(join(tmpdir(), "autodecomp-policy-patch-"));
  mkdirSync(join(root, "configs"), { recursive: true });
  const config = structuredClone(DEFAULT_CONFIG);
  config.runtimeDir = join(root, "run_output");
  config.sourcePolicy.allowlist = { ovl_31_target: ["embedded-asm"] };

  const patch = [
    "+++ b/src/overlays/ovl_31/ovl_31_target.c",
    "@@ -1,0 +2,1 @@",
    '+    __asm__("nop");',
  ].join("\n");

  /* The allowlist is keyed by the function's name. Reading the path tail as the
     name yields `overlays/ovl_31/ovl_31_target`, which matches no entry. */
  const result = checkSourcePolicy({ projectRoot: root, config, changedFiles: [], patch });
  assert.equal(result.newlyAddedForbiddenConstructs.length, 0);
});

test("CAPTURE_PREV_RET has standing approval at either AST scope; absent census is advisory", () => {
  for (const source of [
    'CAPTURE_PREV_RET(phantom);\nint target(void) { return phantom; }\n',
    'int target(void) {\n CAPTURE_PREV_RET(phantom);\n return phantom; }\n',
  ]) {
    const { root, config } = fixture(source);
    const result = checkSourcePolicy({ projectRoot: root, config, functionName: "target" });
    assert.equal(result.pass, true); assert.equal(result.hardFailures.length, 0);
    assert.equal(result.warnings.length, 1); assert.match(result.warnings[0]!.message, /census/);
    mkdirSync(join(root, "build"));
    writeFileSync(join(root, "build/nestedFunctionCensus.json"), JSON.stringify({ rows: [{ id: "exe:target", function: "target", container: "exe", callee: { form: "dead-spill" } }] }));
    assert.equal(checkSourcePolicy({ projectRoot: root, config, functionName: "target" }).warnings.length, 0);
  }
});

test("CAPTURE_RA has standing approval; a local asm redefinition of it does not", () => {
  const { root, config } = fixture('#include "debughook.h"\nvoid target(int *dst) {\n CAPTURE_RA(dst);\n}\n');
  assert.equal(checkSourcePolicy({ projectRoot: root, config, functionName: "target" }).pass, true);
  writeFileSync(join(root, "src/target.c"), '#define CAPTURE_RA(dst) __asm__ volatile("sw $31,0(%0)" : : "r"(dst))\nvoid target(int *dst) { CAPTURE_RA(dst); }\n');
  const result = checkSourcePolicy({ projectRoot: root, config, functionName: "target" });
  assert.equal(result.pass, false); assert(result.hardFailures.some((finding) => finding.kind === "embedded-asm"));
});

test("raw entry-$2 declarations route to the macro/census, including multiline syntax", () => {
  const { root, config } = fixture('register s32\n phantom\n asm("$2");\nvoid target(void) {}\n');
  const result = checkSourcePolicy({ projectRoot: root, config, functionName: "target" });
  assert.equal(result.pass, false); assert.equal(result.hardFailures.length, 1);
  assert.match(result.hardFailures[0]!.message, /CAPTURE_PREV_RET.*census/);
});

test("AST policy ignores mentions/disabled arms, permits nested asm labels and refuses macro redefinition", () => {
  const { root, config } = fixture('/* register s32 x asm("$2"); CAPTURE_PREV_RET(x); */\n#if 0\nregister s32 x asm("$2");\nCAPTURE_PREV_RET(x);\n#endif\nint target(void) {\n auto s32 nested(s32, s32) __asm__("real");\n return nested(1,2);\n}\n');
  assert.equal(checkSourcePolicy({ projectRoot: root, config, functionName: "target" }).pass, true);
  writeFileSync(join(root, "src/target.c"), '#define CAPTURE_PREV_RET(x) int x\nvoid target(void) { CAPTURE_PREV_RET(p); }\n');
  const result = checkSourcePolicy({ projectRoot: root, config, functionName: "target" });
  assert.equal(result.pass, false); assert.match(result.hardFailures[0]!.message, /redefine/);
});

test("a bare-address allowlist key is the executable's, never another container's", () => {
  const root = mkdtempSync(join(tmpdir(), "autodecomp-policy-keys-"));
  mkdirSync(join(root, "src", "overlays", "ovl_30"), { recursive: true });
  mkdirSync(join(root, "src"), { recursive: true });
  mkdirSync(join(root, "configs"), { recursive: true });
  writeFileSync(join(root, "configs", "flag_overrides.mk"), "");
  const body = `void f(void) {\n    __asm__(\"nop\");\n}\n`;
  writeFileSync(join(root, "src", "exe_fn.c"), body);
  writeFileSync(join(root, "src", "overlays", "ovl_30", "ovl_30_fn.c"), body);

  const config = structuredClone(DEFAULT_CONFIG);
  config.runtimeDir = join(root, "run_output");
  config.sourcePolicy.allowlist = { "0x800b7e24": ["embedded-asm"] };

  const scan = (name: string, container: string, source: string) => checkSourcePolicy({
    projectRoot: root,
    config,
    functionName: name,
    functionVram: "0x800B7E24",
    functionContainer: container,
    functionSources: { [name]: source },
    scanFunctions: [name],
  });

  /* Same address, same exception key, two different functions. Granting it to
     both is how one function's policy exception licenses another's assembly. */
  assert.equal(scan("exe_fn", "exe", "src/exe_fn.c").pass, true);
  assert.equal(scan("ovl_30_fn", "ovl_30", "src/overlays/ovl_30/ovl_30_fn.c").pass, false);

  config.sourcePolicy.allowlist = { "ovl_30:0x800b7e24": ["embedded-asm"] };
  assert.equal(scan("ovl_30_fn", "ovl_30", "src/overlays/ovl_30/ovl_30_fn.c").pass, true);
  assert.equal(scan("exe_fn", "exe", "src/exe_fn.c").pass, false);
});

/* --- reconstructed modules: per-function attribution ----------------------- */

const MODULE_SOURCE = [
  "#include \"common.h\"",
  "",
  "void first(void) {",
  "    register int x __asm__(\"v0\");",
  "    __asm__(\"nop\");",
  "}",
  "",
  "void second(void) {",
  "    __asm__(\"nop\");",
  "}",
  "",
].join("\n");
const MEMBERS = [{ name: "first", vram: "0x80100000", container: "ovl_t" }, { name: "second", vram: "0x80100010", container: "ovl_t" }];

test("module-local asm is charged to the function whose body holds it", async () => {
  const { moduleConstructFindings } = await import("./source-policy.ts");
  const config = structuredClone(DEFAULT_CONFIG);
  const findings = moduleConstructFindings(MODULE_SOURCE, config, MEMBERS);
  assert.deepEqual(findings.map((f) => [f.kind, f.line, /\(in (\w+)\)/.exec(f.message)?.[1]]),
    [["register-asm", 4, "first"], ["embedded-asm", 5, "first"], ["embedded-asm", 9, "second"]]);
});

test("one member's allowlist entry does not authorize its neighbour", async () => {
  const { moduleConstructFindings } = await import("./source-policy.ts");
  const config = structuredClone(DEFAULT_CONFIG);
  config.sourcePolicy.allowlist["first"] = ["register-asm", "embedded-asm"];
  const findings = moduleConstructFindings(MODULE_SOURCE, config, MEMBERS);
  assert.deepEqual(findings.map((f) => [f.kind, f.line]), [["embedded-asm", 9]], "only second's asm remains");
  config.sourcePolicy.allowlist["ovl_t:0x80100010"] = ["embedded-asm"];
  assert.deepEqual(moduleConstructFindings(MODULE_SOURCE, config, MEMBERS), [], "a container-qualified address key is honoured");
});

test("module-scope constructs cannot borrow a member's exception", async () => {
  const { moduleConstructFindings } = await import("./source-policy.ts");
  const config = structuredClone(DEFAULT_CONFIG);
  config.sourcePolicy.allowlist["first"] = ["register-asm", "embedded-asm", "include-asm"];
  config.sourcePolicy.allowlist["second"] = ["register-asm", "embedded-asm", "include-asm"];
  const source = [
    "register int pinned __asm__(\"s0\");",
    "#define WRAP() __asm__(\"nop\")",
    "void first(void) { WRAP(); }",
    "INCLUDE_ASM(\"asm\", second);",
    "",
  ].join("\n");
  const findings = moduleConstructFindings(source, config, MEMBERS);
  assert.ok(findings.some((f) => f.kind === "register-asm" && f.line === 1 && /module scope/.test(f.message)), "file-scope pin");
  assert.ok(findings.some((f) => f.kind === "embedded-asm" && f.line === 2 && /module scope/.test(f.message)), "module-local asm macro");
  assert.ok(findings.some((f) => f.kind === "include-asm" && /partial fold/.test(f.message)), "a stub inside a module, allowlisted or not");
});

test("a module source is scanned once with member scopes, and a module flag override is never allowlisted", () => {
  const root = mkdtempSync(join(tmpdir(), "autodecomp-module-policy-"));
  mkdirSync(join(root, "src", "modules"), { recursive: true });
  mkdirSync(join(root, "configs"));
  writeFileSync(join(root, "src", "modules", "widget.c"), MODULE_SOURCE);
  writeFileSync(join(root, "configs", "flag_overrides.mk"), "CC1FLAGS_modules__widget := -fno-gcse\n");
  const config = structuredClone(DEFAULT_CONFIG);
  config.sourcePolicy.allowlist["modules__widget"] = ["flag-override"];
  const result = checkSourcePolicy({
    projectRoot: root, config, scanFunctions: ["first", "second"],
    functionSources: { first: "src/modules/widget.c", second: "src/modules/widget.c" },
  });
  const constructs = result.hardFailures.filter((f) => f.kind !== "flag-override");
  assert.equal(constructs.length, 3, "each construct reported once although two members share the file");
  assert.ok(result.hardFailures.some((f) => f.kind === "flag-override" && /Module-level/.test(f.message)));
});
