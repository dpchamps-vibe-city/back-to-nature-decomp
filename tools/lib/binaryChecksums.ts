/** Checked-in reference hashes. Verification never rewrites this manifest. */
import { createHash } from "node:crypto";
import { readFileSync } from "node:fs";
import { join } from "node:path";
import type { Container } from "./container.js";
import { ROOT } from "./psxExeInfo.js";

export const CHECKSUM_MANIFEST = "configs/checksums.json";

interface FileChecksum {
  size: number;
  sha256: string;
}
export interface BinaryChecksum extends FileChecksum {
  scope: "payload" | "file";
  offset: number;
  fullFile?: FileChecksum;
}
export interface ChecksumManifest {
  schemaVersion: 1;
  containers: Record<string, BinaryChecksum>;
}

function record(value: unknown): value is Record<string, unknown> {
  return value !== null && typeof value === "object" && !Array.isArray(value);
}

function fileChecksum(value: unknown): value is FileChecksum {
  return record(value) && Number.isSafeInteger(value.size) && (value.size as number) > 0 &&
    typeof value.sha256 === "string" && /^[0-9a-f]{64}$/.test(value.sha256);
}

export function parseChecksumManifest(value: unknown, containerIds?: readonly string[]): ChecksumManifest {
  if (!record(value) || value.schemaVersion !== 1 || !record(value.containers)) {
    throw new Error(`${CHECKSUM_MANIFEST}: expected schemaVersion 1 and a containers object`);
  }
  const entries = Object.entries(value.containers);
  if (!entries.length) throw new Error(`${CHECKSUM_MANIFEST}: no container hashes`);
  for (const [id, entry] of entries) {
    if (!record(entry) || !fileChecksum(entry) || (entry.scope !== "payload" && entry.scope !== "file") ||
        !Number.isSafeInteger(entry.offset) || (entry.offset as number) < 0 ||
        !Number.isSafeInteger((entry.offset as number) + entry.size) ||
        (entry.scope === "file" && entry.offset !== 0) ||
        (Object.hasOwn(entry, "fullFile") && (entry.scope !== "payload" || !fileChecksum(entry.fullFile) ||
          entry.fullFile.size < (entry.offset as number) + entry.size)) ||
        (id === "exe" && !Object.hasOwn(entry, "fullFile"))) {
      throw new Error(`${CHECKSUM_MANIFEST}: invalid checksum entry for ${id}`);
    }
  }
  if (containerIds) {
    const expected = new Set(containerIds), actual = Object.keys(value.containers);
    const missing = containerIds.filter(id => !Object.hasOwn(value.containers as object, id));
    const extra = actual.filter(id => !expected.has(id));
    if (missing.length || extra.length) {
      throw new Error(`${CHECKSUM_MANIFEST}: container coverage differs (missing: ${missing.join(", ") || "none"}; extra: ${extra.join(", ") || "none"})`);
    }
  }
  return value as unknown as ChecksumManifest;
}

export function loadChecksumManifest(root = ROOT, containerIds?: readonly string[]): ChecksumManifest {
  return parseChecksumManifest(JSON.parse(readFileSync(join(root, CHECKSUM_MANIFEST), "utf8")), containerIds);
}

/** Reject truncation instead of silently hashing a shorter Buffer.subarray. */
export function verifyChecksum(bytes: Buffer, expected: BinaryChecksum, label: string): string {
  const end = expected.offset + expected.size;
  if (bytes.length < end || (expected.scope === "file" && bytes.length !== expected.size)) {
    throw new Error(`${label}: size mismatch (${bytes.length} bytes; expected ${expected.scope === "file" ? "exactly" : "at least"} ${end})`);
  }
  const actual = createHash("sha256").update(bytes.subarray(expected.offset, end)).digest("hex");
  if (actual !== expected.sha256) {
    throw new Error(`${label}: SHA-256 mismatch against ${CHECKSUM_MANIFEST}\n  expected: ${expected.sha256}\n  actual:   ${actual}`);
  }
  if (expected.fullFile) {
    verifyChecksum(bytes, { scope: "file", offset: 0, size: expected.fullFile.size, sha256: expected.fullFile.sha256 }, `${label} full file`);
  }
  return actual;
}

/** Both sides must match the committed reference, not merely each other. */
export function verifyContainerChecksums(container: Container, manifest: ChecksumManifest, root = ROOT): void {
  const expected = manifest.containers[container.id];
  if (!expected) throw new Error(`${CHECKSUM_MANIFEST}: missing container ${container.id}`);
  const scope = container.kind === "exe" ? "payload" : "file";
  if (expected.scope !== scope || (scope === "payload" &&
      (expected.offset !== container.payloadOffset || expected.size !== container.payloadSize))) {
    throw new Error(`${container.id}: configured byte range disagrees with ${CHECKSUM_MANIFEST}`);
  }
  if (scope === "payload" && !expected.fullFile) {
    throw new Error(`${container.id}: missing full-file checksum in ${CHECKSUM_MANIFEST}`);
  }
  for (const [role, path] of [["input", container.targetPath], ["built", container.paths.builtBin]] as const) {
    verifyChecksum(readFileSync(join(root, path)), expected, `${container.id} ${role} (${path})`);
  }
}
