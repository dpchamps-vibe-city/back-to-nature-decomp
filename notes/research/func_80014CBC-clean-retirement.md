# func_80014CBC — clean-C exception retirement

The sector reader now matches **117/117 words**, staged residual `[0,0,0,0]`,
without inline asm, register bindings, volatile parameters, aggregate parameter
carriers, hand-addressed home slots or a cast on the recursive call. Its
`embedded-asm` allowlist entry has been removed. This retires a function-specific
hybrid, rather than extracting another asm helper.

## Two declaration premises, not an instruction C cannot express

1. **The caller manufactured an argument to `func_80021B20`.** The callee reads
   no incoming arguments. Matched callers `func_8002194C` and `func_80021B90`
   already declared/called it with none. The earlier audit's no-argument branch
   was the right starting point; an incoming value left in `$a0` at a call is
   not proof that the callee consumes it. The definition is now consistently
   `void func_80021B20(void)`, still exact at 17/17. The bytes do not prove an
   original upper arity bound; this is a consistent, verified reconstruction.
2. **The scratch draft forced argument 4 to be volatile.** Repeated target
   reads of its incoming slot had been treated as a source storage requirement.
   But an ordinary scalar can acquire that residence through allocation/reload.
   Removing `volatile` supplies the allocator with a pseudo which subsequently
   spills. The final loads are unchanged while the earlier allocation state
   changes enough to select the correct registers for other variables.

All SDK prototypes and production flags were retained. In particular, the
callee-truth warning about `CdSync`'s return type is immaterial here because
this function discards the result; the vendored declaration was not changed.

## Measured chain

Sources are preserved under `build/repair14cbc-clean/`, with measurements in
`build/experimentLedger/func_80014CBC.jsonl`.

| Source | Staged residual | Meaning |
| --- | --- | --- |
| `build/asm-exception-audit/14cbc-no-debug-arg.c` | `[0,0,1,2]` | No asm, but volatile arg4 and an identical-arm retry test retained |
| `retry-direct.c` | `[0,0,2,2]` | Removed the artificial retry test; retry store order fixed, entry register order changed |
| `completion-shared-clears.c` | `[0,0,2,2]` | Also removed unused `pos = 1`, a redundant buffer assignment, and duplicated completion clears; same compiled words as `retry-direct.c` |
| `scalar-parameters.c` | `[0,0,0,0]` | Only changed arg4 from `volatile s32` to `s32` relative to the preceding row |
| `final.c` / integrated source | `[0,0,0,0]` | Meaningful names, sector-size/return-behavior comments and braces; same exact words |

No redundant conditional, empty loop or reference pump from the old repair
survives. The real start/poll, retry and completion branches remain. The
constant `one` is established on the busy path and reused after `CdReadSync`,
as required by the target's call-crossing value.

## Why the final qualifier change matters

The instrumented compiler agrees with the production output. Compare:

- `build/localAllocationOracle/func_80014CBC/variants/retry-direct-de7bf8f78e84b120/`
- `build/localAllocationOracle/func_80014CBC/variants/func_80014CBC-3b6a30bded9db796/`

The `.lreg` statistics and `.greg` allocation order show this is **global
allocation**, not a local quantity tie. `gcc/global.c:allocno_compare` ranks
`floor_log2(refs) * refs / live_length * 10000` (integer-truncated here), then
breaks ties by allocno number.

| Value | Forced-volatile draft: refs / life / priority | Plain-scalar final: refs / life / priority |
| --- | --- | --- |
| File index | 4 / 64 / 1250 | 4 / 66 / 1212 |
| Restart flag | 5 / 80 / 1250 | 5 / 80 / 1250 |
| Constant one | 3 / 22 / 1363 | 3 / 20 / 1500 |

In the volatile draft, the file index wins the tie and takes `$s4`, leaving
`$s5` for the restart flag. In the final source, one > restart > file index
produces `$s3`, `$s4`, `$s5` as required. The final `.greg` dispositions have
r96 in 19, r86 in 20, r81 in 21; the compact flag (r85) and file offset (r82)
have no hard-register disposition. Reload supplies the target's stack-resident
reads and the incoming-offset home store naturally. The local oracle replays
10/10 quantity decisions for the final source; it does not itself establish
the global ranking, which is read from the dumps above.

This explains why preserving the final stack accesses with a volatile or
BLKmode declaration was not equivalent to letting a scalar spill. Those source
choices erased compiler state that matters before the final instructions exist.
It also supersedes the old hybrid's claimed need for an alias-opaque entry
store, reference-count carriers and a hand-addressed recursive argument load.

## Integration and verification

- `src/func_80014CBC.c`: clean scalar-parameter C, exact 117/117.
- `src/func_80021B20.c`: removed its unused formal parameter, exact 17/17.
- Context exported through the generator for both functions; the obsolete
  generated `ReadFlag` type was removed by that generator, not hand-edited.
- Both source-policy scans pass; the sector reader passes without an exemption.
- `make check` passes after source and context publication.
- No flags, new helpers, new exemptions, assembly stubs or disabled bodies.

Integrated source SHA-256:
`39a912d89343dc32686b36f811a42c1e09161d0c772131ab56554cfe5cae9152`.

No new original-TU membership is established by this repair. The existing
CD-state cluster evidence is unchanged. Earlier allocation/parameter-residence
notes describe historical premises, not remaining barriers to clean C.
