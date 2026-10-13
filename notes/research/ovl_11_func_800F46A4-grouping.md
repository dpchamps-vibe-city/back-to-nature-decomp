# ovl_11_func_800F46A4: adjacent spawn-effect record consumers

## Provenance and evidence

The handoff supplies a byte-exact result and a passed full finalize gate for
`ovl_11_func_800F46A4`, with verified source/evidence identity
`fab6cf60430594d76f0aa0881ebd167c8f7d32c4dfa92c9cffde71a5c43aff69`.
This documentation pass did not rerun or alter that verification.

Evidence read:
- `src/overlays/ovl_11/ovl_11_func_800F46A4.c` (SHA-256
  `ede6bd7fe2c88f91dcc49e0e7eef56836a5e115d41240a17c9922bbf989fc6dd`),
  and its record view in `include/game_types.h`.
- `build/ovl_11/functions/ovl_11_func_800F46A4.s`.
- `build/ovl_11/functions/ovl_11_func_800F474C.s`.
- `build/ovl_11/functions/ovl_11_func_800F482C.s`.
- `configs/splat/ovl_11.yaml`, entries at offsets `0x3C884`, `0x3C92C`
  and `0x3CA0C`; these corroborate address order, not original TU boundaries.

## New grouping evidence

The verified function and its two immediate successors form a candidate
same-TU consumer trio (medium confidence):

| Function | Size | Exclusive end |
| --- | --- | --- |
| `800F46A4` | `0xA8` | `800F474C` |
| `800F474C` | `0xE0` | `800F482C` |
| `800F482C` | `0x168` | `800F4994` |

The original assembly supplies both gapless link order and a shared call/record
fingerprint. All three first call `800F4240` and reject a zero low-halfword
result. Their successful paths clear bit 0 of the record's u16 at +4, then
call `8012135C` with s16 at +2 and words at +8/+C/+10/+14, the last word in the
fifth-argument stack slot. The `800F46A4` and `800F474C` success tails have the
same instruction sequence modulo register assignment. `800F482C` retains the
same record transition and argument payload but adds item-specific updates.

The gates differ coherently: `800F46A4` rejects far-state mode 0x13 and tests
`800D5D38`; `800F474C` tests `800D5D9C` and two thresholds via `800D5E64` and
`800D5ED4`; `800F482C` uses `800D5E00` with the same threshold helpers. This
supports a local family of consume/spawn variants rather than adjacency alone.
The ledger records just the candidate membership and one-line roles.

## Limits

This is a TU-membership hypothesis, not a recovered source-file boundary or
proof of a shared static. The two successors are still assembly-backed.
The new record view does not independently establish original type identity.

The earlier spawn-record writer family includes `800F4240` and touches the
same +2/+4 prefix, but that helper edge does not prove the intervening functions
belong to one TU. No merge with that family is asserted. Likewise, the distant
`8012135C` effect-pool allocator and the broad `D_8007AFF0` state buffer do not
establish shared TU ownership; mode +0x25474 is not a newly identified private
global. The immediate predecessor `800F4618` is not included on adjacency alone.
