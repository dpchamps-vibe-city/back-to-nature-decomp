# ovl_11_func_800CC4E0: translation-unit grouping evidence

Documentation-only review of `src/overlays/ovl_11/ovl_11_func_800CC4E0.c`.
The supplied handoff reports byte-exact matching and a passing full finalize
gate, with source/evidence identity
`b242014b91a899f1cc6c4dcc5e41db69ec0bf4bb4319f37cea7e5f9ec39830da`.
No solver work or build-input changes were made during this review.

## New evidence

- **Shared sprite initializer/draw pair.** The verified function initializes
  `D_80128C80` from `D_800977F8` through `func_80015704`, calls
  `func_80015840` on it with argument 0x17, and sets the object's +0x6 state to 3.
  Its immediate predecessor `ovl_11_func_800CC0F8` draws that same sprite
  through `func_80015C50` at 0x800CC394 for object states 3/4; its state-3
  path also tests bit 0x100 at `D_80128C82` (sprite +2), at 0x800CC2F4.
  These are complementary producer/consumer uses, not merely references
  to a broadly shared engine work-area base.
- **Gapless link order.** Original assembly sizes give
  `800CC0F8 + 0x3E8 = 800CC4E0`,
  `800CC4E0 + 0x1C8 = 800CC6A8`, and
  `800CC6A8 + 0x124 = 800CC7CC`.
  `800CC7CC` occupies 0x3C4 bytes, ending at 0x800CCB90.
  `configs/splat/ovl_11.yaml` lists the same four functions consecutively
  at file offsets 0x142D8, 0x146C0, 0x14888 and 0x149AC.
- **Corroborating local call graph.** `800CC0F8` calls `800CC7CC` at
  0x800CC21C. That dispatcher calls `800CC4E0` at 0x800CC9D0 and
  `800CC6A8` at 0x800CCB48, linking the adjacent sprite transition and
  grid-query helpers to the update/draw driver.

Assembly evidence is in
`build/ovl_11/asm/nonmatchings/<function>/<function>.s` for the four named
functions. The matched grid helper's source is
`src/overlays/ovl_11/ovl_11_func_800CC6A8.c`.

## Limits

This supports a medium-confidence original-TU hypothesis, strongest for
`800CC0F8`/`800CC4E0`; the wider run rests on corroborated call/link adjacency.
It does not establish a private static, GP-relative ownership, a TU-specific
compiler quirk, or exact file boundaries. The two-argument sprite-loader idiom
and accesses to `D_8007AFF0`/`D_8006C838` alone do not justify merging distant
families. Likewise, the `D_8012DB80` flag's distant consumers are not added to
this group merely because the verified function writes that flag.

## Extension review: ovl_11_func_800CCBE8

The supplied handoff reports byte-exact matching and a passing full finalize
gate for `src/overlays/ovl_11/ovl_11_func_800CCBE8.c`, source/evidence identity
`2f5dbc8a0223676db41900354184c2a16d770882ad9a04e088c20f1e2e2c66b4`,
origin agent; the supplied changed-file list contains only that source.
This review changed documentation only and did not repeat the solver gate.

### Additional membership evidence

- **Shared local removal helper.** The verified `800CCBE8` clears the object's
  item halfword at +2 and bit 0x40 of `D_8006C838`+0x5234. For item 0x186 it
  saves the object's +0x24/+0x28/+0x2C coordinates, with Y minus 10, into
  `D_8012DB78`; otherwise it calls `800CBD78` on `D_8006C838`+0x51C8.
  The existing group's update/draw driver `800CC0F8` calls it at
  0x800CC2E0 and 0x800CC308, passing the current object. Its interaction
  dispatcher `800CC7CC` calls it at 0x800CC9B8, 0x800CCA60, 0x800CCABC and
  0x800CCAFC. Thus the newly verified function is a shared callee of two
  existing group members, not merely another user of an engine base.
- **Gapless extension with a corroborating caller.** The original assembly
  gives `800CC7CC + 0x3C4 = 800CCB90`,
  `800CCB90 + 0x58 = 800CCBE8`, and
  `800CCBE8 + 0x74 = 800CCC5C`.
  `configs/splat/ovl_11.yaml` lists the two new candidate members at offsets
  0x14D70 and 0x14DC8, immediately after the recorded run.
  The intervening `800CCB90` walks the three `D_80128BB0` entries at +0x34
  stride, tests each item's +2 halfword, and calls `800CC0F8` for occupied
  entries at 0x800CCBBC. Its traversal therefore connects the intervening
  function to the same object-update family in both call graph and link order.

Evidence paths: the verified source above and
`build/ovl_11/asm/nonmatchings/<function>/<function>.s` for `800CC0F8`,
`800CC7CC`, `800CCB90` and `800CCBE8` (using their full function names).

### Limits of the extension

These edges support extending the existing medium-confidence candidate run
through `800CCB90` and `800CCBE8`, not establishing an exact TU boundary.
`800CCB90` also remains a member of the ledger's `D_80128BB0` init/accessor
hypothesis; the overlap is corroborating evidence, not proof that every
neighbour belongs to one original file. No private static, GP ownership,
declaration-order constraint or TU-specific register quirk was established.
The already-recorded `D_8012DB78` relationship to distant `80120850` does not
justify merging that state cluster into this run; nor does the call to
`800CBD78` alone establish that callee's original-TU membership.

## Extension review: ovl_11_func_800CC7CC

The supplied handoff reports byte-exact matching and a passing full finalize
gate for `src/overlays/ovl_11/ovl_11_func_800CC7CC.c`, source/evidence identity
`e1f1be83f6bcf90b3c707e3f920b388bb209e3cf571d2ca6a7fad06ee0f50f66`,
origin agent. Its supplied changed-file list is `include/globals_override.h`,
`include/sdk_types.h` and that source. This review changed documentation only
and did not repeat the solver gate.

### New shared-table evidence

The verified dispatcher reverses the object's direction at +0x8 modulo four,
reads the corresponding 8-byte pair from `D_80123130`, and writes twice the
pair's signed words to X/Z velocity at +0x14/+0x1C. Original instructions
0x800CC878–0x800CC8B8 show the direction update, table addressing and stores;
the subsequent path preserves those velocities for item 0x65 and otherwise
clears them.

`src/overlays/ovl_11/ovl_11_func_800CBF70.c` allocates an entry from the three
0x34-byte `D_80128BB0` records, stores its direction at +0x8 and initializes
those same velocity fields from the same table pair. Its item-0x65 case also
doubles both table components. The original assembly places the table base
at 0x800CBFA0/0x800CBFA4, the direction store at 0x800CBFE4, ordinary velocity
stores at 0x800CC01C/0x800CC038, and doubled stores at
0x800CC054/0x800CC058. Thus the new tie is a specific initializer/update
relationship, not another use of a general engine base. The existing init-run
ledger already records the allocator's table use, but not this shared use by
`800CC7CC`.

### Corroboration and membership limits

The already-recorded link relation `800CBF70 + 0x188 = 800CC0F8` makes the
allocator the gapless predecessor of the sprite/helper run. The existing
call path `800CCB90` → `800CC0F8` → `800CC7CC` passes occupied entries from
that same pool into the dispatcher. Together with the new table/field tie,
this supports adding `800CBF70` to the medium-confidence candidate run.

Evidence paths: the two C sources above,
`build/ovl_11/asm/nonmatchings/<function>/<function>.s` for full names
`ovl_11_func_800CBF70`, `ovl_11_func_800CC7CC`,
`ovl_11_func_800CC0F8` and `ovl_11_func_800CCB90`, and
`configs/splat/ovl_11.yaml` (allocator offset 0x14150, followed by updater
0x142D8). The current nonmatching assembly corpus contains only the allocator
and dispatcher as named users of `D_80123130`; this is not proof that the
table was private or that all original references are named.

The premise remains that this linked pool lifecycle and local direction-table
reuse reflect source-file locality. No private static, GP ownership,
declaration-order effect or register-variable quirk was established. The
allocator also remains in the separate `D_80128BB0` init-run hypothesis;
this overlap does not merge every member of those hypotheses or establish
an exact TU boundary. The dispatcher's distant spawn/helper calls and shared
`D_8006C858` item-table accesses do not justify adding their other users.
