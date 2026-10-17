# ovl_11_func_800D0698 grouping evidence

Verified-source/evidence identity supplied by the handoff:
`3829a29a064c6fbd940d393d71ca185cf7e7c6a3e2eeb8d8f94f91ef49bd75a3`.
Matching and the full finalize gate were already complete; this is documentation only.

## Evidence

- `src/overlays/ovl_11/ovl_11_func_800D0698.c` calls
  `ovl_11_func_800D062C(arg0)` on the unchanged record pointer, then uses
  its result and an absolute-delta comparison to select the s16 code at
  record offset +0x22.
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800D062C/ovl_11_func_800D062C.s`
  gives the predecessor a size of 0x6C: 0x800D062C + 0x6C = 0x800D0698.
  It reads the same +0x38/+0x58 record pair as the verified caller, plus
  +0x40/+0x60, and indexes `D_80123A00` using two rank-comparison results.
- `configs/splat/ovl_11.yaml` places these functions at consecutive offsets
  0x1880C and 0x18878. The target assembly at
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800D0698/ovl_11_func_800D0698.s`
  corroborates the call at 0x800D06E0. Thus the call graph and zero-gap
  link order agree, with a shared record-layout tie.

## Inference and limits

This newly verified caller supports a **low-confidence candidate same-TU pair**
with 800D062C. It does not establish original translation-unit boundaries:
a direct call, common record layout and adjacency can cross object boundaries.
No shared static or declaration-order dependency was established here.

The existing 800D0600/800D062C nested-function evidence does not establish
that 800D0698 shares their original TU. Likewise, the next function,
800D075C, is adjacent but has no call/data tie established by this target;
this finding does not merge the pair with the tag-setter/scanner group.
The target's newly assigned jump-table rodata split is not independent
original-TU evidence.
