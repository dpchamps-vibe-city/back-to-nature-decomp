# ovl_11_func_8010D748: candidate sibling-helper grouping

## Verified handoff

- Source: `src/overlays/ovl_11/ovl_11_func_8010D748.c`.
- Supplied verified source/evidence identity:
  `d582f0a7c96e9178a43875f977b5f12a81fafb8e1cecf2b160be1d52ab98a891`.
- The handoff reports byte identity and a passed full finalize gate. This
  documentation-only review accepts that result without rebuilding or changing
  build inputs.

## New grouping evidence

The [D648 review](ovl_11_func_8010D648-grouping.md) left D748 outside its
candidate caller/helper pair: participation in the fallback chain alone was
insufficient. The verified D748 source now supplies a matching sibling-helper
pattern as well as that call/link relationship:

- D748 and `src/overlays/ovl_11/ovl_11_func_8010D648.c` both derive an attempt
  count from the same object's unsigned halfword at +0xB2 plus
  `func_80012A34(3)`. D748 divides by 100; D648 divides by 50. Both take the
  object and a signed-halfword area, loop over
  `ovl_11_func_80110944(area, mode)`, and report failure when its signed-halfword
  result is -1. Their modes are 2 and 1 respectively. This corroborates a
  local pair of complementary area-action helpers, rather than just two
  arbitrary callees of the same dispatcher.
- `build/ovl_11/asm/nonmatchings/ovl_11_func_8010D314/ovl_11_func_8010D314.s`
  calls D748 at 0x8010D5B8 with the current object and area 5, and at
  0x8010D5F8 with that object and area 1. Both calls participate in the
  failure-tested chain containing D648 and D850.
- `configs/splat/ovl_11.yaml` places D314, D648, D748 and D850 at file
  offsets 0x554F4, 0x55828, 0x55928 and 0x55A30.
  `build/ovl_11/ovl_11.map` gives the gapless sequence
  D314 (0x334 bytes) → D648 (0x100) → D748 (0x108) → D850. D748's target
  assembly at
  `build/ovl_11/asm/nonmatchings/ovl_11_func_8010D748/ovl_11_func_8010D748.s`
  independently supplies its start and size. The reconstruction's separate
  object-file names are not evidence of original TU boundaries.

D748's role is area-selected action-6 dispatch on 0xB8-byte records, choosing
800DF010 or 800E0C24 from flag 0x10000 and returning selector failure.

## Conclusion and limits

Add D748 as a low-confidence same-TU candidate alongside D314/D648 in the
existing object-mode dispatch section. The evidence is the verified sibling
pattern together with the shared caller and gapless link order; it does not
establish an original TU boundary or increase the wider section's confidence.

D748 defines no shared static or global. Its use of the common `D_80074838`
work area is not ownership evidence, and no new declaration-order or
register-variable quirk is established here. Do not extend membership to D850,
80110944, 800DF010 or 800E0C24 from these calls alone.
