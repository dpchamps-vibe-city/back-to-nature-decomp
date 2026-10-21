# ovl_11_func_800DD374: three-slot initializer/transition grouping

## Verified handoff

The verified source is `src/overlays/ovl_11/ovl_11_func_800DD374.c`.
The supplied source/evidence identity is
`14b37fcd62f798613db7c83854b24dc18f2ae2065c0a6deb687f974b4746393c`.
The handoff reports byte identity and a passed full finalize gate, with changes
in that source, `include/globals_override.h` and `include/overlays/ovl_11.h`.
This note records grouping evidence only; verification was not repeated.

## New membership evidence

- **Initializer/consumer global cluster:** 800DD374 clears all three pointer
  slots in `D_80129178` and the immediately following state word
  `D_80129184`, then populates the slots from the fifteen 0x30-byte entries
  of `D_80128E08`. The original
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800DD374/ovl_11_func_800DD374.s`
  shows the four clears at 800DD394–800DD3A4 and indexed pointer store at
  800DD444. The original
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800DD4D8/ovl_11_func_800DD4D8.s`
  reads the three slots at 800DD4DC, 800DD548, 800DD564/800DD568 and
  800DD580, and dispatches on `D_80129184` loaded at 800DD4F8.
- **Shared entry/subrecord protocol:** the initializer selects entries by
  the +0x28 halfword, clears their +0x2C pointee's +0x2E counter, and sets
  its +0x30 limit. `build/ovl_11/functions/ovl_11_func_800DD45C.s`
  increments and compares those same counter/limit fields, resets counters,
  transfers the entry +0x28 bit 0x1000, and writes the next state to
  `D_80129184` at 800DD4CC. These are the entries passed by 800DD4D8.
- **Gapless link order:** `configs/splat/ovl_11.yaml` places 800DD374,
  800DD45C and 800DD4D8 at offsets 0x25554, 0x2563C and 0x256B8.
  `build/ovl_11/functions.csv` gives sizes 0xE8, 0x7C and 0xD8:
  0x800DD374 + 0xE8 = 0x800DD45C, and
  0x800DD45C + 0x7C = 0x800DD4D8. The initializer thus directly precedes
  the already documented caller/helper pair, without intervening code.
- **Existing core call edges:** the original 800DD4D8 assembly calls
  800DD45C at 800DD54C, 800DD56C and 800DD584 for successive slot pairs.
  This corroborates the shared-state data flow and the core's link order;
  800DD374 itself is a leaf, not a direct caller of either core member.

The former ledger description of a two-word block referenced only by the
caller/helper pair is superseded: `D_80129178` has three pointer slots,
followed by the state word, and 800DD374 initializes all four words.
The previously noted incoming-$v0 save in 800DD45C at 800DD464 is a
per-function compiler detail, not evidence for this initializer's membership.

## Membership and limits

800DD374 is a **medium-confidence same-TU extension** of the documented
800DD45C/800DD4D8 core (whose existing high-confidence classification is
retained). This assumes the consecutive initializer/helper/driver run and
its shared narrow state cluster belong to one original object rather than
straddling a link boundary. No shared static, symbol visibility or
source-declaration-order observation proves that boundary. Absolute-addressed
global accesses do not establish ownership; the new header declarations do
not establish original linkage or types beyond the witnessed layout.

No merge with other `D_80128E08` users is proposed. In particular,
800DD5B0 is a similar initializer for a separate pointer/state cluster, and
800DCA60 calls several such initializers. Those similarities and call edges
supply family context, not proof that all the existing groups share one TU.
