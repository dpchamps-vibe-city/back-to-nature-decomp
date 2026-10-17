# ovl_19_func_800B9AB0 — state-cluster membership corroboration

Verified source/evidence identity supplied by the finalization handoff:
`d9f4028d571c07344f0b0f340ab363526ef1a5b7c035ca742e8e8d1ca4ade5ea`.
The handoff reports byte identity and the full finalize gate; this notes-only
review did not rerun matching or change build inputs.

## New evidence

`800B9AB0` was already listed in the `D_800BF4D0` s16-state consumer cluster.
The verified source adds specific connections beyond its recorded base and
callee set:

- It reads `D_800BF4C0[0xCA]` and `[0xBC]` to gate direction selection.
  Cluster initializer `800B82B8` seeds those exact fields with `0x1E` and
  `0x5A`. The original loads at `800B9AEC` and `800B9B18` use byte offsets
  +0x194 and +0x178 from the state base derived from `D_800BF4D0 - 0x10`.
- It stores `func_8001FABC(0x3B)` into `D_800BF4C0[0x101]`, exactly as
  `800B8F38` does. `800B9FB0` stores the same callee's `0x3C` result into
  that slot. The target's original store at `800B9CB8` names `D_800BF6C2`
  (`D_800BF4C0 + 0x202`), confirming the shared field independently of the
  reconstructed array view.
- Its byte-weight table `D_800BCF58` immediately follows `D_800BCF28`, the
  halfword-weight table consumed by cluster member `800BA33C`, in the
  overlay data. Both routines select among four cumulative weights using
  a sign-extended `func_80012A34(100)` draw. This is an additional related
  data/selection-idiom connection, not evidence from table adjacency alone.

These reinforce the existing candidate membership. The already-recorded call
edge from `800B998C` also agrees with gapless link order:
`800B998C + 0x124 = 800B9AB0`; the caller passes each of the two 0x48-stride
records beginning at `D_800BF4D0`.

**Limit:** shared globals, related tables and caller/callee adjacency can cross
translation-unit boundaries. This corroboration is conditional on the paired
records and their initialization/update machinery belonging to one original
module. It establishes no static linkage or original file boundary, does not
raise the cluster's confidence, and adds no other members.

## Evidence paths

- `src/overlays/ovl_19/ovl_19_func_800B9AB0.c`: verified field accesses,
  byte-weight selection and command updates.
- `src/overlays/ovl_19/ovl_19_func_800B82B8.c`: shared-field initialization.
- `src/overlays/ovl_19/ovl_19_func_800B8F38.c` and
  `src/overlays/ovl_19/ovl_19_func_800B9FB0.c`: shared return-value slot.
- `src/overlays/ovl_19/ovl_19_func_800BA33C.c`: neighbouring weight-table
  consumer and related four-choice selection idiom.
- `build/ovl_19/asm/data/3F54.data.s`: adjacent weight tables at
  `D_800BCF28` and `D_800BCF58`.
- `build/ovl_19/asm/nonmatchings/ovl_19_func_800B9AB0/ovl_19_func_800B9AB0.s`:
  original shared-field loads/store and byte-table access.
- `build/ovl_19/asm/nonmatchings/ovl_19_func_800B998C/ovl_19_func_800B998C.s`:
  original record walk, call at `800B99EC`, and size/link adjacency.
