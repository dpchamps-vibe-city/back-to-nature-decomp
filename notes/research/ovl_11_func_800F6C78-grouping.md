# ovl_11_func_800F6C78: candidate texture-ID lookup pair

## Verified premise

The supplied finalize identity is
`f5e6de5486543f7435ea3d3598782deafb57bdb0912182bb9120439611fdd98f`.
`run_output/autoloop/state.json`, entry
`completions.ovl_11_func_800F6C78`, records agent origin, verification passed,
and the sole changed file as `src/overlays/ovl_11/ovl_11_func_800F6C78.c`.
Its recorded source SHA-256,
`5456cb29ee7e063750e218c68d466b934ba1b384e2714f2de5fd095747cd240f`,
matches the source read for this documentation. No solver or new build was run.

## New membership evidence

Candidate members: `ovl_11_func_800F6C78` and its immediate successor/caller
`ovl_11_func_800F6D14` (medium confidence; original TU boundaries unproven).

- **Shared record lookup and state handling.** The verified target scans
  72 records at `D_80129648`, stride `0x22`, comparing the unsigned ID at
  `+0x20`. On a hit it sets byte `+0x1E` to 1 and passes the index to
  `ovl_11_func_800F8CF8`. The successor duplicates this lookup/hit path,
  including the scan bound and field offsets. Both also send ID zero to
  `ovl_11_func_800F7010` with the destination advanced by four bytes.
  Original evidence:
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800F6C78/ovl_11_func_800F6C78.s`
  (`0x800F6C98–0x800F6CA4`, `0x800F6CB0–0x800F6D00`) and
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800F6D14/ovl_11_func_800F6D14.s`
  (`0x800F6D48–0x800F6D54`, `0x800F6D60–0x800F6D80`,
  `0x800F6DB8–0x800F6DE8`).
- **Call graph and gapless link order agree.** The target's `0x9C` bytes end
  exactly at `0x800F6D14`; `configs/splat/ovl_11.yaml` independently places
  the entries at offsets `0x3EE58` and `0x3EEF4`. On a lookup miss, the
  successor searches backward for a record whose active byte is zero,
  clears that record's ID, calls `ovl_11_func_800F6A24`, then calls the
  preceding target to retry the lookup/draw (`jal` at `0x800F6DA8`).
  The successor ASM above and `build/ovl_11/functions.csv` corroborate the
  direct caller/callee edge.

## Limits

The inference combines the shared lookup/state idiom with an immediate
caller/callee link-order pair; it does not establish static ownership,
GP-relative ownership, a declaration-order effect, or a register-variable
quirk. The verified C record view is access evidence, not proof of the
original type spelling.

The distant `800F8CF8`/`800F8F3C` drawing pair remains a separate candidate
(see `ovl_11_func_800F8F3C-grouping.md`). Its shared table and the target's
call to `800F8CF8` do not prove a common TU across the intervening code.
No table-wide TU, additional neighbour membership, or exact boundary is
claimed here.
