# ovl_19_func_800B9124 — state-cluster membership evidence

Verified source/evidence identity supplied by the finalization handoff:
`35f5efc99c214ca209c990af7a39137f4ed87a0eead793e30bb66835d84de7e2`.
The handoff reports byte identity and the full finalize gate; this notes-only
review did not rerun matching or change build inputs.

## New membership evidence

The verified handler uses the existing cluster's `D_800BF4C0` state array:
it updates indices 7 and 3, writes the return-value field at byte offset
+0x202, and passes the +0x10/+0xA0 records to `800BAC40`/`800BAC50`.
Its immediate neighbours `800B9050` and `800B9288` use the same state fields
and record setters.

The indirect call graph and link order agree. `D_800BCEC8` lists
`800B9050`, `800B9124`, and `800B9288` consecutively at indices 3–5;
`800B87C4` dispatches through that table using `D_800BF4C0[3]`.
The code is gapless: `800B9050 + 0xD4 = 800B9124`, and
`800B9124 + 0x164 = 800B9288`. Together these support adding `800B9124`
to the existing overlay-local s16-state consumer cluster.

**Limit:** this is candidate same-TU membership, conditional on the shared
state and ordered handler table belonging to one original module. Shared
globals and adjacent handlers can cross TU boundaries; no static linkage or
original file boundary is established, and no other membership is added here.

## Evidence paths

- `src/overlays/ovl_19/ovl_19_func_800B9124.c`: verified shared-state accesses
  and record-setter calls.
- `src/overlays/ovl_19/ovl_19_func_800B9050.c` and
  `src/overlays/ovl_19/ovl_19_func_800B9288.c`: neighbouring cluster members.
- `build/ovl_19/asm/data/3F54.data.s`: `D_800BCEC8` handler table,
  especially entries at 0x800BCED4–0x800BCEDC.
- `build/ovl_19/asm/nonmatchings/ovl_19_func_800B87C4/ovl_19_func_800B87C4.s`:
  signed selector load at 0x800B87D4 and indirect call at 0x800B8808.
- `build/ovl_19/asm/nonmatchings/ovl_19_func_800B9050/ovl_19_func_800B9050.s`
  and `build/ovl_19/asm/nonmatchings/ovl_19_func_800B9124/ovl_19_func_800B9124.s`:
  original sizes and gapless placement.
