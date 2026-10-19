# ovl_11_func_800CBE2C: adjacent pool-interaction helper

## Verified anchor

The supplied handoff reports byte-exact matching and a passed full finalize
gate, source/evidence identity
`010ccf8e5e8f4a66d875224a41675010eb2ddec1bd4e677b402174c1e5a7817b`
(origin: agent), with changes to `include/globals_override.h` and
`src/overlays/ovl_11/ovl_11_func_800CBE2C.c`. This documentation-only review
read the verified source without changing or rebuilding its inputs.

## New membership evidence

- **The pool update path calls this helper on the current entry.** The
  already-listed `800CCB90` walks the three `D_80128BB0` entries at stride
  0x34 and calls `800CC0F8` at 0x800CCBBC for occupied entries. That updater
  passes the same entry to `800CBE2C` at 0x800CC234 and exits its update when
  the helper returns nonzero. The verified helper probes from entry +0x24
  and, on success, passes the entry to removal helper `800CCBE8` at
  0x800CBEDC. The updater also calls that removal helper at 0x800CC2E0 and
  0x800CC308. This is a connected entry-lifecycle path, not simply another
  reference to the engine work-area base.
- **Link order places the helper immediately before the existing init run.**
  Original sizes give `800CBE2C + 0xCC = 800CBEF8`,
  `800CBEF8 + 0x48 = 800CBF40`, `800CBF40 + 0x30 = 800CBF70`, and
  `800CBF70 + 0x188 = 800CC0F8`. `configs/splat/ovl_11.yaml` lists those
  functions consecutively at offsets 0x1400C, 0x140D8, 0x14120, 0x14150 and
  0x142D8. Thus the nearby caller corroborates the helper's adjacency to
  the recorded pool init/accessor family.

Assembly evidence is in
`build/ovl_11/asm/nonmatchings/<function>/<function>.s` for full names
`ovl_11_func_800CBE2C`, `ovl_11_func_800CBEF8`, `ovl_11_func_800CBF70`,
`ovl_11_func_800CC0F8` and `ovl_11_func_800CCB90`; the clear leaf's original
listing is `build/ovl_11/functions/ovl_11_func_800CBF40.s`. Cleanup semantics
are visible in `src/overlays/ovl_11/ovl_11_func_800CCBE8.c`.

## Membership and limits

Add only `800CBE2C` as a **low-confidence candidate same-TU member** of the
existing `D_80128BB0` init/update family. The premise is that the connected
pool lifecycle and helper-before-init link arrangement reflect source-file
locality rather than a TU boundary immediately before `800CBEF8`.

No private static, GP ownership, declaration-order effect or shared register
quirk was established. The target does not directly name `D_80128BB0`; the
connection is the entry passed down the update chain. Shared accesses to
`D_8006C838`/`D_8007AFF0`, the collision-tag ABI and calls to distant engine
helpers do not independently establish TU membership. This addition does not
merge neighbouring hypotheses or establish exact file boundaries.
