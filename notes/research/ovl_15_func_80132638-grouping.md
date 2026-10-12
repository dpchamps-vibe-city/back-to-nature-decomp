# ovl_15_func_80132638 — grouping evidence

Verified handoff identity:
`d587888c208133fca4b56c24ce8820ab74118e1e5a5a61fb58f3e862ea796239`.
The handoff reports byte identity and a passed full finalize gate; this is a
notes-only review, not a new matching attempt.

## Supported membership

Add the function to the existing medium-confidence
`D_80053350/func_8001AC10` shared-prologue cluster, without changing its
confidence or inferring exact original translation-unit boundaries.

- The verified source, `src/overlays/ovl_15/ovl_15_func_80132638.c`, refreshes
  the slot list through `ovl_15_func_801335F8`, then uses the cluster's
  `func_8001AC10(field_D8 + 0x18, field_D8 + 0x14, text + D_80054BBC[0])`
  idiom. The original assembly at
  `build/ovl_15/asm/nonmatchings/ovl_15_func_80132638/ovl_15_func_80132638.s`
  witnesses that draw setup at 0x80132648–0x80132670.
- It calls `ovl_15_func_801370B4` and writes the shared state bytes
  `D_80137584`/`D_80137588`. Compare the existing cluster member
  `src/overlays/ovl_15/ovl_15_func_80132934.c`: the same draw idiom and
  selection helper, with writes to `D_80137584`/`D_80137587`.
  `src/overlays/ovl_15/ovl_15_func_801370B4.c` reads and updates
  `D_80137588`, corroborating the target's participation in this state graph.
- `configs/splat/ovl_15.yaml` places the target immediately after existing
  member `ovl_15_func_801325AC`: 0x47C4 + 0x8C = 0x4850. Both call
  `ovl_15_func_80134724` and write `D_80137584`; see
  `src/overlays/ovl_15/ovl_15_func_801325AC.c`. Thus adjacency agrees with
  shared state and a shared reset callee, rather than standing alone.

The new text base `D_80051EC6` is not itself a shared-global tie: a search of
this overlay's source and original function assembly found it only in the
target. The grouping inference depends on the combined draw idiom, state
graph and corroborated link adjacency. These can support candidate same-TU
membership but do not prove static linkage or justify merging other clusters.
