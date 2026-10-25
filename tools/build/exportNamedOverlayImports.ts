/**
 * Named overlay callbacks referenced by the EXE are absolute imports, not
 * EXE functions and not aliases for every overlay sharing the RAM slot.
 * Read each overlay's authored names against its original entry inventory;
 * provide only semantic function names, with cross-container collisions
 * refused. This runs before linking, so naming a callback cannot introduce
 * an EXE -> overlay split -> engine export -> EXE dependency cycle.
 */
import { existsSync, mkdirSync, readFileSync, writeFileSync } from "node:fs";
import { dirname, join } from "node:path";
import { ROOT } from "../lib/psxExeInfo.js";
import { loadContainers } from "../lib/container.js";
import { loadOriginalFunctionInventory } from "../lib/symbolIndex.js";
import { applyOverlayFunctionNames } from "../lib/overlayFunctionBoundaries.js";

const owners = new Map<string, string>();
const lines = ["/* Generated container-qualified overlay function imports. */"];
for (const container of loadContainers()) {
  const inventory = loadOriginalFunctionInventory(container);
  if (inventory.findings.length) throw new Error(`${container.id}: ${inventory.findings[0]}`);
  if (container.kind === "exe") {
    for (const fn of inventory.functions) owners.set(fn.name, container.id);
    continue;
  }
  const path = join(ROOT, container.paths.symbolAddrs.replace(/\.txt$/, ".names.txt"));
  if (!existsSync(path)) continue;
  const functions = applyOverlayFunctionNames(inventory.functions.map((fn) => ({
    address: fn.vram, name: fn.name,
  })), readFileSync(path, "utf8"));
  for (const fn of functions) {
    if (/^(?:ovl_\d+_)?func_[0-9A-Fa-f]+$/.test(fn.name)) continue;
    const previous = owners.get(fn.name);
    if (previous) throw new Error(`${fn.name} belongs to both ${previous} and ${container.id}`);
    owners.set(fn.name, container.id);
    lines.push(`PROVIDE(${fn.name} = 0x${fn.address.toString(16).toUpperCase()}); /* ${container.id} */`);
  }
}
const path = join(ROOT, "build/named_overlay_imports.ld");
const text = lines.join("\n") + "\n";
if (!existsSync(path) || readFileSync(path, "utf8") !== text) {
  mkdirSync(dirname(path), { recursive: true });
  writeFileSync(path, text);
}
