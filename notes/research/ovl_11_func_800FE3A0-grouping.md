# ovl_11_func_800FE3A0: adjacent threshold-display helper candidate

## Verified handoff

The documentation handoff reports `ovl_11_func_800FE3A0` byte-exact and through
its exact-diff, full configured build, scope and clean-source gates, with
source/evidence identity
`60cfe0ed69429cfb9011dba0863543471da276dc6d99addae175042fe30a392c`.
The verified source is `src/overlays/ovl_11/ovl_11_func_800FE3A0.c`, SHA-256
`69a3ec37d2e1ea4c322e2363d534b8ea0adc3feb782df44a30175744083b799e`.
The supplied changed-file list also contains `include/overlays/ovl_11.h` and
`src/overlays/ovl_11/ovl_11_func_800FDD74.c`. This notes-only pass did not
repeat the matching gate or change those build inputs.

## Evidence and membership

Its existing `D_8012A028` formatted-number cluster membership is corroborated
by the matched source: it formats a threshold-relative value into that buffer,
with fixed-label alternatives, and draws through `func_80017B3C`. Its formatting
path follows `func_8001A970` with `func_8001ABF0`, rather than writing a `0xFFFF`
terminator locally, so the ledger's universal direct-terminator wording has been
narrowed. The shared buffer was already recorded and is not new evidence by itself.

The new candidate relation is to its immediate predecessor, `800FE14C`:

- `build/ovl_11/functions/ovl_11_func_800FE14C.s` gives a size of `0x254`,
  so `0x800FE14C + 0x254 = 0x800FE3A0`, with no gap.
  `configs/splat/ovl_11.yaml` corroborates consecutive offsets `0x4632C`
  and `0x46580`. Generated C splits do not identify original TU boundaries.
- Both matched helpers use `D_80127214`. `800FE14C` computes a gauge's fill
  width between adjacent thresholds; `800FE3A0` computes a threshold-relative
  numeric value for display.
- Their common caller's original assembly,
  `build/ovl_11/functions/ovl_11_func_800FDD74.s`, calls them at `0x800FDE3C`
  and `0x800FDE58`. Its matched source supplies both helpers the same computed
  level and stored unsigned halfword for each of five rows, connecting the
  adjacent gauge and numeric display helpers to the same row data.

This supports a **low-confidence candidate same-TU pair**, `800FE14C` /
`800FE3A0`, alongside the separately documented
[`800FE068` / `800FE14C` pair](ovl_11_func_800FE14C-grouping.md).
It does not merge the formatted-number and `D_800719FE` families into one
proven TU or establish a wider source boundary. No private ownership of the
threshold table, shared static declaration, original register-variable quirk
or declaration-order constraint was established. The reconciled C prototypes
are reconstruction metadata, not independent original-TU evidence.
