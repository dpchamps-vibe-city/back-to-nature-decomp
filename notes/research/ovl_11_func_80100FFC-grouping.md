# ovl_11_func_80100FFC: state-block menu-controller membership

## Evidence identity

Documentation-only review of the supplied byte-exact, finalize-gated
source/evidence identity
`ccea8cd07c1d6f0ccb7829015d4fc0ca16707616359c72f165757297b3d9047f`
(origin: agent). The handoff lists `include/overlays/ovl_11.h` and
`src/overlays/ovl_11/ovl_11_func_80100FFC.c`; neither was edited here. The
reviewed source SHA-256 is
`0e290126079a22ad4f03bcaaa16fa6b2e8582da6f1c4901fdebec64ddde5b2f2`,
matching the target's verification inputs in `run_output/autoloop/state.json`.
Matching/build checks were not repeated.

## New membership evidence

The ledger already lists `80100FFC` in the low-confidence `D_80070D04`
reader run. Earlier [display-pair](ovl_11_func_80101964-grouping.md) and
[submenu](ovl_11_func_801014F8-grouping.md) notes use it as a caller witness,
but do not establish its own membership in the state-block handler/display
cluster. Its verified source supplies complementary producer/consumer ties:

- **Selected-ID handoff:** the controller updates signed slot selector
  `D_801273E7`, writes `D_801273E4 = 4`, and on that state passes the selector
  to its immediate link successor `801014A4`. That member reads the selected
  `D_801273F4` ID and returns the state to 3. Original instructions show the
  selector load at `801010D8`, call at `801010DC`, and state-4 store at
  `80101398`.
- **Shared flag producer and displays:** for slots below 7, the controller
  sets/clears the corresponding bit in `D_801273EE` (original stores at
  `80101404` and `80101464`), and can clear the whole halfword at `80101308`.
  Those are the seven individual bits consumed by both existing display
  members `80101814` and `80101964`, called together at `801010AC` and
  `801010B4`. For the aggregate slot, the controller enters state 5 (store
  at `80101478`) and dispatches `801014F8` at `801010F8`; that submenu edits
  bits 7–11 of the same halfword. Both displays and the controller's
  `80101B28` status helper test those aggregate bits as mask `0xF80`.
  The header aliases ending in `_state`/`_flags` and `Ovl11_D_801273E7`
  name the original symbols, not separate storage.
- **Call graph corroborates gapless link order:**
  `80100FFC + 0x4A8 = 801014A4`,
  `801014A4 + 0x54 = 801014F8`,
  `801014F8 + 0x31C = 80101814`, and
  `80101814 + 0x150 = 80101964`. The controller therefore immediately
  precedes the detail/submenu/display group it drives, rather than merely
  calling distant users of a common engine API. Splat offsets
  `0x491DC`, `0x49684`, `0x496D8`, `0x499F4`, `0x49B44` agree with the
  extents in `build/ovl_11/functions.csv`.

## Evidence paths and limits

- `src/overlays/ovl_11/ovl_11_func_80100FFC.c`
- `src/overlays/ovl_11/ovl_11_func_801014A4.c`
- `src/overlays/ovl_11/ovl_11_func_801014F8.c`
- `src/overlays/ovl_11/ovl_11_func_80101814.c`
- `src/overlays/ovl_11/ovl_11_func_80101964.c`
- `src/overlays/ovl_11/ovl_11_func_80101B28.c`
- `src/overlays/ovl_11/ovl_11_func_80101B84.c`
- `include/globals_override.h` (original-symbol aliases)
- `build/ovl_11/asm/nonmatchings/ovl_11_func_80100FFC/ovl_11_func_80100FFC.s`
- `build/ovl_11/functions.csv`
- `configs/splat/ovl_11.yaml`

Assuming this specialized shared-state ownership and corroborated call/link
locality reflect original source organization, add `80100FFC` as a candidate
same-TU member of the existing state-block cluster. Retain low confidence:
the globals are not proven private statics, and no original TU boundary,
declaration-order effect or shared register-variable quirk is established.
Its existing membership in the `D_80070D04` reader run does not establish that
that entire run shares a TU with the state-block cluster. This review does
not merge either broader family or the separate `D_801273DC` handler run.
