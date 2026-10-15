# ovl_11_func_801014F8: state-block submenu grouping evidence

Documentation-only review of the verified source. The supplied handoff reports
byte-exact matching and a passing full finalize gate, with source/evidence
identity `d9daf480a99701632f1ac893d5bc7931eadcbe10403d250bab8b532b8dd81418`
(origin: agent). Its changed files are `include/globals_override.h`,
`include/overlays/ovl_11.h` and
`src/overlays/ovl_11/ovl_11_func_801014F8.c`; none was modified in this review.
Matching/build checks were not repeated.

## New evidence relative to the ledger

The existing low-confidence D_801273E4/D_801273F4 handler/display cluster
mentions `801014F8` only as the successor of member `801014A4`, not as a member.
The verified source supplies a specific shared-state producer/consumer tie:

- `801014F8` cycles the menu index `D_801273E9` through five choices and
  toggles bit `selection + 7` in the halfword `D_801273EE`. Those five bits
  are exactly mask `0xF80`, tested by both existing display members
  `80101814` and `80101964` for their final aggregate slot. Cancellation
  writes `D_801273E4 = 3`, also the state transition used by `801014A4`.
  Reset member `800FFDCC` clears all three state variables. The new header
  views `D_801273E9_selection`, `D_801273EE_flags` and `D_801273E4_state`
  refer to those original symbols, not separate storage.
- Link order is gapless locally: `801014A4 + 0x54 = 801014F8`,
  `801014F8 + 0x31C = 80101814`, and `80101814 + 0x150 = 80101964`.
  Splat offsets `0x49684`, `0x496D8`, `0x499F4`, `0x49B44` agree with the
  function sizes in `build/ovl_11/functions.csv`.
- The original caller `80100FFC` renders the display pair at `801010AC`
  and `801010B4`, then dispatches state 4 to `801014A4` at `801010DC` and
  state 5 to `801014F8` at `801010F8`. Thus the linked neighbours are
  complementary substates and displays of the same caller, not merely
  unrelated users of an engine API. There is no direct call between
  `801014F8` and either display function.

## Evidence paths and limits

- `src/overlays/ovl_11/ovl_11_func_801014F8.c`
- `src/overlays/ovl_11/ovl_11_func_801014A4.c`
- `src/overlays/ovl_11/ovl_11_func_80101814.c`
- `src/overlays/ovl_11/ovl_11_func_80101964.c`
- `src/overlays/ovl_11/ovl_11_func_800FFDCC.c`
- `include/globals_override.h` (symbol aliases)
- `build/ovl_11/asm/nonmatchings/ovl_11_func_80100FFC/ovl_11_func_80100FFC.s`
- `build/ovl_11/functions.csv`
- `configs/splat/ovl_11.yaml`

Assuming the specialized shared-state relationship and corroborated call/link
locality reflect original source organization, add `801014F8` as a candidate
same-TU member of the existing cluster. Keep its low confidence: these globals
are not proven private statics, and neither original TU boundaries nor a
gapless run across the whole cluster are established. This review supplies no
new declaration-order or register-variable evidence and does not merge the
cluster with the separate D_801273DC handler run.
