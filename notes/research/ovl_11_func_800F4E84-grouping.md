# ovl_11_func_800F4E84: pool-gate caller and adjacent completion leaf

## Provenance and evidence

The agent handoff reports a byte-exact result and a passed full finalize gate,
with verified source/evidence identity
`909b4cf830671f25c779786b04e014a738f06dafe889b258447ea682db5ec6fa`.
Its changed-file list names `include/globals_override.h` and
`src/overlays/ovl_11/ovl_11_func_800F4E84.c`. This documentation pass did not
rerun verification or edit build inputs.

Evidence read:
- `src/overlays/ovl_11/ovl_11_func_800F4E84.c` (SHA-256
  `521f347d470ca895413a03762fe5fa77a085d4ae9b9ee264f116f8441da3c0e9`)
  and the pool-gate views in `include/globals_override.h`.
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800F4E84/ovl_11_func_800F4E84.s`:
  original pool reads, position probes and direct calls.
- `src/overlays/ovl_11/ovl_11_func_800F4FC8.c` and
  `build/ovl_11/functions/ovl_11_func_800F4FC8.s`: adjacent completion leaf.
- `src/overlays/ovl_11/ovl_11_func_800F4CEC.c` and
  `build/ovl_11/functions/ovl_11_func_800F4CEC.s`: current-record transition.
- `src/overlays/ovl_11/ovl_11_func_800F4994.c` and
  `src/overlays/ovl_11/ovl_11_func_800F4A58.c`: earlier pool-entry pair.
- `configs/splat/ovl_11.yaml`, entries at offsets `0x3CB74` through
  `0x3D1FC`: corroborating link order, not recovered TU boundaries.

## New grouping evidence

`800F4E84` (0x144 bytes) ends exactly at `800F4FC8`. Its call at
`800F4F28` therefore targets its immediate link successor. These are not
merely adjacent consumers of a broad work area: both test bit `0x02000000`
of `D_8006C838+0x44F8` and read the pool pointer at `D_8006C838+0xDDD4`
(absolute `0x8007A60C`). Both original instruction streams form the latter
address with the base plus `0x8000`, then load at displacement `0x5DD4`.

The record relationship is complementary. The caller's first path compares
the signed tag at pool+0x18 and probes the position at pool+0x20: the second
0x18-byte record's tag and +8 position. A hit calls `800F4FC8`, which clears
the shared gate bit and bit 0 of that same second record's halfword at +4,
then zeroes the byte at `D_8006C838+0xE650`. It does not advance the stored
pool pointer. Thus direct call, gapless link adjacency, shared gate/slot,
and corresponding probe/completion operations support a medium-confidence
candidate same-TU pair. Previously the ledger tied `800F4FC8` to the earlier
pair only through the shared pool slot; the verified caller adds this
stronger local tie.

The second path probes the current record's tag/position at +0/+8 and calls
`800F4CEC` at `800F4FA4`. That nearby leaf reads the identical pool slot,
publishes the pointer to `D_80129630`, and writes the current record's
+0/+8/+C/+10 fields. This corroborates the two record transitions, without
requiring a broader TU-membership claim.

## Existing pair and limits

The earlier candidate pair `800F4994`/`800F4A58` shares `D_80129634`:
the bounds gate stores 3 and the flag timer tests/decrements it. Only the
8-byte `800F4A50` lies between them in link order. Both read the same pool
slot as the new pair, and `800F4994` compares the current record's signed
+0 tag against `D_8007AFF0+0x25476`, also used by `800F4E84`. These retain
the existing low-confidence bridge between the two pairs, not proof that
the intervening functions all belong to one TU.

The candidate-pair inference is conditional on this local gate/probe/clear
sequence sharing an original source file; a TU boundary could still lie
between caller and leaf. No shared static, TU-owned global, declaration-order
effect or register-variable quirk was established. The pool slot is not
exclusive to the ledger's members, and modern view typedefs do not prove
original type or TU identity. No original filename, source-file boundary,
or merger with the separately documented pool-walk or spawn-record writer
families is claimed. `800F4CEC` is supporting call evidence, not a new ledger
member in this pass.
