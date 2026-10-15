# ovl_11_func_800FECB8: translation-unit grouping evidence

Verified handoff identity: `5559bae96cef260e065c6fa41047a048f92e06e60972a8142cd00676e66e422e` (origin: static).
Source SHA-256: `12ebf8f1991151d0804ac78947d202e5d35164f113ca47c142c8e5bd628907e9`.
The supplied handoff reports byte identity and a passed full finalize gate;
this documentation pass did not modify build inputs or repeat solver work.

## New membership evidence

- **Direct call agrees with gapless link order.**
  `src/overlays/ovl_11/ovl_11_func_800FECB8.c` calls already-listed
  `ovl_11_func_800FED0C`.
  `configs/splat/ovl_11.yaml` places the wrapper at offset `0x46E98`
  and the dispatcher immediately next at `0x46EEC`, a `0x54`-byte span.
  The extracted target in
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800FECB8/ovl_11_func_800FECB8.s`
  confirms the call at `0x800FECE8` and the final instruction at
  `0x800FED08`, immediately before the dispatcher's `0x800FED0C` entry.
- **Shared active-menu state.** The wrapper runs the dispatcher only when
  `D_801273B0 != 0`, surrounding it with `func_80017A38(0, 2)` and
  `func_80017A38(0, 0)`. The dispatcher's verified source,
  `src/overlays/ovl_11/ovl_11_func_800FED0C.c`, reads and advances that
  same menu-state word and clears it on completed settings restoration.
  This supports a wrapper/dispatcher relationship within the menu-state
  cluster, rather than proximity alone.

These call/link and shared-state ties support adding 800FECB8 beside 800FED0C
in the existing `D_801273D8/D_801273DA` init/reset-pair section. The wrapper
does not access that flag pair itself; its membership is through the
already-listed dispatcher. The assignment remains a medium-confidence
suspected translation unit, conditional on the existing cluster assignment;
it does not establish original file boundaries or strengthen other members.
