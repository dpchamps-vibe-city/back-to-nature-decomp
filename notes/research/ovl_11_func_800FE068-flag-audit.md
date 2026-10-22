# ovl_11_func_800FE068: withdrawn flag-based grouping

The actual matched `src/overlays/ovl_11/ovl_11_func_800FE068.c` is
byte-identical to the original after relocation (57/57 words) both under
baseline overlay flags (`-G0`) and with `-fno-strength-reduce`.
The explicit baseline compilation used `useOverrides: false`.
The obsolete override was removed; the full finalization gate passed.

Evidence: `build/ovl_11_func_800F8224/existing-flag-audit.json` and
`build/ovl_11_func_800FE068/existing-flag-audit/{baseline,no-strength-reduce}/`.
The audit driver is `build/ovl_11_func_800F8224/audit-existing-flags.ts`.
These results are scoped to the current matched source; an older draft's
strength-reduced walker does not establish the original translation unit's flags.

Therefore the prior flag-based separation from baseline-matched neighbours,
and the proposed shared-flag association with `ovl_11_func_800F8224`, are
unsupported. This establishes no new original-TU membership. The existing
zero-gap predecessor and shared text-draw path remain the grouping evidence.
The verified source visits threshold entries 1 through 4, not the old ledger's
three-iteration description.
