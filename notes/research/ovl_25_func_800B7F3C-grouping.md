# ovl_25_func_800B7F3C grouping evidence

Verified source/evidence identity: `2eb3fe8f597db3dac0cb6c6a5c1e119d77fe6f0d2465a2436d2e56cfefba1fed`.
The supplied handoff reports byte identity and a passed full finalize gate;
this documentation pass does not repeat that verification.

## Additional support for the existing D_800BCBD8 candidate group

- `src/overlays/ovl_25/ovl_25_func_800B7F3C.c` finishes setup by incrementing
  `((s32 *)&D_8006C838)[0x1123]`, the word at byte offset `0x448C`.
  `build/ovl_25/functions/ovl_25_func_800B7F3C.s` corroborates the load,
  increment and store at `0x800B8084`–`0x800B8090`.
- `src/overlays/ovl_25/ovl_25_func_800B7EB4.c` reads that same word to select
  a callback from `D_800BCBD8`; the established group member
  `src/overlays/ovl_25/ovl_25_func_800B81B4.c` also increments it. This adds
  a second callback writer to the already-recorded shared counter cluster.
- `build/ovl_25/asm/data/3D20.data.s` places `800B7F3C` first in the five-entry
  `D_800BCBD8` table. Its original function listing has size `0x168`, ending
  exactly at the next callback, `800B80A4`; its predecessor is the recorded
  dispatcher `800B7EB4`. Thus the shared state-transition contract reinforces
  the existing table and link-order evidence rather than relying on adjacency
  alone.

This supports retaining `800B7F3C` in the existing medium-confidence candidate
translation-unit group, not extending or merging groups. The shared engine
state base is not a private static or proof of translation-unit ownership;
matching C pointer spellings alone are not independent original-source evidence.
