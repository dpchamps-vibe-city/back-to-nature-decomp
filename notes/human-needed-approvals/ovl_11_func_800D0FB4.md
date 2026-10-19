# ovl_11_func_800D0FB4 — human decision needed

- **Parked:** 2026-10-19T16:48:56.700Z
- **Reason:** escalation-exhausted
- **Escalation reached:** gpt-6.1-sol
- **Source:** `src/overlays/ovl_11/ovl_11_func_800D0FB4.c` (INCLUDE_ASM restored)
- **Preserved attempt:** preserved the best measured program ([0, 0, 0, 1] at 185/187 words, measured 2026-10-19T15:49:31, first scored from build/preparation/ovl_11_func_800D0FB4/0491d012cb348be43361852200a441bb11fed8350599e9b00ac7de92c0c82254/draft.c) rather than the source left on disk, which measured [0, 0, 0, 1] at 185/187 words, measured 2026-10-19T16:48:49

## What the loop needs

Every tier on the escalation ladder returned without a byte-exact match. The function
needs either a new structural hypothesis or a policy decision that the ladder cannot
make on its own. The preserved attempt and the oracle report below are the starting
point.

## Policy findings

- none recorded

## Oracle report for the preserved attempt

```
Oracle: ovl_11_func_800D0FB4 verdict UNKNOWN — 0/0 words (0%).
```

## Preserved attempt

```c
#include "common.h"
#include "game_types.h"

s32 ovl_11_func_800BE208(s32 *arg0, Ovl11CollisionTag arg1, u16 arg2, u16 arg3, void **arg4, s32 *arg5, s32 arg6, s32 arg7);

void ovl_11_func_800D0FB4(void) {
    Ovl11ResetRecordB4 *p;
    s32 i;
    u8 *status_base;

    p = (Ovl11ResetRecordB4 *)&D_800759E4;
    if ((s16)p->state == 6 && D_80070D0C == 0) {
        p->position[0] = -1800;
        p->position[1] = 0;
        p->position[2] = 3100;
        p->state = 1;
    } else {
        if (((s16)p->state == 1 || (s16)p->state == 6) &&
            ovl_11_func_800BE208(p->position, *(Ovl11CollisionTag *)&p->tag, 0, 0, 0, 0, 0, 0x30)) {
            if ((s16)p->state == 1) {
                p->position[0] = -1800;
                p->position[1] = 0;
                p->position[2] = 3100;
            } else {
                p->position[0] = -1800;
                p->position[1] = 0;
                p->position[2] = 3100;
                p->state = 1;
            }
        }
    }
    p = (Ovl11ResetRecordB4 *)&D_80075AD4;
    if ((s16)p->state == 1 &&
        ovl_11_func_800BE208(p->position, *(Ovl11CollisionTag *)&p->tag, 0, 0, 0, 0, 0, 0x30)) {
        p->position[0] = 5000;
        p->position[1] = 0;
        p->position[2] = 2700;
    }
    for (i = 0; i < 20; i++) {
        p = (Ovl11ResetRecordB4 *)&((Ovl11ResetRecordB8 *)&D_800749F4)[i];
        if ((s16)p->state == 1 &&
            ovl_11_func_800BE208(p->position, *(Ovl11CollisionTag *)&p->tag, 0, 0, 0, 0, 0, 0x30)) {
            p->position[0] = i * 400 - 1000;
            p->position[1] = 0;
            p->position[2] = 2200;
        }
    }
    for (i = 0; i < 10; i++) {
        p = &((Ovl11ResetRecordB4 *)&D_800742EC)[i];
        if ((s16)p->state == 6 &&
            (status_base = (u8 *)(&D_800742EC - 0x1EAD), *(u16 *)(status_base + 0x44D4) == 0)) {
            p->position[0] = -5000;
            p->position[1] = 0;
            p->position[2] = i * 400 - 7600;
            p->state = 1;
        } else {
            if (((s16)p->state == 1 || (s16)p->state == 6) &&
                ovl_11_func_800BE208(p->position, *(Ovl11CollisionTag *)&p->tag, 0, 0, 0, 0, 0, 0x30)) {
                if ((s16)p->state == 1) {
                    p->position[0] = -5000;
                    p->position[1] = 0;
                    p->position[2] = i * 400 - 7600;
                } else {
                    p->position[0] = -5000;
                    p->position[1] = 0;
                    p->position[2] = i * 400 - 7600;
                    p->state = 1;
                }
            }
        }
    }
}
```
