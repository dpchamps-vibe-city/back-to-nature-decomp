# ovl_11_func_800CB618: extension of the global-object state/flag run

## Verified anchor

Supplied source/evidence identity:
`70bf3770030a79690a3d02f6556a4005b0c4c8a44f16f99dfdf6276b0180a83a`
(origin: agent). The handoff reports byte identity and a passed full finalize
gate, with `include/globals_override.h` and
`src/overlays/ovl_11/ovl_11_func_800CB618.c` as its changed files. This
notes-only review accepts that verification without changing build inputs.

## New membership evidence

Extend the existing [800CB4E0 / 800CB540 candidate pair](ovl_11_func_800CB4E0-grouping.md)
with 800CB618, rather than merging it into the distant state-switch family.

- **Gapless link order.** `configs/splat/ovl_11.yaml` places 800CB540 at
  file offset 0x13720 and 800CB618 at 0x137F8. The original target assembly
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800CB540/ovl_11_func_800CB540.s`
  gives 800CB540 size 0xD8, ending exactly at 800CB618.
- **A direct call back into the pair.** The verified source and
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800CB618/ovl_11_func_800CB618.s`
  show that the flag-clear branch calls 800CB4E0 at 800CB640. This ties the
  newly adjacent member to the existing reset helper, not just to an overlay-wide
  dispatcher.
- **Repeated object/dispatcher/flag-update pattern.** Both 800CB540 and
  800CB618 retain `&D_80071A00` in `$s0`, pass it to 800C70CC with final
  argument -2, then update its word at +0x6C. At 800CB69C, 800CB618 selects
  state 0xC with animation `u16@+0x38 + 0x1D`, then clears flag 0x8000.
  The predecessor calls the same dispatcher at 800CB5DC with an animation
  derived from that same halfword (work-area +0x5200 equals object +0x38),
  then sets flags 0x8004. The reset helper supplies the third instance of
  this shared-object dispatcher-followed-by-flag-update idiom.

## Inference and limits

The contiguous placement, direct reset call and repeated concrete object/field
usage support a **low-confidence candidate same-TU run** of 800CB4E0 /
800CB540 / 800CB618. This assumes the local repetition reflects one source
unit rather than adjacent units using public helpers. No private symbol,
original declaration order or distinctive register-variable quirk is proven;
retaining the object in `$s0` alone is not such a quirk. Unlike the preceding
pair, 800CB618 does not derive the work-area sub-base.

The call to 800C9D98 and the eight-record view added to the override header
explain the selected-record access, but do not establish that the distant
wrapper belongs to this TU. Nor does this evidence establish membership of
800CB6C8, either outer TU boundary, or a merger with the broader global-object
or state-switch groups. The reconstructed typedef is not original-TU evidence.
