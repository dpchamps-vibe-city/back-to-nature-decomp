# ovl_11_func_800E1A3C — handler-run membership

Verified source/evidence identity supplied by the finalization handoff:
`ab78e94b327b0f5a6ed32a9c781baa189b0b33be7d7ec79deecc8d26ffcf9157`.
The handoff reports byte identity and the full finalize gate, with
`include/globals_override.h` and
`src/overlays/ovl_11/ovl_11_func_800E1A3C.c` as the changed files. This
notes-only review did not rerun matching or change build inputs.

## New evidence for existing candidate membership

The ledger already names E1A3C in the `D_800B966C` dispatcher table and as
E1974/E1B38's link neighbour. Its verified body identifies the intervening
frame sequence and shares the neighbouring state/flag-handler idiom:

- `D_800B966C[3]` at `0x800B9678` points to E1A3C, between entry 2
  (E1974) and entry 4 (E1B38). E0B40 invokes the table through the object's
  handler id at `+0x26`; E0C24 selects that id and resets `+0x28/+0x2A/+0x2C`.
- Code order agrees without gaps: `800E1974 + 0xC8 = 800E1A3C`, and
  `800E1A3C + 0xFC = 800E1B38`. Their splat offsets are respectively
  `0x29B54`, `0x29C1C`, and `0x29D18`.
- E1A3C passes `D_80123FDC` to `800D04D4`. This 12-byte sequence
  (`0x003C0004, 0x00000001, 0x0000FFFF`) lies without gaps between
  E1974's `D_80123FD0` and E1B38's `D_80123FE8`, in the same order as
  the callbacks and code.
- The neighbours share state at `+0x28`, flag `0x800` at `+0x34`, the
  completion value `0x12C` at `+0x2C`, and stepper `800D04D4`. E1A3C
  is a variant: it uses `+0x2C` rather than `+0x2A` for the stepper timer,
  calls `800D12B8(obj, 0x12, 0x8031)`, and clears flag `0x4000` on completion.

The identified sequence consumer and shared handler idiom reinforce
medium-confidence candidate membership in the existing E0C24 dispatcher/TU
run, especially the local E1974/E1A3C/E1B38 cluster. Callback, code and data
order agree; none establishes static ownership or an original TU boundary.

E1A3C is also a source-structural twin of `800DFE74`, substituting only its
function and sequence names. That supports parallel handler families, not
merging the two dispatcher runs. Shared reconstruction types and engine
callees are not independent original-TU evidence.

## Evidence paths

- `src/overlays/ovl_11/ovl_11_func_800E1A3C.c`: verified sequence consumer.
- `src/overlays/ovl_11/ovl_11_func_800E1974.c` and
  `src/overlays/ovl_11/ovl_11_func_800E1B38.c`: neighbouring handler bodies.
- `src/overlays/ovl_11/ovl_11_func_800E0B40.c` and
  `src/overlays/ovl_11/ovl_11_func_800E0C24.c`: table invocation and selection.
- `build/ovl_11/asm/data/1740.rodata.s`: current `D_800B966C` callback listing.
- `build/ovl_11/asm/data/69960.data.s`: consecutive frame sequences.
- `configs/splat/ovl_11.yaml`: consecutive code offsets.
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800E1A3C/ovl_11_func_800E1A3C.s`:
  original 0xFC extent, sequence relocation, timer pointer and calls.
- `src/overlays/ovl_11/ovl_11_func_800DFE74.c`: parallel-run handler variant.
