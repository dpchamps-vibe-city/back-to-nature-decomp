# ovl_19_func_800B8DA8 — final-handler membership evidence

## Provenance

The agent handoff reports byte-exact source and a passed full finalize gate,
with verified source/evidence identity
`2d77d4d0f42a014f706d7c9be7ac2d9ea72ff57580ab62d902d8caf0f2df3f30`.
Supplied changed files: `include/globals_override.h`,
`include/overlays/ovl_19.h`, and
`src/overlays/ovl_19/ovl_19_func_800B8DA8.c`. This notes-only review did not
rerun matching or change build inputs.

## Additional grouping evidence

The target is entry 7, the final entry of overlay-local table `D_800BCEA8`.
Existing candidate member `800B80E8` dispatches this table using
`D_800BF4C4` (`D_800BF4C0[2]`); entry 5 is the recorded cluster hub
`800B89C8`, and entry 6 is `800B8C74`. These last three callbacks are also
consecutive in text order:
`800B89C8 + 0x2AC = 800B8C74`,
`800B8C74 + 0x134 = 800B8DA8`, and
`800B8DA8 + 0x40 = 800B8DE8`, the next recorded cluster member.
The configured offsets 0xBA8, 0xE54, 0xF88 and 0xFC8 corroborate this run.

The verified target adds a complementary state-exit connection to its
immediate predecessor. Both begin by calling cluster render driver
`800BA770`. In `800B8C74`, the `func_800226A4() == 2` arm with the saved
`u8@D_800BF4C0+0xC` equal to zero either restarts the inner state at 0 or
sets `D_800BF4C0[2] = 7`, selecting the target. With that saved byte nonzero,
the predecessor instead clears `s32@D_8006C838+0xE624` and increments
`s32@+0x448C`. The target sets the same completion word to 1 and increments
the same counter unconditionally. That counter is `D_80070CC4`, used by
`800B7ED8` to select the outer callback from `D_800BCE94`.

Thus the target is not connected by address proximity alone: table order,
the predecessor's explicit transition to index 7, the shared render call and
the complementary completion/state-exit writes agree with link order.

## Membership conclusion and limits

Add only `ovl_19_func_800B8DA8` as a candidate member of the existing
`D_800BF4D0`/`D_800BF4C0` state-consumer cluster: the final inner handler,
which runs the shared render driver, marks completion and advances the
outer state.

The same-TU inference assumes this ordered handler family, its dispatcher
and shared render driver belonged to one original module. The target does
not directly access the overlay-local state array; its completion word and
counter are resident shared RAM, not private-data evidence. Tables and calls
can cross TUs, so this does not establish static linkage or an original TU
boundary. No other handler is added by this review.

## Evidence paths

- `src/overlays/ovl_19/ovl_19_func_800B8DA8.c`: verified render call,
  completion-word set and outer-counter increment.
- `include/globals_override.h`: `Ovl19CompletionCounterView.counter` at
  +0x448C and `.completed` at +0xE624.
- `build/ovl_19/asm/nonmatchings/ovl_19_func_800B8DA8/ovl_19_func_800B8DA8.s`:
  original size 0x40 and stores at 0x800B8DD4/0x800B8DDC.
- `src/overlays/ovl_19/ovl_19_func_800B80E8.c`: indirect table dispatch.
- `build/ovl_19/asm/data/3F54.data.s`: `D_800BCEA8` entries at
  0x800BCEA8–0x800BCEC4 and selector label at 0x800BF4C4.
- `build/ovl_19/asm/nonmatchings/ovl_19_func_800B8C74/ovl_19_func_800B8C74.s`:
  render call at 0x800B8C80, index-7 transition at 0x800B8D5C/0x800B8D74,
  and complementary completion clear/counter increment at
  0x800B8D88–0x800B8D90.
- `build/ovl_19/asm/nonmatchings/ovl_19_func_800B7ED8/ovl_19_func_800B7ED8.s`:
  outer-selector load and dispatch at 0x800B7EF4–0x800B7F14.
- `build/ovl_19/asm/nonmatchings/ovl_19_func_800BA770/ovl_19_func_800BA770.s`:
  render driver's shared `D_800BF4D0`/`D_800BF560` state accesses.
- `build/ovl_19/functions.csv` and `configs/splat/ovl_19.yaml`: consecutive
  original function sizes and configured text offsets.
- [Existing dispatcher evidence](ovl_19_func_800B80E8-grouping.md):
  connection from the table selector to the recorded state-consumer cluster.
