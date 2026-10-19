# ovl_11_func_800C523C: adjacent counter-action siblings

## Verified anchor

The handoff reports byte identity and a passed full finalize gate for
source/evidence identity
`cd22aa9f3a01e4f3a7a87f4e11c611fe2932402e57484627b8e64900491ae90c`.
Its changed-file list is `include/globals_override.h`, `include/sdk_types.h`
and `src/overlays/ovl_11/ovl_11_func_800C523C.c`. This is a notes-only
review, not a new verification run.

## New membership evidence

Earlier reviews left this function outside the
[local dispatch-family candidate](ovl_11_func_800C55FC-grouping.md) and
explicitly did not infer membership from its
[request-consumer predecessor](ovl_11_func_800C5098-grouping.md).
The verified body and original assembly now support a specific sibling tie:

- **Shared counter and dispatch pipeline with immediate successor 800C541C.**
  Both obtain a mode from 800C9D64, divide the object's signed +0x18 tier
  by 20, convert its position through `D_80123234[mode]` outside mode 2,
  and call 800D5960 followed by 800D806C. Both then update the same
  `D_80128C64` halfword by `1 << tier` and pass it with object +0x86 to
  their action variant. In mode 2 both instead require
  `D_80128C54 & 0x8000`, update the same counter, and act on D_80128C4C.
  Their grid/object action pairs are 800D8C6C/800F482C and
  800D88D0/800F474C respectively; both finish through 800CD5BC with
  0x28 or 0x96. Original assembly under
  `build/ovl_11/asm/nonmatchings/<function>/<function>.s` shows the
  counter accesses at 800C5360–800C5384 / 800C53B8–800C53E0 and
  800C5540–800C5564 / 800C5598–800C55C0.
- **Call graph corroborates exact link adjacency.**
  `configs/splat/ovl_11.yaml` places 800C523C and 800C541C at offsets
  0xD41C and 0xD5FC. Each original body is 0x1E0 bytes: the target ends
  exactly at 800C541C, which ends at existing family member 800C55FC.
  In `build/ovl_11/asm/nonmatchings/ovl_11_func_800C580C/ovl_11_func_800C580C.s`,
  adjacent dispatcher cases call 800C541C at 800C65D4 and 800C523C at
  800C65F0 with the same `$s1` object. Each case first calls 800CD0F4
  with `(object, 0)`, then joins the same completion path. This is a
  common local dispatcher over contiguous sibling handlers, not adjacency
  or a shared reconstructed type alone.

The [800D8C6C review](ovl_11_func_800D8C6C-grouping.md) already noted the
parallel grid-action call sites but did not infer caller TU membership.
The shared counter across both modes and corroborating local dispatcher/link
order supply the caller-side membership case here.

## Inference and limits

Add 800C523C to the existing local dispatch-caller TU candidate with
**medium confidence**, alongside immediate successor 800C541C and common
caller 800C580C. The premise is that the contiguous sibling handlers,
shared counter/dispatch pipeline and paired caller cases reflect one original
source unit. Neither a TU boundary nor private linkage is established:
these globals are accessed absolutely. The counter is also reset by
800C401C; that fact alone does not add the resetter to this family.

Do not merge the family with the distant grid-action, object-action or
800CD5BC tier/clamp groups. The existing broad D_80128C4C/D_80128C54
reader membership is unchanged; the new inference concerns the narrower
local dispatch-family candidate.
