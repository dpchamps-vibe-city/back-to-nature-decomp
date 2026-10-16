# ovl_11_func_800D9F1C — fixed-grid cell-sprite renderer

Verified handoff source/evidence identity:
`37373c0625104342f6094b849444226bc5fc90274203c874e16c65b6eec956ea`.
Source SHA-256:
`8029948648303cc98bbe80dace404add3cc47b87393a98f0c269023a9803a438`.
Byte identity and the full finalize gate were supplied as passed; this is
notes-only documentation, not a new verification run.

## Membership evidence

The verified `src/overlays/ovl_11/ovl_11_func_800D9F1C.c` renders a fixed
7x7 grid. It reads eight-byte cells at `D_8006C838+0x78EC`, with a u16
kind at cell +0 and a u8 value at +4. This agrees with the cell layout
consumed by `src/overlays/ovl_11/ovl_11_func_800D93C8.c`, although that
renderer traverses a different map at +0x55C4.

Both renderers project positions spaced by 400 with height -100 through
`ovl_11_func_800F5888`, gate drawing on its result, and pass resolved
sprite/texture outputs to `func_80015EE8`, using the same
`D_8005E3C0->field_120 + ((projected[2] >> 2) * 4)` ordering-table slot.
The original assemblies under `build/ovl_11/functions/` corroborate the
projection/draw calls:

| Function | Projection call | Draw call |
| --- | --- | --- |
| `ovl_11_func_800D93C8.s` | 0x800D94F4 | 0x800D958C |
| `ovl_11_func_800D9F1C.s` | 0x800D9F94 | 0x800DA338 |

800D9F1C also calls `ovl_11_func_800D9EFC` at 0x800DA2DC, passing the
cell value, value + 2, a kind-specific selector and two output pointers,
as the existing mode handlers do. That call was already noted in the
[800D95EC helper evidence](ovl_11_func_800D95EC-grouping.md); the newly
verified renderer supplies the additional common cell/rendering pattern.

`configs/splat/ovl_11.yaml` places 800D9EFC at offset 0x220DC and
800D9F1C at 0x220FC. The 0x20 size in
`build/ovl_11/functions/ovl_11_func_800D9EFC.s` confirms that the helper
ends exactly at this renderer. Thus the shared-helper call and rendering
pattern agree with gapless link adjacency, supporting 800D9F1C's inclusion
in the existing bounds / cell-sprite dispatch run at low confidence.

## Limits

This remains a suspected TU, not a proven original boundary. Neither the
widely shared D_8006C838 buffer nor common projection/draw calls alone
establish same-file ownership. No new shared static, declaration-order,
register-variable or SDK fingerprint was established. The evidence does
not establish membership for the next function, 800DA390, or merge the
distant halfword-state run.
