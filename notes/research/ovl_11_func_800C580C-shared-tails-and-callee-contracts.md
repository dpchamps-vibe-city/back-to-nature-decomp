# 800C580C: shared tails and callee contracts

`ovl_11_func_800C580C` now has a clean C implementation under the production
flags, with no register bindings, assembly, or flag override. The relocated
oracle reports **985/985 words, MATCH**. `make -j8 check-all` verifies the PS-X
EXE and all 13 configured overlay containers.

The experiment sources, pass dumps and measurements are under
`build/ovl_11_func_800C580C/completion/`. The terminal scratch source is
`integration-final.c`; `power-column-birth.c` is the untrimmed exact-code draft.
The four initially undetermined words were jump-table relocations, not matches.
After building the live object, `deriveRodataSplits.ts --container ovl_11
--write` attributed `[0x1B8, 0x3E4)` to this function. `make split-ovl_11`
refreshed the linker input. The existing tracked `rtir` alias was restored
unchanged after the splitter removed it.

## What transferred from 80104EC8

The useful lesson was **local return joins and independent declaration
checks**, not its compiler-flag override. Flag probes on the actual candidate
provided no justification for an override here. Probes of an unchanged
`INCLUDE_ASM` stub are not measurements of the proposed C.

The preserved draft ended with the zero return. Placing the script-null/end
return of one last, with the `waitForAction` zero return earlier, changed which
tails GCC could share. `jump.c:find_cross_jump` first compares the instructions
before the destination label, then other jumps to that label. An epilogue's
zero assignment can therefore absorb a return before the preceding call is
considered. Natural complete calls then shared at the target's late anchors:
`CD3C4`/`D622C`, `CD0F4`, `C7270`, and the three `CB540` request calls.

A scoped zero-return label after a call is not interchangeable with a global
return label. `flow.c:count_basic_blocks` inserts a zero-width `USE(0)` after a
call immediately preceding a label. This participates in later cross-jump
comparison. The retained `reset_done`, `state_done`, and `tool_done` joins have
real incoming paths and preserve the target's separate calls. They are a
verified source construction, not a claim that the original used these labels.
The negative first-switch action uses `break`; the positive actions return.
The earlier bounded return/break searches had a different final-return and
call-sharing premise and did not exclude this construction.

Other population/control-flow corrections:

- Every remaining arm of the second switch's large item group calls `CB4E0`,
  not only state `0x5C`. The target branch at `800C6118` goes to that call.
- Duplicating the complete item-update tail in states `0x33` and `0x34`
  recovers its late shared anchor.
- The state-`0xC` buffer pointer is born inside the successful `CE53C` arm.
  Keeping the subsequent pointer join reproduces the separate argument moves.
- Conditional `applyCost` assignments belong in the success/failure arms, not
  before the predicate call. The latter makes the comparison reuse `$s2`.

## Declaration evidence and corrections

### CD08C returns no value to its callers

Its old matched definition returned the value of its final `CB4E0` call.
That final call also permits a byte-identical **void** definition, so the old
source alone did not establish an original return type. The active callers
`CCCC0`, `8011FFA8`, and `C580C` discard its result.

Changing the candidate's declaration to void corrected the first-switch
`0x4C` and `0x49` allocations and removed an unintended AND/store tail merge.
The staged residual changed from `[1,353,1,49]` (degraded positional block
correspondence) to `[0,0,2,11]`. This was a declaration-dependent allocation
residual, not hundreds of missing operations. The callee and both other active
callers remain individually byte-exact with the corrected declaration.
The disabled `ED4A8` draft still has its old declaration and needs the published
context when it is resumed.

### C70CC accepts a word-sized state argument

At `800C6738`, the original loads the script state with signed `lh` and passes
it without a `0xFFFF` mask. The old `u16` prototype manufactured that mask.
The callee stores this argument as a halfword; its definition remains exact
with an `s32` parameter. The definition and generated context now publish that
contract. Other callers' old local `u16` declarations were not batch-rewritten.

### F43CC takes a by-value VECTOR

The caller passes four contiguous vector words, followed by the collision ID
and selector. Other matched callers (`CC7CC`, `8011C298`) already use this
aggregate contract. Replacing the callee's four artificial four-byte aggregate
parameters with `VECTOR position`, and reading `vx/vy/vz/pad`, remains
byte-identical. The corrected definition and generated context are included.
The callee audit currently leaves VECTOR's ABI slot layout undetermined; this
is a tool limitation, not an unresolved byte comparison.

Two audit cautions remain explicit rather than prompting unsupported edits:

- `D622C` reads three parameters, but all eleven calls in this target supply a
  fourth zero argument. Those stores/moves are retained; unread does not prove
  an argument was absent from the original interface.
- The audit's `func_8001FABC` void inference is not authoritative: the matched
  definition returns `func_800212A8(...)`. Changing it to void was measured and
  changed this caller's allocation. It was not retained. A return carried
  through another call is not disproved by absence of a direct `$v0` write.

## The last three source mechanisms

After population and control flow agreed:

1. A word-sized `status` temporary is reused for the first negative action's
   cleared flags and the second switch selector. This reproduces the target's
   `$v0` mask / `$v1` flags allocation without perturbing the item-count update.
2. State `0x56` sets `0x1800`, updates the attributes, then clears `0x40000000`.
   Keeping those statements interleaved, rather than combining the flag updates
   before the attributes, fixes the count/flags register rotation. Adjacent
   separate flag updates were compiler-identical to the combined expression.
3. The signed power/20 column is computed into a local before the table lookup.
   The scheduler trace showed the inline expression's table address born before
   both quotient calculations. The named column moves that birth after the
   column calculation, reproducing the target's `$a2` base and ordering. The
   staged key becomes `[0,0,0,0]`; relocation attribution then establishes the
   complete byte match.

No new original translation-unit grouping evidence was established beyond the
existing local grid-action and shared-record cluster entries, so
`notes/file-groupings.md` was not changed.
