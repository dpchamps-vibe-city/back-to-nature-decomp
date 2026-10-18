# ovl_11_func_800D2334: extension of the local object-updater run

## Verified anchor

Supplied source/evidence identity:
`32c42f81c34602b5b05a7378b9ef7cc3c87b59c5fd8920a26dd78435c58df816`
(origin: agent). The handoff reports byte identity and a passed full finalize
gate, with `src/overlays/ovl_11/ovl_11_func_800D2334.c` as its changed file.
This notes-only review accepts that verification without modifying build inputs.
The inspected source's SHA-256 is
`644f4eaab0cefabf02ebae00f3af0dbe67a9e662cb42774ed69d08dd711ac7f2`,
also recorded as exact in `build/experimentLedger/ovl_11_func_800D2334.jsonl`.

## New membership evidence

Extend the existing D_80123754 setter/getter and updater-sibling candidate
section with 800D2240 and 800D2334, specifically as an extension of its
800D1EB8 / 800D2160 updater tail.

- **Gapless link order.** `configs/splat/ovl_11.yaml` places 800D2160,
  800D2240 and 800D2334 at offsets 0x1A340, 0x1A420 and 0x1A514.
  `build/ovl_11/functions.csv` records sizes 0xE0, 0xF4 and 0x1C8:
  800D2160 ends at 800D2240, which ends at 800D2334, which ends at
  800D24FC. The latter address is a boundary here, not an asserted member.
- **A common caller corroborates this local updater set.** Original assembly
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800CE834/ovl_11_func_800CE834.s`
  calls 800D2240 at 800CE860, 800D2160 at 800CE868, 800D1EB8 at
  800CE870 and 800D2334 at 800CE878, with no intervening calls. Execution
  order differs from address order; the evidence is a common update sequence
  covering the contiguous four-function run, not identical ordering.
- **Repeated concrete field/gate/reset/timer idiom.** The verified target
  and `src/overlays/ovl_11/ovl_11_func_800D2240.c` both test an active
  initial halfword and word +0x34 bit 0x20000, compare the record's signed
  halfword +0x30 with `D_8009AFF0+0x5476`, and use 80107DD0 / 80107DEC
  on the pair at record +0xA8. Both gate an update on
  `D_8006C838+0xC` bit 0x08000000. These accesses and calls are present in
  their respective original assemblies under
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800D2240/` and
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800D2334/`.
  `src/overlays/ovl_11/ovl_11_func_800D2160.c` carries the same mode
  comparison, global update gate and record+0xA8 reset/timer pair, though
  the placement of the gates differs among these routines.
- **The selected object storage forms a local cluster.** 800D2334 selects
  D_800759E4 and D_80075AD4, bases separated by 0xF0; another 0xF0 reaches
  D_80075BC4, the pool base used by 800D2240's original assembly. Its six
  0xB0 records span 0x420 bytes and end at D_80075FE4, the three-record
  pool used by 800D2160. This corroborates sibling object-family updaters;
  it does not make those engine globals private to the original source file.

## Inference and limits

The contiguous updater code, common caller, storage cluster and repeated
specific field/helper pattern support a **low-confidence candidate same-TU
extension** by 800D2240 / 800D2334. The premise is that these sibling update
routines were kept in one source unit rather than adjacent units using public
engine interfaces. No shared static, declaration-order effect or distinctive
register-variable quirk is established by this match.

800D2334 does not access D_80123754, so this does not independently prove its
membership in that global's setter/getter TU or strengthen the older group's
outer boundaries. The common caller 800CE834, the later caller-sequence member
800D2710, the non-updater successor 800D24FC and the distant per-object
handlers are not added on this evidence. The call to 800D266C alone does not
establish a wider same-TU run through that helper. Reconstructed shared types
are not original-TU evidence.
