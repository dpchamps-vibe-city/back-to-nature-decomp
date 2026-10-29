import assert from "node:assert/strict";
import { createHash } from "node:crypto";
import { mkdtempSync, mkdirSync, readFileSync, rmSync, writeFileSync } from "node:fs";
import { tmpdir } from "node:os";
import { join } from "node:path";
import { test } from "node:test";
import type { Container } from "./container.js";
import { CHECKSUM_MANIFEST, loadChecksumManifest, parseChecksumManifest, verifyChecksum, verifyContainerChecksums, type BinaryChecksum } from "./binaryChecksums.js";

const hash = (bytes: Buffer) => createHash("sha256").update(bytes).digest("hex");
const bytes = Buffer.from("abc");
const entry: BinaryChecksum = { scope: "file", offset: 0, size: 3, sha256: "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad" };
const manifest = { schemaVersion: 1 as const, containers: { test: entry } };

test("manifest validates schema, hash, range and exact container coverage", () => {
  assert.deepEqual(parseChecksumManifest(manifest, ["test"]), manifest);
  for (const value of [null, [], {}, { ...manifest, schemaVersion: 2 }, { schemaVersion: 1, containers: {} }]) {
    assert.throws(() => parseChecksumManifest(value), /configs\/checksums.json/);
  }
  for (const changes of [{ sha256: "abc" }, { sha256: null }, { scope: "unknown" }, { offset: -1 },
    { offset: 1 }, { size: 0 }, { size: 1.5 }, { size: Number.MAX_SAFE_INTEGER + 1 },
    { scope: "payload", offset: Number.MAX_SAFE_INTEGER }]) {
    assert.throws(() => parseChecksumManifest({ ...manifest, containers: { test: { ...entry, ...changes } } }), /invalid checksum entry/);
  }
  assert.throws(() => parseChecksumManifest(manifest, ["test", "new"]), /missing: new/);
  assert.throws(() => parseChecksumManifest(manifest, []), /extra: test/);
});

test("manifest requires a valid full-file reference for the EXE", () => {
  const payload = { ...entry, scope: "payload", offset: 4 };
  const fullFile = { size: 7, sha256: hash(Buffer.from("HEADabc")) };
  const value = { schemaVersion: 1, containers: { exe: { ...payload, fullFile } } };
  assert.doesNotThrow(() => parseChecksumManifest(value, ["exe"]));
  assert.throws(() => parseChecksumManifest({ ...value, containers: { exe: payload } }), /invalid checksum entry for exe/);
  for (const invalid of [undefined, null, {}, { ...fullFile, size: 6 }, { ...fullFile, size: 7.5 }, { ...fullFile, sha256: "bad" }]) {
    assert.throws(() => parseChecksumManifest({ ...value, containers: { exe: { ...payload, fullFile: invalid } } }), /invalid checksum entry for exe/);
  }
  assert.throws(() => parseChecksumManifest({ schemaVersion: 1, containers: { test: { ...entry, fullFile } } }), /invalid checksum entry/);
});

test("file hashes cover every byte and reject truncation, extension and changed content", () => {
  assert.equal(verifyChecksum(bytes, entry, "fixture"), entry.sha256);
  assert.throws(() => verifyChecksum(Buffer.from("abd"), entry, "fixture"), /fixture: SHA-256 mismatch.*configs\/checksums.json/s);
  assert.throws(() => verifyChecksum(Buffer.from("ab"), entry, "fixture"), /size mismatch/);
  assert.throws(() => verifyChecksum(Buffer.from("abcd"), entry, "fixture"), /size mismatch/);
});

test("pinned payload ranges preserve partial-block boundaries and exclude headers", () => {
  const data = Buffer.alloc(140_000); for (let i = 0; i < data.length; i++) data[i] = (i * 37 + 19) & 255;
  for (const [offset, size] of [[0, 1], [1, 1], [2048, 65535], [2048, 65536], [2049, 65537], [139999, 1]]) {
    const expected: BinaryChecksum = { scope: "payload", offset: offset!, size: size!, sha256: hash(data.subarray(offset, offset! + size!)) };
    assert.equal(verifyChecksum(data, expected, "payload"), expected.sha256);
    assert.throws(() => verifyChecksum(data.subarray(0, offset! + size! - 1), expected, "short"), /size mismatch/);
  }
  const expected: BinaryChecksum = { ...entry, scope: "payload", offset: 4 };
  assert.equal(verifyChecksum(Buffer.from("HEADabc"), expected, "input"), entry.sha256);
  assert.equal(verifyChecksum(Buffer.from("DIFFabc"), expected, "built"), entry.sha256);
  assert.throws(() => verifyChecksum(Buffer.from("HEADabd"), expected, "built"), /SHA-256 mismatch/);
});

test("input and build must independently match the immutable reference", t => {
  const root = mkdtempSync(join(tmpdir(), "binary-checksums-"));
  t.after(() => rmSync(root, { recursive: true, force: true }));
  mkdirSync(join(root, "configs"));
  const path = join(root, CHECKSUM_MANIFEST), original = JSON.stringify(manifest);
  writeFileSync(path, original);
  const input = join(root, "input.bin"), built = join(root, "built.bin");
  const container = { id: "test", kind: "overlay", targetPath: "input.bin", paths: { builtBin: "built.bin" } } as Container;
  const check = () => verifyContainerChecksums(container, loadChecksumManifest(root, ["test"]), root);
  writeFileSync(input, bytes); writeFileSync(built, bytes); check();
  writeFileSync(built, "abd"); assert.throws(check, /test built.*SHA-256 mismatch/);
  writeFileSync(input, "abd"); assert.throws(check, /test input.*SHA-256 mismatch/);
  writeFileSync(built, bytes); assert.throws(check, /test input.*SHA-256 mismatch/);
  writeFileSync(input, bytes); rmSync(built); assert.throws(check, /ENOENT/);
  assert.equal(readFileSync(path, "utf8"), original, "verification never updates the reference");
  rmSync(path); assert.throws(check, /ENOENT/);
});

test("EXE header-derived range cannot silently replace the pinned range", t => {
  const root = mkdtempSync(join(tmpdir(), "payload-range-"));
  t.after(() => rmSync(root, { recursive: true, force: true }));
  writeFileSync(join(root, "input.bin"), "HEADabc"); writeFileSync(join(root, "built.bin"), "HEADabc");
  const container = { id: "exe", kind: "exe", targetPath: "input.bin", paths: { builtBin: "built.bin" }, payloadOffset: 4, payloadSize: 3 } as Container;
  const manifest = { schemaVersion: 1 as const, containers: { exe: { ...entry, scope: "payload" as const, offset: 4,
    fullFile: { size: 7, sha256: hash(Buffer.from("HEADabc")) } } } };
  verifyContainerChecksums(container, manifest, root);
  for (const changed of [{ ...container, payloadOffset: 5 }, { ...container, payloadSize: 2 }]) {
    assert.throws(() => verifyContainerChecksums(changed, manifest, root), /byte range disagrees/);
  }
  assert.throws(() => verifyContainerChecksums(container, { schemaVersion: 1, containers: {} }, root), /missing container exe/);
  assert.throws(() => verifyContainerChecksums(container, { schemaVersion: 1, containers: { exe: entry } }, root), /byte range disagrees/);
  const { fullFile, ...payloadOnly } = manifest.containers.exe;
  assert.throws(() => verifyContainerChecksums(container, { schemaVersion: 1, containers: { exe: payloadOnly } }, root), /missing full-file checksum/);
});

test("full EXE check catches header-only drift and trailing bytes despite a matching payload", t => {
  const root = mkdtempSync(join(tmpdir(), "full-exe-"));
  t.after(() => rmSync(root, { recursive: true, force: true }));
  const original = Buffer.from("HEADabc"), input = join(root, "input.bin"), built = join(root, "built.bin");
  const container = { id: "exe", kind: "exe", targetPath: "input.bin", paths: { builtBin: "built.bin" }, payloadOffset: 4, payloadSize: 3 } as Container;
  const expected: BinaryChecksum = { ...entry, scope: "payload", offset: 4, fullFile: { size: original.length, sha256: hash(original) } };
  const manifest = { schemaVersion: 1 as const, containers: { exe: expected } };
  const check = () => verifyContainerChecksums(container, manifest, root);
  writeFileSync(input, original); writeFileSync(built, original); check();
  writeFileSync(built, "DIFFabc");
  assert.equal(verifyChecksum(readFileSync(built), { ...entry, scope: "payload", offset: 4 }, "payload"), entry.sha256);
  assert.throws(check, /exe built.*full file: SHA-256 mismatch/);
  writeFileSync(input, "DIFFabc"); assert.throws(check, /exe input.*full file: SHA-256 mismatch/);
  writeFileSync(built, original); assert.throws(check, /exe input.*full file: SHA-256 mismatch/);
  writeFileSync(input, original);
  writeFileSync(built, "HEADabc!"); assert.throws(check, /exe built.*full file: size mismatch/);
  writeFileSync(built, "HEADab"); assert.throws(check, /exe built.*size mismatch/);
  writeFileSync(built, original); check();
});
