# ovl_30_func_80130C20: translation-unit grouping evidence

Documentation-only review of the verified source
`src/overlays/ovl_30/ovl_30_func_80130C20.c` and the supplied passing-finalize
handoff, source/evidence identity
`a524d63954618933985ad38ec743e71d49d69d9b4456161cfed2a10b72dd8913`.
The current source SHA-256 is
`948924cf1d9385781c49c8ba4dbb224586c4c8b4aeb3c7b4093368dd5025f775`,
also recorded by the exact experiment in
`build/experimentLedger/ovl_30_func_80130C20.jsonl`.
No solver work, build-input edits or new build were performed.

## New evidence: adjacent animal-record editors

The verified function supports a previously unrecorded medium-confidence
same-TU candidate with `ovl_30_func_801306F4` and `ovl_30_func_80131100`:

- **Gapless link run.** Original function sizes give
  `801306F4 + 0x52C = 80130C20`,
  `80130C20 + 0x4E0 = 80131100`, and
  `80131100 + 0x4E0 = 801315E0`.
  `configs/splat/ovl_30.yaml` lists the three editors consecutively at
  offsets 0x290C, 0x2E38 and 0x3318.
- **Shared labels and mirrored editor protocol.** All three construct local
  20-byte menu-entry arrays using the animal-field labels at
  `D_8012E868`–`D_8012E988`. Each gates on `func_80013394`, prints its
  selector with `KanjiFntPrint`, passes its menu to `80133454` through the
  shared cursor `D_80134014`, and uses input bits 4/8 to call `80133974`
  with a four-byte selector and delta -1/+1. The predecessor has 25 entries;
  the verified editor and successor each have 23. This is a shared-callee
  sibling tie, not a direct call between the editors.
- **Selector and heading order agree with code order.** Their selector
  words are respectively `D_801349A0`, `D_801349A4` and `D_801349A8`,
  consecutive initialized words in the original data. Their respective
  heading strings, `D_8012E994`, `D_8012E9B8` and `D_8012E9D8`, likewise
  form a consecutive rodata run immediately after the shared labels.
  The selectors address separate animal-record bases: `D_800749F4`,
  `_D_800742EC` and `D_80075FE4`; the verified source names its view
  `Ovl30_AnimalRecords`.

Original instruction evidence is in
`build/ovl_30/asm/nonmatchings/<function>/<function>.s` for the three
members. Original headings/labels and selector words are in
`build/ovl_30/asm/data/0.rodata.s` (0x8012E868–0x8012E9FC) and
`build/ovl_30/asm/data/6188.data.s` (0x801349A0–0x801349A8).
The two neighbours remain assembly stubs in the current source; their
roles here come from original instructions.

## Limits

This hypothesis assumes the mirrored adjacent editors and matching
selector/heading order reflect source-local organization. Separate files
using common declarations or pooled strings remain possible. No private
static ownership, declaration-order effect or TU-specific register quirk
was established. The shared cursor, input source and menu helpers are used
elsewhere in the overlay; those references alone do not justify merging
this group with the earlier value-menu wrappers or the distant
`80133454`/`80133974`/`80133A8C` group. Neither outer file boundary nor
membership of the next editor `801315E0` is established by this review.
