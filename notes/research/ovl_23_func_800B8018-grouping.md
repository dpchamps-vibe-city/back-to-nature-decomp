# ovl_23_func_800B8018: outer-state callback membership

## Provenance

Documentation-only handoff: the target is supplied as byte-exact and having
passed the full finalize gate. Supplied source/evidence identity:
`c38482375b231877956462cfed85571497c726cf06618dbba8d9c732fc9edfba`
(origin: agent). Supplied changed files: `include/overlays/ovl_23.h` and
`src/overlays/ovl_23/ovl_23_func_800B8018.c`. This pass changes only notes
and does not rerun matching.

## New membership evidence

- **Shared outer selector:** the verified target conditionally increments
  the s32 at `D_8006C838 + 0x448C = D_80070CC4`. Existing dispatch-cluster
  member `800B7F00` advances that same word, and `800B7EA4` uses it to
  select the outer callback. This is a specific shared state transition,
  not merely use of the large resident work-area base.
- **Callback graph and gapless link order agree:** `D_800BB9E4[1]` points
  to the target, between `800B7F00` and `800B8084`, both existing members.
  Their text boundaries agree exactly: `0x800B7F00 + 0x118 = 0x800B8018`
  and `0x800B8018 + 0x6C = 0x800B8084`. The verified target therefore fills
  the intervening outer-callback slot and text interval.
- **Supporting transition idiom:** the target and separately recorded
  `800B8104` poll `func_80013394() == 1` and `func_8001FE6C() == 0`, count
  successes with byte truncation, and act only when the count is 2.
  Their original assembly confirms the same guard sequence. This supports
  the outer-state connection but is not evidence of a compiler or
  register-variable quirk.

## Evidence paths and limits

- `src/overlays/ovl_23/ovl_23_func_800B8018.c` and
  `build/ovl_23/asm/nonmatchings/ovl_23_func_800B8018/ovl_23_func_800B8018.s`:
  guard at 0x800B8030–0x800B8058; selector increment at
  0x800B8060–0x800B8070; original size 0x6C.
- `src/overlays/ovl_23/ovl_23_func_800B7F00.c`: same selector increment.
- `build/ovl_23/asm/nonmatchings/ovl_23_func_800B7EA4/ovl_23_func_800B7EA4.s`:
  outer dispatch indexed by `D_80070CC4` at 0x800B7EC0–0x800B7EE4.
- `build/ovl_23/asm/nonmatchings/ovl_23_func_800B8104/ovl_23_func_800B8104.s`:
  matching polling guard at 0x800B810C–0x800B8134.
- `build/ovl_23/asm/data/3A40.data.s`: ordered outer callbacks at
  `D_800BB9E4`, including the target at 0x800BB9E8.
- `configs/splat/ovl_23.yaml`: consecutive text entries at offsets 0xE0,
  0x1F8 and 0x264; corroborates layout, not original TU boundaries.

Add only `ovl_23_func_800B8018` to the existing `D_800BF87C` dispatch-index
candidate cluster, retaining medium confidence under its same-TU hypothesis.
The target does not directly access `D_800BF87C`; its calls to `800B823C`
and `800B9310` do not justify merging the separately recorded initializer,
render-driver or counter groups. Shared state, tables and polling idioms
can cross TUs; no exact original boundary or static ownership is established.
