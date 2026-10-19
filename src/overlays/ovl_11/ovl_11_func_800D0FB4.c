#include "common.h"

INCLUDE_ASM("build/ovl_11/asm/nonmatchings/ovl_11_func_800D0FB4", ovl_11_func_800D0FB4);


/* PARKED by /auto_decompilation_loop on 2026-10-19T16:48:56.700Z.
 * Reason: escalation-exhausted.
 * Escalation reached: gpt-6.1-sol.
 * The best non-matching attempt is preserved verbatim below, disabled.
 * Findings and the decision needed: notes/human-needed-approvals/ovl_11_func_800D0FB4.md
 */

#if 0
/* Best non-matching attempt, preserved for the next session. */
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
#endif
