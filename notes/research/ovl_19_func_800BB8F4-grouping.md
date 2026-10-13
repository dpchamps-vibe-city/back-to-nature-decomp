# ovl_19_func_800BB8F4 — shared sprite-cluster grouping evidence

Verified source/evidence identity supplied by the finalization handoff:
`5d7d0f5bf3792cc8cb7f3099995bac3e0592031a160b08f7936766fcaa62dd7e`.
The handoff reports byte identity and the full finalize gate; this notes-only
review did not rerun the solver or change build inputs.

## New membership evidence

The ledger already places `ovl_19_func_800BB8F4` in the
0x800BB5D4–0x800BBD14 display-setup run. Its shared SDK/load idiom and
call/link adjacency are therefore not new evidence.

The verified initializer additionally identifies the same two sprite arrays
walked by the matched slot-programmer `ovl_19_func_800BAC9C`:

| Array | 800BB8F4 initialization | 800BAC9C flag programming |
| --- | --- | --- |
| `D_800BF570` | `D_800BF660 - 0xF0`, three records, stride 0x40 | three records, stride 0x40 |
| `D_800BF4E8` | two records, stride 0x48 | two records, stride 0x48 |

This is a shared overlay-local global cluster with identical record traversal,
not merely two callers of an EXE helper. It supports a conditional same-TU
membership link between the initializer's existing run and the slot-programmer
in the 0x800BAC40–0x800BAC9C run.

**Limit:** shared globals can be declared across TUs. Neither function proves
that these arrays were static, and the pair is not adjacent in link order.
Treat the connection as a candidate, not proof that the two runs or every
intervening function form one original TU. The initializer's unique blob and
offset table do not independently establish shared ownership.

## Evidence paths

- `src/overlays/ovl_19/ovl_19_func_800BB8F4.c`: the two initialization loops.
- `src/overlays/ovl_19/ovl_19_func_800BAC9C.c`: the two slot-programming loops.
- `build/ovl_19/functions/ovl_19_func_800BB8F4.s`: original setup and loops at
  0x800BBA4C–0x800BBB04; the first base is formed at 0x800BBA5C.
- `build/ovl_19/functions/ovl_19_func_800BAC9C.s`: original array bases at
  0x800BACD8–0x800BACDC and 0x800BAD08–0x800BAD0C.
- `notes/file-groupings.md`: the existing display-setup and arg-record
  setter/slot-programmer sections; no new ledger section is needed.
