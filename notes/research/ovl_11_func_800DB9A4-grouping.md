# ovl_11_func_800DB9A4: record-table initializer

## Verified handoff

`src/overlays/ovl_11/ovl_11_func_800DB9A4.c` passed the exact diff, full
configured build, scope and clean-source gates. Documentation handoff identity:
`4fd4dac3fb4fef87b73531a36805b5accc901c4f57ca4546c84693bded3a7f4c`.
The integrated scratch winner is `build/ovl_11_func_800DB9A4/clean.c`,
byte-exact at 67/67 words. No flag override or policy exception was added.

## Grouping evidence

The initializer shares a specific table and pointer with the existing
low-confidence 800DBAB0 record-family band:

- It clears 0x40 bytes beginning at D_800742AC, then writes -1 to the
  leading halfword of each of the following five 12-byte records. These
  addresses are D_8006C838+0x7A74 for the word and +0x7A78 for the table,
  the storage already documented for 800DBAB0, 800DBB94, 800DBD78,
  800DBE30, 800DBE9C and 800DBEF8.
- It also clears D_8006C838+0x30. Matched 800DBAB0 publishes the selected
  table pointer at that same offset; matched 800DBEF8 reads it.
- The original initializer occupies 0x10C bytes at 800DB9A4 and ends
  exactly at 800DBAB0. Shared table/pointer state corroborates this
  zero-gap link adjacency.

This supports extending the existing **low-confidence candidate band**
backwards to the initializer. It does not establish original TU ownership,
static linkage, or membership of distant users merely sharing D_8006C838.
The byte-buffer resets at +0xE640, +0xE6C0 and +0xE6C8 do not by themselves
merge this band with editors in other containers.

Evidence:
- `src/overlays/ovl_11/ovl_11_func_800DB9A4.c`
- `src/overlays/ovl_11/ovl_11_func_800DBAB0.c`
- `src/overlays/ovl_11/ovl_11_func_800DBEF8.c`
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800DB9A4/ovl_11_func_800DB9A4.s`
- `include/game_types.h`: `D8006C838RecordTableView`
- `include/globals_override.h`: D_800742AC evidence and declaration

## Matching mechanism, not grouping evidence

The preserved v77 draft, copied to
`build/ovl_11_func_800DB9A4/baseline.c`, had residual [0,0,1,0]. Its
D_8006C838 high-half formation followed the first loop's sentinel, count
and pointer initializations; the target places it before them. The
scheduler trace showed the candidate's late-born UID 242 following those
three plain-priority instructions in both scheduling passes.

The first loop's hand-transcribed descending pointer walk was the wrong
source origin. Replacing it with an ascending indexed source loop:

```c
for (i = 0; i < 5; i++) {
    *(s16 *)((u8 *)&D_800742AC + 4 + i * 0xC) = -1;
}
```

lets the compiler produce the target's descending walk and the correct
preheader order. `ascending-record-source.c` measured exact 67/67 words;
removing unused declarations and unrelated headers in `clean.c` preserved
that result. The intermediate `natural-first-loop.c` instead described
reverse-indexed source writes and compiled to an ascending walk, showing
why simply preserving the machine's traversal direction in C was not the
right premise.

The `memset` declaration comes unchanged from the vendored
`include/psyq/memory.h` (`void *memset()`); no local prototype was invented.
The experiment ledger is `build/experimentLedger/ovl_11_func_800DB9A4.jsonl`.
The loop-reversal finding is a compiler-mechanism result, not additional
translation-unit evidence.
