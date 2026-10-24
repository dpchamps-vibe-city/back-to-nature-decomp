import type { ExtensionAPI } from "@earendil-works/pi-coding-agent";
import { getSessionBaseline } from "../tools/session-baseline.ts";
import { DEFAULT_CHECKPOINT_AT_TOKENS, loadLoopConfig } from "./config.ts";
import { registerCheckpointMonitor } from "./checkpoint.ts";
import { runLoop, summarize, type AbortFlag, type LoopSinks } from "./loop.ts";
import { prepSystemPrompt, registerPrepHandoffTool, setPrepHandoffToolActive } from "./prep.ts";
import { createHandoffSink, registerHandoffTool, setHandoffToolActive } from "./handoff.ts";
import { createVerdictSink, registerPolicyVerdictTool, setVerdictToolActive } from "./policy-verdict.ts";
import { readState } from "./state.ts";
import { documentedExceptions, repairCandidates, runRepairLoop } from "./repair.ts";
import { runFoldLoop } from "./fold.ts";
import { loadCallGraph } from "../../shared/call-graph.ts";
import { loadConfig } from "../../shared/config.ts";
import { createTurnGate, registerTurnGate } from "./turn-gate.ts";

export const LOOP_COMMAND = "auto_decompilation_loop";

interface ParsedArgs {
  action: "run" | "repair" | "fold" | "status" | "stop" | "usage";
  target?: string;
  maxFunctions?: number;
}

const USAGE = `Usage: /${LOOP_COMMAND} [function] [--max=N] | repair [function] [--max=N] | fold | status | stop`;

export function parseArgs(args: string): ParsedArgs {
  const tokens = args.trim().split(/\s+/).filter(Boolean);
  if (tokens.length === 0) return { action: "run" };
  if (tokens[0] === "status") return { action: "status" };
  if (tokens[0] === "stop") return { action: "stop" };
  /* `fold` is a mode word like `repair`; it takes no arguments — the queue is
     derived from tuProgress.ts on every iteration, never named or limited. */
  if (tokens[0] === "fold") return tokens.length === 1 ? { action: "fold" } : { action: "usage" };

  /* `repair` is a mode word, not a function name; no symbol is spelled that way. */
  const repair = tokens[0] === "repair";
  const parsed: ParsedArgs = { action: repair ? "repair" : "run" };
  for (const token of repair ? tokens.slice(1) : tokens) {
    const limit = token.match(/^--max(?:-functions)?=(\d+)$/);
    if (limit) {
      parsed.maxFunctions = Number.parseInt(limit[1], 10);
      continue;
    }
    if (/^[A-Za-z_][A-Za-z0-9_]*$/.test(token) && !parsed.target) {
      parsed.target = token;
      continue;
    }
    return { action: "usage" };
  }
  if (parsed.maxFunctions !== undefined && parsed.maxFunctions < 1) return { action: "usage" };
  return parsed;
}

/**
 * `/auto_decompilation_loop` — the in-TUI escalation loop.
 *
 * It runs in this session, on this tree: no worktrees, no detached supervisor,
 * no forked agents. The command handler is the loop body, `sendUserMessage` +
 * `waitForIdle` is the turn, and the two oracles are the only things allowed to
 * call a function finished.
 */
export function registerAutoloopCommands(pi: ExtensionAPI, projectRoot: string): () => boolean {
  const sink: LoopSinks = { verdict: createVerdictSink(), handoff: createHandoffSink(), prep: {}, gate: createTurnGate(), checkpoint: {} };
  registerPolicyVerdictTool(pi, sink.verdict);
  registerHandoffTool(pi, sink.handoff);
  registerPrepHandoffTool(pi, sink.prep);
  registerTurnGate(pi, sink.gate);
  registerCheckpointMonitor(pi, sink.checkpoint!);

  let active: AbortFlag | null = null;
  let running: Promise<unknown> | undefined;
  pi.on("session_shutdown", async (_event, ctx) => {
    if (!active) return;
    active.aborted = true;
    ctx.abort();
    await running;
  });

  /* These are turn-scoped instruments: they stay out of the ordinary active set
   * until the loop opens a policy review or a handoff. */
  pi.on("session_start", async () => {
    setVerdictToolActive(pi, false);
    setHandoffToolActive(pi, false);
    setPrepHandoffToolActive(pi, false);
  });

  /* Interactive input while the loop runs is the stop signal. The loop's own
   * messages carry source="extension", so it never interrupts itself. */
  pi.on("input", async (event) => {
    if (active && event.source === "interactive") active.aborted = true;
    return { action: "continue" };
  });

  pi.on("before_agent_start", (event) => {
    if (active && !active.aborted && sink.role === "prep") {
      return { systemPrompt: prepSystemPrompt(event.systemPromptOptions) };
    }
  });

  pi.registerCommand(LOOP_COMMAND, {
    description:
      "Run the tiered escalation decompilation loop in this session; `repair` removes asm/pin exceptions from functions that already match; " +
      "`fold` reconstructs source modules (plans/tu-prompt.md) until tuProgress.ts reports every eligible function folded. " + USAGE,
    handler: async (args, ctx) => {
      const parsed = parseArgs(args);

      if (parsed.action === "usage") {
        ctx.ui.notify(USAGE, "warning");
        return;
      }

      if (parsed.action === "stop") {
        if (!active) {
          ctx.ui.notify("No escalation loop is running.", "info");
          return;
        }
        active.aborted = true;
        ctx.ui.notify("Escalation loop will stop at the next checkpoint.", "info");
        return;
      }

      let config: ReturnType<typeof loadLoopConfig>;
      try {
        config = loadLoopConfig(projectRoot);
      } catch (error) {
        ctx.ui.notify(error instanceof Error ? error.message : String(error), "error");
        return;
      }

      if (parsed.action === "status") {
        const state = readState(config);
        const parked = Object.keys(state.parked);
        const approvals = Object.keys(state.approvals);
        const repairs = Object.values(state.repairs ?? {});
        const folds = state.folds ?? [];
        let queued = "unknown (call graph unavailable)";
        try {
          const queue = repairCandidates(loadCallGraph(projectRoot).functions, loadConfig(projectRoot).sourcePolicy.allowlist, documentedExceptions(projectRoot));
          const pending = queue.filter((candidate) => !["failed", "repaired"].includes(state.repairs?.[candidate.name]?.status ?? ""));
          queued = `${pending.length} queued, ${queue.length - pending.length} after a failed repair`;
        } catch { /* status must not fail on a stale or missing graph */ }
        ctx.ui.notify(
          [
            `ladder: ${config.ladder.map((tier) => tier.label + (tier.role ? ` [${tier.role}]` : "")).join(" → ")}`,
            `checkpoints: ${config.ladder.map((tier) => `${tier.label}: ${tier.checkpointAtTokens ?? DEFAULT_CHECKPOINT_AT_TOKENS} tokens (0 = off)`).join("; ")}`,
            `returns per tier: ${config.returnsPerTier}`,
            `running: ${active ? "yes" : "no"}`,
            `parked (${parked.length}): ${parked.join(", ") || "none"}`,
            `agent-approved exemptions (${approvals.length}): ${approvals.join(", ") || "none"}`,
            `repair: ${queued}; repaired ${repairs.filter((record) => record.status === "repaired").length}`,
            `fold: ${folds.filter((record) => record.status === "accepted").length} module(s) accepted, ` +
              `${folds.filter((record) => record.status === "failed").length} failed, ` +
              `${folds.filter((record) => record.status === "interrupted").length} interrupted (settled by the next fold run)`,
          ].join("\n"),
          "info",
        );
        return;
      }

      if (active) {
        ctx.ui.notify("An escalation loop is already running. Send any message, or /auto_decompilation_loop stop.", "warning");
        return;
      }

      const flag: AbortFlag = { aborted: false };
      active = flag;
      ctx.ui.notify(
        `Escalation loop starting. Ladder: ${config.ladder.map((tier) => tier.label + (tier.role ? ` [${tier.role}]` : "")).join(" → ")}. Type any message to stop.`,
        "info",
      );

      const task = Promise.resolve().then(async () => {
        const deps = { pi, ctx, projectRoot, config, sink, flag, baseline: await getSessionBaseline(projectRoot) };
        if (parsed.action === "fold") return runFoldLoop(deps);
        return parsed.action === "repair"
          ? runRepairLoop(deps, { ...(parsed.target ? { target: parsed.target } : {}), ...(parsed.maxFunctions ? { maxFunctions: parsed.maxFunctions } : {}) })
          : runLoop(deps, { firstTarget: parsed.target, maxFunctions: parsed.maxFunctions });
      });
      running = task.catch(() => {});
      try {
        const outcomes = await task;
        ctx.ui.notify(summarize(outcomes), "info");
      } catch (error) {
        ctx.ui.notify(`Escalation loop failed: ${error instanceof Error ? error.message : String(error)}`, "error");
      } finally {
        active = null;
        running = undefined;
      }
    },
  });
  return () => active !== null;
}
