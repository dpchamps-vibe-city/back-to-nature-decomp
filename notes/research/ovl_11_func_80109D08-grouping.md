# ovl_11_func_80109D08: shared record-selection globals

## Verified handoff

The handoff reports byte-exactness and a passed full finalize gate for
`ovl_11_func_80109D08`, source/evidence identity
`c5d10fc0ef8a6fd32e60c56aa2befcbe8644986b547a397336fee24783f2733f`.
Reviewed source: `src/overlays/ovl_11/ovl_11_func_80109D08.c`.
This documentation pass did not rerun matching or change build inputs.

## New grouping evidence

- **Shared output pair, not merely nearby addresses.** The verified target
  clears `D_8012D078`, passes `&D_8012D07C` and `&D_8012D078` to
  `ovl_11_func_801096FC`, and on success forwards the selected record's
  +0x38/+0x3C/+0x40/+0x44 words to `ovl_11_func_800D05D0`.
  Original evidence:
  `build/ovl_11/asm/nonmatchings/ovl_11_func_80109D08/ovl_11_func_80109D08.s`,
  0x80109D7C–0x80109DE8. The scanner loads the two output addresses at
  0x80109714/0x80109718 and writes the flag and record pointer at
  0x801097C4–0x801097D8 in
  `build/ovl_11/asm/nonmatchings/ovl_11_func_801096FC/ovl_11_func_801096FC.s`.
- **Existing cluster user consumes both outputs.** In
  `build/ovl_11/asm/nonmatchings/ovl_11_func_8010A47C/ovl_11_func_8010A47C.s`,
  0x8010A540 loads `D_8012D078` and copies the same four record words to
  object +0x58..+0x64; 0x8010A598 reads `D_8012D07C` to choose between
  `ovl_11_func_800DF010` and `ovl_11_func_800E0C24`, both receiving that
  record pointer. This function is already named in the ledger's D_8012D0xx
  cluster as a `D_8012D070` user. The newly documented producer/consumer
  relationship therefore ties the target to an existing cluster user.
- **Supporting local context.** `build/ovl_11/asm/data/69960.data.s` places
  the two word cells immediately before the cluster's `D_8012D080` pointer.
  `build/ovl_11/functions.csv` records the target's 0xFC-byte extent ending
  exactly at `80109E04`, whose original assembly forwards the same four
  words from `D_8012D080` to `800D05D0`. These support the shared-global
  evidence; adjacency and the already-recorded `D_800BA848` table membership
  are not independently new findings.

## Conclusion and limits

Add the target to the existing **low-confidence candidate** D_8012D0xx
same-TU family, without raising confidence or merging other ledger groups.
The evidence is shared record/flag state with `8010A47C`, corroborated by
local data/link order and the neighbouring handler idiom. If these cells
were shared across original translation units, the producer/consumer tie
would not establish a common TU. Absolute addressing and modern header/type
sharing do not prove ownership or static linkage; no new declaration-order
or register-variable evidence was found.
