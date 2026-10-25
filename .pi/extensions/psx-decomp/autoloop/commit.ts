import { runCommand } from "../../shared/process.ts";
import type { ParkReason } from "./types.ts";
import type { Completion } from "../tools/prepared-attempt.ts";

type MatchAttribution = Pick<Completion, "origin" | "tier" | "documentationModel"> &
  Partial<Pick<Completion, "documentation" | "error">>;

export interface CommitResult {
  committed: boolean;
  detail: string;
  /** git itself refused, as opposed to there being nothing to commit. */
  failed?: boolean;
}

/**
 * The subject line matches the project's history: `match <function>`, with the
 * matching tier recorded separately from the documentation model. Static
 * verification is an origin, not a model. No attribution trailers. A match is
 * committed before its documentation when documentation did not finish; the
 * body says so, and the notes arrive in a later commit when it resumes.
 */
export function commitMessage(functionName: string, tierLabel: string, attribution?: MatchAttribution): string {
  const finalized = attribution?.origin === "static"
    ? "Byte-exact and finalized by /auto_decompilation_loop via static verification."
    : attribution && !attribution.tier
      ? "Byte-exact and finalized by /auto_decompilation_loop. Matching tier not recorded."
      : `Byte-exact and finalized by /auto_decompilation_loop on ${attribution?.tier ?? tierLabel}.`;
  const documentation = attribution?.documentationModel
    ? [`Documentation completed on ${attribution.documentationModel}.`]
    : attribution?.documentation === "pending"
      ? [`Documentation pending: ${attribution.error ?? "not completed"}.`]
      : [];
  return [
    `match ${functionName}`,
    "",
    finalized,
    ...documentation,
  ].join("\n");
}

/**
 * A park is a result too, and it is spelled `park <function>` so history says
 * plainly which functions the ladder handed back rather than finished.
 */
export function parkCommitMessage(
  functionName: string,
  reason: ParkReason,
  tierLabel: string,
  notePath: string,
): string {
  return [
    `park ${functionName}`,
    "",
    `Parked by /auto_decompilation_loop on ${tierLabel}: ${reason}.`,
    `The function is back on INCLUDE_ASM and needs a human decision; see ${notePath}.`,
  ].join("\n");
}

/**
 * A repair takes exceptions away from a function that already matched, so it is
 * spelled `clean <function>`: history should say the match did not change and
 * the source did.
 */
export function repairCommitMessage(functionName: string, tierLabel: string, revokedKinds: string[]): string {
  return [
    `clean ${functionName}`,
    "",
    `Removed the ${revokedKinds.join(", ")} exception(s) and their allowlist entry.`,
    `Still byte-exact; finalized by /auto_decompilation_loop repair on ${tierLabel}.`,
  ].join("\n");
}

/** A repair that did not finish leaves the source as it was; only its record is committed. */
export function repairAttemptCommitMessage(functionName: string, tierLabel: string, notePath: string): string {
  return [
    `repair attempt ${functionName}`,
    "",
    `/auto_decompilation_loop repair did not reach a clean match on ${tierLabel}.`,
    `The matching source and its allowlist entry are unchanged; see ${notePath}.`,
  ].join("\n");
}

/**
 * A fold reorganizes source without changing a byte, so it is spelled
 * `fold <module>`: history names the reconstructed unit and its members.
 */
export function foldCommitMessage(moduleName: string, members: string[], tierLabel: string, beforeFolded: number, afterFolded: number, outsideRoots: string[] = [], preexisting: string[] = []): string {
  return [
    `fold ${moduleName}`,
    "",
    `Reconstructed translation unit ${moduleName}: ${members.join(", ")}.`,
    `Folded ${beforeFolded} -> ${afterFolded} eligible functions; every container still byte-identical.`,
    `Accepted by /auto_decompilation_loop fold on ${tierLabel}.`,
    ...(outsideRoots.length ? ["", `Includes changes outside the integration roots: ${outsideRoots.join(", ")}.`] : []),
    ...(preexisting.length ? ["", `Also carries uncommitted changes that pre-date this fold, in files it edited: ${preexisting.join(", ")}.`] : []),
  ].join("\n");
}

export async function commitFold(projectRoot: string, moduleName: string, members: string[], tierLabel: string, beforeFolded: number, afterFolded: number, files: string[], outsideRoots: string[] = [], preexisting: string[] = []): Promise<CommitResult> {
  return commitFiles(projectRoot, files, foldCommitMessage(moduleName, members, tierLabel, beforeFolded, afterFolded, outsideRoots, preexisting));
}

export async function commitRepairedFunction(projectRoot: string, functionName: string, tierLabel: string, revokedKinds: string[], files: string[]): Promise<CommitResult> {
  return commitFiles(projectRoot, files, repairCommitMessage(functionName, tierLabel, revokedKinds));
}

export async function commitRepairAttempt(projectRoot: string, functionName: string, tierLabel: string, notePath: string, files: string[]): Promise<CommitResult> {
  return commitFiles(projectRoot, files, repairAttemptCommitMessage(functionName, tierLabel, notePath));
}

/**
 * Commit exactly the files this function's work touched, on the current branch.
 *
 * The file list comes from the loop's own changed-file reading, which has
 * already subtracted the dirt that pre-dated the loop — so unrelated working
 * tree state stays where the user left it instead of being swept into an
 * automated commit.
 */
async function commitFiles(projectRoot: string, files: string[], message: string): Promise<CommitResult> {
  if (files.length === 0) return { committed: false, detail: "nothing to commit" };

  const staged = await runCommand("git", ["add", "--", ...files], { cwd: projectRoot, timeoutMs: 60_000 });
  if (staged.code !== 0) {
    return { committed: false, failed: true, detail: `git add failed: ${staged.stderr || staged.stdout}` };
  }

  const pending = await runCommand("git", ["diff", "--cached", "--name-only"], { cwd: projectRoot, timeoutMs: 60_000 });
  if (pending.code === 0 && !pending.stdout.trim()) {
    return { committed: false, detail: "nothing staged" };
  }

  const commit = await runCommand("git", ["commit", "-m", message], {
    cwd: projectRoot,
    timeoutMs: 120_000,
  });
  if (commit.code !== 0) {
    return { committed: false, failed: true, detail: `git commit failed: ${commit.stderr || commit.stdout}` };
  }

  const head = await runCommand("git", ["rev-parse", "--short", "HEAD"], { cwd: projectRoot, timeoutMs: 30_000 });
  return { committed: true, detail: head.code === 0 ? head.stdout.trim() : "committed" };
}

export async function commitMatchedFunction(
  projectRoot: string,
  functionName: string,
  tierLabel: string,
  files: string[],
  attribution?: MatchAttribution,
): Promise<CommitResult> {
  return commitFiles(projectRoot, files, commitMessage(functionName, tierLabel, attribution));
}

/**
 * Commit the park, for the same reason a match is committed: the next function
 * must start from a tree that holds no unfinished business of the last one.
 *
 * An uncommitted park is charged to whoever comes next. Its `src/<fn>.c` and
 * its approvals note are dirt the loop cannot tell apart from the next
 * function's own edits, so they fail that function's scope gate, or ride into
 * its `match` commit under a subject line that names the wrong function. Either
 * way the park's cost is paid by a function that had nothing to do with it.
 */
export async function commitParkedFunction(
  projectRoot: string,
  functionName: string,
  reason: ParkReason,
  tierLabel: string,
  notePath: string,
  files: string[],
): Promise<CommitResult> {
  return commitFiles(projectRoot, files, parkCommitMessage(functionName, reason, tierLabel, notePath));
}
