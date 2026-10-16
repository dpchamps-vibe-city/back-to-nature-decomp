# ovl_23_func_800B8C18: complementary record-driver grouping evidence

Documentation-only handoff: the target is byte-exact and passed the full
finalize gate. Supplied source/evidence identity:
`e5da4be26ad6712bb69c267d8cb3f9e7a9bd93dffaa1025a10c76be2628fc7a2`
(origin: agent). The supplied changed-file list is
`include/globals_override.h`, `include/overlays/ovl_23.h`, and
`src/overlays/ovl_23/ovl_23_func_800B8C18.c`. This pass changes only notes.

## New membership evidence

- **Complementary access to the same record array:** the verified target
  walks six 0x44-byte records at `D_800BFA80`, excluding index 2 in both
  passes. `D_800BFA80 + 2 * 0x44 = D_800BFB08`, the exact record handled by
  the existing candidate member `ovl_23_func_800B8B94`. That neighbour
  selects state from display-object bits; the target selects state from
  timers, position differences and the `800BA494`/`800BA3B4` threshold
  helpers. These are complementary drivers of one array, not merely two
  users of a generic record type.
- **Shared update path:** both drivers pass their records to
  `ovl_23_func_800BB0D8` and `ovl_23_func_800BA610`. The target additionally
  dispatches state 3 to `ovl_23_func_800B9A00`. This extends the earlier
  global/call fingerprint for `800B8B94` into the documented
  `D_800BF87C` record cluster: `D_800BFA80 = D_800BF87C + 0x204`, and the
  target uses both addresses. The named `Ovl23_D_800BF87C_draw` view is an
  alias of `D_800BF87C`, not a second object.
- **Call order and gapless link order agree:** `800B8B94` occupies 0x84
  bytes and ends exactly at `800B8C18`. The splat entries are consecutive
  at file offsets 0xD74 and 0xDF8. The original caller
  `ovl_23_func_800B87F8` calls them consecutively at 0x800B88D8 and
  0x800B88E0, in that same order. This corroborates the shared-array and
  complementary-index evidence.

The previous ledger entry's per-function detail is retained here: `800B8B94`
reads the display-object word at `D_8005E3A8 + 4`, maps bits 0x20, 0x8000
and 0x2000 to mode/phase, and uses `D_800BFB0E` as its fallback phase.
`D_800BFB0E = D_800BFB08 + 6`, the record's own `unk6`; the selected record
then goes through the setter and movement updater named above.

## Evidence paths

- `src/overlays/ovl_23/ovl_23_func_800B8C18.c` (both index-2 exclusions,
  record stride, aggregate accesses and dispatch calls).
- `src/overlays/ovl_23/ovl_23_func_800B8B94.c` (concrete record instance,
  display-bit selection and the shared callees).
- `include/globals_override.h` (`Ovl23DecisionRecord`, `D_800BFA80`, and
  the `Ovl23_D_800BF87C_draw` alias).
- `configs/splat/ovl_23.yaml` (consecutive entries at 0xD74 and 0xDF8).
- `build/ovl_23/asm/nonmatchings/ovl_23_func_800B8B94/ovl_23_func_800B8B94.s`
  (original function size 0x84).
- `build/ovl_23/asm/nonmatchings/ovl_23_func_800B87F8/ovl_23_func_800B87F8.s`
  (consecutive calls at 0x800B88D8 and 0x800B88E0).

## Inference and limits

Extend the existing `800B8B94` candidate group with `800B8C18`, retaining
medium confidence. The shared array, complementary index ownership, shared
callees and corroborating call/link order support suspected same-TU
membership. They do not establish original static linkage or exact TU
boundaries. The recovered struct views and alias spelling are not themselves
evidence of original C declarations; the inference rests on the verified
addresses and operations, with no compiler-flag or register-allocation premise.
