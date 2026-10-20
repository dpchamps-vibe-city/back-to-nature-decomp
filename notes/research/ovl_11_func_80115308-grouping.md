# ovl_11_func_80115308 — adjacent caller of the counter-forwarding leaf

## Verified premise

The supplied handoff reports byte-exact matching and a passed full finalize
gate for `ovl_11_func_80115308`, source/evidence identity
`77a9288bd0919a2d5746771ba3e71a6fe46c7739ce075780430f2905faee014d`
(origin: agent). Its supplied changed-file list is `configs/splat/ovl_11.yaml`,
`include/globals_override.h` and
`src/overlays/ovl_11/ovl_11_func_80115308.c`. This notes-only review reads the
verified source, the existing counter-leaf sources and original assembly;
it does not rerun matching or the build gate.

## New membership evidence

Extend the existing `80115C60` forwarding-twin candidate to include its
immediate predecessor and caller, `ovl_11_func_80115308`, as a
**low-confidence candidate same-TU pair**:

- **Gapless original text order:**
  `build/ovl_11/asm/nonmatchings/ovl_11_func_80115308/ovl_11_func_80115308.s`
  gives size `0x958`, ending with the delay-slot instruction at `0x80115C5C`.
  Thus `0x80115308 + 0x958 == 0x80115C60`. The corresponding text offsets in
  `configs/splat/ovl_11.yaml` are `0x5D4E8` and `0x5DE40`;
  `build/ovl_11/functions.csv` corroborates the addresses and sizes.
- **Direct call agrees with adjacency:** the original caller assembly has
  `jal ovl_11_func_80115C60` at `0x8011586C`, passing `$s1` in the delay slot.
  Earlier setup at `0x801157C8` makes `$s1 = &D_8012D538`. The verified source
  likewise calls `ovl_11_func_80115C60(state)` after testing the purchase
  cost and quantity limit. This is the only direct call to that helper found
  in `build/ovl_11/asm/nonmatchings/`.
- **Record use agrees:** the caller reads signed halfwords at `+8` and `+A`
  of `D_8012D538` to test their product before passing the record. The leaf's
  source, `src/overlays/ovl_11/ovl_11_func_80115C60.c`, consumes the same
  fields, forwarding their negated product to `800F2354` and the quantity
  to `800F3AF0`. Its original assembly under the corresponding
  `build/ovl_11/asm/nonmatchings/` directory confirms these accesses and
  calls. This is caller/callee record-layout agreement, not shared-static
  ownership.

## Existing forwarding-twin evidence retained

The ledger already related `80115C60` to `801152BC`. Their original bodies
are identical except for the second call target: both read signed `+8`/`+A`
fields and call `800F2354(-(unkA * unk8), 1)`, then forward `(2, unk8)` to
`800F3AF0` and `800F397C`, respectively. Their sources are
`src/overlays/ovl_11/ovl_11_func_80115C60.c` and
`src/overlays/ovl_11/ovl_11_func_801152BC.c`; the original assemblies and
`build/ovl_11/functions.csv` corroborate the calls and `0x4C` sizes.

`801152BC` remains in its existing adjacent-caller candidate with
`80114604`. The new local pair does not establish a single TU spanning both
caller/leaf pairs.

## Limits

The hypothesis depends on call/link adjacency and compatible record use
reflecting source organization rather than a boundary between neighbouring
translation units. No shared static, GP-relative ownership, declaration-order
effect or register-variable quirk is established.

The handler also uses `D_8012D524`/`D_8012D528`/`D_8012D52C` and
`D_8012D538`/`D_8012D548`, and calls the distant state setters and the
`8011B840`/`8011BAFC` interaction pair. Those functional links do not prove
same-TU membership with their existing groups. The newly assigned per-function
switch-table rodata split is reconstruction bookkeeping, not an original TU
boundary. Likewise, the leaf ends exactly at `80115CAC`, but no new evidence
justifies merging that separately documented selected-record helper run.
