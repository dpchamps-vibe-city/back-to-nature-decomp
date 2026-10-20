# ovl_11_func_8010EE5C: nested helper / parent grouping

## Scope

The verified `src/overlays/ovl_11/ovl_11_func_8010EE5C.c` supplies new
same-original-TU evidence for **8010EE5C / 8010EEE4 only**. The documentation
handoff reports an exact match and a passed full finalize gate (evidence
identity `d3d253246c690a50ffa32c7bbf390b9502ea3ecc7723dcdf636ef949349b3fac`).
The existing `build/pipelineReversal/ovl_11_func_8010EE5C/report.json` also
records byte identity for all 34 words. The parent remains an assembly stub.

## Evidence

The original listings are
`build/ovl_11/asm/nonmatchings/ovl_11_func_8010EE5C/ovl_11_func_8010EE5C.s`
and
`build/ovl_11/asm/nonmatchings/ovl_11_func_8010EEE4/ovl_11_func_8010EEE4.s`.

- **Bilateral static-chain connection:** the parent allocates 0x48 bytes,
  stores its incoming stack pointer at current `sp+0x28` (8010EEE8,
  8010EF0C), and spills its object argument at `sp+0x48` (8010EF18).
  Its call paths seed entry `$v0` with `sp+0x18` (8010EF2C, 8010EF48,
  8010EF60, 8010EF80) before calling the helper at 8010EF6C / 8010EF8C.
- The helper preserves entry `$v0` (8010EE70 / 8010EE80), then loads
  `chain+0x10` and dereferences that pointer (8010EEC0 / 8010EEC8) for
  the call to `80110544`. In the caller's frame these loads resolve to
  `*(sp+0x28) == sp+0x48`, then the captured outer object argument.
  This agrees with the verified C's captured-chain use, not merely a
  dead entry-register spill or an ordinary call dependency.
- **Independent link adjacency:** the 0x88-byte helper starts at
  0x8010EE5C and ends exactly at its parent, 0x8010EEE4. The parent is
  0x440 bytes, ending at 0x8010F324. `configs/splat/ovl_11.yaml` records
  the corresponding consecutive offsets 0x5703C, 0x570C4 and 0x57504.

## Membership and limits

Under the established GCC nested-function/static-chain interpretation,
this caller-frame capture plus gapless helper/parent order supports high
confidence that the pair belonged to one original translation unit.
8010EE5C is its nested state/flag helper; 8010EEE4 is the enclosing
object-mode handler.

This does **not** add `80110544` to that TU: it is an ordinary callee of
the helper. Nor does the parent's existing `D_800BAABC` dispatcher-family
connection prove common TU membership with the dispatcher, other table
entries, or either outside neighbour. No wider boundary is established.
