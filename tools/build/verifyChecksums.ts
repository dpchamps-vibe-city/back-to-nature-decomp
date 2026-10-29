/** Read-only binary verification against the committed SHA-256 references. */
import { resolve } from "node:path";
import { fileURLToPath } from "node:url";
import { loadContainers } from "../lib/container.js";
import { CHECKSUM_MANIFEST, loadChecksumManifest, verifyContainerChecksums } from "../lib/binaryChecksums.js";

export function verifyChecksums(args: string[]): void {
  if (args.length && !(args.length === 1 && args[0] === "--all") &&
      !(args.length === 2 && args[0] === "--container" && args[1])) {
    throw new Error("Usage: npx tsx tools/build/verifyChecksums.ts [--all | --container <id>]");
  }
  const containers = loadContainers();
  const manifest = loadChecksumManifest(undefined, containers.map(container => container.id));
  const selected = args[0] === "--container" ? containers.filter(container => container.id === args[1]) : containers;
  if (!selected.length) throw new Error(`Unknown container: ${args[1]}`);
  for (const container of selected) {
    verifyContainerChecksums(container, manifest);
    console.log(`OK: ${container.id} input and build match ${CHECKSUM_MANIFEST}${container.kind === "exe" ? " (payload + full file)" : ""}`);
  }
}

if (process.argv[1] && resolve(process.argv[1]) === fileURLToPath(import.meta.url)) {
  try { verifyChecksums(process.argv.slice(2)); }
  catch (error) { console.error(`verifyChecksums: ${(error as Error).message}`); process.exitCode = 1; }
}
