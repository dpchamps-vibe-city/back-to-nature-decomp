# ovl_11_func_800F6E38: texture-ID group quad-caller extension

## Verified premise

The supplied finalize identity is
`c72da02e63305e4734b805f33d181a3b56ce368275218728677996361f7d3593`.
`run_output/autoloop/state.json`, entry
`completions.ovl_11_func_800F6E38`, records agent origin, verification passed,
and the sole changed file as `src/overlays/ovl_11/ovl_11_func_800F6E38.c`.
Its recorded source SHA-256,
`325c244df7da8a959e42c087f64c027ec3fdcd7de05d52454ca4425d1931a7b0`,
matches the verified source read for this documentation. No solver or new
build was run.

## New membership evidence

Extend the existing [texture-ID load/lookup caller run](ovl_11_func_800F6A24-grouping.md)
with `ovl_11_func_800F6E38`, the immediate successor of `800F6D14`.
Candidate members are `800F6A24`, `800F6C78`, `800F6D14`, and `800F6E38`
(medium confidence; original TU boundaries remain unproven).

- **Shared lookup and reclaim/load lifecycle.** The verified target scans
  72 records at `D_80129648`, stride `0x22`, comparing the unsigned ID at
  `+0x20` and setting the active byte at `+0x1E` on a hit. On a miss it
  searches backward from record 71 for an inactive record, clears its ID,
  and calls `800F6A24` to load the requested texture. These are the same
  fields, bounds, traversal directions, and loader used by the immediate
  predecessor `800F6D14`, not merely a shared table reference. Original
  evidence:
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800F6E38/ovl_11_func_800F6E38.s`
  (`0x800F6E74–0x800F6EC8`, `0x800F6ECC–0x800F6F1C`) and
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800F6D14/ovl_11_func_800F6D14.s`
  (`0x800F6D60–0x800F6DAC`, `0x800F6DB8–0x800F6E18`). The target dispatches
  quad drawing through `800F8F3C` and retries itself after loading
  (`jal` at `0x800F6F08`); the predecessor dispatches sprite drawing through
  `800F8CF8` and retries through `800F6C78`. Unlike the predecessor, the
  target does nothing for ID zero.
- **Call graph corroborates the gapless sibling run.** The predecessor's
  `0x124` bytes end exactly at the target:
  `0x800F6D14 + 0x124 == 0x800F6E38`. The target occupies `0x108` bytes,
  ending at `0x800F6F40`. `configs/splat/ovl_11.yaml` independently places
  the pair at offsets `0x3EEF4` and `0x3F018`, following the existing loader
  and lookup helper at `0x3EC04` and `0x3EE58`. Both siblings call the loader
  at the head of this consecutive run (`jal` at `0x800F6D94` and
  `0x800F6EF0`); `build/ovl_11/functions.csv` corroborates their sizes and
  call edges. No direct call between the siblings is claimed.

## Limits

The inference combines the immediate sibling link order with matching record
lifecycle and a shared loader inside the existing run. It does not establish
static/GP-relative ownership, a declaration-order effect, a register-variable
quirk, or an exact original TU boundary. The modern shared record view is
access evidence, not proof of the original C type spelling.

The distant [texture-draw pair](ovl_11_func_800F8F3C-grouping.md) remains a
separate candidate; the target's drawing dependency does not bridge the
intervening code into one TU. No table-wide TU is claimed, and the following
`800F6F40` is not added on link adjacency alone.
