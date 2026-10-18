# ovl_11_func_800CB850: adjacent item-action gate and cell probe

## Verified anchor

Supplied source/evidence identity:
`81dc4a3a5521df0e06ea6bfb6b5c1b9289dc65b328677912d83cd1e716f68081`
(origin: agent). The handoff reports byte identity and a passed full finalize
gate, listing `include/globals_override.h` and
`src/overlays/ovl_11/ovl_11_func_800CB850.c` as changed files. This notes-only
review accepts that verification without modifying or rebuilding its inputs.

## New membership evidence

- **Call graph agrees with immediate, gapless link adjacency.**
  `configs/splat/ovl_11.yaml` places 800CB730 at offset 0x13910 and
  800CB850 at 0x13A30. The original listing
  `build/ovl_11/functions/ovl_11_func_800CB730.s` gives the probe size as
  0x120: 0x800CB730 + 0x120 == 0x800CB850. The target's original assembly,
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800CB850/ovl_11_func_800CB850.s`,
  calls that immediate predecessor at 0x800CB868, passing the saved
  `D_80071A00` object pointer.
- **The probe's result directly drives the adjacent caller's action gate.**
  `src/overlays/ovl_11/ovl_11_func_800CB730.c` computes an object-relative
  position, probes the mode-selected cell table, stores the selected cell
  pointer in `D_80128C60`, and returns 0/2/3. The verified target retains
  this result and combines it with item flags and `D_80128C54` to select or
  reject follow-up states through 800C70CC. This is a local helper/caller
  relationship, not adjacency alone.

## Membership and limits

Record 800CB730 and 800CB850 as a **low-confidence candidate same-TU pair**
within the existing state-pair section, where 800CB850 was already listed
as a reader. The new evidence is the adjacent probe/action-gate relationship,
not another observation of the previously recorded state globals.

The conditional premise is that this helper-before-caller link arrangement
reflects source-file locality rather than an original TU boundary. No static
linkage, GP-relative ownership, declaration-order effect or shared
register-variable quirk is established. The pair does not prove that the
whole state-pair band is one TU, nor merge it with the distant pointer-grid
data family that also lists 800CB730. The target's calls to 800C70CC and
800D736C and its far-work-area access do not independently establish TU
membership for those callees or other work-area consumers.
