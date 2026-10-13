# ovl_11_func_80104148: grouping evidence and matching context

Documentation-only review of the verified source. The supplied handoff reports
byte-exact matching and a passing full finalize gate, with source/evidence
identity `3106a1e118ef7a01f6844277baef7c32b4dc2260973b8e4e0e735b7864d740f8`.
Its changed-file list is `.pi/autoloop.json`, `configs/flag_overrides.mk`,
`notes/file-groupings.md` and
`src/overlays/ovl_11/ovl_11_func_80104148.c`. This review changes only notes
and does not rerun matching or the build.

## New grouping evidence

- **Shared scratch/state cluster.** The verified row-text emitter reads signed
  `D_8012CF10` entries and fills `D_8012CF30` with a glyph/terminator and two
  formatted values, using `func_8001A970` and `func_80017B3C`. The same scratch
  buffer and formatter/drawer pair occur in `ovl_11_func_80103EB8` and original
  `80103F8C`; `801037EC` clears the six-entry state window and `80104394`
  compares it. This ties the emitter to the existing D_80127428 family through
  specific shared storage, not merely common drawing helpers.
- **Local caller plus link order.** Original `ovl_11_func_80103F8C` calls the
  emitter at 0x80104074 in a loop over rows 0..5. It also reads the +0xE514
  selector and formats text into `D_8012CF30`. Original `80103964`, a core
  D_80127428 member, calls `80103EB8`, `80103F8C` and `801040A8` at
  0x80103978/0x80103980/0x80103988. Link order is gapless:
  `80103F8C + 0x11C = 801040A8`, `801040A8 + 0xA0 = 80104148`,
  `80104148 + 0x24C = 80104394`. Thus the call graph corroborates the
  adjacency already noted in the solver handoff.
- **Shared selected-record region.** The emitter reads the +0xE514 selector,
  maps mode 3 to bank 2, and indexes 14-byte records at +0xE516 (bank stride
  0x54), reading the name at +0 and a value at +0xA. The same selector and
  bank/row strides occur in the family's +0xE522 consumers; +0xE522 is the
  +0xC halfword of those records. This is another common data-region tie.

Evidence paths:

- `src/overlays/ovl_11/ovl_11_func_80104148.c`
- `include/globals_override.h` (`Ovl11CardView`, `D_8012CF30_glyph` alias)
- `src/overlays/ovl_11/ovl_11_func_80103EB8.c`
- `src/overlays/ovl_11/ovl_11_func_801037EC.c`
- `src/overlays/ovl_11/ovl_11_func_801040A8.c`
- `src/overlays/ovl_11/ovl_11_func_80104394.c`
- `build/ovl_11/functions/ovl_11_func_80103964.s`
- `build/ovl_11/functions/ovl_11_func_80103F8C.s`
- `build/ovl_11/functions/ovl_11_func_80104148.s`
- `configs/splat/ovl_11.yaml` (consecutive offsets 0x4C16C, 0x4C288,
  0x4C328 and 0x4C574)

## Limits

Assuming the shared scratch/state storage and corroborated local call/link
order reflect original source organization, this supports adding `80104148`
to the existing medium-confidence candidate family. It does not establish an
original TU boundary, private static ownership, GP-relative ownership or a
shared register-variable/declaration-order quirk. The emitter does not itself
access D_80127428/2C. Shared engine drawing helpers alone would not justify
membership or merging distant functions.

## Matching context moved out of the ledger

The solver's flag-override evidence comment reports that CSE branch following
exposes column constants to `gcse.c:cprop_insn`, folding the target's signed-short
cursor updates; disabling GCSE instead loses scratch-address sharing. The
conditional glyph RHS keeps a single scratch-address web where duplicated
branch stores split it. These are per-function matching details, not TU proof.

`build/flagProbe/ovl_11_func_80104148/report.json` records 133 instructions for
baseline, 149 for `-fno-gcse`, 147 with all 147 masked words matching for
`-fno-cse-follow-jumps`, and 152 when skip-blocks is also disabled. The report
itself labels its fingerprint conclusion inconclusive. It covers source hash
`bada7001648bb6da4405831dfaeb2a43b9e5433dba82992a07880d93a6831f0f`, before
unused declarations/includes were removed. The final source hash is
`f92d68d81fc5752ae5c147375c7dfeb0b3f3302d617a39cd52d0e8b95664f409`;
`build/experimentLedger/ovl_11_func_80104148.jsonl` records both as exact with
the same output hash. Exactness/full-gate status is supplied by the handoff,
not inferred from the masked flag-probe score.

`configs/flag_overrides.mk` also records no-follow compatibility for regional
witnesses `80103770`, `801037EC`, `80103D44`, `80103EB8` and `80104394`, with
artifacts under `build/manual/ovl_11_func_80104148/regional/`. Compatibility
is not evidence that those functions originally shared a flag or a TU. None
of these experiments was repeated during documentation.
