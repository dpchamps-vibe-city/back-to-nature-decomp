/**
 * contextExport.ts — Extract function signatures from decompiled C files into the m2c context
 *
 * Stage 4 of the decompilation pipeline. After a function is byte-matched and cleaned up,
 * this tool extracts its signature and adds it to the m2c context so that future
 * m2c runs and LLM agents have type context.
 *
 * Files are generated per container, and m2c must be given them in this order:
 *   include/sdk_types.h        — definitions of every type any signature names
 *   include/functions.h        — the PS-X EXE's signatures: the engine API
 *   include/overlays/<id>.h    — one overlay container's own signatures
 *
 * Deliverable 7 of plans/overlay-decompilation-enablement.md splits the second
 * file. Types stay shared, because the structures both bodies mutate are the
 * same structures; signatures do not, because two overlays sharing a RAM slot
 * can hold different functions at one address.
 *
 * All of them are m2c context only. Nothing #includes them, and they carry no
 * preprocessor directives, because m2c parses its context as plain C with no
 * preprocessor.
 *
 * Usage:
 *   npx tsx tools/agent/contextExport.ts func_80011F08          # extract from src/func_80011F08.c
 *   npx tsx tools/agent/contextExport.ts --all                   # every container
 *   npx tsx tools/agent/contextExport.ts --container ovl_11 --all
 *   npx tsx tools/agent/contextExport.ts func_80011F08 --dry-run # show what would be added
 */

import { execFileSync } from "child_process";
import { readFileSync, writeFileSync, existsSync, readdirSync, mkdirSync, rmSync } from "fs";
import { dirname, join, relative } from "path";
import { prototypesIn } from "./calleeTruth.js";
import { inspectType } from "./type-propagation/c-types.js";
import { contextImplementation, contextSourceReader } from "./contextSourceFacts.js";
import {
  extractPrototypesFromSource,
  extractSignaturesFromSource,
  harvestTypedefs,
  renderSdkTypesHeader,
  resolveTypes,
  typeNamesIn,
  type ExtractedSignature,
  type Resolution,
} from "./sdkTypes.js";

import {
  EXE_CONTAINER_ID,
  loadContainers,
  requireContainer,
  type Container,
} from "../lib/container.js";
import { requireFunctionLocation } from "../lib/symbolIndex.js";
import { functionSourcePath, loadTranslationUnits } from "../lib/translationUnits.js";
import { scopedTypeCatalog, projectScopedSignatures } from "./scopedTypes.js";
import { digest, readCache, writeCache, writeIfChanged } from "../lib/contentCache.js";
import { emptyDeclarationIndex, indexDeclarations, projectDeclarations } from "./declarationContext.js";
import { PREAMBLE_TYPES } from "./sdkTypes.js";

const ROOT = new URL("../..", import.meta.url).pathname;

const SDK_TYPES_HEADER = "include/sdk_types.h";
const FUNCTIONS_HEADER = "include/functions.h";

/** Where a container publishes the signatures its own translation units define. */
export function functionsHeaderFor(container: Container): string {
  return container.kind === "exe" ? FUNCTIONS_HEADER : `include/overlays/${container.id}.h`;
}

/**
 * The context files an m2c run for this container must be given, in order.
 *
 * An overlay translation unit calls the engine constantly — 246 measured entry
 * points — so the engine header is context for it too, and leaving it out is
 * how a callee gets declared implicit-int and poisons the caller's registers.
 */
export function contextFilesFor(container: Container): string[] {
  return container.kind === "exe"
    ? [SDK_TYPES_HEADER, FUNCTIONS_HEADER]
    : [SDK_TYPES_HEADER, FUNCTIONS_HEADER, functionsHeaderFor(container)];
}

export interface ExportResult {
  signatures: string[];
  skipped: boolean;
  reason?: string;
  /** Machine-readable publication outcome consumed by finalization. */
  status: "published" | "skipped";
  signatureMatches: boolean;
  localPrototypes: Array<{ name: string; file: string; signature: string; published: string }>;
}

export const CONTEXT_EXPORT_STATUS = "CONTEXT_EXPORT_STATUS ";

/** Ignore parameter names and trivia, but not arity, types or qualifiers. */
export function signatureContract(signature: string): string {
  const p = prototypesIn(signature, "context")[0];
  return p ? JSON.stringify([p.returnType?.replace(/\s+/g, " "), p.parameters, p.variadic,
    p.paramTypes?.map((t) => inspectType(t).key)]) : signature;
}

function filesUnder(root: string): string[] {
  if (!existsSync(root)) return [];
  return readdirSync(root, { withFileTypes: true }).flatMap((e) => e.isDirectory()
    ? filesUnder(join(root, e.name)) : [join(root, e.name)]);
}

/** These files are inference context, never compiler headers. AST inspection
 * distinguishes real directives from examples inside comments and strings. */
export function generatedContextIncludes(rootDir: string): string[] {
  const findings: string[] = [], read = contextSourceReader(rootDir);
  for (const path of [...filesUnder(join(rootDir, "src")), ...filesUnder(join(rootDir, "include"))]) {
    const file = relative(rootDir, path);
    if (!/\.[ch]$/.test(file) || file === SDK_TYPES_HEADER || file === FUNCTIONS_HEADER || file.startsWith("include/overlays/")) continue;
    for (const included of read(path).includes) {
      if (/(?:^|\/)functions\.h$|(?:^|\/)overlays\/[^/]+\.h$/.test(included.path)) findings.push(`${file}:${included.line}: ${included.path}`);
    }
  }
  return findings;
}

function localPrototypeDisagreements(rootDir: string, pairs: ExtractedSignature[]): ExportResult["localPrototypes"] {
  const signatures = new Map(pairs.map((p) => [p.name, p.signature])), read = contextSourceReader(rootDir);
  return filesUnder(join(rootDir, "src")).filter((p) => p.endsWith(".c")).flatMap((path) =>
    read(path).prototypes.flatMap((p) => {
      const published = signatures.get(p.name);
      return published && signatureContract(published) !== signatureContract(p.signature)
        ? [{ name: p.name, file: relative(rootDir, path), signature: p.signature, published }] : [];
    }));
}


/**
 * Extract the function definitions a decompiled C file publishes.
 */
export function extractSignaturePairs(cFilePath: string, read?: ReturnType<typeof contextSourceReader>): ExtractedSignature[] {
  if (!existsSync(cFilePath)) return [];
  if (read) return read(cFilePath).signatures;
  const source = readFileSync(cFilePath, "utf8");
  return source.includes("INCLUDE_ASM(") ? [] : extractSignaturesFromSource(source);
}

/**
 * Extract function signatures from a decompiled C file.
 */
export function extractSignatures(cFilePath: string): string[] {
  return extractSignaturePairs(cFilePath).map((s) => s.signature);
}

/**
 * Read the current include/functions.h and return a map of funcName -> signature.
 */
function readExistingHeader(headerPath: string): Map<string, string> {
  const map = new Map<string, string>();
  if (!existsSync(headerPath)) return map;

  for (const { name, signature } of extractPrototypesFromSource(readFileSync(headerPath, "utf-8"))) {
    map.set(name, signature);
  }
  return map;
}

/* ------------------------------------------------------------------ */
/* Context generation                                                  */
/* ------------------------------------------------------------------ */

/**
 * Resolve the type definitions the signatures require, and report which
 * signature pulled in any type that could not be resolved.
 */
export function resolveContextTypes(
  rootDir: string,
  signatures: Map<string, string>,
  catalog = scopedTypeCatalog(rootDir),
): { resolution: Resolution; defs: Map<string, string>; referencedBy: Map<string, string[]> } {
  /* A disabled parked body is not an independently witnessed definition. */
  for (const name of catalog.excludedFunctions) signatures.delete(name);
  projectScopedSignatures(signatures, catalog);
  const defs = catalog.defs;
  const key = digest(JSON.stringify([contextImplementation(["tools/agent/contextExport.ts"]), [...signatures], [...defs]]));
  const cache = join(rootDir, "build/cache/context-resolution.json");
  type Cached = { resolution: Resolution; referencedBy: Array<[string, string[]]> };
  const hit = readCache<Cached>(cache, key);
  if (hit) return { resolution: hit.resolution, defs, referencedBy: new Map(hit.referencedBy) };

  const referencedBy = new Map<string, string[]>();
  for (const sig of signatures.values()) {
    for (const name of typeNamesIn(sig)) {
      const users = referencedBy.get(name);
      if (users) users.push(sig);
      else referencedBy.set(name, [sig]);
    }
  }

  const model = emptyDeclarationIndex();
  indexDeclarations(model, [...PREAMBLE_TYPES.values()].join("\n"), "common.h", "public", "public-header");
  for (const [name, text] of defs) indexDeclarations(model, text, name, "public", "public-header");
  for (const [name, signature] of signatures) indexDeclarations(model, signature, name, "public", "public-header");
  const projection = projectDeclarations(model, signatures.keys(), "public");
  const resolution = { ordered: projection.selected.filter((d) => d.kind === "type" && !PREAMBLE_TYPES.has(d.name)).map((d) => d.name),
    unresolved: projection.unknown };
  writeCache(cache, key, { resolution, referencedBy: [...referencedBy] } satisfies Cached);
  return { resolution, defs, referencedBy };
}

/** Render include/functions.h: signatures only, sorted by name. */
function renderFunctionsHeader(signatures: Map<string, string>): string {
  const sorted = [...signatures.entries()].sort((a, b) => a[0].localeCompare(b[0]));
  return [
    "/* Auto-generated by tools/agent/contextExport.ts — do not edit manually */",
    "/* m2c context only. Types live in include/sdk_types.h, which must be",
    " * passed to m2c before this file. */",
    "",
    ...sorted.map(([, sig]) => sig),
    "",
  ].join("\n");
}

/**
 * Ask m2c to parse the generated context, in the order consumers use it.
 *
 * The pair is verified together: sdk_types.h parses in isolation whether or
 * not functions.h does, so a per-file check would pass on exactly the
 * configuration that is broken.
 */
export function verifyContextParses(
  rootDir: string,
  container: Container = requireContainer(EXE_CONTAINER_ID),
): { ok: boolean; skipped?: string; diagnostic?: string } {
  const asmRoot = join(rootDir, container.paths.asmDir, "nonmatchings");
  if (!existsSync(asmRoot)) {
    return { ok: true, skipped: `${container.paths.asmDir}/nonmatchings is absent (gitignored); cannot verify` };
  }
  const dirs = readdirSync(asmRoot).sort();
  let probe: { fn: string; sFile: string } | undefined;
  for (const dir of dirs) {
    const candidate = join(asmRoot, dir, `${dir}.s`);
    if (existsSync(candidate)) {
      probe = { fn: dir, sFile: candidate };
      break;
    }
  }
  if (!probe) return { ok: true, skipped: "no .s file available to verify against" };

  const args = [
    "tools/vendor/m2c/m2c.py",
    "--target", "mipsel-gcc-c",
    /* Parse fresh rather than trusting the on-disk .m2c cache: the point of
     * the gate is to check what was just written. */
    "--no-cache",
    "-f", probe.fn,
  ];
  const globalsCtx = join(rootDir, "build/m2c_globals.ctx");
  if (existsSync(globalsCtx)) args.push("--context", "build/m2c_globals.ctx");
  for (const context of contextFilesFor(container)) args.push("--context", context);
  args.push(probe.sFile);

  let output: string;
  try {
    output = execFileSync("python3", args, { cwd: rootDir, encoding: "utf-8", stdio: ["ignore", "pipe", "pipe"] });
  } catch (e: any) {
    output = `${e.stdout ?? ""}${e.stderr ?? ""}`;
  }

  /* Only a context parse failure is this gate's business. m2c can fail on a
   * given function for unrelated reasons without the context being at fault. */
  if (output.includes("Syntax error when parsing C context")) {
    return { ok: false, diagnostic: output.trim() };
  }
  return { ok: true };
}

/**
 * Write the m2c context pair, then prove it parses before letting it stand.
 *
 * The context is shared by every function, so an unparseable one disables the
 * tool project-wide. On failure both files are restored and the error is
 * raised: this must never return having left a context its consumer rejects.
 */
export function writeContext(
  rootDir: string,
  signatures: Map<string, string>,
  container: Container = requireContainer(EXE_CONTAINER_ID),
  typeSignatures: Map<string, string> = signatures,
  catalog = scopedTypeCatalog(rootDir),
): void {
  for (const name of catalog.excludedFunctions) signatures.delete(name);
  projectScopedSignatures(signatures, catalog);
  const sdkPath = join(rootDir, SDK_TYPES_HEADER);
  const funcsPath = join(rootDir, functionsHeaderFor(container));

  /* Types are resolved over every container's signatures, not just this one's:
     one shared type header serves them all, so writing it from a subset would
     drop whatever the other containers name. */
  const { resolution, defs, referencedBy } = resolveContextTypes(rootDir, typeSignatures, catalog);

  for (const name of resolution.unresolved) {
    const users = referencedBy.get(name) ?? [];
    console.warn(
      `warning: type '${name}' is referenced by a signature but defined nowhere. ` +
      `Publication refused: a missing type cannot be given a fabricated layout.`,
    );
    for (const sig of users.slice(0, 3)) console.warn(`         referenced by: ${sig}`);
    if (users.length > 3) console.warn(`         ...and ${users.length - 3} more`);
  }

  if (resolution.unresolved.length) {
    throw new Error(`Cannot publish context: unresolved types ${resolution.unresolved.join(", ")}`);
  }

  const previous = new Map<string, string | null>([
    [sdkPath, existsSync(sdkPath) ? readFileSync(sdkPath, "utf-8") : null],
    [funcsPath, existsSync(funcsPath) ? readFileSync(funcsPath, "utf-8") : null],
  ]);

  mkdirSync(dirname(funcsPath), { recursive: true });
  writeIfChanged(sdkPath, renderSdkTypesHeader(resolution, defs));
  writeIfChanged(funcsPath, renderFunctionsHeader(signatures));

  const check = verifyContextParses(rootDir, container);
  if (check.skipped) {
    console.warn(`warning: context self-check skipped — ${check.skipped}`);
    return;
  }
  if (!check.ok) {
    for (const [path, content] of previous) {
      if (content === null) rmSync(path, { force: true });
      else writeIfChanged(path, content);
    }
    throw new Error(
      `Generated m2c context does not parse; previous context restored.\n\n${check.diagnostic}`,
    );
  }
}

/**
 * Export context for a single function. Returns extracted signatures.
 * This is the entry point used by the orchestrator.
 */
export function exportContext(
  funcName: string,
  rootDir: string = ROOT,
  container: Container = requireContainer(EXE_CONTAINER_ID),
): ExportResult {
  const cFile = join(rootDir, functionSourcePath(container, funcName));

  if (!existsSync(cFile)) {
    return { signatures: [], skipped: true, status: "skipped", signatureMatches: false, localPrototypes: [], reason: "file not found" };
  }

  const pairs = extractSignaturePairs(cFile, contextSourceReader(rootDir));
  if (pairs.length === 0) {
    return { signatures: [], skipped: true, status: "skipped", signatureMatches: false, localPrototypes: [], reason: "no function definitions (stub?)" };
  }
  const sigs = pairs.map((p) => p.signature);

  const includes = generatedContextIncludes(rootDir);
  if (includes.length) throw new Error(`Generated context is included by compiler sources:\n${includes.join("\n")}`);
  const localPrototypes = localPrototypeDisagreements(rootDir, pairs);
  for (const p of localPrototypes) console.warn(`warning: ${p.file} declares ${p.signature}; publishing ${p.published}. Audit this local prototype with callee truth.`);

  const existing = readExistingHeader(join(rootDir, functionsHeaderFor(container)));
  for (const { name, signature } of pairs) existing.set(name, signature);

  writeContext(rootDir, existing, container, unionSignatures(rootDir, container, existing));
  return { signatures: sigs, skipped: false, status: "published", signatureMatches: true, localPrototypes };
}

/**
 * Every container's published signatures, with this container's in-progress set
 * substituted in. The shared type header is resolved over this union so it
 * never loses a type another container names.
 */
function unionSignatures(
  rootDir: string,
  container: Container,
  current: Map<string, string>,
): Map<string, string> {
  const union = new Map<string, string>(current);
  for (const other of loadContainers()) {
    if (other.id === container.id) continue;
    for (const [name, signature] of readExistingHeader(join(rootDir, functionsHeaderFor(other)))) {
      if (!union.has(name)) union.set(name, signature);
    }
  }
  return union;
}

/**
 * Collect the signatures published by every configured translation unit.
 *
 * Read through the units, not a directory listing: a module defines several
 * functions under `modules/`, which a listing of the container's source root
 * would never see, and each of its members is published from the one file. A
 * member its unit's source does not define (a stub) is withdrawn.
 */
function collectAllSignatures(
  rootDir: string,
  container: Container = requireContainer(EXE_CONTAINER_ID),
): { signatures: Map<string, string>; exported: string[]; skipped: string[] } {
  const srcDir = join(rootDir, container.paths.srcDir);
  if (!existsSync(srcDir)) return { signatures: new Map(), exported: [], skipped: [] };
  const read = contextSourceReader(rootDir);
  const signatures = readExistingHeader(join(rootDir, functionsHeaderFor(container)));
  const exported: string[] = [];
  const skipped: string[] = [];

  for (const unit of loadTranslationUnits(container)) {
    const path = join(rootDir, unit.sourcePath);
    if (!existsSync(path)) continue;
    const pairs = extractSignaturePairs(path, read);
    const defined = new Set(pairs.map((pair) => pair.name));
    for (const member of unit.members) {
      if (defined.has(member.name)) continue;
      signatures.delete(member.name);
      skipped.push(member.name);
    }
    for (const { name, signature } of pairs) signatures.set(name, signature);
    exported.push(...unit.members.filter((member) => defined.has(member.name)).map((member) => member.name));
  }

  return { signatures, exported, skipped };
}

/**
 * Export context for all decompiled (non-stub) C files in src/.
 */
export function exportAll(
  rootDir: string = ROOT,
  containers: Container[] = loadContainers(),
): { exported: string[]; skipped: string[] } {
  const includes = generatedContextIncludes(rootDir);
  if (includes.length) throw new Error(`Generated context is included by compiler sources:\n${includes.join("\n")}`);
  const perContainer = containers.map((container) => ({
    container,
    ...collectAllSignatures(rootDir, container),
  }));

  /* One resolution over every container's signatures, so the shared type header
     covers all of them whichever container is written last. */
  const union = new Map<string, string>();
  for (const entry of perContainer) {
    for (const [name, signature] of unionSignatures(rootDir, entry.container, entry.signatures)) if (!union.has(name)) union.set(name, signature);
  }

  /* Fresh definitions override published headers for every selected container,
     regardless of which header was encountered first in the union. */
  for (const entry of perContainer) for (const [name, signature] of entry.signatures) union.set(name, signature);
  const exported: string[] = [];
  const skipped: string[] = [];
  const catalog = scopedTypeCatalog(rootDir);
  for (const entry of perContainer) {
    exported.push(...entry.exported);
    skipped.push(...entry.skipped);
    /* An overlay with nothing matched yet still gets its header, empty, so the
       m2c context file list is the same shape for every container. */
    if (entry.exported.length > 0 || entry.container.kind === "overlay") {
      writeContext(rootDir, entry.signatures, entry.container, union, catalog);
    }
  }

  return { exported, skipped };
}

// --- CLI ---

export interface ContextExportArgs {
  dryRun: boolean;
  all: boolean;
  containerId?: string;
  funcName?: string;
}

/**
 * Parse the CLI arguments.
 *
 * Extracted so the one thing that has silently broken here is testable: the
 * slot `--container` consumes. `indexOf` answers -1 when the flag is absent,
 * and treating -1 + 1 as a consumed index excluded argument 0 — the function
 * name — so every single-function invocation printed the usage instead.
 */
export function parseContextExportArgs(args: string[]): ContextExportArgs {
  const containerIdx = args.indexOf("--container");
  const consumed = containerIdx >= 0 ? containerIdx + 1 : -1;
  const containerId = containerIdx >= 0 ? args[containerIdx + 1] : undefined;
  const funcName = args.find((a, i) => !a.startsWith("--") && i !== consumed);
  return {
    dryRun: args.includes("--dry-run"),
    all: args.includes("--all"),
    ...(containerId ? { containerId } : {}),
    ...(funcName ? { funcName } : {}),
  };
}

if (process.argv[1]?.endsWith("contextExport.ts")) {
  const { dryRun, all, containerId, funcName } = parseContextExportArgs(process.argv.slice(2));

  if (!all && !funcName) {
    console.error("Usage: npx tsx tools/agent/contextExport.ts <func_name> [--dry-run]");
    console.error("       npx tsx tools/agent/contextExport.ts --all [--dry-run]");
    process.exit(1);
  }

  try {
    if (all) {
      if (dryRun) {
        const { signatures } = collectAllSignatures(ROOT, requireContainer(containerId ?? EXE_CONTAINER_ID));
        const { resolution, defs } = resolveContextTypes(ROOT, signatures);
        console.log(renderSdkTypesHeader(resolution, defs));
        console.log(renderFunctionsHeader(signatures));
        console.log(
          `\n${signatures.size} signature(s), ${resolution.ordered.length} type(s) resolved, ` +
          `${resolution.unresolved.length} unresolved (dry run, nothing written)`,
        );
      } else {
        const result = exportAll(ROOT, containerId ? [requireContainer(containerId)] : loadContainers());
        console.log(`Exported ${result.exported.length} function(s), skipped ${result.skipped.length} stub(s)`);
        if (result.exported.length > 0) {
          console.log(`Updated ${SDK_TYPES_HEADER} and each container's function header`);
        }
      }
    } else {
      const location = requireFunctionLocation(funcName!, containerId);
      const cFile = join(ROOT, functionSourcePath(location.container, funcName!));
      const sigs = extractSignatures(cFile);

      for (const sig of sigs) {
        console.log(`  ${sig}`);
      }

      if (dryRun) {
        console.log(`(dry run, nothing written)`);
      } else {
        const result = exportContext(funcName!, ROOT, location.container);
        console.log(CONTEXT_EXPORT_STATUS + JSON.stringify(result));
        if (result.skipped && result.reason !== "no function definitions (stub?)") process.exitCode = 1;
        if (!result.skipped) {
          console.log(`Updated ${SDK_TYPES_HEADER} and ${functionsHeaderFor(location.container)}`);
        } else if (result.reason) {
          console.log(`Skipped: ${result.reason}`);
        }
      }
    }
  } catch (e: any) {
    console.error(e.message);
    process.exit(1);
  }
}
