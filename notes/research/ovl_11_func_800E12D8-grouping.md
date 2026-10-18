# ovl_11_func_800E12D8 — handler-run grouping

Verified source/evidence identity supplied by the finalization handoff:
`faa420a65c2a8c37b4962e3e72bacd540363d347a870281afd6fa5772a2e9afd`.
The handoff reports byte identity and the full finalize gate, with
`src/overlays/ovl_11/ovl_11_func_800E12D8.c` as the changed file. This
notes-only review did not rerun matching or change build inputs.

## New evidence for existing candidate membership

The ledger already records E12D8 in the `D_800B961C` handler table. Its
verified body adds the reset/state-gate idiom shared with neighbours E11C0
and E1254: reset through `ovl_11_func_800D049C`, then notify through
`ovl_11_func_800D12A0(0xB)` only when the object's `s16@+0x30` equals the
halfword at `(char *)&D_8007AFF0 + 0x25476`. E12D8 also rejects a nonzero
`u16@+0xAE`, the latch E11C0 consumes and clears.

The shared sequence agrees with gapless link order: E11C0 (0x94 bytes)
ends at E1254; E1254 (0x84) ends at E12D8; E12D8 (0x12C) ends at E1404.
All four are entries in `D_800B961C`. Thus neighbouring handler idiom,
common dispatch table and link order jointly reinforce the existing
medium-confidence dispatcher/run grouping.

E12D8's role is a code-0x165 record-pair reservation/reset handler: it
requires clear +0xB2/+0xAE latches and `u16@+0xB4 >= 7`, obtains a pair slot
and a free record, records both indices, marks the free record reserved,
and sets +0xB2 before the reset/notification tail.

**Limits:** E12D8 does not call `ovl_11_func_800E109C`; this is handler-idiom
and adjacency evidence, not a new predicate caller. The far-state global
and engine helpers alone do not prove common ownership or a TU boundary.
The similar reservation body in distant `ovl_11_func_800DF72C` supports a
parallel handler pattern, not merging the two runs. Shared reconstruction
types are not original declaration-order evidence.

## Evidence paths

- `src/overlays/ovl_11/ovl_11_func_800E12D8.c`: verified handler body.
- `src/overlays/ovl_11/ovl_11_func_800E11C0.c` and
  `src/overlays/ovl_11/ovl_11_func_800E1254.c`: neighbouring latch/reset/
  state-gate idioms.
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800E12D8/ovl_11_func_800E12D8.s`
  and the corresponding E11C0/E1254 assembly: original calls and extents.
- `configs/splat/ovl_11.yaml`: consecutive E11C0/E1254/E12D8/E1404 entries
  at file offsets 0x293A0/0x29434/0x294B8/0x295E4.
- `build/ovl_11/asm/data/168C.rodata.s`: current generated table listing;
  E12D8 is the `D_800B961C` entry at 0x800B9644 (index 0xA).
