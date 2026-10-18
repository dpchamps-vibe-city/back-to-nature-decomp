# ovl_11_func_800C907C: adjacent interaction helper/caller pair

## Verified anchor

Supplied source/evidence identity:
`0f945cbf65fd011f65bdcdcafae20fd63647545882500fe145ab2a6a2cb02acc`
(origin: agent). The handoff reports byte identity and a passed full finalize
gate, listing `include/globals_override.h`, `include/sdk_types.h` and
`src/overlays/ovl_11/ovl_11_func_800C907C.c` as changed files. This notes-only
review accepts that verification without modifying or re-verifying build inputs.

## New membership evidence

- **Gapless helper/caller adjacency.** `configs/splat/ovl_11.yaml` places
  800C8FA8 and 800C907C at offsets 0x11188 and 0x1125C.
  `build/ovl_11/functions/ovl_11_func_800C8FA8.s` gives the predecessor's
  size as 0xD4: 0x800C8FA8 + 0xD4 = 0x800C907C. In
  `build/ovl_11/functions/ovl_11_func_800C907C.s`, the call at 800C909C
  passes the incoming object unchanged to 800C8FA8 after testing input bit
  0x40; a nonzero helper result makes the caller return success immediately.
  The verified C source expresses the same relationship.
- **Shared dispatch state and update sequence.** The helper and caller both
  read `D_80128C54` and the object pointer `D_80128C4C`, already recorded
  in the state-pair cluster. The helper handles flag 0x80; the caller handles
  additional flags after that helper declines. Both copy a dispatched
  object's halfword into the incoming object's +0x22, update +0x6C flags,
  and call 800CB4E0. In the original helper, the +0x22 store is at 800C9058
  in the delay slot of the 800CB4E0 call; the caller repeats that sequence
  at 800C9108/800C910C, using dispatched-object +0xAC rather than +0x00.
  These original field/callee patterns corroborate adjacency independently
  of the reconstructed `Ovl11C8FA8Object` type.

## Roles and limits

800C8FA8 handles flag-0x80 interactions. 800C907C is the input-bit-0x40
interaction dispatcher, trying that helper before its other state-flag
branches and fallback. Record the two as a **low-confidence candidate
same-TU pair within the existing state-pair cluster**. The new evidence is
this narrower caller/helper relationship, not another reference to globals
whose shared use the ledger already records.

The conditional premise is that gapless placement, the direct call and
shared interaction-update pattern reflect one original translation unit.
Both state-pair globals are accessed absolutely: neither ownership nor
original static linkage is established. This does not raise the wider
cluster's confidence or locate a TU boundary. It does not add adjacent
800C8E98 or 800C93A4 to this pair, merge their state-switch family, or assign
800CB4E0 and the target's other callees to the same TU. The saved pointer
`D_80128CF4` newly declared by this match does not by itself establish its
owning TU.
