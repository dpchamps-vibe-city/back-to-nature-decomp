# ovl_11_func_8010A850: shared motion-state cell and adjacent sequence/timer pair

## Verified handoff

The supplied handoff reports byte-exactness and a passed full finalize gate
for `ovl_11_func_8010A850`, source/evidence identity
`a8762f37e86540fbe2edba1d634ab70b58feb9246297355af7fbeec2c8a4103b`
(origin: agent). Reviewed source:
`src/overlays/ovl_11/ovl_11_func_8010A850.c`; the supplied changed-file list
also names `configs/splat/ovl_11.yaml` and `include/globals_override.h`.
This documentation pass did not rerun matching or change build inputs.

## New grouping evidence

- **Writer/callee connection through the existing cluster cell.** The target
  writes `D_8012D084 = 1` at 0x8010A900, 0x8010AA34 and 0x8010AAF0. The first
  and last writes feed calls to `ovl_11_func_8010AE64` at 0x8010A904 and
  0x8010AAEC (the latter store is in the call delay slot). That callee reads
  the same cell at 0x8010AE78 to select one of two triples in `D_80127B2C`.
  This ties A850 directly to the D_8012D0xx family's documented reader,
  rather than merely placing its globals near that family. Original
  evidence: `build/ovl_11/asm/nonmatchings/ovl_11_func_8010A850/ovl_11_func_8010A850.s`
  and `build/ovl_11/asm/nonmatchings/ovl_11_func_8010AE64/ovl_11_func_8010AE64.s`;
  reviewed callee: `src/overlays/ovl_11/ovl_11_func_8010AE64.c`.
- **Parallel handler, sequence and timer order.** A850 passes `D_80127AE8`
  and `&D_8012D074` to `ovl_11_func_800D04D4` at 0x8010A95C. Its gapless
  predecessor and existing family member `ovl_11_func_8010A654` passes
  `D_80127ADC` and `&D_8012D072` to the same helper, with corresponding
  +0x28 state handling, +0x34 flag handling and an AE64 refresh path.
  Its original extent is 0x1FC: `0x8010A654 + 0x1FC = 0x8010A850`;
  `configs/splat/ovl_11.yaml` corroborates consecutive offsets
  0x52834 / 0x52A30 / 0x52D8C for A654 / A850 / ABAC.
  In `build/ovl_11/asm/data/69960.data.s`, A654's 12-byte sequence ends
  exactly at `D_80127AE8`, and its halfword timer ends at `D_8012D074`.
  A850's timer and companion state halfword `D_8012D076` fill the four bytes
  before the family's `D_8012D078` record pointer. Reviewed predecessor:
  `src/overlays/ovl_11/ovl_11_func_8010A654.c` and
  `build/ovl_11/asm/nonmatchings/ovl_11_func_8010A654/ovl_11_func_8010A654.s`.

## Conclusion and limits

Add A850 to the existing **low-confidence candidate** D_8012D0xx same-TU
family, without raising confidence or merging other groups. The shared-cell
writer/callee relationship and corresponding code/data order support this
membership, but do not prove static linkage or an original TU boundary.
Absolute addressing, modern shared types/prototypes, and assignment of the
target's jump table in the current splat configuration are not independent
ownership evidence. No declaration-order or register-variable quirk is claimed.
