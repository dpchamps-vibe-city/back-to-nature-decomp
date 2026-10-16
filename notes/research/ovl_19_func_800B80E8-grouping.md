# ovl_19_func_800B80E8 — state-cluster membership evidence

Verified source/evidence identity supplied by the finalization handoff:
`a1788b3cdf013b70cd670a688186c8d28ab6ad53dd163812a74656ec4c9ceca1`.
The handoff reports byte identity and the full finalize gate; this notes-only
review did not rerun matching or change build inputs.

## New membership evidence

The verified dispatcher selects `D_800BCEA8[D_800BF4C4]`. The signed halfword
at `D_800BF4C4` is exactly `D_800BF4C0[2]`, not merely an adjacent global:
existing cluster member `800B82B8` clears it, and `800B93B0` writes 3 or 2 to
it. The eight-entry overlay-local handler table includes the recorded cluster
hub `800B89C8` at index 5. Shared state and the indirect call graph therefore
support adding the dispatcher to the existing s16-state consumer cluster.

Link order is consistent: `800B80E8` ends at `800B8150`, followed by
`800B8150` (0x40 bytes), `800B8190` (0x128 bytes), and the cluster initializer
`800B82B8`. Both intervening functions call existing cluster member
`800BA770`. This corroborates the connection without establishing a TU boundary
or adding those intervening functions as members in this review.

**Limit:** the same-TU inference assumes the shared overlay-local state and
handler table belonged to one original module. Neither absolute addressing nor
a shared global proves static linkage; these objects could have been externs
across TUs. This is candidate membership, not proof that the entire cluster or
the dispatcher's distant `800BB358`/`800BB470`/`800BB4D4` callees shared a TU.

## Evidence paths

- `src/overlays/ovl_19/ovl_19_func_800B80E8.c`: verified update/dispatch sequence.
- `include/globals_override.h`: `D_800BCEA8[8]` and signed `D_800BF4C4` declarations.
- `src/overlays/ovl_19/ovl_19_func_800B82B8.c` and
  `src/overlays/ovl_19/ovl_19_func_800B93B0.c`: writes to `D_800BF4C0[2]`.
- `build/ovl_19/asm/data/3F54.data.s`: handler entries at
  0x800BCEA8–0x800BCEC4 and state labels at 0x800BF4C0/0x800BF4C4.
- `build/ovl_19/functions/ovl_19_func_800B80E8.s`: original signed selector
  load at 0x800B8120 and indirect call at 0x800B8138; size 0x68.
- `build/ovl_19/functions/ovl_19_func_800B8150.s` and
  `build/ovl_19/functions/ovl_19_func_800B8190.s`: intervening sizes and calls
  to `800BA770`.
