import { existsSync, mkdirSync, readFileSync, renameSync, writeFileSync } from "node:fs";
import { dirname, join } from "node:path";
import { isDeepStrictEqual } from "node:util";
import { loadCallGraph, rebuildCallGraph } from "../../shared/call-graph.ts";
import { callGraphIsFresh } from "../../../../tools/lib/callGraphCache.ts";
import { loadConfig } from "../../shared/config.ts";
import { runResidualObjective, type ResidualReading } from "../../shared/gates.ts";
import { runCommand } from "../../shared/process.ts";
import { allowlistKeys, sourceConstructFindings } from "../../shared/source-policy.ts";
import type { CallGraphEntry, PolicyFinding } from "../../shared/types.ts";
import { best, readLedger, variantText, type LedgerEntry } from "../../../../tools/agent/experimentLedger.ts";
import { describe as describeEntry } from "./best-attempt.ts";
import { commitRepairAttempt, commitRepairedFunction } from "./commit.ts";
import { applyTier, captureHandoff, clearContext, notify, setStatus, turn, type LoopDeps } from "./loop.ts";
import {
  environmentIsIntact,
  finalize,
  introducedForbiddenConstructs,
  isMatched,
  loopChangedFiles,
  scopedLoopChanges,
  type FinalizeVerdict,
  type MatchVerdict,
  type OracleContext,
} from "./oracles.ts";
import { archiveSource, noteRelativePath, readSource, sourceRelativePath, writeNote } from "./park.ts";
import { findingsReport, gateReport, matchReport, nudgeMessage, repairEscalationMessage, repairOpeningMessage, repairReport } from "./prompts.ts";
import { isNotesPath } from "./scope-guard.ts";
import { readState, recordRepair, writeState } from "./state.ts";
import { shouldStop } from "./stop-rule.ts";
import type { FunctionOutcome, HandoffSummary, LoopState, RepairRecord } from "./types.ts";

/**
 * `/auto_decompilation_loop repair` — take the exceptions back from functions
 * that already match.
 *
 * A function that reached its match through a register pin or embedded asm is
 * still on the allowlist, and nothing ever comes back to it: the ordinary loop
 * only picks functions that do not match yet. The repair loop is that return
 * visit. For each candidate it revokes the function's asm kinds in
 * `.pi/autoloop.json` — on disk, so every gate the worker can call judges the
 * source as plain C — and runs the ladder until the function is byte-exact with
 * no forbidden construct left. That is committed as `clean <function>`, with the
 * entry gone.
 *
 * A repair never costs the match. If it does not finish, the matching source
 * and its allowlist entry are put back as they were, the best clean program the
 * ledger measured is written into a note for the next attempt, and the tree is
 * proved to build before the loop moves on. That is the opposite of a park,
 * which hands an unmatched function back to INCLUDE_ASM; here the function was
 * already done, and only the cleanup failed.
 */

/** The allowlist kinds a repair takes away. A flag override is a different review. */
export const REPAIR_KINDS = ["register-asm", "embedded-asm"];

/** The exceptions recorded here are justified; anything else allowlisted is a repair candidate. */
export const EVIDENCE_DOC = "notes/asm-exception-evidence.md";

export const ALLOWLIST_FILE = ".pi/autoloop.json";

export interface RepairCandidate {
  name: string;
  /** The repair kinds the allowlist grants this function. */
  kinds: string[];
  size: number;
}

/** Functions whose exceptions `notes/asm-exception-evidence.md` justifies, by `## <name> — ...` heading. */
export function documentedExceptions(projectRoot: string): Set<string> {
  const path = join(projectRoot, EVIDENCE_DOC);
  const names = new Set<string>();
  if (!existsSync(path)) return names;
  for (const match of readFileSync(path, "utf8").matchAll(/^## +([A-Za-z_][A-Za-z0-9_]*) +—/gm)) {
    names.add(match[1]!.toLowerCase());
  }
  return names;
}

/**
 * Every live, decompiled function the allowlist grants a pin or embedded asm,
 * minus the documented exceptions, smallest first.
 *
 * Derived rather than listed: an exception nobody wrote up is exactly the
 * workaround this loop exists to remove, so a new one joins the queue without
 * anyone having to remember to add it.
 */
export function repairCandidates(
  functions: CallGraphEntry[],
  allowlist: Record<string, string[]>,
  documented: ReadonlySet<string>,
): RepairCandidate[] {
  const candidates: RepairCandidate[] = [];
  for (const entry of functions) {
    if (entry.dead || entry.handwritten !== false || !entry.decompiled) continue;
    if (documented.has(entry.name.toLowerCase())) continue;
    const granted = new Set<string>();
    for (const key of allowlistKeys(entry.name, entry.vram, entry.container)) {
      for (const kind of allowlist[key] ?? []) if (REPAIR_KINDS.includes(kind)) granted.add(kind);
    }
    if (granted.size === 0) continue;
    candidates.push({ name: entry.name, kinds: REPAIR_KINDS.filter((kind) => granted.has(kind)), size: entry.size });
  }
  return candidates.sort((left, right) => left.size - right.size || left.name.localeCompare(right.name));
}

async function currentGraph(projectRoot: string): Promise<CallGraphEntry[]> {
  const graph = callGraphIsFresh(projectRoot) ? loadCallGraph(projectRoot)
    : await rebuildCallGraph(projectRoot).catch(() => loadCallGraph(projectRoot));
  return graph.functions;
}

export async function repairQueue(projectRoot: string): Promise<RepairCandidate[]> {
  return repairCandidates(await currentGraph(projectRoot), loadConfig(projectRoot).sourcePolicy.allowlist, documentedExceptions(projectRoot));
}

/**
 * `.pi/autoloop.json` with `kinds` taken off the entries `keys` name.
 *
 * Edited as text, so the rest of the file keeps the human's layout and the
 * repair's commit shows one entry changing; an entry left with no kinds goes.
 * The result is checked against the parsed edit and refused if they disagree.
 */
export function revokeAllowlist(text: string, keys: readonly string[], kinds: readonly string[]): { text: string; revoked: string[] } {
  const parsed = JSON.parse(text) as { sourcePolicy?: { allowlist?: Record<string, string[]> } };
  const allowlist = parsed.sourcePolicy?.allowlist;
  if (!allowlist) return { text, revoked: [] };
  const wanted = new Set(keys.map((key) => key.toLowerCase()));
  const revoked = new Set<string>();
  let next = text;
  for (const key of Object.keys(allowlist)) {
    if (!wanted.has(key.toLowerCase())) continue;
    const current = allowlist[key]!;
    const remaining = current.filter((kind) => !kinds.includes(kind));
    if (remaining.length === current.length) continue;
    for (const kind of current) if (kinds.includes(kind)) revoked.add(kind);
    next = rewriteEntry(next, key, remaining);
    if (remaining.length) allowlist[key] = remaining;
    else delete allowlist[key];
  }
  if (revoked.size === 0) return { text, revoked: [] };
  if (!isDeepStrictEqual(JSON.parse(next), parsed)) throw new Error(`Could not revoke ${[...revoked].join(", ")} in ${ALLOWLIST_FILE} without disturbing other entries`);
  return { text: next, revoked: kinds.filter((kind) => revoked.has(kind)) };
}

function rewriteEntry(text: string, key: string, remaining: string[]): string {
  const escaped = key.replace(/[.*+?^${}()|[\]\\]/g, "\\$&");
  const matches = [...text.matchAll(new RegExp(`"${escaped}"\\s*:\\s*\\[[^\\]]*\\]`, "g"))];
  if (matches.length !== 1) throw new Error(`Allowlist key "${key}" appears ${matches.length} times in ${ALLOWLIST_FILE}`);
  const entry = matches[0]![0];
  const start = matches[0]!.index!;
  const end = start + entry.length;
  if (remaining.length) {
    const open = entry.indexOf("[");
    const array = entry.slice(open);
    const element = array.match(/\n([ \t]*)"/)?.[1];
    const closing = array.match(/\n([ \t]*)\]$/)?.[1];
    const body = element !== undefined && closing !== undefined
      ? `[\n${remaining.map((kind) => `${element}${JSON.stringify(kind)}`).join(",\n")}\n${closing}]`
      : `[${remaining.map((kind) => JSON.stringify(kind)).join(", ")}]`;
    return text.slice(0, start) + entry.slice(0, open) + body + text.slice(end);
  }
  /* Drop the entry's line, and exactly one separating comma: its own when it
     has one, else the one ending the entry before it. */
  const lineStart = text.lastIndexOf("\n", start - 1);
  const before = text.slice(0, lineStart);
  const after = text.slice(end);
  const trailing = after.match(/^\s*,/);
  if (trailing) return before + after.slice(trailing[0].length);
  const comma = before.search(/,\s*$/);
  return comma < 0 ? before + after : before.slice(0, comma) + before.slice(comma + 1) + after;
}

/**
 * The best measured program that carries no forbidden construct.
 *
 * The ledger keeps every program the worker measured, the matching hybrid
 * included, so "best" alone would hand the hybrid back. Only rows whose text
 * passes the revoked policy compete.
 */
export function chooseRepairAttempt(
  entries: LedgerEntry[],
  textOf: (entry: LedgerEntry) => string | undefined,
  isClean: (text: string) => boolean,
): { text: string; entry: LedgerEntry } | undefined {
  const verdicts = new Map<string, boolean>();
  const clean: Array<{ text: string; entry: LedgerEntry }> = [];
  for (const entry of entries) {
    const text = textOf(entry);
    if (text === undefined) continue;
    let verdict = verdicts.get(entry.sourceHash);
    if (verdict === undefined) verdicts.set(entry.sourceHash, verdict = isClean(text));
    if (verdict) clean.push({ text, entry });
  }
  const winner = best(clean.map((candidate) => candidate.entry));
  return winner ? clean.find((candidate) => candidate.entry === winner) : undefined;
}

export function buildRepairNote(
  record: RepairRecord,
  startFindings: PolicyFinding[],
  attempt: { text: string; description: string } | undefined,
  archived: { finalSource?: string; patch?: string; moved: string[] },
): string {
  return [
    `# ${record.functionName} — repair attempt`,
    "",
    `\`/auto_decompilation_loop repair\` did not reach a clean match. \`${record.sourcePath}\` and its allowlist`,
    `entry are back as they were at \`${record.head.slice(0, 12)}\`, so the function still matches.`,
    "",
    `- Revoked for the attempt: ${record.revokedKinds.join(", ")}`,
    `- Reached tier: ${record.reachedTier ?? "none"}`,
    `- Started ${record.startedAt}, finished ${record.finishedAt ?? "?"}`,
    ...(archived.finalSource ? [`- Last program on disk (local runtime archive): \`${archived.finalSource}\``] : []),
    ...(archived.patch ? [`- Its full change set (local runtime archive): \`${archived.patch}\``] : []),
    ...(archived.moved.length ? [`- New files moved out of the tree: ${archived.moved.map((file) => `\`${file}\``).join(", ")}`] : []),
    "",
    "## Constructs the repair had to remove",
    "",
    findingsReport(startFindings),
    "",
    "## Best clean program",
    "",
    ...(attempt
      ? [`${attempt.description} (the experiment ledger's measurement of this exact text).`, "", "```c", attempt.text.trimEnd(), "```"]
      : ["No measured program was free of the forbidden constructs."]),
    "",
    "## Last report",
    "",
    "```text",
    (record.lastReport ?? "(none)").trimEnd(),
    "```",
    "",
    `The next \`/auto_decompilation_loop repair ${record.functionName}\` is pointed at this note.`,
    "",
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

async function isDirty(projectRoot: string, file: string): Promise<boolean> {
  const result = await git(projectRoot, ["status", "--porcelain", "--", file]);
  return result.code !== 0 || result.stdout.trim().length > 0;
}

/**
 * Put every file this repair changed back to HEAD, except notes.
 *
 * The repair started from a committed, matching tree, so HEAD is exactly what
 * it found. Its change set is archived as a patch first; a file the worker
 * created is moved into the archive rather than deleted.
 */
async function restoreTree(
  deps: LoopDeps,
  ctx: OracleContext,
  functionName: string,
  stamp: string,
): Promise<{ patch?: string; moved: string[] }> {
  const { changedFiles } = await loopChangedFiles({ ...ctx, signal: undefined });
  const restore = changedFiles.filter((file) => !isNotesPath(file));
  if (restore.length === 0) return { moved: [] };
  const archiveDir = join(deps.config.runtimeDir, "repairs", `${functionName}.${stamp.replace(/[:.]/g, "-")}`);
  mkdirSync(archiveDir, { recursive: true });

  const tracked: string[] = [];
  const created: string[] = [];
  for (const file of restore) {
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
  for (const file of created) {
    const target = join(archiveDir, "created", file);
    mkdirSync(dirname(target), { recursive: true });
    renameSync(join(deps.projectRoot, file), target);
  }
  return { ...(patch ? { patch } : {}), moved: created };
}

/** The checks a repair runs. Injectable so the controller can be tested without a toolchain. */
export interface RepairOracles {
  isMatched(ctx: OracleContext, functionName: string): Promise<MatchVerdict>;
  findings(ctx: OracleContext, functionName: string): Promise<PolicyFinding[]>;
  finalize(ctx: OracleContext, functionName: string): Promise<FinalizeVerdict>;
  residual(projectRoot: string, functionName: string): Promise<ResidualReading | null>;
  environment(ctx: OracleContext): Promise<{ ok: boolean; detail: string }>;
}

export const DEFAULT_REPAIR_ORACLES: RepairOracles = {
  isMatched,
  findings: introducedForbiddenConstructs,
  finalize,
  residual: (projectRoot, functionName) => runResidualObjective(projectRoot, functionName),
  environment: environmentIsIntact,
};

/**
 * Repair one function: revoke, run the ladder, and either keep a clean match or
 * put the matching source back.
 */
export async function runRepair(
  deps: LoopDeps,
  state: LoopState,
  candidate: RepairCandidate,
  oracles: RepairOracles = DEFAULT_REPAIR_ORACLES,
): Promise<{ state: LoopState; outcome: FunctionOutcome }> {
  const abort = new AbortController();
  const poll = setInterval(() => { if (deps.flag.aborted) abort.abort(); }, 100);
  if (deps.flag.aborted) abort.abort();
  try { return await repairWithSignal(deps, state, candidate, oracles, abort.signal); }
  finally { clearInterval(poll); }
}

async function repairWithSignal(
  deps: LoopDeps,
  state: LoopState,
  candidate: RepairCandidate,
  oracles: RepairOracles,
  signal: AbortSignal,
): Promise<{ state: LoopState; outcome: FunctionOutcome }> {
  const functionName = candidate.name;
  const sourcePath = sourceRelativePath(deps.projectRoot, functionName);
  const allowlistPath = join(deps.projectRoot, ALLOWLIST_FILE);
  /* An agent-approved exemption recorded in loop state would hand the revoked
     kinds straight back to the gates, so the target's own is set aside. */
  const judged = (current: LoopState): LoopState => {
    const approvals = { ...current.approvals };
    delete approvals[functionName];
    return { ...current, approvals };
  };
  const oracle = (current: LoopState): OracleContext => ({ projectRoot: deps.projectRoot, baseline: deps.baseline, state: judged(current), signal });

  /* A repair is only undoable if it starts from what HEAD holds. */
  for (const file of [ALLOWLIST_FILE, sourcePath]) {
    if (deps.baseline.has(file) || await isDirty(deps.projectRoot, file)) {
      return { state, outcome: { kind: "environment-broken", functionName,
        detail: `${file} has uncommitted changes. A repair edits it and must be able to restore it from HEAD; commit or stash it first.` } };
    }
  }

  setStatus(deps, `◎ ${functionName} · repair · checking the starting match`);
  const start = await oracles.isMatched(oracle(state), functionName);
  if (!start.matched) {
    return { state, outcome: { kind: "skipped", functionName, detail: `${functionName} is not byte-exact now; a repair starts from a match.` } };
  }

  const graphEntry = (await currentGraph(deps.projectRoot)).find((entry) => entry.name === functionName);
  const keys = allowlistKeys(functionName, graphEntry?.vram, graphEntry?.container);
  const originalAllowlist = readFileSync(allowlistPath, "utf8");
  const { text: revokedAllowlist, revoked } = revokeAllowlist(originalAllowlist, keys, candidate.kinds);
  if (revoked.length === 0) {
    return { state, outcome: { kind: "skipped", functionName, detail: `${functionName} has no ${REPAIR_KINDS.join("/")} allowlist entry to revoke.` } };
  }

  const startedAt = new Date().toISOString();
  let record: RepairRecord = { functionName, sourcePath, status: "in-progress", startedAt,
    head: await headCommit(deps.projectRoot), keys, revokedKinds: revoked };
  /* Durable before the tree changes, so a dead session can be restored. */
  let current = recordRepair(deps.config, state, record);
  writeFileSync(allowlistPath, revokedAllowlist);
  notify(deps, `Repairing ${functionName}: revoked ${revoked.join(", ")} in ${ALLOWLIST_FILE}`, "info");

  const startFindings = await oracles.findings(oracle(current), functionName);
  const notePath = noteRelativePath(deps.config, `${functionName}.repair.md`);
  const previous = existsSync(join(deps.projectRoot, notePath))
    ? `A previous repair attempt is recorded in \`${notePath}\`. Read it first: its best clean program is a measured starting point, and its report says where that attempt stopped.`
    : undefined;

  let lastReport = repairReport(startFindings, matchReport(start.diff));
  let handoff: HandoffSummary | undefined;
  let tiersRan = 0;
  let blocked = false;
  const matching = deps.config.ladder.filter((tier) => tier.role !== "prep");

  for (let tierIndex = 0; tierIndex < matching.length && !blocked; tierIndex++) {
    const tier = matching[tierIndex]!;
    if (!(await applyTier(deps, tier))) continue;
    if (tiersRan > 0) await clearContext(deps);
    tiersRan += 1;
    record = { ...record, reachedTier: tier.label };
    const tierStarted = Date.now();

    for (let attempt = 1; ; attempt++) {
      if (deps.flag.aborted) return interrupted(deps, current, record, oracle(current));
      setStatus(deps, `↻ ${functionName} · repair · ${tier.label} · return ${attempt} (min ${deps.config.returnsPerTier})`);
      const message = attempt > 1
        ? nudgeMessage(lastReport)
        : tiersRan === 1
          ? repairOpeningMessage(functionName, sourcePath, revoked, startFindings, previous)
          : repairEscalationMessage(functionName, tier.label, sourcePath, revoked, lastReport, handoff);
      if (!(await turn(deps, message))) return interrupted(deps, current, record, oracle(current));

      /* The allowlist is the human's; a repair only ever narrows it, and only by the loop's hand. */
      let allowlistNotice = "";
      if (readFileSync(allowlistPath, "utf8") !== revokedAllowlist) {
        writeFileSync(allowlistPath, revokedAllowlist);
        allowlistNotice = `\`${ALLOWLIST_FILE}\` changed during the turn and has been put back. The allowlist is not part of a repair.`;
        notify(deps, `${functionName}: reverted an edit to ${ALLOWLIST_FILE}`, "warning");
      }

      const findings = await oracles.findings(oracle(current), functionName);
      const match = await oracles.isMatched(oracle(current), functionName);
      if (findings.length === 0 && match.matched) {
        setStatus(deps, `◎ ${functionName} · repair · finalize`);
        const gate = await oracles.finalize(oracle(current), functionName);
        if (gate.passed) {
          notify(deps, `${functionName} repaired on ${tier.label}: byte-exact with no ${revoked.join("/")} exception`, "info");
          record = { ...record, status: "repaired", finishedAt: new Date().toISOString() };
          current = recordRepair(deps.config, current, record);
          return { state: current, outcome: { kind: "repaired", functionName, tier: tier.label,
            changedFiles: (await loopChangedFiles(oracle(current))).changedFiles } };
        }
        lastReport = [allowlistNotice, gateReport(gate.gate)].filter(Boolean).join("\n\n");
        if (attempt >= deps.config.maxReturnsPerTier) break;
        continue;
      }

      const residual = match.matched ? null : await oracles.residual(deps.projectRoot, functionName);
      lastReport = [allowlistNotice, repairReport(findings, matchReport(match.diff, residual))].filter(Boolean).join("\n\n");
      if (match.matched) {
        if (attempt >= deps.config.maxReturnsPerTier) break;
        continue;
      }
      const verdict = shouldStop({ config: deps.config, functionName, returns: attempt, elapsedMs: Date.now() - tierStarted, residual });
      if (verdict.parkNow === "blocked") {
        notify(deps, `${functionName} is blocked, not hard: ${verdict.detail}`, "warning");
        blocked = true;
        break;
      }
      if (verdict.stop) {
        notify(deps, `${functionName}: ${tier.label} is done — ${verdict.detail}`, "info");
        break;
      }
    }

    if (!blocked && tierIndex + 1 < matching.length) {
      handoff = (await captureHandoff(deps, functionName, tier.label)) ?? handoff;
      if (deps.flag.aborted) return interrupted(deps, current, record, oracle(current));
    }
  }

  if (tiersRan === 0) {
    notify(deps, `No escalation tier was reachable for ${functionName}; restoring its matching source.`, "error");
    return interrupted(deps, current, record, oracle(current));
  }
  return restoreAfterFailure(deps, current, { ...record, lastReport }, startFindings, oracles, oracle(current));
}

/** The operator stopped the loop, or no tier ran: restore, keep the attempt, and leave the function queued. */
async function interrupted(
  deps: LoopDeps,
  state: LoopState,
  record: RepairRecord,
  ctx: OracleContext,
): Promise<{ state: LoopState; outcome: FunctionOutcome }> {
  const finishedAt = new Date().toISOString();
  const archived = archiveSource(deps.config.runtimeDir, record.functionName, readSource(deps.projectRoot, record.functionName), "repair-interrupted", finishedAt);
  const restored = await restoreTree(deps, ctx, record.functionName, finishedAt);
  const current = recordRepair(deps.config, state, { ...record, status: "interrupted", finishedAt });
  notify(deps, `Repair of ${record.functionName} stopped; its matching source and allowlist entry are restored.` +
    (archived ? ` The attempt is archived at ${archived}.` : "") + (restored.patch ? ` Change set: ${restored.patch}.` : ""), "warning");
  return { state: current, outcome: { kind: "aborted", functionName: record.functionName } };
}

async function restoreAfterFailure(
  deps: LoopDeps,
  state: LoopState,
  record: RepairRecord,
  startFindings: PolicyFinding[],
  oracles: RepairOracles,
  ctx: OracleContext,
): Promise<{ state: LoopState; outcome: FunctionOutcome }> {
  const functionName = record.functionName;
  const finishedAt = new Date().toISOString();
  setStatus(deps, `◎ ${functionName} · repair · restoring`);

  /* Judged while the revocation is still on disk, so "clean" means plain C. */
  const policy = loadConfig(deps.projectRoot);
  const graphEntry = (await currentGraph(deps.projectRoot)).find((entry) => entry.name === functionName);
  const scope = { name: functionName, vram: graphEntry?.vram, container: graphEntry?.container };
  const attempt = chooseRepairAttempt(readLedger(functionName), variantText,
    (text) => sourceConstructFindings(text, policy, scope).length === 0);
  const finalSource = archiveSource(deps.config.runtimeDir, functionName, readSource(deps.projectRoot, functionName), "repair-final", finishedAt);
  if (attempt) archiveSource(deps.config.runtimeDir, functionName, attempt.text, "repair-best", finishedAt);

  const restored = await restoreTree(deps, ctx, functionName, finishedAt);
  const attemptNote = attempt
    ? `best clean program: ${describeEntry(attempt.entry)}`
    : "no measured program was free of the forbidden constructs";
  const finished: RepairRecord = { ...record, status: "failed", finishedAt, attemptNote,
    notePath: noteRelativePath(deps.config, `${functionName}.repair.md`) };
  writeNote(deps.projectRoot, deps.config, `${functionName}.repair.md`, buildRepairNote(finished, startFindings,
    attempt ? { text: attempt.text, description: describeEntry(attempt.entry) } : undefined,
    { ...(finalSource ? { finalSource } : {}), ...restored }));
  const current = recordRepair(deps.config, state, finished);
  notify(deps, `${functionName} was not repaired; matching source restored (${attemptNote}). Wrote ${finished.notePath}.`, "warning");

  /* The tree is what HEAD held, so this proves the restore, not the repair. */
  setStatus(deps, `◎ ${functionName} · repair · environment check`);
  const back = await oracles.isMatched({ ...ctx, signal: undefined }, functionName);
  const environment = back.matched ? await oracles.environment({ ...ctx, signal: undefined }) : { ok: false, detail: `${functionName} no longer matches after the restore` };
  if (!environment.ok) {
    return { state: current, outcome: { kind: "environment-broken", functionName, detail: environment.detail } };
  }
  return { state: current, outcome: { kind: "repair-failed", functionName, record: finished } };
}

/**
 * A session that died mid-repair left the revocation on disk. Put it back —
 * but only while the allowlist still reads exactly as the loop left it, and
 * only onto the commit the repair started from. Anything else means someone
 * has handled it since, and their tree is not the loop's to rewrite.
 */
export async function recoverInterruptedRepairs(deps: LoopDeps, state: LoopState): Promise<LoopState> {
  let current = state;
  for (const record of Object.values(state.repairs ?? {})) {
    if (record.status !== "in-progress") continue;
    const head = await headCommit(deps.projectRoot);
    const committed = await git(deps.projectRoot, ["show", `HEAD:${ALLOWLIST_FILE}`]);
    const onDisk = readFileSync(join(deps.projectRoot, ALLOWLIST_FILE), "utf8");
    const leftByLoop = head === record.head && committed.code === 0 &&
      revokeAllowlist(committed.stdout, record.keys, record.revokedKinds).text === onDisk;
    if (leftByLoop) {
      const stamp = new Date().toISOString();
      const archived = archiveSource(deps.config.runtimeDir, record.functionName, readSource(deps.projectRoot, record.functionName), "repair-recovered", stamp);
      const checkout = await git(deps.projectRoot, ["checkout", "HEAD", "--", ALLOWLIST_FILE, record.sourcePath]);
      if (checkout.code !== 0) throw new Error(`Could not restore the interrupted repair of ${record.functionName}: ${checkout.stderr}`);
      notify(deps, `Restored ${record.functionName} and ${ALLOWLIST_FILE} from an interrupted repair` + (archived ? `; attempt archived at ${archived}` : ""), "warning");
    } else {
      notify(deps, `An earlier repair of ${record.functionName} did not finish, and the tree has changed since; left it as it is. Check ${record.sourcePath} and ${ALLOWLIST_FILE}.`, "warning");
    }
    current = recordRepair(deps.config, current, { ...record, status: "interrupted", finishedAt: new Date().toISOString() });
  }
  return current;
}

export interface RepairLoopOptions {
  /** Repair only this function; it may be one an earlier run failed. */
  target?: string;
  maxFunctions?: number;
}

/** Work the repair queue: one function per clean context, committing each result. */
export async function runRepairLoop(input: LoopDeps, options: RepairLoopOptions = {}, oracles: RepairOracles = DEFAULT_REPAIR_ORACLES): Promise<FunctionOutcome[]> {
  const deps: LoopDeps = { ...input, baseline: new Set(input.baseline) };
  mkdirSync(deps.config.runtimeDir, { recursive: true });
  const savedModel = deps.ctx.model;
  const savedThinking = deps.pi.getThinkingLevel();
  const outcomes: FunctionOutcome[] = [];
  let state = readState(deps.config);

  try {
    state = await recoverInterruptedRepairs(deps, state);
    setStatus(deps, "↻ repair · selecting targets");
    const queue = await repairQueue(deps.projectRoot);
    let targets: RepairCandidate[];
    if (options.target) {
      const named = queue.find((candidate) => candidate.name.toLowerCase() === options.target!.toLowerCase());
      if (!named) {
        notify(deps, `${options.target} is not a repair candidate: it has no ${REPAIR_KINDS.join("/")} allowlist entry, or ${EVIDENCE_DOC} documents it.`, "error");
        return outcomes;
      }
      targets = [named];
    } else {
      const settled = new Set(["failed", "repaired"]);
      targets = queue.filter((candidate) => !settled.has(state.repairs?.[candidate.name]?.status ?? ""));
      notify(deps, `Repair queue: ${targets.length} function(s)` +
        (queue.length > targets.length ? `; ${queue.length - targets.length} skipped after an earlier failed repair (name one to retry it)` : ""), "info");
    }

    const limit = options.maxFunctions ?? deps.config.maxFunctions;
    for (let index = 0; index < Math.min(limit, targets.length); index++) {
      if (deps.flag.aborted) break;
      if (index > 0) await clearContext(deps);
      const target = targets[index]!;
      const run = await runRepair(deps, state, target, oracles);
      state = run.state;
      outcomes.push(run.outcome);
      const outcome = run.outcome;
      if (outcome.kind === "aborted") break;
      if (outcome.kind === "environment-broken") {
        notify(deps, `Repair loop stopped: ${target.name} — ${outcome.detail}`, "error");
        break;
      }
      if (outcome.kind === "skipped") {
        notify(deps, outcome.detail, "warning");
        continue;
      }

      const ctx: OracleContext = { projectRoot: deps.projectRoot, baseline: deps.baseline, state };
      const { committable, outOfScope } = await scopedLoopChanges(ctx);
      if (outOfScope.length) notify(deps, `Left uncommitted (outside the integration roots): ${outOfScope.join(", ")}`, "warning");
      if (outcome.kind === "repaired" && deps.config.commitOnMatch) {
        setStatus(deps, `◎ ${target.name} · commit`);
        const commit = await commitRepairedFunction(deps.projectRoot, target.name, outcome.tier, state.repairs?.[target.name]?.revokedKinds ?? target.kinds, committable);
        if (commit.committed) {
          outcome.commit = commit.detail;
          notify(deps, `Committed ${target.name} as ${commit.detail}`, "info");
        } else if (commit.failed) {
          notify(deps, `Repair loop stopped: ${target.name} was repaired but not committed — ${commit.detail}`, "error");
          break;
        }
      }
      if (outcome.kind === "repair-failed" && deps.config.commitOnPark) {
        const commit = await commitRepairAttempt(deps.projectRoot, target.name, outcome.record.reachedTier ?? "none", outcome.record.notePath ?? "", committable);
        if (commit.committed) outcome.commit = commit.detail;
        else if (commit.failed) notify(deps, `Repair note for ${target.name} not committed — ${commit.detail}`, "warning");
      }
      /* Whatever could not be committed pre-dates the next function. */
      for (const file of (await loopChangedFiles(ctx)).changedFiles) deps.baseline.add(file);
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
