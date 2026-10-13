# ovl_11_func_800F5404: flag-refresh caller of the entry-lookup run

## Provenance

The handoff reports byte-exact source and a passed full finalize gate, with
source/evidence identity
`6cd7dee1e8fc049f5f9a3a008a7a40f7492044b793c637137f13e08b2c097cb0`.
This notes-only pass did not rerun verification or change build inputs.
The source read at `src/overlays/ovl_11/ovl_11_func_800F5404.c` has SHA-256
`e0eebbb50ffb1485033344c0b8df290539af10ed826ed622421f2bbba6d90e1b`.

Evidence read:
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800F5404/ovl_11_func_800F5404.s`:
  original instruction addresses, calls and record accesses.
- `build/ovl_11/functions.csv`: original sizes, link order and call edges.
- `src/overlays/ovl_11/ovl_11_func_800F5698.c`,
  `src/overlays/ovl_11/ovl_11_func_800F5700.c` and
  `src/overlays/ovl_11/ovl_11_func_800F45A4.c`: existing lookup family.
- `include/globals_override.h`, `Ovl11FlagRefreshState`: the verified
  function's work-area view, not evidence of an original shared declaration.
- `build/preparation/ovl_11_func_800F5404/c2d55cc8eb70f06d94068b1fe4b66aee395debf745c52a909b570da5c6f644ae/evidence.md`:
  earlier offset witnesses and absence of an applicable static-chain pair.
  Its preparation status is superseded by the verified handoff.

## New membership evidence

`800F5404` extends the existing `800F5698` / `800F5700` candidate TU family
backward by one function. Original link order is gapless:

| Function | Overlay offset | Size | Exclusive end |
| --- | --- | --- | --- |
| `800F5404` | `0x3D5E4` | `0x294` | `800F5698` |
| `800F5698` | `0x3D878` | `0x68` | `800F5700` |
| `800F5700` | `0x3D8E0` | `0x40` | `800F5740` |

Call edges agree with that order: `800F5404` calls its immediate successor
`800F5698` eleven times, and `800F5698` calls its immediate successor
`800F5700`. The new caller supplies tables from `D_8006C838+0xDD8C/+0xDD90`
and signed halfword counts at `+0xDDD8/+0xDDDA`, together with keys and
status masks. Its two direct-update loops use the next table/count pairs
(`+0xDD94/+0xDD98`, `+0xDDDC/+0xDDDE`), stride `0x18`, and the same
`u16@+4` bit-0 OR/AND update as `800F5698`. Thus adjacency is corroborated
by both a direct caller/helper relationship and a specific record operation.

## Existing family evidence retained

`800F5700` scans `0x18`-byte records for `u16@+2 == (s16)key`, returning the
first matching pointer or null. `800F5698` forwards its table/count arguments
and conditionally sets or clears bit 0 at `+4` according to `arg3 & arg4`.
The previously listed sibling `800F45A4` (offset `0x3C784`, size `0x74`)
uses the same lookup-then-bit-update idiom on the table pointer at
`D_8006C838+0xDDB4`, with count 13 and predicate `arg1 == 1`.
Its membership remains weaker than the gapless three-function chain;
this target does not independently strengthen that distant sibling's tie.

## Limits

Medium-confidence same-TU membership is a hypothesis, not a recovered source
filename or boundary. No shared private static, TU-owned global, declaration-
order effect or register-variable quirk was established. The broad
`D_8006C838` work area and modern record-view names are not sufficient TU
proof by themselves.

The other calls from `800F5404` to `800F4618` and `800F5740` are recorded
behavior, not an asserted expansion of this group. No merge with the broader
work-area clusters or intervening record-processing runs is inferred.
