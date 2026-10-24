# Asm and register-pin review

Snapshot of 2026-10-23, at `6cfec884`. It covers every function that has a
`register-asm` or `embedded-asm` entry in `.pi/autoloop.json`
(`sourcePolicy.allowlist`): 37 functions.

Terms:

- **Pin:** a variable fixed to one register with `register x asm("$n")`.
- **Asm instruction:** an asm statement that emits machine code, so those
  instructions come from no C at all.
- **Empty asm:** an asm statement that emits nothing. It only stops the
  optimizer from moving or merging values.

## Method

Each file was recompiled in scratch with `tools/agent/diffFunc.ts --src`, which
compares the function byte-for-byte with the original, relocations included.
Up to three variants were built per file:

- with its pins removed;
- with its empty asm removed;
- with both removed.

Where the asm only forms an address or performs simple arithmetic, a fourth
variant replaced it with the plain C it stands for. The test columns give
`matching words / total words`, or MATCH.

`src/` was not touched. The variants are in
`/tmp/claude-1000/-home-dave-Documents-code-btn-decompilation/358c474a-25bd-4785-bef8-c019c70501b8/scratchpad/r3/`
(`v/` holds the automatic variants and `m/` the hand-written ones).

## 1. Stale or removable (5)

A variant without the construct is byte-exact, so the construct does nothing
the compiler would not do anyway. Each change still needs the full gate
(`make check-all` and the source policy) when it is applied.

| Function | What goes | Evidence |
|---|---|---|
| func_80021820 | The allowlist entry | The source has had no asm since `383a33b6`. |
| func_8001FEA4 | The `one` → `$s0` pin, then the allowlist entry | Without the pin: MATCH 51/51. |
| func_8001DFD4 | The local `#define gte_rtps`. Include `psyq/inline_c_gas.h` instead, as func_8001BBD8 and func_8001C37C do. Then the allowlist entry | MATCH 30/30. The GTE command itself is asm, but the shared header is the approved place for it, so this file needs no exception. |
| func_8002437C | The file-scope `__asm__` block that defines `_800243A4`, `_800243C0` and `_800243C8`, then the allowlist entry | MATCH 21/21. No source, asm or data file references the three labels. They reach the overlays only through `build/engine_syms.txt`, and no overlay uses them. `make check-all` settles the link. |
| ovl_11_func_800F227C | All four asm statements and the `en` pin. Use `en = (…)((u8 *)rec + 0xE4)` and `pN = (…)((u8 *)bN + (u32)rec->unk22 * 8)`. Then the allowlist entry | Plain C: MATCH 54/54. |

## 2. Highly suspect (27)

Removing any of these constructs breaks the match. But nothing shows that
plain C cannot reach the target, and most of the files say so in their own
comments ("not evidence of original source", "tracked debt"). Each one stands
in for a source shape that was never found.

### 2a. Register-level transcriptions (5)

Large parts of these functions are written as pinned registers or literal
instruction blocks. They are the furthest from decompiled C.

| Function | Contents | No pins | No empty asm | Notes |
|---|---|---|---|---|
| func_8001231C | 18 pins, 8 asm instructions, 3 empty asm | 40/160 | 157/159 | Near-clone of func_80012598, with the same constructs. |
| func_80012598 | 18 pins, 4 asm instructions, 3 empty asm | 96/209 | 207/209 | The file says "the original was certainly plain C". It names the cause as reload rematerialisation (`notes/research/func_80012598.md` §3.1). |
| func_80014CBC | 5 asm instructions, 8 empty asm | n/a | 84/117 | An asm entry group whose dummy operands steer the scheduler. |
| func_80016280 | 11 pins; a 35-instruction asm block with a hand-encoded branch | 160/214 | 200/214 | The whole entry block is hand-written. |
| func_80019070 | 16 pins, 5 asm blocks, 5 empty asm | 53/81 | 79/81 | The best clean C, 72/81, is kept under `#if 0` in the file. |

### 2b. Address-forming asm (6)

Here the asm writes ordinary address loads: `lui %hi` / `addiu %lo`, or the
`+0x8000` prefix. The project has matched both patterns in plain C elsewhere:
the shared HIGH that reload rebuilds, and the two-stage `+0x8000` base in
func_8001A284 and func_80021E60. A direct C replacement came close in every
case but did not match.

| Function | Contents | No pins | Plain C |
|---|---|---|---|
| ovl_11_func_800DDDBC | 2 pins, 3 asm instructions | **MATCH** | 14/17 |
| ovl_11_func_800EDF3C | 3 pins, `ori`/`addu` `+0x8000` prefix | 44/49 | 42/49 |
| ovl_11_func_800F8B4C | 5 pins, 3 asm instructions | 41/52 | not tried |
| ovl_11_func_8010476C | 3 pins, 2 asm instructions | **MATCH** | 33/36 |
| ovl_11_func_80112D10 | 2 pins, 3 asm instructions | **MATCH** | 31/39 |
| ovl_11_func_8011F574 | 2 pins, 1 asm instruction (`lui 0x8013`) | **MATCH** | 16/18 |

In four of these (800DDDBC, 8010476C, 80112D10 and 8011F574) the pins do
nothing and can be deleted now, even while the asm stays.

### 2c. Arithmetic or dead-write asm (2)

| Function | Contents | No pins | Notes |
|---|---|---|---|
| ovl_11_func_800E74AC | 6 pins; `sll`/`sra`, `subu` and `slt` as asm | 9/22 | |
| ovl_11_func_800BF450 | Asm that writes a zero result and a dead `ori $3,$0,0x8000` | n/a | Not tested. The asm is the only thing that produces the dead write. |

### 2d. Pins only (9)

| Function | Pins | No pins | Notes |
|---|---|---|---|
| func_80017F30 | 2 | 19/22 | The file says the pins are "not evidence of original register bindings". |
| func_8001E9F8 | 1 file-scope (`hidden` → `$v0`) | n/a | Not tested, because removing it changes what the file means. Its research note rules out the nested-function reading but leaves the cause open. |
| ovl_11_func_800E1770 | 1 | 35/37 | Same body as 8010BC54. |
| ovl_11_func_8010BC54 | 1 | 35/37 | Same body as 800E1770. |
| ovl_11_func_800F2880 | 3 | 23/39 | |
| ovl_11_func_800F3EF0 | 3 | 26/35 | |
| ovl_11_func_800FC8C8 | 1 | 46/52 | Cited as the precedent for ovl_19_func_800BB108, but has no write-up of its own. |
| ovl_11_func_801129EC | 3 | 28/38 | |
| ovl_11_func_8011D890 | 1 | 38/41 | |

### 2e. Barriers and ties (5)

| Function | Contents | No pins | No empty asm | Notes |
|---|---|---|---|---|
| func_80015644 | 1 empty asm | n/a | 47/48 | One instruction moves. |
| func_8001A284 | 1 empty asm | n/a | 187/188 | The file calls it "tracked debt". |
| func_8001E340 | 1 empty asm that keeps `i` alive | n/a | 13/19 | |
| func_8001D2D8 | 1 pin, 1 empty asm that ties it | 26/28 | 26/28 | The best documented here: it traces the cause to combine and local-alloc's register suggestion. It argues that no second reference to the argument is possible, but no search for one is recorded. |
| func_80022794 | 1 pin, 1 empty asm, plus `-fno-rerun-cse-after-loop` | 113/116 | 103/116 | Three devices stacked; see `flag-review.md`. |

## 3. Legitimately warranted (5)

| Function | Construct | Why it stays |
|---|---|---|
| func_80015704 | `CAPTURE_RA` | The original almost certainly had this asm. Two sites share the fingerprint `addu $8,<addr>,$0; sw $31,0($8)`, with different compiler-made addresses feeding it, so it is a shared macro. C cannot store `$ra` this way. See `notes/research/caller-capture-debug-hook.md`. |
| func_80016054 | `CAPTURE_RA` | As above. |
| func_800142D8 | 3-instruction branch in asm | The one residual is cc1's delay-slot choice on one branch, which C cannot name. All 1,057,675 variable assignments over the statement skeleton were enumerated, and none reproduce it. Register allocation stays with the compiler. See `notes/retros/2026-08-09-func_800142D8-retro.md`. It has no entry in `notes/asm-exception-evidence.md`. |
| ovl_19_func_800BB108 | 1 pin (`column` → `$16`) | Written up in `notes/asm-exception-evidence.md`: a constant write that sched.c floats above a call. About 460 clean variants failed, and so did 308 loop-note placements. ovl_11_func_800F7230 later matched the same held-write problem with one loop-note barrier, so it may be worth one more attempt. |
| ovl_19_func_800B95D4 | 1 pin (`opponent` → `$19`) | Written up in `notes/asm-exception-evidence.md`: global-alloc priority (`global.c:593`), with 112 measurements recorded. It is the smallest exception found. |

The two `CAPTURE_RA` functions are warranted because the original source had
asm there too. The other three are warranted as toolchain edge cases under the
project's policy:

- the exception is a single, minimal construct;
- its cause is traced to a named compiler decision;
- the clean-C search is recorded.

## Not listed

The source policy allows these without an allowlist entry:

- Empty memory barriers (`__asm__ volatile("" ::: "memory")`) in 7 files.
- `CAPTURE_PREV_RET` in 4 files.

They also fall outside the 100% count:

- 50 dead EXE functions still in `INCLUDE_ASM`.
- `__start` (func_80011278), which is classified pure asm.
