# ovl_11_func_8011F1D0: translation-unit grouping evidence

The supplied handoff records byte-exact verification and a passed full finalize
gate under source/evidence identity
`6c3f9b27b5fb6bd5365ad00143f7b198e868b9732f72074a13a63f12b2e6d65d`.
The reviewed source is `src/overlays/ovl_11/ovl_11_func_8011F1D0.c`
(SHA-256 `5b604ceeefe33fb359c999bc716b9f5b3e28507496e952eb158109750031af77`).
The final entry in `build/experimentLedger/ovl_11_func_8011F1D0.jsonl`
records that source hash with `exact: true` and residual key `[0,0,0,0]`.
Verification was supplied, not rerun for this notes-only role.

## Existing anchor and new evidence

The ledger already associated the target with the `D_8012DB10` / `D_8012DB14`
run by uninterrupted link order. Its first three members reset, initialize and
advance that shared s32 pair. The target does not access the pair, so its new
membership evidence is the corroborating call chain, not shared-global ownership:

- `build/ovl_11/functions/ovl_11_func_8011F114.s` loads `D_8012DB14` at
  `0x8011F17C`, calls `ovl_11_func_8011F1D0` at `0x8011F180`, and passes the
  returned value to `func_8002261C`. This is a direct caller among the existing
  pair-run members, immediately preceding the target in link order.
- The verified target calls `ovl_11_func_8011F4F4()` in case 3 and tests its
  count against 10. `build/ovl_11/functions/ovl_11_func_8011F1D0.s` records
  that call at `0x8011F318`. The callee's source and
  `build/ovl_11/functions/ovl_11_func_8011F4F4.s` identify its role as counting
  nonzero low halfwords in 99 four-byte cells at `D_80075854`.
- `configs/splat/ovl_11.yaml` lists consecutive C starts at file offsets
  `0x672A4` (`8011F0C4`), `0x672B8` (`8011F0D8`), `0x672F4` (`8011F114`),
  `0x673B0` (`8011F1D0`), `0x676D4` (`8011F4F4`), and `0x6770C`
  (`8011F52C`). The extracted assembly gives sizes `0xBC`, `0x324` and
  `0x38` for `8011F114`, `8011F1D0` and `8011F4F4`, respectively: both edges
  of `8011F114 → 8011F1D0 → 8011F4F4` are gapless caller/callee boundaries.

## Membership conclusion and limits

Record the verified target as a member and extend the existing candidate run
through its immediate callee `ovl_11_func_8011F4F4`. Retain the run's existing
medium confidence for the shared-pair head; the two trailing functions remain
**lower-confidence candidate extensions**. The premise is that a local
caller/selection-helper/count-helper chain in the same link order reflects one
original module. Direct calls can cross translation units, so this is evidence,
not a recovered boundary.

The target's `D_8006C838` state view, `D_8007AFE4` byte states/counters and
`D_8012852C` halfword table do not establish shared ownership with the pair-run
head. No shared static, declaration-order effect or register-variable quirk
was established. Absolute addressing alone does not locate a global's defining
translation unit.

The separate ledger family of count-helper copies at `8010C668`, `8011F4F4`
and `800FEAD4` is not merged: a local caller of one copy does not tie the
distant copies to this translation unit. Likewise, proximity of `D_8012852C`
to the `D_801285xx` lookup cluster and adjacency to `8011F52C` do not establish
a shared translation unit with that cluster. No further widening is supported.
