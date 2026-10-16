# ovl_27_func_800B944C: backdrop/effect-family grouping evidence

## Verified handoff

The supplied handoff reports byte-exact source and a passed full finalize gate.
Source/evidence identity: `38e2b0a9cb806fc78fbbfb405620f0173c4969201daa87e23fb27b9093b45668`
(origin: agent). Its changed files are
`include/overlays/ovl_27.h` and
`src/overlays/ovl_27/ovl_27_func_800B944C.c`.
This documentation pass does not rerun or modify the solver result.

## New membership evidence

The recovered body supports a probable same-TU extension of the existing
ovl_27 parallel slot-array effect family, not a seventh slot-array sibling:

- It draws from `D_800C4BD0`, the eighth buffer initialized by the family's
  already-recorded resource loader `ovl_27_func_800BA230`. That loader
  initializes the six slot buffers `D_800C4A80` through `D_800C4B70` and the
  backdrop buffer together from `D_800C4C00`.
- The original function occupies 0x540 bytes, ending exactly at the first
  slot renderer, `ovl_27_func_800B998C`. All three callers
  (`800B9224`, `800B928C`, `800B92E4`) call `800B944C` immediately before
  `800B998C`, followed by the remaining slot renderers. Thus call order
  corroborates link adjacency; adjacency alone is not the premise.
- Its immediate link predecessor `800B9400` uses the same
  `func_80015EE8`/`D_800C4BD0` drawing idiom. The new body is a renderer,
  not the call hub the older ledger description called it.

Role: animated tiled backdrop. The body draws two nine-tile layers with
sine/cosine offsets, advances the `D_800C4A4C`/`D_800C4A4E` phase halfwords
modulo 960 (the first gated by `D_8006C840 & 1`), and draws a final sprite.

Confidence remains medium: the shared buffer cluster and corroborated
call/link order suggest a common original translation unit, but neither
establishes internal linkage or its exact boundaries. This does not merge the
separate state-handler and effect-family ledger groups or enlarge the
six-slot template run.

## Evidence paths

- `src/overlays/ovl_27/ovl_27_func_800B944C.c` — verified renderer body.
- `src/overlays/ovl_27/ovl_27_func_800BA230.c` — shared buffer initializer.
- `src/overlays/ovl_27/ovl_27_func_800B9224.c`,
  `src/overlays/ovl_27/ovl_27_func_800B928C.c`, and
  `src/overlays/ovl_27/ovl_27_func_800B92E4.c` — common ordered callers.
- `src/overlays/ovl_27/ovl_27_func_800B9400.c` — predecessor's shared draw idiom.
- `build/ovl_27/functions/ovl_27_func_800B944C.s` — original address and
  0x540-byte extent; `800B9400`/`800B998C` are its link neighbors.
