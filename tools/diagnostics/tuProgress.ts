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
 *   npx tsx tools/diagnostics/tuProgress.ts --verify [--baseline <receipt|tree-ish>] [--full-convergence] [--no-cache]
 *   npx tsx tools/diagnostics/tuProgress.ts --write-baseline [<path>]   # campaign receipt
 *
 * --verify re-splits what changed before it builds, so it needs no hand-run
 * `make split` or `make check-all` beforehand. Its convergence step re-splits
 * only what changed since the last tree that converged (tools/lib/splitScope.ts),
 * the overlays in parallel, and keeps what the split regenerates — the headers
 * a rename implies — re-running until a pass changes nothing;
 * --full-convergence re-splits every container. Its report is cached under
 * build/tu-campaign/ by the tree object of the result (tracked and untracked
 * files, ignored ones, notes/, plans/ and top-level Markdown excepted), HEAD,
 * the index and the options; asking again about an unchanged tree returns the
 * same verdict at once, with only `git diff --check` re-run.
 *
 * Exit status — the same for every mode:
 *   0  every eligible function is folded and every requested gate passes;
 *   1  the inventory is valid and ordinary module work remains;
 *   2  configuration, membership, policy, build or another required
 *      invariant failed or is undetermined.
 */

import { spawnSync } from "node:child_process";
import { createHash } from "node:crypto";
import { copyFileSync, existsSync, mkdirSync, mkdtempSync, readFileSync, readdirSync, rmSync, statSync, writeFileSync } from "node:fs";
import { availableParallelism } from "node:os";
import { dirname, join, relative } from "node:path";
import { ROOT } from "../lib/psxExeInfo.js";
import { containerPath, loadContainers, requireContainer, unresolvedCodeMembers, type Container } from "../lib/container.js";
import { loadOriginalFunctionInventory, loadSubsegments } from "../lib/symbolIndex.js";
import { loadTranslationUnits, translationUnitOf, type TranslationUnit } from "../lib/translationUnits.js";
import { splitScope, splitToolClosure, type SplitScope } from "../lib/splitScope.js";
import { auditContainer, loadFlagOverrideKeys, type ContainerAudit, type SourceFacts, type UnitAudit } from "../lib/translationUnitAudit.js";
import { computeLiveness } from "../lib/liveness.js";
import { detectMacroIdentities } from "./macroIdentity.js";
import { macroRouteSummary } from "./macroTiler.js";
import { loadConfig } from "../../.pi/extensions/shared/config.ts";
import { moduleConstructFindings } from "../../.pi/extensions/shared/source-policy.ts";
import { activeSources } from "./canonicalNames.js";
import { auditInterfaces, publishedInterfaces, unitFacts, type Finding } from "./interfaceAudit.js";
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
  /** For --verify: the working-tree object judged, and when, if this verdict was reused rather than recomputed. */
  verifiedTree?: string;
  cachedFrom?: string;
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
  fullConvergence: boolean;
  noCache: boolean;
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
    fullConvergence: argv.includes("--full-convergence"),
    noCache: argv.includes("--no-cache"),
  };
}

function hex(value: number): string {
  return `0x${value.toString(16).toUpperCase().padStart(8, "0")}`;
}

/** Interface findings that make a module's interface wrong, not merely unwitnessed. */
const FAILING_INTERFACE = new Set(["conflict", "arity", "implicit", "restated"]);

/**
 * The interface findings the audit makes in these module files.
 *
 * A finding in a file depends on that file and on the definitions of the
 * names it declares or calls, so the audit reads those and nothing else: the
 * TU index places a function in the inventory, and a text search finds any
 * other definer. Memoised on the files it read, because --verify asks twice
 * of a tree that has not changed.
 */
let interfaceMemo: { key: string; findings: Finding[] } | undefined;
export function moduleInterfaceFindings(modulePaths: readonly string[]): Finding[] {
  if (modulePaths.length === 0) return [];
  const all = activeSources();
  const subset = new Map<string, string>();
  const names = new Set<string>();
  for (const path of modulePaths) {
    const text = all.get(path);
    if (text === undefined) continue;
    subset.set(path, text);
    const facts = unitFacts(text, path);
    for (const declared of facts.declarations) names.add(declared.prototype.name);
    for (const call of facts.calls) names.add(call.callee);
    for (const label of facts.labels.values()) names.add(label);
  }
  const containers = loadContainers();
  const sources = [...all.keys()].filter((path) => path.endsWith(".c"));
  for (const name of names) {
    const units = containers.map((container) => translationUnitOf(container, name)).filter((unit): unit is TranslationUnit => !!unit);
    if (units.length > 0) {
      for (const unit of units) if (all.has(unit.sourcePath)) subset.set(unit.sourcePath, all.get(unit.sourcePath)!);
      continue;
    }
    /* C identifiers carry no regular-expression metacharacters. */
    const word = new RegExp(`\\b${name}\\b`);
    for (const path of sources) if (!subset.has(path) && all.get(path)!.includes(name) && word.test(all.get(path)!)) subset.set(path, all.get(path)!);
  }
  const key = createHash("sha256").update(JSON.stringify([...modulePaths].sort())).update(JSON.stringify([...subset].sort())).digest("hex");
  if (interfaceMemo?.key !== key) {
    const findings = auditInterfaces({ sources: subset, sdk: sdkPrototypes(), published: publishedInterfaces(subset) }).findings;
    interfaceMemo = { key, findings: findings.filter((finding) => modulePaths.includes(finding.file)) };
  }
  return interfaceMemo.findings;
}

/**
 * Module findings beyond membership: policy charged per member, interface
 * findings in the module file, and TU-owned global agreement for its members.
 */
function moduleFindingsHook(modulePaths: readonly string[]): (unit: TranslationUnit, source: string, facts: SourceFacts) => string[] {
  const config = loadConfig(ROOT);
  let interfaces: Finding[] | undefined;
  const interfaceFindings = (): Finding[] => interfaces ??= moduleInterfaceFindings(modulePaths);
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
     only when the macro census measured no COP2 instruction in it. Only those
     dumps need the census, so it measures only them. */
  const dumpOf = (container: Container, unit: TranslationUnit, name: string): string | undefined => {
    const asmDir = join(containerPath(container, "asmDir"), "nonmatchings");
    return [join(asmDir, unit.segmentName, `${name}.s`), join(asmDir, name, `${name}.s`)].find((path) => existsSync(path));
  };
  const handwrittenDumps = new Set<string>();
  for (const container of containers) {
    for (const fn of loadOriginalFunctionInventory(container).functions) {
      const unit = fn.origin === "game" ? translationUnitOf(container, fn.name) : null;
      const dump = unit ? dumpOf(container, unit, fn.name) : undefined;
      if (dump && readFileSync(dump, "utf8").includes("Handwritten function")) handwrittenDumps.add(`${container.id}:${fn.name}`);
    }
  }
  const candidates = [...handwrittenDumps].map((key) => key.slice(key.indexOf(":") + 1));
  const macro = candidates.length > 0 ? detectMacroIdentities({ containers, functionNames: candidates }).functions : [];
  const cop2 = new Map(macro.map((fn) => [fn.name, macroRouteSummary(fn).cop2Count]));
  const modulePaths = containers.flatMap((container) => loadTranslationUnits(container).filter((unit) => unit.foldedModule).map((unit) => unit.sourcePath));
  const hook = moduleFindingsHook(modulePaths);
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
      const handwritten = handwrittenDumps.has(`${container.id}:${fn.name}`) && cop2.get(fn.name) === 0;
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
 * Paths no gate reads except `git diff --check`, which a reused verdict re-runs:
 * an edit there leaves the rest of a verdict standing.
 */
const NOT_GATE_INPUTS = ["notes", "plans", ":(glob)*.md"];

/**
 * The tree object of the gate inputs in the working tree as it stands —
 * tracked and untracked files, ignored ones and NOT_GATE_INPUTS excepted — or
 * in the index.
 */
function treeOf(source: "worktree" | "index"): string | null {
  /* Under build/, not the system temp directory: a full tmpfs must not break the gate. */
  mkdirSync(join(ROOT, "build/tu-campaign"), { recursive: true });
  const directory = mkdtempSync(join(ROOT, "build/tu-campaign", "tree-"));
  try {
    const index = join(directory, "index");
    const gitIndex = spawnSync("git", ["rev-parse", "--git-path", "index"], { cwd: ROOT, encoding: "utf8" }).stdout.trim();
    if (gitIndex && existsSync(join(ROOT, gitIndex))) copyFileSync(join(ROOT, gitIndex), index);
    const env = { ...process.env, GIT_INDEX_FILE: index };
    if (source === "worktree" && spawnSync("git", ["add", "-A", "--", "."], { cwd: ROOT, env, encoding: "utf8" }).status !== 0) return null;
    if (spawnSync("git", ["rm", "-r", "-q", "--cached", "--ignore-unmatch", "--", ...NOT_GATE_INPUTS], { cwd: ROOT, env, encoding: "utf8" }).status !== 0) return null;
    const tree = spawnSync("git", ["write-tree"], { cwd: ROOT, env, encoding: "utf8" });
    return tree.status === 0 ? tree.stdout.trim() : null;
  } finally {
    rmSync(directory, { recursive: true, force: true });
  }
}

function gitLine(args: string[]): string | null {
  const result = spawnSync("git", args, { cwd: ROOT, encoding: "utf8" });
  return result.status === 0 ? result.stdout.trim() : null;
}

function fileDigest(path: string): string | null {
  const absolute = join(ROOT, path);
  return existsSync(absolute) ? createHash("sha256").update(readFileSync(absolute)).digest("hex") : null;
}

// --- convergence ---------------------------------------------------------------

/** The last tree a convergence step passed on, and the engine export the overlays were split against then. */
const CONVERGENCE_RECEIPT = "build/tu-campaign/convergence.json";
const ENGINE_EXPORT = "build/engine_syms.txt";

interface ConvergenceReceipt { tree: string; engineExport: string | null; at: string }

function readJson<T>(path: string): T | null {
  try { return JSON.parse(readFileSync(join(ROOT, path), "utf8")) as T; } catch { return null; }
}

function writeJson(path: string, value: unknown): void {
  mkdirSync(dirname(join(ROOT, path)), { recursive: true });
  writeFileSync(join(ROOT, path), JSON.stringify(value, null, 2) + "\n");
}

/** The re-splits this tree needs, against the last tree that converged; null for every container. */
function convergenceScope(tree: string | null, full: boolean): { scope: SplitScope | null; why: string } {
  if (full) return { scope: null, why: "--full-convergence" };
  const receipt = readJson<ConvergenceReceipt>(CONVERGENCE_RECEIPT);
  if (!tree || !receipt || gitLine(["cat-file", "-t", receipt.tree]) !== "tree") return { scope: null, why: "no converged tree on record" };
  const changed = gitLine(["diff", "--name-only", "--no-renames", receipt.tree, tree]);
  if (changed === null) return { scope: null, why: "the converged tree cannot be compared" };
  const scope = splitScope(changed ? changed.split("\n") : [], overlayIds(), splitToolClosure(ROOT));
  return scope.full ? { scope: null, why: scope.reasons.slice(0, 2).join("; ") } : { scope, why: `since ${receipt.tree.slice(0, 12)}` };
}

function overlayIds(): string[] {
  return loadContainers().filter((container) => container.kind === "overlay").map((container) => container.id);
}

/**
 * The overlay splits a set of overlays needs, in waves whose splits run in
 * parallel. An overlay whose split reads another's symbol table (its splat
 * config names that table, for cross-slot calls) joins the set whenever the
 * other is in it, and splits in a later wave.
 */
function overlaySplitWaves(ids: Iterable<string>): string[][] {
  const overlays = loadContainers().filter((container) => container.kind === "overlay");
  const reads = new Map(overlays.map((reader) => {
    const config = existsSync(join(ROOT, reader.paths.splat)) ? readFileSync(join(ROOT, reader.paths.splat), "utf8") : "";
    return [reader.id, overlays.filter((other) => other.id !== reader.id && config.includes(other.paths.symbolAddrs)).map((other) => other.id)];
  }));
  const wanted = new Set(ids);
  for (let grew = true; grew;) {
    grew = false;
    for (const [reader, sources] of reads) {
      if (wanted.has(reader) || !sources.some((id) => wanted.has(id))) continue;
      wanted.add(reader);
      grew = true;
    }
  }
  let left = overlays.map((overlay) => overlay.id).filter((id) => wanted.has(id));
  const waves: string[][] = [];
  while (left.length > 0) {
    const wave = left.filter((id) => !reads.get(id)!.some((source) => left.includes(source)));
    /* A cycle: one at a time, in configuration order, as `make split-all` runs them. */
    if (wave.length === 0) return [...waves, ...left.map((id) => [id])];
    waves.push(wave);
    left = left.filter((id) => !wave.includes(id));
  }
  return waves;
}

/** Regeneration passes the convergence step runs before it calls a tree unsettled. */
const CONVERGENCE_PASSES = 3;

function listed(paths: readonly string[]): string {
  return `${paths.slice(0, 6).join(", ")}${paths.length > 6 ? ", …" : ""}`;
}

/**
 * Config generation reaches a fixed point. Only the splits this tree's changes
 * can reach are re-run, the overlays in parallel. What a pass regenerates — the
 * headers a rename or a fold implies — is kept, as a hand-run split would keep
 * it, and the next pass re-runs only what that touched, until a pass changes
 * nothing. A split that fails, or a tree that has not settled after
 * CONVERGENCE_PASSES, is put back as it was.
 */
function convergence(tree: string | null, full: boolean): { state: GateState; detail: string; regenerated: string[]; engineExport: string | null } {
  const roots = ["configs", "include", "src"];
  const pre = snapshot(roots);
  const saved = saveContents(pre.keys());
  const passes: string[][] = [];
  let failure: string | undefined;
  const step = (label: string, command: string, args: string[]): void => {
    if (failure) return;
    passes.at(-1)!.push(label);
    const result = run(command, args);
    if (result.code !== 0) failure = `${label} exited ${result.code}:\n${tail(result.output, 6)}`;
  };
  const tool = (path: string, flag: string) => step(path.replace(/^tools\/[^/]+\//, ""), "npx", ["tsx", path, flag]);
  /* The export the overlays were last split against. */
  let engine = readJson<ConvergenceReceipt>(CONVERGENCE_RECEIPT)?.engineExport ?? null;
  const pass = (scope: SplitScope | null) => {
    passes.push([]);
    let overlays = scope?.overlays ?? overlayIds();
    if (scope === null || scope.exe) {
      step("split", "make", ["split"]);
      step(ENGINE_EXPORT, "make", ["--no-print-directory", ENGINE_EXPORT]);
      /* The overlays name their external calls from the engine export. */
      const exported = fileDigest(ENGINE_EXPORT);
      if (exported !== engine) overlays = overlayIds();
      engine = exported;
    }
    for (const wave of overlaySplitWaves(overlays)) {
      step(wave.length === 1 ? `split-${wave[0]}` : `split-{${wave.join(",")}}`, "make", ["-k", `-j${Math.min(wave.length, availableParallelism())}`, ...wave.map((id) => `split-${id}`)]);
    }
    if (scope !== null && !scope.exe && scope.tail) {
      /* The EXE split's cross-container tail, which reads every source and header. */
      tool("tools/agent/contextExport.ts", "--all");
      tool("tools/build/classifyGlobals.ts", "--write");
      tool("tools/build/genProjectProfile.ts", "--write");
    }
  };

  const { scope, why } = convergenceScope(tree, full);
  const regenerated = new Set<string>();
  let unsettled: string[] = [];
  let before = pre;
  for (let next: SplitScope | null = scope, count = 1; ; count++) {
    pass(next);
    if (failure) break;
    const after = snapshot(roots);
    const changed = differences(before, after);
    if (changed.length === 0) break;
    for (const path of changed) regenerated.add(path);
    if (count === CONVERGENCE_PASSES) { unsettled = changed; break; }
    const touched = splitScope(changed, overlayIds(), splitToolClosure(ROOT));
    next = touched.full ? null : touched;
    before = after;
  }

  const ran = passes.flat().length === 0 ? `no split input changed ${why}` : `${passes.filter((p) => p.length).map((p) => p.join(", ")).join("; then ")} (${why})`;
  if (failure || unsettled.length > 0) {
    /* Put back what the passes changed, so a failed gate is not an edit. */
    const drift = differences(pre, snapshot(roots));
    restore(drift, saved);
    const restored = drift.length > 0 ? ` (restored ${drift.length} file(s))` : "";
    return {
      state: "fail", regenerated: [], engineExport: engine,
      detail: failure ? `${failure}${restored}` : `${ran} still changed ${unsettled.length} file(s) on pass ${CONVERGENCE_PASSES}: ${listed(unsettled)}${restored}`,
    };
  }
  const kept = [...regenerated].sort();
  return {
    state: "pass", regenerated: kept, engineExport: engine,
    detail: passes.flat().length === 0 ? ran : kept.length === 0 ? `${ran} changed no config, header or source` : `${ran} regenerated ${kept.length} file(s), kept: ${listed(kept)}`,
  };
}

// --- the verdict cache -----------------------------------------------------------

const VERIFY_CACHE = "build/tu-campaign/verify-cache.json";
const VERIFY_CACHE_ENTRIES = 16;

interface CachedVerdict { key: string; tree: string; at: string; report: TuProgressReport; structuralComplete: boolean }

/**
 * What a verdict depends on besides the tree: HEAD and the index, the scope and
 * the baseline. Not --limit: a reused verdict lists the remaining functions as
 * the run that reached it did.
 */
function verdictKey(options: Options, tree: string): string | null {
  const head = gitLine(["rev-parse", "HEAD"]);
  const index = treeOf("index");
  if (!index) return null;
  const baseline = options.baseline === undefined ? null
    : existsSync(join(ROOT, options.baseline)) ? fileDigest(options.baseline) : gitLine(["rev-parse", "--verify", `${options.baseline}^{commit}`]);
  return createHash("sha256").update(JSON.stringify({
    version: 2, tree, head, index, container: options.container ?? null,
    baseline: options.baseline === undefined ? null : [options.baseline, baseline], fullConvergence: options.fullConvergence,
  })).digest("hex");
}

/** No whitespace damage in the working-tree diff: the one gate that reads NOT_GATE_INPUTS, so never reused. */
function whitespaceGate(): { state: GateState; detail: string } {
  const result = run("git", ["diff", "--check"]);
  return result.code === 0 ? { state: "pass", detail: "clean" } : { state: "fail", detail: tail(result.output, 6) };
}

/**
 * The terminal gate, in order. Every step runs; a failure is recorded and the
 * remaining steps still report, so one run names every broken invariant. A
 * tree already judged under the same HEAD, index, options and baseline gets
 * the same verdict back with only the whitespace gate re-run.
 */
export function verify(options: Options): TuProgressReport {
  const tree = treeOf("worktree");
  const key = tree ? verdictKey(options, tree) : null;
  const cache = readJson<CachedVerdict[]>(VERIFY_CACHE) ?? [];
  const hit = key ? cache.find((entry) => entry.key === key) : undefined;
  if (hit && !options.noCache) {
    console.error(`tuProgress: this tree (${hit.tree.slice(0, 12)}) was verified at ${hit.at}; reusing that verdict (--no-cache re-runs it)`);
    const whitespace = whitespaceGate();
    const gates = hit.report.gates.map((gate) => gate.gate === "git diff --check" ? { ...gate, ...whitespace } : gate);
    return {
      ...hit.report, gates, integrity: { ...hit.report.integrity, whitespace: whitespace.state },
      complete: hit.structuralComplete && gates.every((gate) => gate.state === "pass"), verifiedTree: hit.tree, cachedFrom: hit.at,
    };
  }

  const gates: TuProgressReport["gates"] = [];
  const record = (gate: string, state: GateState, detail: string) => gates.push({ gate, state, detail });

  /* 1. Config generation reaches a fixed point. First, because every later
     gate reads what the split produces: after a rename or a fold, verifying
     needs no hand-run split, and keeps what the split regenerates. */
  const converged = convergence(tree, options.fullConvergence);
  record("config convergence", converged.state, converged.detail);
  /* The tree the remaining gates judge, and the verdict is cached under. */
  const judged = converged.regenerated.length > 0 ? treeOf("worktree") : tree;
  const judgedKey = judged === tree ? key : judged ? verdictKey(options, judged) : null;
  const startInputs = inputsDigest();

  /* 2. Source/config structure, against the split it was just given. */
  const before = structuralReport({ container: options.container, limit: options.limit, objects: false });
  record("inventory (source/config)", before.integrity.inventory === "pass" ? "pass" : "fail",
    before.integrity.inventory === "pass" ? `${before.inventory.eligible} eligible, ${before.inventory.folded} folded` : before.failures.slice(0, 3).join("; ") || "invalid modules");

  /* 3. The matching decompilation is still complete, read through units. */
  const complete = run("npx", ["tsx", "tools/diagnostics/progress.ts", "--check-complete"]);
  record("make complete", complete.code === 0 ? "pass" : "fail", complete.code === 0 ? "every live function is compiled C" : `progress --check-complete exited ${complete.code}`);

  /* 4. Repository-wide source policy, attributed per function. */
  const policy = run("npx", ["tsx", "tools/agent/sourcePolicy.ts", "--final"]);
  record("source policy", policy.code === 0 ? "pass" : "fail", policy.code === 0 ? verdict(policy.output, /^Source policy passed/) : tail(policy.output, 8));

  /* 5. Every container's linked bytes, built from what the split produced. */
  const build = run("make", [`-j${Math.min(8, availableParallelism())}`, "check-all"]);
  record("make check-all", build.code === 0 ? "pass" : "fail", build.code === 0 ? verdict(build.output, /^All \d+ overlay container/) : tail(build.output, 10));

  /* The object view is fresh only now. */
  const structural = structuralReport({ container: options.container, limit: options.limit });
  record("inventory (objects)", structural.integrity.inventory === "pass" ? "pass" : "fail",
    structural.integrity.inventory === "pass" ? `${structural.modules.length} module unit(s) valid` : structural.failures.slice(0, 3).join("; ") || "invalid modules");

  /* 6. No whitespace damage in the working-tree diff. */
  const whitespace = whitespaceGate();
  record("git diff --check", whitespace.state, whitespace.detail);

  /* 7. Inputs did not change while verifying. Any other edit leaves the verdict
     standing but unattributable to one tree, so it is not cached. */
  const stable = inputsDigest() === startInputs;
  record("input stability", stable ? "pass" : "fail", stable ? "inputs unchanged during verification" : "sources or configuration changed during verification — rerun");
  const sameTree = stable && judged !== null && treeOf("worktree") === judged;

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
  const report: TuProgressReport = { ...structural, integrity, gates, exceptionGrowth: growth, baseline: baselineLabel, complete: structural.complete && allPass };
  if (sameTree) {
    const at = new Date().toISOString();
    if (converged.state === "pass") writeJson(CONVERGENCE_RECEIPT, { tree: judged, engineExport: converged.engineExport, at } satisfies ConvergenceReceipt);
    if (judgedKey) {
      const entry: CachedVerdict = { key: judgedKey, tree: judged!, at, report, structuralComplete: structural.complete };
      writeJson(VERIFY_CACHE, [entry, ...cache.filter((cached) => cached.key !== judgedKey)].slice(0, VERIFY_CACHE_ENTRIES));
    }
  }
  return { ...report, ...(judged ? { verifiedTree: judged } : {}) };
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
