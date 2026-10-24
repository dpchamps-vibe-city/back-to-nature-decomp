/**
 * translationUnits.ts — original functions and reconstructed translation units
 * are different tables.
 *
 * An original function is a fact about the binary: a container, an address, an
 * extent and a name (`loadOriginalFunctionInventory`). A translation unit is a
 * fact about this project's reconstruction: one configured `c` subsegment, one
 * source file, one object, and every original function whose start falls in
 * its interval. While each function had its own file the two coincided, and
 * most tooling still built `<srcDir>/<function>.c` and `<objDir>/<function>.c.o`
 * by hand. A reconstructed module defines several functions, and none of those
 * paths exist for its members.
 *
 * So a function's source, object, flags and data sections are looked up here,
 * through its translation unit:
 *
 *   function identity -> TU source -> TU object -> function symbol within it
 *
 * Module convention: a reconstructed module is configured under a `modules/`
 * stem — `src/modules/<name>.c` for the executable,
 * `src/overlays/<container>/modules/<name>.c` for an overlay — and its object
 * is the matching `.c.o`. That directory is the source tree's record of which
 * units were deliberately reconstructed; there is no second tracking file.
 *
 * Nothing here parses C or reads objects. The structural audit that does is
 * `translationUnitAudit.ts`.
 */

import { existsSync, readdirSync, statSync } from "fs";
import { join, posix } from "path";
import { ROOT } from "./psxExeInfo.js";
import { containerPath, loadContainers, requireContainer, type Container } from "./container.js";
import {
  loadOriginalFunctionInventory,
  loadSubsegments,
  type OriginalFunctionSpan,
  type SplatSubsegment,
} from "./symbolIndex.js";

/** The directory, under a container's source root, that holds reconstructed modules. */
export const MODULE_DIR = "modules";

export interface TranslationUnit {
  container: string;
  /** The `c` subsegment's name: the source stem relative to the container's source root. */
  segmentName: string;
  /** Project-relative source and object paths. */
  sourcePath: string;
  objectPath: string;
  startVram: number;
  /** Exclusive: the next configured subsegment of any kind. */
  endVram: number;
  startRom: number;
  endRom: number;
  /** Original game functions whose start lies in [startRom, endRom), ascending. */
  members: OriginalFunctionSpan[];
  /** Configured under `modules/`: a deliberately reconstructed unit. */
  foldedModule: boolean;
}

function resolveContainer(container?: Container | string): Container {
  if (container === undefined) return requireContainer("exe");
  return typeof container === "string" ? requireContainer(container) : container;
}

/** True when a `c` subsegment stem names a reconstructed module. */
export function isModuleStem(stem: string): boolean {
  return stem.startsWith(`${MODULE_DIR}/`);
}

/** The module's own name: `modules/cd_loader` -> `cd_loader`. */
export function moduleNameOf(stem: string): string {
  return stem.slice(MODULE_DIR.length + 1);
}

/**
 * Why a module stem is not a semantic name, or null when it is.
 *
 * The directory is the progress record, so the name in it has to say what the
 * unit is. An address in the name — `func_80012345`, `ovl_11_func_800ABCDE`,
 * `D_80012345`, or a cosmetic `module_80012345` — only restates where the code
 * sits. A one-function module is fine; its name is the evidence it was
 * reviewed. Whether the name is *good* is a review question, not this one.
 */
export function moduleStemProblem(stem: string): string | null {
  if (!isModuleStem(stem)) return `${stem} is not under ${MODULE_DIR}/`;
  const name = moduleNameOf(stem);
  if (name.includes("/")) return `${stem}: modules are one level deep (${MODULE_DIR}/<name>)`;
  if (!/^[A-Za-z][A-Za-z0-9_]*$/.test(name)) return `${stem}: a module name is a C-identifier-shaped word`;
  if (/[0-9A-Fa-f]{6,}/.test(name)) return `${stem}: the name encodes an address, not a responsibility`;
  if (/^(?:ovl_\d+_)?(?:func|D|B|jtbl|L|T|module|mod|unit|tu|file)(?:_|$)/i.test(name) && /\d/.test(name)) {
    return `${stem}: a generated or numbered name is not a semantic one`;
  }
  return null;
}

/** Generic names a review should look at twice; never a rejection. */
const GENERIC_MODULE_NAMES = new Set(["game", "main", "misc", "common", "util", "utils", "helpers", "module", "code", "funcs", "functions", "stuff", "all"]);

export function moduleNameIsGeneric(stem: string): boolean {
  return GENERIC_MODULE_NAMES.has(moduleNameOf(stem).toLowerCase());
}

/**
 * Every configured C translation unit in one container, in link order.
 *
 * A unit begins at its `c` subsegment and ends at the next configured
 * subsegment of any kind, so it cannot silently swallow an assembly segment, a
 * library object or loaded data. Its members are the original functions whose
 * starts fall in that interval.
 */
export function loadTranslationUnits(container?: Container | string): TranslationUnit[] {
  return indexOf(resolveContainer(container)).units;
}

interface UnitIndex { stamp: string; units: TranslationUnit[]; byFunction: Map<string, TranslationUnit> }
const indexes = new Map<string, UnitIndex>();

/**
 * Units are memoised against the inputs they are derived from, so a lookup per
 * function stays cheap and a re-split or a fold is seen at once — a
 * long-running session never answers from a stale configuration.
 */
function indexOf(target: Container): UnitIndex {
  const stamp = (["splat", "functionsCsv", "symbolAddrs"] as const).map((key) => {
    const path = containerPath(target, key);
    if (!existsSync(path)) return "-";
    const stat = statSync(path);
    return `${stat.mtimeMs}:${stat.size}`;
  }).join("|");
  const cached = indexes.get(target.id);
  if (cached && cached.stamp === stamp) return cached;
  const units = computeTranslationUnits(target);
  const byFunction = new Map<string, TranslationUnit>();
  for (const unit of units) for (const member of unit.members) byFunction.set(member.name, unit);
  const index = { stamp, units, byFunction };
  indexes.set(target.id, index);
  return index;
}

function computeTranslationUnits(target: Container): TranslationUnit[] {
  return buildTranslationUnits(
    { id: target.id, srcDir: target.paths.srcDir, objDir: target.paths.objDir },
    loadSubsegments(target),
    loadOriginalFunctionInventory(target).functions,
  );
}

/** The units from their inputs alone: the pure core of `loadTranslationUnits`. */
export function buildTranslationUnits(
  target: { id: string; srcDir: string; objDir: string },
  segments: readonly SplatSubsegment[],
  inventory: readonly OriginalFunctionSpan[],
): TranslationUnit[] {
  const functions = inventory.filter((fn) => fn.origin === "game");
  const units: TranslationUnit[] = [];
  for (const segment of segments) {
    if (segment.type !== "c" || !segment.name) continue;
    const startRom = segment.rom, endRom = segment.rom + segment.size;
    units.push({
      container: target.id,
      segmentName: segment.name,
      sourcePath: posix.join(target.srcDir, `${segment.name}.c`),
      objectPath: posix.join(target.objDir, `${segment.name}.c.o`),
      startVram: segment.vram,
      endVram: segment.vram + segment.size,
      startRom,
      endRom,
      members: functions.filter((fn) => fn.rom >= startRom && fn.rom < endRom),
      foldedModule: isModuleStem(segment.name),
    });
  }
  return units;
}

/** The translation unit whose interval holds a function's start, if one is configured. */
export function translationUnitForFunction(container: Container | string, vram: number): TranslationUnit | null {
  return loadTranslationUnits(container).find((unit) => vram >= unit.startVram && vram < unit.endVram) ?? null;
}

/** The translation unit that owns a named function, if one is configured. */
export function translationUnitOf(container: Container | string, functionName: string): TranslationUnit | null {
  return indexOf(resolveContainer(container)).byFunction.get(functionName) ?? null;
}

/**
 * The container and translation unit of a function named without one.
 *
 * Overlay symbols carry their container as a prefix, so the name usually
 * settles it; the scan over every container is the fallback for a name that
 * does not.
 */
export function locateTranslationUnit(functionName: string, preferred?: string): { container: Container; unit: TranslationUnit } | null {
  const containers = loadContainers();
  const ordered = preferred
    ? containers.filter((c) => c.id === preferred)
    : [...containers.filter((c) => c.kind !== "exe" && functionName.startsWith(`${c.id}_`)), ...containers];
  for (const container of ordered) {
    const unit = translationUnitOf(container, functionName);
    if (unit) return { container, unit };
  }
  return null;
}

/**
 * The project-relative C file that defines a function.
 *
 * Its translation unit's source when one is configured; otherwise the
 * per-function path the function would be given, which is where a new stub is
 * created and is never a module.
 */
export function functionSourcePath(container: Container | string, functionName: string): string {
  const target = resolveContainer(container);
  return translationUnitOf(target, functionName)?.sourcePath ?? posix.join(target.paths.srcDir, `${functionName}.c`);
}

/** The project-relative object the build compiles a function into. */
export function functionObjectPath(container: Container | string, functionName: string): string {
  const target = resolveContainer(container);
  return translationUnitOf(target, functionName)?.objectPath ?? posix.join(target.paths.objDir, `${functionName}.c.o`);
}

/** True when the function's translation unit is a reconstructed module. */
export function isModuleMember(container: Container | string, functionName: string): boolean {
  return translationUnitOf(container, functionName)?.foldedModule ?? false;
}

/**
 * The `configs/flag_overrides.mk` key for a source file, exactly as the
 * Makefile's FlagsSwitch computes it.
 *
 * A per-function file keeps its basename — `CC1FLAGS_func_80012345` — so every
 * existing override still applies. A module is keyed by its path below `src/`,
 * with `/` spelled `__`, because module names are only unique within their
 * container: `modules/sound` in two overlays must not share an override.
 */
export function flagOverrideKey(sourcePath: string): string {
  const normalized = sourcePath.replace(/\\/g, "/");
  const module = normalized.match(/(?:^|\/)src\/((?:overlays\/[^/]+\/)?modules\/[^/]+)\.c$/);
  if (module) return module[1]!.split("/").join("__");
  return posix.basename(normalized).replace(/\.c$/, "");
}

/**
 * The flag-override key for a function: its translation unit's.
 *
 * Inside a module every member compiles under the module's one command line,
 * so a function-named override no longer applies to it — which is why a fold
 * must reconcile per-function overrides first instead of carrying them along.
 */
export function functionFlagOverrideKey(container: Container | string, functionName: string): string {
  return flagOverrideKey(functionSourcePath(container, functionName));
}

/**
 * Module sources present on disk under a container's source root, whether or
 * not a subsegment configures them. A configured unit has a subsegment of the
 * same stem; anything else is an orphan.
 */
export function moduleSourcesOnDisk(container: Container | string): string[] {
  const target = resolveContainer(container);
  const directory = join(ROOT, target.paths.srcDir, MODULE_DIR);
  if (!existsSync(directory)) return [];
  return readdirSync(directory)
    .filter((file) => file.endsWith(".c"))
    .map((file) => posix.join(target.paths.srcDir, MODULE_DIR, file))
    .sort();
}

/** A reconstructed module as its source declares it: the symbols it defines. */
export interface ModuleDeclaration {
  /** `modules/<name>` */
  stem: string;
  /** Original-function symbols the module source defines or stubs. */
  names: ReadonlySet<string>;
}

/**
 * The `c` subsegments for an address-ordered function list, honouring modules.
 *
 * Config generators used to emit one `c` line per function, which would expand
 * a folded module back into per-function units on the next split. Here every
 * function a module source defines is placed in that module instead: one line,
 * at its first member's address. The source tree is the record — no list of
 * modules is kept anywhere else.
 *
 * Refuses rather than guesses: a module whose members are not contiguous in the
 * list, a function two modules both claim, or a module none of whose members
 * exist would each produce a config that silently means something else.
 */
export function moduleCSegments<T extends { name: string; address: number }>(
  functions: readonly T[],
  modules: readonly ModuleDeclaration[],
): Array<{ address: number; name: string; members: T[] }> {
  const owner = new Map<string, string>();
  for (const module of modules) {
    for (const name of module.names) {
      const previous = owner.get(name);
      if (previous && previous !== module.stem) throw new Error(`${name} is defined by both ${previous} and ${module.stem}`);
      owner.set(name, module.stem);
    }
  }
  const ordered = [...functions].sort((a, b) => a.address - b.address);
  const segments: Array<{ address: number; name: string; members: T[] }> = [];
  const placed = new Map<string, { address: number; name: string; members: T[] }>();
  for (const fn of ordered) {
    const stem = owner.get(fn.name);
    if (!stem) {
      segments.push({ address: fn.address, name: fn.name, members: [fn] });
      continue;
    }
    const open = placed.get(stem);
    if (!open) {
      const segment = { address: fn.address, name: stem, members: [fn] };
      placed.set(stem, segment);
      segments.push(segment);
      continue;
    }
    if (segments[segments.length - 1] !== open) {
      const between = segments[segments.length - 1]!;
      throw new Error(`${stem} is not contiguous: ${between.name} lies between its members ${open.members[open.members.length - 1]!.name} and ${fn.name}`);
    }
    open.members.push(fn);
  }
  for (const module of modules) {
    if (!placed.has(module.stem)) throw new Error(`${module.stem} defines no function this layout has (${[...module.names].join(", ") || "none"})`);
  }
  return segments;
}
