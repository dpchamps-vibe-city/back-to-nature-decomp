# ovl_19_func_800B8064 — outer-callback membership evidence

## Provenance

The documentation handoff reports byte-exact source and a passed full finalize
gate, with verified source/evidence identity
`4af5d8bf2116c0fefb6e0eaafece25167cc748c6ea7164bf35e1a5d9818dd6b6`.
Its origin is agent; the supplied changed files are
`include/globals_override.h`, `include/overlays/ovl_19.h`, and
`src/overlays/ovl_19/ovl_19_func_800B8064.c`. This notes-only review did not
rerun matching or change build inputs.

## New grouping evidence

- The extracted target assembly records a 0x84-byte body, ending exactly at
  existing candidate member `ovl_19_func_800B80E8`:
  `0x800B8064 + 0x84 = 0x800B80E8`. The splat configuration corroborates
  consecutive offsets 0x244 and 0x2C8.
- Overlay-local table `D_800BCE94` contains five callbacks in consecutive
  text order: `800B7F34`, `800B8064`, `800B80E8`, `800B8150`, `800B8190`.
  Dispatcher `800B7ED8` indexes this table using `D_80070CC4`, the word at
  `D_8006C838 + 0x448C`. The verified target increments that same word when
  `func_80013394() == 1` and `func_8001FE6C() == 0`, advancing to its
  adjacent table/text successor. Thus callback order, state progression and
  link adjacency agree.
- The verified target and `800B80E8` share the ordered update chain
  `800BB358` → `800BB470` → `func_8001B5DC` → `800BB4D4`.
  The existing cluster note ties `800B80E8` to the overlay-local
  `D_800BF4C0[2]` selector and handler table. The preceding outer callback
  `800B7F34` also calls cluster initializer `800B82B8`, supporting a coherent
  setup/update/dispatch family rather than adjacency alone.

## Membership conclusion and limits

Add only `ovl_19_func_800B8064` as a candidate member of the existing
`D_800BF4D0`/`D_800BF4C0` state-consumer cluster: an update/completion
callback that advances the outer state after both completion probes pass.

This is conditional same-TU evidence, not a recovered boundary. The target
has no direct access to the cluster's overlay-local state array; the resident
outer counter alone is not private-data evidence. The inference assumes the
ordered callback family and its shared update sequence belonged to one
original module, whereas tables and calls can cross TUs. Do not infer static
linkage, merge the distant far-buffer cluster, or add the other outer callbacks
from this review.

## Evidence paths

- `src/overlays/ovl_19/ovl_19_func_800B8064.c`: verified update chain and
  completion-gated outer-counter increment.
- `include/globals_override.h`: `Ovl19CompletionCounterView.counter` at
  offset 0x448C.
- `build/ovl_19/asm/nonmatchings/ovl_19_func_800B8064/ovl_19_func_800B8064.s`:
  original size 0x84 and counter load/store at 0x800B80C8/0x800B80D4.
- `configs/splat/ovl_19.yaml`: consecutive callback text offsets.
- `build/ovl_19/asm/data/3F54.data.s`: `D_800BCE94` entries at
  0x800BCE94–0x800BCEA4.
- `build/ovl_19/asm/nonmatchings/ovl_19_func_800B7ED8/ovl_19_func_800B7ED8.s`:
  outer-selector load and table dispatch at 0x800B7EF4–0x800B7F14.
- `src/overlays/ovl_19/ovl_19_func_800B80E8.c` and
  `src/overlays/ovl_19/ovl_19_func_800B7F34.c`: shared update chain and
  setup callback's call to `800B82B8`.
- [Existing dispatcher membership evidence](ovl_19_func_800B80E8-grouping.md):
  connection from `800B80E8` to the recorded state-consumer cluster.
