# ovl_11_func_800C162C — adjacent frame-driver pair

## Verified handoff

The supplied handoff reports byte-exact output and a passed full finalize gate
for source/evidence identity
`829ba59c3a41b0a5450b9caf01f36882760b44ec1a225b425f7750370748a6df`
(origin: agent). Its changed-file list names `include/globals_override.h` and
`src/overlays/ovl_11/ovl_11_func_800C162C.c`. This notes-only review accepts
that gate; it does not rerun matching or change build inputs.

## New grouping evidence

- **Call graph and gapless link order agree.** The original
  `build/ovl_11/functions/ovl_11_func_800C14A0.s` declares size 0x18C:
  `0x800C14A0 + 0x18C = 0x800C162C`. It directly calls the target at
  0x800C155C. `configs/splat/ovl_11.yaml` likewise places the pair
  consecutively at file offsets 0x9680 and 0x980C.
- **The predecessor has a complementary frame path.** Its normal branch
  calls the target unless `D_8007AFF0+0x254A0` is 5, 0x18 or 0x19. Its
  alternate branch calls 800DB674, 800DB78C and func_8001B5DC, then calls
  800DCC5C and 800DB904 when bit 0x800000 of `D_8006C838+0xC` is clear.
  The verified target and its original assembly
  (`build/ovl_11/functions/ovl_11_func_800C162C.s`) contain the same ordered
  subsequence and flag guard within the larger frame-update pipeline. This
  corroborates the local caller/body relationship, rather than merely a
  shared engine-buffer reference.

## Membership inference and limits

Low-confidence candidate same-TU membership for **800C14A0 / 800C162C**.
The premise is that the immediately adjacent caller and frame-update body
reflect one original source unit; separately linked adjacent units remain
possible. No original TU boundary, shared static, declaration-order effect or
register-variable quirk is established.

`D_80128B60` is absolute-addressed and referenced only by the target in the
original `build/ovl_11/functions/ovl_11_func_*.s` corpus. It supplies no
shared-global or data-ownership bridge. The broad `D_8006C838`/`D_8007AFF0`
buffers and reusable scratchpad-stack idiom do not establish membership on
their own. No additional callee, link neighbour or distant state-machine
cluster is merged into this pair.
