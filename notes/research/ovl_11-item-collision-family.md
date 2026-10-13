# `ovl_11_func_800BE2C4`: local collision-family placement

Documentation handoff identity:
`142b10c1948ad7e908d3174df4fc10efed84df8befe77b1043b6a46a218934b9`.
The handoff reports byte-exact source and a passed full finalize gate; this
notes-only review does not repeat matching or change build inputs.

## New grouping evidence

The verified `src/overlays/ovl_11/ovl_11_func_800BE2C4.c` supports a local
collision-family hypothesis beyond its already-recorded use of the widely
shared `D_8006C858` item table:

- It and `src/overlays/ovl_11/ovl_11_func_800BE94C.c` consume the same
  collision-context layout: position pointer at +0, hit-output pointer at +8,
  and flags-output pointer at +0x10. Both test strict X/Z bounds using signed
  halfword extents at ItemData +0x24/+0x26, narrow the computed bounds to s16,
  then record a hit and update flags.
- Their common caller builds that context at `$sp+0x10`.
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800BEB28/ovl_11_func_800BEB28.s`
  calls `800BE94C` at 0x800BEBFC and `800BE2C4` at 0x800BEC90,
  0x800BED10, 0x800BED8C, 0x800BEDCC, 0x800BEE00, 0x800BEE70 and
  0x800BEEB0. Every site passes this same context as the first argument.
- `configs/splat/ovl_11.yaml` gives the gapless order
  `800BE2C4` (0x1E4) → `800BE4A8` (0xF8) → `800BE5A0` (0x144) →
  `800BE6E4` (0x268) → `800BE94C` (0x1DC) → `800BEB28` (0x43C), ending
  at 0x800BEF64. The same caller assembly also calls all three intervening
  helpers with the context at `$sp+0x10`; the intervening code therefore
  belongs to the same collision call-graph neighbourhood, not merely an
  unexplained address gap.

## Conditional conclusion

`800BE2C4` is a low-confidence candidate same-TU sibling of the existing
`800BE94C` / `800BEB28` collision pair. Shared context and bounds idioms,
corroborated by local call/link order, support that placement; they do not
prove original TU boundaries or require every intervening helper to share
one TU. No shared static, declaration-order effect or register-variable
quirk was established. The broad item-table accessor TU remains a separate,
unproven relationship for this function.

## `ovl_11_func_800BE6E4`: tile-probe extension

Documentation handoff identity:
`747ebcc265a15019c948e08387f885b663081ea69525b47dd1453ff5025d4643`.
The handoff reports byte-exact source and a passed full finalize gate;
this review changes notes only.

The verified `src/overlays/ovl_11/ovl_11_func_800BE6E4.c` adds a concrete
source-level tie to the adjacent `800BE94C` / `800BEB28` candidate pair,
beyond the earlier observation that it is an intervening collision helper:

- `800BE6E4` and `src/overlays/ovl_11/ovl_11_func_800BE94C.c` both read
  the position pointer at context +0, record a hit through the pointer at
  +8, and modify flags through the pointer at +0x10. Both set `0x4000`,
  clear it for item ID `0x64` (`800BE94C` also excludes `0x65`), and add
  `0x2000` according to flags in the same stride-0x28 item pool. The tile
  probe loads that pool at `(D_80071DFC - 0x55C4) + 0x20`, the same
  `D_8006C838 + 0x20` pointer used by the item probe. The shared pool alone
  is broad game-state evidence; the matching context and flag/ID conventions
  provide the local fingerprint.
- Original caller assembly
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800BEB28/ovl_11_func_800BEB28.s`
  builds the context at `$sp+0x10`, passes it to `800BE6E4` at 0x800BEB84
  and `800BE94C` at 0x800BEBFC, and handles the tile probe's distinct
  return values 1 and 2. Its `$sp+0x28` field is context +0x18, the
  category mask that the tile probe combines with `0x4000`.
- `configs/splat/ovl_11.yaml` and `build/ovl_11/functions.csv` agree on
  gapless order: `800BE6E4` (0x268) ends at `800BE94C`; `800BE94C`
  (0x1DC) ends at `800BEB28`. The original tile-probe assembly at
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800BE6E4/ovl_11_func_800BE6E4.s`
  corroborates the context fields and flag updates.

This supports extending the existing low-confidence collision pair to
include `800BE6E4` as its tile-map probe. It does not establish original TU
boundaries, a shared static, or a declaration-order/register-variable quirk.
The two coordinate-conversion callees (`800D7EF8`, `8011E090`) and other
address-apart grid readers are dependencies, not newly asserted members.

## `ovl_11_func_800BEF64`: actor-probe and stack-box links

Documentation handoff identity:
`f554627c6d54a0921b01b7757f04d66d68b66fe19d86a96e256d714bc525cc38`.
The handoff reports byte-exact source, a passed full finalize gate and changes
in `include/sdk_types.h` and
`src/overlays/ovl_11/ovl_11_func_800BEF64.c`. This review changes notes only
and does not repeat matching.

### Collision-dispatcher membership (low confidence)

The verified actor probe consumes the same context fields as the recorded
tile/item probes: position at +0, hit-output pointer at +8 and flags-output
pointer at +0x10. Like the tile probe, it writes the category mask at context
+0x18 to the flags output and conditionally adds `0x4000`.

Original caller assembly
`build/ovl_11/asm/nonmatchings/ovl_11_func_800BEB28/ovl_11_func_800BEB28.s`
sets context +0x18 (`$sp+0x28`) to `0x80` at 0x800BEC28, calls `800BEF64`
at 0x800BEC2C and passes the same `$sp+0x10` context in the delay slot.
`configs/splat/ovl_11.yaml` and `build/ovl_11/functions.csv` agree that the
caller is 0x43C bytes long and ends exactly at `800BEF64`; the actor probe
is 0x26C bytes long and ends exactly at `800BF1D0`. Thus a direct call edge,
shared context/flag conventions and gapless original link order support
adding `800BEF64` to the existing collision probe/dispatcher candidate run.

### Stack-box record-scan membership (medium confidence)

The immediate follower's verified source,
`src/overlays/ovl_11/ovl_11_func_800BF1D0.c`, provides a more specific data
and scan-pattern tie:

- `800BEF64` starts at `D_800763F4`, exactly `D_80076220 + 0x1D4`, and
  advances by 0x1D4 over indices 1..36. `800BF1D0` starts at `D_80076220`
  and scans indices 0..36 with that same stride. These are overlapping
  walks of the same record pool, not two unrelated global arrays.
- Both use the halfword at record +0x1E: bit `0x1000` selects the alternate
  position at +0x60 and its eligibility bit `0x800`; the ordinary position
  at +0x30 uses eligibility bit `0x100`. Both compare strict X/Z/Y bounds
  from a six-word stack array via `bp = box`, in X/Z/Y order, with Y
  half-range `0x14`. The actor probe uses X/Z half-range `0xC8` rather than
  `0x96`, has additional filters and flag writes, and omits the Y test on
  its alternate-position branch; this is a shared idiom, not identical logic.
- Original assemblies
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800BEF64/ovl_11_func_800BEF64.s`
  and
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800BF1D0/ovl_11_func_800BF1D0.s`
  corroborate the base addresses, strides, flag tests and bounds access.
  Both hold the record pointer in `$a3` and address the stack box through
  `$a2`. This describes emitted code, not proof of an original register
  variable or the original spelling of the C locals.

The pre-existing `800BF1D0` / `800BF2F4` pair remains supported by its
zero-gap adjacency (0x124-byte `800BF1D0` ends at `800BF2F4`) and shared
`bp = box` strict-bounds scan with X/Z half-range `0x96`.
`src/overlays/ovl_11/ovl_11_func_800BF2F4.c` scans a different, 0x18-stride
pool through `D_8006C838 + 0x8000 + 0x5D8C`, returning u16 at record +2.
The new actor-pool tie extends this candidate pair to include `800BEF64`;
it does not newly establish a common caller among these three leaves.

### Conditional boundaries

The two memberships are compatible local hypotheses, not proof that the
whole collision-dispatcher run and stack-box run formed one original TU.
No shared static, TU-owned global, declaration-order effect or explicit
register-variable quirk was established. The actor pool is also consumed
elsewhere; shared pool use alone does not pull distant consumers or callers
into either group. Keep the existing confidence levels and leave original
boundaries open.
