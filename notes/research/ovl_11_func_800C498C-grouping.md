# ovl_11_func_800C498C grouping evidence

Verified handoff identity: `2507dfaa245637a6e297d7251d88e7d7770f9f328a1012c2d08f8a483735314b` (origin: agent).
The supplied handoff reports byte identity and a passed full finalize gate;
this notes-only pass does not repeat or modify that verification.

## Caller/link extension

- Current source: `src/overlays/ovl_11/modules/primary_object_update.c`,
  `Ovl11ProbePrimaryObjectSurface` (original ovl_11:800C498C).
  It calls `800CB114` with the object's position and selector 0x18, probes
  a direction-offset position through `800BDB88`, and caches the getter's
  flags in `D_80128C50`, substituting 0x8000 when the upper half is zero.
- Caller evidence:
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800C401C/ovl_11_func_800C401C.s`
  calls the target at `0x800C4118` and the already-grouped `800C47F0` at
  `0x800C41D4`. Searching the overlay's generated assembly finds no other
  named reference to the target outside its own definition.
- Original extents in the corresponding
  `build/ovl_11/asm/nonmatchings/<function>/<function>.s` files are
  `800C401C + 0x7D4 = 800C47F0`,
  `800C47F0 + 0x19C = 800C498C`, and
  `800C498C + 0xA0 = 800C4A2C`.
  `configs/splat/ovl_11.yaml` records the same order. Thus the caller and
  its two helpers form a gapless run, not merely a distant call edge.

The current campaign additionally checked original jal targets and stored
address words throughout the EXE and all overlay payloads: the C4118 call is
the only reference found, with no stored pointer to C498C. Together with the
zero-gap helper run and its in-frame ground-contact role, this supports the
three-member update unit. The baseline checked fold passes all three and the
complete overlay. The broadcast result word is now gOvl11PrimarySurfaceFlags.

## Limits

The target does not reference the pair-private `D_80128CF8` or the
`D_801231F4`/`D_801231F8` flags. Its association rests on the two-helper
call/link run, conditional on the existing caller/helper grouping; it does
not raise the pair's confidence or establish original TU boundaries.
Neither the shared offset table nor calls to distant helpers establish
same-TU ownership for their other users. The successor `800C4A2C` starts the
closed cell-action table/request protocol and is not added on adjacency alone.
The original-byte census improves on a generated-name grep, but cannot exclude
an indirect address computed without a literal. Overlay -G0 leaves this a
supported reconstruction inference, not proof of historical object boundaries.
