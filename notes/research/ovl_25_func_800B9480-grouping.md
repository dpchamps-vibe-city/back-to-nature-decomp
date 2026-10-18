# ovl_25_func_800B9480 grouping evidence

Verified source/evidence identity: `30dfee7152de12c7d909559fa9988ecd035022139cc8c5ddc9f99c4e25ba5287`.
The agent handoff reports byte identity and a passed full finalize gate for
`src/overlays/ovl_25/ovl_25_func_800B9480.c` and
`include/overlays/ovl_25.h`. This notes-only pass does not repeat verification.

## Extension of the existing s16-record predicate candidate

The new evidence supports tentatively extending the existing
`800B9758`/`800B977C`/`800B97E4` group backwards to include the decision and
fallback pair `800B9480`/`800B95B0`:

- The verified target calls `ovl_25_func_800B977C(arg0, 4)` and, when that
  predicate does not return 1, calls `ovl_25_func_800B95B0(arg0)`.
  `build/ovl_25/functions/ovl_25_func_800B9480.s` corroborates these edges at
  `0x800B9498` and `0x800B9590`. Thus the new pair has a direct call edge
  into the existing predicate run, as well as an internal fallback edge.
- The original listings under `build/ovl_25/functions/` give a gapless chain:
  `800B9480 + 0x130 = 800B95B0`, `800B95B0 + 0x1A8 = 800B9758`,
  `800B9758 + 0x24 = 800B977C`, and `800B977C + 0x68 = 800B97E4`.
  In particular, the fallback is the target's immediate link successor.
- The target reads the halfwords at `D_800BFE44 + 0x4CE/+0x4D4/+0x4D8`.
  The original fallback listing `ovl_25_func_800B95B0.s` reads the same
  `+0x4D8` at `0x800B9700` and the adjacent `+0x4D6` at `0x800B9710`.
  `src/overlays/ovl_25/ovl_25_func_800B97E4.c` reads the target's `+0x4D4`
  and more halfwords in the same parameter cluster; its original listing
  corroborates the shared field at `0x800B980C` and `0x800B99D0`.
  All three use `ovl_25_func_800B9B00` to change the argument record's state
  and timer. That setter's source is
  `src/overlays/ovl_25/ovl_25_func_800B9B00.c`.

This is call/link-order agreement reinforced by shared overlay-local
parameters, not proof of a translation-unit boundary. Retain low confidence
for the wider candidate, especially the intervening `800B9758` predicate.
The state base is absolute-addressed shared storage, not an established
private static or GP-relative ownership witness. The reconstructed local
pointer and function declarations do not establish original declaration or
register-variable quirks. Do not merge the neighbouring record-array driver
or other users of this large state block on this evidence alone.

## Previously recorded predicate-run evidence

The original `800B9758`/`800B977C` predicate pair reads signed halfword record
fields at `+2` and `+0`, respectively, and returns s32 booleans. Their sources
are `src/overlays/ovl_25/ovl_25_func_800B9758.c` and
`src/overlays/ovl_25/ovl_25_func_800B977C.c`; their original function listings
corroborate the field widths and adjacency above. The former rejects states
7 and 4. The latter calls `func_80012A34(0x80)` and returns
`(result < D_800BCCA0[5 * (field - 1) + arg1]) ^ 1`.

The earlier ledger also associated `D_800BCCA0` with the neighbouring
`D_800BCC10`/`D_800BCC70` data region (`D_800BCC10 + 0x90 = D_800BCCA0`).
Those are different symbols: regional adjacency alone does not establish
shared-symbol use or justify merging the groups.
