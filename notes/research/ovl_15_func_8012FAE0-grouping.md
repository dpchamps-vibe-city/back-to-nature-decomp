# ovl_15_func_8012FAE0 — grouping evidence

Verified handoff identity:
`e843f7c6d11ed62f311da92716a902829ed927c51eeb644c372715bee62ca5de`.
The agent-origin handoff reports byte identity and a passed full finalize gate;
its changed files are `include/globals_override.h`, `include/overlays/ovl_15.h`
and `src/overlays/ovl_15/ovl_15_func_8012FAE0.c`. This review changes notes only
and does not rerun matching.

## New membership in the existing shared-prologue cluster

The verified body supports adding this function to the medium-confidence
`D_80053350/func_8001AC10` cluster at 0x8012ED18–0x80132934:

- The verified source opens with the cluster's draw idiom: one
  `D_8005E3C0->field_D8` load, offsets +0x18/+0x14, and `D_80054BBC[0]`
  added to `D_8005346A` before `func_8001AC10`. This is the same updater
  and call idiom used by the listed members in
  `src/overlays/ovl_15/ovl_15_func_8012F8DC.c` and
  `src/overlays/ovl_15/ovl_15_func_8012FF28.c`. All three also read
  `D_8013758E` and write the private menu-state byte `D_80137584`.
  The target corroborates the updater/call at 0x8012FAEC–0x8012FB3C in
  `build/ovl_15/asm/nonmatchings/ovl_15_func_8012FAE0/ovl_15_func_8012FAE0.s`.
- Link order is gapless between listed members: in
  `configs/splat/ovl_15.yaml`, `ovl_15_func_8012FA70` starts at offset
  0x1C88, this function at 0x1CF8, and `ovl_15_func_8012FF28` at 0x2140.
  Thus the predecessor's 0x70-byte span ends at this function, whose
  target-declared 0x448-byte body ends exactly at 0x8012FF28.
- The call graph agrees with this handler placement. The common dispatcher
  calls `ovl_15_func_8012FA70`, `ovl_15_func_8012FAE0` and
  `ovl_15_func_8012FF28` from consecutive arms at 0x8012EB6C,
  0x8012EB7C and 0x8012EB8C, respectively; it also calls
  `ovl_15_func_8012F8DC` at 0x8012EB3C. See
  `build/ovl_15/asm/nonmatchings/ovl_15_func_8012EA18/ovl_15_func_8012EA18.s`.

The existing `D_80137828` buffer-pointer cluster already lists this function;
its buffer-base accesses are confirmation, not new membership evidence.
The new shared-updater/state tie plus corroborating link and call placement
supports candidate same-TU membership in the shared-prologue cluster. It does
not prove static linkage or exact original TU boundaries. Keep medium
confidence and do not merge the buffer-pointer or title-formatter groups on
this cross-cluster connection alone.
