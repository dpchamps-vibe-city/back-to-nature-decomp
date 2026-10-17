# ovl_19_func_800B8150 — outer-transition callback membership evidence

## Provenance

The agent handoff reports byte-exact source and a passed full finalize gate,
with verified source/evidence identity
`c6213a021d67ec5d92f9b3c0813a6aa09c815ff86ef3f7923c450775200149ec`.
Supplied changed files: `include/overlays/ovl_19.h` and
`src/overlays/ovl_19/ovl_19_func_800B8150.c`. This notes-only review did not
rerun matching or change build inputs.

## Additional grouping evidence

Earlier dispatcher/callback research recorded the target's link position,
`800BA770` call and entry in `D_800BCE94`, without adding it as a member.
The additional connection is the verified target's unconditional increment
of the signed word at `D_8006C838 + 0x448C`, the same counter advanced by
candidate member `800B8064`. This is `D_80070CC4`, the selector used by
`800B7ED8` to index the overlay-local outer-callback table.

The target occupies table index 3, immediately after candidate member
`800B80E8` and before `800B8190`. Its increment therefore selects the next
callback. Table and text order agree: `800B80E8 + 0x68 = 800B8150`, and
`800B8150 + 0x40 = 800B8190`; the configured text offsets are 0x2C8, 0x330
and 0x370. Both the target and its successor call existing state-cluster
member `800BA770`. Thus shared state progression, the local call graph and
link adjacency corroborate the ordered callback-family connection.

## Membership conclusion and limits

Add only `ovl_19_func_800B8150` as a candidate member of the existing
`D_800BF4D0`/`D_800BF4C0` state-consumer cluster: a transition callback that
runs the shared render driver and advances the outer state.

The inference assumes the ordered callback family and its render driver
belonged to one original module. The counter is resident shared RAM, not
private-data evidence, and the target does not directly access the cluster's
overlay-local state array. Tables and calls can cross TUs; no original boundary
or static linkage is established. Do not add the other callbacks from this
review.

## Evidence paths

- `src/overlays/ovl_19/ovl_19_func_800B8150.c`: verified call sequence and
  unconditional `Ovl19CompletionCounterView.counter` increment.
- `include/globals_override.h`: counter view at offset 0x448C.
- `src/overlays/ovl_19/ovl_19_func_800B8064.c`: same counter increment.
- `build/ovl_19/functions/ovl_19_func_800B8150.s`: original size 0x40,
  `800BA770` call at 0x800B8168 and counter load/store at
  0x800B8178/0x800B8184.
- `build/ovl_19/functions/ovl_19_func_800B80E8.s` and
  `build/ovl_19/functions/ovl_19_func_800B8190.s`: adjacent bodies and the
  successor's `800BA770` call.
- `build/ovl_19/asm/data/3F54.data.s`: `D_800BCE94` callback table.
- `build/ovl_19/asm/nonmatchings/ovl_19_func_800B7ED8/ovl_19_func_800B7ED8.s`:
  `D_80070CC4` selector load and table dispatch.
- `configs/splat/ovl_19.yaml`: consecutive callback text offsets.
- [Earlier outer-callback evidence](ovl_19_func_800B8064-grouping.md) and
  [dispatcher evidence](ovl_19_func_800B80E8-grouping.md): existing candidate
  connections to the state-consumer cluster.
