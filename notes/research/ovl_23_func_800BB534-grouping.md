# ovl_23_func_800BB534: shared sprite-state grouping evidence

Documentation-only handoff: the function is byte-exact and passed the full
finalize gate. Supplied source/evidence identity:
`160bd1246ef208acf6f36abfdda2fda8e9e9ed2dbf1ba4129d615ce76954994c`
(origin: agent). The handoff lists `include/overlays/ovl_23.h` and
`src/overlays/ovl_23/ovl_23_func_800BB534.c` as changed files; no build inputs
were edited during this documentation pass.

## New evidence

The verified initializer connects the existing display-setup run to the
adjacent array walkers and ObjectState reset/handoff leaf through the same
sprite-state storage, not merely a shared engine helper:

| 800BB534 destination | Address | Existing user |
| --- | --- | --- |
| `D_800BFC30 - 0x374 + i * 0x50`, six slots | `D_800BF8BC` | `800BB0E8` / `800BB150` |
| `D_800BFC30 - 0x1A0 + i * 0x44`, six slots | `D_800BFA90` | `800BB0E8` / `800BB150` |
| `D_800BFC30` | `0x800BFC30` | `800BB1B8` |

Both walkers derive their first array from `D_800BFA90 - 0x1D4`, which equals
`D_800BF8BC`, and use the same six-slot counts and 0x50/0x44 strides.
`800BB1B8` resets and dispatches the object that `800BB534` initializes.
These are shared destinations and complementary operations on one local
state cluster.

Original instructions corroborate the verified source: in
`build/ovl_23/asm/nonmatchings/ovl_23_func_800BB534/ovl_23_func_800BB534.s`,
0x800BB690–0x800BB6BC derives all three destinations from `D_800BFC30`;
0x800BB6E8–0x800BB728 initializes the two arrays, advancing 0x50/0x44 and
stopping after six iterations. The corresponding source evidence is:

- `src/overlays/ovl_23/ovl_23_func_800BB534.c`
- `src/overlays/ovl_23/ovl_23_func_800BB0E8.c`
- `src/overlays/ovl_23/ovl_23_func_800BB150.c`
- `src/overlays/ovl_23/ovl_23_func_800BB1B8.c`

`configs/splat/ovl_23.yaml` records uninterrupted link order:
`800BB0E8` → `800BB150` → `800BB1B8` → `800BB214` → `800BB488` →
`800BB534` → `800BB758`, ending at `800BB7A0`. The latter four already share
the dispatcher `800B7F00`; its original assembly is in
`build/ovl_23/asm/nonmatchings/ovl_23_func_800B7F00/ovl_23_func_800B7F00.s`.
The ledger already recorded that dispatcher/link-order evidence and the
DrawSync/ClearOTagR/load/poll idiom. Those confirmations are not the new
reason to extend membership; the shared sprite destinations are.

## Inference and limits

Extend the existing display-setup candidate group to include the adjacent
walkers and reset/handoff leaf, retaining medium confidence. This inference
combines shared local state and matching array geometry with uninterrupted
link order; it does not establish exact original TU boundaries or prove that
these globals were static. Do not infer an original aggregate declaration
from the recovered address arithmetic, or widen membership to other users
of engine helpers. The matching source's unprototyped initializer and blob
copy are not independently established TU fingerprints here.
