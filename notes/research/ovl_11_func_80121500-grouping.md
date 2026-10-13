# ovl_11_func_80121500 — farm-effect pool grouping evidence

The handoff reports byte-exact source and a passed full finalize gate for
`ovl_11_func_80121500`, with source/evidence identity
`a62a862ac073794f618f0b5a50e11695fc8231fbd7337cf82f44a46ee87ee663`.
This note records grouping evidence only; no matching experiments were run.

## New evidence

The initializer is connected to the existing farm-object clear/update run by
an original call on the same pool entry, not merely by its address:

- `build/ovl_11/asm/nonmatchings/ovl_11_func_8012135C/ovl_11_func_8012135C.s`
  loads the `D_8012DB90` base at `0x80121360–0x80121364`, tests the pointer
  at entry `+0x14` at `0x80121380`, and advances entries by `0x18` at
  `0x801213C0`. On finding a free entry, it copies that entry pointer into
  `$a0` at `0x801213A4` and calls `80121500` at `0x801213A8`.
- `src/overlays/ovl_11/ovl_11_func_80121500.c` initializes that record.
  Its original target in
  `build/ovl_11/asm/nonmatchings/ovl_11_func_80121500/ovl_11_func_80121500.s`
  stores the sprite pointer at `+0x14`, an animation selector at `+0`,
  zero tick at `+2`, and four-byte coordinate payloads at `+4/+8/+C`
  (`0x80121748–0x8012175C`).
- `src/overlays/ovl_11/ovl_11_func_801213D8.c` and its original target in
  `build/ovl_11/asm/nonmatchings/ovl_11_func_801213D8/ovl_11_func_801213D8.s`
  consume the matching selector, tick, position and sprite fields. The
  updater reads only the low halfwords of the position slots; this does not
  establish the full original coordinate type. It calls `801214F8` at
  `0x801214DC` when the animation ends.
- `src/overlays/ovl_11/ovl_11_func_801214F8.c` clears the pointer slot
  (`arg0[5]`, byte offset `0x14`).

## Corroborating run and link order

`src/overlays/ovl_11/ovl_11_func_801212C4.c` walks `D_8012DB90` and calls
`801213D8`; `src/overlays/ovl_11/ovl_11_func_80121318.c` walks the same pool
and calls `801214F8`. Both step `0x18`-byte records with a countdown from
`0x18` through zero (25 entries), agreeing with the allocator's `0x19` limit.

`configs/splat/ovl_11.yaml` lists the gapless sequence at file offsets:

| Function | Offset | Length to next boundary |
| --- | --- | --- |
| `801212C4` | `0x694A4` | `0x54` |
| `80121318` | `0x694F8` | `0x44` |
| `8012135C` | `0x6953C` | `0x7C` |
| `801213D8` | `0x695B8` | `0x120` |
| `801214F8` | `0x696D8` | `0x08` |
| `80121500` | `0x696E0` | `0x280` |

The initializer's target ends at `0x80121780` (exclusive), corresponding to
the data boundary at file offset `0x69960`. Thus the shared-record call graph
and link order support extending the existing candidate family through it.

## Limits

This remains a medium-confidence candidate original translation unit, not a
proven boundary. Cross-TU calls can operate on the same record layout, and
neither adjacency nor the following data boundary establishes an original
source-file boundary. No new shared static, GP-relative ownership cluster,
declaration-order effect or neighbour register-variable quirk is established
here. The four-byte by-value coordinate view in the verified C is not used as
a same-TU fingerprint. The two distant item-query callees are not added to
this group merely because the initializer calls them.
