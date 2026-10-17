# ovl_19_func_800B8908 — outer-handler membership evidence

## Provenance

The agent handoff reports byte-exact source and a passed full finalize gate,
with verified source/evidence identity
`4a1cb809f06d691d06638eba0566f7425ea0f6196bfd74383550d381ca6359cf`.
Supplied changed files: `include/overlays/ovl_19.h` and
`src/overlays/ovl_19/ovl_19_func_800B8908.c`. This notes-only review did not
rerun matching or change build inputs.

## Additional grouping evidence

[Earlier dispatcher research](ovl_19_func_800B87C4-grouping.md) already
recorded the target's placement between `800B8830` and cluster hub
`800B89C8` in the outer callback table and text order. The new connection is
the verified handler's completion handoff and shared command-record triplet.

The target is entry 4 of `D_800BCEA8`, which existing cluster member
`800B80E8` dispatches using the signed selector `D_800BF4C4`
(`D_800BF4C0[2]`). When `func_800226A4()` returns 5, the target increments
`D_800BF4C0[0x100]`. At 45 it commands the records at byte offsets
+0xA0/+0xE0/+0x120 through `800BAC50`, clears the counter and writes that
returned 5 to the outer selector. This explicitly selects entry 5,
`800B89C8`, its immediate text successor: `800B8908 + 0xC0 = 800B89C8`.
The hub itself commands the same three records through `800BAC50`.

Existing cluster initializer `800B8544` initializes exactly that three-record
run at +0xA0 with stride 0x40. Member `800B93B0` also clears the same
+0x200 counter on one exit arm. The target calls the cluster's shared render
driver `800BA770`, also called by the successor hub. The verified state
handoff, shared record/counter usage and indirect call graph corroborated by
gapless link order support adding the target to the existing
`D_800BF4D0`/`D_800BF4C0` s16-state consumer cluster.

## Membership conclusion and limits

Add only `ovl_19_func_800B8908` as a candidate member: a timed outer callback
that resets three command records and hands off to the next outer handler.

The same-TU inference assumes the shared state, record family and ordered
callbacks belonged to one original module. Overlay-local globals, tables and
calls can cross TU boundaries; neither static linkage nor an original file
boundary is established. No other membership or cluster boundary changes.

## Evidence paths

- `src/overlays/ovl_19/ovl_19_func_800B8908.c`: verified counter, record-setter
  calls, render-driver call and selector write.
- `src/overlays/ovl_19/ovl_19_func_800B80E8.c` and
  `include/globals_override.h`: outer-table dispatch and signed selector.
- `src/overlays/ovl_19/ovl_19_func_800B8544.c` and
  `src/overlays/ovl_19/ovl_19_func_800B93B0.c`: initialization of the same
  three-record run and reset of the same counter, respectively.
- `build/ovl_19/asm/data/3F54.data.s`: adjacent target/hub table entries at
  0x800BCEB8/0x800BCEBC.
- `build/ovl_19/asm/nonmatchings/ovl_19_func_800B8908/ovl_19_func_800B8908.s`:
  original size 0xC0, record arguments at 0x800B8974/0x800B8984/0x800B8994
  and selector store at 0x800B89B0.
- `build/ovl_19/asm/nonmatchings/ovl_19_func_800B89C8/ovl_19_func_800B89C8.s`:
  same three command records on the 0x800B8AA0–0x800B8B08 path and common
  render-driver call at 0x800B8C4C.
- `configs/splat/ovl_19.yaml`: consecutive text offsets 0xAE8/0xBA8 for the
  target and hub.
