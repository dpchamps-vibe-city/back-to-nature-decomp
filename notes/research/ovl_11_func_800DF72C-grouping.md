# ovl_11_func_800DF72C — predicate-run grouping

Verified source/evidence identity supplied by the finalization handoff:
`9527552a3cae9ff897b824235cd83d0841c1abb31b0ab64fd1b3929e539a58c1`.
The handoff reports byte identity and the full finalize gate, with changes to
`include/game_types.h` and `src/overlays/ovl_11/ovl_11_func_800DF72C.c`.
This notes-only review did not rerun matching or change build inputs.

## New evidence for existing candidate membership

DF72C was already listed as a caller in the `800DF4F0` predicate run. Its
verified body adds a shared handler idiom with neighbours DF614 and DF6A8:
all three reject an object when `ovl_11_func_800DF4F0` returns zero, reset it
through `ovl_11_func_800D049C`, and finish by calling
`ovl_11_func_800D12A0(9)` only when the object's `s16@+0x30` equals the far
state halfword at `(char *)&D_8007AFF0 + 0x25476`. DF72C also tests the same
`u16@+0xAE` latch that DF614 consumes and clears.

This agrees with gapless link order: DF614 occupies 0x94 bytes and ends at
DF6A8; DF6A8 occupies 0x84 and ends at DF72C; DF72C occupies 0x120 and ends at
DF84C. The splat file offsets are respectively 0x277F4, 0x27888, 0x2790C and
0x27A2C. Thus the newly confirmed reset/state-gate sequence reinforces the
existing medium-confidence predicate-run grouping, rather than relying only
on the already recorded common predicate call.

DF72C's distinct role is to reserve a free record for a code-0x162 object:
it obtains a pair slot and a free record, stores the two record indices via
`800D0DB0`, marks the free record with flag 0x02000000, sets the object's
`+0xB2` latch, and performs the shared reset/state-gated notification tail.

**Limits:** the far-state base and engine helpers are widely used; none alone
establishes same-TU ownership. The combined neighbour idiom and link order
support candidate membership, not an original TU boundary or shared-static
proof. The new shared types in `game_types.h` are reconstruction views, not
original declaration-order evidence. Do not merge the distant record-array
accessors into this run merely because DF72C calls them.

## Evidence paths

- `src/overlays/ovl_11/ovl_11_func_800DF72C.c`: verified reservation handler,
  latch tests, predicate, reset and state-gated notification.
- `src/overlays/ovl_11/ovl_11_func_800DF614.c` and
  `src/overlays/ovl_11/ovl_11_func_800DF6A8.c`: neighbouring predicate/reset/
  notification sequence and DF614's complementary +0xAE latch use.
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800DF72C/ovl_11_func_800DF72C.s`:
  original call sequence, far-base relocation and 0x120 extent.
- `configs/splat/ovl_11.yaml`: consecutive DF614/DF6A8/DF72C/DF84C entries.
