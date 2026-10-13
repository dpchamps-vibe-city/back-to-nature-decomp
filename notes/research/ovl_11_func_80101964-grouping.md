# ovl_11_func_80101964: display-pair grouping evidence

Documentation-only review of the verified source. The supplied handoff reports
byte-exact matching and a passing full finalize gate, with source/evidence
identity `36265444827bbc3dcb89c2d933cf63f93df0c1f7c5a0afaa72a343a16ce35fad`
and changed file `src/overlays/ovl_11/ovl_11_func_80101964.c`. Its source SHA-256
is `c32b766c446f78ecc7058dbbb2c2752f6e5014593d2d7681271cc4b3911ffea4`, matching
the verification record in `run_output/autoloop/state.json`. Matching/build
checks were not repeated; this review changes only notes.

## New evidence relative to the ledger

The existing D_801273E4/D_801273F4 handler/display cluster already includes
`80101814`, but not its companion `80101964`.

- Both verified sources read seven consecutive u16 IDs from `D_801273F4` and
  the flag halfword `D_801273EE`. They share the seven-slot loop plus final
  ID `0x79` slot, x origin `0x84`, `0x22` coordinate step, `0xEB` row-wrap
  threshold, and final `0xF80` state-mask test. Both call `800F6D14` for an
  icon and `800F7010` for the alternative, using the same object's +0x68
  and +0x6C views. The companion starts at y=`0x5E` rather than `0x14`,
  gates icons on the byte at `D_8006C838+0x44CC`, and conditionally calls
  `800F6F7C` with object+0x60 when the state flags are set.
- Original link order is gapless: `80101814 + 0x150 = 80101964`. The latter
  occupies 0x1C4 bytes, ending at `80101B28`; that endpoint alone does not
  establish additional membership.
- Original caller `80100FFC` calls `80101814` at `801010AC`, immediately
  followed by `80101964` at `801010B4`. The paired call order agrees with
  link order and the complementary grid roles. Neither display function
  calls the other.

## Evidence paths and limits

- `src/overlays/ovl_11/ovl_11_func_80101964.c`
- `src/overlays/ovl_11/ovl_11_func_80101814.c`
- `build/ovl_11/asm/nonmatchings/ovl_11_func_80101814/ovl_11_func_80101814.s`
- `build/ovl_11/asm/nonmatchings/ovl_11_func_80101964/ovl_11_func_80101964.s`
- `build/ovl_11/asm/nonmatchings/ovl_11_func_80100FFC/ovl_11_func_80100FFC.s`
- `configs/splat/ovl_11.yaml` (consecutive offsets 0x499F4, 0x49B44, 0x49D08)

Assuming the shared state, specialized draw pattern and corroborated call/link
order reflect original source organization, the display pair is a
medium-confidence same-TU candidate within the existing low-confidence
cluster. This adds `80101964` without upgrading the broader cluster or claiming
original TU boundaries. Shared engine globals/callees alone are insufficient;
there is no new private-static, declaration-order or register-variable evidence.
