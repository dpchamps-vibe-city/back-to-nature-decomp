# ovl_30_func_801326C4: translation-unit grouping evidence

Documentation-only review of the verified source
`src/overlays/ovl_30/ovl_30_func_801326C4.c`. The handoff and
`run_output/autoloop/state.json` record passing verification under identity
`bc81358be909b9cf5f0959af41b09b5b36759c6370d0aed5359403ebfa15d04b`.
The source SHA-256 is
`722ae292f48310e7bd8baef489baa333c7e00e849b640b668b3a6ba9b94bec9d`,
also recorded by the final exact experiment in
`build/experimentLedger/ovl_30_func_801326C4.jsonl`.
No solver work or build-input edits were made in this review.

## New grouping evidence

The verified function and its two immediate successors support a
medium-confidence original-TU candidate:

- **Gapless link run and mirrored editor idiom.** Each function is 0x2B4
  bytes: `801326C4 + 0x2B4 = 80132978`,
  `80132978 + 0x2B4 = 80132C2C`, and
  `80132C2C + 0x2B4 = 80132EE0`. The original assemblies share the same
  eight input tests, selector steps of one/ten, byte-value steps of one/ten,
  wrap tests, selected-value print, ten-entry bounded print loop and
  incoming-argument return. The variants change the selector, byte-array
  offset, bound and row format, rather than the overall editor structure.
- **Adjacent overlay-local selector cluster.** In link order they use the
  three consecutive zero-initialized words `D_80134AA8`, `D_80134AAC` and
  `D_80134AB0`, respectively. These are separate selectors, not a shared
  static established by the match.
- **Shared header and consecutive row formats.** All three pass
  `D_8012EED0` (`"\n\nEDIT.[%d] = %d \n\n"`) to `FntPrint`. Their row
  strings follow it consecutively at `D_8012EEE4` (Ke-Day-Cnt0),
  `D_8012EEFC` (Ke-Day-CNT1) and `D_8012EF14` (Ke-N-VAL). They edit
  contiguous byte regions of `D_8006C838`: +0xE640 with 0x80 entries,
  +0xE6C0 with eight entries and +0xE6C8 with 0x80 entries.

Instruction evidence is in
`build/ovl_30/asm/nonmatchings/<function>/<function>.s` for
`ovl_30_func_801326C4`, `ovl_30_func_80132978` and
`ovl_30_func_80132C2C`. The original data evidence is in
`build/ovl_30/asm/data/6188.data.s` (0x80134AA8–0x80134AB0) and
`build/ovl_30/asm/data/0.rodata.s` (0x8012EED0–0x8012EF2C).
`configs/splat/ovl_30.yaml` lists the three consecutively at offsets
0x48DC, 0x4B90 and 0x4E44; `build/ovl_30/functions.csv` corroborates
sizes and the two-FntPrint call shape.

## Limits

This is a same-file hypothesis, not a recovered file boundary. It assumes
that the mirrored code and parallel code/data/string ordering reflect
source-local editor organization; copied implementations in separate TUs
and pooled strings could explain individual ties. No private static,
GP-relative ownership, declaration-order effect or TU-specific register
quirk was established, and these three functions do not call one another.
`D_8006C838`, `D_8005E3A8` and `FntPrint` alone are too widely shared to
justify grouping. The following helper/item/tool-editor group remains
separate: adjacent selector words and generic debug printing do not by
themselves establish a common TU with that group.
