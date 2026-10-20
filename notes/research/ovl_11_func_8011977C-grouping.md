# ovl_11_func_8011977C — adjacent descriptor-wrapper run

The agent handoff reports byte-exact source and a passed full finalize gate,
with source/evidence identity
`1cd9692d242b7fb34e628ea64952d5eb63127c425041aeb7036cf6eec9e72f6f`.
Its changed-file list is `include/globals_override.h`, `include/sdk_types.h`
and `src/overlays/ovl_11/ovl_11_func_8011977C.c`. This review is documentation
only; no matching experiments or build-input edits were performed.

## New grouping evidence

Low-confidence candidate same-TU membership is supported for
`ovl_11_func_801194DC`, `ovl_11_func_801195BC`, `ovl_11_func_8011969C`
and `ovl_11_func_8011977C`:

- **Shared record and descriptor-call idiom.** The verified target constructs
  three two-halfword message pairs and a 0x14-byte by-value descriptor, with
  `D_8012D548` as its first word and 4 as its last word, then calls
  `ovl_11_func_8011C6CC`. Each of the three predecessor listings constructs
  the same argument layout using the same record, message bank 2 and final
  word 4, then calls that helper. The predecessors gate the call on
  `D_8006C838 + 0x44D4` / `+0x44BA`, selecting variants 0, 1 and 2;
  the target supplies a different fixed message set without those gates.
- **Gapless link order.** `configs/splat/ovl_11.yaml` lists the four entries
  consecutively at offsets `0x616BC`, `0x6179C`, `0x6187C` and `0x6195C`.
  Their original listings give sizes 0xE0, 0xE0, 0xE0 and 0xA4,
  respectively, so each starts exactly where its predecessor ends; the
  target ends at `0x80119820`.
- **Callback order corroborates code order.** In
  `build/ovl_11/asm/data/3408.rodata.s`, consecutive words
  `0x800BB274`–`0x800BB280` of `D_800BB248` name precisely these four
  functions in code-address order. The table is dispatched by
  `src/overlays/ovl_11/ovl_11_func_801189C8.c` using `D_8012D520`.

The original listings are
`build/ovl_11/asm/nonmatchings/<function>/<function>.s` for each of the four
full function names above. Together with the verified target source, they
support a local wrapper family through shared record use, argument-building
idiom and matching callback/link order, rather than proximity alone.

## Limits

This is candidate membership, not proof of original TU boundaries or ownership
of `D_8012D548`. The record and helper have other clients, callback tables can
span translation units, and the reconstructed descriptor type/prototype is not
itself evidence of an original shared declaration. No shared private static,
register-variable quirk or declaration-order effect was established.

Do not extend this group to the distant helper, dispatcher, all table entries,
or the preceding constant/flag-selected wrappers merely from table membership
or adjacency. The four-function range is the reviewed local run, not a claimed
original file boundary.
