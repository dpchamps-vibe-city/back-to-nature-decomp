# ovl_11_func_800BBC74 — adjacent dispatcher wrapper

## Verified handoff

- Supplied source/evidence identity: `141ab7b87a867051a41ba747c66fbcc8a82335841de34a9b16f7265e0aeff343` (origin: agent).
- The supplied handoff reports byte-exact output and a passed full finalize
  gate; its changed-file list names only
  `src/overlays/ovl_11/ovl_11_func_800BBC74.c`.
- This is notes-only documentation, not a new verification run.

## New grouping evidence

The wrapper supports a tentative backward extension of the existing
`D_801287F4` counter-state run from `800BBCB8` to `800BBC74`:

- **Gapless link adjacency:** `configs/splat/ovl_11.yaml` places the wrapper
  at file offset 0x3E54 and `800BBCB8` at 0x3E98. The original assembly in
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800BBC74/ovl_11_func_800BBC74.s`
  declares size 0x44, so 0x800BBC74 + 0x44 = 0x800BBCB8.
- **Corroborating call edge:** the same assembly calls `800BBCB8` at
  0x800BBC94. The verified source does nothing beyond switching to the
  scratchpad stack, calling that immediate successor and restoring the stack.
  The call graph therefore agrees with link order for this wrapper/body pair.
- **Existing group anchor:** `src/overlays/ovl_11/ovl_11_func_800BBCB8.c`
  is already a candidate member through shared counter/control fields and
  adjacency, as documented in [its grouping evidence](ovl_11_func_800BBCB8-grouping.md).

## Limits

This is low-confidence wrapper/body membership evidence, not proof of an
original translation-unit boundary. The wrapper accesses no cluster global
or shared static itself. Its reusable scratchpad-stack idiom is not evidence
that other users of that idiom share this translation unit. No additional
caller, callee or link neighbour is added by this observation; the existing
shared-state core retains its prior confidence.
