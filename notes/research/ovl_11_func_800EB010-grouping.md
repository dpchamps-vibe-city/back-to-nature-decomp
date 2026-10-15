# ovl_11_func_800EB010 grouping evidence

## Candidate membership

Extend the existing `ovl_11` `D_80129560` accessor family with
`ovl_11_func_800EB010`. This is a shared-table/helper-cluster tie, not proof
of original translation-unit boundaries; retain the family's existing
confidence.

## Evidence

- Verified source: `src/overlays/ovl_11/ovl_11_func_800EB010.c`.
  It reads the low halfword of `D_80129560[arg1]` and reads/writes
  `D_80129560[arg0]` as initialization/update state. Both parameters are
  signed-halfword slot indices into the same table used by the recorded
  family.
- Original instructions:
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800EB010/ovl_11_func_800EB010.s`.
  Addresses 0x800EB014–0x800EB024 form the family's signed-halfword,
  four-byte-stride table index; 0x800EB058 loads the selected halfword.
  The state-slot writes are at 0x800EB298 and 0x800EB2F4.
- The same original assembly calls recorded members `800E5C60` at
  0x800EB13C/0x800EB164/0x800EB1A4/0x800EB1E0, `800EDF3C` at
  0x800EB17C/0x800EB1C4, and the contiguous helper pair `800E5ADC` and
  `800E5B84` at 0x800EB24C/0x800EB260. These direct edges corroborate the
  shared-table family membership; they do not independently establish a TU.

## Limits

The ten-ID template `D_800B9B1C` and parallel tables `D_80124920`,
`D_80124948`, `D_80124970` have no other named consumers in the extracted
`build/ovl_11/asm/` assembly, so they supply no new shared-static tie.
`configs/splat/ovl_11.yaml` places the 0x318-byte function immediately before
`800EB328`, but the target does not call that neighbour; do not infer a new
same-file pair from adjacency alone. No new TU ownership, boundary,
declaration-order or register-variable claim is supported.

The candidate membership remains conditional on the ledger's existing
shared-table-family premise; neither matching declarations nor an overlay-wide
data symbol prove original internal linkage.
