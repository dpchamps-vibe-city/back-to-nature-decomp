# ovl_11_func_800C4B6C: tier-table grid-action siblings

## Verified anchor

The documentation handoff reports byte identity and the full finalize gate
passed for source/evidence identity
`5ebddf625c80344f5c6fdb8e62b19402321e317fc784d63b3e015a3d01028781`.
Its changed inputs are `src/overlays/ovl_11/ovl_11_func_800C4B6C.c` and
`include/globals_override.h`. This is a notes-only evidence review, not a
new verification run.

## New grouping evidence

The existing local dispatch-family candidate is recorded under the ledger's
tier/clamp section and explained in
[the 800C55FC review](ovl_11_func_800C55FC-grouping.md). That review left
membership of the earlier 800C4A2C–800C523C band unresolved. The verified
800C4B6C body and its immediate assembly neighbours now support a specific
extension:

- **Adjacent data and code in corresponding order.**
  `build/ovl_11/asm/data/69960.data.s` defines three consecutive six-halfword
  tables at `D_8012319C`, `D_801231A8` and `D_801231B4`. The consecutive
  handlers 800C4B6C, 800C4C94 and 800C4DE0 respectively index those tables
  by the signed object halfword at +0x18 divided by 20. Their original
  assembly sizes are 0x128, 0x14C and 0x164, so the first two end exactly
  at the next handler; the third ends at 800C4F44.
- **Shared dispatch and request idiom.** All three obtain a mode from
  800C9D64, use `D_80123234[mode]` for position-to-cell conversion outside
  mode 2, and call nearby 800C5048 with operation selectors 0, 1 and 2
  respectively. Their successful tails call 800C70CC with `(object, 0x68,
  0, -2)` and return 1. The source of 800C5048 shows it initializes the
  shared request-state block `D_80128CB4`–`D_80128CC8`.
- **Call graph corroborates the local link order.** The already-recorded
  dispatcher 800C580C calls 800C4DE0 at 800C64AC, 800C4B6C at 800C6538
  and 800C4C94 at 800C6574, passing its same `$s1` object to each. It also
  calls the existing family's 800C541C, 800C55FC and 800C5738. The new
  triple shares their mode-selected callback table, not merely a
  reconstructed object type.

Original handler evidence:
`build/ovl_11/asm/nonmatchings/ovl_11_func_800C4B6C/ovl_11_func_800C4B6C.s`,
`build/ovl_11/asm/nonmatchings/ovl_11_func_800C4C94/ovl_11_func_800C4C94.s`
and
`build/ovl_11/asm/nonmatchings/ovl_11_func_800C4DE0/ovl_11_func_800C4DE0.s`.
Caller evidence:
`build/ovl_11/asm/nonmatchings/ovl_11_func_800C580C/ovl_11_func_800C580C.s`.
Helper source: `src/overlays/ovl_11/ovl_11_func_800C5048.c`.
Link order: `configs/splat/ovl_11.yaml`, offsets 0xCD4C, 0xCE74,
0xCFC0, 0xD228 and 0xD9EC for the new triple, helper and dispatcher.

## Membership inference and limits

Extend the existing local dispatch-family candidate with 800C4B6C,
800C4C94, 800C4DE0 and their nearby request initializer 800C5048,
**medium confidence**. The premise is that the ordered table/handler
cluster, shared callback/request idiom and nearby common dispatcher reflect
one original source unit. Separate linked units remain possible: neither
the tables' original linkage nor the original TU boundaries is established.
The intervening functions are not assigned automatically.

Calls to distant 800C70CC, 800C9D64 and 800CD5BC do not merge their candidate
TUs with this local family. In particular, this addition remains distinct
from the tier/clamp run under which the ledger cross-reference resides.
