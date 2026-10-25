import { snapshotDirt, type DirtSnapshot } from "../../shared/workspace.ts";

/**
 * Session-scoped snapshot of the workspace's dirty files, captured once at
 * extension activation, with the content each one had then.
 *
 * The finalize scope gate must only fail on files the worker itself touched
 * during the session. Dirt that pre-dates the session (user WIP, local vendor
 * patches, unrelated tool edits) is recorded here and subtracted from the
 * scope check, so it cannot block finalization of an unrelated function. It is
 * subtracted only while its content is unchanged: a file the worker edits on
 * top of old dirt is the worker's change.
 *
 * Fail-safe: if the snapshot cannot be taken, the baseline is empty and the
 * scope gate keeps its original strict behavior.
 */
let baselinePromise: Promise<DirtSnapshot> | undefined;

export function captureSessionBaseline(projectRoot: string): void {
  if (baselinePromise) return;
  baselinePromise = snapshotDirt(projectRoot).catch(() => new Map<string, string | null>());
}

export function getSessionBaseline(projectRoot: string): Promise<DirtSnapshot> {
  captureSessionBaseline(projectRoot);
  return baselinePromise as Promise<DirtSnapshot>;
}
