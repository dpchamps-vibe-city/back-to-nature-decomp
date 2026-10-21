# ovl_17_func_800B9784 — grouping corroboration

The documentation handoff reports byte identity and a passed full finalize gate.
Verified source/evidence identity:
`0c66c2825513765f0263d4fdf512568753d80cf07f80d43eba38c483584a855c`.

## Membership evidence

The existing low-confidence `ovl_17` D_800BD848 record-area/caller-hub
entry already records the gapless adjacency
`0x800B9784 + 0xE8 = 0x800B986C`. Adjacency alone is not new evidence.

The verified body now corroborates that local connection: 800B9784 reads
D_800BD848's signed-halfword record selector at +0x2BC (overridden with
record 2 in mode 3), uses the same 0x50 stride and s32 position at
+0x38 as 800B986C, and adjusts the shared scrolling-speed halfword at
+0x20 relative to the origin at +0x24.

The call graph agrees with this link-order pair. Their sole caller,
800B8898, calls 800B9784 at 0x800B8A74 and 800B986C at 0x800B8BE0.
That caller also advances the same record positions, integrates the
+0x20 speed into the +0x24 origin at 0x800B8AD8–0x800B8B0C, and passes
the origin to existing cluster renderers 800BAC24 and 800BA808. Thus the
new member's recovered field accesses connect directly to the existing
hub's update/render path, not merely to an overlay-wide global name.

Together these facts support adding 800B9784 to the existing
low-confidence cluster. They do not establish an original translation-unit
boundary or justify increasing confidence; the reconstructed types and
current one-function source files are not original-TU evidence.

## Evidence

- Verified source: `src/overlays/ovl_17/ovl_17_func_800B9784.c`.
- Original accesses and extent:
  `build/ovl_17/asm/nonmatchings/ovl_17_func_800B9784/ovl_17_func_800B9784.s`.
- Adjacent member's selector, stride and position accesses:
  `build/ovl_17/asm/nonmatchings/ovl_17_func_800B986C/ovl_17_func_800B986C.s`.
- Common caller and field integration:
  `build/ovl_17/asm/nonmatchings/ovl_17_func_800B8898/ovl_17_func_800B8898.s`.
- Link extents and call lists: `build/ovl_17/functions.csv`.
