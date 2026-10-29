# Memory-access bug research — findings index (BTN / SLUS-01115)

Controller = reachable with pad only, in play. Save = needs crafted/loaded memory card.
"Confirmed" = verified in source and/or runtime; "demonstrated" = observed via runner.

## A. Name-entry (text_engine.c / text_entry.c) — strongest set
1. **OOB read** (controller, confirmed + demonstrated in play)
   - Writer `SetTextSourcePointer` clamps index to 0..10; four readers
     (text_engine.c:848/1121/1199/1300) index `gTextSourceAddresses[*ptr & 0xFFF]`
     UNCLAMPED (11-word table). A name halfword with bit 0x4000 and id 0xB..0xFFF
     reads past the table.
   - Trigger: L1+L2 charset toggle with the cursor in the tall set-0 grid -> set-1
     table index lands past its 28-entry end -> harvested glyph is a raw memory word.
   - Window contents (note §15): math_errno / math_err_point / _spu_* -> a non-zero
     pointer's low halfword carries bit 0x4000, so it is a REFERENCE code the reader
     follows via gTextSourceAddresses[id] (OOB) -> state-dependent wild pointer.
   - Observed value 0x7350 (id 0x350) resolved to 0 (benign), but the mechanism is real.
   - Note: name-entry-charset-toggle-oob.md §11-15
2. **OOB write** (controller, fully triggerable, confirmed)
   - Confirm path: ProcessTextEntryInput writes gTextEntryBuffer[Length+1]=0xFFFF with
     NO length cap. Buffer holds 8; clear touches 9; confirm writes index 9.
   - Target 0x80071A12 = gPrimaryObjectBase+0x12 (a LIVE player-object field, note §14),
     2-byte deterministic corruption, adjacent to gMenuGridLayoutIndex (0x80071A1E).
   - Trigger: enter 8 chars, cursor to OK cell (0x53), CROSS.
   - Note: name-entry-charset-toggle-oob.md §13-14
3. **Self-recursion crash** (save-gated, confirmed UNBOUNDED)
   - MeasureTextSpanLength (text_engine.c:1175) recurses per 0x4000 reference, passing
     `arg2 ? arg2-index : 0`; ALL 6 top-level call sites pass arg2==0, so the limit
     never engages. Name buffer starting with 0x400A (id 0xA == player name) ->
     ResolveTextSource returns the name buffer -> recurses on itself -> infinite ->
     PSX downward stack smash -> trap.
   - Trigger: crafted save with name=0x400A (file 0x3341) + in-game name dialogue.
   - Note: name-entry-charset-toggle-oob.md §16

## B. Other player-triggerable
4. **Interaction-menu grid-nav OOB read** (controller, confirmed, benign 1-byte)
   - interaction_menu.c:1035 links[4][3] indexed by gMenuGridLayoutIndex; a 16-slot
     layout (index 3) makes links[3][3] 1 byte past the row; the bogus byte then
     indexes the 17-row gOvl11MenuGridNavigation (2nd OOB if >16).
   - Note: interaction-menu-grid-nav-oob.md

## C. Ruled out (verified safe this session)
- choices[20] (eligible_record_selection): loop bound <=20, count++ <=1/iter.
- money (record_trade_interaction.c:184): s16*s16 fits s32; quantity bounded.
- gTextNumberBuffer: real size 4 hw (next sym 0x8005E498); write fits (decomp .comm 2
  is an under-sizing, not a game bug).
- WriteWeekdayNameText: weekday 0..6 bounded.
- actuator sequences: internal preset/slot indices bounded.
- Ovl11GetItemSlotRecord pool index: 0..63 in-bounds for valid cursor 9..16.

## D. Latent / data-driven (not clearly player-triggerable)
- clutList[12] (sprite_animation.c:944) >10 unique CLUTs.
- area_resource_load.c: CD-resource-size memcpy into fixed buffers.
- InsertRecordListText: variable memmove/memcpy (bounded record pool).
- sound_driver.c:485/505: memcpy len from gSoundArchiveOffsets (fixed CD data).

## Main open blocker
- Demonstrating A3 (self-recursion) needs a REAL save (playable world) with name
  grafted to 0x400A, then the in-game name dialogue render. In-game navigation to a
  save point / the name-dialogue NPC is the current blocker (no single early-farm
  button opens the save; probed SELECT/START/L1/L2/R1/R2/TRI/SQUARE).
