# ovl_11_func_800F0A58: paired row/record evidence

Documentation handoff: byte-exact, full finalize gate passed. Supplied
source/evidence identity:
`279320b8b201f699364c21be6fb34442b726c768b532f045f313efca3a0ddffe`.
No solver experiments or build-input changes were made for this note.

## New corroboration

The existing D_800A0494 cluster already included this function for its
nine-halfword dual-row scan. The verified source additionally identifies the
selected record as the same row/slot pairing used by
`ovl_11_func_800D666C`:

- `src/overlays/ovl_11/ovl_11_func_800F0A58.c` searches rows 0–1 of
  D_800A0494 and copies `records[row][i]` through D_8007AFF0 into the active
  record at D_8006C838+0x445A.
- `include/globals_override.h` defines the far record view at +0x254C8;
  D_8007AFF0+0x254C8 equals D_800A04B8. Its entry size is 0x1E, with nine
  entries per row.
- `src/overlays/ovl_11/ovl_11_func_800D666C.c` walks the corresponding
  D_800A0494 row through D_8007AFF0+0x254A4 and clears the paired
  `D_800A04B8[row][slot]` record on a match.

Thus the new tie is a shared two-object cluster with consistent row/slot
indexing, not merely another use of the container-wide far-buffer base.
It corroborates the existing candidate grouping. Both objects are externally
visible; this evidence alone does not establish original TU membership or a
TU boundary. The functions are address-apart, so no link adjacency is claimed.

## Function role

After selecting and copying the paired record, 800F0A58 uses `MoveImage2`
to relocate its texture to (0x2C0, 0x1C0) and its CLUT to (0x2C0, 0x1BF),
updates those coordinates in the active copy, and clears its final halfword.
It returns 1 on success; a missing identifier prints the error string and
returns 0. These runtime details are not additional grouping evidence.
