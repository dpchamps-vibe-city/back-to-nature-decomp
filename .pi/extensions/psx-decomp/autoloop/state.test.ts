import assert from "node:assert/strict";
import { mkdtempSync, readFileSync, rmSync, writeFileSync } from "node:fs";
import { tmpdir } from "node:os";
import { join } from "node:path";
import test from "node:test";
import { DEFAULT_CONFIG } from "../../shared/config.ts";
import { DEFAULT_LOOP_CONFIG } from "./config.ts";
import { emptyState, readState, statePath, withLoopExemptions, writeState } from "./state.ts";
import type { Completion } from "../tools/prepared-attempt.ts";
import { KEEP_GOING, escalationMessage, nudgeMessage, openingMessage } from "./prompts.ts";

test("state storage losslessly delta-encodes inputs, including removals, empty maps and replaced records", (t) => {
  const runtimeDir = mkdtempSync(join(tmpdir(), "autoloop-state-"));
  t.after(() => rmSync(runtimeDir, { recursive: true, force: true }));
  const config = { ...DEFAULT_LOOP_CONFIG, runtimeDir };
  const completion = (inputs: Record<string, string>): Completion => ({
    origin: "agent", tier: "matching-tier", documentationModel: "docs-tier",
    verifiedIdentity: "identity", verification: "passed", inputs,
    changedFiles: ["src/a.c"], documentation: "pending", error: "resume me",
  });
  const state = { ...emptyState(), completions: {
    first: completion({ "src/a.c": "a", "src/deleted.c": "d" }),
    second: completion({ "src/a.c": "a", "src/new.c": "n" }),
    third: completion({ "src/a.c": "changed", "src/new.c": "n" }),
    empty: completion({}),
    restored: completion({ "src/a.c": "a" }),
  } };
  state.approvals.first = { functionName: "first", kinds: ["embedded-asm"], approvedAt: "today", approvedBy: "reviewer", rationale: "evidence" };
  writeState(config, state);
  const stored = JSON.parse(readFileSync(statePath(config), "utf8"));
  assert.equal(stored.format, "autoloop-input-deltas-v1");
  assert.deepEqual(stored.completions.second.inputsDelta, { "src/deleted.c": null, "src/new.c": "n" });
  assert.deepEqual(stored.completions.third.inputsDelta, { "src/a.c": "changed" });
  const restored = readState(config);
  assert.deepEqual(restored, state);
  restored.completions!.second!.inputs["src/a.c"] = "mutation";
  assert.equal(restored.completions!.first!.inputs["src/a.c"], "a", "decoded snapshots must not alias");
  assert.equal(state.completions.second.inputs["src/a.c"], "a", "writing must not mutate the caller");

  /* Updating/removing an earlier completion must rebase the following deltas. */
  state.completions.first = completion({ "src/replacement.c": "r" });
  delete (state.completions as Record<string, Completion>).third;
  writeState(config, state);
  assert.deepEqual(readState(config), state);
});

test("legacy states remain readable and migrate on write without dropping parked work", (t) => {
  const runtimeDir = mkdtempSync(join(tmpdir(), "autoloop-state-legacy-"));
  t.after(() => rmSync(runtimeDir, { recursive: true, force: true }));
  const config = { ...DEFAULT_LOOP_CONFIG, runtimeDir };
  assert.deepEqual(readState(config), emptyState());
  const state = emptyState();
  state.parked.first = { functionName: "first", reason: "blocked", parkedAt: "today", reachedTier: "tier", lastReport: "full report", findings: [] };
  writeFileSync(statePath(config), JSON.stringify(state));
  assert.deepEqual(readState(config), state);
  writeState(config, readState(config));
  assert.deepEqual(readState(config), state);
  state.completions = {};
  writeState(config, state);
  assert.deepEqual(readState(config), state, "absent and empty completion maps both survive");
});

test("invalid or unsupported state fails closed instead of silently discarding all history", (t) => {
  const runtimeDir = mkdtempSync(join(tmpdir(), "autoloop-state-invalid-"));
  t.after(() => rmSync(runtimeDir, { recursive: true, force: true }));
  const config = { ...DEFAULT_LOOP_CONFIG, runtimeDir };
  for (const text of ["{", '{"format":"future-version"}', '{"format":"autoloop-input-deltas-v1","completions":{"first":{}}}',
    '{"format":"autoloop-input-deltas-v1","completions":{"first":{"inputsDelta":{"a":42}}}}']) {
    writeFileSync(statePath(config), text);
    assert.throws(() => readState(config));
    assert.equal(readFileSync(statePath(config), "utf8"), text);
  }
});

test("repeated large snapshots are stored once rather than growing with every completion", (t) => {
  const runtimeDir = mkdtempSync(join(tmpdir(), "autoloop-state-size-"));
  t.after(() => rmSync(runtimeDir, { recursive: true, force: true }));
  const config = { ...DEFAULT_LOOP_CONFIG, runtimeDir };
  const inputs = Object.fromEntries(Array.from({ length: 2000 }, (_, i) => [`src/function_${i}.c`, "a".repeat(64)]));
  const state = { ...emptyState(), completions: Object.fromEntries(Array.from({ length: 100 }, (_, i) => [`function_${i}`, {
    origin: "static" as const, inputs, verifiedIdentity: "identity", changedFiles: [], documentation: "passed" as const,
  }])) };
  writeState(config, state);
  assert.ok(readFileSync(statePath(config)).length < JSON.stringify(state).length / 50);
  assert.deepEqual(readState(config), state);
});

test("a parked function is exempted from the include-asm rule, and nothing else is", () => {
  const state = emptyState();
  state.parked.func_80012345 = {
    functionName: "func_80012345",
    reason: "escalation-exhausted",
    parkedAt: "2026-08-09T00:00:00.000Z",
    reachedTier: "gpt-5.6-sol",
    lastReport: "",
    findings: [],
  };
  const config = withLoopExemptions(DEFAULT_CONFIG, state);
  assert.deepEqual(config.sourcePolicy.allowlist.func_80012345, ["include-asm"]);
  assert.equal(config.sourcePolicy.allowlist.func_80099999, undefined);
});

test("an agent-approved exemption is honoured by the loop and merged with existing entries", () => {
  const state = emptyState();
  state.approvals.func_80012345 = {
    functionName: "func_80012345",
    kinds: ["embedded-asm"],
    approvedAt: "2026-08-09T00:00:00.000Z",
    approvedBy: "gpt-5.6-sol",
    rationale: "documented exception class",
  };
  const base = {
    ...DEFAULT_CONFIG,
    sourcePolicy: { ...DEFAULT_CONFIG.sourcePolicy, allowlist: { func_80012345: ["register-asm"] } },
  };
  const config = withLoopExemptions(base, state);
  assert.deepEqual(config.sourcePolicy.allowlist.func_80012345.sort(), ["embedded-asm", "register-asm"]);
});

test("loop exemptions never mutate the config they were derived from", () => {
  const state = emptyState();
  state.parked.func_80012345 = {
    functionName: "func_80012345",
    reason: "escalation-exhausted",
    parkedAt: "2026-08-09T00:00:00.000Z",
    reachedTier: "kimi-k3",
    lastReport: "",
    findings: [],
  };
  withLoopExemptions(DEFAULT_CONFIG, state);
  assert.deepEqual(DEFAULT_CONFIG.sourcePolicy.allowlist, {});
});

test("the first tier opens a fresh decompilation and every later tier resumes the attempt", () => {
  assert.match(openingMessage("func_80012345"), /Mode: fresh decompilation/);
  assert.match(escalationMessage("func_80012345", "kimi-k3", "report"), /Mode: resume\/fix/);
  assert.match(escalationMessage("func_80012345", "kimi-k3", "report"), /you are now kimi-k3/);
});

test("every non-matching return carries the same nudge", () => {
  assert.ok(nudgeMessage("report").startsWith(KEEP_GOING));
  assert.ok(escalationMessage("func_80012345", "kimi-k3", "report").includes(KEEP_GOING));
  /* The nudge states the four-step protocol, not just encouragement: a turn told
   * only to keep going spends its context reasoning forward from the last
   * report instead of producing a measurement. */
  assert.match(KEEP_GOING, /there is clean C that matches this function 100%/);
  assert.match(KEEP_GOING, /One experiment is one edit and one measurement/);
  /* "Turn" is the harness's word for one assistant response. Using it for a
   * loop iteration told an agent it owed exactly one measurement per response,
   * and it stopped after one. The pacing words must never say "turn". */
  assert.doesNotMatch(KEEP_GOING, /\bturn\b/i);
  for (const step of ["OBSERVE", "HYPOTHESISE", "ACT", "MEASURE"]) {
    assert.match(KEEP_GOING, new RegExp(step), `the nudge must name the ${step} step`);
  }
});

test("the opening message carries the protocol, because the nudge may never fire", () => {
  /* `nudgeMessage` is sent from the second return onward. With returnsPerTier
   * at 1 it never fires, so the opening message is the only thing an agent
   * reads — and a protocol that lives only in the nudge is read by nothing. */
  const opening = openingMessage("func_80012345");
  assert.match(opening, /Do not stop until the function is byte-exact/);
  assert.match(opening, /back to back/);
  assert.match(opening, /psx_residual_objective/);
  assert.doesNotMatch(opening, /This turn runs/);
});
