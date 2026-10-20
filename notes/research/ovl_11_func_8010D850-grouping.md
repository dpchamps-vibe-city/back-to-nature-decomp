# ovl_11_func_8010D850: candidate sibling-helper grouping

## Verified handoff

- Source: `src/overlays/ovl_11/ovl_11_func_8010D850.c`.
- Supplied verified source/evidence identity:
  `b1d83cd797bbfde6f22691d34575253f9ae03a0afb68047d8d42ea7be2a4f08b`.
- The handoff reports byte identity and a passed full finalize gate. This
  documentation-only review accepts that result without rebuilding or changing
  build inputs.

## New grouping evidence

The [D748 review](ovl_11_func_8010D748-grouping.md) excluded D850 from its
candidate group on calls alone. The verified D850 body now supplies the sibling
pattern needed to corroborate those call/link observations:

- D850 and `src/overlays/ovl_11/ovl_11_func_8010D748.c` use the same
  unsigned-halfword +0xB2 / 100 calculation plus `func_80012A34(3)`, accept
  the object and a signed-halfword area, and report failure on a signed-halfword
  -1 from `ovl_11_func_80110944`. D850 requests mode 4, then mode 8 for its
  fallback; D748 requests mode 2. D648 supplies the related /50, mode-1
  variant documented in its [review](ovl_11_func_8010D648-grouping.md).
- D850's first path and D748 dispatch on the same 0xB8-byte record array:
  D850 uses `D_800749F4`, equal to D748's `D_80074838 + 0x1BC`, and both
  select `800DF010` versus `800E0C24` from bit 0x10000 at record +0x34.
  D748 uses action 6; D850 uses actions 7/8 and stages the object's +0xAC
  value at record +0xB6. This is a complementary local action-helper pattern,
  not merely a shared general-purpose callee.
- `build/ovl_11/asm/nonmatchings/ovl_11_func_8010D314/ovl_11_func_8010D314.s`
  calls D850 with the current object and areas 5, 4 and 1 at 0x8010D5C8,
  0x8010D5E8 and 0x8010D608. The results feed the same failure-tested chain
  containing D648 and D748.
- `configs/splat/ovl_11.yaml` places D314, D648, D748 and D850 at offsets
  0x554F4, 0x55828, 0x55928 and 0x55A30. The address/size entries in
  `build/ovl_11/ovl_11.map` give the gapless sequence D314 (0x334) ->
  D648 (0x100) -> D748 (0x108) -> D850 (0x1B4).
  `build/ovl_11/asm/nonmatchings/ovl_11_func_8010D850/ovl_11_func_8010D850.s`
  independently confirms D850's start, size and dispatch paths. The
  reconstruction's separate object-file names do not identify original TUs.

D850's role is area-selected action-7/8 dispatch with a 0x18-byte-record
fallback through `800F413C`, +0xB6 result handling and failure reporting.

## Conclusion and limits

Add D850 as a low-confidence same-TU candidate alongside D314/D648/D748 in the
existing object-mode dispatch section. The new evidence is the verified
sibling pattern together with the shared caller and gapless link order; it
neither establishes original TU boundaries nor raises the wider section's
confidence.

D850 defines no shared static or global. Access to the common work area is not
ownership evidence, and no declaration-order or register-variable quirk is
established here. Do not extend membership to its distant selector, dispatch,
fallback or reset callees, or to the adjacent DA04 handler, from this review.
