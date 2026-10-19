# `ovl_11_func_800D2DB8`: shared-flag helper association

The verified helper supports a low-confidence association with the existing
`D_80123754` setter/getter family, specifically `800D12B8` and `800D196C`.
It does not establish the family's original translation-unit boundaries.

## Evidence

- Verified source: `src/overlays/ovl_11/ovl_11_func_800D2DB8.c`.
  It reads `D_80128D6C`; when the word equals 1, it tries `800BDCE8` against
  the `D_8007AFF0` work area before falling back to `800BDB88`.
  Supplied verified source/evidence identity:
  `7d0db234f5bc2d53c39ac59a3a9e872842d7a929510b25fa7f52a6279230e68a`.
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800D12B8/ovl_11_func_800D12B8.s`:
  `800D13B0` clears `D_80128D6C` for object ID `0x15B`; `800D13B8` sets it
  to 1 otherwise. The same function calls `800D2DB8` at `800D14BC`,
  `800D15B0`, `800D16A8` and `800D1848`.
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800D196C/ovl_11_func_800D196C.s`:
  `800D1B20` calls `800D2DB8`. Both callers are already listed in the
  `D_80123754` family in `notes/file-groupings.md`.
- Searching the original assembly under `build/ovl_11/asm/nonmatchings/`
  for `D_80128D6C` finds only the writer `800D12B8` and reader `800D2DB8`.
  This narrowly shared control word, together with the calls from the
  existing family, is the new grouping evidence.

## Limits

The original accesses are absolute, so they do not prove a TU-local definition
or static linkage. Shared control state can cross file boundaries. The target
is beyond the currently listed run, and no shared compiler quirk or adjacent
call edge establishes the intervening functions as members. If the flag was
private to the original file, the writer and reader belonged together; that
privacy remains unproven. Record the helper only as a low-confidence candidate,
not a contiguous range extension and not a merger with its distant probe
callees' groups. The shared `D_8007AFF0` engine work area alone is not evidence
of common TU ownership.
