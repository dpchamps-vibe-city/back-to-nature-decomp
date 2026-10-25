import { execFileSync } from "node:child_process";
import { copyFileSync, existsSync, mkdirSync, readFileSync, renameSync, rmSync, writeFileSync } from "node:fs";
import { dirname, join } from "node:path";
import { runCommand } from "../../shared/process.ts";
import { newChangesSince, snapshotDirt } from "../../shared/workspace.ts";
import { loadConfig } from "../../shared/config.ts";
import { withinAllowedRoots } from "../../shared/source-policy.ts";
import type { TuProgressReport } from "../../../../tools/diagnostics/tuProgress.ts";
import { commitFold } from "./commit.ts";
import { applyTier, captureHandoff, clearContext, notify, setStatus, turn, type LoopDeps } from "./loop.ts";
import { loopChangedFiles, scopedLoopChanges, type OracleContext } from "./oracles.ts";
import { foldEscalationMessage, foldNudgeMessage, foldOpeningMessage, type FoldBrief } from "./prompts.ts";
import { isNotesPath } from "./scope-guard.ts";
import { readState, recordFold, writeState } from "./state.ts";
import type { FoldRecord, FunctionOutcome, HandoffSummary, LoopState } from "./types.ts";

/**
 * `/auto_decompilation_loop fold` — reconstruct source modules until every
 * eligible function is folded into one.
 *
 * Each dispatch hands one agent `plans/tu-prompt.md`: find one evidence-backed
 * group, fold it into a semantic module, name and document it, keep every byte.
 * The loop never trusts the agent's account. Progress is re-derived from the
 * source tree, configuration and objects by `tools/diagnostics/tuProgress.ts`,
 * and a module is accepted only when
 *
 *  - the folded count strictly rises and no previously folded function regresses;
 *  - the new folded functions belong to one module, and no other module changes
 *    what it holds (another module may change: a rename reaches its callers);
 *  - the terminal gate (`tuProgress.ts --verify`) passes, including no exception
 *    growth against the campaign's baseline receipt. Its verdict is cached by
 *    tree, so the agent's own run on an unchanged tree is reused; the gate that
 *    ends the campaign re-splits every container.
 *
 * An accepted module is committed as `fold <module>` when `commitOnMatch` is
 * set — the whole change set the gate judged, tooling fixes included; files
 * outside the integration roots are named in the commit, not left behind.
 * What is dirty when the loop starts, and after each commit, is pre-existing by
 * content: a file a dispatch edits on top of it is that dispatch's change. A
 * dispatch that ends without an accepted module restores the tree it
 * started from and stops the loop: a gate that fails after a full escalation is
 * evidence that the infrastructure hypothesis is wrong, not that the next
 * module deserves a try. Typing any message stops the loop between returns and
 * leaves the tree exactly as the last return left it; the next `fold` run
 * settles that interrupted dispatch first.
 */

/** The task every dispatch is given, verbatim. */
export const FOLD_TASK = "plans/tu-prompt.md";
/** The campaign's baseline receipt, under ignored build/. */
export const FOLD_RECEIPT = "build/tu-campaign/baseline.json";

export interface ProgressReading {
  code: number;
  report: TuProgressReport | null;
  output: string;
}

/** The measurements a fold runs. Injectable so the controller can be exercised without a toolchain. */
export interface FoldOracles {
  progress(projectRoot: string, signal?: AbortSignal): Promise<ProgressReading>;
  /** The terminal gate; `full` re-splits every container rather than what changed since the last converged tree. */
  verify(projectRoot: string, baseline: string, signal?: AbortSignal, full?: boolean): Promise<ProgressReading>;
  captureBaseline(projectRoot: string, path: string): Promise<{ ok: boolean; detail: string }>;
}

async function tuProgress(projectRoot: string, args: string[], timeoutMs: number, signal?: AbortSignal): Promise<ProgressReading> {
  const result = await runCommand("npx", ["tsx", "tools/diagnostics/tuProgress.ts", ...args], {
    cwd: projectRoot, timeoutMs, signal, maxCaptureBytes: 64 * 1024 * 1024,
  });
  let report: TuProgressReport | null = null;
  try { report = JSON.parse(result.stdout) as TuProgressReport; } catch { /* reported below */ }
  return { code: result.code, report, output: `${result.stderr}${report ? "" : result.stdout}`.trim() };
}

export const DEFAULT_FOLD_ORACLES: FoldOracles = {
  progress: (projectRoot, signal) => tuProgress(projectRoot, ["--json", "--limit", "20"], 15 * 60_000, signal),
  verify: (projectRoot, baseline, signal, full) =>
    tuProgress(projectRoot, ["--verify", "--json", "--limit", "20", "--baseline", baseline, ...(full ? ["--full-convergence"] : [])], 3 * 60 * 60_000, signal),
  captureBaseline: async (projectRoot, path) => {
    const result = await runCommand("npx", ["tsx", "tools/diagnostics/tuProgress.ts", "--write-baseline", path], { cwd: projectRoot, timeoutMs: 15 * 60_000 });
    return { ok: result.code === 0, detail: `${result.stdout}${result.stderr}`.trim() };
  },
};

/** `<container>:<vram>` for every member of a valid module. */
export function foldedIdentities(report: TuProgressReport): Set<string> {
  const folded = new Set<string>();
  for (const module of report.modules) {
    if (module.status !== "valid") continue;
    for (const member of module.members) folded.add(`${module.container}:${member.vram}`);
  }
  return folded;
}

const MODULE_SOURCE = /^src\/(?:overlays\/[^/]+\/)?modules\/[^/]+\.c$/;

export interface FoldJudgement {
  ok: boolean;
  reasons: string[];
  module?: TuProgressReport["modules"][number];
}

/**
 * Whether one dispatch's tree moved the campaign by exactly one coherent
 * module. Structure only; the terminal gate is run separately.
 */
export function judgeFold(before: TuProgressReport, after: TuProgressReport, changedFiles: readonly string[]): FoldJudgement {
  const reasons: string[] = [];
  if (after.failures.length > 0 || after.integrity.inventory !== "pass") {
    reasons.push(`the inventory is ${after.integrity.inventory}: ${after.failures.slice(0, 3).join("; ") || "see tuProgress.ts"}`);
  }
  if (after.inventory.folded <= before.inventory.folded) {
    reasons.push(`the folded count did not rise (${before.inventory.folded} -> ${after.inventory.folded})`);
  }
  const was = foldedIdentities(before), now = foldedIdentities(after);
  const lost = [...was].filter((identity) => !now.has(identity));
  if (lost.length > 0) reasons.push(`previously folded function(s) regressed: ${lost.slice(0, 6).join(", ")}`);
  const gained = [...now].filter((identity) => !was.has(identity));
  const owners = new Set(after.modules.filter((m) => m.status === "valid" && m.members.some((member) => gained.includes(`${m.container}:${member.vram}`))).map((m) => m.source));
  if (owners.size > 1) reasons.push(`newly folded functions span ${owners.size} modules (${[...owners].join(", ")}); one dispatch reconstructs one module`);
  /* Other modules may change — a rename reaches callers wherever they live —
     but only the reconstructed one may change what it holds. */
  const modules = new Set(changedFiles.filter((file) => MODULE_SOURCE.test(file)));
  const membership = (report: TuProgressReport, source: string) =>
    report.modules.find((m) => m.source === source)?.members.map((member) => `${member.vram}`).sort().join(",") ?? "";
  for (const source of modules) {
    if (owners.has(source)) continue;
    if (membership(before, source) !== membership(after, source)) {
      reasons.push(`${source} changed which functions it holds; one dispatch reconstructs one module`);
    }
  }
  for (const module of after.modules) {
    if (module.status !== "valid" && modules.has(module.source)) reasons.push(`${module.source} is ${module.status}: ${module.findings.slice(0, 3).join("; ")}`);
  }
  const module = owners.size === 1 ? after.modules.find((m) => m.source === [...owners][0]) : undefined;
  return { ok: reasons.length === 0, reasons, ...(module ? { module } : {}) };
}

/** A gate reading as the report a dispatch needs: the gates that failed, then the structural failures. */
export function foldGateReport(reading: ProgressReading): string {
  if (!reading.report) return `tuProgress.ts --verify did not produce a report (exit ${reading.code}):\n${reading.output.split("\n").slice(-12).join("\n")}`;
  const failed = reading.report.gates.filter((gate) => gate.state !== "pass");
  return [
    `Terminal gate: exit ${reading.code}.`,
    ...failed.map((gate) => `  ${gate.gate}: ${gate.detail}`),
    ...reading.report.failures.slice(0, 8).map((failure) => `  failure: ${failure}`),
  ].join("\n");
}

function structureReport(reading: ProgressReading, judgement?: FoldJudgement): string {
  if (!reading.report) return `tuProgress.ts did not produce a report (exit ${reading.code}):\n${reading.output.split("\n").slice(-12).join("\n")}`;
  const report = reading.report;
  return [
    `tuProgress.ts: exit ${reading.code}; folded ${report.inventory.folded} of ${report.inventory.eligible}, remaining ${report.inventory.remaining}.`,
    ...(judgement?.reasons ?? []).map((reason) => `  not accepted: ${reason}`),
    ...report.modules.filter((m) => m.status !== "valid").flatMap((m) => [`  ${m.source} is ${m.status}:`, ...m.findings.slice(0, 6).map((f) => `    ${f}`)]),
    ...report.failures.slice(0, 6).map((failure) => `  failure: ${failure}`),
  ].join("\n");
}

async function git(projectRoot: string, args: string[]): Promise<{ code: number; stdout: string; stderr: string }> {
  const result = await runCommand("git", args, { cwd: projectRoot, timeoutMs: 60_000 });
  return { code: result.code, stdout: result.stdout, stderr: result.stderr };
}

async function headCommit(projectRoot: string): Promise<string> {
  const result = await git(projectRoot, ["rev-parse", "HEAD"]);
  if (result.code !== 0) throw new Error(`git rev-parse HEAD failed: ${result.stderr}`);
  return result.stdout.trim();
}

/**
 * Put exactly these files back as the dispatch found them, notes excepted:
 * as HEAD has them, or, for a file that was already dirty, with the content
 * it had then (`preexisting`), so a failed fold never discards older work.
 *
 * The change set is archived first; a file the dispatch created is moved into
 * the archive rather than deleted.
 */
async function restoreFiles(deps: LoopDeps, files: readonly string[], stamp: string, preexisting: Readonly<Record<string, string | null>> = {}): Promise<{ patch?: string; moved: string[] }> {
  const restore = files.filter((file) => !isNotesPath(file));
  if (restore.length === 0) return { moved: [] };
  const archiveDir = join(deps.config.runtimeDir, "folds", stamp.replace(/[:.]/g, "-"));
  mkdirSync(archiveDir, { recursive: true });
  const tracked: string[] = [];
  const created: string[] = [];
  for (const file of restore) {
    if (file in preexisting) {
      const path = join(deps.projectRoot, file);
      if (existsSync(path)) {
        const target = join(archiveDir, "dispatch", file);
        mkdirSync(dirname(target), { recursive: true });
        copyFileSync(path, target);
      }
      const blob = preexisting[file];
      if (blob === null || blob === undefined) rmSync(path, { force: true });
      else {
        mkdirSync(dirname(path), { recursive: true });
        writeFileSync(path, execFileSync("git", ["cat-file", "blob", blob], { cwd: deps.projectRoot, maxBuffer: 256 * 1024 * 1024 }));
      }
      continue;
    }
    const known = await git(deps.projectRoot, ["ls-tree", "--name-only", "HEAD", "--", file]);
    (known.stdout.trim() ? tracked : created).push(file);
  }
  let patch: string | undefined;
  if (tracked.length) {
    const diff = await git(deps.projectRoot, ["diff", "HEAD", "--", ...tracked]);
    if (diff.stdout.trim()) writeFileSync(patch = join(archiveDir, "changes.patch"), diff.stdout);
    const checkout = await git(deps.projectRoot, ["checkout", "HEAD", "--", ...tracked]);
    if (checkout.code !== 0) throw new Error(`Restoring ${tracked.join(", ")} failed: ${checkout.stderr}`);
  }
  const moved: string[] = [];
  for (const file of created) {
    const source = join(deps.projectRoot, file);
    if (!existsSync(source)) continue;
    const target = join(archiveDir, "created", file);
    mkdirSync(dirname(target), { recursive: true });
    renameSync(source, target);
    moved.push(file);
  }
  return { ...(patch ? { patch } : {}), moved };
}

function oracleContext(deps: LoopDeps, state: LoopState, signal?: AbortSignal): OracleContext {
  return { projectRoot: deps.projectRoot, baseline: deps.baseline, state, ...(signal ? { signal } : {}) };
}

/** The changed files that were already dirty before the dispatch, with the content they had then. */
function preexistingOf(deps: LoopDeps, changedFiles: readonly string[]): Record<string, string | null> {
  return Object.fromEntries(changedFiles.filter((file) => deps.baseline.has(file)).map((file) => [file, deps.baseline.get(file) ?? null]));
}

function moduleNameOf(source: string): string {
  return source.replace(/^.*\/modules\//, "").replace(/\.c$/, "");
}

/** One dispatch: one module reconstruction, accepted or restored. */
export async function runFold(
  deps: LoopDeps,
  state: LoopState,
  before: TuProgressReport,
  baseline: string,
  oracles: FoldOracles = DEFAULT_FOLD_ORACLES,
): Promise<{ state: LoopState; outcome: FunctionOutcome }> {
  const task = readFileSync(join(deps.projectRoot, FOLD_TASK), "utf8");
  const brief: FoldBrief = {
    task,
    baseline,
    folded: before.inventory.folded,
    eligible: before.inventory.eligible,
    remaining: before.inventory.remaining,
    sample: before.remaining,
  };
  const foldedAt = new Date().toISOString();
  let record: FoldRecord = {
    container: "", moduleName: "", sourcePath: "", members: [], status: "interrupted", foldedAt, tier: "",
    beforeFolded: before.inventory.folded, afterFolded: before.inventory.folded, head: await headCommit(deps.projectRoot),
  };
  /* Durable before the tree changes, so a dead session can be settled. */
  let current = recordFold(deps.config, state, record);

  const interrupted = async (): Promise<{ state: LoopState; outcome: FunctionOutcome }> => {
    const { changedFiles } = await loopChangedFiles(oracleContext(deps, current));
    const module = changedFiles.find((file) => MODULE_SOURCE.test(file));
    record = { ...record, status: "interrupted", changedFiles, preexisting: preexistingOf(deps, changedFiles), ...(module ? { sourcePath: module, moduleName: moduleNameOf(module) } : {}) };
    current = recordFold(deps.config, current, record);
    notify(deps, `Fold stopped; the tree is as the last return left it (${changedFiles.length} changed file(s)). The next fold run settles it.`, "warning");
    return { state: current, outcome: { kind: "aborted", functionName: record.moduleName || "fold" } };
  };

  let lastReport = "";
  let handoff: HandoffSummary | undefined;
  let tiersRan = 0;
  const matching = deps.config.ladder.filter((tier) => tier.role !== "prep");
  for (let tierIndex = 0; tierIndex < matching.length; tierIndex++) {
    const tier = matching[tierIndex]!;
    if (!(await applyTier(deps, tier))) continue;
    if (tiersRan > 0) await clearContext(deps);
    tiersRan += 1;
    record = { ...record, tier: tier.label };
    const tierStarted = Date.now();

    for (let attempt = 1; ; attempt++) {
      if (deps.flag.aborted) return interrupted();
      setStatus(deps, `↻ fold · ${tier.label} · return ${attempt}`);
      const message = attempt > 1
        ? foldNudgeMessage(lastReport)
        : tiersRan === 1 ? foldOpeningMessage(brief) : foldEscalationMessage(brief, tier.label, lastReport, handoff);
      if (!(await turn(deps, message))) return interrupted();

      setStatus(deps, `◎ fold · ${tier.label} · measuring`);
      const after = await oracles.progress(deps.projectRoot);
      if (after.report) {
        const { changedFiles } = await loopChangedFiles(oracleContext(deps, current));
        const judgement = judgeFold(before, after.report, changedFiles);
        if (judgement.ok && judgement.module) {
          setStatus(deps, `◎ fold · ${tier.label} · terminal gate`);
          const gate = await oracles.verify(deps.projectRoot, baseline);
          const passed = gate.report !== null && gate.code !== 2 && gate.report.gates.every((g) => g.state === "pass");
          if (passed) {
            const module = judgement.module;
            /* The gate keeps what its re-split regenerates (the headers a rename implies); the fold carries that too. */
            const { changedFiles: settled } = await loopChangedFiles(oracleContext(deps, current));
            record = {
              ...record, status: "accepted", container: module.container, sourcePath: module.source,
              moduleName: moduleNameOf(module.source), members: module.members.map((m) => m.name),
              afterFolded: after.report.inventory.folded, changedFiles: settled, preexisting: preexistingOf(deps, settled),
            };
            current = recordFold(deps.config, current, record);
            notify(deps, `Accepted ${module.source}: ${module.members.length} member(s); folded ${before.inventory.folded} -> ${after.report.inventory.folded}`, "info");
            return { state: current, outcome: { kind: "folded", functionName: record.moduleName, members: record.members,
              moduleName: record.moduleName, tier: tier.label, changedFiles: settled } };
          }
          lastReport = foldGateReport(gate);
        } else {
          lastReport = structureReport(after, judgement);
        }
      } else {
        lastReport = structureReport(after);
      }

      if (attempt >= deps.config.maxReturnsPerTier) break;
      if (deps.config.tierMinutes > 0 && Date.now() - tierStarted > deps.config.tierMinutes * 60_000) break;
    }

    if (tierIndex + 1 < matching.length) {
      handoff = (await captureHandoff(deps, "fold", tier.label)) ?? handoff;
      if (deps.flag.aborted) return interrupted();
    }
  }

  /* No tier produced an acceptable module: restore what the dispatch changed. */
  const finishedAt = new Date().toISOString();
  const { changedFiles } = await loopChangedFiles(oracleContext(deps, current));
  const preexisting = preexistingOf(deps, changedFiles);
  const restored = await restoreFiles(deps, changedFiles, finishedAt, preexisting);
  const detail = tiersRan === 0 ? "no escalation tier was reachable" : lastReport || "no acceptable module";
  record = { ...record, status: "failed", changedFiles, preexisting, detail };
  current = recordFold(deps.config, current, record);
  notify(deps, `Fold not accepted; restored ${changedFiles.length} file(s) from HEAD` + (restored.patch ? ` (change set: ${restored.patch})` : ""), "warning");
  const back = await oracles.progress(deps.projectRoot);
  if (!back.report || back.code === 2) {
    return { state: current, outcome: { kind: "environment-broken", functionName: "fold", detail: `the tree does not inventory after the restore:\n${structureReport(back)}` } };
  }
  return { state: current, outcome: { kind: "fold-failed", functionName: record.moduleName || "fold", members: record.members, record, detail } };
}

/**
 * Settle a dispatch the previous session left `interrupted`.
 *
 * If its module is reflected in the TU index and passes the terminal gate
 * against the commit it started from, it is accepted — the agent finished and
 * only the commit was missed. If the module is not reflected at all, the files
 * it changed are restored from HEAD. Anything else is restored and recorded as
 * failed. A tree whose HEAD has moved since is no longer the loop's to rewrite.
 */
export async function recoverInterruptedFolds(deps: LoopDeps, state: LoopState, oracles: FoldOracles = DEFAULT_FOLD_ORACLES): Promise<LoopState> {
  let current = state;
  for (const record of (state.folds ?? []).filter((fold) => fold.status === "interrupted")) {
    const head = await headCommit(deps.projectRoot);
    if (record.head && head !== record.head) {
      current = recordFold(deps.config, current, { ...record, status: "failed", detail: `HEAD moved from ${record.head.slice(0, 12)} since the interruption; the tree was left as it is` });
      notify(deps, `An interrupted fold started on ${record.head.slice(0, 12)}, but HEAD is now ${head.slice(0, 12)}; left the tree as it is.`, "warning");
      continue;
    }
    const label = record.sourcePath || "an unnamed module";
    setStatus(deps, `◎ fold · settling the interrupted fold of ${label} · measuring`);
    const reading = await oracles.progress(deps.projectRoot);
    const module = reading.report?.modules.find((m) => m.source === record.sourcePath);
    const reflected = !!(module && module.status === "valid" && reading.report!.inventory.folded > record.beforeFolded);
    if (reflected) {
      /* The receipt the dispatch was judged against, as runFold would have: a
         verdict the agent already reached on this tree is then reused. */
      const against = existsSync(join(deps.projectRoot, FOLD_RECEIPT)) ? FOLD_RECEIPT : record.head ?? "HEAD";
      notify(deps, `Settling the interrupted fold of ${label}: running the terminal gate against ${against}`, "info");
      setStatus(deps, `◎ fold · settling ${label} · terminal gate`);
      const dirt = await snapshotDirt(deps.projectRoot);
      const gate = await oracles.verify(deps.projectRoot, against);
      if (gate.report && gate.code !== 2 && gate.report.gates.every((g) => g.state === "pass")) {
        const accepted: FoldRecord = { ...record, status: "accepted", container: module!.container, members: module!.members.map((m) => m.name),
          afterFolded: reading.report!.inventory.folded, detail: "accepted on recovery: the module was complete when the loop stopped" };
        current = recordFold(deps.config, current, accepted);
        if (deps.config.commitOnMatch) {
          /* The gate keeps what its re-split regenerates; the fold carries that too. */
          const regenerated = (await newChangesSince(deps.projectRoot, [...(await snapshotDirt(deps.projectRoot)).keys()], dirt)).newFiles;
          const files = [...new Set([...(record.changedFiles ?? []), ...regenerated])].sort();
          const commit = await commitFold(deps.projectRoot, accepted.moduleName, accepted.members, accepted.tier || "recovery", accepted.beforeFolded, accepted.afterFolded, files,
            files.filter((file) => !withinAllowedRoots(loadConfig(deps.projectRoot), file)), Object.keys(record.preexisting ?? {}));
          if (commit.committed) current = recordFold(deps.config, current, { ...accepted, commit: commit.detail });
        }
        notify(deps, `Recovered the interrupted fold of ${record.sourcePath}: accepted.`, "info");
        continue;
      }
    }
    const restored = await restoreFiles(deps, record.changedFiles ?? [], new Date().toISOString(), record.preexisting ?? {});
    current = recordFold(deps.config, current, { ...record, status: "failed",
      detail: reflected ? "the recovered module did not pass the terminal gate; restored from HEAD" : "the module was not reflected in the TU index; restored from HEAD" });
    notify(deps, `Restored ${record.changedFiles?.length ?? 0} file(s) from an interrupted fold` + (restored.patch ? `; change set at ${restored.patch}` : ""), "warning");
  }
  return current;
}

/** Work the campaign: one module per clean context, until the terminal gate passes. */
export async function runFoldLoop(input: LoopDeps, oracles: FoldOracles = DEFAULT_FOLD_ORACLES): Promise<FunctionOutcome[]> {
  const deps: LoopDeps = { ...input, baseline: new Map(input.baseline) };
  mkdirSync(deps.config.runtimeDir, { recursive: true });
  const savedModel = deps.ctx.model;
  const savedThinking = deps.pi.getThinkingLevel();
  const outcomes: FunctionOutcome[] = [];
  let state = readState(deps.config);

  try {
    state = await recoverInterruptedFolds(deps, state, oracles);
    /* Whatever is dirty now pre-dates the first dispatch, by content: a file a
       dispatch edits on top of it becomes that dispatch's change and goes
       into its commit. The session's activation snapshot is older than any
       recovery commit, so it cannot stand in for this one. */
    deps.baseline = await snapshotDirt(deps.projectRoot);
    setStatus(deps, "◎ fold · capturing the campaign baseline");
    const receipt = await oracles.captureBaseline(deps.projectRoot, FOLD_RECEIPT);
    if (!receipt.ok) {
      notify(deps, `Fold loop could not capture its baseline receipt: ${receipt.detail}`, "error");
      outcomes.push({ kind: "environment-broken", functionName: "fold", detail: receipt.detail });
      return outcomes;
    }
    notify(deps, receipt.detail, "info");

    while (!deps.flag.aborted) {
      setStatus(deps, "◎ fold · measuring");
      const before = await oracles.progress(deps.projectRoot);
      if (!before.report || before.code === 2) {
        const detail = structureReport(before);
        notify(deps, `Fold loop stopped: the inventory is not valid.\n${detail}`, "error");
        outcomes.push({ kind: "environment-broken", functionName: "fold", detail });
        break;
      }
      if (before.report.inventory.remaining === 0) {
        setStatus(deps, "◎ fold · terminal verification");
        const final = await oracles.verify(deps.projectRoot, FOLD_RECEIPT, undefined, true);
        if (final.code === 0) {
          notify(deps, `Every eligible function is folded (${before.report.inventory.folded}) and the terminal gate passed.`, "info");
        } else {
          const detail = foldGateReport(final);
          notify(deps, `Nothing remains to fold, but the terminal gate did not pass:\n${detail}`, "error");
          outcomes.push({ kind: "environment-broken", functionName: "fold", detail });
        }
        break;
      }
      if (outcomes.length > 0) await clearContext(deps);

      const run = await runFold(deps, state, before.report, FOLD_RECEIPT, oracles);
      state = run.state;
      outcomes.push(run.outcome);
      const outcome = run.outcome;
      if (outcome.kind === "aborted") break;
      if (outcome.kind === "environment-broken") {
        notify(deps, `Fold loop stopped: ${outcome.detail}`, "error");
        break;
      }
      if (outcome.kind === "fold-failed") {
        notify(deps, `Fold loop stopped: the dispatch did not produce an acceptable module.\n${outcome.detail}`, "error");
        break;
      }
      if (outcome.kind !== "folded") continue;

      /* The terminal gate judged the whole tree, so the whole change set is the
         result: a module often needs the tooling fix it exposed (a generator
         that must preserve a new name). Files outside the integration roots
         are flagged, never split off — committing the module without them
         would publish a tree the gate never saw. */
      const ctx = oracleContext(deps, state);
      const { committable, outOfScope } = await scopedLoopChanges(ctx);
      if (outOfScope.length) notify(deps, `Committed with the module, outside the integration roots: ${outOfScope.join(", ")}`, "warning");
      if (deps.config.commitOnMatch) {
        const record = state.folds?.at(-1);
        setStatus(deps, `◎ fold · commit ${outcome.moduleName}`);
        const commit = await commitFold(deps.projectRoot, outcome.moduleName, outcome.members, outcome.tier,
          record?.beforeFolded ?? 0, record?.afterFolded ?? 0, [...committable, ...outOfScope], outOfScope, Object.keys(record?.preexisting ?? {}));
        if (commit.committed) {
          outcome.commit = commit.detail;
          if (record) state = recordFold(deps.config, state, { ...record, commit: commit.detail });
          notify(deps, `Committed ${outcome.moduleName} as ${commit.detail}`, "info");
        } else if (commit.failed) {
          notify(deps, `Fold loop stopped: ${outcome.moduleName} was accepted but not committed — ${commit.detail}`, "error");
          break;
        }
      }
      /* Whatever is still dirty pre-dates the next module, as it stands now. */
      deps.baseline = await snapshotDirt(deps.projectRoot);
    }
  } finally {
    writeState(deps.config, state);
    if (savedModel && deps.ctx.isIdle()) {
      await deps.pi.setModel(savedModel);
      deps.pi.setThinkingLevel(savedThinking);
    }
    delete deps.sink.tier;
    setStatus(deps, undefined);
  }
  return outcomes;
}
