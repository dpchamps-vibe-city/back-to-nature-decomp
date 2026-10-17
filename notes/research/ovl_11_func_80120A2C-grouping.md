# ovl_11_func_80120A2C: state-driver / texture-update grouping

## Verified anchor

The documentation handoff reports byte identity and a passed full finalize gate
for source/evidence identity
`7214089dcd41360b1251fac1cba3d2a4154df5a5806209be627c1d4ad8767344`.
The verified source is `src/overlays/ovl_11/ovl_11_func_80120A2C.c`;
the supplied changed files also include `include/globals_override.h` and
`include/overlays/ovl_11.h`. This is a notes-only review, not a new matching or
verification run.

## New grouping evidence

The verified state machine calls `ovl_11_func_80121134` during state 1.
The original assembly corroborates that edge at **0x80120ADC** in
`build/ovl_11/asm/nonmatchings/ovl_11_func_80120A2C/ovl_11_func_80120A2C.s`.
This ties the map-camera driver to the existing local texture-update subgroup,
not merely to another reader of a broadly shared engine buffer.

Call order agrees with an unbroken local text run. `configs/splat/ovl_11.yaml`
lists these consecutive offsets and extents:

| Function | Overlay file offset | Size |
| --- | --- | --- |
| 80120A2C | 0x68C0C | 0x298 |
| 80120CC4 | 0x68EA4 | 0x26C |
| 80120F30 | 0x69110 | 0x204 |
| 80121134 | 0x69314 | 0xD0 |

Each ends at the next function's start. The target is therefore the gapless
predecessor of the subgroup's first helper, while directly calling its driver.
The driver's original assembly,
`build/ovl_11/asm/nonmatchings/ovl_11_func_80121134/ovl_11_func_80121134.s`,
then calls 80120CC4 at 0x8012119C and 80120F30 at 0x801211B4 using the shared
`D_8012DB38` UV/CLUT buffer. The core subgroup's additional data-cluster
evidence is recorded in [the 80120F30 note](ovl_11_func_80120F30-grouping.md).

## Existing evidence and limits

The ledger already places 80120A2C with the link-contiguous `D_8012DB58`
state trio, 801209A8 / 801209D4 / 80120A2C. The verified source clarifies its
role as a map-camera input/exit state machine: it advances the state halfword
and updates coordinates at +0xC/+0x14. Those accesses do not by themselves
provide new membership evidence; the shared state and adjacency were already
recorded. The new evidence is the direct call into the adjacent texture-update
subgroup.

A **low-confidence extension** of the medium-confidence texture-update core
by `ovl_11_func_80120A2C` is supported, conditional on this local driver/helper
chain reflecting one original source unit rather than adjacent linked units.
No static ownership, original declaration order, or TU boundary is established.
The existing low-confidence state-trio hypothesis remains separate; it is not
automatically promoted by the new edge. Calls to address-separated helpers
800BDA20 / 800DB7F0 / 800D78D0, and accesses to the common `D_8007AFF0` work
area, do not establish membership with those distant functions.
