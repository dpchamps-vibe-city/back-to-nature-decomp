# ovl_11_func_80115CAC: extension of the selected-record flag-helper run

## Verified premise

The supplied finalize handoff reports byte-exact matching and a passed full
finalize gate for `ovl_11_func_80115CAC`, with source/evidence identity
`428bfe5c5ba3732a89b7729fef1a6c1683b43a8d5ce3ca2b65aa8e25f307e899`.
Its supplied changed-file list is `configs/splat/ovl_11.yaml` and
`src/overlays/ovl_11/ovl_11_func_80115CAC.c`. This notes-only review reads
that source, its original disassembly and the existing helper sources; it
does not rerun matching or the build gate.

## New membership evidence

Extend the existing `80115F38` / `80115F80` helper-pair candidate backward
to include `ovl_11_func_80115CAC`.

- **Gapless original link order:**
  `build/ovl_11/asm/nonmatchings/ovl_11_func_80115CAC/ovl_11_func_80115CAC.s`
  gives size `0x28C`, with the final instruction at `0x80115F34`:
  `0x80115CAC + 0x28C == 0x80115F38`. The text splits in
  `configs/splat/ovl_11.yaml` agree: `0x5DE8C` → `0x5E118` → `0x5E160`
  → `0x5E1A8`, corresponding to the handler and two `0x48`-byte helpers.
- **Direct calls corroborate adjacency:** the same original disassembly
  calls `ovl_11_func_80115F80` at `0x80115D44` and `0x80115DE0`, and the
  immediate successor `ovl_11_func_80115F38` at `0x80115E0C`. The verified
  handler uses the former's flag result to choose IDs `0x61`/`0x60` and
  amounts `2000`/`500`, then uses the latter's selected index to pass
  `D_800742EC + index * 0xB4` to `ovl_11_func_800E2904`.
- **Record layout agrees with the helper pair:**
  `src/overlays/ovl_11/ovl_11_func_80115F38.c` and
  `src/overlays/ovl_11/ovl_11_func_80115F80.c` access flags at the address
  of `D_8006C838` plus `0x7AE8`, stepping `0xB4` bytes per record. Address
  arithmetic gives `0x8006C838 + 0x7AE8 == 0x80074320 == 0x800742EC + 0x34`.
  Thus the handler's record pointer and the helpers' flags agree on a
  `+0x34` flag field within the same indexed record family. This is shared
  record-layout evidence, not proof that any member owns the global data.

## Existing helper evidence retained

The prior pair has medium-confidence candidate same-TU membership: gapless
link order, a shared flag array, and a direct call.
`build/ovl_11/asm/nonmatchings/ovl_11_func_80115F80/ovl_11_func_80115F80.s`
places that call to `80115F38` at `0x80115F88` and gives size `0x48`.

The helper sources both form the flag base with
`base = (char *)&D_8006C838; base += 0x7AE8`, using a `0x2D`-word
(`0xB4`-byte) stride. `80115F38` scans ten entries for bit `0x20000`,
returning the last selected index or -1 as `s16`; the prior ledger also
records its mask's preheader `lui` and two-stage `lui %hi` / `addiu %lo`
base construction. `80115F80` calls the selector and tests bit `0x40` of
that entry's flag word. These details are retained here rather than in the
membership ledger.

## Limits

The handler extension is low-confidence candidate membership, not a recovered
original translation-unit boundary. Call/link adjacency and the shared record
layout support it; no shared static, GP-relative ownership, declaration-order
effect or register-variable quirk has been established. The per-function
Splat text splits and the handler's newly assigned switch-table rodata do not
by themselves identify original source files.

The handler reads `D_8012D524` and calls `8011D084` / `8011D0B4`, linking it
functionally to the recorded state-block setters. Those remote call edges do
not justify merging that cluster into this run. Likewise, adjacency to
`80115C60` and a shared call to the widely used `800F2354` counter helper do
not establish membership in the preceding counter-leaf group. The following
`80115FC8` getter is not added on adjacency alone. No original data ownership
or wider TU boundary is claimed.
