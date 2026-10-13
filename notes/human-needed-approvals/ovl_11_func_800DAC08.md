# ovl_11_func_800DAC08 — human decision needed

- **Parked:** 2026-10-13T21:00:03.558Z
- **Reason:** escalation-exhausted
- **Escalation reached:** gpt-6.1-sol
- **Source:** `src/overlays/ovl_11/ovl_11_func_800DAC08.c` (INCLUDE_ASM restored)
- **Preserved attempt:** preserved the best measured program (EXACT at 51/51 words, measured 2026-10-13T20:53:56, first scored from build/preparation/ovl_11_func_800DAC08/barrier-vector.c) rather than the source left on disk, which measured [0, 0, 1, 0] at 50/50 words, measured 2026-10-13T19:56:29

## What the loop needs

Every tier on the escalation ladder returned without a byte-exact match. The function
needs either a new structural hypothesis or a policy decision that the ladder cannot
make on its own. The preserved attempt and the oracle report below are the starting
point.

## Policy findings

- none recorded

## Oracle report for the preserved attempt

```
Oracle: ovl_11_func_800DAC08 verdict MATCH — 51/51 words (100%).
```

## Preserved attempt

```c
#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"

s32 ovl_11_func_8012135C();

s32 ovl_11_func_800DAC08(u16 arg0, s16 arg1, s16 arg2, s16 arg3) {
    VECTOR position;
    s32 value;
    s32 origin;
    s32 id;

    id = arg0;
    if (arg1 == 0) {
        value = arg2 * 400 - 0x1068;
        position.vx = value;
        value = arg3 * 400;
        origin = 0x578;
    } else {
        value = arg2 * 400 - 0x578;
        position.vx = value;
        value = arg3 * 400;
        origin = 0x3E8;
    }
    origin -= value;
    position.vy = 0;
    position.vz = origin;
    __asm__ volatile("" : "=r"(id) : "0"(id) : "memory");
    id = (s16)id;
    return ovl_11_func_8012135C(id, position);
}
```
