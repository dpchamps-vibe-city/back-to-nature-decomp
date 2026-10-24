import { existsSync, mkdirSync, readFileSync, renameSync, writeFileSync } from "node:fs";
import { dirname, join } from "node:path";
import type { AutodecompConfig } from "../../shared/types.ts";
import type { Completion } from "../tools/prepared-attempt.ts";
import type { ApprovalRecord, LoopConfig, LoopState, ParkRecord, RepairRecord } from "./types.ts";

const STATE_FORMAT = "autoloop-input-deltas-v1";
type StoredCompletion = Omit<Completion, "inputs"> & { inputsDelta: Record<string, string | null> };
type StoredState = Omit<LoopState, "completions"> & {
  format: typeof STATE_FORMAT;
  completions?: Record<string, StoredCompletion>;
};

/** Each completion keeps its full independent snapshot in memory. On disk,
 * store only changes from the previous record (null removes a path). Repeating
 * the whole project map per match reached V8's 512 MiB string ceiling. */
function encodeState(state: LoopState): StoredState {
  const stored: StoredState = { format: STATE_FORMAT, parked: state.parked, approvals: state.approvals,
    ...(state.repairs ? { repairs: state.repairs } : {}) };
  if (state.completions) {
    stored.completions = {};
    let previous: Record<string, string> = {};
    for (const [name, { inputs, ...completion }] of Object.entries(state.completions)) {
      const inputsDelta: Record<string, string | null> = Object.create(null);
      for (const path of Object.keys(previous)) {
        if (!Object.hasOwn(inputs, path)) inputsDelta[path] = null;
      }
      for (const [path, hash] of Object.entries(inputs)) {
        if (!Object.hasOwn(previous, path) || previous[path] !== hash) inputsDelta[path] = hash;
      }
      stored.completions[name] = { ...completion, inputsDelta };
      previous = inputs;
    }
  }
  return stored;
}

function decodeState(stored: StoredState): LoopState {
  const state: LoopState = { parked: stored.parked ?? {}, approvals: stored.approvals ?? {},
    ...(stored.repairs ? { repairs: stored.repairs } : {}) };
  if (stored.completions) {
    state.completions = {};
    let previous: Record<string, string> = {};
    for (const [name, { inputsDelta, ...completion }] of Object.entries(stored.completions)) {
      if (!inputsDelta || typeof inputsDelta !== "object" || Array.isArray(inputsDelta)) {
        throw new Error(`Invalid autoloop input delta for ${name}`);
      }
      const inputs = { ...previous };
      for (const [path, hash] of Object.entries(inputsDelta)) {
        if (hash === null) delete inputs[path];
        else if (typeof hash === "string") Object.defineProperty(inputs, path, {
          value: hash, writable: true, enumerable: true, configurable: true,
        });
        else throw new Error(`Invalid autoloop input hash for ${name}: ${path}`);
      }
      state.completions[name] = { ...completion, inputs };
      previous = inputs;
    }
  }
  return state;
}

export function statePath(config: LoopConfig): string {
  return join(config.runtimeDir, "state.json");
}

export function emptyState(): LoopState {
  return { parked: {}, approvals: {} };
}

export function readState(config: LoopConfig): LoopState {
  const path = statePath(config);
  if (!existsSync(path)) return emptyState();
  /* Never turn a read/parse/format failure into empty history: the next write
   * would silently discard every parked attempt and completion. */
  const raw = JSON.parse(readFileSync(path, "utf8")) as Partial<LoopState> & { format?: unknown };
  if (raw.format === STATE_FORMAT) return decodeState(raw as unknown as StoredState);
  if (raw.format !== undefined) throw new Error(`Unsupported autoloop state format: ${String(raw.format)}`);
  /* Legacy state migrates losslessly on its next write. */
  return { parked: raw.parked ?? {}, approvals: raw.approvals ?? {}, ...(raw.completions ? { completions: raw.completions } : {}),
    ...(raw.repairs ? { repairs: raw.repairs } : {}) };
}

export function recordRepair(config: LoopConfig, state: LoopState, record: RepairRecord): LoopState {
  const next: LoopState = { ...state, repairs: { ...state.repairs, [record.functionName]: record } };
  writeState(config, next);
  return next;
}

export function writeState(config: LoopConfig, state: LoopState): void {
  const path = statePath(config);
  mkdirSync(dirname(path), { recursive: true });
  const temp = `${path}.tmp`;
  writeFileSync(temp, `${JSON.stringify(encodeState(state), null, 2)}\n`);
  renameSync(temp, path);
}

export function recordPark(config: LoopConfig, state: LoopState, record: ParkRecord): LoopState {
  const next: LoopState = { ...state, parked: { ...state.parked, [record.functionName]: record } };
  writeState(config, next);
  return next;
}

export function recordApproval(config: LoopConfig, state: LoopState, record: ApprovalRecord): LoopState {
  const next: LoopState = { ...state, approvals: { ...state.approvals, [record.functionName]: record } };
  writeState(config, next);
  return next;
}

/**
 * Project the loop's own decisions onto the shared source-policy allowlist.
 *
 * A parked function is `INCLUDE_ASM` on purpose, and an agent-approved
 * exemption has been adjudicated one rung up the ladder — both must stop the
 * gate from failing every *later* function whose patch happens to contain
 * those lines. Neither is written into `.pi/autoloop.json`: that allowlist is
 * the human's permanent assertion about a function, and the loop only ever
 * files a request for one (see the approvals directory).
 */
export function withLoopExemptions(config: AutodecompConfig, state: LoopState): AutodecompConfig {
  const allowlist: Record<string, string[]> = { ...config.sourcePolicy.allowlist };
  const add = (name: string, kinds: string[]) => {
    const key = name.toLowerCase();
    allowlist[key] = [...new Set([...(allowlist[key] ?? []), ...kinds])];
  };
  for (const name of Object.keys(state.parked)) add(name, ["include-asm"]);
  for (const [name, approval] of Object.entries(state.approvals)) add(name, approval.kinds);
  return { ...config, sourcePolicy: { ...config.sourcePolicy, allowlist } };
}
