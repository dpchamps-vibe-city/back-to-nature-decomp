import assert from "node:assert/strict";
import { execFileSync } from "node:child_process";
import { readFileSync } from "node:fs";
import { join } from "node:path";
import { test } from "node:test";
import { CHECKSUM_MANIFEST, loadChecksumManifest } from "../lib/binaryChecksums.js";
import { loadContainers } from "../lib/container.js";
import { ROOT } from "../lib/psxExeInfo.js";
import { verifyChecksums } from "./verifyChecksums.js";

test("committed manifest covers the configured binaries and the read-only CLI verifies all of them", () => {
  const containers = loadContainers();
  const manifest = loadChecksumManifest(ROOT, containers.map(container => container.id));
  assert.ok(manifest.containers.exe?.fullFile, "full EXE reference must not be silently dropped");
  const path = join(ROOT, CHECKSUM_MANIFEST), before = readFileSync(path, "utf8");
  const output = execFileSync("npx", ["tsx", "tools/build/verifyChecksums.ts", "--all"], { cwd: ROOT, encoding: "utf8" });
  assert.equal(output.trim().split("\n").length, containers.length);
  for (const container of containers) assert.ok(output.includes(`OK: ${container.id} input and build match ${CHECKSUM_MANIFEST}`));
  assert.ok(output.includes(`${CHECKSUM_MANIFEST} (payload + full file)`));
  assert.equal(readFileSync(path, "utf8"), before);
});

test("Make's EXE and overlay checks use the committed manifest verifier", () => {
  const makefile = readFileSync(join(ROOT, "Makefile"), "utf8");
  assert.match(makefile, /check: \$\(BUILT_BIN\)\n[^\n]+\n\t@npx tsx tools\/build\/verifyChecksums.ts --container exe/);
  assert.match(makefile, /check-\$\(1\):[^\n]+\n[^\n]+\n\t@npx tsx tools\/build\/verifyChecksums.ts --container \$\(1\)/);
  assert.doesNotMatch(makefile, /original\.sha256|built\.sha256|configs\/checksum\.sha256/);
});

test("CLI rejects unknown containers and has no implicit reference-update mode", () => {
  assert.throws(() => verifyChecksums(["--container", "unknown"]), /Unknown container/);
  for (const args of [["--update"], ["--write"], ["--container"], ["--all", "--container", "exe"]]) {
    assert.throws(() => verifyChecksums(args), /Usage/);
  }
});
