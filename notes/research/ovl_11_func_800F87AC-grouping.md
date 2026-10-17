# ovl_11_func_800F87AC: corroboration of the 800F84xx candidate run

## Verified premise

The supplied handoff reports byte-exact matching and a passed full finalize
gate for `ovl_11_func_800F87AC`, evidence identity
`517f007c08cad15a8eeae0e700e7d3296669c320a8d65a9ee8a30b7a7b30e7f9`.
The changed build inputs are `include/overlays/ovl_11.h` and
`src/overlays/ovl_11/ovl_11_func_800F87AC.c`. This documentation reads the
verified source and original disassembly; it does not rerun the solver or gate.

## New grouping evidence

The existing 6-byte struct-copy helper candidate already records the
`800F84F0` → `800F87AC` adjacency and shared `D_80126E4E`. New call-graph
corroboration supports listing `800F87AC` as a candidate member:

- `build/ovl_11/functions/ovl_11_func_800F787C.s` calls `800F84F0` at
  `0x800F7A00` and `800F87AC` at `0x800F7A40`. For the latter call it loads
  the cursor/page state `D_80126E44` and `D_80126E4A` into `$a2`/`$a3`.
- The verified source forwards those two arguments to `800F82FC`. The
  original target disassembly,
  `build/ovl_11/functions/ovl_11_func_800F87AC.s`, confirms this call at
  `0x800F87D0`. This links the message handler to the existing run's record
  selector rather than relying on address proximity alone.
- `build/ovl_11/functions/ovl_11_func_800F82FC.s` calls the run's `800F8404`
  at `0x800F8364` and ends at `0x800F8400`, immediately before that helper.
  Its record addressing uses a six-byte stride. This evidence comes from
  original instructions, not its disabled, nonmatching C attempt.

## Existing evidence retained

`build/ovl_11/functions/ovl_11_func_800F84F0.s` ends at `0x800F87A8`,
so `800F87AC` is its zero-gap successor. The matched source
`src/overlays/ovl_11/ovl_11_func_800F84F0.c` permits cursor/page updates
when `D_80126E4E == 255`; the verified target handles the complementary
state and restores 255 when `func_800226A4()` returns 2. Thus adjacent
handlers share both a caller and the same state protocol, with the target
also reaching the regional record-selector/helper chain.

## Limits

This corroborates the existing low-confidence regional candidate; it does
not prove an original translation-unit boundary or data ownership. No shared
static, declaration-order effect, or register-variable quirk was established.
The common caller and remote message/UI callees are dependencies, not newly
claimed members. The target's link successor is not added on adjacency alone.
