# ovl_11_func_800FED0C: translation-unit grouping evidence

Verified handoff identity: `73eb18f3f634079fd36375138c7a85199fcfbf8373140ff62d0900d95fa02478`.
Source SHA-256: `490653f9b58853e70f44e48620a7139c8446ca7689ee920305e9408dcb097f42`.
The supplied handoff reports byte identity and a passed full finalize gate;
this documentation pass did not modify build inputs or repeat solver work.

## New membership evidence

- **Callback edge agrees with gapless link order.**
  `src/overlays/ovl_11/ovl_11_func_800FED0C.c` dispatches through
  `D_801273C0[(state >> 4) - 1]`. The extracted table in
  `build/ovl_11/asm/data/69960.data.s` at `0x801273C0` contains
  `ovl_11_func_800FEFEC`, followed by `ovl_11_func_800FF324`.
  `configs/splat/ovl_11.yaml` places 800FED0C at offset `0x46EEC` and
  already-listed 800FEFEC immediately next at `0x471CC`, a `0x2E0`-byte span.
  The target in
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800FED0C/ovl_11_func_800FED0C.s`
  confirms the indirect call at `0x800FEE28` and the final instruction at
  `0x800FEFE8`, immediately before 800FEFEC's entry.
- **Complementary access to the same menu-state cluster.** 800FED0C handles
  state `1` using selector `D_801273B4`, then selects state `0x10` or `0x20`.
  `src/overlays/ovl_11/ovl_11_func_800FEFEC.c` handles the `0x10` menu and
  returns `D_801273B0` to `1` on cancel.
  `src/overlays/ovl_11/ovl_11_func_800FEBF8.c` resets that state word and
  `D_801273B4/B6/B8/BA/BC` together. This connects the new dispatcher to
  the state-block relationship already cited by the ledger.
- **Settings save/restore and an existing band member.** The target's
  `0x800FED64–0x800FED90` copy restores the twelve bytes at `D_8012CEE8`
  to `D_8006C838 + 0xE4CC`, reversing the save at `0x800FEC70–0x800FEC9C`
  in `build/ovl_11/asm/nonmatchings/ovl_11_func_800FEC2C/ovl_11_func_800FEC2C.s`.
  It then calls already-listed `ovl_11_func_800FF96C` at `0x800FEDB0`;
  `src/overlays/ovl_11/ovl_11_func_800FF96C.c` applies the settings at
  work-area offsets `0xE4CE/0xE4D0/0xE4D2`. This supplies a functional
  settings-cluster tie beyond that member's previously cited link adjacency.

The callback/link and shared-state evidence supports adding 800FED0C to the
existing `D_801273D8/D_801273DA` init/reset-pair section, through 800FEFEC
and 800FF96C. It does not itself access that flag pair. The grouping remains
a medium-confidence suspected translation unit, conditional on the existing
cluster assignment; this evidence does not establish original file boundaries
or justify adding every function between the listed members.
