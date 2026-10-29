# Interaction-menu grid navigation OOB read (links[4][3] with index 3)

## Summary
A 1-byte out-of-bounds read in the in-game item/satchel menu's grid navigation.
`gOvl11MenuGridNavigation[cursor].links[direction][gMenuGridLayoutIndex]` indexes a
`u8 links[4][3]` field, but `gMenuGridLayoutIndex` can be 3 (a 16-slot layout,
`2^(index+1)` slots), where the valid second index is only 0..2. For `direction==3`
(RIGHT) and `index==3`, `links[3][3]` is 1 byte past the 16-byte struct row, reading
the low byte of the next row's `x`.

## Evidence (source)
- struct: include/globals_override.h:4098-4101
    struct Ovl11MenuNavigation { s16 x; s16 y; u8 links[4][3]; };  // 16 bytes
- access: src/overlays/ovl_11/modules/interaction_menu.c:1035
    gOvl11MenuGridCursorSlot =
        gOvl11MenuGridNavigation[gOvl11MenuGridCursorSlot].links[direction][gMenuGridLayoutIndex];
  inside Ovl11UpdateMenuGridNavigation (line 1006), guarded by
  gOvl11MenuGridMessageState==255 and IsMessageWindowStatusFinal().
- direction is always 0..3 (from D-pad: 0x1000->0, 0x4000->1, 0x8000->2, 0x2000->3),
  so the first dimension is safe.
- gMenuGridLayoutIndex: drawn as pow_int(2, gMenuGridLayoutIndex+1) slots
  (interaction_menu.c:620); the satchel calls Ovl11UpdateMenuGridNavigation(0x40).
  index 3 == a 16-slot layout.

## Why it is OOB
links[4][3] is 12 bytes. links[d][3] == offset d*3+3:
  d=0 -> off 3 (links[1][0]), d=1 -> off 6 (links[2][0]), d=2 -> off 9 (links[3][0]),
  d=3 -> off 12 = 1 byte past the row (next row's x low byte).
So RIGHT in a 16-slot layout reads out of bounds.

## Trigger / reachability
- Player-driven: open the item/satchel grid and press RIGHT when the grid is in the
  16-slot layout (gMenuGridLayoutIndex==3).
- UNCONFIRMED at runtime whether the game actually uses a 16-slot layout
  (needs the in-game satchel open; the layout index is set in a stub). If the max
  layout is 8 slots (index 2), the OOB never fires.
- Impact: 1-byte OOB read of adjacent row data -> wrong cursor slot value (the read
  byte becomes the next cursor index). Benign (no crash) but a genuine out-of-bounds
  memory access, and the resulting bogus cursor index is then used to index
  gOvl11MenuGridNavigation (17 rows) at interaction_menu.c:1064/1059, which could
  itself go out of the 17-row table if the byte is >16.

## Classification
Latent/conditional player-triggerable OOB read. Lower severity than the name-entry
OOB (1 byte, adjacent data) but a clean example of an unbounded 2-D table index.

## Open
- Confirm gMenuGridLayoutIndex reaches 3 in the satchel (trace 0x... in-game).
- Check the 17-row gOvl11MenuGridNavigation bounds when the bogus cursor is used.
