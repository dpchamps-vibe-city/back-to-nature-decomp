# ovl_23_func_800BABD8: render-driver grouping evidence

Documentation-only handoff: the function is byte-exact and passed the full
finalize gate. Supplied source/evidence identity:
`180598c5acee56da89133936c163dcc4616757298c8bc940c5431f394d51aa7d`
(origin: agent). The supplied changed-file list is `include/overlays/ovl_23.h`
and `src/overlays/ovl_23/ovl_23_func_800BABD8.c`. No build inputs were edited
or matching experiments performed during this documentation pass.

## New membership evidence

The verified renderer supports extending the existing record-pair driver
candidate group to include `800BABD8`:

- **Shared overlay-local state:** the renderer derives its scroll position
  from `s32@D_800BF87C+0x10`, its marker separation from
  `s16@D_800BF87C+0x39C`, and its sprite from `D_800BF87C+0x3B4`.
  The latter equals `D_800BFC30`, which the first two draw loops address
  directly. The named `Ovl23_D_800BF87C_draw` view resolves to `D_800BF87C`
  in `include/globals_override.h`; these are not separate objects.
  The record handlers already consume this aggregate, and their common
  projection helper `800BB040` uses `D_800BF88C` (= aggregate +0x10).
- **Common render driver and gapless handler adjacency:** the original
  `800B9310` calls `800BABD8` at 0x800B9320, before its six-iteration paired
  `800BA73C`/`800BAA60` loop at 0x800B933C/0x800B9344. These three callees
  form one gapless link-order block: `800BA73C` (0x324 bytes) ends at
  `800BAA60`; `800BAA60` (0x178 bytes) ends at `800BABD8`; `800BABD8`
  (0x424 bytes) ends at `800BAFFC`. The common caller and adjacency
  corroborate the shared-state evidence, although call order is not link
  order: the strip renderer runs before the paired record handlers.
- **Corroborating sprite-state connection:** `800BB534` initializes
  `D_800BFC30` and the paired record sprites, while `800BB1B8` resets and
  dispatches that same object. `800BABD8` also uses the latter leaf's
  `D_800BFC30 - 0x3B4` base bias and +0x3B8/+0x3B9 byte reads when calling
  `func_80015EE8`. This connects the renderer to the already documented
  local sprite cluster, not merely to a commonly used engine helper.

Evidence paths:

- `src/overlays/ovl_23/ovl_23_func_800BABD8.c`
- `build/ovl_23/asm/nonmatchings/ovl_23_func_800BABD8/ovl_23_func_800BABD8.s`
  (sprite/base derivation at 0x800BABE0–0x800BAC24, aggregate-relative
  sprite at 0x800BAE14, separation read at 0x800BAF00)
- `build/ovl_23/asm/nonmatchings/ovl_23_func_800B9310/ovl_23_func_800B9310.s`
- `src/overlays/ovl_23/ovl_23_func_800BAA60.c`
- `src/overlays/ovl_23/ovl_23_func_800BB040.c`
- `src/overlays/ovl_23/ovl_23_func_800BB1B8.c`
- `src/overlays/ovl_23/ovl_23_func_800BB534.c`
- `include/globals_override.h`, `configs/splat/ovl_23.yaml`
- [Paired-handler evidence](ovl_23_func_800BA73C-grouping.md) and
  [sprite-initializer evidence](ovl_23_func_800BB534-grouping.md)

## Inference and limits

Add `800BABD8` to the existing record-pair/render-driver candidate group,
retaining medium confidence. Shared state, the common render driver and
contiguous handler block support suspected same-TU membership, not exact
original TU boundaries or static linkage. The recovered aliases, struct
views and local-variable spelling are not evidence of original declarations.
Do not merge all aggregate users or extend membership to the following
helpers on adjacency alone.
