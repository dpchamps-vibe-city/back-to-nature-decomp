# ovl_11_func_8011EE98: translation-unit grouping evidence

The supplied handoff records byte-exact verification and a passed full finalize
gate under source/evidence identity
`f757f83aa06873c144a06564c7ebba7d5d35bfed7be2d2d68ef40ea9777714ed`.
The reviewed source is `src/overlays/ovl_11/ovl_11_func_8011EE98.c`
(SHA-256 `90d7effb88d92fbd039f74b1cb6c232d2649522c06bde5e36bf2ee8128bbb557`);
the final entry in `build/experimentLedger/ovl_11_func_8011EE98.jsonl`
records that source hash with `exact: true` and a zero residual.
Verification was supplied, not rerun for this notes-only role.

## New evidence

- **Shared overlay-local initializer/consumer pair:** the verified loader
  copies its resource header to `D_8012D810` and initializes
  `D_8012DA80[0]`, `[1]` and `[2]`. The existing cluster member
  `src/overlays/ovl_11/ovl_11_func_8011ECC4.c` blits those same three
  records. A source search finds `D_8012DA80` only in these two functions,
  and `D_8012D810` only in the loader. The record views are documented in
  `include/globals_override.h` under those symbol names.
- **Corroborating call graph:**
  `src/overlays/ovl_11/ovl_11_func_8011EC84.c`, already a member of the
  `D_80128420/D_80128422` cluster, calls this loader with `D_80070CF2`
  after resetting the cluster and setting its state to 6. The renderer
  `8011ECC4` calls `8011EE40` for its position lookup.
- **Corroborating link order:** `configs/splat/ovl_11.yaml` lists the
  consecutive boundaries `8011EC84` at file offset `0x66E64`,
  `8011ECC4` at `0x66EA4`, `8011EE40` at `0x67020`, and this loader at
  `0x67078`, ending at `8011F0C4` at `0x672A4`.
  `build/ovl_11/functions/ovl_11_func_8011EE98.s` corroborates the
  loader's `0x22C` size and the three record addresses at base,
  base + `0x30`, and base + `0x60`.

## Membership conclusion and limits

Add `ovl_11_func_8011EE98` to the existing
`D_80128420/D_80128422` init/set cluster at **low confidence**: it is the
resource initializer used by the cluster's reset path and paired with its
nearby sprite renderer. This extends the existing group rather than creating
a separate translation-unit claim.

The premise is that the local initialization/rendering ownership, call edges
and uninterrupted link run reflect one original module. They do not prove a
translation-unit boundary: globals and calls can cross files, and link
adjacency alone is insufficient. The loader does not access
`D_80128420/D_80128422`, and no shared static, GP-relative ownership or
compiler declaration-order quirk was established. Do not promote the
confidence or merge additional neighbours on this evidence alone.
