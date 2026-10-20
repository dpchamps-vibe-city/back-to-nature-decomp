# ovl_11_func_80104CB8 — grouping evidence

Verified-source/evidence identity supplied by the handoff:
`01daaf18e56d60fb1124ff2ae1a041f35126563269e5f5c6031882809ef14180`.
The handoff reports byte identity and a passed full finalize gate, with changes
to `src/overlays/ovl_11/ovl_11_func_80104CB8.c`. This is notes-only
documentation, not a new verification claim.

## New local membership evidence

The verified source supports candidate membership alongside the existing
`80104A90` initializer in the ledger's `D_801287F4` counter-state family:

- **Complementary state and initialization cluster:**
  `src/overlays/ovl_11/ovl_11_func_80104A90.c` writes
  `D_8006C838+0x4488 = 1` and clears `D_8007AFF0+0x2548C`;
  `80104CB8` writes the same pair with the first value 2. Both clear flag
  0x800000 at `D_8006C838+0xC`, execute the consecutive calls
  `800BFEA4`, `800BFF00`, `800C044C(base->field_E4C8)`, and invoke
  `800CBEF8`, `800DC990` and `800DCECC`. The predecessor publishes
  `D_8009F78C` in the far-state data slot; the new function passes that
  buffer to `800DB2AC` and `80106EB0`. These ties concern specific state
  sites and a shared setup sequence, not merely the ubiquitous base globals.
- **Gapless link adjacency:** `configs/splat/ovl_11.yaml` places
  `80104A90` at file offset 0x4CC70, `80104CB8` at 0x4CE98, and
  `80104EA8` at 0x4D088. The original assembly under
  `build/ovl_11/asm/nonmatchings/ovl_11_func_80104A90/` and
  `build/ovl_11/asm/nonmatchings/ovl_11_func_80104CB8/` gives sizes
  0x228 and 0x1F0, respectively: both boundaries are gapless.
- **Dispatch graph agrees with local link order:** the original
  `D_80121780` table in `build/ovl_11/asm/data/69960.data.s` contains
  `80104A90`, `80104CB8`, `80104EA8` as consecutive entries 5/6/7.
  `src/overlays/ovl_11/ovl_11_func_800BBC34.c` dispatches through this
  table. The trailing wrapper's source calls `80104A90`, placing the new
  function inside a local initializer/handler/wrapper run.

## Limits

This is a **low-confidence local same-TU candidate**, conditional on the
adjacent handlers belonging to one original object. Neither handler directly
calls the other; dispatch membership and common setup do not establish a TU
boundary, private linkage, or an observed execution order between them.
`80104CB8` does not access `D_801287F4` itself. Its ledger placement follows
the already-recorded `80104A90` candidate, not proof that either shares an
original TU with the distant 0x800BBC74–0x800BCD28 core.

The `D_80128818` / `D_800A00C4` / `D_800A00CC` scene-grid setup also appears
in address-apart `800DBFEC`. As documented in
[that function's grouping note](ovl_11_func_800DBFEC-grouping.md), this is a
data/setup-family relationship, not grounds to merge its candidate TU pair
or the `D_8012D110` record pair with these handlers.
