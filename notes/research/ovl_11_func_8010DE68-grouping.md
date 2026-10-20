# ovl_11_func_8010DE68: candidate handler-run membership

## Verified handoff

- Source: `src/overlays/ovl_11/ovl_11_func_8010DE68.c`.
- Supplied verified source/evidence identity:
  `080fc803fa19ea03e14646ca0bf3bd0d489a8b17149052de2d259ecb41687f57`.
- The handoff reports byte identity and a passed full finalize gate. This
  documentation-only review does not repeat verification or change build inputs.

## Grouping evidence

The existing `D_800BAA84` handler-run section lists DE68 only as a gapless
boundary marker. The verified body adds a coherent data/role tie to that run:

- `build/ovl_11/asm/nonmatchings/ovl_11_func_8010DA04/ovl_11_func_8010DA04.s`
  tests the signed item field at `D_8006C838 + 0x5252`, then calls DE68 at
  0x8010DA88 with the same object argument. DE68 reads that item field and
  returns 0 after queuing a message, or -1 otherwise; DA04 tests that result
  before continuing its handler. This is a local caller/helper relationship,
  not merely shared use of an engine helper.
- The verified body reads two eight-halfword message tables, `D_80127D14`
  and `D_80127D24`, and the two-by-five signed delta table `D_80127D34`.
  `build/ovl_11/asm/data/69960.data.s` places these consecutively immediately
  after the eight-halfword `D_80127D04` table used by the immediate predecessor,
  `src/overlays/ovl_11/ovl_11_func_8010DDC8.c`. Both functions index their
  message tables with the unsigned object halfword at +0xAC and pass the
  selected message to `func_8002261C(0, message)`. The adjacent tables and
  matching use corroborate the call/link relationship; the engine call alone
  would not establish TU membership.
- `configs/splat/ovl_11.yaml` and `build/ovl_11/functions.csv` record the
  gapless target sequence DA04/0x334, DD38/0x90, DDC8/0xA0, DE68/0x45C.
  Thus the caller and the table-using predecessor belong to the already
  recorded local link run. The present one-function C splits are not evidence
  of original file boundaries.

## Conclusion and limits

Treat DE68 as a candidate member of the existing handler run, rather than
only its boundary marker. Retain the section's medium-low confidence: the
call relationship and adjacent data cluster support membership but do not
prove an original TU boundary. No shared static, declaration-order effect,
register-variable quirk or GP-relative definition was established. Do not
merge the neighbouring dispatch/handler sections on this evidence alone.

Role: item-response helper selecting a message and updating a clamped object
value and response flags.
