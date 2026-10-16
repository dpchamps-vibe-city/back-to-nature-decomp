# func_8001B5DC — object-setup grouping evidence

Verified source/evidence identity supplied by the finalization handoff:
`5de01c7147d574ffc5a64131be9498cafb4be49a5b1c4becfbcdc11b57a3ebdc`.
This is a documentation-only assessment, not a new verification run.

New evidence strengthens membership in the existing
`0x8001B530–0x8001BB88 object-setup cluster`:

- `src/func_8001B5DC.c` directly calls the camera builder
  `src/func_8001B6A0.c`. `configs/splat/exe.yaml` places B530, B5DC and B6A0
  consecutively; the target in
  `build/asm/nonmatchings/func_8001B5DC/func_8001B5DC.s` spans
  0x8001B5DC–0x8001B69F, ending exactly where B6A0 begins. Thus the call
  relationship corroborates link adjacency rather than adjacency alone.
- B5DC loads D_80061E68 as the GTE color matrix and D_80061E48 as the light
  matrix. `src/func_8001B530.c` initializes both objects, tying this apply
  routine to the existing setup cluster's adjacent-data fingerprint.

These support the cluster hypothesis, not a proven original translation-unit
boundary: the matrix references are absolute, and no shared static or exclusive
ownership evidence was established here. Keep the ledger's low–medium
confidence and existing range; replace B5DC's former padding/stub designation
with its verified camera/fog/lighting-apply role.
