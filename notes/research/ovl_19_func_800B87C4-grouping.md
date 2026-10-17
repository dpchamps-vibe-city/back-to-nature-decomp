# ovl_19_func_800B87C4 — nested-dispatcher membership evidence

## Provenance

The agent handoff reports byte-exact source and a passed full finalize gate,
with verified source/evidence identity
`789451f25143eee7f2e8f3e8a34731909baaaa547dd3d30d77cfe2d89c979df0`.
Supplied changed files: `include/globals_override.h`,
`include/overlays/ovl_19.h`, and
`src/overlays/ovl_19/ovl_19_func_800B87C4.c`. This notes-only review did not
rerun matching or change build inputs.

## Additional grouping evidence

[Earlier handler research](ovl_19_func_800B9124-grouping.md) already recorded
that the target dispatches `D_800BCEC8` using `D_800BF4C0[3]`. The additional
connection is its verified completion handoff: when that inner selector is 7,
it clears `D_800BF4C0[3]`, sets `D_800BF4C0[2]` to 3 and returns without the
normal update/render tail. Existing cluster member `800B93B0` also clears
index 3 and, on one exit arm, sets index 2 to 3.

Index 2 is the selector of `D_800BCEA8`, dispatched by existing candidate
member `800B80E8`. The target is entry 2 of that table; entry 3 is
`800B8830`, its immediate text successor (`800B87C4 + 0x6C = 800B8830`).
Entries 4 and 5 continue in text order through `800B8908` to the recorded
cluster hub `800B89C8`. Thus the target's completion write explicitly selects
its next table/text neighbour, rather than merely referencing a nearby global.
That successor accesses the same `D_800BF4C0` state and calls recorded cluster
members `800B843C` and `800BA770`.

On the normal path, the target dispatches the seven handlers in `D_800BCEC8`
and then calls `800B998C` followed by `800BA770`, the same ordered pair used
by cluster hub `800B89C8`. All seven table entries are already recorded
state-cluster members. Shared overlay-local state, the nested indirect call
graph and the explicit handoff corroborated by link order support adding the
target to that existing cluster.

## Membership conclusion and limits

Add only `ovl_19_func_800B87C4` as a candidate member of the existing
`D_800BF4D0`/`D_800BF4C0` s16-state consumer cluster: a nested seven-handler
dispatcher with a completion handoff to the next outer callback.

The same-TU inference assumes this state object, ordered handler family and
shared update/render helpers belonged to one original module. Overlay-local
globals, tables and calls can cross TU boundaries; neither static linkage nor
an original file boundary is established. The neighbouring callbacks are not
added by this review.

## Evidence paths

- `src/overlays/ovl_19/ovl_19_func_800B87C4.c`: verified selector, completion
  writes and update/render tail.
- `src/overlays/ovl_19/ovl_19_func_800B80E8.c` and
  `include/globals_override.h`: outer-table dispatch and its signed selector
  `D_800BF4C4` (`D_800BF4C0[2]`); seven-entry inner-table declaration.
- `src/overlays/ovl_19/ovl_19_func_800B93B0.c`: complementary exit writes to
  the same inner and outer selectors.
- `build/ovl_19/asm/data/3F54.data.s`: `D_800BCEA8` entries at
  0x800BCEA8–0x800BCEC4 and `D_800BCEC8` entries at 0x800BCEC8–0x800BCEE0.
- `build/ovl_19/asm/nonmatchings/ovl_19_func_800B87C4/ovl_19_func_800B87C4.s`:
  original size 0x6C, exit stores at 0x800B87E8/0x800B87F0 and direct calls
  at 0x800B8810/0x800B8818.
- `build/ovl_19/asm/nonmatchings/ovl_19_func_800B8830/ovl_19_func_800B8830.s`:
  immediate successor's shared-state accesses and cluster calls.
- `build/ovl_19/asm/nonmatchings/ovl_19_func_800B89C8/ovl_19_func_800B89C8.s`:
  hub paths call `800B998C` before the common `800BA770` tail.
- `configs/splat/ovl_19.yaml`: consecutive text offsets 0x9A4, 0xA10,
  0xAE8 and 0xBA8 for `800B87C4` through `800B89C8`.
