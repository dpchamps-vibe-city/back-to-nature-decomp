# ovl_17_func_800B92B0 — grouping corroboration

The documentation handoff reports byte identity and a passed full finalize gate.
Verified source/evidence identity:
`f664551b5faa2f6cc2ba4effdf9813b4952a6e64c36ec6dd7e93952cebd85403`.

## Membership evidence

The existing low-confidence `ovl_17` D_800BD848 record-area/caller-hub
entry already records 800B92B0's six-record, 0x50-stride D_800BD870 walk
and its call to 800BAD9C. Those facts are not new grouping evidence.

Additional corroboration is the gapless link adjacency to existing hub
member 800B9324 (`0x800B92B0 + 0x74 = 0x800B9324`) together with their
common caller 800B8898: its alternate branch calls 800B92B0 at 0x800B8914,
while its update path calls 800B9324 at 0x800B8A24. Thus the call graph and
link order agree on this local pair.

The verified body also connects three existing display-cluster members:
800BAF50 setup, six 800BAD9C record draws, then 800BA808 backdrop rendering.
Both render paths receive the s32 at D_800BD848+0x24; the record walk
reaches that work-area base through D_800BD870-0x28. In 800B8898's update
path, the corresponding six-record 800BAC24 walk and final 800BA808 call
use the same work-area field.

These observations support adding 800B92B0 to the existing low-confidence
cluster. They do not establish an original translation-unit boundary;
shared overlay-wide storage and call edges alone would be insufficient.

## Evidence

- Verified source: `src/overlays/ovl_17/ovl_17_func_800B92B0.c`.
- Original extent, calls and loads:
  `build/ovl_17/asm/nonmatchings/ovl_17_func_800B92B0/ovl_17_func_800B92B0.s`.
- Link extents and call lists: `build/ovl_17/functions.csv`.
- Common caller and alternate rendering path:
  `build/ovl_17/asm/nonmatchings/ovl_17_func_800B8898/ovl_17_func_800B8898.s`.
