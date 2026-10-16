# ovl_11_func_8011E5B0: translation-unit grouping evidence

The supplied handoff records byte-exact verification and a passed full finalize
gate under source/evidence identity
`d57b2bfb471df7b401bce19a973d6dd6703b7c901f6af84f1339d86c1b18de10`.
The reviewed source is `src/overlays/ovl_11/ovl_11_func_8011E5B0.c`
(SHA-256 `3baf234fa26b64e02bd0dabd83a023c8e1e2c782a5caf4ac4d9898a512e8a493`).
The final entry in `build/experimentLedger/ovl_11_func_8011E5B0.jsonl`
records that source hash with `exact: true` and residual key `[0,0,0,0]`.
Verification was supplied, not rerun for this notes-only role.

## New evidence

- **Shared state driver and input handler:** the verified function reads and
  advances `D_80128420`, consuming state 1 installed by the existing member
  `src/overlays/ovl_11/ovl_11_func_8011E588.c`. Its default nonzero-state
  path calls `ovl_11_func_8011EA0C`, whose input handling installs states
  7, 8 and 11 in the same variable. States 4 and 12 call the cluster's
  reset/resource helper `ovl_11_func_8011EC84` before storing state 5.
  Evidence: the target source and the corresponding `8011EA0C.c` and
  `8011EC84.c` files in `src/overlays/ovl_11/`.
- **Shared sprite data and drawing sequence:** states 5, 8, 9 and 11 draw
  `D_8012DA80[0]` using the same `func_80022580` setup (320 by 176 at
  `field_D8 + 0x68`) and centered `func_80015BF0` blit at
  `field_D8 + 0x64` as the existing renderer
  `src/overlays/ovl_11/ovl_11_func_8011ECC4.c`. The existing loader
  `src/overlays/ovl_11/ovl_11_func_8011EE98.c` initializes that record.
  This adds a third verified consumer/initializer member to the previously
  documented sprite-data pair, not merely another use of a general draw API.
- **Call graph agrees with both link neighbours:**
  `configs/splat/ovl_11.yaml` lists `8011E588` at file offset `0x66768`,
  this function at `0x66790`, and its input-handler callee `8011EA0C` at
  `0x66BEC`. `build/ovl_11/functions.csv` gives sizes `0x28` and `0x45C`,
  so both boundaries are gapless: `8011E588 + 0x28 = 8011E5B0` and
  `8011E5B0 + 0x45C = 8011EA0C`. The original listing
  `build/ovl_11/functions/ovl_11_func_8011E5B0.s` corroborates the size,
  reset-helper calls at 0x8011E68C/0x8011E9AC and input-handler call at
  0x8011E9E0.

## Membership conclusion and limits

Add `ovl_11_func_8011E5B0` to the existing
`D_80128420/D_80128422` init/set cluster at **low confidence** as its
transition/framebuffer/background driver. It fills the link-order interval
between the state-1 installer and input handler while sharing the cluster's
state protocol and sprite data.

The premise is that this local state/resource ownership and corroborated
call/link run reflect one original module. Shared globals and calls can cross
files, and gapless adjacency does not prove a translation-unit boundary. No
shared static, GP-relative ownership, declaration-order effect or
register-variable quirk was established. Do not promote existing members or
add other callees/neighbours on this evidence alone.
