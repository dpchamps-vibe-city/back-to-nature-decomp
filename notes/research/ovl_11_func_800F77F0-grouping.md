# ovl_11_func_800F77F0: CD-load progress run extension

## Verified premise

The supplied finalize identity is
`fdeb4c7abc97a17713720ec145602fb9ec4f205fdf88e3f85baad82f4ad9c8ca`.
`run_output/autoloop/state.json`, entry
`completions.ovl_11_func_800F77F0`, records the same identity, agent origin,
and verification passed. Its changed files are `include/globals_override.h`,
`include/overlays/ovl_11.h`, and
`src/overlays/ovl_11/ovl_11_func_800F77F0.c`. The verified source read for
this documentation has SHA-256
`202df90d88a7fe37c8b40412433f31da9c87074723eb8c0602b5a8a682e37929`.
No solver or new build was run; this is a notes-only grouping update.

## New membership evidence

Extend the existing `800F71DC` / `800F7230` CD-load candidate with
`800F77A8` and `800F77F0` (medium confidence).

- **Shared reset/advance state with the immediate caller.** The verified
  `src/overlays/ovl_11/ovl_11_func_800F77A8.c` resets the halfword
  `D_80126E2C` for argument -1, otherwise calls `800F77F0(1)`.
  The target reads and updates that same halfword while progressing loads.
  Original evidence: `build/ovl_11/functions/ovl_11_func_800F77A8.s`
  (`jal` at `0x800F77C0`, reset `sh` at `0x800F77D8`) and
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800F77F0/ovl_11_func_800F77F0.s`
  (`lh` at `0x800F7800`, updates at `0x800F782C` / `0x800F7868`).
  The target starts a load below the `0x400` state marker, adds the marker,
  and polls/processes the indexed load above it; a zero callee result
  advances the encoded state by subtracting `0x3B0`.
- **Call graph corroborates the consecutive helper run.** The target calls
  `800F71DC` at `0x800F7814` and `800F7230` at `0x800F7840`, connecting
  the shared-state caller/callee pair to both members of the existing group.
  `build/ovl_11/ovl_11.map` records the gapless text sequence:
  `800F71DC + 0x54 = 800F7230`,
  `800F7230 + 0x578 = 800F77A8`,
  `800F77A8 + 0x48 = 800F77F0`, and
  `800F77F0 + 0x8C = 800F787C` (exclusive end).
  `configs/splat/ovl_11.yaml` preserves that order at offsets `0x3F3BC`,
  `0x3F410`, `0x3F988`, and `0x3F9D0`, followed by `0x3FA5C`.

## Retained evidence for the original helper pair

`build/ovl_11/functions/ovl_11_func_800F71DC.s` and
`build/ovl_11/asm/nonmatchings/ovl_11_func_800F7230/ovl_11_func_800F7230.s`
show the same `func_80014CBC` buffer argument, `D_8005E3B0 + 0x4290`,
formed in the call delay slots at `0x800F721C` / `0x800F72A4`.
Both use `0x3C19000` plus a scaled index and size `0x28000`.
The start wrapper's scale is `(arg0 << 16) >> 5`; the larger helper
sign-extends its halfword index before shifting by 11. Their final outgoing
argument differs (1 versus 0), consistent with the target's start/poll split.
The larger helper remains unmatched; its original assembly, not its disabled
C attempt, supplies this evidence.

## Limits

This supports candidate membership, not an exact original TU boundary,
static ownership, declaration order, or a register-variable quirk. The
central header declaration is an access-type correction, not ownership proof.
`800F6680` also clears `D_80126E2C`, alongside other state; that distant
initializer alone does not justify merging the intervening groups.
Neither the preceding `800F7094` nor following `800F787C` is added on adjacency
alone. Modern one-function object boundaries in the map are not original TU
boundaries.
