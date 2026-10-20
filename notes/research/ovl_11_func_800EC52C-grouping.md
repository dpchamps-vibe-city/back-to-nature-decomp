# ovl_11_func_800EC52C grouping evidence

## Provenance

The supplied handoff reports byte-exact source and a passed full finalize
gate (origin: agent; source/evidence identity
`193fd29f502261c85798df5583fe62cf1cbf8d4ae498fa43f18c4faac0c7025c`).
This is a notes-only review of
`src/overlays/ovl_11/ovl_11_func_800EC52C.c`, not a new matching run.

## Candidate membership

The verified sequencer supports membership in the existing `D_80129560`
accessor family. Its shared-table sequencing pattern and helper cluster
corroborate that membership; its table use also corroborates the previously
unconfirmed adjacency to `800EC490`. These are conditional source-organization
ties, not proof of a common original translation unit.

- The target reads the signed low halfword of `D_80129560[arg0]` as its
  sequencing state and writes the updated state back as a word. It also
  reads and writes the item, score and selected-actor slots supplied through
  its other arguments. Original assembly:
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800EC52C/ovl_11_func_800EC52C.s`.
  The absolute table base, four-byte slot stride and `lh` are visible at
  0x800EC624 / 0x800EC694–0x800EC6A0; the final state `sw` is at 0x800ED460.
- Recorded family member `800EB85C` uses the same signed-low-halfword
  state/word-writeback pattern and the same position/action helpers
  `800E5ADC` and `800E5B84`, plus predicate `800EDF3C`. See
  `src/overlays/ovl_11/ovl_11_func_800EB85C.c` and
  [its grouping evidence](ovl_11_func_800EB85C-grouping.md).
  The target's original calls include 0x800EC7C0, 0x800EC7D4 and
  0x800ECA18 respectively, as well as the accessor-family item helper
  `800E6EE0` at 0x800EC794.
- `configs/splat/ovl_11.yaml` places `800EC490`, the target and `800ED4A8`
  at consecutive offsets 0x34670, 0x3470C and 0x35688. The preceding
  0x9C-byte `800EC490` ends exactly at the target and also writes
  `D_80129560` (`src/overlays/ovl_11/ovl_11_func_800EC490.c`). This extends
  the existing shared-table run through the target, without asserting a
  direct call between those neighbours.

## Limits

The target's 0xF7C-byte extent ends exactly at `800ED4A8`, which it calls at
0x800ECBA8. That call/link adjacency was already recorded under `800ED4A8`;
it is not a newly discovered edge. It remains a low-confidence local same-TU
hint, not grounds to merge the distant `D_80076220` record-init family into
the accessor family.

`D_80129560` is overlay-private, not proven TU-local. The recovered score
tables `D_800B9BD4` and `D_801249B8` supply no demonstrated shared-static tie.
The target's function-local jump-table slice in the splat configuration is
not an original TU boundary witness. No declaration-order, register-variable,
SDK-idiom or new outer-boundary evidence is established.
