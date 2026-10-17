# ovl_19_func_800B8C74 — selection-handler membership evidence

## Provenance

The agent handoff reports byte-exact source and a passed full finalize gate,
with verified source/evidence identity
`61567ec2ac8c466797e1fe932c3ccd59a3eb53d7a0fc8b7ab2101035dd6413de`.
Supplied changed files: `include/globals_override.h`,
`include/overlays/ovl_19.h`, and
`src/overlays/ovl_19/ovl_19_func_800B8C74.c`. This notes-only review did not
rerun matching or change build inputs.

## Additional grouping evidence

[The successor's existing research](ovl_19_func_800B8DA8-grouping.md) already
records this target's table/text adjacency, render call and complementary
completion writes. Those facts alone are not new. The additional connection
is the verified target's direct consumption of state prepared by existing
cluster members:

- Immediate predecessor and cluster hub `800B89C8` selects handler 6 by
  writing `s16@D_800BF4C0+0x4`, writes the result flag at +0xC, and stores a
  `func_8001FABC(0x43)` handle at +0x204. The target is entry 6 of
  `D_800BCEA8`, dispatched by cluster member `800B80E8`. It reads the low
  byte of that same flag and passes the same signed +0x204 handle to
  `func_8001FAE8` after the completion probe returns 2. Thus table order
  agrees with an explicit predecessor-to-target field handoff, not just a
  common render callee.
- Cluster initializer `800B82B8` clears the signed selection at +0x8 and
  seeds eight halfwords at +0x160 through +0x16E. On the nonzero-flag arm,
  the target indexes precisely those seeded values through overlay-local
  byte table `D_800BCF24` (entries 1, 3, 6 for selections 0, 1, 2), then
  obtains a byte offset into `D_800BCD68` through `800BADAC`. This connects
  its text lookup to the cluster's existing selection/data initialization.
- The target increments the same +0x8 selection and, on the saved-zero-flag
  arm while the result is below 3, calls cluster resetter `800B843C` before
  clearing the +0x4 selector. That resetter initializes the same +0xC flag
  and +0x204 handle fields to -1. Otherwise the target selects handler 7;
  with the saved flag nonzero it instead clears completion and advances the
  resident outer counter, as already documented for `800B8DA8`.

The target also fills the gapless text-order slot between recorded members
`800B89C8` (`0x2AC` bytes) and `800B8DA8`:
`800B89C8 + 0x2AC = 800B8C74`, and
`800B8C74 + 0x134 = 800B8DA8`. This corroborates the direct shared-state and
call-graph evidence above.

## Membership conclusion and limits

Add only `ovl_19_func_800B8C74` as a candidate member of the existing
`D_800BF4D0`/`D_800BF4C0` state-consumer cluster: a selection-result text
callback that advances selection and chooses restart, completion or exit.

The same-TU inference assumes this shared overlay-local state, its
initializers and ordered callbacks belonged to one original module. Globals,
handler tables and calls can cross TUs; the evidence does not establish
static linkage or an original TU boundary. Resident `D_8006C838` accesses are
not private-data evidence. The `800BADAC` call alone does not extend cluster
membership to that helper or merge its separately recorded lookup run.

## Evidence paths

- `src/overlays/ovl_19/ovl_19_func_800B8C74.c`: verified flag, selection,
  values and handle accesses; text lookup, reset call and state exits.
- `include/globals_override.h`: `Ovl19SelectionState` field offsets and
  `Ovl19CompletionCounterView` resident-field view.
- `build/ovl_19/asm/nonmatchings/ovl_19_func_800B89C8/ovl_19_func_800B89C8.s`:
  selector-6, flag and handle stores at 0x800B8B38/0x800B8B40/0x800B8B48;
  original size 0x2AC.
- `src/overlays/ovl_19/ovl_19_func_800B82B8.c`: selection initialization and
  eight-value seeding at halfword indices 0xB0–0xB7.
- `src/overlays/ovl_19/ovl_19_func_800B843C.c`: reset of the shared flag and
  handle fields.
- `src/overlays/ovl_19/ovl_19_func_800B80E8.c`: table dispatch using the
  signed +0x4 selector.
- `build/ovl_19/asm/data/3F54.data.s`: target at `D_800BCEA8[6]`
  (0x800BCEC0), adjacent hub/successor entries and `D_800BCF24` byte values.
- `build/ovl_19/asm/nonmatchings/ovl_19_func_800B8C74/ovl_19_func_800B8C74.s`:
  original size 0x134, selection/value lookup at 0x800B8CD4–0x800B8CF0,
  handle read/call at 0x800B8D34/0x800B8D3C and reset call at 0x800B8D60.
