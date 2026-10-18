# ovl_11_func_800E8C14 grouping evidence

Verified handoff identity: `a5e735915af48756d7e8f12e9b86261f5473e072a80a0a7234bca01e88012591`.
The supplied handoff reports byte identity and a passed full finalize gate;
this is a notes-only grouping assessment, not a new verification run.

## Candidate membership

Extend the existing `D_80129560` accessor family, with a candidate local
same-TU tie to `ovl_11_func_800E8D00`:

- `src/overlays/ovl_11/ovl_11_func_800E8C14.c` conditionally reads its selector
  from `D_80129560[arg0]` and writes a one-based lookup result (or zero) to
  `D_80129560[arg1]`. The table declaration and overlay-private 0x50-byte
  extent are documented in `include/globals_override.h`.
- The same source reads the signed halfword at `D_8006C838 + 0x5252` and
  passes its address to `ovl_11_func_800D60D4`; the offset is explicit in
  `struct Ovl11E8C14State` in `include/globals_override.h`.
- `src/overlays/ovl_11/ovl_11_func_800E8D00.c` also reads that exact halfword
  (`StoreView.field_5252`) and can copy it into the same private table.
- `configs/splat/ovl_11.yaml` places these functions consecutively at
  file offsets `0x30DF4` and `0x30EE0`: the target's `0xEC`-byte extent ends
  exactly at `0x800E8D00`. Thus local link adjacency corroborates the shared
  table and state-field cluster rather than standing alone.

## Limits

This supports candidate membership, not a proven original TU boundary.
There is no direct call between the pair, and neither source supplies a
shared static, GP-ownership or declaration-order witness. Reconstructed
struct views are evidence for the accessed offsets, not original type names.
Calls to the distant `800D60D4`/`800D60F0` helpers establish dependencies,
not same-TU membership with those helpers.
