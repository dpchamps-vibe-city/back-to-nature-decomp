# ovl_11_func_80120CC4 — human decision needed

- **Parked:** 2026-10-13T13:12:44.800Z
- **Reason:** escalation-exhausted
- **Escalation reached:** gpt-6.1-sol
- **Source:** `src/overlays/ovl_11/ovl_11_func_80120CC4.c` (INCLUDE_ASM restored)
- **Preserved attempt:** preserved the best measured program (EXACT at 155/155 words, measured 2026-10-13T13:02:14, first scored from build/schedulingPolicyProbe/ovl_11_func_80120CC4/input-barrier.c) rather than the source left on disk, which measured [0, 0, 1, 0] at 154/154 words, measured 2026-10-13T10:37:18

## What the loop needs

Every tier on the escalation ladder returned without a byte-exact match. The function
needs either a new structural hypothesis or a policy decision that the ladder cannot
make on its own. The preserved attempt and the oracle report below are the starting
point.

## Policy findings

- none recorded

## Oracle report for the preserved attempt

```
Oracle: ovl_11_func_80120CC4 verdict MATCH — 155/155 words (100%).
```

## Preserved attempt

```c
#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"

static __inline__ s32 texture_y(s32 row) {
    return row + 0x100;
}

s32 ovl_11_func_80120CC4(Ovl11TextureCoordinates *arg0, s32 arg1) {
    s32 var_a0;
    u16 temp_a1;
    s32 temp_a0;
    u16 temp_v1;
    s32 temp_a0_2;
    s32 temp_a2;
    s32 temp_a3 = 0;
    s32 temp_v1_2;
    register u32 u_left;
    s32 v_top;
    s32 v_top_coord;
    u32 clut_shift;
    s32 season = D_80070CF2;

    switch (arg1) {
    case 0x26:
        var_a0 = 0;
        break;
    case 0x25:
        var_a0 = 1;
        break;
    case 0x24:
        var_a0 = 2;
        break;
    case 0x31:
        var_a0 = 3;
        break;
    case 0x34:
        var_a0 = 4;
        break;
    case 0x23:
        var_a0 = 5;
        break;
    case 0x22:
        var_a0 = 6;
        break;
    case 0x35:
        var_a0 = 7;
        break;
    case 0x2A:
        var_a0 = 8;
        break;
    case 0x2B:
        var_a0 = 9;
        break;
    case 0x32:
        var_a0 = 0xA;
        break;
    case 0x29:
        var_a0 = 0xB;
        break;
    case 0x27:
        var_a0 = 0xC;
        break;
    case 0x28:
        var_a0 = 0xD;
        break;
    case 0x33:
    case 0x53:
    case 0x178:
        var_a0 = 0xE;
        break;
    case 0x54:
    case 0x179:
        var_a0 = 0xF;
        break;
    case 0x2F:
        var_a0 = 0x10;
        break;
    case 0x2C:
        var_a0 = 0x11;
        break;
    case 0x2D:
        var_a0 = 0x12;
        break;
    case 0x30:
        var_a0 = 0x13;
        break;
    case 0x2E:
        var_a0 = 0x14;
        break;
    case 0x3A:
        var_a0 = 0x15;
        if (season == 3) {
            var_a0 = 0x25;
        }
        break;
    case 0x40:
        var_a0 = 0x16;
        if (season == 2) {
            var_a0 = 0x17;
        }
        break;
    case 0x3E:
        var_a0 = 0x18;
        if (season == 3) {
            var_a0 = 0x26;
        }
        break;
    case 0x3F:
        var_a0 = 0x19;
        if (season == 3) {
            var_a0 = 0x24;
        }
        break;
    case 0x175:
        var_a0 = 0x1A;
        if (season == 3) {
            var_a0 = 0x23;
        }
        break;
    case 0x3B:
    case 0x3C:
        var_a0 = 0x1B;
        if (season == 3) {
            var_a0 = 0x27;
        }
        break;
    case 0x16C:
    case 0x16F:
        var_a0 = 0x1C;
        if (season == 3) {
            var_a0 = 0x28;
        }
        break;
    case 0x16D:
    case 0x170:
        var_a0 = 0x1E;
        if (season == 3) {
            var_a0 = 0x2A;
        }
        break;
    case 0x16E:
    case 0x171:
        var_a0 = 0x1D;
        if (season == 3) {
            var_a0 = 0x29;
        }
        break;
    case 0x3D:
        var_a0 = 0x1F;
        if (season == 3) {
            var_a0 = 0x2B;
        }
        break;
    case 0x172:
        var_a0 = 0x20;
        if (season == 3) {
            var_a0 = 0x2C;
        }
        break;
    case 0x173:
        var_a0 = 0x22;
        if (season == 3) {
            var_a0 = 0x2E;
        }
        break;
    case 0x174:
        var_a0 = 0x21;
        if (season == 3) {
            var_a0 = 0x2D;
        }
        break;
    case 0:
    default:
        return 0;
    }
    temp_a1 = D_80128764[var_a0];
    temp_a0 = 1;
    temp_v1 = (u32) (temp_a1 & 0xE00) >> 9;
    temp_a2 = temp_v1;
    temp_a2 <<= 5;
    temp_a2 |= 0x1F;
    var_a0 = (temp_a1 & 0x1C0) / 2;
    temp_a3 = var_a0 + 0x11F;
    /* Diagnostic only: order bottom -> left -> top; needs owner exception. */
    __asm__ volatile ("" : "=r" (temp_v1), "=r" (temp_a0), "=r" (var_a0) : "0" (temp_v1), "1" (temp_a0), "2" (var_a0));
    u_left = temp_v1 << 5;
    __asm__ volatile ("" : "=r" (var_a0) : "0" (var_a0), "r" (u_left));
    arg0->u1 = u_left;
    arg0->u3 = u_left;
    var_a0 += 0x100;
    arg0->u0 = temp_a2;
    arg0->v0 = temp_a3;
    arg0->v1 = temp_a3;
    arg0->u2 = temp_a2;
    arg0->v2 = var_a0;
    arg0->v3 = var_a0;
    clut_shift = (u32) temp_a1 >> 1;
    v_top = temp_a1 & 0x1F;
    temp_v1_2 = (clut_shift & 0x10) + 0x3C0;
    temp_a0_2 = v_top + 0x1E0;
    arg0->clut = (temp_v1_2 >> 4) | (temp_a0_2 << 6);
    return temp_a0;
}
```
