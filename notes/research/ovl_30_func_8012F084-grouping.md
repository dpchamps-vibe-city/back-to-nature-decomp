# ovl_30_func_8012F084: translation-unit grouping evidence

Documentation-only review of the verified source and supplied passing-finalize
handoff, identity
`09c14468dae6f65ee7b4f9e8f2d2123a331f3c0c957fb38e4ea2fdfa5bc83fd5`.
The reviewed `src/overlays/ovl_30/ovl_30_func_8012F084.c` has SHA-256
`b84ef7680998030fc930019d2b34e22adfdbae74e6b7b9236df42aa9f300c945`.
No solver work, build-input edits or new verification run were performed.

## New evidence: nested display helper and parent

`ovl_30_func_8012F030` and `ovl_30_func_8012F084` now have strong same-TU
evidence beyond their previously recorded adjacency/data-cluster tie:

- The original helper stores incoming `$v0` at `0x10($sp)` at 0x8012F040,
  without a subsequent read. Its parent supplies `$v0 = $sp + 0x28` on
  the state-0 and state-4 dispatch paths (0x8012F0C0 and 0x8012F0D8),
  then calls the helper at 0x8012F11C and 0x8012F2F0. The otherwise unused
  incoming value and caller frame address form a static-chain fingerprint.
- The verified source defines `copy_display` inside the parent's state-4
  block, with the symbol name `ovl_30_func_8012F030`. This nested definition
  reproduces both sides of that fingerprint and emits the helper immediately
  before the parent: 0x8012F030 + 0x54 = 0x8012F084; the parent occupies
  0x350 bytes through 0x8012F3D4 (exclusive).
- `src/overlays/ovl_30/ovl_30_func_8012F030.c` is now only an explanatory
  placeholder. The current link map gives it zero text bytes and assigns
  the full 0x3A4-byte helper/parent run to the `8012F084.c` object. This
  confirms the verified reconstruction's ownership, rather than providing
  independent evidence of the original file boundary.

Original instruction evidence:
`build/ovl_30/asm/nonmatchings/ovl_30_func_8012F030/ovl_30_func_8012F030.s`
and
`build/ovl_30/asm/nonmatchings/ovl_30_func_8012F084/ovl_30_func_8012F084.s`.
Emission/ownership evidence: `build/src/overlays/ovl_30/ovl_30_func_8012F084.s`,
`build/ovl_30/ovl_30.map` and the nested-definition comment in
`configs/splat/ovl_30.yaml`.

## Wider cluster: shared data, not a recovered boundary

The existing init/read pair remains a medium-confidence candidate:
`8012EFEC` clears `D_80134008` and `D_80134B0C` and ends at `8012F000`,
which reads and conditionally clears `D_80134B0C`. The verified parent
writes the same state word and tests `D_80134008` on exit. Its display
helper uses the rectangle at `D_8013400C`, immediately after `D_80134008`.
These ties connect the two pairs as a data family but do not establish
that the reset/read leaves shared the parent's TU.

The verified parent also supplies a direct shared-global bridge to the
existing later candidate: it compares `D_80134B14` with the setting at
`D_8007AFF0 + 0x2547C`, and copies the edited word to `D_8005E32C` when
different. `src/overlays/ovl_30/ovl_30_func_8012FDEC.c` edits that same word
with input masks 0x2000/0x8000: increment with 0x100 reset to zero, or
decrement with zero mapping to 0x100. Its immediate predecessor/caller `8012F454` initializes the
word from the same setting and prints it; the original assembly records
the initialization at 0x8012F634 and the call at 0x8012FA74 in
`build/ovl_30/asm/nonmatchings/ovl_30_func_8012F454/ovl_30_func_8012F454.s`.
`configs/splat/ovl_30.yaml` corroborates their gapless link order.

## Limits

The high-confidence helper/parent membership assumes the bilateral
static-chain fingerprint is compiler-generated nested-function machinery;
byte identity establishes a viable reconstruction, not unique original
source syntax. No original outer TU boundary is recovered. Shared overlay
state and nearby data addresses alone remain weak same-file evidence, so
membership of the init/read pair and `8012FDEC` in that larger TU stays
low-confidence. The helper/parent result does not absorb the intervening
mirror-call leaves or extend membership across all callback-table entries.
