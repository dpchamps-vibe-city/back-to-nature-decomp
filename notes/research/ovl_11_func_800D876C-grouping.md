# ovl_11_func_800D876C: grid-entry action grouping

## Verified anchor

The handoff reports byte identity and a passed full finalize gate for
source/evidence identity
`c8188f3ef70f929fab4290dd85bf915bb141b5db15a5393edc6f9e296180791b`.
The supplied changed-file list contains only
`src/overlays/ovl_11/ovl_11_func_800D876C.c`. This review changes notes only;
it does not rerun matching or alter build inputs.

## New membership evidence

- **Related operations in the existing entry cluster.** The verified source
  obtains its record through `ovl_11_func_800DAF60(arg0, arg1, arg2)`.
  `src/overlays/ovl_11/ovl_11_func_800DAF60.c` selects the 25×45
  `D_80071DFC` or 7×7 `D_80074124` grid. Like nearby cluster member
  `src/overlays/ovl_11/ovl_11_func_800D8428.c`, the target tests the entry
  id through `800D5868`, conditionally uses byte +4 as the item-state
  argument to a two-argument predicate (`800D5D38` here, `800D5F44` there),
  and uses `800D812C` followed by `800CF95C` for a cell effect, clearing
  the aggregate's second word first. Its non-0x36 reset copies halfword
  +2 to +0 and clears bytes +4/+5 and halfword +6, also present in
  `src/overlays/ovl_11/ovl_11_func_800D85F8.c`.
- **Link order corroborates those relationships.**
  `configs/splat/ovl_11.yaml` places 800D8428, 800D85F8, 800D876C and
  800D88D0 at offsets 0x20608, 0x207D8, 0x2094C and 0x20AB0.
  The original assembly files under
  `build/ovl_11/asm/nonmatchings/<function>/<function>.s` show 800D8428
  ending at 800D85F8, 800D85F8 ending at 800D876C, and the target's
  0x164-byte body ending at 800D88D0. Thus the target immediately follows
  an existing member in the already-related local run, rather than being
  associated solely through a distant lookup helper. Its original calls
  to `800DAF60`, `800D5868`, `800D5D38`, `800D812C` and `800CF95C` are
  at 800D87AC, 800D87C4, 800D87E0, 800D8880 and 800D88A0.
- **A shared caller treats the nearby operations as alternatives.**
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800C5098/ovl_11_func_800C5098.s`
  selects 800D8428, 800D8320 or 800D876C for mode 0, 1 or 2, respectively
  (calls at 800C5190, 800C51C0 and 800C51DC). Each receives the same
  grid selector and computed cell coordinates as its first three
  arguments. This call-graph relationship reinforces the local link-order
  and entry-operation evidence; it does not establish the caller's TU.

## Inference and limits

Add 800D876C to the existing Ovl11D124Entry table cluster as a
**low-confidence same-TU candidate**, without merging or upgrading clusters.
The conditional premise is that the neighbouring, caller-selected operations
with the shared lookup/state/effect pipeline reflect a local source unit.
The tables and helpers are not proven TU-private, and a TU boundary could
still occur within the gapless run. Reconstructed type names alone are not
membership evidence; no shared static, declaration-order effect or register
variable is established. The target only reaches the callback-table cluster
through a callee, so no direct `D_80123DFC` membership is added.
