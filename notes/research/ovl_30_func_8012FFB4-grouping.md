# ovl_30_func_8012FFB4: translation-unit grouping evidence

Documentation-only review of the verified source and supplied passing-finalize
handoff, source/evidence identity
`6b9fb7c86858c13efcc4f3324f740162d76fc3bb42646ec89329c37f512956b0`.
The reviewed `src/overlays/ovl_30/ovl_30_func_8012FFB4.c` has SHA-256
`64d3cb950f6dd1c0e23a3ee62d65fe460ac20c7b48eb2d32a5433dbaef0c5984`,
also recorded by the exact entry in
`build/experimentLedger/ovl_30_func_8012FFB4.jsonl`.
No solver work, build-input edits or new verification run were performed.

## New evidence: extension of the adjacent value-menu pair

The verified callback supports extending the existing medium-confidence
`8012FEEC`/`8012FF50` candidate group to `8012FFB4`:

- **Code, table and caller order agree.** The two preceding callbacks occupy
  0x64 bytes each: `8012FEEC + 0x64 = 8012FF50`, then
  `8012FF50 + 0x64 = 8012FFB4`. Their 20-byte-record menu tables are likewise
  consecutive: `80134018 + 22*20 = 801341D0`, then
  `801341D0 + 23*20 = 8013439C`. The verified function passes this third
  table and 29 entries. The original `D_80133FC0` callback array lists
  `8012FEEC`, `8012FF50`, `8012FFB4` consecutively at +4, +8 and +0xC;
  `8012F084` invokes entries from that array. This corroborates the link
  adjacency with common-caller and data-layout evidence.
- **Shared callback protocol.** Like its two predecessors, `8012FFB4` gates
  on `func_80013394`, prints with `KanjiFntPrint`, transfers its incoming
  cursor through `D_80134014` around a call to `80133454`, and returns the
  cursor. Its additional command dispatch uses `D_80134B10`, which that
  helper sets and the lifecycle driver resets on a menu change.
- **The third table links to the action-parameter cluster.** Its entries
  point at `D_80134AFC`, `D_80134B00`, `D_80134B04` and `D_80134B08`,
  the adjacent parameter words consumed by the callback's actions. This
  ties the newly verified dispatcher to its menu data rather than relying
  on a similar-looking function body alone; it does not prove private
  static ownership.

Evidence paths: `configs/splat/ovl_30.yaml` (consecutive text splits),
`build/ovl_30/asm/data/6188.data.s` (callback array and menu/parameter tables),
`build/ovl_30/asm/nonmatchings/ovl_30_func_8012FFB4/ovl_30_func_8012FFB4.s`
(original helper call at 0x8013000C and command-state accesses), and the
reviewed C sources for `8012FEEC`, `8012FF50`, `8012FFB4`, `8012F084` and
`80133454` under `src/overlays/ovl_30/`.
The earlier [pair evidence](ovl_30_func_8012FEEC-grouping.md) did not yet
establish membership of this third callback.

## Limits

The same-TU hypothesis assumes the aligned code/table/callback order reflects
source-local menu organization; separate files using the same menu API remain
possible. No private-static ownership, declaration-order fingerprint or
TU-specific register quirk was established. Medium confidence is retained;
neither outer TU boundary is recovered. Shared cursor/command state and helper
calls do not justify merging this run with the distant `80133454` helper group
or the earlier lifecycle cluster, nor absorbing other callback-array members.
The new function's switch-rodata split is a reconstruction detail, not
independent evidence of an original file boundary.
