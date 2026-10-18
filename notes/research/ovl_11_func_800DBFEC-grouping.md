# ovl_11_func_800DBFEC: scene-setup/render-context helper pairing

## Evidence

The verified source is `src/overlays/ovl_11/ovl_11_func_800DBFEC.c`;
the supplied source/evidence identity is
`56618a0aa5cd309a394f69bd46346ae6494043cfbe33a93ef686df7b9bd66411`.
The handoff reports byte identity and a passed full finalize gate, with changes
to that source and `include/globals_override.h`. This note records grouping
evidence only; it does not repeat verification.

- **Gapless link order:** `configs/splat/ovl_11.yaml` places 800DBFEC at
  offset 0x241CC and 800DC114 immediately next at 0x242F4.
  `configs/symbols/ovl_11.txt` agrees with that order. The original
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800DBFEC/ovl_11_func_800DBFEC.s`
  gives size 0x128 and a final return delay slot at 800DC110:
  0x800DBFEC + 0x128 = 0x800DC114.
- **Direct call across that boundary:** the original assembly calls
  800DC114 at 800DC094 with all three arguments zero. The verified C preserves
  this unconditional call after scene-grid setup and before header-selected
  initialization. `src/overlays/ovl_11/ovl_11_func_800DC114.c` supplies the
  complementary helper role: write the three color bytes at +0x19..+0x1B of
  both `D_8005E5E8` render contexts and set each +0x18 flag to 1.

## Membership and limits

800DBFEC and 800DC114 are a **low-confidence candidate same-TU pair**, assuming
this adjacent caller/helper sequence lies within an original object rather
than across an object boundary. The direct call and gapless link order agree;
no shared static, symbol visibility or declaration-order effect proves the
boundary.

The helper is not private to this caller:
`src/overlays/ovl_11/ovl_11_func_800DDB64.c` and
`src/overlays/ovl_11/ovl_11_func_800DDBA0.c` also call it. Its
`D_8005E5E8` render-context global is shared RAM, not evidence of TU ownership.
Those other callers are not added to this pair.

The setup routine's `D_8007AFF0` work-area access, `D_80128818` descriptor,
coordinate inputs `D_800A00C4`/`D_800A00CC`, and calls to 800BCF54/800DB00C
establish data flow, not membership in those distant functions' units. In
particular, this pair does not extend the ledger's D_8012D110 record pair,
where 800DBFEC was already mentioned only as a data-family tie. The separate
candidate is recorded alongside that existing mention, not merged with it.
No extension to predecessor 800DBFE4 or successor 800DC148 is proposed.
