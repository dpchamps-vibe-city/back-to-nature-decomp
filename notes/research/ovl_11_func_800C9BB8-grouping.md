# ovl_11_func_800C9BB8 grouping evidence

Verified handoff identity: `daaddcfc71817f139ca745f2305abcbaca78bf10f3ec5ce3a53a67ea68eb76e4` (origin: agent).
The supplied handoff reports byte identity and a passed full finalize gate;
this documentation pass does not repeat or modify that verification.

## New caller/link pairing

- Verified source: `src/overlays/ovl_11/ovl_11_func_800C9BB8.c`.
  It walks three 3×s16 record tables, passing each record's first halfword
  to `ovl_11_func_800D6A94`: two entries at `D_80071A00+0x84`, eight at
  `+0xC0`, then eight at `+0x90`.
- Original assembly: `build/ovl_11/functions/ovl_11_func_800C9BB8.s`.
  The function occupies 0xA8 bytes and ends exactly at `0x800C9C60`.
- Immediate successor/caller:
  `build/ovl_11/functions/ovl_11_func_800C9C60.s` calls the target at
  `0x800C9D00`. It also retains `&D_80071A00` as its base and conditionally
  seeds the `+0x90` record table before that call. Thus call/link adjacency
  agrees with a shared record-table producer/consumer relationship.

This supports a candidate same-TU pairing of `800C9BB8` and `800C9C60`.
The latter is already listed in the ledger's `D_80128C4C / D_80128C54`
state-pair cluster, so the target is recorded there as its adjacent helper.

## Limits

The target does not itself reference `D_80128C4C` or `D_80128C54`; association
with that wider cluster is conditional on the existing membership hypothesis
for `800C9C60`. This evidence neither fixes TU boundaries nor raises the
wider cluster's confidence. The same tables have distant and cross-container
consumers (see the ledger's record-table compaction cluster); table identity
alone does not join those consumers. Nor does calling the distant
`800D6A94` place that callee in this TU.
