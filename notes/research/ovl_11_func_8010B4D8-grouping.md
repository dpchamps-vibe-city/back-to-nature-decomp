# ovl_11_func_8010B4D8: dual-table dispatcher family extension

## Verified handoff

The handoff reports byte-exactness and a passed full finalize gate for
`ovl_11_func_8010B4D8`, with source/evidence identity
`4c5b7fae4878fd56c1687cfbdbb59589958ae50f4030317e5cadc1e2fdf6cab0`.
Reviewed source: `src/overlays/ovl_11/ovl_11_func_8010B4D8.c`.
This documentation pass did not rerun matching or change build inputs.

## New grouping evidence

- **Shared table and complementary record fields.** The verified function
  dispatches through `D_800BAA34[u16@+0x26]`. Its immediate successor,
  `src/overlays/ovl_11/ovl_11_func_8010B57C.c`, tests the same table entry
  before recording a new selection at +0x26 and clearing +0x28/+0x2A/+0x2C.
  The target increments that +0x2C counter and, when flag 0x800 at +0x34 is
  clear and the counter reaches 300, invokes `ovl_11_func_8010B8DC` and
  resets the counter. Both functions use the same +0x34 flag word.
- **Gapless link order.** The original target assembly,
  `build/ovl_11/asm/nonmatchings/ovl_11_func_8010B4D8/ovl_11_func_8010B4D8.s`,
  is 0xA4 bytes: `0x8010B4D8 + 0xA4 = 0x8010B57C`.
  `build/ovl_11/functions.csv` and `configs/splat/ovl_11.yaml` corroborate
  the consecutive functions at offsets 0x536B8 and 0x5375C.
- **Call graph agrees with the local link run.** The target calls
  `ovl_11_func_8010B8DC` at 0x8010B550. The original selector assembly,
  `build/ovl_11/asm/nonmatchings/ovl_11_func_8010B8DC/ovl_11_func_8010B8DC.s`,
  reads the same +0x26 selection and +0x34 flags, then calls
  `ovl_11_func_8010B57C` at 0x8010B978 when the desired selection differs.
  This closes a handler-step / selector / transition-dispatcher relationship,
  rather than relying on adjacency alone.

## Existing family context

The ledger already associated `8010B57C` with the gapless initializer band
at `8010B64C` and its continuation through `8010B8DC` / `8010B994` to
`8010BA84`. Both `8010B8DC` and `8010B994` call `8010B57C`; the original
`8010B994` assembly contains one such call, at 0x8010BA5C.

The current table evidence is `build/ovl_11/asm/data/2BB0.rodata.s`:
`D_800BA9E4` and `D_800BAA34` are adjacent twenty-word handler tables
(`0x800BA9E4 + 0x50 = 0x800BAA34`). The former names the documented
`8010BA84`–`8010BCE8` far-state gate run and `8010B994`; the latter contains
handler addresses such as `8010BD6C`, `8010BED4` and `8010BE24`, not boolean
flags. `8010B57C` uses the latter only as a presence test, whereas the target
calls through it. `D_800BAA84` begins immediately after it at 0x800BAA84.
The old ledger's `2A28.rodata.s` reference predates the current data split.

## Conclusion and limits

Add `ovl_11_func_8010B4D8` to the existing **medium-confidence candidate
same-TU dispatcher family**, without raising confidence or merging the
separately recorded initializer and handler groups. The new membership rests
on shared table/state access plus gapless link order corroborated by calls.
Original TU boundaries remain unproven: the shared extern declaration and
modern per-function source split do not establish original ownership. No
new static-linkage, declaration-order or register-variable evidence was
found. Do not extend membership to the preceding `8010B49C` on adjacency
alone.
