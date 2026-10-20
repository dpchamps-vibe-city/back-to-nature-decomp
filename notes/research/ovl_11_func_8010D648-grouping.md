# ovl_11_func_8010D648: candidate caller/helper grouping

## Verified handoff

- Source: `src/overlays/ovl_11/ovl_11_func_8010D648.c`.
- Supplied verified source/evidence identity:
  `dff79a390bd54b24c6d1eadf867499dcd89f648784dfdb02f2b7b5c0307a470f`.
- The handoff reports byte identity and a passed full finalize gate. This
  documentation-only review accepts that result without rebuilding or changing
  build inputs.

## Grouping evidence

The existing object-mode dispatch section mentions `ovl_11_func_8010D314`
only as the endpoint of its gapless link chain. Its relationship to the
verified helper supports a tentative caller/helper pair at that endpoint:

- `configs/splat/ovl_11.yaml` places D314, D648 and D748 at file offsets
  0x554F4, 0x55828 and 0x55928. The target assembly and
  `build/ovl_11/ovl_11.map` agree: D314 starts at 0x8010D314 and occupies
  0x334 bytes, ending exactly at D648; D648 occupies 0x100 bytes and ends
  exactly at D748. These are address/size observations, not evidence from
  the reconstruction's separate object-file names.
- `build/ovl_11/asm/nonmatchings/ovl_11_func_8010D314/ovl_11_func_8010D314.s`
  calls D648 at 0x8010D5A8 with the current object and area 5, then at
  0x8010D5D8 with that same object and area 4. Its nonzero results select
  further fallback helpers. This directly corroborates the adjacent
  caller/helper relationship rather than relying on proximity alone.
- The verified source supplies the matching helper role: derive an attempt
  count from the object's unsigned halfword at +0xB2 and `func_80012A34(3)`,
  request indices through `ovl_11_func_80110944(area, 1)`, consume the
  area-selected counter, and flag successful indices through
  `ovl_11_func_800D0EA4`. It returns a failure flag to the caller's fallback
  chain. D314 also derives attempt counts from neighbouring object fields
  +0xAE and +0xB0 using division plus `func_80012A34`; this corroborates the
  local object-action family, not a distinctive compiler quirk.

## Conclusion and limits

Record D314 and D648 as low-confidence same-TU candidates within the existing
object-mode dispatch section. The new evidence is the verified helper role
coupled to its gapless predecessor's direct calls and fallback handling.

This does not establish original TU boundaries or promote the wider section's
confidence. D648 defines no shared static or global, and this review found no
new declaration-order or register-variable evidence. Do not infer that its
link-separate counter/index helpers share its TU, or extend membership through
D748/D850 solely from their participation in the fallback chain.
