# ovl_11_func_800FD7B4: paired-value renderer caller extension

## Verified handoff

The documentation handoff reports `ovl_11_func_800FD7B4` byte-exact and through
its full finalize gate, with source/evidence identity
`81e1f574a8813931391b8f06bb148579f40e5cd22a3968e08334ca054486df1b`.
The verified source is
`src/overlays/ovl_11/ovl_11_func_800FD7B4.c` (SHA-256
`29b6ed620bf21ee901d958beea2b61ce8e0f59366ae99caae60e97dbec930e95`).
This documentation pass did not repeat the matching gate.

## New evidence relative to the existing pair

The [earlier note](ovl_11_func_800FD968-grouping.md) already records the direct
call and gapless link order of `800FD7B4` / `800FD968`. The new extension is
their immediate predecessor, `ovl_11_func_800FD5E8`:

- `build/ovl_11/asm/nonmatchings/ovl_11_func_800FD5E8/ovl_11_func_800FD5E8.s`
  gives its size as `0x1CC`: `0x800FD5E8 + 0x1CC = 0x800FD7B4`. Its final
  delay-slot instruction is at `0x800FD7B0`.
- The same assembly calls `ovl_11_func_800FD7B4` at `0x800FD778`, looping
  over indices 0 through 3. The y coordinates start at `0x68` and advance by
  `0x20`. Before the loop it reads the two words at `D_8006C838 + 0x49C4`
  and `+0x49C8` to draw their clamped numeric values.
- The verified `800FD7B4` source selects the eight-byte pair at
  `D_8006C838 + 0x49C4 + 8 * index`, passes it to `800FD968`, and uses both
  words to choose the row label. The new `Ovl11PairedValues` view in
  `include/globals_override.h` describes these four pairs; the predecessor
  reads the same first pair, not merely the same container-wide work area.
- `configs/splat/ovl_11.yaml` lists `800FD5E8`, `800FD7B4` and `800FD968`
  consecutively at offsets `0x457C8`, `0x45994` and `0x45B48`, corroborating
  the original assembly's two gapless call/link edges.

## Grouping conclusion and limits

Extend the existing **low-confidence candidate same-TU group** to
`800FD5E8` / `800FD7B4` / `800FD968`. Membership rests on the direct calls
agreeing with gapless link order, corroborated by the predecessor and target
consuming the same value pair. Original TU boundaries remain unproven;
generated function splits and a recovered struct view do not prove ownership.

`800FD5E8` is already listed in the ledger's `D_8012A028` formatted-number
text-buffer cluster. This is a candidate bridge, not evidence that the whole
cluster shares one TU with these renderers: `800FD7B4` does not access that
scratch buffer. No new shared static, declaration-order constraint or original
register-variable declaration was established. Do not extend to `800FDD74`
on adjacency alone.
