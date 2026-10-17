/** One content stamp for make's tool/configuration inputs. Header dependencies
 * live in each object's cpp depfile; this stamp covers changes timestamps miss. */
import { execFileSync } from "node:child_process";
import { readFileSync, existsSync, statSync } from "node:fs";
import { join, resolve } from "node:path";
import { digest, filesUnder, snapshot, writeIfChanged } from "../lib/contentCache.js";

/** Ask make for its actual source selection, without building anything. This
 * includes its executable wildcard and configured overlay C subsegments; a
 * second parser (or a stale call graph) cannot establish dependency coverage. */
export function configuredCSources(root: string): string[] {
  const goal = "__psx_inspect_c_sources";
  const output = execFileSync("make", ["--no-print-directory", "--silent",
    `--eval=.PHONY: ${goal}\n${goal}: ; @printf '%s\\n' $(C_SRCS) $(foreach overlay,$(OVERLAYS),$($(overlay)_C_SRCS))`, goal],
    { cwd: root, encoding: "utf8", timeout: 15_000, maxBuffer: 4 * 1024 * 1024 });
  const sources = output.trim().split("\n").filter(Boolean);
  if (sources.some((p) => !/^src\/[\w./-]+\.c$/.test(p) || p.split("/").includes("..")))
    throw new Error("make returned an invalid configured C source list");
  return [...new Set(sources)].sort();
}

export function buildToolInputs(root: string): string[] {
  const mk = readFileSync(join(root, "Makefile"), "utf8");
  const version = mk.match(/^GCC_VERSION\s*:=\s*(\S+)/m)?.[1];
  const cross = mk.match(/^CROSS\s*:=\s*(\S+)/m)?.[1] ?? "mips-linux-gnu-";
  const executables = ["python3", ...["cpp", "as", "ld", "objcopy"].map((name) => cross + name)];
  const frontend = execFileSync(cross + "cpp", ["-print-prog-name=cc1"], { cwd: root, encoding: "utf8" }).trim();
  return [frontend, "Makefile", "configs/flag_overrides.mk", "tools/build/buildInputs.ts", "tools/lib/contentCache.ts",
    `tools/vendor/old-gcc/build-gcc-${version}-psx/cc1`, "tools/vendor/maspsx/maspsx.py", "tools/vendor/maspsx/maspsx",
    ...executables.map((exe) => execFileSync("which", [exe], { cwd: root, encoding: "utf8" }).trim())];
}
export function refreshBuildInputs(root: string, values: string[] = []): boolean {
  const explicit = values.flatMap((v) => v.split(/\s+/)).filter((p) => p.length > 0 && existsSync(resolve(root, p)) && statSync(resolve(root, p)).isFile());
  const state = snapshot(root, [...buildToolInputs(root), ...explicit], (p) => !p.endsWith(".pyc"));
  const environment = Object.fromEntries(["CPATH", "C_INCLUDE_PATH", "GCC_EXEC_PREFIX", "COMPILER_PATH", "SOURCE_DATE_EPOCH", "PYTHONPATH"]
    .map((p) => [p, process.env[p] ?? null]));
  /* Newly available quote/angle headers can shadow an old dependency without
     touching it. Membership belongs in the stamp; content changes use depfiles. */
  const membership = filesUnder(join(root, "include"));
  return writeIfChanged(join(root, "build/toolchain-inputs.stamp"), digest(JSON.stringify({ state, values, environment, membership })) + "\n");
}
if (process.argv[1]?.endsWith("buildInputs.ts")) refreshBuildInputs(resolve("."), process.argv.slice(2));
