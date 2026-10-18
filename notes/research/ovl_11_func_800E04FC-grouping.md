# ovl_11_func_800E04FC: candidate allocator/clone pair

## Verified premise

The supplied handoff records byte identity and the full finalize gate passing
for identity `62f407175da5739f415635a5144148975931fce798722f711ef8f7f87ab4cdc5`
(agent origin). Its changed files are
`src/overlays/ovl_11/ovl_11_func_800E04FC.c` and
`include/overlays/ovl_11.h`. The source read for this documentation has SHA-256
`f933cbe2172c123d6e0dbfd74a2bb71c75609b3d46930033a39cfdbbf749351e`.
No solver, rebuild, or build-input edit was performed for documentation.

## New membership evidence

Candidate same-TU members: `ovl_11_func_800E047C` and
`ovl_11_func_800E04FC` (low confidence).

- **Gapless link order.** `configs/splat/ovl_11.yaml` places the two at
  offsets `0x2865C` and `0x286DC`. The predecessor is `0x80` bytes, ending
  exactly at `0x800E04FC`; the clone is `0xAC` bytes, ending at `0x800E05A8`.
- **Shared initializer and complementary roles.** Original code in
  `build/ovl_11/functions/ovl_11_func_800E047C.s` accepts codes
  `0x164–0x165`, allocates through `800D0C34`, resolves the index through
  `800D0D7C`, and calls `800E0D3C` at `0x800E04C0` before tagging the record.
  The verified clone calls the same initializer at `0x800E0538` with
  code `0x164`, on the selected `D_800749F4` destination (stride `0xB8`).
  It then copies source fields `+0x38/+0x3C/+0x40` (last minus `0xC8`),
  `+0x30`, and `+0x22`. Original clone evidence:
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800E04FC/ovl_11_func_800E04FC.s`.

## Limits

This is an adjacency-plus-shared-callee inference, not a proven original TU
boundary. `D_800749F4` is cross-container shared storage, as the existing
accessor entry records; using that array alone cannot establish membership.
The analogous `800DE8A4/800DE924` pair uses a different initializer and remains
separate. No membership is inferred for the distant initializer `800E0D3C`,
the clone's caller `800D09E0`, or the next function `800E05A8`. No static
ownership, declaration-order effect, register-variable quirk, or original
record-type spelling is established.
