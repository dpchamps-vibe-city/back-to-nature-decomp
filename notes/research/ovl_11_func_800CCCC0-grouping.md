# ovl_11_func_800CCCC0: local object-reset caller/helper grouping

## Verified anchor

Supplied source/evidence identity:
`201f4ad70585f4d86a470f2e3dc5b42407be8eea7d5e262ba2d5a75368c3bb8a`
(origin: agent). The handoff reports byte identity and a passed full finalize
gate for `src/overlays/ovl_11/ovl_11_func_800CCCC0.c`, with
`include/globals_override.h` and `include/sdk_types.h` also listed as changed.
This notes-only review accepts that verification; it does not modify or
re-verify build inputs.

## New membership evidence

- **A local call edge now corroborates the recorded link run.**
  `configs/splat/ovl_11.yaml` lists 800CCC5C, 800CCCC0, 800CCF14,
  800CCF58, 800CCFF8 and 800CD08C at offsets 0x14E3C, 0x14EA0,
  0x150F4, 0x15138, 0x151D8 and 0x1526C. Their successive extents are
  0x64, 0x254, 0x44, 0xA0 and 0x74, with no gaps. In
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800CCCC0/ovl_11_func_800CCCC0.s`,
  800CCCF4 calls 800CD08C with `&D_80071A00`. Thus the helper immediately
  after the previously listed run is a nearby callee of its newly verified
  reset routine, not merely another adjacent symbol.
- **The caller and helper operate on the same object flag and share a
  reset callee.** The target tests object `+0x6C` bit 0x1000 before that
  call and subsequently masks the same word and calls 800CB4E0 at
  800CCD44. `build/ovl_11/functions/overlay/ovl_11_func_800CD08C.s`
  loads `+0x6C`, clears bit 0x1000, stores it in the delay slot of the
  800BD168 call, then calls 800CB4E0 at 800CD0A8. The helper's 0x34-byte
  extent ends at 800CD0C0. Its source,
  `src/overlays/ovl_11/ovl_11_func_800CD08C.c`, expresses the same sequence.
- **An intervening member repeats the flag/reset idiom.**
  `src/overlays/ovl_11/ovl_11_func_800CCF58.c` masks the argument object's
  `+0x6C` flag word and calls 800CB4E0 on its zero-item path. This supports
  a local object-reset family within the link run, independently of the
  new caller's reconstructed struct names.

## Function role

800CCCC0 resets fields and flags of the global object at D_80071A00 and
its work area, conditionally applies gauge adjustments through 800D622C,
updates the time-derived counter and runs follow-up object helpers. The
angle-dependent path uses the previously recorded 800CD578/800CD534 pair.
The work-area pointer is visibly derived as object minus 0x51C8 at
800CCD18, corroborating the source's D_8006C838 view.

## Inference and limits

Extend the existing **low-confidence candidate same-TU caller run** to
include 800CD08C and mark 800CCCC0 matched. The premise is that the local
caller/helper edge, repeated object-flag/reset pattern and gapless link
order reflect one source unit. This replaces the ledger's previous claim
that adjacency was the only tie; it does not prove all intervening
functions share a TU or establish either outer boundary.

D_80071A00 and D_8006C838 are widely shared, the sub-base idiom occurs
elsewhere, and 800CD08C also has distant callers (800C580C and 8011FFA8).
Neither the globals nor the callees are proven private. Do not merge this
run with the broad D_80071A00 pool, the distant 800CB4E0/800CB540 pair,
the clamp-leaf run or the state-switch dispatcher family on this evidence.
The published object type and unused declarations in the verified source
are not original-TU evidence; no register-variable, declaration-order or
SDK-idiom fingerprint is established here.
