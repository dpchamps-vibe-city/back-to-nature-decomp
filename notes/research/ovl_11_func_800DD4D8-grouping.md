# ovl_11_func_800DD4D8: verified nested transition pair

## Verified handoff

The handoff reports a passed full finalize gate and identity
`ffb8adfd70d37691e8507d86f872a76235140cd8ada9a76e81e1bc248063c386`.
Reviewed files:

- `src/overlays/ovl_11/ovl_11_func_800DD4D8.c`
- `src/overlays/ovl_11/ovl_11_func_800DD45C.c`

This documentation pass changes no build inputs or generated context.

## New same-TU evidence

The verified caller defines `ovl_11_func_800DD45C` as a GNU C nested
function. Its translation unit emits the helper and caller in their original
order; the helper's former source file now contains only a relocation-of-definition
comment. This confirms the previously high-confidence core as a nested
helper/caller pair, rather than merely two adjacent functions sharing globals.

The original caller assembly at
`build/ovl_11/asm/nonmatchings/ovl_11_func_800DD4D8/ovl_11_func_800DD4D8.s`
contains three calls to the preceding helper at 800DD54C, 800DD56C and
800DD584, with frame-address static-chain setup along their paths. The nested
definition also supplies the helper's incoming-static-chain save without the
former explicit register-capture construct. Its counter/limit and flag updates
operate on the entries indexed by `D_80129178`; both functions use the state
word `D_80129184`.

The [initializer study](ovl_11_func_800DD374-grouping.md) independently
records the zero-gap order: 800DD374 + 0xE8 = 800DD45C and
800DD45C + 0x7C = 800DD4D8. The integrated pair passed the full linked-image
verification with this layout.

## Limits

The nesting confirms the 800DD45C/800DD4D8 core only. Keep 800DD374 as a
medium-confidence candidate extension on its existing shared-state,
entry-protocol and adjacency evidence. Neither this reconstruction nor the
shared external globals prove the original TU's outer boundaries or justify
merging the analogous pointer/state clusters elsewhere in the overlay.
