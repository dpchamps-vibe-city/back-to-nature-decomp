# ovl_11_func_800ED7E0 — accessor-family membership

## Provenance

Notes-only review of verified
`src/overlays/ovl_11/ovl_11_func_800ED7E0.c`. The supplied handoff reports
byte-exact source and a passed full finalize gate (origin: agent;
source/evidence identity
`50418b189bec4d43f2e9da31823747dc68cbf493c5b5a77d88837c24dcf83c11`),
with that source as its changed-file list. No new matching experiment or
verification claim is made here.

## Grouping evidence

The recovered function supports **candidate membership in the existing
D_80129560 accessor family**, not a newly established TU boundary:

- It writes a sign-extended candidate ID to the overlay-private s32 table
  `D_80129560[arg1]` under `arg1 != -1`. The original target listing
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800ED7E0/ovl_11_func_800ED7E0.s`
  shows the guard, absolute table base, index*4 and word store at
  0x800EDA4C–0x800EDA70. This is the same guarded slot-writer interface as
  `src/overlays/ovl_11/ovl_11_func_800E78E4.c`, which also calls the engine
  random-number routine before its optional store.
- It immediately follows the already recorded table reader
  `ovl_11_func_800ED760`: `build/ovl_11/functions.csv` gives ED760 size
  0x80, ending exactly at ED7E0. `configs/splat/ovl_11.yaml` corroborates
  their consecutive offsets 0x35940 and 0x359C0. Shared table access therefore
  has local link-order support rather than only a distant data dependency.
- The original call at 0x800ED8D8 reaches existing family member
  `ovl_11_func_800EDF3C` in the same local address band. This edge was already
  recorded under EDF3C; the newly verified writer confirms the caller's
  membership through its own table access. EDF3C's parked recovery constructs
  are not evidence of an original register-variable or assembly idiom.

## Role and limits

The function filters five candidate IDs copied from `D_800B9DB0`, selects the
maximum unsigned halfword at work-area +0x99EC with record stride 0x1D4,
breaks ties randomly, and optionally stores the selected ID in the table.
The rodata listing `build/ovl_11/asm/data/1B58.rodata.s` supplies the five
halfwords 3, 7, 9, 14 and 20 at 0x800B9DB0; this single-function constant
is not independently a shared-static grouping fingerprint.

The inference remains conditional on the shared private table, writer API
and local placement reflecting source organization. Overlay-private storage
is not proof of TU-local ownership; neither adjacency nor a call proves a TU
boundary. The ubiquitous `D_8006C838` base and the additional call to
`800EFA1C` do not justify merging this family into the flags/state-buffer
group. No new outer boundary or compiler-level TU quirk is established.
