# ovl_17_func_800B91B8 — queue-renderer membership evidence

The agent handoff reports byte identity and a passed full finalize gate.
Verified source/evidence identity:
`d850ee4c5800455384a68e8fc23942ff3bf5e62a164bb05a83d86730d5087f45`.
This notes-only review does not rerun matching or alter build inputs.

## Membership evidence

The verified body supplies the consumer side of the existing queue
initializer/append pair: it scans 90 stride-8 entries, reads the s16 at
D_800BD848+0x2DC, skips the initializer's -1 sentinel, and uses the paired
s32 at +0x2D8 to position a sprite. These are precisely the fields written
by 800B90F8 and 800B9158, not merely another use of the overlay-wide base.
The original text extents form a gapless initializer/writer/renderer run:
`0x800B90F8 + 0x60 = 0x800B9158`,
`0x800B9158 + 0x60 = 0x800B91B8`, and
`0x800B91B8 + 0xF8 = 0x800B92B0` (the existing display-hub member).

The call graph connects this local queue run to the established display
cluster: 800B8294 calls the initializer, 800BAC24 calls the append routine,
and 800B8898/800B8C38/800B8D88/800B8F28 call this renderer. Its sprite is
D_800BD848+0x22C, also addressed as D_800BDA74 by existing member 800BAF50;
both render through func_80015EE8. This corroborates the queue's display
role rather than introducing an independent SDK or register-variable quirk.

The specific producer/consumer fields together with uninterrupted link
order support adding only 800B91B8 to the existing low-confidence
D_800BD848 record-area/caller-hub group. If that local queue-helper run
belongs to one original translation unit, the renderer plausibly belongs
with its initializer and writer. These relationships can cross translation
units; no original TU boundary or confidence upgrade is established, and
modern per-function source splits and reconstructed types are not evidence.

## Evidence

- Verified body: `src/overlays/ovl_17/ovl_17_func_800B91B8.c`.
- Original queue loads, sprite argument and extent:
  `build/ovl_17/asm/nonmatchings/ovl_17_func_800B91B8/ovl_17_func_800B91B8.s`
  (loads at 800B91E0/800B9200, sprite address at 800B9268).
- Queue producers and extents: `src/overlays/ovl_17/ovl_17_func_800B90F8.c`,
  `src/overlays/ovl_17/ovl_17_func_800B9158.c`, and their corresponding
  `build/ovl_17/asm/nonmatchings/` assembly files.
- Link order: `configs/splat/ovl_17.yaml`, text offsets
  0x12D8/0x1338/0x1398/0x1490.
- Cluster callers: `src/overlays/ovl_17/ovl_17_func_800B8294.c`,
  `ovl_17_func_800BAC24.c`, `ovl_17_func_800B8898.c`,
  `ovl_17_func_800B8C38.c`, `ovl_17_func_800B8D88.c` and
  `ovl_17_func_800B8F28.c` in the same directory; their original assembly
  call sites agree under `build/ovl_17/asm/nonmatchings/`.
- Shared sprite setup/render: `src/overlays/ovl_17/ovl_17_func_800BAF50.c`.
