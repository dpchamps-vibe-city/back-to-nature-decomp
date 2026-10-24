#!/usr/bin/env npx tsx
/**
 * tuProgress.ts — translation-unit reconstruction progress and its integrity gate.
 *
 * The matching decompilation is complete when every live function is compiled
 * C and every container is byte-identical (`make complete`, `make check-all`).
 * Its *organization* is complete when every eligible function is owned by an
 * explicit, reconstructed module translation unit. This reports the second
 * without weakening the first, and keeps the two as separate dimensions.
 *
 * An eligible function — a game function implemented in C, live or dead,
 * including ones with authorized source-policy exceptions — is **folded**
 * exactly when:
 *
 *   1. the original-function inventory gives it a unique container/address;
 *   2. exactly one configured C translation unit's interval owns it;
 *   3. that unit is configured under `modules/`;
 *   4. its stem is a semantic name, not an address;
 *   5. the source AST defines it exactly once;
 *   6. the built object emits it exactly once;
 *   7. source order and object offsets agree with original address order;
 *   8. no per-function source or subsegment remains for it; and
 *   9. the unit has no membership, flag, rodata, ownership, policy or
 *      interface finding.
 *
 * Handwritten assembly, library members, pending `INCLUDE_ASM` stubs and
 * containers without a solved base are excluded from the denominator, and are
 * always reported. Nothing here reads a list of modules: the source tree and
 * the configuration are the record, and progress is derived from them afresh.
 *
 * Usage:
 *   npx tsx tools/diagnostics/tuProgress.ts                       # human report
 *   npx tsx tools/diagnostics/tuProgress.ts --remaining [--limit N]
 *   npx tsx tools/diagnostics/tuProgress.ts --container ovl_11
 *   npx tsx tools/diagnostics/tuProgress.ts --json
 *   npx tsx tools/diagnostics/tuProgress.ts --check-complete      # read-only, structural
 *   npx tsx tools/diagnostics/tuProgress.ts --verify [--baseline <receipt|tree-ish>]
 *   npx tsx tools/diagnostics/tuProgress.ts --write-baseline [<path>]   # campaign receipt
 *
 * Exit status — the same for every mode:
 *   0  every eligible function is folded and every requested gate passes;
 *   1  the inventory is valid and ordinary module work remains;
 *   2  configuration, membership, policy, build or another required
 *      invariant failed or is undetermined.
 */

import { spawnSync } from "node:child_process";
import { createHash } from "node:crypto";
import { existsSync, readFileSync, readdirSync, rmSync, statSync, writeFileSync } from "node:fs";
import { availableParallelism } from "node:os";
import { join, relative } from "node:path";
import { ROOT } from "../lib/psxExeInfo.js";
import { containerPath, loadContainers, requireContainer, unresolvedCodeMembers, type Container } from "../lib/container.js";
import { loadOriginalFunctionInventory, loadSubsegments } from "../lib/symbolIndex.js";
import { translationUnitOf, type TranslationUnit } from "../lib/translationUnits.js";
import { auditContainer, loadFlagOverrideKeys, type ContainerAudit, type SourceFacts, type UnitAudit } from "../lib/translationUnitAudit.js";
import { computeLiveness } from "../lib/liveness.js";
import { detectMacroIdentities } from "./macroIdentity.js";
import { macroRouteSummary } from "./macroTiler.js";
import { loadConfig } from "../../.pi/extensions/shared/config.ts";
import { moduleConstructFindings } from "../../.pi/extensions/shared/source-policy.ts";
import { activeSources } from "./canonicalNames.js";
import { auditInterfaces, publishedInterfaces, type Finding } from "./interfaceAudit.js";
import { sdkPrototypes } from "../agent/calleeTruth.js";
import { deriveOwnership, witnessDisagreements } from "../build/deriveTuOwnedGlobals.js";
import { captureReceipt, currentExceptionSets, DEFAULT_RECEIPT, exceptionGrowth, inputsDigest, loadBaseline, type ExceptionGrowth } from "./tuExceptions.js";

export type GateState = "pass" | "fail" | "not-run";

export interface TuProgressReport {
  schemaVersion: 1;
  complete: boolean;
  inventory: {
    eligible: number;
    folded: number;
    remaining: number;
    /** Eligible functions whose units cannot be proved either way. */
    undetermined: number;
    /** Eligible functions whose units fail an invariant. */
    invalid: number;
    excluded: Record<string, number>;
  };
  modules: Array<{
    container: string;
    source: string;
    object: string;
    members: Array<{ name: string; vram: string }>;
    status: "valid" | "invalid" | "undetermined";
    findings: string[];
    warnings: string[];
  }>;
  remaining: Array<{
    container: string;
    name: string;
    vram: string;
    source: string;
    neighbors: string[];
  }>;
  /** True when `remaining` lists fewer records than `inventory.remaining` counts. */
  remainingTruncated: boolean;
  /** Infrastructure failures: inventory conflicts, invalid units, orphans, unresolved containers. */
  failures: string[];
  integrity: {
    inventory: "pass" | "fail" | "undetermined";
    decompilation: GateState;
    policy: GateState;
    linkedBytes: GateState;
    convergence: GateState;
    whitespace: GateState;
    stability: GateState;
    exceptions: GateState;
  };
  /** One line per executed gate, in order. */
  gates: Array<{ gate: string; state: GateState; detail: string }>;
  /** Exception growth against the baseline, when one was given. */
  exceptionGrowth: ExceptionGrowth[];
  baseline: string | null;
}

interface Options {
  container?: string | undefined;
  json: boolean;
  remaining: boolean;
  checkComplete: boolean;
  verify: boolean;
  baseline?: string | undefined;
  writeBaseline?: string | undefined;
  limit: number;
}

function parseOptions(argv: string[]): Options {
  const value = (flag: string): string | undefined => {
    const index = argv.indexOf(flag);
    return index >= 0 && argv[index + 1] && !argv[index + 1]!.startsWith("--") ? argv[index + 1] : undefined;
  };
  const writeIndex = argv.indexOf("--write-baseline");
  return {
    container: value("--container"),
    json: argv.includes("--json"),
    remaining: argv.includes("--remaining"),
    checkComplete: argv.includes("--check-complete"),
    verify: argv.includes("--verify"),
    baseline: value("--baseline"),
    writeBaseline: writeIndex >= 0 ? value("--write-baseline") ?? DEFAULT_RECEIPT : undefined,
    limit: Number(value("--limit") ?? 25),
  };
}

function hex(value: number): string {
  return `0x${value.toString(16).toUpperCase().padStart(8, "0")}`;
}

/** Interface findings that make a module's interface wrong, not merely unwitnessed. */
const FAILING_INTERFACE = new Set(["conflict", "arity", "implicit", "restated"]);

/**
 * Module findings beyond membership: policy charged per member, interface
 * findings in the module file, and TU-owned global agreement for its members.
 */
function moduleFindingsHook(): (unit: TranslationUnit, source: string, facts: SourceFacts) => string[] {
  const config = loadConfig(ROOT);
  let interfaces: Finding[] | undefined;
  const interfaceFindings = (): Finding[] => {
    if (!interfaces) {
      const sources = activeSources();
      interfaces = auditInterfaces({ sources, sdk: sdkPrototypes(), published: publishedInterfaces(sources) }).findings;
    }
    return interfaces;
  };
  return (unit, source) => {
    const findings: string[] = [];
    const members = unit.members.map((member) => ({ name: member.name, vram: hex(member.vram), container: member.container }));
    for (const finding of moduleConstructFindings(source, config, members)) {
      findings.push(`policy: ${unit.sourcePath}${finding.line ? `:${finding.line}` : ""}: ${finding.kind}: ${finding.message}`);
    }
    for (const finding of interfaceFindings()) {
      if (finding.file === unit.sourcePath && FAILING_INTERFACE.has(finding.kind)) {
        findings.push(`interface: ${unit.sourcePath}:${finding.line}: ${finding.kind} ${finding.callee}${finding.differences ? ` [${finding.differences.join(", ")}]` : ""}`);
      }
    }
    if (unit.container === "exe") {
      for (const member of unit.members) {
        for (const record of deriveOwnership(member.name)) {
          for (const problem of witnessDisagreements(record)) findings.push(`ownership: ${problem}`);
        }
      }
    }
    return findings;
  };
}

interface FunctionState {
  container: Container;
  name: string;
  vram: number;
  unit: TranslationUnit | null;
  state: "folded" | "remaining" | "invalid" | "undetermined" | "excluded";
  exclusion?: string;
}

/** The structural report: inventory, units and the folded heuristic. Reads, never writes. */
export function structuralReport(options: { container?: string | undefined; limit: number; objects?: boolean }): TuProgressReport {
  const containers = options.container ? [requireContainer(options.container)] : loadContainers();
  const failures: string[] = [];
  const excluded: Record<string, number> = {};
  const exclude = (reason: string, count = 1) => { excluded[reason] = (excluded[reason] ?? 0) + count; };

  if (!options.container) {
    const unresolved = unresolvedCodeMembers();
    if (unresolved.length > 0) {
      exclude("unresolved-container", unresolved.length);
      failures.push(`code members without a solved load base: ${unresolved.map((m) => m.id).join(", ")} — their functions cannot be inventoried`);
    }
  }

  const liveness = computeLiveness();
  /* The rule progress.ts applies: a "Handwritten function" dump is assembly
     only when the macro census measured no COP2 instruction in it. */
  const macro = detectMacroIdentities({ containers });
  const cop2 = new Map(macro.functions.map((fn) => [fn.name, macroRouteSummary(fn).cop2Count]));
  const hook = moduleFindingsHook();
  const flagKeys = loadFlagOverrideKeys();

  const states: FunctionState[] = [];
  const modules: TuProgressReport["modules"] = [];
  const audits: ContainerAudit[] = [];
  for (const container of containers) {
    const inventory = loadOriginalFunctionInventory(container);
    for (const finding of inventory.findings) failures.push(`${container.id}: inventory: ${finding}`);
    const audit = auditContainer(container, { objects: options.objects !== false, moduleFindings: hook, flagOverrideKeys: flagKeys, largeModuleWarning: 64 });
    audits.push(audit);
    for (const finding of audit.findings) failures.push(`${container.id}: ${finding}`);
    const byUnit = new Map<string, UnitAudit>(audit.units.map((entry) => [entry.unit.segmentName, entry]));
    for (const entry of audit.units) {
      /* A per-function unit that fails its own membership is broken
         infrastructure, not backlog. */
      if (entry.status === "invalid" && !entry.unit.foldedModule) {
        for (const finding of entry.findings) failures.push(`${container.id}: ${finding}`);
      }
    }
    const libraries = inventory.functions.filter((fn) => fn.origin === "library").length;
    if (libraries > 0) exclude("library", libraries);

    const asmDir = join(containerPath(container, "asmDir"), "nonmatchings");
    for (const fn of inventory.functions) {
      if (fn.origin !== "game") continue;
      const unit = translationUnitOf(container, fn.name);
      const base = { container, name: fn.name, vram: fn.vram, unit };
      if (!unit) {
        states.push({ ...base, state: "excluded", exclusion: "handwritten-asm" });
        exclude("handwritten-asm");
        continue;
      }
      const entry = byUnit.get(unit.segmentName)!;
      const dump = [join(asmDir, unit.segmentName, `${fn.name}.s`), join(asmDir, fn.name, `${fn.name}.s`)].find((path) => existsSync(path));
      const handwritten = dump !== undefined && readFileSync(dump, "utf8").includes("Handwritten function") && cop2.get(fn.name) === 0;
      if (handwritten) {
        states.push({ ...base, state: "excluded", exclusion: "handwritten-asm" });
        exclude("handwritten-asm");
        continue;
      }
      if (entry.stubbed.has(fn.name) && !unit.foldedModule) {
        const dead = container.kind === "exe" && liveness.referenced.size > 0 && !liveness.referenced.has(fn.vram);
        const reason = dead ? "pending-dead-stub" : "pending-live-stub";
        states.push({ ...base, state: "excluded", exclusion: reason });
        exclude(reason);
        if (!dead) failures.push(`${container.id}: ${fn.name} is live and still an INCLUDE_ASM stub — the matching decompilation is not complete`);
        continue;
      }
      if (!unit.foldedModule) {
        states.push({ ...base, state: entry.status === "valid" ? "remaining" : entry.status === "invalid" ? "invalid" : "undetermined" });
        continue;
      }
      states.push({ ...base, state: entry.status === "valid" ? "folded" : entry.status === "invalid" ? "invalid" : "undetermined" });
    }
    for (const entry of audit.units) {
      if (!entry.unit.foldedModule) continue;
      modules.push({
        container: container.id,
        source: entry.unit.sourcePath,
        object: entry.unit.objectPath,
        members: entry.unit.members.map((member) => ({ name: member.name, vram: hex(member.vram) })),
        status: entry.status,
        findings: [...entry.findings, ...entry.undetermined],
        warnings: entry.warnings,
      });
    }
  }

  const eligible = states.filter((s) => s.state !== "excluded");
  const folded = eligible.filter((s) => s.state === "folded").length;
  const remainingStates = eligible.filter((s) => s.state === "remaining");
  const invalid = eligible.filter((s) => s.state === "invalid").length;
  const undetermined = eligible.filter((s) => s.state === "undetermined").length;
  if (eligible.length === 0) failures.push("no eligible function: an empty or unconfigured project never passes");
  const undeterminedUnits = audits.flatMap((audit) => audit.units.filter((u) => u.status === "undetermined"));

  const neighbors = (state: FunctionState): string[] => {
    const segments = loadSubsegments(state.container).filter((segment) => segment.size > 0 || segment.type === "c");
    const index = segments.findIndex((segment) => segment.type === "c" && segment.name === state.unit?.segmentName);
    const describe = (segment: (typeof segments)[number] | undefined) =>
      segment ? `${segment.type}${segment.name ? ` ${segment.name}` : ""}${segment.section ? ` ${segment.section}` : ""}` : "(none)";
    return [describe(segments[index - 1]), describe(segments[index + 1])];
  };

  const inventoryState: TuProgressReport["integrity"]["inventory"] =
    failures.length > 0 || invalid > 0 ? "fail" : undetermined > 0 || undeterminedUnits.length > 0 ? "undetermined" : "pass";
  if (undeterminedUnits.length > 0) {
    failures.push(`${undeterminedUnits.length} unit(s) undetermined, e.g. ${undeterminedUnits[0]!.undetermined[0]}`);
  }
  const limit = Number.isFinite(options.limit) && options.limit >= 0 ? options.limit : 25;
  return {
    schemaVersion: 1,
    complete: inventoryState === "pass" && remainingStates.length === 0 && folded > 0,
    inventory: { eligible: eligible.length, folded, remaining: remainingStates.length, undetermined, invalid, excluded },
    modules,
    remaining: remainingStates.slice(0, limit).map((state) => ({
      container: state.container.id,
      name: state.name,
      vram: hex(state.vram),
      source: state.unit?.sourcePath ?? "",
      neighbors: neighbors(state),
    })),
    remainingTruncated: remainingStates.length > limit,
    failures,
    integrity: { inventory: inventoryState, decompilation: "not-run", policy: "not-run", linkedBytes: "not-run", convergence: "not-run", whitespace: "not-run", stability: "not-run", exceptions: "not-run" },
    gates: [],
    exceptionGrowth: [],
    baseline: null,
  };
}

// --- the terminal gate --------------------------------------------------------

function run(command: string, args: string[], timeoutMs = 60 * 60_000): { code: number; output: string } {
  const result = spawnSync(command, args, { cwd: ROOT, encoding: "utf8", timeout: timeoutMs, maxBuffer: 256 * 1024 * 1024, env: { ...process.env, MAKEFLAGS: "" } });
  return { code: result.status ?? 1, output: `${result.stdout ?? ""}${result.stderr ?? ""}` };
}

function tail(text: string, lines = 6): string {
  return text.trim().split("\n").slice(-lines).join("\n");
}

/** The tool's own verdict line when it printed one, else its last lines. */
function verdict(text: string, pattern: RegExp, lines = 6): string {
  return text.split("\n").find((line) => pattern.test(line))?.trim() ?? tail(text, lines);
}

/** Content hashes of every file under the generated-config roots, for the convergence check. */
function snapshot(roots: string[]): Map<string, string> {
  const result = new Map<string, string>();
  const walk = (directory: string) => {
    if (!existsSync(directory)) return;
    for (const entry of readdirSync(directory, { withFileTypes: true })) {
      const path = join(directory, entry.name);
      if (entry.isDirectory()) walk(path);
      else if (entry.isFile()) result.set(relative(ROOT, path), createHash("sha256").update(readFileSync(path)).digest("hex"));
    }
  };
  for (const root of roots) walk(join(ROOT, root));
  return result;
}

function differences(before: Map<string, string>, after: Map<string, string>): string[] {
  const changed: string[] = [];
  for (const [path, hash] of after) if (before.get(path) !== hash) changed.push(path);
  for (const path of before.keys()) if (!after.has(path)) changed.push(path);
  return changed.sort();
}

/** The bytes of files a gate step may change, or null for a file that does not exist yet. */
function saveContents(paths: Iterable<string>): Map<string, Buffer | null> {
  const saved = new Map<string, Buffer | null>();
  for (const path of paths) {
    const absolute = join(ROOT, path);
    saved.set(path, existsSync(absolute) && statSync(absolute).isFile() ? readFileSync(absolute) : null);
  }
  return saved;
}

/** Put files back as saved, so verification never leaves the tree in a state the operator did not have. */
function restore(paths: readonly string[], saved: ReadonlyMap<string, Buffer | null>): void {
  for (const path of paths) {
    const content = saved.get(path) ?? null;
    const absolute = join(ROOT, path);
    if (content === null) rmSync(absolute, { force: true });
    else writeFileSync(absolute, content);
  }
}

/**
 * The terminal gate, in order. Every step runs; a failure is recorded and the
 * remaining steps still report, so one run names every broken invariant.
 */
export function verify(options: Options): TuProgressReport {
  const gates: TuProgressReport["gates"] = [];
  const record = (gate: string, state: GateState, detail: string) => gates.push({ gate, state, detail });
  const startInputs = inputsDigest();

  /* 1. Source/config structure before anything is built. */
  const before = structuralReport({ container: options.container, limit: options.limit, objects: false });
  record("inventory (source/config)", before.integrity.inventory === "pass" ? "pass" : "fail",
    before.integrity.inventory === "pass" ? `${before.inventory.eligible} eligible, ${before.inventory.folded} folded` : before.failures.slice(0, 3).join("; ") || "invalid modules");

  /* 2. The matching decompilation is still complete, read through units. */
  const complete = run("npx", ["tsx", "tools/diagnostics/progress.ts", "--check-complete"]);
  record("make complete", complete.code === 0 ? "pass" : "fail", complete.code === 0 ? "every live function is compiled C" : `progress --check-complete exited ${complete.code}`);

  /* 3. Repository-wide source policy, attributed per function. */
  const policy = run("npx", ["tsx", "tools/agent/sourcePolicy.ts", "--final"]);
  record("source policy", policy.code === 0 ? "pass" : "fail", policy.code === 0 ? verdict(policy.output, /^Source policy passed/) : tail(policy.output, 8));

  /* 4. Every container's linked bytes. */
  const build = run("make", [`-j${Math.min(8, availableParallelism())}`, "check-all"]);
  record("make check-all", build.code === 0 ? "pass" : "fail", build.code === 0 ? verdict(build.output, /^All \d+ overlay container/) : tail(build.output, 10));

  /* The object view is fresh only now. */
  const structural = structuralReport({ container: options.container, limit: options.limit });
  record("inventory (objects)", structural.integrity.inventory === "pass" ? "pass" : "fail",
    structural.integrity.inventory === "pass" ? `${structural.modules.length} module unit(s) valid` : structural.failures.slice(0, 3).join("; ") || "invalid modules");

  /* 5. Config generation reaches a fixed point: a re-split changes nothing. */
  const roots = ["configs", "include", "src"];
  const pre = snapshot(roots);
  const saved = saveContents(pre.keys());
  const split = run("make", ["split-all"]);
  const post = snapshot(roots);
  const drift = differences(pre, post);
  if (drift.length > 0) {
    /* Put back exactly what the gate found, so verification is not an edit. */
    restore(drift, saved);
    record("config convergence", "fail", `make split-all changed ${drift.length} file(s): ${drift.slice(0, 6).join(", ")}${drift.length > 6 ? ", …" : ""} (restored)`);
  } else {
    record("config convergence", split.code === 0 ? "pass" : "fail", split.code === 0 ? "make split-all changed no config, header or source" : tail(split.output, 6));
  }

  /* 6. No whitespace damage in the working-tree diff. */
  const whitespace = run("git", ["diff", "--check"]);
  record("git diff --check", whitespace.code === 0 ? "pass" : "fail", whitespace.code === 0 ? "clean" : tail(whitespace.output, 6));

  /* 7. Inputs did not change while verifying. */
  const endInputs = inputsDigest();
  record("input stability", endInputs === startInputs ? "pass" : "fail", endInputs === startInputs ? "inputs unchanged during verification" : "sources or configuration changed during verification — rerun");

  let growth: ExceptionGrowth[] = [];
  let baselineLabel: string | null = null;
  if (options.baseline) {
    const baseline = loadBaseline(options.baseline);
    baselineLabel = baseline.label;
    growth = exceptionGrowth(baseline.exceptions, currentExceptionSets());
    record("exception growth", growth.length === 0 ? "pass" : "fail",
      growth.length === 0 ? `no exception added since ${baseline.label}` : growth.map((g) => `${g.category}: ${g.added.slice(0, 4).join(", ")}`).join("; "));
  }

  const state = (name: string): GateState => gates.find((g) => g.gate === name)?.state ?? "not-run";
  const integrity: TuProgressReport["integrity"] = {
    inventory: structural.integrity.inventory === "pass" && before.integrity.inventory === "pass" ? "pass" : structural.integrity.inventory === "undetermined" ? "undetermined" : "fail",
    decompilation: state("make complete"),
    policy: state("source policy"),
    linkedBytes: state("make check-all"),
    convergence: state("config convergence"),
    whitespace: state("git diff --check"),
    stability: state("input stability"),
    exceptions: options.baseline ? state("exception growth") : "not-run",
  };
  const allPass = gates.every((g) => g.state === "pass");
  return { ...structural, integrity, gates, exceptionGrowth: growth, baseline: baselineLabel, complete: structural.complete && allPass };
}

// --- reporting -------------------------------------------------------------------

/** The 0/1/2 contract, from a report alone. */
export function exitCode(report: TuProgressReport): number {
  const broken = report.integrity.inventory !== "pass" || report.inventory.invalid > 0 || report.inventory.undetermined > 0 ||
    report.failures.length > 0 || report.gates.some((gate) => gate.state === "fail");
  if (broken) return 2;
  return report.complete ? 0 : 1;
}

function printReport(report: TuProgressReport, options: Options): void {
  const excluded = Object.entries(report.inventory.excluded).sort().map(([reason, count]) => `${count} ${reason}`).join(", ") || "none";
  console.log("Translation-unit reconstruction");
  console.log(`  Eligible C functions:  ${report.inventory.eligible}`);
  console.log(`  Folded into modules:   ${report.inventory.folded}`);
  console.log(`  Remaining:             ${report.inventory.remaining}`);
  if (report.inventory.invalid > 0) console.log(`  In invalid modules:    ${report.inventory.invalid}`);
  if (report.inventory.undetermined > 0) console.log(`  Undetermined:          ${report.inventory.undetermined}`);
  console.log(`  Valid module TUs:      ${report.modules.filter((m) => m.status === "valid").length} of ${report.modules.length}`);
  console.log(`  Excluded:              ${excluded}`);
  console.log();
  console.log("Project integrity");
  const label = (state: string) => state.toUpperCase().replace("NOT-RUN", "not run");
  const rows: Array<[string, string]> = [
    ["Original-function inventory", report.integrity.inventory],
    ["Decompilation completeness", report.integrity.decompilation],
    ["Source policy", report.integrity.policy],
    ["Linked container bytes", report.integrity.linkedBytes],
    ["Config convergence", report.integrity.convergence],
    ["Whitespace (git diff --check)", report.integrity.whitespace],
    ["Input stability", report.integrity.stability],
    ["Exception growth", report.integrity.exceptions],
  ];
  for (const [name, state] of rows) console.log(`  ${name.padEnd(31)} ${label(state)}`);
  for (const gate of report.gates) if (gate.state !== "pass") console.log(`    ${gate.gate}: ${gate.detail.split("\n").join("\n      ")}`);

  for (const failure of report.failures.slice(0, 20)) console.log(`  FAIL ${failure}`);
  if (report.failures.length > 20) console.log(`  … ${report.failures.length - 20} more failure(s)`);

  if (report.modules.length > 0) {
    console.log();
    console.log("Module units");
    for (const module of report.modules) {
      console.log(`  ${module.status.padEnd(12)} ${module.source} (${module.members.length} member${module.members.length === 1 ? "" : "s"})`);
      for (const finding of module.findings) console.log(`      ${finding}`);
      for (const warning of module.warnings) console.log(`      review: ${warning}`);
    }
  }

  if (options.remaining) {
    console.log();
    console.log("Remaining functions — a bounded, address-ordered listing, NOT a proposed group.");
    console.log("Membership evidence comes from source, callers, globals, link order and notes/file-groupings.md.");
    for (const entry of report.remaining) {
      console.log(`  ${entry.container.padEnd(7)} ${entry.vram}  ${entry.name.padEnd(28)} ${entry.source}  [prev: ${entry.neighbors[0]}; next: ${entry.neighbors[1]}]`);
    }
    if (report.remainingTruncated) console.log(`  … ${report.inventory.remaining - report.remaining.length} more (--limit N)`);
  }
}

function main(): void {
  const options = parseOptions(process.argv.slice(2));
  if (options.writeBaseline) {
    const receipt = captureReceipt(options.writeBaseline);
    const counts = Object.entries(receipt.exceptions).map(([category, entries]) => `${entries.length} ${category}`).join(", ");
    console.log(`Wrote ${options.writeBaseline}: ${counts} (inputs ${receipt.inputs.slice(0, 12)}, HEAD ${receipt.head?.slice(0, 12) ?? "none"})`);
    return;
  }
  const report = options.verify ? verify(options) : structuralReport({ container: options.container, limit: options.limit });
  if (!options.verify && options.baseline) {
    const baseline = loadBaseline(options.baseline);
    report.baseline = baseline.label;
    report.exceptionGrowth = exceptionGrowth(baseline.exceptions, currentExceptionSets());
    report.integrity.exceptions = report.exceptionGrowth.length === 0 ? "pass" : "fail";
    if (report.exceptionGrowth.length > 0) report.gates.push({ gate: "exception growth", state: "fail", detail: report.exceptionGrowth.map((g) => `${g.category}: ${g.added.join(", ")}`).join("; ") });
  }
  const code = exitCode(report);
  if (options.json) console.log(JSON.stringify(report, null, 2));
  else if (!options.checkComplete) printReport(report, options);
  process.exitCode = code;
}

if (process.argv[1]?.endsWith("tuProgress.ts")) main();
