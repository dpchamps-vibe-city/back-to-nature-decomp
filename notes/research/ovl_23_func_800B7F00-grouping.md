# ovl_23_func_800B7F00: outer-state callback membership

## Provenance

Documentation-only handoff: the target is supplied as byte-exact and having
passed the full finalize gate. Supplied source/evidence identity:
`1eb65de170b1d479bf3566b365461eae59b6a525b9ea891187b84b3b7dac0883`
(origin: agent). Supplied changed files: `configs/splat/ovl_23.yaml`,
`include/overlays/ovl_23.h`, and
`src/overlays/ovl_23/ovl_23_func_800B7F00.c`. This pass changes only notes
and does not rerun matching.

## New membership evidence

- **Shared outer-state word:** after completing setup, the verified target
  increments the s32 at `D_8006C838 + 0x448C`, which is `D_80070CC4`
  (`0x8006C838 + 0x448C = 0x80070CC4`). The already-recorded counter pair
  `800B80CC`/`800B8104` respectively increments that same word and stores
  0xFF to it. This is the actual selector used by the existing dispatch
  cluster's entry point `800B7EA4`, not merely another field of the large
  engine work area.
- **Callback graph and link order agree:**
  `build/ovl_23/asm/data/3A40.data.s` records `D_800BB9E4` as
  `800B7F00`, `800B8018`, `800B8084`, `800B80CC`, `800B8104`, in precisely
  their consecutive text order. The existing members `800B7EA4` and
  `800B8084` respectively dispatch through this table and its adjacent
  sibling `D_800BB9F8`. The target is the first outer callback and is
  gapless with `800B7EA4`: `0x800B7EA4 + 0x5C = 0x800B7F00`.
  Its 0x118-byte body ends at `800B8018`; it is not directly adjacent to
  the counter pair.
- **Local initialization connection:** the target calls the verified
  `800B81AC` initializer before advancing the outer state. That callee
  resets `D_800BF87C` and initializes its movement parameters, as recorded
  in [the initializer membership note](ovl_23_func_800B81AC-grouping.md).
  This connects the outer callback to the dispatch-index aggregate without
  claiming that the target directly accesses that aggregate.

The ledger already records the target's four display/audio setup calls and
its `D_800BBA3C` switch. Confirming those calls, or assigning the switch's
jump table to its compiled source in splat, is not new TU evidence here.
The addition rests on the shared outer selector, ordered callback graph and
local initialization connection.

## Evidence paths

- `src/overlays/ovl_23/ovl_23_func_800B7F00.c` and its original assembly at
  `build/ovl_23/asm/nonmatchings/ovl_23_func_800B7F00/ovl_23_func_800B7F00.s`:
  initializer call at 0x800B7FC8; outer-state increment at
  0x800B7FF0–0x800B8004.
- `src/overlays/ovl_23/ovl_23_func_800B7EA4.c` and its corresponding original
  assembly: table dispatch indexed by `D_80070CC4`.
- `src/overlays/ovl_23/ovl_23_func_800B80CC.c` and
  `src/overlays/ovl_23/ovl_23_func_800B8104.c`: complementary outer-state
  writes, corroborated by their corresponding original assembly listings.
- `build/ovl_23/asm/data/3A40.data.s`: `D_800BB9E4` at 0x800BB9E4–0x800BB9F4
  and sibling `D_800BB9F8` immediately afterward.
- `configs/splat/ovl_23.yaml`: consecutive text offsets 0x84, 0xE0, 0x1F8,
  0x264, 0x2AC and 0x2E4.

## Membership conclusion and limits

Add only `ovl_23_func_800B7F00` to the existing `D_800BF87C` dispatch-index
candidate cluster, retaining medium confidence. Under that cluster's
same-TU hypothesis, shared selector updates, callback/link order and the
initialization call support this previously unlisted member.

Tables, calls and engine-state writes can cross TUs. This does not establish
original boundaries, static ownership, original aggregate declarations or a
compiler/register-variable quirk. Do not merge the separately recorded
counter, initializer and display-setup groups wholesale, or add other table
entries solely on this function's evidence.
