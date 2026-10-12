# ovl_11_func_800D95EC — shared cell-sprite output helper

Verified handoff source/evidence identity:
`6dbc40e710d72c7ad86a58888ae9011d66470dbe0cf5bcf250e45a31cb71125a`.
The supplied changed-file list names
`src/overlays/ovl_11/ovl_11_func_800D95EC.c`; byte identity and the full
finalize gate were supplied as passed. This is notes-only documentation,
not a new verification run.

## New membership evidence

The verified source calls `ovl_11_func_800D9EFC` for seven cell kinds,
passing the cell value as the condition, value + 2 as the first output,
a kind-specific selector as the second output, and two of its three
output pointers. The helper returns failure for a zero condition;
otherwise it writes both supplied outputs and returns success.

The original function assemblies independently corroborate a shared
helper among three members of the existing dispatch run:

| Assembly under `build/ovl_11/functions/` | Direct call address |
| --- | --- |
| `ovl_11_func_800D95EC.s` | 0x800D9800 |
| `ovl_11_func_800D981C.s` | 0x800D9AF8 |
| `ovl_11_func_800D9B14.s` | 0x800D9DF4 |

Each calls `ovl_11_func_800D9EFC`, passing the same two output-pointer
slots. `ovl_11_func_800D9EFC.s` corroborates the helper's conditional
stores and 1/0 result; its size is 0x20.

`configs/splat/ovl_11.yaml` places the helper at offset 0x220DC, directly
after the fallback handler at 0x21FF0. The latter's assembly size is
0xEC, so 0x800D9E10 + 0xEC = 0x800D9EFC with no gap. The shared calls
now corroborate the link adjacency that the
[earlier dispatch-run note](ovl_11_func_800D93C8-grouping.md) explicitly
left insufficient on its own. This supports adding 800D9EFC to that
suspected TU at low confidence.

## Limits

The helper is not exclusive to those three handlers:
`build/ovl_11/functions/ovl_11_func_800D9F1C.s` also calls it at
0x800DA2DC. Neither a shared call nor gapless placement proves an original
TU boundary, helper visibility or static ownership. This update records
the helper connection to the already documented dispatch run; it does
not claim that the TU ends at the helper, add further callers, or merge
the distant halfword-state group. No new shared-global, register-variable,
declaration-order or SDK fingerprint was established by 800D95EC.
