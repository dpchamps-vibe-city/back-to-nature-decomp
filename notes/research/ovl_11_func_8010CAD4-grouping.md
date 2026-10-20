# ovl_11_func_8010CAD4: tentative seven-record run membership

## Verified handoff

- Source: `src/overlays/ovl_11/ovl_11_func_8010CAD4.c`.
- Supplied verified source/evidence identity:
  `f4f29553670c7d00d86535062f4f3ed33e782e5fefd7b1d51460ad015f46f619`.
- The handoff reports byte identity and a passed full finalize gate. This
  documentation-only review accepts that result without rebuilding or changing
  build inputs.

## Evidence

- The verified body gates a call to `ovl_11_func_8010D314` on the unsigned
  halfwords at `D_8006C838+0xE4C8` and `+0x44C0`, the result of
  `ovl_11_func_800C0824`, and bit 0x40 being clear in the object's u16 at
  +0xB8. It passes the original object pointer unchanged and returns zero.
- `build/ovl_11/asm/nonmatchings/ovl_11_func_8010C95C/ovl_11_func_8010C95C.s`
  calls CAD4 at 0x8010C98C on each eligible record in the seven-entry,
  0xF8-stride `D_8007A638` array. This caller relationship was already
  recorded in `ovl_11_func_8010C9B4-grouping.md`; it is not itself new evidence.
- The verified wrapper supplies the additional downstream edge: CAD4 calls
  D314 at 0x8010CB4C, witnessed also by
  `build/ovl_11/asm/nonmatchings/ovl_11_func_8010CAD4/ovl_11_func_8010CAD4.s`.
  D314 dispatches on bits 1/2/4 of the same +0xB8 halfword, as shown in
  `build/ovl_11/asm/nonmatchings/ovl_11_func_8010D314/ovl_11_func_8010D314.s`.
  CAD4 is thus a per-record gate into the nearby object-action dispatcher,
  not merely an intervening address after the array sweeps.
- `configs/splat/ovl_11.yaml` records the local sequence C95C, C9B4, CA0C,
  CAA8, CAD4, CB6C, continuing through the dispatch run to D314. CAD4 occupies
  0x98 bytes at 0x8010CAD4 and ends exactly at CB6C, consistent with its target
  assembly and `build/ovl_11/ovl_11.map`. These are target address/size facts;
  the reconstruction's separate object-file names do not establish original
  translation-unit boundaries.

## Interpretation and limits

Add CAD4 as a low-confidence candidate in the existing seven-record state
section: its known sweep caller and newly documented local dispatch edge
corroborate the already-recorded link-order continuation. Retain the core
section's medium confidence, and do not merge it with the object-mode dispatch
section. The role is a state/flag-gated per-record action wrapper.

This remains conditional on the local sweep/action family belonging to one
original TU. CAD4 does not reference `D_8007A638` directly, and neither a nearby
call nor the common object layout proves same-TU membership. The shared
`D_8006C838` buffer is widespread; no new shared static, global definition,
declaration-order constraint or register-variable quirk was established.
Do not infer membership for CA0C/CAA8, the distant C0824 helper, or other
intervening functions from these edges alone.
