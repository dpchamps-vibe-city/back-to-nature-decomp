# ovl_11_func_800C93A4: adjacent state-initializer/caller grouping

## Verified anchor

Supplied source/evidence identity:
`dd60950a8dfa7ee3f7b20fd3a6d8ae878dc39e2e2b3d1551c476820fccc6c134`
(origin: agent). The handoff reports byte identity and a passed full finalize
gate for `src/overlays/ovl_11/ovl_11_func_800C93A4.c`, its sole listed changed
file. This notes-only review accepts that verification without modifying or
re-verifying build inputs.

## New membership evidence

- **The caller immediately follows its helper.**
  `configs/splat/ovl_11.yaml` places 800C93A4 and 800C9424 at offsets
  0x11584 and 0x11604. In
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800C93A4/ovl_11_func_800C93A4.s`,
  the target's original size is 0x80, ending exactly at 800C9424. In
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800C9424/ovl_11_func_800C9424.s`,
  the call at 800C94E8 passes the caller's saved object pointer in s0 to
  800C93A4. That pointer is D_80071A00; the call is selected by bit 0x80
  of its halfword at +0x36, which the caller clears after the helper returns.
  Thus link adjacency agrees with a local dispatch/helper relationship.
- **The same caller connects the pair to existing state-switch members.**
  The caller passes that same object to existing member 800C8D9C at
  800C95DC and 800C963C. It also calls existing member 800C70CC at
  800C94B8, 800C96A4 and 800C96D0. The target itself calls 800C70CC at
  800C93C8. This is a shared local caller/object cluster, not just another
  reconstructed use of the Ovl11C7010Object typedef.
- **Original object-update idioms corroborate the connection.**
  The verified target sets flags at +0x6C, selects state 0x3B through
  800C70CC, invokes func_80015840 on object +0x260, and copies the low
  byte of u16@+0x38 to +0x265. The original target instructions store the
  halfword at 800C93E8 and reload its low byte at 800C93EC. Existing
  members `src/overlays/ovl_11/ovl_11_func_800C7010.c` and
  `src/overlays/ovl_11/ovl_11_func_800C70CC.c` use the same +0x260 sprite
  and +0x38-to-+0x265 byte-copy idiom. These shared field/callee patterns
  support the caller/adjacency evidence; they are not a newly established
  register-allocation or declaration-order fingerprint.

## Roles and limits

800C93A4 initializes state 0x3B: it sets flags 0x1800, requests that state,
writes -0x2286 and -0x8C to words at +0x100 and +0x114, selects sprite
animation 4, stores direction 2 at +0x38 and its low byte at +0x265, sets
flag 0x40000000 and calls func_8001FABC(6). 800C9424 dispatches pending
flags and input for D_80071A00, including this initializer.

Extend the existing **medium-confidence candidate** 0x38/0x3C state-switch
family with this adjacent helper/caller pair. The conditional premise is
that the gapless helper/caller arrangement, common object and shared local
dispatch idioms reflect one original translation unit. Neither original
static linkage nor a TU boundary is established. The helper/caller pair is
more directly supported than the broader family's single-TU interpretation;
reconstructed typedef reuse and calls to widely used 800C70CC alone would
not establish membership.

This does not assign every intervening function to the family, add the
preceding 800C907C merely because it is adjacent, or merge the units of
other callees reached by 800C9424.
