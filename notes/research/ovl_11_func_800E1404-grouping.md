# ovl_11_func_800E1404 — handler-run grouping

Verified source/evidence identity supplied by the finalization handoff:
`a363fe7d11b9fed494433846c8b022922a026e717409871d5976e16f4ebb13a5`.
The handoff reports byte identity and the full finalize gate, with
`src/overlays/ovl_11/ovl_11_func_800E1404.c` as the changed file. This
notes-only review did not rerun matching or change build inputs.

## New evidence for existing candidate membership

The ledger already records E1404 as a caller of `800E109C` and an entry in
`D_800B961C`. Its verified body adds the specific guard combination shared
with its immediate predecessor E12D8: object code `0x165`, clear
`u16@+0xB2`, and `u16@+0xB4 >= 7`. E12D8 sets that same +0xB2 latch on
successful reservation; E1404 clears +0xB4 when accepting a request.

E1404 also shares the neighbours' reset/state-gated notification idiom:
reset through `800D049C`, then, after accepting a request, call
`800D12A0(0xB)` only when the object's `s16@+0x30` equals the far-state
halfword at `(u8 *)&D_8007AFF0 + 0x25476`. E12D8 and immediate successor
E15C8 both use that reset and notification gate; E15C8 also calls the same
`800E109C` predicate.

These body relationships agree with gapless original link order:
`800E12D8 + 0x12C = 800E1404`, and
`800E1404 + 0x1C4 = 800E15C8`. All three are entries in `D_800B961C`.
Together, shared neighbour guards/tail, dispatch-table membership and link
order reinforce E1404's membership in the existing medium-confidence
handler/predicate-run candidate group beyond its previously known table
entry and predicate call.

E1404's role is a code-0x165 threshold-selected request submission/queue
handler. It selects a request from `s16@+0x16` and flag `0x40`, submits it
through `800D7338`/`800CB540` or an empty pending slot in `D_8006C838`, and
resets the object; the accepted-request path can also call `800E0E38`.

**Limits:** this is corroborating neighbour-idiom/link-order evidence, not
shared-static proof or a recovered TU boundary. Common engine helpers and
far-state globals alone do not establish ownership. The similar distant
`800DF84C` request handler supports parallel handler patterns, not merging
the two runs. Shared reconstruction types and source-local base pointers
are not evidence of original declaration order or register variables.

## Evidence paths

- `src/overlays/ovl_11/ovl_11_func_800E1404.c`: verified guards, request
  submission/queueing, reset and state-gated notification.
- `src/overlays/ovl_11/ovl_11_func_800E12D8.c`: neighbouring code/latch/timer
  guards, latch writer and reset/notification sequence.
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800E1404/ovl_11_func_800E1404.s`:
  original calls, guards and `0x1C4` extent.
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800E15C8/ovl_11_func_800E15C8.s`:
  successor's predicate call, reset and same state-gated `0xB` notification.
- `configs/splat/ovl_11.yaml`: consecutive E12D8/E1404/E15C8 entries at file
  offsets `0x294B8`/`0x295E4`/`0x297A8`.
- `build/ovl_11/asm/data/168C.rodata.s`: previously known `D_800B961C`
  entries for E12D8/E1404/E15C8 at `0x800B9644`/`0x800B963C`/`0x800B9630`.
