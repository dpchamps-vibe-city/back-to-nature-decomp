/**
 * tuExceptions.ts — the normalized set of source-policy exceptions a tree
 * carries, so a module-folding campaign can prove it never grows.
 *
 * Source policy permits exceptions that are already authorized, so passing it
 * says nothing about whether an iteration *added* one. This compares a tree
 * against the snapshot a campaign started from, category by category:
 *
 *  - allowlist entries (`key:kind` in `.pi/autoloop.json`);
 *  - compiler-flag overrides (`configs/flag_overrides.mk`);
 *  - register pins and embedded asm, charged by the pinned C AST to the
 *    original function whose body holds them — so moving a function into a
 *    module moves its exceptions without changing their identity;
 *  - `INCLUDE_ASM` stubs, by the symbol they hand to the assembler;
 *  - module-scope constructs, which belong to no function and would broaden
 *    one function's exception to a whole unit.
 *
 * Removing an exception is always allowed. The receipt lives under ignored
 * `build/` and holds hashes and these sets — never a work queue or a list of
 * modules.
 */

import { execFileSync } from "node:child_process";
import { createHash } from "node:crypto";
import { existsSync, mkdirSync, readFileSync, writeFileSync } from "node:fs";
import { dirname, join, posix } from "node:path";
import { ROOT } from "../lib/psxExeInfo.js";
import { loadContainers, type Container } from "../lib/container.js";
import { flagOverrideKey, isModuleStem, loadTranslationUnits } from "../lib/translationUnits.js";
import { enclosingDefinition, sourceFacts } from "../lib/translationUnitAudit.js";
import { loadConfig } from "../../.pi/extensions/shared/config.ts";
import { sourceConstructFindings } from "../../.pi/extensions/shared/source-policy.ts";
import type { AutodecompConfig } from "../../.pi/extensions/shared/types.ts";

export const RECEIPT_DIR = "build/tu-campaign";
export const DEFAULT_RECEIPT = `${RECEIPT_DIR}/baseline.json`;

export interface ExceptionSets {
  allowlist: string[];
  flagOverrides: string[];
  constructs: string[];
  includeAsm: string[];
  moduleScope: string[];
}

export interface CampaignReceipt {
  /** 2: exceptions keyed by function identity (`<container>:0xVRAM`), not name. */
  schemaVersion: 2;
  createdAt: string;
  head: string | null;
  /** Content hash of every configured source, splat config, hand header and policy input. */
  inputs: string;
  exceptions: ExceptionSets;
}

export interface ExceptionGrowth {
  category: keyof ExceptionSets;
  added: string[];
}

/** One tree's readable state: a file reader and the list of C sources it configures. */
export interface TreeView {
  read(path: string): string | null;
  /** Configured sources with the original functions each owns, per container. */
  units: Array<{ container: string; sourcePath: string; segmentName: string; members: string[] }>;
}

function workingTree(): TreeView {
  return {
    read: (path) => existsSync(join(ROOT, path)) ? readFileSync(join(ROOT, path), "utf8") : null,
    units: loadContainers().flatMap((container) => loadTranslationUnits(container).map((unit) => ({
      container: container.id, sourcePath: unit.sourcePath, segmentName: unit.segmentName,
      members: unit.members.map((member) => member.name),
    }))),
  };
}

/**
 * Another tree, read through git without checking it out.
 *
 * Its units are approximated from its configured `c` lines: a per-function
 * stem owns the function it is named for, and a module owns whatever its AST
 * defines. That is exactly the identity the exception sets are keyed by.
 */
function gitTree(treeish: string): TreeView {
  const git = (args: string[], input?: string) => execFileSync("git", args, { cwd: ROOT, maxBuffer: 1024 * 1024 * 1024, ...(input !== undefined ? { input } : {}) });
  /* Every blob the comparison can read, in one `cat-file --batch` pass. */
  const blobs = new Map<string, string>();
  for (const line of git(["ls-tree", "-r", treeish, "--", "src", "configs", ".pi/autoloop.json"]).toString("utf8").split("\n")) {
    const match = line.match(/^\d+ blob ([0-9a-f]+)\t(.+)$/);
    if (match && (/\.(c|yaml|mk|json|txt)$/.test(match[2]!))) blobs.set(match[2]!, match[1]!);
  }
  const paths = [...blobs.keys()];
  const contents = new Map<string, string>();
  const batch = git(["cat-file", "--batch"], paths.map((path) => blobs.get(path)).join("\n") + "\n");
  let offset = 0;
  for (const path of paths) {
    const header = batch.indexOf(0x0a, offset);
    const size = Number(batch.toString("utf8", offset, header).split(" ")[2]);
    contents.set(path, batch.toString("utf8", header + 1, header + 1 + size));
    offset = header + 1 + size + 1;
  }
  const read = (path: string): string | null => contents.get(path) ?? null;
  const units: TreeView["units"] = [];
  for (const container of loadContainers()) {
    const config = read(container.paths.splat);
    if (!config) continue;
    for (const match of config.matchAll(/^\s*- \[0x[0-9A-Fa-f]+, c, ([^\]\s]+)\]/gm)) {
      const stem = match[1]!;
      const sourcePath = posix.join(container.paths.srcDir, `${stem}.c`);
      units.push({ container: container.id, sourcePath, segmentName: stem, members: isModuleStem(stem) ? [] : [stem] });
    }
  }
  return { read, units };
}

/**
 * Function identity as `<container>:0xVRAM`, from each container's symbol
 * table in the same tree.
 *
 * Exceptions are keyed by what a function *is*, not what it is called: a
 * module reconstruction renames functions, and a renamed function's
 * allowlist entry, flag override or pin is the same exception, not a new one.
 * A name no table resolves (a module key, a typo) keeps its spelling.
 */
interface Identities {
  /** Exact name -> identity, for names one container defines. */
  byName: Map<string, string>;
  /** Lowercased name -> identity, as the allowlist keys names. */
  byLowerName: Map<string, string>;
  /** Per container: exact name -> identity. */
  inContainer: Map<string, Map<string, string>>;
}

function identity(container: string, address: number): string {
  return `${container}:0x${address.toString(16).toUpperCase().padStart(8, "0")}`;
}

function identities(view: TreeView): Identities {
  const byName = new Map<string, string>(), byLowerName = new Map<string, string>(), inContainer = new Map<string, Map<string, string>>();
  const ambiguous = new Set<string>();
  for (const container of loadContainers()) {
    const own = new Map<string, string>();
    for (const line of (view.read(container.paths.symbolAddrs) ?? "").split("\n")) {
      const match = line.match(/^\s*([A-Za-z_][A-Za-z0-9_.]*)\s*=\s*(0x[0-9A-Fa-f]+)\s*;.*\btype:func\b/);
      if (!match) continue;
      const id = identity(container.id, Number(match[2]));
      own.set(match[1]!, id);
      for (const [map, key] of [[byName, match[1]!], [byLowerName, match[1]!.toLowerCase()]] as const) {
        if (map.has(key) && map.get(key) !== id) ambiguous.add(key);
        map.set(key, id);
      }
    }
    inContainer.set(container.id, own);
  }
  for (const key of ambiguous) { byName.delete(key); byLowerName.delete(key); }
  return { byName, byLowerName, inContainer };
}

/** `identity:kind` for every allowlist grant; name, bare and qualified address keys of one function coincide. */
function allowlistSet(text: string | null, ids: Identities): string[] {
  if (!text) return [];
  const allowlist = (JSON.parse(text) as { sourcePolicy?: { allowlist?: Record<string, string[]> } }).sourcePolicy?.allowlist ?? {};
  const normalize = (key: string): string => {
    const address = key.match(/^(?:([A-Za-z0-9_]+):)?(0x[0-9A-Fa-f]+)$/);
    if (address) return identity(address[1] ?? "exe", Number(address[2]));
    return ids.byLowerName.get(key.toLowerCase()) ?? key.toLowerCase();
  };
  return [...new Set(Object.entries(allowlist).flatMap(([key, kinds]) => kinds.map((kind) => `${normalize(key)}:${kind}`)))].sort();
}

function flagOverrideSet(text: string | null, ids: Identities, view: TreeView): string[] {
  if (!text) return [];
  const singletons = new Map<string, string>();
  for (const unit of view.units) {
    if (!isModuleStem(unit.segmentName)) continue;
    const source = view.read(unit.sourcePath);
    if (source === null) continue;
    const facts = sourceFacts(source);
    if (!facts.parses || facts.definitions.length !== 1 || facts.includeAsm.length !== 0) continue;
    const owner = ids.inContainer.get(unit.container)?.get(facts.definitions[0]!.name);
    if (owner) singletons.set(flagOverrideKey(unit.sourcePath), owner);
  }
  return text.split("\n").flatMap((line) => {
    const match = line.match(/^CC1FLAGS_(\S+?)\s*:?=\s*(.*)$/);
    return match ? [`${ids.byName.get(match[1]!) ?? singletons.get(match[1]!) ?? match[1]}=${match[2]!.trim().split(/\s+/).join(" ")}`] : [];
  }).sort();
}

/**
 * The exception sets of one tree.
 *
 * Constructs are found with an empty allowlist, so every non-permitted pin or
 * asm statement is seen whether or not it is authorized, and charged to the
 * original function whose body holds it.
 */
export function exceptionSets(view: TreeView, config: AutodecompConfig): ExceptionSets {
  const unrestricted: AutodecompConfig = { ...config, sourcePolicy: { ...config.sourcePolicy, allowlist: {} } };
  const ids = identities(view);
  const of = (container: string, name: string) => ids.inContainer.get(container)?.get(name) ?? `${container}:${name}`;
  const constructs = new Set<string>();
  const includeAsm = new Set<string>();
  const moduleScope = new Set<string>();
  for (const unit of view.units) {
    const source = view.read(unit.sourcePath);
    if (source === null) continue;
    const module = isModuleStem(unit.segmentName);
    const facts = sourceFacts(source);
    for (const finding of sourceConstructFindings(source, unrestricted, {})) {
      if (finding.kind === "include-asm") {
        if (finding.text) includeAsm.add(of(unit.container, finding.text));
        continue;
      }
      if (finding.kind !== "register-asm" && finding.kind !== "embedded-asm") continue;
      /* The same rule for both layouts, so a fold changes no identity: the
         innermost definition holding the construct; outside every definition,
         a per-function file's own function, or a module's module scope. */
      const enclosing = finding.line !== undefined ? enclosingDefinition(facts.definitions, finding.line)?.name : undefined;
      const owner = enclosing ?? (module ? undefined : unit.members[0] ?? unit.segmentName);
      if (owner) constructs.add(`${of(unit.container, owner)}:${finding.kind}`);
      else moduleScope.add(`${unit.container}:${unit.sourcePath}:${finding.kind}`);
    }
  }
  return {
    allowlist: allowlistSet(view.read(".pi/autoloop.json"), ids),
    flagOverrides: flagOverrideSet(view.read("configs/flag_overrides.mk"), ids, view),
    constructs: [...constructs].sort(),
    includeAsm: [...includeAsm].sort(),
    moduleScope: [...moduleScope].sort(),
  };
}

export function currentExceptionSets(): ExceptionSets {
  return exceptionSets(workingTree(), loadConfig(ROOT));
}

/** What `after` holds that `before` does not, per category. Removals never count. */
export function exceptionGrowth(before: ExceptionSets, after: ExceptionSets): ExceptionGrowth[] {
  const growth: ExceptionGrowth[] = [];
  for (const category of Object.keys(after) as Array<keyof ExceptionSets>) {
    const known = new Set(before[category] ?? []);
    const added = after[category].filter((entry) => !known.has(entry));
    if (added.length > 0) growth.push({ category, added });
  }
  /* An allowlist key that names no function — a module stem or a path — is a
     function exception broadened to a unit, whatever the baseline held. */
  const broadened = after.allowlist.filter((entry) => /\/|^modules__|\bmodules\b/.test(entry.slice(0, entry.lastIndexOf(":"))));
  if (broadened.length > 0) growth.push({ category: "moduleScope", added: broadened.map((entry) => `allowlist ${entry}`) });
  return growth;
}

/** Hash of everything that decides source organization and policy. */
export function inputsDigest(): string {
  const hash = createHash("sha256");
  const paths = new Set<string>([".pi/autoloop.json", "configs/flag_overrides.mk", "include/globals_override.h", "include/game_types.h", "include/common.h"]);
  for (const container of loadContainers()) {
    paths.add(container.paths.splat);
    paths.add(container.paths.symbolAddrs);
    for (const unit of loadTranslationUnits(container)) paths.add(unit.sourcePath);
  }
  for (const path of [...paths].sort()) {
    hash.update(path).update("\0");
    const absolute = join(ROOT, path);
    hash.update(existsSync(absolute) ? readFileSync(absolute) : Buffer.from("<absent>")).update("\0");
  }
  return hash.digest("hex");
}

function headCommit(): string | null {
  try { return execFileSync("git", ["rev-parse", "HEAD"], { cwd: ROOT, encoding: "utf8" }).trim(); } catch { return null; }
}

export function captureReceipt(path = DEFAULT_RECEIPT): CampaignReceipt {
  const receipt: CampaignReceipt = {
    schemaVersion: 2,
    createdAt: new Date().toISOString(),
    head: headCommit(),
    inputs: inputsDigest(),
    exceptions: currentExceptionSets(),
  };
  const absolute = join(ROOT, path);
  mkdirSync(dirname(absolute), { recursive: true });
  writeFileSync(absolute, JSON.stringify(receipt, null, 2) + "\n");
  return receipt;
}

/**
 * The baseline a comparison starts from: a receipt file, or any git tree-ish
 * read without a checkout.
 */
export function loadBaseline(reference: string): { label: string; exceptions: ExceptionSets } {
  const asFile = join(ROOT, reference);
  if (existsSync(asFile) || existsSync(reference)) {
    const text = readFileSync(existsSync(asFile) ? asFile : reference, "utf8");
    const receipt = JSON.parse(text) as CampaignReceipt;
    if ((receipt.schemaVersion as number) !== 2 || !receipt.exceptions) {
      throw new Error(`${reference} is not a schemaVersion 2 campaign receipt — recapture it with --write-baseline, or pass the commit it was taken on`);
    }
    return { label: `receipt ${reference} (${receipt.head?.slice(0, 12) ?? "no commit"}, ${receipt.createdAt})`, exceptions: receipt.exceptions };
  }
  try {
    execFileSync("git", ["rev-parse", "--verify", `${reference}^{tree}`], { cwd: ROOT, stdio: "ignore" });
  } catch {
    throw new Error(`--baseline ${reference}: neither a receipt file nor a git tree-ish`);
  }
  const config = loadConfig(ROOT);
  const view = gitTree(reference);
  /* The baseline's module members come from its own module sources' ASTs. */
  for (const unit of view.units) {
    if (unit.members.length > 0) continue;
    const source = view.read(unit.sourcePath);
    if (source) unit.members.push(...sourceFacts(source).definitions.filter((d) => !d.nested).map((d) => d.name));
  }
  return { label: `tree ${reference}`, exceptions: exceptionSets(view, config) };
}

export type { Container };
