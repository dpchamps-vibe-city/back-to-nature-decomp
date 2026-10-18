# ovl_11_func_800D3250 grouping evidence

Verified source/evidence identity supplied by the handoff:
`064eb8aa2fae7d626701cebc49874c4a0e804be778acf1f13d0189ab399f3ca6`.
The handoff reports byte identity and a passing full finalize gate; this
notes-only review does not repeat that verification.

## Candidate adjacent caller pair

The verified body in
`src/overlays/ovl_11/ovl_11_func_800D3250.c` supports a low-confidence
same-TU candidate pair with its immediate predecessor, `800D3230`:

- **Shared selector:** `src/overlays/ovl_11/ovl_11_func_800D3230.c` forwards
  a u16 mode to `800D3D60` and returns the selected index. The newly verified
  `800D3250` consumes that same selection, trying mode 0 and then mode 1 on
  failure before updating a halfword in the selected record. Original calls
  are at 0x800D3238, 0x800D32B8 and 0x800D32D0, respectively, in
  `build/ovl_11/functions/ovl_11_func_800D3230.s` and
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800D3250/ovl_11_func_800D3250.s`.
- **Matching record families:** the original selector assembly in
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800D3D60/ovl_11_func_800D3D60.s`
  selects from `D_800749F4` at stride 0xB8 for mode 0, or from
  `D_800749F4 - 0x708` (`D_800742EC`) at stride 0xB4 otherwise.
  `800D3250` uses the corresponding +0x16 halfwords, through `D_80074A0A`
  and `D_8006C838 + 0x7ACA`, with those same strides. Thus the common
  callee is a selector for the records being modified, not just a shared
  general-purpose engine service.
- **Gapless original link order:** `800D3230` occupies 0x20 bytes and ends
  at `800D3250`. `configs/splat/ovl_11.yaml` independently records consecutive
  offsets 0x1B410 and 0x1B430; the target occupies 0x140 bytes through
  0x800D3390. Only the first adjacency has the shared-selector corroboration
  used here.

## Limits

This is candidate membership, not a recovered translation-unit boundary.
The shared specialized selector plus gapless caller adjacency is the evidence;
there is no demonstrated shared static, declaration-order effect or unusual
register-variable convention. The main-RAM record arrays and `D_8006C838`
are not private overlay ownership evidence. The selector has other callers,
and its own address is not adjacent to this pair, so neither it nor those
callers are added as same-TU members. The following `800D3390` is not added
on adjacency alone.
