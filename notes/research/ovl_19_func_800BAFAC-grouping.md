# ovl_19_func_800BAFAC — paired render helpers

Verified source/evidence identity supplied by the finalization handoff:
`2a1cadfe31a399d53e08120273963c10e5afa471591fb03526c96be501be26cf`.
The handoff reports byte identity and the full finalize gate; this notes-only
review did not rerun matching or change build inputs.

## New membership evidence

The verified function and its immediate predecessor, `ovl_19_func_800BADF8`,
share a specific render-helper sequence: adjust the supplied position through
`ovl_19_func_800BB548`, copy its three coordinates into an overlay-local vector,
project through `func_8001DFD4`, and use `(depth >> 3) * 4` to select corresponding
buckets for `func_80015BF0` and `func_800248E8`. BADF8 has additional sprite and
depth special cases; BAFAC selects the sprite state through `func_80015840`.

Their distinct scratch buffers form two adjacent pairs in `3F54.data.s`:
BADF8 uses the two-word output at `D_800BD048` and vector at `D_800BD080`;
BAFAC uses the next two-word output at `D_800BD050` and next vector at
`D_800BD088`. The original inventory gives BADF8 a size of 0x1B4, ending
exactly at BAFAC (0x800BADF8 + 0x1B4 = 0x800BAFAC). Thus the paired data and
render idiom agree with gapless code adjacency, rather than relying on a
common engine call alone.

The caller graph also ties both helpers to the existing `D_800BF4D0` consumer
cluster: its render driver `ovl_19_func_800BA770` calls BAFAC directly and calls
`ovl_19_func_800BA834` / `ovl_19_func_800BAB4C`, which both call BADF8. These
caller branches and their paired helpers occur in the same local link-order
region. This supports recording BAFAC as a conditional same-TU candidate
alongside that cluster, with BADF8 as its strongest immediate-neighbour link.

**Limits:** the two helpers do not share a scratch symbol. Adjacency of their
separate buffers does not establish static ownership, and the common engine
state `D_8005E3C0` is not TU-private evidence. No new declaration-order or
register-variable fingerprint proves a TU boundary. This evidence does not
justify merging the ledger's consumer, clamp-table, nested-callee or far-buffer
runs, or assigning the entire intervening address range to one original TU.

## Evidence paths

- `src/overlays/ovl_19/ovl_19_func_800BAFAC.c`: verified render-helper body.
- `include/globals_override.h`: evidence-backed output/vector declarations.
- `build/ovl_19/asm/nonmatchings/ovl_19_func_800BADF8/ovl_19_func_800BADF8.s`:
  predecessor's matching sequence and distinct scratch buffers.
- `build/ovl_19/asm/nonmatchings/ovl_19_func_800BAFAC/ovl_19_func_800BAFAC.s`:
  original target's calls, scratch accesses and depth-bucket calculation.
- `build/ovl_19/asm/nonmatchings/ovl_19_func_800BA770/ovl_19_func_800BA770.s`:
  render driver's three caller branches.
- `build/ovl_19/functions.csv`: caller edges, link order and BADF8/BAFAC sizes.
- `build/ovl_19/asm/data/3F54.data.s`: paired output and vector placement.
