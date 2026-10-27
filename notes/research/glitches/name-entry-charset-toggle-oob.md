# Name-entry charset-toggle OOB read ("kana-table" read)

Status: static analysis only. Every claim below is traceable to the original
SLUS-01115 EXE bytes, the matched (byte-exact) reconstruction, or the
project's symbol files; no emulator or console execution was performed.
Origin: follow-up to `notes/research/speedrun-calendar-glitch-audit.md`
section 9 (abbreviated there); this file is the full record. No game source,
configuration or headers were changed.

## Summary

The name-entry character palette has an out-of-bounds read that a player
triggers with ordinary controller input: a same-frame **L1+L2** or **R1+R2**
press toggles the character set without moving or revalidating the cursor,
so a cursor cell that is legal in the wide hiragana grid but past the end of
the katakana code table is appended as an OOB halfword. The harvested
halfword comes from EXE `.data`/`.bss` immediately after the 28-entry
`gTextEntryCodeTableSet1` — including **`_spu_tsa`, libspu's live 16-bit SPU
transfer address**, which varies with the game's per-area sound-bank loads.
That halfword becomes a *character of the player's name*.

Downstream, the message-window renderer treats any name halfword with bit
0x4000 set as a string-reference control code: it indexes
`D_8005F0C8[code & 0xFFF]` (a table whose real extent is 124 words, of which
only slots 0–10 are ever written) or resolves ids 10–16 to the name buffers,
and **recurses into the result with no depth limit**. A name character whose
id is 0x00A resolves to the player's own name — unbounded recursion, a
stack overflow whose smash path crosses the event-program scratch, the
calendar, and the scene dispatch globals.

This is the first input-driven memory-safety chain found in this game.

## 1. The OOB read (original instructions)

`GetTextEntryCodeSet1` (original 800245C8, symbol `configs/symbols/exe.txt`
line 809; matched C in `src/modules/text_entry.c`) computes the code for
character-set 1 as `gTextEntryCodeTableSet1[row * 10 + column]`:

```
800245c8: 3c028005   lui   r2, 0x8005
800245cc: 244259d4   addiu r2, r2, 0x59d4   ; r2 = 0x800559D4 = gTextEntryCodeTableSet1
800245d0: 00051880   sll   r3, r5, 2        ; row * 4
800245d4: 00651821   addu  r3, r3, r5       ; row * 5
800245d8: 00031840   sll   r3, r3, 1        ; row * 10
800245dc: 00641821   addu  r3, r3, r4       ; row * 10 + column
800245e0: 00031840   sll   r3, r3, 1        ; * 2 (byte offset)
800245e4: 00621821   addu  r3, r3, r2       ; + table base
800245e8: 94620000   lhu   r2, 0x0(r3)      ; codes[index] — no bounds check
800245ec: 03e00008   jr    ra
```

The cursor grid is 14 columns wide (`GetTextEntryCode`, original 800244FC:
`quot = cursor / 14`, `rem = cursor % 14`, then
`gTextEntryCodeHandlers[set](rem, quot, cursor)` — handlers at
`gTextEntryCodeHandlers` 0x800559CC = {0x80024578, 0x800245C8}).

**The table is 28 entries**: `gTextEntryCodeTableSet1 = 0x800559D4` with
size 0x38 (`configs/symbols/exe.txt` line 825); the next symbol is
`math_errno = 0x80055A14`, and the 8-byte gap 0x80055A0C..0x80055A14
bounds it exactly. Rows 3+ on the 14-wide grid index up to 63 — past the
end.

Movement alone cannot reach those cells in set 1 — the set-1 bounds arrays
(`gTextEntryColumnBounds[1]` at 0x8005E36C = {13,13,7,0,0,13,...} and
`gTextEntryRowBounds[1]` at 0x800559AC = {2,2,2,2,2,2,2,2,1,1,0,0,0,5},
dumped from the EXE) confine the cursor to rows 0–2. **The charset toggle
is what breaks the invariant** (section 2). The set-0 bounds
(`gTextEntryColumnBounds[0]` at 0x8005E364 = {13,13,12,10,9,13};
`gTextEntryRowBounds[0]` at 0x8005599C = {5,4,4,4,4,5,4,4,4,4,3,2,2,5})
permit rows 3–5 on columns 0–9, which is where the OOB indices live.

Note on column 13: `AdjustTextEntryRow` (original 800243D0, size 0x38,
disassembled) remaps column-13 rows 2–4 to row 1 (moving up) or row 5
(moving down) — those cells are transient, so the OOB positions below are
reached through the ordinary grid cells, not column 13.

## 2. The trigger (original instructions)

`ProcessTextEntryInput` (original 80023DBC, symbol line 791; matched C in
`src/modules/text_entry.c` ~lines 457–530) switches on the newly-pressed
button word. Buttons 0x05 and 0x0A jump **directly to the character-set
flip, with no cursor check** — the same code the cursor-0x0D cell reaches
through its `else` arm:

```
80023e00: 1083001d   beq  r4, r3, -> 80023e78   ; case 0x20 (CIRCLE)
80023e08: 2c820021   sltiu r2, r4, 33           ; pressed < 33?
80023e10: 24020005   addiu r2, r0, 5
80023e14: 1082006b   beq  r4, r2, -> 80023fc4   ; case 0x05 → flip, no cursor check
80023e18: 2402000a   addiu r2, r0, 10
80023e1c: 10820069   beq  r4, r2, -> 80023fc4   ; case 0x0A → flip, no cursor check
...
80023ed4: 2402000d   addiu r2, r0, 13           ; the 0x0D cell check
80023ee0: 10620038   beq  r3, r2, -> 80023fc4   ; cursor == 0x0D → same flip code
80023ee4: 24020053   addiu r2, r0, 83           ; the 0x53 (END) cell check
```

```
80023fc4: 978200e4   lhu  r2, 0xe4(r28)         ; gTextEntryCharacterSet (0x8005E358)
80023fcc: 24420001   addiu r2, r2, 1
80023fd0: 30420001   andi r2, r2, 1
80023fd4: a78200e4   sh   r2, 0xe4(r28)         ; set ^= 1 — cursor untouched
80023fd8: 08009003   j    8002400c              ; exit
80023fdc: 24110005   addiu r17, r0, 5           ; sound effect id (delay slot)
```

The flip code is the lexical continuation of `case 0x40`'s `else` arm in the
C switch (a Duff's-device-style fall-through; the matched source retains
it). The cursor is neither reset nor revalidated, so a position that is
legal in the wide set-0 grid is carried into set 1 where its index is out
of bounds.

### Button mapping (derived and cross-checked)

`ReadPadRawButtons` (`src/modules/pad.c` 311–323) returns
`(ReadPadDirectionByte(port) << 8) | gPadReadBuffers[port][3]`, so the
game's 16-bit word maps the standard pad bytes as:

| bit | button | evidence |
|---|---|---|
| 0x0001 | L2 | byte 3 bit 0 |
| 0x0002 | R2 | byte 3 bit 1 |
| 0x0004 | L1 | byte 3 bit 2 |
| 0x0008 | R1 | byte 3 bit 3 |
| 0x0010 | TRIANGLE | byte 3 bit 4 |
| 0x0020 | CIRCLE | byte 3 bit 5 — backspace in text entry (80023E00 case) |
| 0x0040 | CROSS | byte 3 bit 6 — append (case 0x40) |
| 0x0080 | SQUARE | byte 3 bit 7 |
| 0x0100 | SELECT | byte 2 bit 0 << 8 |
| 0x0800 | START | byte 2 bit 3 << 8 — confirm (case 0x800); "Start is reported" comment, pad.c 199–204 |
| 0x1000..0x8000 | UP/RIGHT/DOWN/LEFT | byte 2 bits 4–7 << 8 — matches every movement check |

Hence **0x05 = L1+L2**, **0x0A = R1+R2** — both bits must be newly pressed
in the same frame (the switch matches the exact pressed-word value). A
mistimed chord (0x4 or 0x2 alone) falls to the default branch and is
harmless. Input registers only once the prompt text is fully shown
(`IsMessageWindowStatusFinal()` gate).

### Step-by-step (harvesting `_spu_tsa`)

From the cursor's reset position (top-left, cursor 0):

1. Wait for the naming prompt text to finish.
2. D-pad: **DOWN ×4, RIGHT ×8** — row 4, column 8, cursor value 0x40.
   (Both cells (4,0)–(4,8) and (4,0) are inside the set-0 bounds: row 4's
   column bound is 9; column 8's row bound is 4.)
3. Press **L1+L2** (or R1+R2) once, clean same-frame press. The palette
   visibly flips to the second set; the cursor stays at 0x40.
4. Press **X** — appends `gTextEntryCodeTableSet1[4*10+8] =
   gTextEntryCodeTableSet1[48]`, the halfword at 0x80055A34 =
   **`_spu_tsa & 0xFFFF`**. The preview shows a junk or blank glyph.
5. X up to 7 more times appends the same value again (set and cursor both
   stay). CIRCLE backspaces (restores the 0xFFFF terminator).
6. Confirm with **START** (the on-screen END cell at length 8 additionally
   performs the known +1 write — stamina/affection/padding, see
   `speedrun-calendar-glitch-audit.md` sections 1 and 6).

For the static value 0x7350 use cursor 0x24 instead: **DOWN ×2, RIGHT ×8**
(row 2, column 8; row 2's column bound is 12, column 8's row bound is 4).
Any cell whose `row*10+col ≥ 28` works while carried from set 0.

### Where it works

Every naming screen uses the shared `ProcessTextEntryInput`:

- new game: player `BeginRequiredTextEntry(&gPrimaryObjectBase)`
  (`src/overlays/ovl_27/modules/menu_scene.c:159`), farm
  `BeginTextEntryAlternateLayout(&D_80070D18)` (line 204), dog
  `BeginTextEntry(&D_800759E8)` (line 229);
- gameplay: horse/animal rename scratch `D_801295D0`
  (`src/overlays/ovl_11/ovl_11_func_800E729C.c`,
  `ovl_11_func_800F0474.c`), animal-purchase naming `D_8012D638`
  (`ovl_11_func_80114604.c:184`), quantity-purchase name
  `gOvl11QuantityPurchaseNameText`
  (`src/overlays/ovl_11/modules/quantity_purchase_interaction.c:178`).

## 3. What gets harvested

The OOB indices 28–51 read EXE `.data`/`.bss` at 0x80055A0C..0x80055A54
(values dumped from `extracted/iso/slus_011.15`, file offset 0x4620C;
symbol identities from `configs/symbols/exe.txt`):

| index | cursor (D-pad route) | address | symbol | value |
|---|---|---|---|---|
| 28 | 0x24 (down 2, right 8) | 0x80055A0C | unnamed gap | **0x7350 — bit 0x4000 already set** |
| 29 | 0x25 | 0x80055A0E | gap | 0x000B |
| 30 | 0x26, 0x2A | 0x80055A10 | gap | 0x0000 |
| 31 | 0x27, 0x2B | 0x80055A12 | gap | 0x0047 |
| 32–33 | 0x28/0x2C, 0x2D | 0x80055A14/16 | math_errno | runtime (0 initially) |
| 34–35 | 0x2E, 0x2F | 0x80055A18/1A | math_err_point | runtime pointer (0 initially) |
| 36–37 | 0x30, 0x31 | 0x80055A1C/1E | _spu_RXX | 0x1F801C00 (low 0x1C00, high 0x1F80) |
| 38–47 | 0x32–0x36, 0x38–0x3F | 0x80055A20–32 | unnamed libspu pointers | 0x1F8010C0/0x1F8010C4/0x1F8010C8, 0x1F8010F0, 0x1F801014 |
| **48** | **0x40 (down 4, right 8)** | **0x80055A34** | **_spu_tsa** | **live: the 16-bit SPU transfer address** |
| 49 | 0x41 | 0x80055A36 | _spu_tsa high | 0 |
| 50–51 | 0x46, 0x47 | 0x80055A38/3A | _spu_transMode / _spu_addrMode | 0–2 |

`_spu_tsa` is written at runtime by (constant-address scan of the EXE,
gp-relative included):

- `SpuSetTransferStartAddr` (0x800329B0, `sh`),
- `SpuWritePartly` (0x80032A74, `sh`),
- libsnd's `_spu_t` (0x80026824, `sh`),
- `_spu_init` (0x8002624C/0x80026370, initial values 0 / 0x200).

The game's sound driver loads per-area banks
(`src/modules/sound_driver.c`: `StepSoundBankBodyTransfer` →
`SsVabTransBodyPartly`, ~line 691; the SPU load address starts at 0x1010,
line 129, and advances per bank). So **the last transferred SPU address is
player-influenced: stand in a different area before the gameplay naming
screens to harvest a different value.** The set of reachable values is the
set of bank transfer addresses; watch 0x80055A34 while entering areas to
enumerate them.

The static 0x7350 needs no runtime state and works on the first naming
screen deterministically; its reference id is 0x350.

## 4. The sink: name halfwords are reference control codes

The message-window renderer substitutes names into dialogue text — the
fixed EXE text region 0x80051000..0x8005E000 contains 0x400A (player name)
once, 0x400C (dog) once, 0x400D (farm) once, and slot references 0x4000
(25 uses) and 0x4001–0x4006 (scanned from `extracted/iso/slus_011.15`).

Every halfword with bit 0x4000 set is resolved in the draw path
`func_80018B98` (original 80018B98; matched C
`src/func_80018B98.c:195–207`) — original instructions:

```
80018ea8: 96040000   lhu  r4, 0x0(r16)        ; the code from the string
80018eac: 0c0068a1   jal  0x8001a284          ; func_8001A284(code)
80018eb4: 1440000b   bne  r2, r0, -> 80018ee4 ; resolved? skip the fallback
80018ebc: 3c038006   lui  r3, 0x8006
80018ec0: 96020000   lhu  r2, 0x0(r16)
80018ec4: 2463f0c8   addiu r3, r3, -3896      ; r3 = 0x8005F0C8 = D_8005F0C8
80018ec8: 30420fff   andi r2, r2, 0xFFF       ; code & 0xFFF
80018ecc: 00021080   sll  r2, r2, 2
80018ed0: 00431021   addu r2, r2, r3
80018ed4: 8c420000   lw   r2, 0x0(r2)         ; D_8005F0C8[id] — no bounds check
80018edc: 1040003b   beq  r2, r0, -> 80018fcc ; 0 → skip
...
80018f0c: 0c0062e6   jal  0x80018b98          ; RECURSION — no depth limit
```

- `func_8001A284` (`src/func_8001A284.c`) resolves ids 10–16 (arg & 0xFFF):
  case 10 → `D_8006C838+0x51C8` = **0x80071A00, the player's own name**;
  case 11 → D_80075AD8 (horse); 12 → D_800759E8 (dog); 13 → D_80070D18
  (farm); 14 → D_8007AFBC.
- The fallback table `D_8005F0C8` has a **real extent of 124 words**
  (0x8005F0C8..0x8005F2B8; the next symbol is `gAuxSpriteSource` at
  0x8005F2B8 — the oversized `[4096]` declaration in
  `include/globals_override.h:2575` is a neutral view, not the real size).
  Only **slots 0–10 are ever written**: `func_80017A08`
  (`src/func_80017A08.c`) clamps ids ≥ 0xB to 0xA. Its callers set live
  string pointers (save-menu slots, `src/overlays/ovl_15/modules/
  save_menu_ui.c:106–159`).
- Ids **0x07C..0xFFF read live `.bss` as pointers**: the aux-sprite
  work area 0x8005F2E8..0x800605E8 (memset 0x1300 and written each frame
  by `PresentAuxSpriteDisplay`, `src/modules/aux_sprite_display.c`), the
  pad input state at 0x80060000+ (SetPadRepeatTiming etc.), the sprite
  packet pool cursor at 0x80060118, `gAuxSpriteHeader` at 0x800605F0.
- The same fallback appears in three more consumers: `func_8001929C`
  (`src/func_8001929C.c:32`, counter with a budget), `func_8001945C`
  (`src/func_8001945C.c:43`), and `func_800198E0`
  (`src/func_800198E0.c:18`, the width measurer — recurses for
  `(code & 0xF000) == 0x4000`).
- **No depth limit** in the draw recursion: the recursive call passes
  arg8=1, which only skips the typewriter pacing block
  (`func_80018B98.c:41–66`); the reference block at the bottom of the
  loop is unconditional.

## 5. Downstream outcomes by harvested id

For a name character with bit 0x4000 (harvested value in [0x4000, 0x7FFF];
also [0xC000, 0xFFFF] for the `& 0x4000` consumers, excluding the 0xFFFF
terminator), id = value & 0xFFF:

| id | resolution | effect |
|---|---|---|
| 0x00A | func_8001A284 case 10 → **the player's own name** | the walk re-enters the same name → **unbounded recursion → stack overflow**; smash path descends from the stack top through the graphics heap, the asset scratch (where event programs load), ovl_11, the game state (calendar 0x80070CF0), `gCurrentSceneId` 0x8005E39C and the scene function pointer 0x8005E384 |
| 0x00B–0x010 | another name buffer (horse/dog/farm/…) | bounded recursion — renders that name inside yours; a mutual pair (player name carries 0x4x0C, dog name carries 0x4x0A) is unbounded |
| 0x000–0x009 | D_8005F0C8 slot (runtime pointers in save menus; 0 elsewhere) | skip in gameplay; live pointer in save menus |
| 0x00A..0x07B (unset slots) | D_8005F0C8[id] = 0 | silent skip |
| 0x07C–0xFFF | **OOB read of live .bss as a pointer** | the renderer walks sprite packets / pad state as u16 text; packet halfwords such as tpage words (0xE1xx) themselves carry bit 0x4000, cascading through further OOB table reads — a self-sustaining pointer-chaos walk with unbounded recursion and unbounded glyph-packet writes (`func_80019070` writes raw packets at an advancing cursor with no bound of its own) |
| no bit 0x4000 | — | inert junk glyph |

The self-reference id 0x00A requires the harvested `_spu_tsa` value to end
in 0x00A with bit 0x4000 (e.g. 0x400A/0x500A/0x600A/0x700A) — reachable in
principle by area choice; the value set is enumerable at runtime.

## 6. Verified vs. open

Verified statically (original bytes, matched sources, symbol files —
sections 1–4 above and the citation list):

- table size 28 vs index space 63; the index arithmetic with no bounds
  check;
- the toggle branches with no cursor check and the flip that does not touch
  the cursor;
- the set-0/set-1 bounds arrays and the reachable OOB cell set;
- the harvested values, their addresses and symbol identities;
- `_spu_tsa`'s writers and the per-area bank-load influence;
- the four fallback consumers, the 124-word real extent, the 0–10 slot
  writes, the resolver's name cases;
- the missing depth limit in the draw recursion;
- the name-reference codes present in the fixed text bank.

Open (runtime questions, cheapest tests first):

- the actual `_spu_tsa` value at each naming screen (initial load address
  is 0x1010; per-area banks advance it) — needs a watchpoint on
  0x80055A34;
- whether the word at 0x8005FE08 (the 0x7350 id 0x350 target, inside the
  aux-sprite work area) is nonzero in practice;
- which end state each OOB id produces (crash, hang, garbage render,
  surviving walk);
- whether any pointer-chaos walk can be steered into a useful write.

## 7. Runtime test plan

1. New-game player naming: cursor to 0x24 (down 2, right 8), L1+L2, X,
   START. Watchpoint on 0x80071A00 — confirm the 0x7350 halfword landed.
2. Play to the first dialogue that says the player's name; break on the
   fallback load (the `lw` at original 0x80018ED4 / breakpoint on
   0x8005F0C8+4*0x350 = 0x8005FE08) and observe the resolved value.
3. Same with cursor 0x40 (down 4, right 8): note the harvested `_spu_tsa`
   value at each naming screen.
4. Enumerate `_spu_tsa` across areas (watch 0x80055A34 while entering
   buildings/areas), then name the dog in the area whose value carries bit
   0x4000; prefer an id ending 0x00A for the self-reference, or 0x07C+ for
   the OOB-pointer reads.
5. Trigger a dog-name dialogue (or player-name dialogue) with a
   self-referencing name: watch `$sp` march down (the recursion), and the
   smash targets 0x8005E39C (scene id) / 0x80070CF0 (calendar).

## 8. Relationship to the other findings

- The known length-8 `+1` write (audit section 1) is a separate, weaker
  effect of the same input module; this read makes it largely irrelevant.
- The farm-name +0x12 write lands on dead padding (audit section 6) —
  unchanged.
- The debug scene-select cluster (audit section 7) is unreachable by
  input; the stack-overflow variant of this chain passes through
  `gCurrentSceneId` and the scene function pointer, which is the first
  known route that could write them.
- The event-interpreter sinks (audit section 8) are the downstream
  consumers if a smash reaches the asset scratch where programs load.

## Sources

Original binary reads (all from `extracted/iso/slus_011.15`, PS-X EXE
header 0x800, load 0x80010000; file offset = ram − 0x80010000 + 0x800):

- `GetTextEntryCodeSet1` disassembly 0x800245C8–0x800245EC (section 1).
- `ProcessTextEntryInput` dispatch 0x80023E00–0x80023EE8 and flip code
  0x80023FC4–0x80023FDC (section 2).
- `AdjustTextEntryRow` disassembly 0x800243D0–0x80024404 (column-13 remap).
- `func_80018B98` reference/recursion block 0x80018EA8–0x80018F10
  (section 4), including the `lui 0x8006 / addiu −3896` formation of
  0x8005F0C8.
- Bounds arrays: 0x8005599C, 0x800559AC, 0x8005E364, 0x8005E36C;
  handler-pointer table 0x800559CC; the code table 0x800559D4 and the
  following 0x48 bytes (0x80055A0C–0x80055A54) including the 0x7350 gap
  value and the `_spu_*` region.
- Text-bank name-reference census over 0x80051000..0x8005E000.

Matched (byte-exact) sources: `src/modules/text_entry.c` (entry, bounds,
code handlers), `src/modules/pad.c` 311–323 and 196–204 (button word
layout, Start reporting), `src/func_8001A284.c` (resolver cases),
`src/func_80018B98.c` 41–66 and 195–207 (pacing gate, reference block),
`src/func_8001929C.c`, `src/func_8001945C.c`, `src/func_800198E0.c`
(fallback consumers), `src/func_80017A08.c` (slot setter clamp),
`src/modules/aux_sprite_display.c` (work area 0x1300 bytes),
`src/modules/sound_driver.c` 119–129 and 653–712 (bank transfer and SPU
load address), `src/overlays/ovl_27/modules/menu_scene.c` 159/204/229,
`src/overlays/ovl_11/` naming call sites, and the ovl_30 debug-table dumps
that identify the name buffers.

Symbol references: `configs/symbols/exe.txt` lines 784, 791, 794, 802–809,
821–826 (text-entry symbols and sizes) and the libspu neighborhood
(`math_errno` 0x80055A14, `_spu_RXX` 0x80055A1C, `_spu_tsa` 0x80055A34,
`_spu_transMode` 0x80055A38, `_spu_addrMode` 0x80055A3C,
`_spu_mem_mode` 0x80055A40), `gAuxSpriteSource` 0x8005F2B8 / size 0x30,
`gAuxSpriteOrderingTables` 0x8005F2E8 / size 0x100, `gAuxSpriteHeader`
0x800605F0 / size 0x40, `gCurrentSceneId` 0x8005E39C,
`gTextEntryCharacterSet` 0x8005E358.

Methodology (reproducible): all cross-references were found by scanning
the EXE and the 13 runtime-loaded overlays (`extracted/overlays/*.bin`,
load bases 0x800B7E20 / 0x8012DDE8 per the EXE `.data` words D_80010098 /
D_8001009C) for constant-address formation (lui/addiu/ori/addu chains with
16-bit sign extension) and GP-relative accesses; overlays are raw loads at
those bases. The per-area bank addresses and runtime .bss contents are the
only inputs that require execution to resolve.

Parent research: `notes/research/speedrun-calendar-glitch-audit.md`
(sections 1, 6, 7, 8, 9 and the updated priorities).
