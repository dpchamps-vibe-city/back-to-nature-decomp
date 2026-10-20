# ovl_11_func_800DFE74 — handler-run membership

Verified source/evidence identity supplied by the finalization handoff:
`bfdaf7e973e562a82ca615fa5698e32a9e980ff64cf30bb3a4a01830762eea8a`.
The handoff reports byte identity and the full finalize gate; this notes-only
review did not rerun matching or change build inputs.

## New grouping evidence

The verified function fills a specific position in the existing
`D_800B957C`/`D_800B95CC` two-table dispatcher group:

- `D_800B95CC[3]` at `0x800B95D8` points to `ovl_11_func_800DFE74`,
  between entry 2 (`800DFDAC`) and entry 4 (`800DFF70`). The per-tick driver
  `800DEF30` invokes this table using the object's handler id at `+0x26`;
  `800DF010` selects that id and clears `+0x28/+0x2A/+0x2C`.
- Code order agrees with table order: `800DFDAC + 0xC8 = 800DFE74`, and
  `800DFE74 + 0xFC = 800DFF70`. Splat offsets are respectively
  `0x27F8C`, `0x28054`, and `0x28150`.
- Frame-sequence data has the same order, without gaps: DFDAC's
  `D_80123F18` occupies 12 bytes, DFE74's `D_80123F24` occupies 12 bytes,
  and DFF70's `D_80123F30` follows. The middle sequence contains the words
  `0x003C0004, 0x00000001, 0x0000FFFF`.
- The neighbours share the state field at `+0x28`, flag `0x800` at `+0x34`,
  completion timer value `0x12C` at `+0x2C`, and frame-step callee
  `800D04D4`. DFE74 is a variant, not a byte-identical twin: it passes
  `+0x2C` rather than `+0x2A` as the stepper's timer pointer, and also calls
  `800D12B8(obj, 0x12, 0x8031)` and clears flag `0x4000` on completion.

Together, the callback relationship and matching code/data order support
medium-confidence candidate membership in the dispatcher's original-TU group,
with a stronger local association to DFDAC/DFF70. They do not prove static
ownership or an original TU boundary. The shared engine callees alone do not
justify merging other handler families into this group.

## Evidence paths

- `src/overlays/ovl_11/ovl_11_func_800DFE74.c`: verified state/flag handler.
- `src/overlays/ovl_11/ovl_11_func_800DFDAC.c` and
  `src/overlays/ovl_11/ovl_11_func_800DFF70.c`: neighbouring handler bodies.
- `src/overlays/ovl_11/ovl_11_func_800DEF30.c` and
  `src/overlays/ovl_11/ovl_11_func_800DF010.c`: table invocation and selection.
- `build/ovl_11/asm/data/1740.rodata.s`: `D_800B95CC` callback entries.
- `build/ovl_11/asm/data/69960.data.s`: consecutive frame sequences.
- `configs/splat/ovl_11.yaml`: consecutive code offsets.
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800DFE74/ovl_11_func_800DFE74.s`:
  original 0xFC extent, table relocation, timer pointer and calls.
