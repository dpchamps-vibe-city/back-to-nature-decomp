# ovl_11_func_8010C9B4: grouping corroboration

Documentation-only review of the verified source. Supplied source/evidence
identity: `2b040df6a94e8b441e08898c7e3dff89b87a16de9f3b85cd495b13d9a0eac571`.
The handoff reports byte-exactness and a passed full finalize gate; this review
did not rerun matching or change build inputs.

## Observations and evidence

- `src/overlays/ovl_11/ovl_11_func_8010C9B4.c` walks the seven records at
  `D_8007A638`, stepping by 0xF8. It calls `ovl_11_func_8010CB6C` on each
  record whose u16 field at +0xB8 has bit 0x20 clear.
- The original target assembly,
  `build/ovl_11/asm/nonmatchings/ovl_11_func_8010C9B4/ovl_11_func_8010C9B4.s`,
  and the immediately preceding function's assembly,
  `build/ovl_11/asm/nonmatchings/ovl_11_func_8010C95C/ovl_11_func_8010C95C.s`,
  are both 0x58 bytes. Their instruction words agree except for the `jal`:
  the predecessor calls `8010CAD4`, whereas the target calls `8010CB6C`.
  Both use the same array base, stride, seven-record bound and +0xB8 gate.
- `configs/splat/ovl_11.yaml` lists the contiguous sequence at offsets
  0x54B3C (`8010C95C`), 0x54B94 (`8010C9B4`), 0x54BEC (`8010CA0C`),
  0x54C88 (`8010CAA8`), 0x54CB4 (`8010CAD4`), and 0x54D4C (`8010CB6C`).
  Thus both sweep callees lie shortly after the twin callers in the same
  recorded link run. The existing ledger already records the array cluster
  and its gapless continuation toward the object-mode dispatch cluster.

## Grouping interpretation

The adjacent sweep twin and the target's direct call to nearby `8010CB6C`
add corroboration beyond the ledger's previous generic array-reader role.
They strengthen the existing medium-confidence `D_8007A638` candidate and
provide call-graph support for its proximity to the object-mode dispatch run.

This remains candidate same-TU evidence, not proof: conventional loop code
can recur across translation units, and a nearby call can cross a TU boundary.
No original static linkage, register-variable declaration or declaration-order
constraint was established. Keep the existing groups and confidence; do not
merge them or infer intervening functions' membership from contiguity alone.
