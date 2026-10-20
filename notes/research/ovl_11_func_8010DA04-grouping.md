# ovl_11_func_8010DA04: handler-run table-cluster evidence

## Verified handoff

- Source: `src/overlays/ovl_11/ovl_11_func_8010DA04.c`.
- Supplied verified source/evidence identity:
  `73c35e1cb81e7abc766e04c3ed41e0770142cfda8df015b3a7cc0a269a2d4a56`.
- The handoff reports byte identity and a passed full finalize gate, with
  changes to the source and `include/globals_override.h`. This notes-only
  review does not repeat solver work or modify build inputs.

## New grouping evidence

The verified body ties DA04 to the message-table cluster of the already
listed `D_800BAA84` handler run:

- DA04 indexes the eight signed halfwords at `D_80127CF4` with the object's
  unsigned halfword at +0xAC, selecting a message or its successor for
  `func_8002261C`. `build/ovl_11/asm/data/69960.data.s` places that table at
  0x80127CF4–0x80127D03, immediately before the eight-halfword `D_80127D04`
  table used by `src/overlays/ovl_11/ovl_11_func_8010DDC8.c` with the same
  object index and message-queue callee. This extends the DDC8/DE68 data
  cluster documented in [the DE68 grouping note](ovl_11_func_8010DE68-grouping.md)
  backward by one adjacent table.
- DA04 and DDC8 also share a coherent wrapper: set flag 0x2 of the object word
  at +0x34, map the unsigned `D_8006C838 + 0x5200` value by adding 2 below
  2 or subtracting 2 otherwise into object +0x22, and clear bit 0x400 of
  `D_8006C838 + 0x5234` on a -1 return. The shared global alone would be
  weak evidence; the matching operations reinforce the adjacent table use.
- `configs/splat/ovl_11.yaml` and `build/ovl_11/functions.csv` place DA04,
  DD38, DDC8 and DE68 in the existing gapless run: 0x8010DA04/0x334,
  0x8010DD38/0x90, 0x8010DDC8/0xA0, 0x8010DE68/0x45C. The original DA04
  assembly at
  `build/ovl_11/asm/nonmatchings/ovl_11_func_8010DA04/ovl_11_func_8010DA04.s`
  corroborates the table access, wrapper and call to DE68. That local
  caller/helper relationship is already recorded in the DE68 note, not a
  new discovery here.

## Conclusion and limits

Retain DA04 as a candidate member of the existing handler run at medium-low
confidence. The additional table adjacency and matching wrapper strengthen
that membership, but establish neither an original file boundary nor shared
static linkage. No declaration-order or register-variable evidence was
established; do not merge neighbouring runs on this evidence alone.

Role: object-mode handler selecting messages and updating facing/response flags.
