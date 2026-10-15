# ovl_11_func_800E499C: shared-state coordinator in the pointer-getter run

The verified function strengthens its existing candidate membership in the
0x800E48CC–0x800E516C pointer-getter run. Retain medium confidence and the
existing range; this does not establish original TU boundaries.

New grouping evidence:
- `src/overlays/ovl_11/ovl_11_func_800E499C.c` initialises the adjacent
  halfwords `D_80129410`/`D_80129412` before calling `ovl_11_func_800E4BA4`.
  That neighbour's source reads both globals for its `func_80017240` calls.
  `src/overlays/ovl_11/ovl_11_func_800E4DAC.c` also consumes the pair during
  sprite initialisation. This adds the coordinator/writer to the previously
  documented reader cluster.
- After relocating sprite data, 800E499C writes its destination to
  `D_801291B8`, then calls 800E4D08 to populate `D_801291C0` and 800E4DAC to
  consume the relocated base and offset table. The latter two sources show
  that producer/consumer chain; the ledger previously recorded their shared
  offset table but not 800E499C's relocated-base contribution.
- The same coordinator directly calls 800E4AEC, 800E4B6C, 800E4BA4,
  800E4C30, 800E4C84, 800E4D08, 800E4DAC and the documented tail 800E5078.
  These calls corroborate the existing link-contiguous helper run rather
  than relying on similar operations alone.

Target-assembly anchors:
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800E499C/ovl_11_func_800E499C.s`:
  0x800E49C8–0x800E49F0 writes the halfword pair; 0x800E4A74 calls 800E4BA4;
  0x800E4AA0–0x800E4AAC publishes the relocated base and calls the two
  consumers. Its 0x150-byte extent ends at the next member, 800E4AEC.
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800E4DAC/ovl_11_func_800E4DAC.s`:
  0x800E4E04–0x800E4E38 combines `D_801291C0` offsets with `D_801291B8`;
  0x800E4E58–0x800E4E64 consumes the halfword pair.

These are shared absolute-addressed globals, not demonstrated file-local
statics or definitions proving data ownership. Same-TU membership remains
conditional on the existing run hypothesis. Neither the widespread
`D_8006C838` access nor the distant `func_800120BC`/800CF36C calls justifies
merging other groups, and this evidence does not add the successor function
800E516C to the run.

Verified source/evidence identity supplied by the finalization handoff:
`f64982d587e686abc2cd185f28aadbdf689e8367e13efa45bff522665b9fdd9e`.
Origin: agent. Supplied changed file:
`src/overlays/ovl_11/ovl_11_func_800E499C.c`.
This records the supplied byte-exact, full-finalize result; no solver or
build-verification pass was repeated for this notes-only update.
