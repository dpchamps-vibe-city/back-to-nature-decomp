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
- what a long walk actually corrupts: the glyph packets march upward
  through the graphics heap (texture/asset scratch) only — the pool
  geometry and the post-walk halt are in audit section 10, so "steered
  into a useful write" now means steered into something *the same frame's
  walk re-reads* (self-modifying cascade), not into game-state globals.

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
  input, and the scene-17 request site is dead code (audit section 10).
  Moreover the packet-pool geometry rules the §9 engines out of the scene
  globals entirely: the pools sit in the graphics heap *above* 0x8005E39C
  and the cursor only advances upward, and finite overflowing walks halt
  at the pool bound (ReservePacketWorkBytes) after the corruption lands —
  so no chain from this bug reaches gCurrentSceneId or WORK.state. See
  audit section 10 for the full writer census and geometry.
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

## 9. Runtime verification (psxw headless, this session)

Verified end-to-end in the headless runner (`/home/dave/Documents/code/psx-wasm`,
`psxw run btn --replay oob_test5.rpl`), new-game player naming, controller only:

- Replay pad mapping (calibrated against in-guest pad-state traces at
  0x8005E870/74/78): `game_held = ~bswap16(replay_word)`. Pure-button replay
  words: CROSS 0xBFFF, START 0xFFF7, DOWN 0xFFBF, RIGHT 0xFFDF, CIRCLE 0xDFFF,
  L1+L2 0xFAFF. Cursor movement runs on the auto-repeat stream
  (`ProcessTextEntryInput`'s default case reads `repeatButtons`, not held):
  a new press emits exactly one move; pad polls every 2 vblanks; 2-tick
  holds move exactly one cell.
- Executed sequence (player name screen): DOWN x2, RIGHT x8 (cursor 0x24),
  L1+L2 same-frame (charset 0->1, cursor untouched), CROSS.
- Result (RAM dumps): cursor 0x24 / set 1 confirmed; appended halfword =
  `gTextEntryCodeTableSet1[2*10+8] = gTextEntryCodeTableSet1[28]` =
  **0x7350** (bit 0x4000 set, reference id 0x350); player name at 0x80071A00
  = [0x7350, 0xFFFF, ...] after confirm+execute. Naming flow then continued
  normally (farm 0x80070D18, dog 0x800759E8).
- This confirms the bug is player-triggerable with ordinary controller input
  on the very first screen of a new game.

Still open (next steps):
1. Continue the run to the first dialogue that renders the player name;
   0x7350 resolves id 0x350 via `D_8005F0C8[0x350]` = the .bss word at
   0x8005FE08 (aux-sprite work area): nonzero => OOB pointer walk + unbounded
   glyph-packet writes; zero => silent skip.
2. Enumerate `_spu_tsa` (OOB index 48, live SPU transfer address) at the
   in-game naming screens (horse/dog rename, animal purchase) after per-area
   sound-bank loads; a harvested value with bit 0x4000 and id 0x00A
   (0x400A/500A/600A/700A) is a self-reference => unbounded recursion in
   `func_80018B98` => stack overflow whose smash path crosses the scene-id
   block (0x8005E39C). A 0x400C/0x400A player/dog mutual pair works too.

## 10. Resolver internals, unbounded fallback table, and the save-data route (this session)

`ResolveTextSource` (src/modules/text_engine.c) resolves `code & 0xFFF`:
ids 10-14 -> name buffers (10 = player 0x80071A00, 12 = dog, 13 = farm),
16-21 -> padded numbers, 22/23 -> `&D_800749F8 + (s16)state * 0xB8`,
default -> 0. When the resolver returns 0, the renderer falls back to
`gTextSourceAddresses[code & 0xFFF]` (text_engine.c scan/count/measure loops).

- `gTextSourceAddresses` = 0x8005F0C8, **size 0x2C (11 u32, ids 0..10)**.
  Writes are clamped (`SetTextSourcePointer` caps the index at 0xA) but the
  READ index is the full 12-bit text code: an asymmetric-bounds bug.
  Id 0x350 reads the u32 at 0x8005F0C8+0xD40 = 0x8005FE08 — a fixed .bss
  word, observed 0 at runtime, so the OOB-harvested 0x7350 name char is
  silently skipped (no crash, no walk) at retail timing.
- The measure/count/render loops RECURSE into the resolved pointer with no
  depth limit and no cycle check. A name character of 0x400A (id 10 = the
  player name buffer itself) is a self-reference: infinite recursion, and
  the PSX stack grows down through the 2 MiB main RAM, smashing .bss
  including gCurrentSceneId (0x8005E39C). Static proof complete.
- Name entry cannot produce 0x400A: the charset tables output 0x0000-0x008B
  plus the fixed OOB window (0x80055A0C..0x80055A5C = math_errno, _spu_RXX,
  _spu_tsa, SPU modes/transfer flag). The window was dumped at the name
  screen and in-game: static, 0x7350 the only reference value
  (_spu_tsa frozen in the emulator). In-game text entry exists (ovl_11:
  record trade, status text, quantity purchase), so the OOB harvest is
  also reachable mid-game; on real hardware _spu_tsa is a live SPU address
  whose low half cycles through 0x4000-0x7FFF as samples stream.
- SAVE DATA is the clean injection path: ovl_15 save image = 0x8000 bytes =
  256 x 0x80 records. Records 5..255 store the XOR of their first 127 bytes
  in byte 0x7F; record 4 (metadata) stores the XOR of all record checksums
  at +0x38 and its own at +0x7F. The player name is at payload offset 0x3060
  (record 5 base) = **card file offset 0x3341** (section 12 has the
  compaction mapping; the earlier 0x32E0 figure was wrong), copied raw on load
  (Ovl15RestoreSaveImage) with no per-character validation. Checksums are
  trivial XOR. Card filenames: BASLUS-01115-FARM0/1/2 (ovl_15 .data).
  => A crafted card save with name[0] = 0x400A loads cleanly and, on the
  first render of the player name (save-menu preview or any name dialogue),
  recurses to a stack overflow. Runtime proof in progress (crafted MCR +
  psx-wasm card0.mcr + save-screen replay).

## 11. Save-data injection route (id-0x00A self-reference) — status

To runtime-demonstrate the unbounded-recursion chain (section 10), the player
name must contain 0x400A (id 10 = the name buffer itself). In-game harvesting
only yields 0x7350 (section 10), so the name must come from a crafted save.

Format facts established this session (runner-authoritative):
- Memory card: `psx-wasm/crates/psx-devices/src/memcard.rs`. 128 KiB, 16
  blocks of 64 frames of 128 B. Directory: frame 0 "MC"; frames 1..15 hold one
  entry per data block (entry for block b is in frame b): [0] state
  (FREE 0xA0 / FIRST 0x51 / MIDDLE 0x52 / LAST 0x53), [4..8) size (FIRST),
  [8..10) next = next_block-1 (0xFFFF end), [0x0A..0x1F) name (FIRST); each
  frame sealed with XOR(0..0x7E) at 0x7F. Data block b occupies frames
  16b..16b+15. A 32 KiB save = blocks 1..4.
- Save image (0x8000, 256 records of 0x80): the slot SUMMARY is record 4 at
  file offset 0x200 (`Ovl15ReadSaveSummary` does
  `MemCardReadFile(channel, name, buf, 0x200, 0x80)` = offset,size).
  Summary field_34 (file 0x234) must be 0x14 — the load gate in
  `Ovl15CardReadVerifyChecksums` (save_menu.c); record 4 carries its own XOR
  at 0x27F and the XOR-of-all-records at 0x238. Records 5..255 are payload
  with per-record XOR at +0x7F. **CORRECTED (2026-10-29): the player name
  block is at save-image offset 0x3341, NOT 0x32E0** — see section 12 for the
  compaction mapping that explains the delta.
- **RESTORE RUNS ON THE SAVE SCREEN (resolved).** The earlier "no load
  starts / slot summary stays zeroed" stall was the 0x32E0 vs 0x3341 offset
  error, not a card handshake problem. The real flow (save_menu.c CardRead
  states 0-19): state 13 `Ovl15VerifySaveChecksums` -> state 19 (0x13)
  `Ovl15CardReadRestoreImage` -> "load complete" panel -> after a button OR a
  91-frame timer: `RemoveSaveChecksumSlots` + `Ovl15RestoreSaveImage` + exit.
  With 0x400A at 0x3341 (+ record-102 XOR, record-4 data-XOR @0x238, record-4
  XOR @0x27F, all re-verified against `Ovl15VerifySaveChecksums`), the runner
  confirms the card, the restore runs (~V2840), and **0x80071A00 = 0x400A is
  in RAM from V2880** — the self-reference is live.
- **Post-restore deadlock.** A minimal crafted card (zeros except the name
  fields) restores an all-zero world: `gScenePhase` and friends come back
  zero, the game enters scene 14 and ignores START/CROSS for 2100+ frames,
  renders nothing, and traps nothing. A *valid* world image (a real save with
  only the name grafted) is needed for the restored game to run and render the
  name in play.
- **CORRECTED (2026-10-29): the save-screen slot summary is NOT a recursion
  vector.** `Ovl15DrawSaveSlotSummary` (save_menu_ui.c) uses *per-draw* text
  source pointers 1..6 (not the global gTextSourceAddresses), and for a valid
  slot (status 0) it **sanitizes the loaded header names in place**: each of
  the 8 halfwords of name_8/name_1A with value >= 0xE01 (and != 0xFFD) is
  replaced by 0xFFFF before the name is walked. A 0x4000|id reference is
  >= 0xE01, so a crafted header name cannot carry a reference — the label
  walk is bounded. The earlier "re-open the save screen to crash" plan is
  invalid. The only recursion trigger is the in-game name dialogue (text
  0x80051B0E, the sole 0x400A in text), which walks the player name via the
  global gTextSourceAddresses[10]; reaching it needs in-game navigation.
- A runtime-mod route was ruled out for this purpose: the psxw mod API
  (psxmod.h) is log-only; it cannot write the name into RAM, and data
  patches apply at load, before the title entry overwrites the name.

Remaining steps (revised): (a) get a *valid* save image — the game reaches
  in-game scene 2 with an empty name (working replay, vblank 4239), so find
  the in-game save trigger (ovl_11 in-game menu -> ovl_15 diary save), play
  to it, save, and capture the card; (b) graft 0x400A at file 0x3341 (+ its
  checksums) so the restored world runs; (c) from the restored world, navigate
  to the day-1 name dialogue (text 0x80051B0E) to fire the recursion, and
  capture the stack smash (expected to cross `gCurrentSceneId` 0x8005E39C via
  RAM dumps / no-trap). Alternatively (d) demonstrate the L1 0x7350 render:
  from a controller-only OOB name, navigate to the same dialogue and observe
  the unbounded walk of the word at 0x8005FE08 (id 0x350).

## 12. Compaction mapping, naming-flow stages, and replay construction (2026-10-29)

**Why 0x3341, not 0x32E0.** `RemoveSaveChecksumSlots` compacts the payload
*in the transfer buffer* (drops each record's XOR byte 0x7F, so records
5..255 fall to a 0x7F stride) **before** `Ovl15RestoreSaveImage` reads game
state from the compacted payload base (buffer+0x280). For a payload
(game-data) offset T the restore reads at U = T + T//0x7F, i.e. card file
offset 0x280 + U. Player name T = 0x3060 -> U = 0x3060 + 0x61 = 0x30C1 ->
**file 0x3341** (record 102, byte 0x41). The old 0x32E0 = 0x280 + 0x3060
ignored the compaction shift, so the restore read a zero and the name never
changed. Crafting rule: place the value at 0x280 + T + T//0x7F and re-verify
the record's XOR (byte 0x7F) plus record 4's two XORs against
`Ovl15VerifySaveChecksums`.

**New-game naming flow** (`src/overlays/ovl_27/modules/title_scene.c`,
`gOvl27TitleMenuStage` 0x800C4A24): the title menu reaches stage 13 (prompt
0x397) -> 14 `BeginRequiredTextEntry(&gPrimaryObjectBase)` (player name,
cancel disabled) -> 15 `UpdateTextEntry` -> 16 prompt 0x3A5 -> 17
`BeginIndexedSelection` (NOT text entry — a cursor+confirm selection) -> 19
prompt 0x3A4 -> 20 `BeginTextEntryAlternateLayout(&D_80070D18)` (farm,
cancelable) -> 22 prompt 0x3A3 -> 23 `BeginTextEntry(&D_800759E8)` (dog,
cancelable) -> 25 record summary -> 26 choice 1 proceeds to the game. A
replay must handle the indexed-selection and the summary-confirm stages, not
just the three text entries.

**Text-entry state machine** (`text_entry.c`): state 1 = typing, state 2 =
confirm prompt (START with a non-empty name), state 3 = canceled, state 4 =
confirmed. `ProcessTextEntryInput` is gated by `IsMessageWindowStatusFinal()`
(status 2 **or** 5) — the prompt must finish animating before any input
registers; the player-name prompt settles to status 5 ~46 vblanks after the
naming screen opens.

**Replay-construction gotchas (psxw).**
- Replay entry index = the guest clock u32 at 0x8006C840, NOT the vblank
  number. For BTN the clock runs ~0.46 x vblank and starts ~100 vblanks in,
  so a press at clock C lands at vblank ~C/0.46. The player-name typing
  window is narrow (~150 vblanks); a long input sequence spans many clock
  units and mostly fires after the window advances. Build sequences in clock
  units and keep them tight.
- Replay word = bswap16(~game_held) (game space per section 2: CROSS=0x0040
  append, CIRCLE=0x0020 backspace, START=0x0800 confirm, L1+L2=0x0005 flip,
  DOWN=0x4000, RIGHT=0x2000, LEFT=0x8000, UP=0x1000). Verified replay words:
  CROSS=0xBFFF, CIRCLE=0xDFFF, START=0xFFF7, DOWN=0xFFBF, RIGHT=0xFFDF,
  LEFT=0xFF7F, UP=0xFFEF, L1=0xFBFF, L2=0xFEFF, L1+L2=0xFAFF. (Section 9's
  labels were correct. A prior "correction" here that swapped DOWN/CROSS and
  RIGHT/CIRCLE was wrong — it pressed append/backspace instead of moving the
  cursor, which is why rebuilt replays left gTextEntryCursor at 0.)
- The pad-0 input-substitution path (`D_8006C838+0xC & 0x10000`) is inactive
  during naming (0x8006C844 traced 0), so it is not an input blocker.
- A working title->naming prefix: `out/oracle/replays/
  btn-title-to-gameplay.rpl`. Its inputs decode (verified mapping) as: clock
  400–1143 title menu (START/CROSS), naming screen opens clock 1122 / vblank
  2425, player-name prompt final ~clock 1145 / vblank 2471, player name
  confirmed clock 1200 (START), then indexed-selection/farm/dog/summary
  input to clock ~2008, in-game D-pad clock 2150–2509 (scene 2 at vblank
  4239).

**Working OOB replay (reproduced this session, `oob_full.rpl`).** The whole
reason rebuilt replays failed: the title prefix must run to **clock 1150**
(the title menu needs input to clock 1143); truncating it at 1030 leaves the
game in the title menu so the OOB D-pad hits the title (no-op). Recipe:
prefix = working[0:1150], then into the player-name window — DOWN x2
(0xFFBF) @1160,1168; RIGHT x8 (0xFFDF) @1176+8i; L1+L2 (0xFAFF) @1240;
CROSS (0xBFFF) @1248; START (0xFFF7) @1256; CROSS @1264. Result:
`0x80071A00` = **0x7350** from vblank 2677, stable to 4000, no trap. Timing:
**vblank = 2*clock + 181** (measured: clock 400→vblank 981, 550→1281).
Calibration (traced pad0 heldButtons 0x8005E874 during title menu): replay
0xFFBF→game 0x4000, 0xFFDF→0x2000, 0xBFFF→0x0040, 0xFAFF→0x0005. Reachable
set-1 OOB references (dumped): idx28=0x7350 (id 0x350→word @0x8005FE08),
idx48=0x657a (id 0x57a→word @0x800606B0, DOWN x4/RIGHT x8); idx29-47 are
glyphs/zeros, idx49+ zero. No 0x400A (self-ref) in the reachable window —
that still needs the save route.

**OOB name carries through to in-game (`oob_align.rpl`).** Compressing the
OOB sequence to confirm the player name inside the working replay's own
window (so later stages stay on their natural timeline) works: prefix =
working[0:1122]; OOB DOWN x2 @1150,1157; RIGHT x8 @1164+7i; L1+L2 @1220;
CROSS @1227; START @1234 (confirm 34 clocks after working's 1200); then
working[1240:2510] shifted +34. Result: name 0x80071A00 = 0x7350 from
vblank 2635, stable; scene id 0x8005E39C goes 0→13 (title, vblank 99)→
**2 (in-game, vblank 4307)**; title stage machine runs 15→26 cleanly; no
trap to vblank 7000. So the controller-only OOB is stable and the game keeps
running in-world with the reference-control-code name. The observable effect
still requires the name to be *rendered* via the reference walk (record
summary stage 25 ~vblank 4109 did not trap; the in-game name dialogue is the
text-bank 0x80051B0E caller).

## 13. Set arithmetic/table split, full reachable window, and a second OOB (write)

**Set 0 is arithmetic, set 1 is a table — different index math.**
`GetTextEntryCode(cursor, set)` splits `quot=cursor/14`, `rem=cursor%14` and
calls the per-set handler with `(rem, quot, cursor)`:
- `GetTextEntryCodeSet0(col, row)` (src/modules/text_entry.c:732) returns an
  ARITHMETIC code, no memory read: col<5 → `row*5+col`; col<0xA →
  `0x20+row*5+(col-5)`; else → `0x40+row*3+(col-10)`. So set 0 produces small
  codes and can never itself yield a 0x4000 reference.
- `GetTextEntryCodeSet1(col, row)` (line 757) reads
  `gTextEntryCodeTableSet1[row*10 + col]` — a **10-wide** table, base
  0x800559D4. The set-1 index for a cursor that was positioned in set 0 is
  `(cursor/14)*10 + (cursor%14)`. Because set 0's grid is much taller than set
  1's 2 rows, a cursor valid in set 0 maps far past set 1's end: that is the
  OOB read. The reachable set-1 index for a set-0 cursor at (row0,col0) is
  `row0*10 + ((row0*14+col0)%14)`.

**Full reachable window (dumped at the naming screen, ram_002600).** 64
halfwords from 0x800559D4: idx0-27 are in-table glyph codes (0x62-0x8E, no
0x4000 bit); **idx28 = 0x7350 (ref id 0x350 → word @0x8005FE08) is the ONLY
stable reference value in the whole window**; idx29-35 = 0x000B/zeros;
idx36-48 = volatile .bss (CLUT/sprite words 0x1C00/0x1F80/0x10C0…, and idx48
was 0xA5AC in this run vs 0x657a in the earlier one — the window content is
**state-dependent, not stable**); idx49-63 = a small lookup table (0x0000-0x0008
pairs). Conclusion: the controller-only harvest reliably yields **0x7350
only**; the self-reference 0x400A (id 0x00A = player name) is **not
reachable** through this window in any observed state. The deterministic
self-recursion crash therefore still requires the save-data route (section
11), not the controller.

**Second OOB — a WRITE, in the confirm path.** In `ProcessTextEntryInput`
(src/modules/text_entry.c:440) the CROSS/OK-cell confirm branch fires when the
pad word is 0x40 (CROSS) *and* the cursor is at 0x53 (the grid OK cell) with a
non-empty name: it runs `gTextEntryBuffer[gTextEntryLength + 1] = 0xFFFF` with
**no length cap**, then sets state 2. The append path is capped at
`gTextEntryLength < 8`, so the name field holds 8 halfwords (0x10 bytes) at
the start of `gPrimaryObjectBase` (0x80071A00; the START path, 0x800, sets
state 2 with no terminator write). With a FULL 8-character name the confirm
writes index 9 → **0x80071A12**, one halfword past the name field (a default
terminator slot sits at index 8 / +0x10). Dumped, 0x80071A12 currently holds
0x0064 ("d"), i.e. the word 0x00640064; the OOB write overwrites its low
halfword with 0xFFFF. The value is fixed (0xFFFF) and only 2 bytes, so impact
is limited to corrupting the field immediately after the player name — but it
is a controller-reachable out-of-bounds write (also present for the dog name
buffer 0x800759E8 and farm name buffer 0x80070D18, each writing past its own
name field). Distinct from the section-1 OOB *read*+walk: this one writes.
Reachability requires: enter 8 characters, move the cursor to grid cell 0x53
(row 5, col 13 of the active set), press CROSS.

**Precise OOB mechanism — writer clamps, readers do not (text_engine.c).**
`gTextSourceAddresses` (0x8005F0C8) is an **11-word** table because the ONLY
writer, `SetTextSourcePointer` (line 37), clamps: `idx = arg0 & 0xFFFF; if
(idx >= 0xB) idx = 0xA; gTextSourceAddresses[idx] = arg1;` — writes are
bounded to 0..0xA. But the four READERS index it with a raw 12-bit mask and
NO clamp:
- line 848  `work4      = gTextSourceAddresses[*arg2 & 0xFFF];`
- line 1121 `r          = gTextSourceAddresses[*cur  & 0xFFF]`
- line 1199 `value      = gTextSourceAddresses[*ptr  & 0xFFF]`
- line 1300 `temp_a0_2  = gTextSourceAddresses[temp_a0 & 0xFFF]`

**Where the name is actually rendered through this path.** Only the in-game
name dialogue (text bank 0x80051B0E) renders the player name via these
gTextSourceAddresses readers. The title record summary
(`Ovl27DrawTitleRecordSummary`, title_scene.c:704) does NOT draw the name — it
draws a text-bank label, a portrait sprite (base+0x5246) and a decimal
(base+0x5248) — so it is not a name-render point. The on-screen entry preview
(`DrawTextEntryPreview`, text_entry.c:397) builds a separate
`gTextEntryPreviewBuffer` via `BuildTextEntryPreview` (INCLUDE_ASM) and draws
that as glyphs; it does not route the stored reference codes through the
gTextSourceAddresses walk. So both the OOB read (finding 1) and the
self-recursion (finding 2) only EXECUTE when the in-game dialogue renders the
name; the controller harvest puts the trigger value in the name, but the effect
still needs the in-game render (navigation blocker). The OOB WRITE (finding 3)
executes during the naming flow itself (no in-game nav needed).

So any reference halfword whose low 12 bits (id) are 0xB..0xFFF reads past the
table: id 0x350 (from the harvested 0x7350) reads
0x8005F0C8 + 0x350*4 = **0x8005FE08** (volatile .bss, ~0 at the naming screen,
so the reference is skipped/benign). This is the controller-triggerable OOB
READ. Separately, id 0xA (0x400A) is IN bounds: `gTextSourceAddresses[0xA]` is
the player-name pointer (0x80071A00, set via SetTextSourcePointer(10,…)), so
0x400A in a rendered name is a **self-reference** — the reader resolves it back
to the name buffer and re-loads it, with no depth/cycle guard → unbounded
recursion (the deterministic crash; needs the save route since the controller
harvest only yields id 0x350). `ResolveTextSource` (line 1605) is a DIFFERENT,
separate id-switch (case 10 → player name … default → 0) used for other source
types; the name-render path is the gTextSourceAddresses readers above.

## 14. OOB-write target is a live player-object field (gPrimaryObjectBase+0x12)
Symbol map (ovl_19/27/17 disassembler_symbol_addrs.txt:1724-1726):
  gPrimaryObjectBase  = 0x80071A00   (player object; name = first 8 hw, +0x00..+0x0F)
  gMenuGridLayoutIndex= 0x80071A1E   (0x1E bytes on -> gPrimaryObjectBase is 30 bytes)
So gPrimaryObjectBase spans 0x80071A00..0x80071A1D. The name entry uses
`(u16 *)&gPrimaryObjectBase` as its buffer (title_scene.c:392). ClearTextEntryBuffer
writes indices 0..8 (+0x00..+0x11). The confirm OOB write is index 9 = +0x12, which is
INSIDE gPrimaryObjectBase (a live field between the name and the next global), not mere
.bss padding. Writing 0xFFFF there clobbers a player-object field at offset 0x12 on
name confirm. (Field identity: tentative defs mark +0x12-class fields "zeroed on init,
purpose unknown"; not a pointer per current views.)
Impact: deterministic 2-byte corruption of a live player-object field, fully
controller-triggerable (8 chars + OK-cell + CROSS). Benign for the observed field but a
real state-corruption bug, and it sits next to gMenuGridLayoutIndex (see the interaction
menu grid-nav OOB note).

## 15. OOB-read window contents = math + SPU globals (state-dependent wild pointer)
gTextEntryCodeTableSet1 @0x800559D4 is 28 halfwords (idx 0..27, ends 0x80055A0B).
The reachable OOB window (set-1 idx 28..48) reads past it into:
  0x80055A14 math_errno
  0x80055A18 math_err_point   (a POINTER)
  0x80055A1C _spu_RXX         (SPU/sound register globals: _spu_tsa/_transMode/_addrMode...)
So the charset-toggle OOB read is not just "reads .bss" — it reads live library/sound
state. A pointer's low halfword (e.g. math_err_point = 0x8005xxxx -> 0x5xxx) HAS bit
0x4000 set, so if such a global is non-zero at render time, the harvested value is a
string-REFERENCE code, and the reader does gTextSourceAddresses[id & 0xFFF] (unclamped,
idx 0xB..0xFFF = OOB) and follows the resulting pointer. The observed 0x7350 (id 0x350)
target was 0 (benign); but the mechanism can follow a non-zero/wild pointer in other
states. This upgrades the OOB read from "benign for one value" to "state-dependent
out-of-bounds read + wild pointer follow."

## 16. Self-recursion CONFIRMED unbounded (arg2==0 at every call site)
MeasureTextSpanLength(u16 *arg0, u16 arg1, s16 arg2) [text_engine.c:1175] recurses on
each 0x4000 reference:  index += MeasureTextSpanLength(nin, arg1, arg2 ? arg2-index : 0).
ALL top-level callers pass arg2 == 0:
  :581  MeasureTextSpanLength(gTextScriptCursor, 0xFFC,  0)
  :605  MeasureTextSpanLength(cur,               0xFFFE, 0)
  :620  MeasureTextSpanLength(gTextScriptCursor, ...)   [arg2 0]
  :1071 MeasureTextSpanLength(base,               *cur,  0)
  :1152 MeasureTextSpanLength(arg0,               0xFFFE, 0)
  :1588 MeasureTextSpanLength(gTextScriptCursor,  0xFFFF, 0)
and the recursive call keeps arg2==0 (arg2 ? ... : 0). So the limit never engages.
Chain for a name buffer starting with 0x400A (id 0xA == player name):
  render name dialogue -> MeasureTextSpanLength(dlg, 0xFFFF, 0)
    -> sees 0x400A, ResolveTextSource(0x400A & 0xFFF=10) = 0x80071A00 (name buffer)
    -> recurse MeasureTextSpanLength(0x80071A00, ..., 0)
    -> reads 0x400A again -> recurses ... -> INFINITE (no depth/arg2 limit)
=> unbounded recursion, PSX downward stack smash (sp grows toward .bss), eventual
   illegal-instruction / bus-error trap. Deterministic given (a) name buffer = 0x400A
   [crafted save at file 0x3341] and (b) the in-game name dialogue rendering it.

## 17. Name dialogue 0x80051B0E confirmed to render the player name (dump-verified)
RAM dump (oob_align, vblank 5327):
  0x80051B0E +0x00 = 0x400A  (player-name self/reference code, FIRST token)
  +0x02 = 0xFFFE (line break), then glyphs 0x0001 0x0028 0x0031 ...
So 0x80051B0E is a dialogue whose first token is the player-name reference -> when
rendered it runs MeasureTextSpanLength over it, hits 0x400A, ResolveTextSource ->
player name buffer, and recurses into the name. This is the render point for A1 (OOB
read, name=0x7350) and A3 (self-recursion, name=0x400A).
The in-game text window that DID open (work buffer 0x8006E110) is a DIFFERENT message
(glyphs 0x0F87 0x0FF7 0x0FF4 ...; no 0x400A), so the name dialogue has not yet been
triggered in any captured run. Trigger = the specific day-1 NPC/event that plays
0x80051B0E (in-game navigation, the current blocker).
