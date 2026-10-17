# ovl_11_func_800EB85C grouping evidence

## Candidate membership

Add `ovl_11_func_800EB85C` to the existing `D_80129560` accessor family,
retaining its confidence and conditional same-TU premise. Shared table and
helper use support candidate membership, not a proven original TU boundary.

Verified source/evidence identity supplied by the handoff:
`0291ebaf97a89e173090b27cc15ba3c1de8385e91f4cff5cf93b47fe92b14940`.

## Evidence

- Verified source: `src/overlays/ovl_11/ovl_11_func_800EB85C.c`.
  It reads the signed low halfword of `D_80129560[arg0]` as sequencing state
  and writes the updated state back as an s32, using an s16 slot index.
- Original assembly:
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800EB85C/ovl_11_func_800EB85C.s`.
  Addresses 0x800EB870–0x800EB8AC establish the signed-halfword index,
  four-byte stride, absolute table base and halfword load; the word store is
  at 0x800EBEF4.
- Direct calls to recorded family members `800EDF3C` (0x800EB8B0),
  `800E5ADC` (0x800EB938) and `800E5B84` (0x800EB950 and later sites)
  corroborate the shared-table tie. Nearby member `800EB010` uses the same
  table and all three helpers; see its verified source and
  `ovl_11_func_800EB010-grouping.md`.
- `configs/splat/ovl_11.yaml` places the target at offset 0x33A3C,
  immediately after family member `800EB79C` at 0x3397C (0xC0 bytes).
  Shared table use corroborates this zero-gap adjacency; there is no direct
  call between those neighbours.

## Limits

`D_80124984` and `D_80124990` have no other named code consumers in
`build/ovl_11/asm/`, so their recovered layouts supply no shared-static tie.
The calls to `800E5870` connect the sequencer to the recorded clamp/helper
family but do not prove the two candidate groups share a TU. Overlay-private
state need not have had internal linkage. No new TU ownership, boundary,
declaration-order, register-variable or SDK-idiom claim is established.
