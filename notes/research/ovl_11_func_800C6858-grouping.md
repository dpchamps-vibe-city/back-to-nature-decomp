# ovl_11_func_800C6858: adjacent item/state caller and helper

## Verified anchor

Supplied source/evidence identity:
`d37bcf7eb732a6d32fcc73550552dd0eda8d0f8017db6ea8a012ef42732e6f97`
(origin: agent). The handoff reports byte identity and a passed full finalize
gate, listing `notes/file-groupings.md` and
`src/overlays/ovl_11/ovl_11_func_800C6858.c` as changed files. This notes-only
review accepts that verification without modifying or re-verifying build inputs.

## Membership evidence

- **Call graph agrees with immediate link adjacency.**
  `configs/splat/ovl_11.yaml` places 800C6858, 800C69B8 and existing member
  800C6A24 at offsets 0xEA38, 0xEB98 and 0xEC04. The original assembly in
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800C6858/ovl_11_func_800C6858.s`
  gives the target size 0x160, ending exactly at 800C69B8. Its call at
  800C6988 passes the same saved object pointer to that immediate successor
  and uses the helper's result to decide whether to call 800CB4E0. The
  helper's original assembly at
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800C69B8/ovl_11_func_800C69B8.s`
  gives size 0x6C, ending exactly at 800C6A24.
- **The local pair repeats a specific state/sound idiom.** The target's
  state-0x30 branch calls 800C70CC with `(object, 0x31, 5, -2)`, then
  func_8001FABC(0x4A), and returns success. Its adjacent helper tests the
  flags of the item indexed by s16@+0x8A, calls 800C70CC with
  `(object, 3, 5, -2)`, then the same sound and success return. These
  original call sequences corroborate the caller/helper relationship;
  reconstructed typedef reuse is not the evidence.
- **Both connect to the existing state-switch family.** The shared callee
  800C70CC belongs to the recorded helper/dispatcher run. The target also
  calls it with state 0x33, animation `u16@+0x38 + 0x1D` and argument -2,
  and updates the same +0x6C flag word. The gapless successor 800C6A24
  likewise uses 800C70CC and the +0x38/+0x3C state fields; its independently
  documented local relationship is in
  [the timer/state helper research](ovl_11_func_800C6A24-grouping.md).

## Role and limits

800C6858 copies the pending +0xF0 item into a selected six-byte slot, clears
the pending item, and selects follow-up states or delegates to the adjacent
item/state helper, falling back to 800CB4E0 when that helper rejects it.

Retain the target in the existing **medium-confidence candidate** 0x38/0x3C
state-switch family. The conditional premise is that the immediate caller/helper
arrangement, repeated state/sound sequence and connection to the gapless local
run reflect one original translation unit. The local pair is better supported
than the broader family's single-TU interpretation. Shared public callees or
object layout alone would not suffice. No original static linkage, global
ownership, declaration-order fingerprint or TU boundary is established, and
this does not assign distant item/sound callees to the same TU.
