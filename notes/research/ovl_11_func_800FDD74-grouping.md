# ovl_11_func_800FDD74: adjacent lookup-caller TU candidate

## Verified handoff

The documentation handoff reports `ovl_11_func_800FDD74` byte-exact and through
its full finalize gate, with source/evidence identity
`c45a6c484569e40f5e885d14a8970bc44c03e0e29db31eccc73bd822441b75ba`.
The verified source is
`src/overlays/ovl_11/ovl_11_func_800FDD74.c` (SHA-256
`ead09d3abb247f24cddb7d3b832fc25a4247c2fc6e2aa47fb051102d0d111715`).
This documentation pass did not repeat the matching gate.

## New grouping evidence

The ledger already records this function as an entry in `D_80127308`'s
row-builder dispatch table. The new evidence concerns its immediate successor:

- `build/ovl_11/asm/nonmatchings/ovl_11_func_800FDD74/ovl_11_func_800FDD74.s`
  gives its size as `0x124`: `0x800FDD74 + 0x124 = 0x800FDE98`. Its last
  delay-slot instruction is at `0x800FDE94`.
- The same assembly calls `ovl_11_func_800FDE98` at `0x800FDE20`, passing
  the row index. The verified source loops over five rows and passes the
  returned value, together with the corresponding unsigned halfword at
  `D_8006C838 + 0x5208 + 2 * index`, to `800FE14C` and `800FE3A0`.
- `src/overlays/ovl_11/ovl_11_func_800FDE98.c` clamps its index to 0..4,
  looks up `D_80127370`, and calls `800FDEDC`. This agrees with the caller's
  five-row use and connects it to the already documented lookup/consumer run.
- `configs/splat/ovl_11.yaml` lists `800FDD74` and `800FDE98` consecutively
  at offsets `0x45F54` and `0x46078`, corroborating the original assembly's
  gapless link order. Generated function splits are not original TU boundaries.

## Conclusion and limits

The direct call agrees with zero-gap link adjacency, supporting a
**low-confidence candidate same-TU pair**, `800FDD74` / `800FDE98`. This is
conditional on no original TU boundary between them; it does not prove common
ownership. The caller itself does not access the lookup run's `D_800719FE`.

Do not extend the preceding `800FD5E8` / `800FD7B4` / `800FD968` renderer
group on adjacency alone. The other nearby render-helper calls and existing
dispatch-table membership do not merge the broader UI families into one TU,
nor resolve the ledger's separate `800FE068` compiler-flag boundary evidence.
The container-wide work area and text-render context are not private storage
fingerprints. No shared static, declaration-order constraint or original
register-variable declaration was established here.
