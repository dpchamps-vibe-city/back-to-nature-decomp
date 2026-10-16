# ovl_11_func_800D6A94: paired row/record placement evidence

Documentation handoff: byte-exact, full finalize gate passed. Supplied
source/evidence identity:
`93877746ae2c49193d7e5cad1bc083bac8e992eb483679fff4bdd37c77d9b08e`.
The verified source SHA-256 is
`a3fcd6ea3159aa66beea23775d87422e2873411d005f008fb65af4014a47c7ef`,
also recorded in `run_output/autoloop/state.json` for this identity.
No solver experiments or build-input changes were made for this note.

## New grouping evidence

The ledger already records 800D6A94 as the sole caller and zero-gap link
successor of the sprite blitter 800D6944. Its verified body additionally
joins the nearby paired nine-slot row/record family:

- `src/overlays/ovl_11/ovl_11_func_800D6A94.c` selects a row with
  800D5750, remaps row 2 to row 1, and passes the identifier and row to
  800D6730. The returned slot selects `D_800A04B8[row][slot]`.
- `src/overlays/ovl_11/ovl_11_func_800D6730.c` assigns that slot in the
  parallel nine-halfword row of D_800A0494.
- `src/overlays/ovl_11/ovl_11_func_800D666C.c` uses the same row selector
  and 2-to-1 remapping, clears the identifier's slot in D_800A0494, and
  fills its paired D_800A04B8 record with 0xFF. This is the complementary
  release operation over the same two-object mapping.
- `src/overlays/ovl_11/ovl_11_func_800D678C.c` and
  `src/overlays/ovl_11/ovl_11_func_800D67F4.c` scan that same record row.
  800D6A94 calls both to choose placement, then writes the first halfword
  and the +0x18 coordinate that their scans test.

The original assembly at
`build/ovl_11/asm/nonmatchings/ovl_11_func_800D6A94/ovl_11_func_800D6A94.s`
corroborates the slot-allocation call at 800D6AF8, the record address
`D_800A04B8 + row*0x10E + slot*0x1E` at 800D6B94–800D6BE4, and the
placement-helper calls at 800D6CA4 and 800D6D00. The shared layout is declared
in `include/globals_override.h`.

This supports candidate membership with the local row/record helpers, beyond
the previously recorded blitter adjacency. It also corroborates the mapping
already documented for the address-apart consumer in
[ovl_11_func_800F0A58-grouping.md](ovl_11_func_800F0A58-grouping.md).
Both tables are externally visible: the shared mapping and local calls do not
prove original TU boundaries or place every distant consumer in one TU.
800D6944 still has no shared-global tie to these helpers; its membership
remains supported only by its previously recorded caller/link adjacency.

## One-line role

800D6A94 allocates a paired sprite-placement record and uploads its texture
and CLUT from a cached or loaded sprite source.
