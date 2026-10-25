import { createHash, randomUUID } from "node:crypto";
import { existsSync, lstatSync, mkdirSync, mkdtempSync, readlinkSync, readdirSync, rmSync, symlinkSync, writeFileSync } from "node:fs";
import { tmpdir } from "node:os";
import { dirname, join, resolve } from "node:path";
import type { AutodecompConfig, WorkspaceInfo } from "./types.ts";
import { runCommand } from "./process.ts";

async function git(projectRoot: string, args: string[], cwd = projectRoot, env?: NodeJS.ProcessEnv, timeoutMs = 120_000, signal?: AbortSignal) {
  const result = await runCommand("git", args, { cwd, env, timeoutMs, signal });
  if (result.code !== 0) throw new Error(`git ${args.join(" ")} failed:\n${result.stderr || result.stdout}`);
  /* trimEnd only: a leading trim would eat the status column of the first
     --porcelain line (" M path" -> "M path"), corrupting slice(3) parsing */
  return result.stdout.trimEnd();
}

export async function headRevision(projectRoot: string): Promise<string> {
  return git(projectRoot, ["rev-parse", "HEAD"]);
}

export async function trackedDirtyFiles(projectRoot: string): Promise<string[]> {
  const output = await git(projectRoot, ["status", "--porcelain=v1", "--untracked-files=no", "--ignore-submodules=dirty"]);
  return output ? output.split("\n").map((line) => line.slice(3).trim()).filter(Boolean) : [];
}

async function gitDir(projectRoot: string, signal?: AbortSignal): Promise<string> {
  const path = await git(projectRoot, ["rev-parse", "--absolute-git-dir"], projectRoot, undefined, 120_000, signal);
  return resolve(path);
}

export async function createTreeFromWorktree(
  projectRoot: string,
  worktree: string,
  roots: string[],
  signal?: AbortSignal,
): Promise<string> {
  const temp = mkdtempSync(join(tmpdir(), "autodecomp-index-"));
  const index = join(temp, "index");
  try {
    const env = { GIT_INDEX_FILE: index, GIT_DIR: await gitDir(projectRoot, signal), GIT_WORK_TREE: resolve(worktree) };
    await git(projectRoot, ["read-tree", "HEAD"], worktree, env, 120_000, signal);
    await git(projectRoot, ["add", "-A", "--", ...roots], worktree, env, 120_000, signal);
    return await git(projectRoot, ["write-tree"], worktree, env, 120_000, signal);
  } finally {
    rmSync(temp, { recursive: true, force: true });
  }
}

export async function treePatch(projectRoot: string, fromTree: string, toTree: string, roots: string[], signal?: AbortSignal): Promise<string> {
  const result = await runCommand("git", ["diff", "--binary", "--no-ext-diff", fromTree, toTree, "--", ...roots], {
    cwd: projectRoot,
    timeoutMs: 120_000,
    maxCaptureBytes: 32 * 1024 * 1024,
    signal,
  });
  if (result.code !== 0) throw new Error(`Unable to create integration patch:\n${result.stderr || result.stdout}`);
  return result.stdout;
}

export async function changedFilesBetweenTrees(projectRoot: string, fromTree: string, toTree: string, signal?: AbortSignal): Promise<string[]> {
  const output = await git(projectRoot, ["diff", "--name-only", fromTree, toTree], projectRoot, undefined, 120_000, signal);
  return output ? output.split("\n").filter(Boolean) : [];
}

export async function workspaceChangedFiles(workspace: string, signal?: AbortSignal): Promise<string[]> {
  const output = await git(workspace, ["status", "--porcelain=v1", "--untracked-files=all", "--ignore-submodules=all"], workspace, undefined, 120_000, signal);
  if (!output) return [];
  const files: string[] = [];
  for (const line of output.split("\n")) {
    const raw = line.slice(3).trim();
    const renamed = raw.includes(" -> ") ? raw.split(" -> ").at(-1)! : raw;
    if (renamed) files.push(renamed.replace(/^\"|\"$/g, ""));
  }
  return [...new Set(files)].sort();
}

/**
 * Dirty files and the content each had when the snapshot was taken: a git blob
 * id, or null for a path that was absent. The blobs are written to the object
 * database, so a snapshot can also put a file back as it was.
 */
export type DirtSnapshot = Map<string, string | null>;

/** Blob ids of these paths as they stand now; null for a path that is absent. */
export async function contentIds(projectRoot: string, paths: readonly string[], options: { write?: boolean; signal?: AbortSignal } = {}): Promise<Map<string, string | null>> {
  const ids = new Map<string, string | null>();
  const present = paths.filter((path) => {
    const stat = lstatSync(join(projectRoot, path), { throwIfNoEntry: false });
    if (!stat?.isFile()) ids.set(path, null);
    return !!stat?.isFile();
  });
  for (let start = 0; start < present.length; start += 200) {
    const chunk = present.slice(start, start + 200);
    const output = await git(projectRoot, ["hash-object", ...(options.write ? ["-w"] : []), "--", ...chunk], projectRoot, undefined, 120_000, options.signal);
    output.split("\n").forEach((id, index) => ids.set(chunk[index]!, id.trim()));
  }
  return ids;
}

/** Every dirty file with the content it has now. */
export async function snapshotDirt(projectRoot: string, signal?: AbortSignal): Promise<DirtSnapshot> {
  return contentIds(projectRoot, await workspaceChangedFiles(projectRoot, signal), { write: true, ...(signal ? { signal } : {}) });
}

/**
 * Split a changed-file list into files that were already dirty when a
 * baseline was captured (preExisting) and files dirtied afterwards (newFiles).
 * Used by the finalize scope gate so pre-existing workspace dirt (user WIP,
 * local vendor patches) cannot block finalization of an unrelated function.
 *
 * Given a snapshot and the current content ids, a file is pre-existing only
 * while its content is still what the snapshot recorded: an edit on top of
 * old dirt belongs to whoever made it. A path list alone matches by name.
 */
export function filterNewChanges(
  changedFiles: string[],
  baseline: Iterable<string> | ReadonlyMap<string, string | null>,
  current?: ReadonlyMap<string, string | null>,
): { newFiles: string[]; preExisting: string[] } {
  const snapshot = baseline instanceof Map ? baseline as ReadonlyMap<string, string | null> : undefined;
  const baselineSet = snapshot ? new Set(snapshot.keys()) : baseline instanceof Set ? baseline : new Set(baseline as Iterable<string>);
  const newFiles: string[] = [];
  const preExisting: string[] = [];
  for (const file of changedFiles) {
    const unchanged = baselineSet.has(file) && (!snapshot || !current || snapshot.get(file) === current.get(file));
    (unchanged ? preExisting : newFiles).push(file);
  }
  return { newFiles, preExisting };
}

/** filterNewChanges against a snapshot, reading the current content of the files it names. */
export async function newChangesSince(
  projectRoot: string,
  changedFiles: string[],
  baseline: DirtSnapshot,
  signal?: AbortSignal,
): Promise<{ newFiles: string[]; preExisting: string[] }> {
  const current = await contentIds(projectRoot, changedFiles.filter((file) => baseline.has(file)), signal ? { signal } : {});
  return filterNewChanges(changedFiles, baseline, current);
}

export function patchHash(patch: string): string {
  return createHash("sha256").update(patch).digest("hex");
}

function ensureSymlink(target: string, link: string): void {
  if (!existsSync(target)) return;
  mkdirSync(dirname(link), { recursive: true });
  try {
    const stat = lstatSync(link, { throwIfNoEntry: false });
    if (stat?.isSymbolicLink() && readlinkSync(link) === target) return;
    if (stat) rmSync(link, { recursive: true, force: true });
  } catch {
    /* Missing destination. */
  }
  symlinkSync(target, link);
}

/**
 * The make goals that populate a workspace's generated sources.
 *
 * `null` — an unpinned run — means every container, which is `split-all`. A
 * pinned run splits the executable plus each named container; `split-exe` is
 * spelled `split`, the target that predates containers and every note that
 * names it.
 */
export function splitTargets(containers: string[] | null): string[] {
  if (containers === null) return ["split-all"];
  const overlays = containers.filter((id) => id !== "exe").map((id) => `split-${id}`);
  return ["split", ...overlays];
}

export async function createWorkspace(
  projectRoot: string,
  config: AutodecompConfig,
  id: string = randomUUID(),
  containers: string[] | null = null,
): Promise<WorkspaceInfo> {
  const baseHead = await headRevision(projectRoot);
  const baselineTree = await createTreeFromWorktree(projectRoot, projectRoot, config.integration.allowedRoots);
  const path = resolve(config.runtimeDir, "workspaces", id);
  mkdirSync(dirname(path), { recursive: true });
  if (existsSync(path)) rmSync(path, { recursive: true, force: true });

  await git(projectRoot, ["worktree", "add", "--detach", path, baseHead], projectRoot, undefined, 120_000);
  try {
    const submodules = await runCommand("git", ["submodule", "update", "--init", "--recursive"], {
      cwd: path,
      timeoutMs: 10 * 60_000,
    });
    if (submodules.code !== 0) throw new Error(`Submodule preparation failed:\n${submodules.stderr || submodules.stdout}`);

    ensureSymlink(join(projectRoot, "extracted"), join(path, "extracted"));
    ensureSymlink(join(projectRoot, "node_modules"), join(path, "node_modules"));

    const oldGcc = join(projectRoot, "tools", "vendor", "old-gcc");
    if (existsSync(oldGcc)) {
      for (const entry of readdirSync(oldGcc)) {
        if (entry.startsWith("build-gcc-") && entry.endsWith("-psx")) {
          ensureSymlink(join(oldGcc, entry), join(path, "tools", "vendor", "old-gcc", entry));
        }
      }
    }

    if (baselineTree !== baseHead) {
      const baselinePatch = await treePatch(projectRoot, baseHead, baselineTree, config.integration.allowedRoots);
      if (baselinePatch.trim()) {
        const patchPath = join(config.runtimeDir, "workspaces", `${id}.baseline.patch`);
        writeFileSync(patchPath, baselinePatch);
        const applied = await runCommand("git", ["apply", "--whitespace=nowarn", patchPath], { cwd: path, timeoutMs: 120_000 });
        if (applied.code !== 0) throw new Error(`Unable to apply accumulated baseline to workspace:\n${applied.stderr || applied.stdout}`);
      }
    }

    /* Every container this workspace may have to build. The executable is
       always split — every overlay links against its symbol export — and beyond
       that either the named containers or, when the run is not pinned, all of
       them. Splitting only the executable leaves an overlay with no assembly to
       include and a link that fails on symbols the workspace never saw. */
    const targets = splitTargets(containers);
    const split = await runCommand("make", targets, { cwd: path, timeoutMs: 15 * 60_000 });
    if (split.code !== 0) throw new Error(`Workspace ${targets.join(" ")} failed:\n${split.stderr || split.stdout}`);
    return { id, path, baseHead, baselineTree };
  } catch (error) {
    await removeWorkspace(projectRoot, path);
    throw error;
  }
}

export async function cleanupRuntimeWorkspaces(projectRoot: string, runtimeDir: string): Promise<void> {
  const result = await runCommand("git", ["worktree", "list", "--porcelain"], { cwd: projectRoot, timeoutMs: 120_000 });
  if (result.code !== 0) return;
  const root = resolve(runtimeDir, "workspaces");
  for (const line of result.stdout.split("\n")) {
    if (!line.startsWith("worktree ")) continue;
    const path = resolve(line.slice("worktree ".length));
    if (path === root || path.startsWith(`${root}/`)) await removeWorkspace(projectRoot, path);
  }
}

export async function removeWorkspace(projectRoot: string, path: string): Promise<void> {
  await runCommand("git", ["worktree", "remove", "--force", path], { cwd: projectRoot, timeoutMs: 120_000 });
  await runCommand("git", ["worktree", "prune"], { cwd: projectRoot, timeoutMs: 120_000 });
  if (existsSync(path)) rmSync(path, { recursive: true, force: true });
}

export async function applyPatch(projectRoot: string, patchPath: string, signal?: AbortSignal): Promise<void> {
  const check = await runCommand("git", ["apply", "--check", patchPath], { cwd: projectRoot, timeoutMs: 120_000, signal });
  if (check.code !== 0) throw new Error(`Patch no longer applies to trunk:\n${check.stderr || check.stdout}`);
  signal?.throwIfAborted();
  /* Finish this short mutation before the caller's gate sees cancellation and
     rolls it back; killing git mid-write can leave a partially applied patch. */
  const apply = await runCommand("git", ["apply", "--whitespace=nowarn", patchPath], { cwd: projectRoot, timeoutMs: 120_000 });
  if (apply.code !== 0) throw new Error(`Patch application failed:\n${apply.stderr || apply.stdout}`);
}

export async function reversePatch(projectRoot: string, patchPath: string): Promise<void> {
  const result = await runCommand("git", ["apply", "-R", "--whitespace=nowarn", patchPath], { cwd: projectRoot, timeoutMs: 120_000 });
  if (result.code !== 0) throw new Error(`Patch rollback failed:\n${result.stderr || result.stdout}`);
}
