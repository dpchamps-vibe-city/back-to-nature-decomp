# ovl_11_func_800D93C8 — bounds / cell-sprite dispatch run

Verified handoff source/evidence identity:
`8ca0227bf71b5a4d71348f5f03d1fa20fac755f8995aae0095b5c6833de6975c`.
Source SHA-256: `054445bdbd92361dd29bf41fb911d9c647f25d9169067311548c730e72016832`.
Byte identity and the full finalize gate were supplied as passed; this is
notes-only documentation, not a new verification run.

## New membership evidence

The verified `src/overlays/ovl_11/ovl_11_func_800D93C8.c` selects
800D95EC, 800D981C, 800D9B14 or 800D9E10 for modes 0, 1, 2 or the fallback,
respectively, through `D_80128DA8`. It invokes that handler with a cell's
kind/value and three output pointers, then consumes the outputs as sprite,
texture and vertical offset. Those four handlers are its consecutive link
successors, extending the previously recorded 800D92FC/800D93C8 pair.

`configs/splat/ovl_11.yaml` (offsets 0x214DC–0x21FF0) and the size headers
in `build/ovl_11/functions/ovl_11_func_<address>.s` corroborate the gapless run:

| Function address | Size | End / next function |
| --- | --- | --- |
| 0x800D92FC | 0xCC | 0x800D93C8 |
| 0x800D93C8 | 0x224 | 0x800D95EC |
| 0x800D95EC | 0x230 | 0x800D981C |
| 0x800D981C | 0x2F8 | 0x800D9B14 |
| 0x800D9B14 | 0x2FC | 0x800D9E10 |
| 0x800D9E10 | 0xEC | 0x800D9EFC |

`build/ovl_11/functions/ovl_11_func_800D93C8.s` shows the existing bounds
helper call at 0x800D940C, handler-address selection at 0x800D9420–0x800D9464,
the dispatch-pointer store at 0x800D9468, and its load/`jalr` at
0x800D9520/0x800D9528. The four handler assembly files independently show
three word-sized output slots initialized on entry, agreeing with the
caller's output ABI. These are dispatch call edges, not direct `jal` edges.

## Existing connection and limits

`src/overlays/ovl_11/ovl_11_func_800D92FC.c` reads the two s16 fields at
`D_8007AFF0+0x253B4/+0x253B8` and computes the clamped rectangle traversed
by 800D93C8. `ovl_11_func_800DB978.c` zeroes those same fields along with
four other halfwords at +0x253AC..+0x253B8. This connection to the distant
halfword-state run was already recorded; it is not new membership evidence.

The agreeing local dispatch graph, output ABI and uninterrupted link order
support extending the suspected TU run through 800D9E10, at low confidence.
They do not prove a TU boundary or ownership of `D_80128DA8`; an absolute
address is not evidence of a shared static. The final end address in the
table does not establish membership for 800D9EFC. The distant 800DB674–
800DB978 family remains a separate group, and the widely shared D_8006C838
buffer alone is not a reason to merge its consumers.
