# ovl_19_func_800BB548 — shared descriptor and adjacency

Verified source/evidence identity supplied by the finalization handoff:
`cf0e85e339520cba317cace023c438bd5b60d9172ea5841f493c07ace0187f02`.
The handoff reports byte identity and the full finalize gate; this notes-only
review did not rerun matching or change build inputs.

## New membership evidence

The verified function passes the overlay-local `D_800BF700` descriptor to
`func_8001E4C0` and conditionally replaces the supplied position's Y component
with `func_8001E38C()`'s collision height. Its immediate predecessor,
`ovl_19_func_800BB4D4`, passes that same descriptor to `func_8001C0D4`.
The original function inventory gives BB4D4 a size of 0x74, ending exactly at
BB548; BB548 occupies 0x8C bytes, ending at BB5D4.

The shared descriptor plus gapless adjacency supports adding BB548 as a
conditional same-TU candidate alongside the existing far-buffer reader run.
BB548 does not itself access that run's `D_8007AFF0` state block. The descriptor
is also initialized by `ovl_19_func_800BBB48`, a connection between the reader
and display-setup runs already recorded in the ledger; that connection is not
new evidence from this function.

**Limit:** descriptor sharing does not prove static ownership. There is no
new declaration-order or register-variable fingerprint proving a TU boundary.
Do not merge the reader and display-setup runs, or infer membership from
BB548's adjacency to BB5D4 alone.

## Evidence paths

- `src/overlays/ovl_19/ovl_19_func_800BB548.c`: verified descriptor query and
  conditional Y update.
- `src/overlays/ovl_19/ovl_19_func_800BB4D4.c`: predecessor's use of the same
  descriptor.
- `src/overlays/ovl_19/ovl_19_func_800BBB48.c`: descriptor initialization.
- `build/ovl_19/asm/nonmatchings/ovl_19_func_800BB548/ovl_19_func_800BB548.s`:
  original descriptor argument and query at 0x800BB580–0x800BB58C.
- `build/ovl_19/functions.csv`: consecutive BB4D4, BB548 and BB5D4 addresses
  and sizes.
- `src/func_8001E38C.c`: collision-triangle height calculation.
