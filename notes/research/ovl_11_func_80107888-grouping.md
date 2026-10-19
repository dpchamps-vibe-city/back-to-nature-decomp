# ovl_11_func_80107888: nested guard and translation-unit evidence

Documentation-only review of the supplied byte-exact, passing-finalize handoff,
identity
`217716b77c9d3f084226b260fff5ef31e59edf745cb1826098af299a9009bcc6`
(origin: agent). The reviewed source
`src/overlays/ovl_11/ovl_11_func_80107888.c` has SHA-256
`a0244884dd0ef369ceab12e886aec82f76e8a5631df72f190a562f8c5cd4d7b0`.
No solver experiments, build-input edits or new binary verification were
performed in this review.

## New same-TU evidence

The guard `ovl_11_func_8010780C` and its sole caller
`ovl_11_func_80107888` have stronger evidence than their previously recorded
zero-gap adjacency and shared state reads:

- The original caller sets `$v0 = $sp + 0x10` at 0x801079C4 before calling
  the guard at 0x801079C8. The guard saves incoming `$v0` at `0x10($sp)` at
  0x80107818, with no subsequent read. Together these are a bilateral
  static-chain fingerprint, not merely an unexplained dead store.
- The verified source defines the guard inside the dispatcher, with an
  `auto` forward declaration and asm symbol label preserving the original
  guard name. The nested definition reproduces the static-chain machinery
  without the earlier `CAPTURE_PREV_RET` reconstruction.
- Supplied matching evidence reports that a declaration-only reconstruction
  left five dispatch words different. Compiling the nested definition made
  the parent exact and preserved the child's bytes, also checked by
  `make check-ovl_11`. The source comment identifies the cause as
  `cse_not_expected` surviving the child compilation, changing address
  expansion in the parent and placing its switch-index shift before the
  jump-table address. The compiler mechanism is documented separately in
  [the 800D1CFC investigation](ovl_11_func_800D1CFC-nested-function-cse-not-expected.md#4-the-mechanism-cse_not_expected-survives-a-nested-compile).
- Original link order agrees: 0x8010780C + 0x7C = 0x80107888; the parent
  occupies 0x2CC bytes through 0x80107B54 (exclusive). The emitted assembly
  puts the guard immediately before its parent in one compilation.

Original instruction evidence:
`build/ovl_11/asm/nonmatchings/ovl_11_func_8010780C/ovl_11_func_8010780C.s`
and
`build/ovl_11/asm/nonmatchings/ovl_11_func_80107888/ovl_11_func_80107888.s`.
Reconstruction evidence: the verified parent source,
`src/overlays/ovl_11/ovl_11_func_8010780C.c` (now an explanatory placeholder),
and `build/src/overlays/ovl_11/ovl_11_func_80107888.s`.
`configs/splat/ovl_11.yaml` corroborates the original address order and
associates the parent's jump table with its source.

## Scope of the inference

High-confidence shared original TU membership for this pair assumes that the
bilateral static-chain fingerprint and parent expansion effect reflect GCC's
nested-function machinery. Byte identity establishes a viable reconstruction,
not unique original source syntax. The modern placeholder and emitted object
arrangement describe the reconstruction, not independent original-TU evidence.

This does not establish either outer file boundary, raise confidence for the
whole 0x801075C0 guard run, or merge the pair with other nested-function
families merely because they share a compiler mechanism. The parent's write
to `D_8012D038` identifies its successful-selector role; proximity to other
`D_8012D0xx` words alone does not establish membership in their larger TU.
