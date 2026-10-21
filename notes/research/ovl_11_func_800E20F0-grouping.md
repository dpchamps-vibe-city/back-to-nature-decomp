# ovl_11_func_800E20F0 — candidate 0xB4-entry update-run member

## Verified premise

The supplied handoff reports byte identity and a passed full finalize gate
for source/evidence identity
`f434ae51a4468ca75f0d9a2ddb97080ba7acf0f6af034b9e4049827911ebfc7b`,
origin agent, with changes to `include/globals_override.h` and
`src/overlays/ovl_11/ovl_11_func_800E20F0.c`. This notes-only review did not
rerun matching or edit build inputs.

## New evidence for candidate membership

E20F0 fits the existing low-confidence `D_800742EC` 0xB4-entry
scan/count/init group, specifically its already-listed E21E4 updater:

- **Shared record protocol:** on type 0x109 records, E20F0 increments the
  halfword at +0xAC and sets flag 0x1 at +0x34 when that counter is at least
  five. E21E4 tests that same flag to choose the +0x16 energy adjustment,
  then clears both +0xAC and flag 0x1 in its tail. Both also select behavior through
  the signed halfword at +0x30. This is a complementary counter/flag
  protocol, not merely use of the widely shared main-RAM state buffer.
- **Neighbour clamp idiom in original code:** E20F0 at 0x800E2184–0x800E21A8
  subtracts five from the unsigned halfword at +0x16, preserves the full
  result, stores it, tests its signed low half via `sll 16` / `bgez`, and
  stores either the preserved result or zero. E21E4 repeats that sequence
  at 0x800E2400–0x800E2428 with a subtraction of twenty. The shared
  store-before-clamp construction is visible in original instructions;
  the reconstructed `checkedEnergy` local name is not independent evidence.
- **Caller and link-order corroboration:** CEBC8 calls E20F0 on ten
  0xB4-stride records beginning at `D_8006C838 + 0x7AB4` (`D_800742EC`).
  Its gapless successor CECB0 calls E21E4 over the same pool, with an
  additional flag-based eligibility case. The handlers themselves are
  gapless: `800E20F0 + 0xF4 = 800E21E4`, with splat offsets 0x2A2D0 and
  0x2A3C4. E20F0 also directly follows the group's E20B8 count helper.
  The [existing sweep-run note](ovl_11_func_800CECB0-grouping.md) records
  those callers' complementary pool-pass sequence.

Together these support adding E20F0 as a **low-confidence candidate same-TU
member** beside E21E4. The premise is that the adjacent, complementary
record handlers and their repeated clamp idiom originated together. There
is no proven private static, GP ownership, declaration-order effect or
original TU boundary. Separate adjacent TUs remain possible. The caller
sweep run is not merged into this group, and neither `D_8006C838` nor the
modern `Ovl11E20F0StateView` alone establishes membership.

## Evidence paths

- `src/overlays/ovl_11/ovl_11_func_800E20F0.c`: verified update body.
- `src/overlays/ovl_11/ovl_11_func_800E21E4.c`: neighbouring updater.
- `include/globals_override.h`: `Ovl11E20F0StateView` access-width evidence.
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800E20F0/ovl_11_func_800E20F0.s`:
  original counter/flag update, clamp and 0xF4 extent.
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800E21E4/ovl_11_func_800E21E4.s`:
  original energy clamp, flag consumer and counter/flag resets.
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800CEBC8/ovl_11_func_800CEBC8.s`
  and `build/ovl_11/asm/nonmatchings/ovl_11_func_800CECB0/ovl_11_func_800CECB0.s`:
  corresponding pool walks and handler calls.
- `configs/splat/ovl_11.yaml`: original code-order offsets for both pairs.
