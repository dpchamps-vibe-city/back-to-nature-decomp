# ovl_11_func_800CF428: item-spawn cluster extension

## Scope and identity

Documentation-only review of the byte-exact, finalize-gated source
`src/overlays/ovl_11/ovl_11_func_800CF428.c`. Supplied source/evidence
identity: `d8df6251390ff8fa6d430f9d4c80fec2e0d8e41477810ffc5a7a3b6b6cc70aed`.
The supplied changed-file list also contains `include/globals_override.h`
and `include/overlays/ovl_11.h`. No matching experiments or build-input
edits were performed in this review.

## New grouping evidence

The verified coordinator supplies a **medium-confidence candidate same-TU
link** between the existing D_80123758 lookup/walker pair and the adjacent
800CF748–800CFA48 spawn run:

- Call graph and link order agree: 800CF428 begins by calling 800CF748 and
  800CF848, in that order. Its gapless successor is the first helper:
  `0x800CF428 + 0x320 == 0x800CF748`. The splat offsets are 0x17608,
  0x17928 and 0x17A28. The existing run continues without gaps through
  800CF848, 800CF95C and 800CFA48.
- Shared spawn sequence: 800CF428, 800CF748 and 800CF848 call
  `ovl_11_func_800CE744(id, -1)`, copy three coordinate words into the
  returned object's +0x38/+0x3C/+0x40 fields, and choose +0x22 using
  `func_80012A34(2)`. CF428 and CF748 use 0/3; CF848 uses 0/2. CF428
  and CF748 also use the identical `D_8007AFF0 + 0x25476` s16 area view;
  CF848 reaches that cell through `D_8009AFF0 + 0x5476`. CF95C shares
  the spawn/coordinate idiom and the 0/3 choice on its random branch.
- Specific state producer/consumer tie to the run's tail: 800CFA48 sets
  bit 0x800000 of `D_8006C838 + 0x44F8`; 800CF428 tests that same word
  with `(flags & 0x1800000) == 0x800000` before spawning item 0x15B.
  Both use the +0xE4C8 zero gate, and their conditional paths also read
  +0x44BA and call `func_80012A34(10)`. CFA48 is a flag setter, not
  another coordinate-copying spawn leaf.
- Supporting data-band continuity: CF428's 17 records at D_80123758,
  stride 0x18, reach the next-stride address D_801238F0. Its three
  coordinate records there, stride 0x10, reach D_80123920, the eight-word
  block consumed by CF748; that block ends at D_80123940, CF848's table.
  These extents follow the access patterns, not recovered original object
  declarations, so this is supporting evidence rather than proof of
  allocation boundaries or data ownership.

The CF258/CF428 shared-table tie was already recorded: CF258 scans the
same 17 records and uses the same s16-narrowed 800D2E20 result against
record mask +0x4. CF428 additionally copies coordinates +0x8/+0xC/+0x10.
That layout detail alone is not the new same-file evidence; the new link
is the coordinator's adjacent callees, shared spawn sequence and specific
flag dependency.

## Evidence paths and limits

- `src/overlays/ovl_11/ovl_11_func_800CF258.c`
- `src/overlays/ovl_11/ovl_11_func_800CF428.c`
- `src/overlays/ovl_11/ovl_11_func_800CF748.c`
- `src/overlays/ovl_11/ovl_11_func_800CF848.c`
- `src/overlays/ovl_11/ovl_11_func_800CF95C.c`
- `src/overlays/ovl_11/ovl_11_func_800CFA48.c`
- `include/globals_override.h` (D_80123758, D_801238F0 and D_80128D66)
- `configs/splat/ovl_11.yaml` (0x17438 and 0x17608–0x17CB0)

This extends existing candidate membership, not a proven original file
boundary. Extern tables and the widely shared main-RAM buffer do not
establish ownership. The functions between CF258 and CF428 are not added
merely because they lie between the lookup and coordinator; neither are
other distant state-buffer or object-creation consumers.
