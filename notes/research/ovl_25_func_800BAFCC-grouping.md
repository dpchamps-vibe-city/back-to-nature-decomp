# ovl_25_func_800BAFCC: shared sprite-cluster grouping evidence

Verified handoff identity: `1281e220e744ef7ac5a66f1256135ce829b6d3c9f5f44a0e15ddf15612777dc0`.
The supplied handoff reports byte identity and a passed full finalize gate;
this documentation pass does not rerun matching or alter build inputs.

## New association

`ovl_25_func_800BAFCC`, already listed in the display-setup state-handler
run, initializes the same complete sprite-record set whose flag bits
`ovl_25_func_800BA2E8` sets and `ovl_25_func_800BA40C` clears:

| Byte-addressed record set | Traversal shared by all three |
| --- | --- |
| `D_800C024C` | Four records at +0x00/+0x30/+0x60/+0x90 |
| `D_800C024C - 0x3E0` and `- 0x3B0` | Six pairs, stride 0x78 |
| `D_800C013C` and `+ 0x30` | Two pairs, stride 0x78 |
| `D_800C0334` | Six records, stride 0x30 |

Evidence paths:
- `src/overlays/ovl_25/ovl_25_func_800BAFCC.c`: the `func_80015704`
  destinations cover all four sets, using sprite headers in `D_800BE1B4`.
- `include/globals_override.h`: the `Ovl25Sprite800C013C`,
  `Ovl25Sprite800C024C` and `Ovl25Sprite800C0334` views alias the original
  `D_` symbols; they are not separate storage.
- `src/overlays/ovl_25/ovl_25_func_800BA2E8.c` and
  `src/overlays/ovl_25/ovl_25_func_800BA40C.c`: matching flag-set/clear
  traversals; their halfword-pointer offsets convert to the byte sets above.
- `build/ovl_25/functions/ovl_25_func_800BAFCC.s`,
  0x800BB130–0x800BB284: original destination bases and strides corroborate
  the shared cluster independently of the new C aliases.

## Grouping limit

The display-setup call/link adjacency was already recorded and is not new.
The new evidence is its initializer's association with the recorded
`D_800C024C` object-state base run through the complete global cluster, not
merely one shared rendering helper. This supports a candidate same-TU link
if the overlay-local sprite lifecycle was kept in one original source file.
Absolute references and shared storage do not establish static visibility or
TU ownership, and these runs are not directly adjacent. Retain the existing
candidate groups and confidence rather than asserting a merged TU boundary.
