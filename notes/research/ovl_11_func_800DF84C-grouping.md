# ovl_11_func_800DF84C — predicate-run grouping

Verified source/evidence identity supplied by the finalization handoff:
`8100d2c78398e52f6f9cccc3485fb2e23af1964ebf0c06dba0de64ca8e6896a9`.
The handoff reports byte identity and the full finalize gate, with changes to
`include/globals_override.h` and
`src/overlays/ovl_11/ovl_11_func_800DF84C.c`. This notes-only review did not
rerun matching or change build inputs.

## New evidence for existing candidate membership

DF84C was already recorded as a caller of `800DF4F0` and an entry in the
`D_800B957C` handler table. Its verified body adds a more specific relationship
to its immediate predecessor DF72C: both apply the predicate, require object
code `0x162`, and reject a nonzero `u16@+0xB2`. DF72C sets that same latch on
successful reservation. DF84C also shares the neighbouring handlers' reset
and notification sequence: after accepting a request it calls `800D049C`,
then calls `800D12A0(9)` only when the object's `s16@+0x30` equals the far
state halfword at `(u8 *)&D_8007AFF0 + 0x25476`. DF72C and the immediate
successor DF9F0 use that same predicate/reset/state-gated notification idiom;
DF614 and DF6A8 also exhibit it.

The shared guard and tail agree with gapless link order:
`800DF72C + 0x120 = 800DF84C`, and `800DF84C + 0x1A4 = 800DF9F0`.
The corresponding splat file offsets are `0x2790C`, `0x27A2C`, and `0x27BD0`.
Together these reinforce DF84C's candidate membership in the existing
medium-confidence predicate run beyond the previously known predicate call
and table entry.

DF84C selects a request code from the object's `s16@+0x16` thresholds and
flag `0x40`. It either submits through `800D7338`/`800CB540` or writes the code
to an empty `u16` slot at `D_8006C838 + s16@+0xB6 * 0xF8 + 0xDEB6`, then
performs the shared reset/notification sequence.

**Limits:** common engine helpers and the far-state base alone do not prove
same-TU ownership. This is combined neighbour-idiom/link-order evidence, not
shared-static proof or a recovered TU boundary. `Ovl11DF84CStateView` is a
reconstruction view, not evidence of original declarations. Do not merge other
users of the shared state or helper implementations into this run on this basis.

## Evidence paths

- `src/overlays/ovl_11/ovl_11_func_800DF84C.c`: verified guards, request
  submission/queueing, reset and state-gated notification.
- `src/overlays/ovl_11/ovl_11_func_800DF72C.c`: matching code/latch guards,
  latch writer and reset/notification tail.
- `src/overlays/ovl_11/ovl_11_func_800DF9F0.c`,
  `src/overlays/ovl_11/ovl_11_func_800DF614.c` and
  `src/overlays/ovl_11/ovl_11_func_800DF6A8.c`: neighbouring shared idiom.
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800DF84C/ovl_11_func_800DF84C.s`:
  original guard/call sequence, queue offset and `0x1A4` extent.
- `configs/splat/ovl_11.yaml`: consecutive DF72C/DF84C/DF9F0 entries.
- `build/ovl_11/asm/data/168C.rodata.s`: previously known `D_800B957C`
  handler-table entry at `0x800B9598`.
