# ovl_19_func_800B8830 — timed outer-callback membership evidence

## Provenance

The agent handoff reports byte-exact source and a passed full finalize gate,
with verified source/evidence identity
`7400ecd3900918109e49c0c0273d1d36f2a5b7cde37fdede6007098580c54534`.
Supplied changed files: `include/globals_override.h`,
`include/overlays/ovl_19.h`, and
`src/overlays/ovl_19/ovl_19_func_800B8830.c`. This notes-only review did not
rerun matching or change build inputs.

## Additional grouping evidence

[Earlier dispatcher research](ovl_19_func_800B87C4-grouping.md) already
recorded this target's outer-table/text adjacency, shared `D_800BF4C0` state
and calls to cluster members `800B843C` and `800BA770`. The new connection is
the verified completion handoff and its shared timer/command-record pattern
with successor `800B8908`, whose membership is
[already recorded](ovl_19_func_800B8908-grouping.md).

The target is entry 3 of `D_800BCEA8`, dispatched by cluster member
`800B80E8` using `D_800BF4C4` (`D_800BF4C0[2]`). When `func_800226A4()`
returns 5, it increments `D_800BF4C0[0x100]`. At 60 it commands the records
at byte offsets +0xA0/+0xE0/+0x120 through `800BAC50` with values 1/1/4,
calls `func_80022738`, clears that counter and sets the outer selector to 4.
This explicitly selects `800B8908`, its immediate table and text successor:
`800B8830 + 0xD8 = 800B8908`.

That successor uses the same completion probe, counter, ordered three-record
setter calls and `func_80022738` tail; its threshold is 45, its third command
is 0, and its handoff selects the following callback. Cluster initializer
`800B8544` initializes the same record run at +0xA0 with stride 0x40. Thus
shared state and helpers are corroborated by a concrete sequential handoff
and matching callback/link order, supporting candidate membership in the
existing `D_800BF4D0`/`D_800BF4C0` s16-state consumer cluster.

## Membership conclusion and limits

Add only `ovl_19_func_800B8830`: a timed outer callback that commands three
shared records and advances to the next outer handler.

The same-TU inference assumes the shared state, record family and sequential
callbacks belonged to one original module. Overlay-local globals, tables and
calls can cross TU boundaries; neither static linkage nor an original file
boundary is established. No other membership or cluster boundary changes.

## Evidence paths

- `src/overlays/ovl_19/ovl_19_func_800B8830.c`: verified timer, record commands,
  shared helper calls and selector write.
- `src/overlays/ovl_19/ovl_19_func_800B8908.c`: successor's parallel timer,
  command-record sequence and outer-state handoff.
- `src/overlays/ovl_19/ovl_19_func_800B80E8.c` and
  `include/globals_override.h`: outer-table dispatch and signed selector.
- `src/overlays/ovl_19/ovl_19_func_800B8544.c`: initialization of the same
  three-record run.
- `build/ovl_19/asm/data/3F54.data.s`: adjacent target/successor table entries
  at 0x800BCEB4/0x800BCEB8.
- `build/ovl_19/asm/nonmatchings/ovl_19_func_800B8830/ovl_19_func_800B8830.s`:
  original size 0xD8, threshold at 0x800B88AC, three record arguments at
  0x800B88B4/0x800B88C4/0x800B88D4 and selector store at 0x800B88F4.
- `configs/splat/ovl_19.yaml`: consecutive text offsets 0xA10/0xAE8 for the
  target and successor.
