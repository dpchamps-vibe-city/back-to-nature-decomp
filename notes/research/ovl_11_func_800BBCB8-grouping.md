# ovl_11_func_800BBCB8 — counter-state run membership

## Verified handoff

- Identity: `c59d18b43e1b00e9c7ddb2658f71d407b9bef3458bcb854890abe733569352ec` (origin: agent).
- The supplied handoff and `run_output/autoloop/state.json` completion for
  `ovl_11_func_800BBCB8` report passed verification. This is notes-only
  documentation, not a new build or matching claim.
- Verified source: `src/overlays/ovl_11/ovl_11_func_800BBCB8.c`.

## New grouping evidence

The function supports extending the existing `D_801287F4` counter-state run
backward from `800BC3AC` to `800BBCB8`:

- **Immediate link neighbour:** `configs/splat/ovl_11.yaml` places the target
  at file offset 0x3E98 and `800BC3AC` at 0x458C. The target's original
  assembly declares size 0x6F4: 0x800BBCB8 + 0x6F4 = 0x800BC3AC.
- **Complementary counter/state operations:** the verified target sets
  `D_801287F4 = 3` and decrements the signed word at `D_8006C838+0x4488`
  in its completion paths. Its immediate successor's matched source,
  `src/overlays/ovl_11/ovl_11_func_800BC3AC.c`, clears `D_801287F4` and
  increments that same counter. The next function, `800BC3E0`, dispatches
  on `D_801287F4`.
- **Shared control words:** the target reads/writes `D_801217A0`, which
  `800BCD28` reads and clears, and writes the pending-mode word at
  `D_8007AFF0+0x2548C`, which `800BC3AC` clears. These specific shared
  fields corroborate adjacency; use of the ubiquitous base pointers alone
  would not establish membership.

The target's `WORK` and `STATUS` field offsets are recorded by
`Ovl11BBCB8Work` and `Ovl11BBCB8Status` in `include/globals_override.h`.
Original assembly witnesses for the target, `800BC3E0` and `800BCD28` are
`build/ovl_11/asm/nonmatchings/<function>/<function>.s`.

## Limits

This supports candidate same-TU membership in the existing medium-confidence
run, not a proven original translation-unit boundary. No shared static,
private linkage, or global ownership is established. The target's calls to
other helpers do not justify adding those callees to the group, nor does
this evidence prove membership of the address-apart initializers already
recorded as candidates.
