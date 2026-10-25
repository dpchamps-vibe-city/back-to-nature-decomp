/**
 * splitScope.ts — which re-splits a change set can need.
 *
 * The convergence gate proves the configuration is what the generators
 * produce: a re-split changes no config, header or source. Re-running every
 * generator proves it for any change, but a fold touches one container, and
 * the full `make split-all` re-derives all fourteen. A scope is sound when it
 * re-runs every generator whose inputs the change reaches:
 *
 *  - a generator or the Makefile itself changed: every container (`full`);
 *  - an EXE input changed (its sources, its configuration, shared
 *    configuration): the EXE split, and every overlay too if the engine
 *    export the overlays name their external calls from has changed;
 *  - an overlay's source or configuration changed: that overlay's split;
 *  - any source or header changed: the cross-container tail of the EXE split
 *    (context headers, globals.h, the project profile), which reads them all.
 *
 * Notes, plans, Pi configuration and tools no split recipe reaches are not
 * split inputs. Anything this does not recognise counts as an EXE input.
 */

import { existsSync, readFileSync } from "node:fs";
import { dirname, join, normalize } from "node:path";

/** The recipes that make up `make split-all`. */
const SPLIT_TARGETS = [/^split-sdk-libs:/, /^split:/, /^split-\$\(1\):/, /^\$\(ENGINE_SYMS\):/];

/** Every tools/*.ts file a split recipe runs, plus everything those import, project-relative. */
export function splitToolClosure(root: string): Set<string> {
  const makefile = readFileSync(join(root, "Makefile"), "utf8").split("\n");
  const entries = new Set<string>();
  for (let index = 0; index < makefile.length; index++) {
    if (!SPLIT_TARGETS.some((target) => target.test(makefile[index]!))) continue;
    for (let line = index + 1; line < makefile.length && /^\t/.test(makefile[line]!); line++) {
      for (const match of makefile[line]!.matchAll(/(?:^|\s)((?:tools|\.pi)\/[\w./-]+\.ts)\b/g)) entries.add(match[1]!);
    }
  }
  if (entries.size === 0) throw new Error("no split recipe names a tool; the Makefile's split targets were not recognised");

  const closure = new Set<string>();
  const pending = [...entries];
  while (pending.length) {
    const file = pending.pop()!;
    if (closure.has(file)) continue;
    const absolute = join(root, file);
    if (!existsSync(absolute)) continue;
    closure.add(file);
    const text = readFileSync(absolute, "utf8");
    for (const match of text.matchAll(/(?:\bfrom\s*|\bimport\s*\(\s*|^\s*import\s+)["'](\.{1,2}\/[^"']+)["']/gm)) {
      const target = normalize(join(dirname(file), match[1]!));
      const candidates = [target, target.replace(/\.js$/, ".ts"), `${target}.ts`, join(target, "index.ts")];
      const found = candidates.find((candidate) => /\.ts$/.test(candidate) && existsSync(join(root, candidate)));
      if (found) pending.push(found);
    }
  }
  return closure;
}

export interface SplitScope {
  /** Every container, because a generator or the Makefile changed. */
  full: boolean;
  /** The EXE split. */
  exe: boolean;
  /** Overlays whose own inputs changed. */
  overlays: string[];
  /** The cross-container tail: context headers, globals.h and the project profile. */
  tail: boolean;
  /** Why the widest part of the scope is needed. */
  reasons: string[];
}

/** Paths that feed no split recipe. */
function notASplitInput(path: string, tools: ReadonlySet<string>): boolean {
  if (/^(?:notes|plans|\.pi)\//.test(path)) return true;
  if (/^[^/]+\.md$/.test(path)) return true;
  return path.startsWith("tools/") && !tools.has(path);
}

/** The scope a set of changed paths needs, given the overlays' ids and the split tools' closure. */
export function splitScope(changed: readonly string[], overlays: readonly string[], tools: ReadonlySet<string>): SplitScope {
  const scope: SplitScope = { full: false, exe: false, overlays: [], tail: false, reasons: [] };
  const touched = new Set<string>();
  const owner = (path: string): string | undefined => {
    const id = path.match(/^src\/overlays\/([^/]+)\//)?.[1]
      ?? path.match(/^configs\/splat\/([^/]+)\.yaml$/)?.[1]
      ?? path.match(/^configs\/symbols\/([^/.]+)\.[^/]*$/)?.[1];
    return id && overlays.includes(id) ? id : undefined;
  };
  for (const path of changed) {
    if (notASplitInput(path, tools)) continue;
    if (path === "Makefile" || tools.has(path)) {
      scope.full = true;
      scope.reasons.push(`${path} is a split input`);
      continue;
    }
    const overlay = owner(path);
    if (overlay) {
      touched.add(overlay);
      if (path.startsWith("src/")) scope.tail = true;
      continue;
    }
    if (path.startsWith("include/")) {
      scope.tail = true;
      continue;
    }
    scope.exe = true;
    scope.tail = true;
    scope.reasons.push(`${path} is an EXE input`);
  }
  scope.overlays = overlays.filter((id) => touched.has(id));
  return scope;
}
