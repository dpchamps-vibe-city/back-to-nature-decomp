# ovl_11_func_800DF400 — dispatcher-run grouping

Verified source/evidence identity supplied by the finalization handoff:
`e628d21f6f4f1f3010bd46448ea71917369b774ca6affe1e164ce2eb992a9a8c`.
The agent-origin handoff reports byte identity and the full finalize gate,
with changes to `include/overlays/ovl_11.h` and
`src/overlays/ovl_11/ovl_11_func_800DF400.c`. This notes-only review did not
rerun matching or change build inputs.

## New candidate membership evidence

The verified body connects DF400 to the existing `D_800B957C`/`D_800B95CC`
dispatcher group through the local chain `800DEF30 -> 800DF400 -> 800DF010`.
DEF30 calls DF400 when the object's `s16@+0x2C` timer reaches 300 and flag
`0x800` at `s32@+0x34` is clear. DF400 selects a handler id and calls DF010
only if it differs from `s16@+0x26`; DF010 records that id and resets the
same timer. This supplies the state-selection link between the already
recorded per-tick invoker and dispatcher.

DF400 also reads two pieces of the neighbouring handlers' object protocol:
flag `0x2000` at `s32@+0x34`, set by DFA7C/DFB3C, changes its random-selection
limit; the `u16@+0xB2` latch, set by DF72C and checked by DF84C, redirects one
selection from id 17 to id 14. These relationships strengthen the local call
chain beyond a shared engine helper or generic object layout.

Code order agrees: `configs/splat/ovl_11.yaml` places DF400 at file offset
`0x275E0`, between DF3BC (`0x2759C`) and DF4F0 (`0x276D0`), within the
existing `0x800DE9C8–0x800DFB98` predicate-caller span. Its original assembly
extent is `0xF0`, so `0x800DF400 + 0xF0 = 0x800DF4F0` with no gap.
Together the state protocol, in-span calls and link order support candidate
membership in the existing medium-confidence dispatcher group.

**Limits:** these relationships do not prove static ownership or recover an
original TU boundary; callers and object protocols can cross TUs. DF400 does
not itself call DF4F0 or reference either handler table. Its adjacency does
not establish DF3BC's membership. The shared engine helper `func_80012A34`,
main-state global `D_80070CF8` and reconstructed `UnkStruct800DF4F0` type are
not independent same-TU evidence.

## Evidence paths

- `src/overlays/ovl_11/ovl_11_func_800DF400.c`: verified selection, state-id
  comparison, flag/latch reads and call to DF010.
- `src/overlays/ovl_11/ovl_11_func_800DEF30.c`: timer/flag-gated DF400 call.
- `src/overlays/ovl_11/ovl_11_func_800DF010.c`: id write and timer reset.
- `src/overlays/ovl_11/ovl_11_func_800DFA7C.c` and
  `src/overlays/ovl_11/ovl_11_func_800DFB3C.c`: flag writers and dispatcher calls.
- `src/overlays/ovl_11/ovl_11_func_800DF72C.c` and
  `src/overlays/ovl_11/ovl_11_func_800DF84C.c`: latch writer/checks.
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800DF400/ovl_11_func_800DF400.s`:
  original field reads, dispatcher call and `0xF0` extent.
- `configs/splat/ovl_11.yaml`: DF3BC/DF400/DF4F0 link order and offsets.
