# ovl_11_func_8010FBC4: candidate predecessor to the far-state/reset band

Documentation-only review of the verified source. The supplied handoff reports
byte-exact matching and a passing full finalize gate, with source/evidence
identity `372b13cda671728e2f05275b2cc721625ced937f3f9188100659af2c30ec93bd`.
Its changed-file list is `configs/splat/ovl_11.yaml` and
`src/overlays/ovl_11/ovl_11_func_8010FBC4.c`. Matching and build checks were
not repeated; this review changes only notes.

## New grouping evidence

The existing band already lists `8010FEF8` as a low-confidence predecessor to
its reset-helper core. `8010FBC4` is its immediate gapless predecessor:
original assembly gives size 0x334, and `0x8010FBC4 + 0x334 = 0x8010FEF8`.
The consecutive code offsets 0x57DA4 and 0x580D8 in the splat configuration
record the same order.

The call graph corroborates this sibling adjacency. In the original
`ovl_11_func_8010EEE4` assembly, alternative branches call `8010FBC4` at
0x8010F0F0 and `8010FEF8` at 0x8010F104 with the same object in `$s0`;
further calls occur at 0x8010F150 and 0x8010F204 respectively. Thus these
adjacent drivers are alternatives within one enclosing object-mode handler,
not merely unrelated users of an engine utility.

The verified bodies also share a local state-machine/helper fingerprint:
s16@+0x28 selects the state, s16@+0xBA holds the record selection returned by
`80110944`, and `80110544(..., 0xE6)` gates the initial movement. Both use
`80110520` with object +0xB6, obtain a return position through
`80110188(object, 2, ..., object->field_30)`, and forward it to `800D05D0`.
Their states 5 and 6 have the same completion sequence:
`80110544(object, 0x258)` advances to 6, then `80110118(object)` resets the
state to 0. This links the new driver to both its adjacent sibling and the
existing band's helper core.

## Evidence paths

- `src/overlays/ovl_11/ovl_11_func_8010FBC4.c` (verified mode-6 driver)
- `src/overlays/ovl_11/ovl_11_func_8010FEF8.c` (adjacent mode-8 sibling)
- `build/ovl_11/asm/nonmatchings/ovl_11_func_8010FBC4/ovl_11_func_8010FBC4.s`
  (0x334 size; completion calls at 0x8010FEB0 and 0x8010FEC8; final
  instruction at 0x8010FEF4)
- `build/ovl_11/asm/nonmatchings/ovl_11_func_8010EEE4/ovl_11_func_8010EEE4.s`
  (shared caller and alternative branches cited above)
- `configs/splat/ovl_11.yaml` (consecutive code offsets 0x57DA4 / 0x580D8)
- [Existing predecessor evidence](ovl_11_func_8010FEF8-grouping.md)

## Limits

Assuming the sibling call/link organization reflects source organization,
`8010FBC4` is a low-confidence predecessor extension of the existing band.
There is no direct call between the two drivers, and adjacency can cross a TU
boundary. No shared static, TU-owned global, declaration-order effect or
register-variable quirk was established. `D_800749F4`, `D_8006C838`, the shared
object views and the newly associated switch rodata do not independently prove
same-TU membership. The core band's confidence is unchanged; this evidence
does not merge the enclosing `8010EEE4` handler or distant animal/vector
helpers into it.
