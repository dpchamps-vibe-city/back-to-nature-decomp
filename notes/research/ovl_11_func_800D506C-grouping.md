# ovl_11_func_800D506C grouping evidence

Verified source/evidence identity supplied by the handoff:
`69f8b1b2da76b28422030a7025aa07a2badae9893dd8d445068b83ab1c8d0897`.
The handoff reports byte identity and a passing full finalize gate for
`src/overlays/ovl_11/ovl_11_func_800D506C.c`; this notes-only review does
not repeat verification or change build inputs.

## New membership evidence

The verified body supports a low-confidence candidate same-TU pair with
its immediate predecessor, `ovl_11_func_800D4EA0`, within the existing
D_800B8EC0 state-dispatch group:

- **Complementary progress-flag use:** the verified function clears
  0x01000000 and 0x00800000 from `D_8006C838+0x44F8` in state 0.
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800D4EA0/ovl_11_func_800D4EA0.s`
  sets 0x01000000 in that same word at 0x800D4F1C–0x800D4F28, also in
  state 0. The target's original assembly corroborates its clears at
  0x800D510C–0x800D5124 in
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800D506C/ovl_11_func_800D506C.s`.
- **State-transition edge agrees with link order:**
  `build/ovl_11/asm/data/104C.rodata.s` places the predecessor and target
  in consecutive `D_800B8EC0[0x12]` and `[0x13]` slots. At 0x800D5030,
  the predecessor calls `800D3104` with state 0x13 on the same object
  (its alternate path selects 0x14). The assembly for `800D3104` stores
  this selector at object +0x26 and resets +0x28; the assembly for
  `800D3048` subsequently dispatches `D_800B8EC0[object->state]` on that
  object. This is an indirect state-transition relationship, not a direct
  call from the predecessor to the target.
- **Gapless code adjacency:** `configs/splat/ovl_11.yaml` places
  `800D4EA0` at file offset 0x1D080 and `800D506C` at 0x1D24C, a 0x1CC
  difference. The predecessor's original assembly is 0x1CC bytes and ends
  at 0x800D506C. Thus code order and the state-transition edge agree.
- **Corroborating placement/update convention:** both state-0 paths pass
  a padded by-value vector with x = -0x1324 and y = 0 to `800D05D0`
  (different z values), set object flag 0x800, and advance the signed
  substate at +0x28. Both then call `800D425C` and test object +0x34 bit
  0x2000 before advancing. This supports the pair beyond table membership
  alone, but the helpers and object layout are not private-TU evidence.

## Limits

The inference depends on the original code/table layout represented by the
assembly and configuration, together with the complementary flag use and
state transition. It does not recover a file boundary or establish ownership
of the main-binary progress buffer. No shared static, declaration-order
effect or register-variable quirk was established. Modern per-function
source/object boundaries are not original translation-unit boundaries.
This records only the candidate pair; it does not add every dispatch-table
handler, absorb adjacent `800D5224`, or widen the scattered D_8006C838
flags/state-buffer cluster.
