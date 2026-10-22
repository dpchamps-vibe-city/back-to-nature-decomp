# ovl_17_func_800B9324: six-record decision hub

## Verified handoff

`src/overlays/ovl_17/ovl_17_func_800B9324.c` passed the exact diff, full
configured build, scope and clean-source gates. Documentation handoff identity:
`a55bb8fef1bd12d77e30e18f6d9ef34bc7e8d0687ec7beace4d53afa64ffddbf`.
The integrated winner is `build/ovl_17_func_800B9324/clean.c`, exact at
82/82 words. No flag override or source-policy exception was added.

## Grouping evidence

The existing low-confidence D_800BD848 record-area group already listed
this function as the caller of 800B9CE4, 800B9F44, 800B9F10 and 800B9F78.
The verified implementation adds a specific producer/consumer link to its
immediate successor:

- 800B9324 walks six 0x50-byte records and writes the s32 at +0x30 in each
  record, clearing a prior value of 1 or selecting a new result through
  the helper sequence and the halfword table at work-area +0x208.
- Matched 800B946C tests those same +0x30 words before updating the
  corresponding record's +0x2A and +0x2E halfwords. Both functions also
  consult the mode halfword at D_80074838+0x6514.
- The target's 0x148-byte body at 800B9324 ends exactly at 800B946C.
  Matched shared caller 800B8898 invokes 800B9324 and 800B946C
  consecutively, in that same link order, followed by 800B9594.

Shared per-record producer/consumer state, gapless adjacency and ordered
calls corroborate a local subgroup within the existing candidate band.
They do not prove an original translation-unit boundary or static linkage.
The source's choice of symbolic alias is not independent TU evidence.

Evidence:
- `src/overlays/ovl_17/ovl_17_func_800B9324.c`
- `src/overlays/ovl_17/ovl_17_func_800B946C.c`
- `src/overlays/ovl_17/ovl_17_func_800B8898.c`
- `build/ovl_17/asm/nonmatchings/ovl_17_func_800B9324/ovl_17_func_800B9324.s`
- `include/globals_override.h`: D_800BD848 and its D_800BD870 record alias

## Matching mechanism, not grouping evidence

The earlier v68-family search exhausted 4518 candidates with a population
floor of four instructions. That result was conditional on its symbolic
storage origins and source-copy geometry, not a proof that clean C or the
production flags could not match.

The new scratch sequence under `build/ovl_17_func_800B9324/` separates
independently witnessed storage views: the persistent table base comes from
D_800BD870-0x28, while the in-loop settings base comes from D_800BD848.
They address the same storage but remain distinct symbolic values to the
compiler. `record-alias-origin.c` removes the population floor without
changing flags. A diagnostic -fno-gcse probe also removed that floor for
the earlier single-origin source, but was unnecessary and was not adopted.

`offset-web.c` reuses the doubled column offset for the group-stride
addition, preserving the target's local register web. The remaining saved
register rotation came from CSE reversing an adjacent register copy:
`tools/vendor/gcc/2.95.2/src/gcc/cse.c`, around line 7923, explicitly swaps
such a copy and the destination of the preceding computation. An intervening
real initialization prevents that transformation.

`separated-copy.c` puts the mode-base capture between result-base formation
and the cursor copy, removing the saved-register rotation. The final
`early-work-offset-break.c` instead captures the mode base earlier and puts
the offset's zero initialization between the pointer assignments. This also
fixes the preheader high-half quantity's allocation, reaching exact 82/82.
The offset-first integer store-address expression preserves the target's
commutative operand order. Removing unused declarations and unrelated
headers, using readable locals and integer arithmetic for the alias address
in `clean.c` preserves exactness.

These source/pass findings do not add TU evidence. Measurements and scoped
closure records are retained by `build/experimentLedger/ovl_17_func_800B9324.jsonl`
and the experiment tools; the live function required no compiler override.
