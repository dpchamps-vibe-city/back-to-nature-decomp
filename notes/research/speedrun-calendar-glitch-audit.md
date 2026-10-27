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

## Practical priority

1. Reproduce the length-8 name corruption on the retail image and inspect its
   first downstream consumers.
2. Determine whether gameplay can reach debug mode 6 / scene 17 without RAM
   editing. Do not assume a button code exists.
3. Look for an actual input-to-event-pointer/buffer corruption chain; existing
   script commands could then suffice for a date change or event sequence
   break without full native ACE.
4. Verify the desired ending/event trigger independently of the displayed
   date. A raw date write skips daily side effects and may miss exact-date
   triggers or leave required flags/counters inconsistent.
