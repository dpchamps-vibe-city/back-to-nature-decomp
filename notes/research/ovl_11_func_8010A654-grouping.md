# ovl_11_func_8010A654: shared record and adjacent action-state data

## Verified handoff

The handoff reports byte-exactness and a passed full finalize gate for
`ovl_11_func_8010A654`, source/evidence identity
`f3652bd1954014cd052735c347d08a1ff4ff4519ca2f678b02372be0ecdd5fc7`.
Reviewed source: `src/overlays/ovl_11/ovl_11_func_8010A654.c`; the supplied
changed-file list also names `include/globals_override.h` and
`include/sdk_types.h`. This documentation pass did not rerun matching or
change build inputs.

## New grouping evidence

- **Shared record with an existing cluster member.** The target reads
  `D_8012D080` and copies its +0x38/+0x3C/+0x40/+0x44 words to object
  +0x58..+0x64 before calling `ovl_11_func_8010AE64`. It also passes that
  record to `ovl_11_func_800D3104` with state 6. Original evidence:
  `build/ovl_11/asm/nonmatchings/ovl_11_func_8010A654/ovl_11_func_8010A654.s`,
  0x8010A728–0x8010A764 and 0x8010A7EC–0x8010A7F8. Existing ledger member
  `ovl_11_func_80109E04` reads the same pointer and forwards the same four
  words to `ovl_11_func_800D05D0`
  (`src/overlays/ovl_11/ovl_11_func_80109E04.c`). This adds a second
  documented consumer, not merely another nearby global address.
- **Corresponding sequence/timer pair in code and data order.** The target
  supplies `D_80127ADC` and `&D_8012D072` to `ovl_11_func_800D04D4`.
  Its immediate predecessor `8010A47C` supplies `D_80127AD0` and
  `&D_8012D070`, with the same +0x28 state switch, +0x34 flag handling,
  `8010AE64` fallback, and `800D0698`/`800D0408` update tail. See
  `src/overlays/ovl_11/ovl_11_func_8010A47C.c` and
  `build/ovl_11/asm/nonmatchings/ovl_11_func_8010A47C/ovl_11_func_8010A47C.s`.
  The predecessor's original extent is 0x1D8, so 0x8010A47C + 0x1D8 =
  0x8010A654; `configs/splat/ovl_11.yaml` records the same link order.
  In `build/ovl_11/asm/data/69960.data.s`, its 12-byte sequence ends exactly
  at `D_80127ADC`, and its halfword timer ends exactly at `D_8012D072`.
  The sequence words differ only in the middle word (0x003C0010 versus
  0x001E0010). These parallel data and handler idioms corroborate the
  shared-record tie to the already recorded D_8012D0xx candidate family.

## Conclusion and limits

Add the target to the existing **low-confidence candidate** D_8012D0xx
same-TU family; do not raise confidence or merge other groups. Shared record
use and corresponding adjacent sequence/timer pairs support membership, but
could also occur across original translation units. Absolute addressing,
shared modern types/prototypes, and placement in the broad extracted data
segment do not prove static linkage or TU ownership. No new declaration-order
or register-variable evidence is claimed.
