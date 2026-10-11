# ovl_11_func_800E5078: candidate TU tail extension

The verified function supports extending the existing 0x800E48CC–0x800E5078
pointer-getter candidate group through 0x800E516C (exclusive), retaining medium
confidence.

Evidence:
- `src/overlays/ovl_11/ovl_11_func_800E5078.c` copies 37 0x30-byte sprite
  records from `D_800762D0 - 0x30 + i * 0x1D4` to
  `D_800762D0 + i * 0x1D4`, optionally reinitialising each destination through
  `ovl_11_func_800C28CC` when the current object's signed halfword at +2 is
  0x82.
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800E4DAC/ovl_11_func_800E4DAC.s`
  at 0x800E4E78–0x800E4EA8 selects the same `D_800762D0 + index * 0x1D4`
  sprite destinations and initialises them through `func_80015704`. Its
  0x800E4F38–0x800E4F4C sequence also reads the current-object pointer at
  `D_8006C838 + 0x30` and tests the signed halfword at +2.
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800E5078/ovl_11_func_800E5078.s`
  confirms the shared storage and current-object access. The predecessor's
  0x2CC bytes end exactly at 0x800E5078; this function's 0xF4 bytes end at
  0x800E516C. The shared sprite-record operation therefore corroborates
  adjacency rather than relying on adjacency alone.

The existing ledger already ties 800E4DAC to 800E4D08 through D_801291C0;
this adds a specific shared-data tie between that tail and 800E5078. Neither
function defines a shared static, so the extension remains a candidate TU
membership, not a proven boundary. The distant 800C28CC call and the widespread
D_8006C838 base do not independently establish same-TU membership, nor does
this establish membership for the successor 800E516C.

Verified source/evidence identity supplied by the finalization handoff:
`304d759e78c9feebcb16b8976422fbfbbe8801436c76ab946a9e18993fa609e7`.
This is documentation of the supplied byte-exact result, not a new solver or
verification pass.
