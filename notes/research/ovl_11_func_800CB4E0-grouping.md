# ovl_11_func_800CB4E0: adjacent global-object state callers

## Verified anchor

Supplied source/evidence identity:
`79401ff28f06cf786add35217cad285a208c3b73e4ef3ca133b10d677909a04a`
(origin: static). The handoff reports byte identity and a passed full finalize
gate for `src/overlays/ovl_11/ovl_11_func_800CB4E0.c`, its sole listed
changed file. This documentation accepts that verification; it does not
modify or re-verify build inputs. The neighbour remains assembly-backed.

## New membership evidence

- **Gapless original link order.** `configs/splat/ovl_11.yaml` places
  800CB4E0 at file offset 0x136C0 and 800CB540 at 0x13720. The original
  target assembly gives size 0x60, so the target ends exactly at 800CB540.
- **Shared object and derived work-area base, witnessed in instructions.**
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800CB4E0/ovl_11_func_800CB4E0.s`
  retains `&D_80071A00` in `$s0` across the dispatcher call, then subtracts
  0x51C8 to address the work area at `D_8006C838`.
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800CB540/ovl_11_func_800CB540.s`
  likewise retains that object in `$s0` and derives `$s3 = $s0 - 0x51C8`
  at 800CB57C. This corroborates the verified source's sub-base expression;
  it is not evidence inferred from reconstructed typedefs or prototypes.
- **Common dispatcher and the same flag word.** At 800CB500 and 800CB5DC,
  both functions pass `&D_80071A00` to `ovl_11_func_800C70CC`, with a
  selected state/animation and final argument -2. Both then update the
  object's flag word: the target reaches it as work-area +0x5234 and the
  neighbour as object +0x6C, the same address 0x80071A6C. The target clears
  bit 0x20000000 and sets bit 0x04000000; the neighbour sets bits 0x8004.
  Thus adjacent routines share a concrete object/work-area cluster and a
  dispatcher-followed-by-flag-update idiom, not just one public call.

## Inference and limits

Record a **low-confidence candidate same-TU pair**, 800CB4E0 / 800CB540.
The premise is that their gapless placement and repeated object/sub-base,
common-dispatcher and flag-update pattern reflect one local source unit.
Neither symbol is proven private, and the sub-base idiom occurs elsewhere;
separate adjacent translation units remain possible. No distinctive
register-variable or declaration-order effect is established.

This does not merge the pair with the distant state-switch dispatcher run
or the broad D_80071A00/D_8006C838 candidate families. The distant caller
`src/overlays/ovl_11/ovl_11_func_800CD08C.c` confirms the target's reset role
but does not establish a common TU. Unused declarations in the verified
source are not call edges or grouping evidence. Neither outer TU boundary
nor membership of other neighbouring functions is established here.
