# ovl_11_func_800C5098: adjacent request-state initializer/consumer

## Verified anchor

The documentation handoff reports byte identity and a passed full finalize gate
for source/evidence identity
`033ebea43aefefaae9c6d7c85401d591bd57e99ba59dbd63de3e741d3251a213`.
Its changed-file list is `include/globals_override.h`, `include/sdk_types.h`
and `src/overlays/ovl_11/ovl_11_func_800C5098.c`. This review changes notes
only; it does not rerun matching or alter build inputs.

## New membership evidence

- **Shared request-state cluster with an existing member.**
  `src/overlays/ovl_11/ovl_11_func_800C5048.c` initializes the three words
  `D_80128CB4`, `D_80128CB8`, `D_80128CBC` and five halfwords
  `D_80128CC0`, `D_80128CC2`, `D_80128CC4`, `D_80128CC6`, `D_80128CC8`.
  The verified target consumes all eight: it compares and advances the
  counter against the limit, adds the coordinate offsets, selects the
  grid/action and lookup tier, and updates the completion result. Once
  the counter exceeds the limit, it passes that result to 800CD5BC.
  This is the consumer of the same request state, not just another user
  of the public grid tables or a reconstructed object type.
- **The initializer immediately precedes its consumer.**
  `configs/splat/ovl_11.yaml` places 800C5048 at offset 0xD228 and
  800C5098 at 0xD278: a 0x50-byte initializer directly before the target.
  The original target assembly,
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800C5098/ovl_11_func_800C5098.s`,
  records a 0x1A4-byte body ending at 800C523C. It independently shows
  the shared-state loads and counter/result stores, including the
  completion call at 800C5200 and counter update at 800C5210–800C521C.
- **The request connects to the already-recorded local handler family.**
  The sources of 800C4B6C, 800C4C94 and 800C4DE0 call 800C5048 with
  operation selectors 0, 1 and 2; the target consumes that selector from
  `D_80128CC6`. Their common-dispatcher and link-order evidence is recorded
  in [the sibling review](ovl_11_func_800C4B6C-grouping.md).
  The initializer and target communicate through globals, not a direct call.

The evidence-backed declarations in `include/globals_override.h` agree with
these accesses; their addition is not itself evidence of original linkage.

## Inference and limits

Add 800C5098 to the existing local dispatch-caller TU candidate recorded under
the ledger's tier/clamp section, retaining **medium confidence**. The premise
is that this adjacent initializer/consumer pair and its shared eight-symbol
request state belong with the initializer's existing local handler family.
Neither original TU boundaries nor private linkage is established: the target
uses absolute addresses, not proven file-local statics. Do not automatically
add its successor 800C523C or other intervening functions.

Calls to 800D5960, 800D806C, 800D8428, 800D8320, 800D876C and 800CD5BC do
not merge their candidate groups with this one. In particular, the target
remains distinct from both the distant grid-entry table cluster and the
tier/clamp run under which its local family's ledger cross-reference resides.
