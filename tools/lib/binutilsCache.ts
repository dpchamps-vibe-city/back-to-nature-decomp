/**
 * binutilsCache.ts — readelf, nm and objcopy output for a file, cached by the file's bytes.
 *
 * The SDK-library steps of `make split` query the same few hundred extracted
 * objects with the same invocations once per tool, and again in every tool
 * that runs another: about 5,400 processes, each through a shell, per split.
 * An invocation's output is a function of the executable, its arguments and
 * the file's bytes, which is the key. A run that fails is never cached, and
 * throws as `execSync` threw, so callers keep their failure handling.
 */
import { execFileSync } from "node:child_process";
import { randomUUID } from "node:crypto";
import { accessSync, constants, existsSync, mkdirSync, readFileSync, renameSync, rmSync, statSync, writeFileSync } from "node:fs";
import { delimiter, join, resolve } from "node:path";
import { digest } from "./contentCache.js";
import { ROOT } from "./psxExeInfo.js";

const CACHE_DIR = join(ROOT, "build/cache/binutils");
const OBJCOPY = "mips-linux-gnu-objcopy";

/** The bytes of the executable PATH resolves `name` to, once per process; null when it resolves nothing. */
const executables = new Map<string, string | null>();
export function executableDigest(name: string): string | null {
  if (!executables.has(name)) {
    const path = (process.env.PATH ?? "").split(delimiter).filter(Boolean).map((dir) => join(dir, name)).find((candidate) => {
      try {
        accessSync(candidate, constants.X_OK);
        return statSync(candidate).isFile();
      } catch {
        return false;
      }
    });
    executables.set(name, path ? digest(readFileSync(path)) : null);
  }
  return executables.get(name)!;
}

const files = new Map<string, { size: number; mtimeMs: number; digest: string }>();
function contentDigest(path: string): string | null {
  if (!existsSync(path)) return null;
  const { size, mtimeMs } = statSync(path);
  const known = files.get(path);
  if (known && known.size === size && known.mtimeMs === mtimeMs) return known.digest;
  const value = digest(readFileSync(path));
  files.set(path, { size, mtimeMs, digest: value });
  return value;
}

function keyOf(tool: string, args: readonly string[], path: string): string | null {
  const executable = executableDigest(tool);
  const content = contentDigest(path);
  return executable && content ? digest(JSON.stringify([1, tool, executable, args, content])) : null;
}

function cached(key: string | null): Buffer | undefined {
  if (!key) return undefined;
  try {
    return readFileSync(join(CACHE_DIR, key));
  } catch {
    return undefined;
  }
}

/* Renamed into place: a concurrent writer leaves a whole entry, never part of one. */
function store(key: string | null, value: Buffer): void {
  if (!key) return;
  mkdirSync(CACHE_DIR, { recursive: true });
  const scratch = join(CACHE_DIR, `${key}.${randomUUID()}.tmp`);
  try {
    writeFileSync(scratch, value);
    renameSync(scratch, join(CACHE_DIR, key));
  } finally {
    rmSync(scratch, { force: true });
  }
}

/** The standard output of `tool ...args file`, run from `cwd`, stderr discarded. */
export function toolOutput(tool: string, args: readonly string[], file: string, cwd = process.cwd()): string {
  const key = keyOf(tool, args, resolve(cwd, file));
  const hit = cached(key);
  if (hit) return hit.toString("utf-8");
  const output = execFileSync(tool, [...args, file], { cwd, stdio: ["ignore", "pipe", "ignore"], maxBuffer: 256 * 1024 * 1024 });
  store(key, output);
  return output.toString("utf-8");
}

/** The bytes `objcopy -O binary -j <section>` writes for a file: empty when the section is absent. */
export function sectionBytes(file: string, section: string, cwd = process.cwd()): Buffer {
  const args = ["-O", "binary", "-j", section];
  const key = keyOf(OBJCOPY, args, resolve(cwd, file));
  const hit = cached(key);
  if (hit) return hit;
  mkdirSync(CACHE_DIR, { recursive: true });
  const out = join(CACHE_DIR, `objcopy-${randomUUID()}.tmp`);
  try {
    execFileSync(OBJCOPY, [...args, file, out], { cwd, stdio: "ignore" });
    const bytes = readFileSync(out);
    store(key, bytes);
    return bytes;
  } finally {
    rmSync(out, { force: true });
  }
}
