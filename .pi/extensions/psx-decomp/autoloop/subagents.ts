import type { ExtensionAPI } from "@earendil-works/pi-coding-agent";
import { loadLoopConfig } from "./config.ts";
import type { SubagentConfig } from "./types.ts";

/**
 * Research subagents for the fold driver, through pi-subagents.
 *
 * Folding writes shared state — the container's split configuration, symbol
 * files, `globals_override.h`, the build — so one agent folds, in sequence.
 * Finding units does not: it reads sources, notes and the original bytes,
 * one address range at a time, and ranges are independent. With `subagents`
 * in `.pi/autoloop.json`, this registers a read-only research agent on that
 * model, which the driver runs `quantity` at a time over separate ranges and
 * folds from their reports. Without the field, or without pi-subagents,
 * nothing is registered and the driver works alone.
 */

export const RESEARCH_AGENT = "psx-unit-research";

/* pi-subagents' process-local registration contract (docs/extension-api.md). */
const REGISTER_EVENT = "pi-subagents:runtime-agent-register:v1";

interface RegistrationRequest {
  version: 1;
  name: string;
  definition: Record<string, unknown>;
  result?: { ok: true; registration: { dispose(): void } } | { ok: false; error: Error };
}

export function researchAgentPrompt(config: SubagentConfig): string {
  return [
    "You research translation-unit boundaries for a matching PSX decompilation, for a driver agent that folds",
    "the units you find. Read AGENTS.md and the \"Discover the units\" section of plans/tu-prompt.md first; their",
    "evidence rules are yours. You are given one address range or candidate group; stay inside it.",
    "",
    "You only read. Never edit or write a file, never run make, a split, foldTranslationUnit.ts --write,",
    "canonicalNames.ts --write, interfaceAudit.ts --write, tuProgress.ts --verify, or any git command that",
    "changes state. Other agents work in this tree at the same time. Read-only commands are fine: rg, sed -n,",
    "cat, `npx tsx tools/diagnostics/tuProgress.ts --remaining --json`, `npx tsx tools/build/deriveTuOwnedGlobals.ts",
    "--json`, `npx tsx tools/diagnostics/interfaceAudit.ts --callee <name>`. Search notes/file-groupings.md with",
    "grep -n for names and addresses; never read it whole.",
    "",
    "Report, for each unit the evidence supports, compactly and without file dumps:",
    "- container, and the members in address order by their current names;",
    "- the evidence that puts them in one unit, and what each boundary rests on;",
    "- a proposed module name, and names for members and the globals they own, each with its evidence;",
    "- constraints the fold must respect: flag overrides, pooled .rodata order, live INCLUDE_ASM stubs;",
    "- what remains uncertain.",
    "Say plainly when the range holds no unit the evidence supports.",
    "",
    `Report before your context reaches about ${config.model.checkpointAtTokens} tokens. If the range is larger than that,`,
    "report what you finished and name the addresses you did not reach.",
  ].join("\n");
}

/** Register the research agent on each session start, when autoloop.json configures one. */
export function registerResearchAgent(pi: ExtensionAPI, projectRoot: string): void {
  let registration: { dispose(): void } | undefined;
  const release = (): void => {
    registration?.dispose();
    registration = undefined;
  };
  pi.on("session_start", () => {
    release();
    /* A child loads the ambient extensions, this one included; it delegates nothing. */
    if (process.env.PI_SUBAGENT_CHILD === "1") return;
    let config: SubagentConfig | undefined;
    try {
      config = loadLoopConfig(projectRoot).subagents;
    } catch {
      return; /* the loop command reports a malformed config itself */
    }
    if (!config) return;
    const request: RegistrationRequest = {
      version: 1,
      name: RESEARCH_AGENT,
      definition: {
        description: "Read-only research of translation-unit boundaries over one address range; reports units, evidence and names for the fold driver.",
        systemPrompt: researchAgentPrompt(config),
        model: `${config.model.provider}/${config.model.model}`,
        thinking: config.model.thinking,
        tools: ["read", "grep", "find", "ls", "bash"],
        /* `extensions` stays unset: the ambient extensions include whatever registers the
           model's provider, and a child without them cannot resolve it. */
      },
    };
    pi.events.emit(REGISTER_EVENT, request);
    if (request.result?.ok) registration = request.result.registration;
  });
  pi.on("session_shutdown", release);
}
