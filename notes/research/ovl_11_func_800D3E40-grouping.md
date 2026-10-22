# ovl_11_func_800D3E40 grouping evidence

Verified source/evidence identity supplied by the handoff:
`a31e5f47afb0300a767632481fb5d92ea5819a81f8b2d724bced0cd10a889a69`.
The handoff reports byte identity and a passing full finalize gate. This
notes-only review does not repeat solver work or change build inputs.

## Candidate adjacent selector helpers

The verified bodies in `src/overlays/ovl_11/ovl_11_func_800D3D60.c` and
`src/overlays/ovl_11/ovl_11_func_800D3E40.c` support a low-confidence
same-TU candidate pair:

- **The same two record families:** both scan twenty 0xB8-byte records at
  `D_800749F4` and ten 0xB4-byte records at `D_800742EC`.
  `800D3D60` derives the second base as `D_800749F4 - 0x708`, whereas
  `800D3E40` names it directly. The identity, counts and strides are
  witnessed in their original listings under
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800D3D60/` and
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800D3E40/`.
- **Related selection predicates:** both require a nonzero unsigned halfword
  at +0, inspect the signed halfword at +0x30, and reject flag 0x20000 at
  +0x34. `800D3D60` chooses a random eligible index from the selected family
  and also rejects 0x400. `800D3E40` searches the large family before the
  small one, adds strict X/Z range tests, and returns the first matching
  index with record-category and object-pointer outputs. This is the same
  specialized record cluster, not merely a common SDK call.
- **Gapless original link order:** `800D3D60` has original size 0xE0 and
  ends exactly at `800D3E40`. `configs/splat/ovl_11.yaml` independently
  records consecutive offsets 0x1BF40 and 0x1C020. The latter function has
  size 0x1AC and ends at the following `800D3FEC`; that following function
  is not included on adjacency alone.

## Limits

These are candidate members, not recovered original translation-unit
boundaries. Both arrays are shared main-RAM globals, not private overlay
statics. No common caller, static linkage, declaration-order effect or
original register-variable convention has been demonstrated for the pair.
The matching C's statement scopes are reconstruction choices, not evidence
of the author's original syntax or a shared source-file quirk.

The distant selector callers `800D3230`/`800D3250` retain their separate
candidate grouping. The position-query caller `800D5224` and the similar
helper `801096FC` are not added to this pair solely for call or ABI overlap.
