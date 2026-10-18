# ovl_11_func_800C74AC: item-code helper/dispatcher grouping

## Verified anchor

Supplied source/evidence identity:
`8b313ce96981ac75e838291ed497749b0103bdc842f5f199487510627496f01e`
(origin: agent). The handoff reports byte identity and a passed full finalize
gate for `src/overlays/ovl_11/ovl_11_func_800C74AC.c`, its sole listed changed
file. This notes-only review accepts that verification without modifying or
re-verifying build inputs.

## New local membership evidence

- **Gapless helper run.** `configs/splat/ovl_11.yaml` places 800C72F8,
  800C73FC, 800C74AC and 800C7524 at offsets 0xF4D8, 0xF5DC, 0xF68C and
  0xF704. Their original assembly sizes are respectively 0x104, 0xB0,
  0x78 and 0x12C: each ends exactly at the next, with the last ending at
  800C7650. The assembly evidence is under
  `build/ovl_11/asm/nonmatchings/<function>/<function>.s`.
- **The call graph agrees with helper link order.** In that directory's
  `ovl_11_func_800C76EC/ovl_11_func_800C76EC.s`, calls at 800C7774,
  800C7784, 800C7794 and 800C77AC try those four helpers in exactly that
  order. Every call receives the same object and its `+0x84` item record.
  The caller treats their scalar results as handled/not-handled decisions;
  success in any of the first three clears the object's byte at +0x265.
  The dispatcher starts at 800C76EC, separated from the last helper by the
  intervening 800C7650 function, so it is nearby, not directly adjacent.
- **Shared dispatch idiom.** The verified target tests the record's u16
  code for 0x22–0x36, ORs 0x1800 into object flags at +0x6C, and calls
  800C70CC with state 0x53, the signed item-table halfword at +0x22, and
  -2. The immediately preceding 800C73FC tests codes 0x1A–0x1D and uses
  the same flag mask and dispatcher/-2 tail. The following 800C7524 has
  code-selected branches with that same flag/dispatcher idiom. 800C72F8
  handles codes 1–0x19 and also reaches 800C70CC, including a branch with
  the same 0x1800 mask. These are ordered cases of one local dispatch
  family, not merely unrelated users of a public item-table type.

## Inference and limits

The matching helper now supports **medium-confidence candidate same-TU
membership** with 800C72F8, 800C73FC, 800C7524 and their local dispatcher
800C76EC. This rests on the premise that the gapless helper layout,
matching call order, common record argument and repeated flag/dispatch
idiom reflect one original source unit. Separate linked units remain
possible; no shared static or original TU boundary has been established.
The four neighbours remain assembly stubs in `src/`; their roles above
come from original instructions, not newly verified C reconstructions.

This does not merge the family with the distant 800D5750–800D6090 accessor
run: `include/globals_override.h` identifies D_8006C858 as an extern
main-binary item-table pointer, and its reuse was already recorded in the
ledger. Nor do calls alone establish membership of the widely used
800C70CC dispatcher, 800CB020/800CB0B0 mapping pair, or intervening
800C7650. Those functions retain their existing ledger placements.
