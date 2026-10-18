# ovl_21_func_800B8F98 — record/audio completion handler

## Provenance

The documentation handoff reports byte-exact source and a passed full finalize
gate, with source/evidence identity
`8318af05be7eb376870eccce82cbeb42e4893fafdf8a960ed157b412248fc3f7`.
Its changed files are `include/overlays/ovl_21.h` and
`src/overlays/ovl_21/ovl_21_func_800B8F98.c`. This note does not rerun matching
or change build inputs.

## New grouping evidence

- The verified source walks the six `D_800C0448` records, updating the
  halfwords at record +0x18/+0x2E. It calls established cluster members
  `ovl_21_func_800B990C`, `ovl_21_func_800BA4C0`, and
  `ovl_21_func_800B9844`. The shared record storage is also initialized by
  `src/overlays/ovl_21/ovl_21_func_800B836C.c` and consumed by
  `src/overlays/ovl_21/ovl_21_func_800BA510.c`.
- Its audio-completion test uses `D_800C0448 +0x986` as the index into
  `D_800BCC60`, calls `SpuGetKeyStatus`, and tests the counter at +0x988
  against 150. This is the same global/field/SDK cluster used by
  `src/overlays/ovl_21/ovl_21_func_800B9178.c` and the already-recorded
  800B871C/800B88A0/800B92C8/800B93F0 handlers. The extracted target at
  `build/ovl_21/asm/nonmatchings/ovl_21_func_800B8F98/ovl_21_func_800B8F98.s`
  confirms these accesses and the final +0x988 reset/state-9 write.
- `build/ovl_21/asm/data/3C64.data.s` places this function in slot 8 of
  `D_800BCB6C`, between 800B8B3C and 800B90C4. The matched dispatcher
  `src/overlays/ovl_21/ovl_21_func_800B806C.c` indexes that table with
  `D_800C0448[0]`. `configs/splat/ovl_21.yaml` gives those same consecutive
  handlers at offsets 0xD1C, 0x1178, and 0x12A4; the target's 0x12C-byte
  body ends exactly at 800B90C4. Thus table order and link order agree.
  800B9178 is not the immediate successor: 800B90C4 intervenes.

## Membership conclusion and limits

Add only `ovl_21_func_800B8F98` to the existing `ovl_21` D_800C0448
state-pair cluster, as its six-record cooldown/audio-completion handler,
which advances the outer state to 9 when completion probes pass. The shared
record/audio cluster, calls into established members, and ordered dispatcher
table support membership under the existing same-TU hypothesis. Confidence
remains medium: shared globals and calls can cross TUs. No TU boundary,
additional neighbour membership, or data ownership is established.
