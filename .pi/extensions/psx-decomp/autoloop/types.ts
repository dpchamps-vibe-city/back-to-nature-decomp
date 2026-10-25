import type { Completion } from "../tools/prepared-attempt.ts";
import type { PolicyFinding } from "../../shared/types.ts";

export type ThinkingLevel = "off" | "minimal" | "low" | "medium" | "high" | "xhigh";

/** One rung of the escalation ladder: a concrete provider/model plus its thinking level. */
export interface LoopTier {
  provider: string;
  model: string;
  thinking: ThinkingLevel;
  /** Human-facing label used in status lines and notes. */
  label: string;
  /** Omitted means the existing matching role; prep only prepares compiling C. */
  role?: "prep";
  /** Steer at this context size and force a checkpoint at 110%; defaults to 350k, 0 disables. */
  checkpointAtTokens?: number;
}

/** Research subagents the driving agent may delegate to through pi-subagents. */
export interface SubagentConfig {
  /** Children the driver may run at once. */
  quantity: number;
  /** The model every child runs; its checkpointAtTokens is the context a child reports within. */
  model: LoopTier;
}

export interface LoopConfig {
  /** Ordered escalation ladder, cheapest/most-local first. */
  ladder: LoopTier[];
  /** Absent: the driver works alone. */
  subagents?: SubagentConfig;
  /**
   * Non-matching yields a tier gets before the loop *may* escalate.
   *
   * A floor, not a ceiling. The loop stops working a function when the evidence
   * says the search is out of moves, not when a counter runs out: over one
   * overnight run, 68% of the wall clock went into the nine functions the loop
   * gave up on, and at least two of them needed a four-line edit it never
   * reached. See `maxReturnsPerTier` for the runaway bound and
   * `tierMinutes` for the wall-clock one.
   */
  returnsPerTier: number;
  /**
   * Hard bound on returns per tier, whatever the evidence says.
   *
   * The evidence gate can be satisfied late or not at all — a turn that stops
   * measuring stops producing evidence — so the counter still exists. It is
   * just no longer the thing that decides.
   */
  maxReturnsPerTier: number;
  /** Wall-clock ceiling per tier, in minutes; the tier escalates rather than parks. Zero disables. */
  tierMinutes: number;
  /**
   * Distinct measurements with no improvement before the loop may park.
   *
   * Counted from the experiment ledger, so it survives a context clear and a
   * model change, and so respellings of an already-measured program do not
   * count as failures to move.
   */
  parkAfterStalledMeasurements: number;
  /**
   * Acknowledge a one-rung ladder.
   *
   * A single tier silently disables escalation, the handoff, and policy
   * adjudication — which turns every forbidden construct into an immediate
   * human park. Three of nine parks in one overnight run were that. Requiring
   * the field makes it a decision instead of an accident.
   */
  singleTier: boolean;
  /** Upper bound on functions attempted in one loop invocation. */
  maxFunctions: number;
  /** Clear between tiers/functions; prep role boundaries always clear. */
  clearContextBetween: boolean;
  /** Capture matching-tier exit interviews; prep always has its own handoff. */
  handoffSummary: boolean;
  /** Give the agent one notes-only turn to record grouping evidence before committing. */
  updateFileGroupings: boolean;
  /** Commit each byte-exact, finalized function before moving to the next one. */
  commitOnMatch: boolean;
  /** Commit each parked function before moving to the next one. */
  commitOnPark: boolean;
  /** Where durable loop state is written (absolute). */
  runtimeDir: string;
  /** Directory for documents that need a human decision (project-relative). */
  approvalsDir: string;
}

/**
 * Why a function was parked.
 *
 * `blocked` is not a verdict on the function or on the model: the build cannot
 * express the function at all, so no source is reachable. It reads as
 * "escalation-exhausted" only if the two are conflated, and they need opposite
 * responses — one needs a build fix, the other needs a human's structural
 * judgement. `ovl_10_func_800BA394` should have been parked at minute one with
 * "the container has no rodata attribution for this function's jump table",
 * not at minute forty-six with "needs a new structural hypothesis".
 */
export type ParkReason =
  | "escalation-exhausted"
  | "asm-needs-human-approval"
  | "environment-guard"
  | "blocked";

export interface ParkRecord {
  functionName: string;
  /** Project-relative C file, so a note names the container's path, not `src/`. */
  sourcePath?: string;
  reason: ParkReason;
  parkedAt: string;
  /** Ladder tier the loop reached before parking. */
  reachedTier: string;
  /**
   * Oracle report for the source the park preserved, re-measured at park time.
   *
   * Re-measured rather than carried forward because the park may preserve a
   * different program from the one the last turn left on disk, and a note whose
   * report describes a program its own listing does not contain is worse than
   * no report.
   */
  lastReport: string;
  /** Which program was preserved and why — always stated, never inferred. */
  attemptNote?: string;
  findings: PolicyFinding[];
}

export interface ApprovalRecord {
  functionName: string;
  kinds: string[];
  approvedAt: string;
  /** Tier that granted the exemption. */
  approvedBy: string;
  rationale: string;
}

/**
 * One `/auto_decompilation_loop repair` attempt on a function that already
 * matches through source-policy exceptions.
 *
 * `in-progress` is written before the loop touches the tree, so a session that
 * dies mid-repair can be put back on its matching source by the next run.
 * `failed` takes the function out of the queue until it is named explicitly;
 * `interrupted` (the operator stopped the loop) leaves it in.
 */
export interface RepairRecord {
  functionName: string;
  /** Project-relative C file. */
  sourcePath: string;
  status: "in-progress" | "repaired" | "failed" | "interrupted";
  startedAt: string;
  finishedAt?: string;
  /** The commit the repair started from; recovery only restores onto it. */
  head: string;
  /** The allowlist keys the function answers to, and the kinds the repair revoked under them. */
  keys: string[];
  revokedKinds: string[];
  /** Ladder tier the loop reached. */
  reachedTier?: string;
  /** Oracle report for the last program on disk before the restore. */
  lastReport?: string;
  /** Which clean program the note preserves, and how it measured. */
  attemptNote?: string;
  /** Project-relative note written for a failed repair. */
  notePath?: string;
}

/**
 * One `/auto_decompilation_loop fold` dispatch: one module reconstruction.
 *
 * Written as `interrupted` before the agent touches the tree and rewritten
 * when the dispatch settles, so a session that dies mid-fold leaves a record
 * the next `fold` run can recover from. `head` and `changedFiles` say exactly
 * what a restore may put back; nothing else in the tree is the loop's.
 */
export interface FoldRecord {
  container: string;
  moduleName: string;
  sourcePath: string;
  members: string[];
  status: "accepted" | "failed" | "interrupted";
  foldedAt: string;
  tier: string;
  beforeFolded: number;
  afterFolded: number;
  /** The commit the dispatch started from; a restore only ever goes back onto it. */
  head?: string;
  /** Project-relative files the dispatch changed, for an interrupted fold's recovery. */
  changedFiles?: string[];
  /** Of those, the ones already dirty before the dispatch, with the blob each held then (null: absent); a restore puts that back, not HEAD. */
  preexisting?: Record<string, string | null>;
  /** Why a fold failed, or how an interrupted one was settled. */
  detail?: string;
  commit?: string;
}

export interface LoopState {
  parked: Record<string, ParkRecord>;
  approvals: Record<string, ApprovalRecord>;
  completions?: Record<string, Completion>;
  repairs?: Record<string, RepairRecord>;
  folds?: FoldRecord[];
}

export type FunctionOutcome =
  | { kind: "matched"; functionName: string; tier: string; changedFiles: string[]; documentation?: "pending" | "passed"; commit?: string }
  | { kind: "parked"; functionName: string; record: ParkRecord; commit?: string }
  | { kind: "repaired"; functionName: string; tier: string; changedFiles: string[]; commit?: string }
  | { kind: "repair-failed"; functionName: string; record: RepairRecord; commit?: string }
  | { kind: "folded"; functionName: string; members: string[]; moduleName: string; tier: string; changedFiles: string[]; commit?: string }
  | { kind: "fold-failed"; functionName: string; members: string[]; record: FoldRecord; detail: string; commit?: string }
  | { kind: "skipped"; functionName: string; detail: string }
  | { kind: "aborted"; functionName: string }
  | { kind: "environment-broken"; functionName: string; detail: string };

export interface PrepHandoffSummary {
  functionName: string;
  candidatePath: string;
  headerChanges: string;
  sdkIdioms: string;
  compilation: string;
  unresolved: string;
  source: "tool" | "prose";
}

export interface HandoffSummary {
  functionName: string;
  whatWasTried: string;
  ruledOut: string;
  currentDivergence: string;
  leadingHypothesis: string;
  /** "tool" when the tier filled in the structured form; "prose" when it was scraped from the turn. */
  source: "tool" | "prose";
}

export type PolicyVerdict =
  | { decision: "approve"; rationale: string }
  | { decision: "reject"; rationale: string };
