# ovl_11_func_800BD6A8 — CD asset-load run evidence

Verified handoff identity:
`098aa0d57858a5bc5823a030c17967982d889a662b08cf31acf52c0ea317d672`
(origin: agent; changed source: `src/overlays/ovl_11/ovl_11_func_800BD6A8.c`).
The handoff reports byte identity and a passed full finalize gate; this is a
notes-only review, not a new verification run.

## New grouping evidence

The verified source supports its existing candidate membership in the
`ovl_11` CD asset-load init run; confidence remains **medium**.

- `src/overlays/ovl_11/ovl_11_func_800BD6A8.c` and
  `src/overlays/ovl_11/ovl_11_func_800BD5A8.c` share the indexed loader
  pattern: adjacent ROM boundary words give the read range, a counted
  word-offset list gives the copy length, and both load through
  `func_80014BCC(0, start, end-start, 0, D_8005E3B0+0x4290)`, copy the
  leading block, then call `func_8001719C` on the remaining buffer.
  Their boundary/offset tables are distinct, not shared statics.
- `src/overlays/ovl_11/ovl_11_func_800BDA70.c` shares the target's
  `D_8009CBF8` copy destination and load/copy/`func_8001719C` sequence.
  Thus BDA70's render-context source buffer differs, but its destination
  is not separate from the run's global cluster. Both functions also use
  `DrawSync(0)` before loading (conditional in BD6A8).
- Retail call evidence in
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800DDDFC/ovl_11_func_800DDDFC.s`
  shows BD5A8 and BD6A8 called consecutively at `0x800DE030` and
  `0x800DE044`, selecting their indices from adjacent object halfwords
  at +0xA and +0xC. This corroborates their paired loader roles.
- `configs/splat/ovl_11.yaml` offsets `0x5788`, `0x5848`, `0x5888`,
  `0x5ABC` and `build/ovl_11/ovl_11.map` text entries corroborate the
  gapless BD5A8 → BD668 → BD6A8 → BD8DC order. The map's per-function
  objects are reconstruction units, not proof of original TU boundaries.

## Limits

Shared loaders, destination state, caller pairing and retail text adjacency
strengthen the existing hypothesis, but do not establish original file-local
ownership. No new TU boundary or confidence increase is warranted.

`src/overlays/ovl_11/ovl_11_func_800C28CC.c` consumes the target's
`D_8009CBF8` / `D_8008F7F8` data and `D_80125DC0` offsets through a
`SpriteDataHeader` view, supporting the sprite-loader role. That distant
consumer relationship alone does not justify extending the candidate run.
