# ovl_11_func_800C6A24: adjacent timer/state helper and caller

## Verified anchor

Supplied source/evidence identity:
`c88fb24342e103d29c658a54e2edd8a5a78f17a33f8bf9de6bfb70f458c705cf`
(origin: agent). The handoff reports byte identity and a passed full finalize
gate; its changed-file list is `include/globals_override.h` and
`src/overlays/ovl_11/ovl_11_func_800C6A24.c`. This notes-only review accepts
that verification without modifying or re-verifying build inputs.

## New membership evidence

- **Call graph agrees with gapless helper/caller link order.**
  `configs/splat/ovl_11.yaml` places 800C6A24, 800C6AFC and the existing
  state-switch member 800C6E0C at offsets 0xEC04, 0xECDC and 0xEFEC.
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800C6A24/ovl_11_func_800C6A24.s`
  gives the verified helper size 0xD8, ending exactly at 800C6AFC. In
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800C6AFC/ovl_11_func_800C6AFC.s`,
  800C6DBC calls 800C6A24 with the caller's saved object pointer. The caller
  has size 0x310 and ends exactly at 800C6E0C. Thus the pair directly
  precedes the already-recorded helper/dispatcher run.
- **The caller and helper share a specific timer/sprite idiom.**
  The caller's 800C6D0C–800C6D78 path compares the signed halfwords at
  object +0x18 and +0x86 (the latter scaled by 20), updates +0x18, then
  divides its signed low halfword by 20 and stores the quotient to byte
  +0x265. The verified helper reads that same signed +0x18 timer, divides
  it by 20, uses the quotient in its sound choice and +0x84/+0x86 animation
  calculation, then writes quotient + 1 to +0x265. This complementary use of
  the same fields supports a local helper/caller relationship beyond
  reconstructed typedef reuse.
- **Both connect to the existing state-switch family.**
  The helper calls 800C70CC at 800C6AD8 with state `u16@+0x3C + 1` and an
  animation incorporating `u16@+0x38 * 5`. Its adjacent caller invokes
  existing members 800C70CC at 800C6BB8/800C6DA8 and 800C7270 at
  800C6C38/800C6CDC, using the same object, +0x38/+0x3C state fields,
  +0x6C flags and +0x260 sprite. The caller's 800C6BC0–800C6BE8 path also
  repeats the family's animation-call and +0x38-low-byte-to-+0x265 idiom.

## Roles and limits

800C6A24 advances the object's state, selects timer-dependent sound and
animation, saves the sound result in D_80128C70 and updates the sprite byte.
800C6AFC dispatches input/state-dependent actions, including timer updates
and the call to this immediately preceding helper.

Extend the existing **medium-confidence candidate** 0x38/0x3C state-switch
family with this pair. The conditional premise is that the gapless local
helper/caller arrangement and complementary timer/sprite operations reflect
one original translation unit. The local pair is better supported than the
broader family's single-TU interpretation; neither original static linkage
nor a TU boundary is established. Calls to widely used 800C70CC, or use of
the reconstructed Ovl11C7010Object type, would not suffice alone.

The new D_80128C70 halfword declaration confirms a sound-result store, not
data ownership. Other absolute-addressed users include 800C401C, 800C580C
and 800C9424; it does not merge those functions into one TU. No new
register-variable or declaration-order fingerprint is established, and
800C69B8 is not added merely because it precedes the verified helper.
