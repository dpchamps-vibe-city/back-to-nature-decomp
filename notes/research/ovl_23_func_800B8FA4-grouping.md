# ovl_23_func_800B8FA4: complementary record-driver run extension

Documentation-only handoff: the supplied target is byte-exact and passed the
full finalize gate. Supplied source/evidence identity:
`1505a502d815871890316e71e94f18803ab01b974d54d95595ae0d3620d5b17f`
(origin: agent). Supplied changed files: `include/globals_override.h`,
`include/overlays/ovl_23.h`, and
`src/overlays/ovl_23/ovl_23_func_800B8FA4.c`. This pass changes only notes.

## New membership evidence

- **Paired storage and the same comparison:** the target walks the six
  0x50-byte actors beginning at `D_800BF87C+0x24 = D_800BF8A0`. It compares
  their x positions at aggregate +0x38+i*0x50 against the paired 0x44-byte
  records' positions at +0x210+i*0x44, dividing both by 4096. Its horizontal
  distance threshold is the actor halfword at aggregate +0x2E+i*0x50.
  Existing member `800B8C18` uses exactly those two positions, that threshold
  and the same fixed-point conversion while selecting states for the other
  side of the pair (`D_800BFA80`, excluding record 2). This is a concrete
  complementary update protocol, not merely use of the overlay-wide base.
- **Gapless link order:** `800B8C18` occupies 0x38C bytes and ends exactly at
  `800B8FA4`. The target occupies 0x344 bytes, ending at `800B92E8`.
  Consecutive splat offsets 0xD74, 0xDF8 and 0x1184 place the existing
  `800B8B94`/`800B8C18` pair immediately before the target.
- **Matching call order:** original caller `800B87F8` invokes `800B8B94`,
  `800B8C18` and `800B8FA4` consecutively at 0x800B88D8, 0x800B88E0 and
  0x800B88E8. It increments `D_800BF87C+0x14` in the first call's delay
  slot; the target uses that same halfword as its decision cadence counter.
  Thus call order and link order corroborate the paired-storage evidence.

## Evidence paths and limits

- `src/overlays/ovl_23/ovl_23_func_800B8FA4.c`
- `src/overlays/ovl_23/ovl_23_func_800B8C18.c`
- `include/globals_override.h` (`Ovl23ActorDecisionView`,
  `Ovl23DecisionView`, and the aliases of `D_800BF87C`).
- `configs/splat/ovl_23.yaml` (consecutive text entries).
- `build/ovl_23/functions/ovl_23_func_800B8FA4.s` (coordinate accesses at
  0x800B9048/0x800B905C, actor threshold at 0x800B90D8).
- `build/ovl_23/functions/ovl_23_func_800B8C18.s` (matching accesses at
  0x800B8CA0/0x800B8CB4/0x800B8CDC).
- `build/ovl_23/functions/ovl_23_func_800B87F8.s` (counter and call order).
- [Existing complementary-driver evidence](ovl_23_func_800B8C18-grouping.md).

Extend the existing complementary state-driver candidate group with the
actor-side driver, retaining medium confidence. These observations support
suspected same-TU membership, not static linkage or exact original boundaries.
The recovered views and aliases do not establish original declarations; no
register-variable, declaration-order or compiler-flag premise is asserted.
Calls to the movement/vector helpers connect the families but do not justify
merging their other members into this group.
