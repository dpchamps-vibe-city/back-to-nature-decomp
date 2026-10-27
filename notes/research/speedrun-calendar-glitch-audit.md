# Speedrun calendar / memory-corruption investigation

## Scope and status

Read-only investigation of the current reconstructed source and the original
SLUS-01115 executable/overlay/archive bytes. No emulator or console execution
was performed. No game source, configuration or headers were changed by this
investigation. This is a first-pass lead inventory, not an exhaustive audit of
every function, and not a confirmed runnable skip or ACE exploit.

## 1. Concrete input-controlled off-by-one in text entry

`src/modules/text_entry.c:492`, `ProcessTextEntryInput` (EXE 80023DBC):

```c
gTextEntryBuffer[gTextEntryLength + 1] = 0xFFFF;
```

This happens when the on-screen confirmation cell (cursor 0x53) is selected.
Appending characters permits length 8; initialization clears nine halfwords
(indices 0..8). Thus at length 8 this confirmation writes index 9, two bytes
past the normal 18-byte string span. The separate Start-button path does not
perform this extra store. The store happens before the subsequent yes/no
confirmation, not only after accepting it.

Original EXE words independently confirm the address calculation:

```
80023F18 978200DA   lhu   v0,0xDA(gp)    # gTextEntryLength
80023F1C 8F8400EC   lw    a0,0xEC(gp)    # gTextEntryBuffer
80023F20 3403FFFF   ori   v1,zero,0xFFFF
80023F28 00021040   sll   v0,v0,1
80023F2C 00441021   addu  v0,v0,a0
80023F30 A4430002   sh    v1,2(v0)
```

The EXE was read from `extracted/iso/slus_011.15`, with PS-X EXE header 0x800
and load address 80010000. The configured gp is 8005E274; the two gp offsets
resolve to the published length and buffer globals, 8005E34E and 8005E360.

New-game callers in `src/overlays/ovl_27/modules/menu_scene.c` establish:

| Name | Destination | Length-8 extra write | Evidence for affected field |
|---|---|---|---|
| Player | 80071A00 | 80071A12 | Status-delta service uses +12 current gauge, +14 limit; original debug label at that address is `体力` (stamina). |
| Farm | 80070D18 | 80070D2A | Address calculation only; gameplay meaning of victim not established here. |
| Dog | 800759E8 | 800759FA | Original debug label is `犬・愛情度` (dog affection). |

Original debug descriptors: ovl_30 menu rows at 801341D0 (player stamina) and
801345F4 (dog affection), decoded as Shift-JIS from the original overlay.
`src/overlays/ovl_11/modules/status_gauge_deltas.c` corroborates the player
field through its arithmetic and clamp behavior.

**Limits:** this gives a fixed halfword value at a destination chosen by the
naming caller, not arbitrary data or an arbitrary address. 0xFFFF is -1 in
signed consumers: do not describe this as free maximum stamina/affection.
The ordinary string terminator at index 8 remains present. Some other naming
callers use larger scratch areas, so writing beyond the string span need not
corrupt a live neighboring variable there. Persistence through new-game
setup and the first update still needs a runtime test.

**First runtime test:** enter eight ordinary characters, select the on-screen
confirmation cell, and watch 80071A12 / 800759FA. Compare against Start
confirmation and a seven-character control. Then trace the next field write
to learn whether the corrupt value survives, clamps, or changes an event.

## 2. Retail debug menu directly edits the date; entry remains unresolved

`src/modules/game_loop.c:370` loads archive entry 0x1E (ovl_30) for scene 17.
`src/overlays/ovl_30/modules/debug_menu_lifecycle.c` runs its menu.
`src/overlays/ovl_30/modules/value_action_pages.c` exposes the main data editor;
`modules/record_menus.c` performs descriptor-directed writes.

The original main-data descriptor table at 80134018 has rows for:

- 80070CF0: year, bounds 0..99;
- 80070CF2: season, bounds 0..3;
- 80070CF4: day, bounds 0..29;
- 80070CF6: weekday, bounds 0..6;
- 80070CF8 / 80070CFA: hour / minute.

The original Japanese labels explicitly identify year, season and day.
The gameplay area-mode machine has an exit for mode 6 that requests scene
17 (`src/overlays/ovl_11/modules/area_mode_sequence.c:277`). This audit did
not find a normal controller-input path setting that mode. A surviving menu
and a surviving exit branch do **not** establish a secret button combination
or normal reachability. Controller-2 reads inside the editor only change edit
step sizes; they are not evidence for an entry shortcut.

## 3. Calendar manipulation needs less than native ACE if script control exists

`src/overlays/ovl_11/modules/state_field_access.c` exposes assign/add/subtract/
multiply/divide/modulo operations over descriptor-selected state fields.
Original `gOvl11StateFields` entries establish:

- field 0x0C: year, 80070CF0;
- field 0x0D: season, 80070CF2;
- field 0x0E: day, 80070CF4.

The read service adds one for year/day; writes do not subtract one.
`ovl_11_func_800E698C` is script opcode 0x13, forwarding its four arguments to
`Ovl11UpdateStateField`. This could change the date through existing game
logic without executing newly injected MIPS instructions.

`modules/object_load_states.c:123` runs 16-byte event instructions. It indexes
`gOvl11ObjectOpcodeHandlers` using an unchecked byte opcode and follows
unchecked branch indices. `Ovl11ResolveStateField` also trusts field IDs and
record indices. These are attractive sinks **conditional on** obtaining
control of an event buffer, program pointer, or operands. The inspected
producers normally supply disc data. No gameplay-input corruption chain to
these sinks was established, including from the name-entry bug above.

The program base/cursor pointers are at 801291B0 / 801291B4. Watchpoints there,
plus on the calendar fields, would distinguish a useful data/script-control
primitive from an ordinary crash.

## 4. Ordinary sleep/time arithmetic does not by itself provide a bulk skip

`src/overlays/ovl_11/modules/clock_calendar.c`:

- A date advances on an hour rollover arriving at 06:00, not at midnight.
- There are 30 zero-based days per season and four seasons per year.
- Ordinary year rollover caps the stored year at 98.
- Time-advance requests replace remaining minutes; they do not accumulate.
- The batched loop processes minute ticks and yields for queued events.
- The day-scene callback still performs daily state/counter/scene work
  (`modules/clock_event_callbacks.c`, `Ovl11ClockRebuildDayScene`).

Thus an unconstrained *backend* minute count is not evidence that repeated
sleep inputs stack into a month skip, nor that large advances bypass daily
processing. The normal sleep offset helper, 800CD4E4, returns only 0, 120,
240 or 360 minutes; it is not a direct day/month/year input.

## 5. Bounded original event-program scan

A disposable TypeScript scan followed the actual archive layout:

1. Read A_FILE.HDT entry 12: object-archive base is A_FILE.BIN + 0x7452800.
2. Read the 452 original offset words at ovl_11:801240D8 (451 entry spans).
3. Find the section carrying descriptor bit 0x4000, as 800E4C84 does.
4. Compute the section data base as 0x38 + 4 * header.s16_at_2, as 800E4B6C
   does; use section-relative offsets from header +0x34, as 800E4B58 does.
5. Inspect aligned 16-byte records, using the original opcode table at
   800B975C, not guessed opcode names.

Measured coverage: **435 program spans, 24,961 aligned records**. Twelve empty
archive spans were skipped. Four nonempty entries were not parsed: IDs 0x105,
0x111, 0x112, 0x113 (their candidate program spans were backwards or not
multiples of 16). Those remain unresolved, not proven script-free.

Within the scanned records:

- No writes to year/season/day through state-update opcodes 13,14,58,5B,61
  (hex) were found.
- Direct time opcode 4C used literal operands (its indirection flags were
  zero). Relative-hour requests found used 1 or 2 hours; the other requests
  selected ordinary times of day.
- Exit/time opcode 25's relative-hour form used 1 hour; its absolute-time
  form selected 16 or 18 hours.

This is a structural record census, not CFG reachability proof for every
record, and excludes the four unresolved nonempty entries and any runtime-
constructed scripts. It found no literal built-in multi-month/year advance
in the parsed set. It does not rule out an exploit.

## 6. Follow-up: the farm-name victim is dead padding; every other
   text-entry overflow slot except stamina/affection is dead too

The length-8 farm-name write at 80070D2A lands on **alignment padding**,
not a live field:

- The farm name is a standalone 18-byte buffer at 80070D18 (nine halfwords:
  8 characters + the 0xFFFF terminator). The next named global is
  gProgressStateFlags at 80070D30; 80070D2A..80070D2F (six bytes) is unnamed
  in configs/symbols/exe.txt and in every reconstructed typed view
  (`pad44E0[0x18]` in the D_8006C838 views — exactly 18 name bytes + 6 pad).
- A constant-address scan of the whole EXE and all 13 runtime-loaded code
  overlays (the archive members the scene dispatch ever loads: 08, 10, 11,
  15, 17, 19, 21, 23, 25, 27, 28, 30, 31; the remaining archive members
  are data, including entry 0xC = the object/event archive)
  (lui/addiu/addu address formation with correct 16-bit sign extension)
  found **zero** loads, stores or address formations in
  [80070D18, 80070D30) other than the known consumers: the name itself.
  Every pointer-based consumer is bounded — the 0xFFFF-terminator walkers
  (ovl_15 80133808 display loop, Ovl11DrawClockHud, the func_8001A284
  case-13 text renderers), the 18-byte aggregate copies
  (Ovl11Status801295D0, animal naming), Ovl15BuildSaveSlotHeader
  (reads year 80070CF0, day 80070CF4 and the 18 name bytes), and the
  0xCFC-byte save block copy (Ovl15SerializeSaveImage/Ovl15RestoreSaveImage,
  save offsets 0x2364..0x3060).
- The ovl_30 debug descriptor tables (dumped below) confirm the gap: the
  main-data rows jump from 80070D0C (ハウスのＬＶ) straight to 80070D38
  (資材残り). Nothing edits or displays 80070D2A.

So the farm-naming glitch is a functional no-op: the 0xFFFF only round-trips
through the save image (save offset 0x238A). The contrast rows are real,
though — the same +0x12 offset in the other two new-game structures is live
(see the ovl_30 table dump below).

All other text-entry destinations were checked the same way. The +0x12 slot
(index 9) is dead for every one of them:

| Caller | Destination | +0x12 address | Verdict |
|---|---|---|---|
| ovl_27 new game (player) | gPrimaryObjectBase 80071A00 | 80071A12 | **live: 体力 stamina** |
| ovl_27 new game (dog) | D_800759E8 | 800759FA | **live: 犬・愛情度 dog affection** |
| ovl_27 new game (farm) | D_80070D18 | 80070D2A | dead padding |
| ovl_11 800E729C / 800F0474 (horse & animal rename scratch) | D_801295D0 | 801295E2 | dead; the aggregate copy moves exactly nine halfwords |
| ovl_11 80114604 (animal purchase naming) | D_8012D63C | 8012D64E | dead; copy loop is `for (i = 0; i < 9; i++)` |
| quantity_purchase_interaction.c | gOvl11QuantityPurchaseNameText 8012D6EC | 8012D6FE | dead; no access anywhere |

Evidence for the ovl_30 labels (decoded as Shift-JIS from the original
ovl_30.bin, load base 0x8012DDE8, table at 80134018, 20-byte rows):

- gOvl30MainDataMenu rows: 年数 80070CF0, 季節 80070CF2, 日 80070CF4,
  曜日 80070CF6, 時間 80070CF8, 分 80070CFA, weather 8007AD00/02,
  家のＬＶ 80070D06, 家畜小屋のＬＶ 80070D08, 鶏小屋のＬＶ 80070D0A,
  ハウスのＬＶ 80070D0C, 調理道具 80070D04, 資材/飼葉/魚の餌/鶏の餌
  80070D38/3A/3E/40, 嫁 fields 8007AFA4+.
- gOvl30PlayerDataMenu rows: 体力 80071A12, 最大体力 80071A14, 疲労度
  80071A16, リュックＬＶ 80071A1E, 幸せ指数 80071A58, お金 80071A5C,
  tool experience 80071A40+.
- gOvl30PetDataMenu rows: 犬・愛情度 800759FA, 馬・愛情度 80075AEA.

The downstream behaviour of the two live writes is unchanged from the
original audit's limits: stamina is consumed by Ovl11ApplyStatusDelta
(signed lh reads, clamped against the +0x14 limit), and dog affection is read
through record pointers, not constant addresses, so its 0xFFFF (−1 signed /
65535 unsigned) handling still needs the runtime test. Nothing here promotes
the farm write to a useful primitive.

## 7. Follow-up: scene 0 is a retail DEBUG MODE scene-select; the debug
   menu cluster is unreachable without corruption

The strongest new fact: **scene 0 is a fully-built debug scene-select menu
(ovl_08), and its row 19 selects scene 17 — the ovl_30 date editor.**

The menu (ovl_08.bin, loads at 0x800B7E20; all data self-contained at
0x800B7E24..0x800B8608):

- Heading `ＤＥＢＵＧ　ＭＯＤＥ`, subheading `「プログラム 選択」`, instruction
  `ＳＴＡＲＴ＝プログラム実行` (START = run program). D-pad moves a 32-row
  cursor; START runs the selected row's scene.
- Table gOvl08SceneSelectEntries at 800B8508, 32 rows of {s32 sceneId;
  s32 label}: row 0 scene 0 `ＤＥＢＵＧ`; row 1 scene 1 `ＰＤＡサンプル`;
  row 2 scene 13 `自分の家`; rows 3-12 scenes 3-12 (minigames 01-05,
  `ヒ・ミ・ツ`, セーブ, ロード, ＰＤＡ, 家畜交換); rows 13-17 scene 13
  (春/夏/秋/冬の家, タイトル — rows 13-16 also set 0x80000/0x40000/0x20000/
  0x10000 in D_8006C838+0x10); row 18 scene 16 `スタッフロール`;
  **row 19 scene 17 `ＭＧデバッグ画面`;** row 20 scene 18
  `メモリーカード云々`; rows 21-22 scenes 19-20 `謎`/`謎2`; rows 23-31
  sceneId -1 (disabled).

Reachability (from the matched scene dispatch in game_loop.c, cross-checked
against the original EXE's jtbl_80010008 and every `jal RequestSceneChange`
site in the original binaries):

- Boot: `main` sets gCurrentSceneId = 0xD (13, the title/ovl_27 scene).
- Scene 0 is requested only by ovl_10_func_800B889C (scene 1, `ＰＤＡサンプル`)
  and by the ovl_31 memory-card utility (scene 18, START press in its state
  0). Scenes 1 and 18 are requested by **nothing** outside the scene-select
  table itself. {0, 1, 18} is a closed dev cluster.
- Scene 17's update entry (D_8012F084 / Ovl30UpdateDebugMenu) is installed by
  exactly one place in the whole binary: main's case 17.
- The ovl_11 area-mode `case 6 → RequestSceneChange(17)` branch stays dead:
  an exhaustive store scan of WORK.state (0x800A0490, D_8007AFF0+0x254A0)
  across the EXE and every overlay finds the written values
  {0,2,3,5,12,15,17,18,19,20,21,22,23,24,25,26,27,30,31} — **never 6**.
  (The one variable-valued store, interaction_volume_activation.c:141, only
  writes on its `result == 1` branch.)

So the audit's earlier phrasing can be sharpened: there is no button code,
  and the debug menu is not reachable through the retail scene graph at all.
  Reaching it needs gCurrentSceneId (0x8005E39C, GP-relative) or WORK.state
  (0x800A0490) to be corrupted to 0/17 or 6 respectively — and no known input
  primitive writes either location (the text-entry bug writes 0xFFFF to
  stamina/affection/dead padding only).

Supporting fact established on the way (corrects the implied overlay bases):
  the overlay load destinations are the EXE .data words D_80010098 =
  0x800B7E20 and D_8001009C = 0x8012DDE8; no code anywhere rewrites them,
  and every overlay loads raw at one of those two bases. Scene 15 (the
  save-return scene) nominally loads ovl_11 through D_8001009C, but the
  card-write/read cases 7/8 never clear the one-shot flag D_8005E288, so the
  retail save-return path (gameplay → scene 7 → scene 15) always skips that
  load and keeps ovl_11 where scene 2/19 loaded it, at 0x800B7E20 via
  D_80010098. The scene-15 load branch is dead in retail for the same reason
  the mode-6 branch is: nothing reaches it with the flag cleared.

## 8. Follow-up: approach 3 (script hijack) — sinks re-confirmed, no input
   chain found

- Program base/cursor 801291B0/801291B4 are written only by the object
  archive pipeline (Ovl11StartObjectArchiveLoad / Ovl11PollObjectArchiveLoad
  reset both to gOvl11ObjectProgramRecords after the read), and read by the
  interpreter driver Ovl11RunObjectRecordProgram.
- The sinks are as the audit described, now with exact sizes: the opcode
  handler table gOvl11ObjectOpcodeHandlers at 800B975C holds 0x1C4/4 = 113
  entries and is indexed by the record's unchecked opcode byte; branch
  targets are u8 record indices applied to gOvl11ObjectProgramBase; the
  archive offset table gOvl11ObjectArchiveOffsets (801240D8, 0x710 bytes) is
  indexed by the current object's id.
- The state-field service remains script-driven: every caller of
  Ovl11UpdateStateField is an opcode-handler-shaped function
  (ovl_11_func_800E648C/800E69B8/800E698C/800EB760/800EBF18/800ED4A8), and
  the value slots they read (gOvl11ValueSlots, 80129560, 0x50 bytes) are
  written by script handlers. One writer is menu-selection-driven
  (ovl_11_func_800E7D4C stores Ovl11ResolveCellItemState(selected) into a
  slot) — that gives a player influence over a *value*, but the *field id*
  still comes from the script record, and the section-5 scan found no
  calendar-field writes in the retail scripts.
- The only input-driven memory corruption found anywhere (the text-entry
  +0x12 write) reaches dead padding, stamina and dog affection — none of the
  interpreter state, the record buffer, the object id, the scene id, or the
  mode word.

Bottom line for the speedrun goal: with no input→script-control chain and no
reachable debug menu, the realistic paths to a calendar change remain (a) an
as-yet-undiscovered corruption primitive, or (b) a save-file edit (out of
scope for a controller-only route).

## Practical priority (updated)

1. **Test the section-9 chain first** — it is the only input-driven
   memory-safety primitive found. At a naming screen: cursor to 0x24 (or
   0x40), L1+L2, X; watch the name buffer and the D_8005F0C8 fallback load
   in 80018B98 during name-printing dialogues. Enumerate _spu_tsa values by
   naming in different areas.
2. First runtime test unchanged but narrower: only the player (80071A12
   stamina) and dog (800759FA affection) writes can do anything; the farm
   write is proven inert. Watch those two addresses plus the first
   status-delta clamp.
3. Debug-menu reachability is now settled: no input path exists. Stop
   searching for a button code; the remaining route is a corruption
   primitive that hits gCurrentSceneId (0x8005E39C) or WORK.state
   (0x800A0490) — the section-9 stack-overflow variant passes through both.
4. For approach 3, the open surface is a *producer*: any input-reachable
   write into the record buffer, the program base/cursor, the object id, or
   the scene id. Everything downstream of those (opcode/branch/field
   indices) is already wide open. The section-9 stack overflow passes
   through the asset scratch where event programs load; a
   terminating-at-a-useful-depth variant would corrupt program records and
   hand control to the interpreter's unchecked indices.
5. Verify the desired ending/event trigger independently of the displayed
   date. A raw date write skips daily side effects and may miss exact-date
   triggers or leave required flags/counters inconsistent.

## 9. Follow-up: a real input-driven OOB chain in the name-entry palette
   (the "kana-table" read)

**A player-controllable out-of-bounds read puts arbitrary .data/.bss
halfwords — including live libspu state — into any name, and the message
renderer then treats those halfwords as string-reference control codes.**

### The OOB read

`GetTextEntryCodeSet1` (exe:800245C8) computes the character code as
`gTextEntryCodeTableSet1[row * 10 + column]`, with `row = cursor / 14` and
`column = cursor % 14`. **The table has 28 entries** (0x800559D4, size 0x38;
bounded by math_errno at 0x80055A14), but the cursor grid is 14 columns wide,
so rows 3+ can compute indices up to 63.

The set-1 movement bounds confine the cursor to in-bounds cells — but the
character-set toggle does not:

```c
case 0x40: {                       /* X pressed */
    d0 = (s16)gTextEntryCursor;
    ...
    if (d0 != 0xD) { ... append via GetTextEntryCode ... }
    else {
    case 0x5:                      /* Duff's-device fall-through: L1+L2 */
    case 0xA:                      /* R1+R2 */
        gTextEntryCharacterSet = ((u16)gTextEntryCharacterSet + 1) & 1;
    }
```

Buttons 0x5/0xA are **L1+L2 / R1+R2** (the game's button word is
`byte3 | (directionByte << 8)`: 0x01=L2, 0x04=L1, 0x02=R2, 0x08=R1,
0x40=X, 0x20=CIRCLE, 0x800=START). The toggle **does not reset or revalidate
the cursor**, and the set-0 grid is wider (row bounds per column: 5,4,4,4,4,5,
4,4,4,4,3,2,2,5; column bounds per row: 13,13,12,10,9,13). So:

1. In the hiragana set, move the cursor to any cell whose set-1 index is
   past 28 — e.g. (row 2, col 8) = cursor 0x24, or (row 4, col 8) = 0x40.
2. Press **L1+L2** (or R1+R2) — same frame — the set flips to 1, cursor stays.
3. Press **X** — the append calls `GetTextEntryCodeSet1(col, row)` and writes
   the OOB halfword into the name buffer.

Reachable OOB indices: 28..51 (plus 33/43/53 from column 13 rows 2-4 which
are reachable inside set 1 itself). The harvested halfwords are EXE
data/bss at 0x80055A0C..0x80055A54:

| index | cursor | address | value |
|---|---|---|---|
| 28 | 0x24 | 0x80055A0C | **0x7350 (static; bit 0x4000 set!)** |
| 29-31 | 0x25-0x27 | 0x80055A0E-14 | 0x000B / 0x0000 / 0x0047 (gap) |
| 32-35 | row 3 | 0x80055A14-1C | math_errno / math_err_point (runtime) |
| 36-47 | rows 3-4 | 0x80055A1C-34 | libspu pointers 0x1F801C00 / 0x1F8010C8 / 0x1F8010F0 / 0x1F801014 |
| **48** | **0x40** | **0x80055A34** | **_spu_tsa — libspu's live 16-bit SPU transfer address** |
| 49-51 | 0x41-0x47 | 0x80055A36-40 | _spu_tsa high (0) / _spu_transMode / _spu_addrMode |

`_spu_tsa` is written by `SpuSetTransferStartAddr` (800329B0),
`SpuWritePartly` (80032A74) and libsnd's `_spu_t` (80026824) on every sound
bank transfer — and the game loads per-area sound banks
(`StepSoundBankBodyTransfer` → `SsVabTransBodyPartly`), so the last
transferred SPU address is **player-influenced (which area you are in)** and
lands in the name at cursor 0x40.

### The sink: the renderer trusts name halfwords as reference codes

Every name character with bit 0x4000 is treated as a string reference by the
message renderer (the names are substituted into dialogues — the fixed EXE
text bank contains 0x400A (player name), 0x400C (dog), 0x400D (farm), and
25 uses of slot 0):

- `func_8001A284` resolves ids 10-16 to the name buffers (case 10 = the
  **player's own name** at 80071A00, 12 = dog, 13 = farm).
- Everything else falls through to `D_8005F0C8[code & 0xFFF]` — **a table
  whose real extent is 0x8005F0C8..0x8005F2B8 (124 words, next symbol
  gAuxSpriteSource), of which only slots 0-10 are ever written**
  (func_80017A08 clamps ids >= 0xB to 0xA). Ids 0x07C..0xFFF read **live
  .bss as pointers**: the aux-sprite OT/packet buffers (8005F2E8-800605E8),
  the pad input state (80060000+), the sprite packet pool cursor, etc.
- The draw path `func_80018B98` (80018B98: `work1 & 0x4000` → resolve →
  **recurse with no depth limit**; arg8 only gates typewriter pacing), the
  counters `func_8001929C` / `func_8001945C`, and the width measurer
  `func_800198E0` (`(code & 0xF000) == 0x4000`) all follow those pointers.

So a name character of 0x4xxx-0x7xxx becomes:

- **id 0x00A** (the harvested value ends in 0x00A): func_8001A284 case 10
  resolves to the player's own name → the walk hits the same code again →
  **unbounded recursion → stack overflow**, smashing downward from
  0x801FFF80 through the graphics heap, the asset scratch (where event
  programs are loaded), the ovl_11 overlay, the game state (calendar at
  80070CF0), gCurrentSceneId (8005E39C) and the scene function pointer
  (8005E384), before crashing.
- ids 0x00B-0x010: another name (horse/dog/farm) rendered inside yours —
  bounded recursion; a mutual pair (player name carries 0x4x0C, dog name
  carries 0x4x0A) is also unbounded.
- ids 0x000-0x009: the runtime string slots (set by the save menu;
  otherwise 0 → skipped).
- ids 0x07C-0xFFF: OOB reads of live .bss pointers → the renderer walks
  sprite packets / pad state as text; packet halfwords like tpage words
  (0xE1xx) themselves carry bit 0x4000, so the walk can cascade through
  further OOB table reads — a self-sustaining pointer-chaos walk with
  unbounded recursion and unbounded glyph-packet writes (func_80019070
  writes raw packets at an advancing cursor with no bound of its own).

### Status

Everything above is verified statically from the original EXE/overlay bytes
and the matched sources (table sizes, bounds tables, the toggle fall-through,
the button mapping, the resolver/fallback paths, the missing depth limit,
_spu_tsa's writers). What is NOT yet established:

- Whether _spu_tsa is in [0x4000, 0x7FFF] at any naming screen in practice
  (it depends on the accumulated bank layout; the initial load address is
  0x1010 and banks advance it).
- Whether the 0x7350 static value (id 0x350 → the word at 0x8005FE08, in the
  aux-sprite packet area) is nonzero in practice.
- Whether any pointer-chaos walk terminates anywhere useful.

The runtime tests are cheap: at the new-game naming, set the cursor to 0x24
(or 0x40), press L1+L2, press X, and watch the name buffer (80071A00) plus a
breakpoint on the D_8005F0C8 fallback load in 80018B98 during the first
dialogue that prints the name.

This is the first input→memory-safety chain found in this audit: it reads
live .data/.bss into a name, and the renderer follows the result as pointers
with unbounded recursion. It is not yet a controlled write — but it is the
class of primitive approach 3 needed, and its self-reference variant
(id 0x00A) is a guaranteed stack-overflow engine.
