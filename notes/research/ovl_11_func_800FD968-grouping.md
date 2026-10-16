# ovl_11_func_800FD968: adjacent caller/callee TU candidate

## Evidence

The documentation handoff reports `ovl_11_func_800FD968` byte-exact and through
its full finalize gate, with source/evidence identity
`20c566a2c9b7e4093011275f6da4034ebcf3c86f7eb50a688cacdd9cc3dfdc08`.
The verified source is
`src/overlays/ovl_11/ovl_11_func_800FD968.c` (SHA-256
`3a605c7cce7ca892699ee034646f4b7c163745dcd232bb5f1d39d2c3cebb2907`).

- `build/ovl_11/asm/nonmatchings/ovl_11_func_800FD7B4/ovl_11_func_800FD7B4.s`
  gives the predecessor's size as `0x1B4`: `0x800FD7B4 + 0x1B4 =
  0x800FD968`. Its last delay-slot instruction is at `0x800FD964`.
- That predecessor calls `ovl_11_func_800FD968` at `0x800FD860`, passing
  `&D_8006C838 + 0x49C4 + 8 * index` and a signed-halfword y coordinate
  offset by two. It then reads both words of the same selected pair and
  chooses the row's text label.
- The verified callee reads `arg0[0]` and `arg0[1]` to draw opposed bars,
  with optional higher-order value labels. This agrees with the caller's
  eight-byte record selection.
- `configs/splat/ovl_11.yaml` lists the two functions consecutively at
  file offsets `0x45994` and `0x45B48`, corroborating the original assembly's
  link adjacency. These generated function splits are not original TU
  boundaries.

## Grouping conclusion and limits

The direct call and zero-gap link order agree, supporting a **low-confidence
candidate same-TU pair**: `ovl_11_func_800FD7B4` /
`ovl_11_func_800FD968`. The ledger previously listed neither member.

This remains conditional on no original TU boundary between them; a call to
an adjacent function does not prove common ownership. The callee's shared
render context `D_8005E3C0` and `D_8012CE88` draw data do not independently
establish ownership of the broader UI clusters. Do not merge this pair into
those clusters, or extend it to successor `ovl_11_func_800FDD74`, on adjacency
alone. No shared static, declaration-order constraint or original
register-variable declaration was established here.
