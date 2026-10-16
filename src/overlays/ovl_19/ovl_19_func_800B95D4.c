#include "common.h"

INCLUDE_ASM("build/ovl_19/asm/nonmatchings/ovl_19_func_800B95D4", ovl_19_func_800B95D4);


/* PARKED by /auto_decompilation_loop on 2026-10-16T07:03:15.344Z.
 * Reason: escalation-exhausted.
 * Escalation reached: gpt-6.1-sol.
 * The best non-matching attempt is preserved verbatim below, disabled.
 * Findings and the decision needed: notes/human-needed-approvals/ovl_19_func_800B95D4.md
 */

#if 0
/* Best non-matching attempt, preserved for the next session. */
#include "common.h"
#include "game_types.h"

s32 func_80012A34(s32 arg0);
s32 func_8001FABC(s16 arg0);
s32 ovl_19_func_800BA2D4(void);
s32 ovl_19_func_800BA33C(u8 *arg0);
s32 ovl_19_func_800BA468(void *arg0, void *arg1);
s32 ovl_19_func_800BA5B4(void *arg0, void *arg1);
s32 ovl_19_func_800BA73C(Ovl19Func800BA73CArg *arg0);
s32 ovl_19_func_800BA750(void *arg0);
void ovl_19_func_800BAC40(Ovl19Func800BAC40Arg *arg0, s32 arg1, s32 arg2, s32 arg3);

typedef struct {
    s16 unk0;
    s16 command;
    s16 angle;
    s16 timer;
    s16 value;
    s16 unkA;
    s32 flag;
    s16 rest[28];
} Ovl19StateRecord;

static __inline__ Ovl19StateRecord *actorAt(s16 *base, s32 index) {
    return (Ovl19StateRecord *)base + index;
}

void ovl_19_func_800B95D4(void) {
    s32 i;
    s32 angle;
    s16 *base;
    s16 *q;
    s16 *other;
    s16 *state;
    s16 *origin;
    s16 *view;
    s32 choice;
    s32 status;
    s32 command;
    s32 one;

    i = 0;
    one = 1;
    base = D_800BF4C0 + 8;
    view = base - 8;
    q = base;
    do {
        Ovl19StateRecord *p = actorAt(base, i);

        angle = 0;
        if (i == 0) {
            q = base + 36;
        }
        other = q;
        status = ovl_19_func_800BA73C((Ovl19Func800BA73CArg *)p);
        origin = base - 8;
        if (status == one && origin[i * 32 + 0x51] == 2) {
            ovl_19_func_800BAC40((Ovl19Func800BAC40Arg *)p, 4, p->angle, 30);
            origin[0x101] = func_8001FABC(0x3B);
            origin[0x101] = func_8001FABC(0x44);
        } else {
        if (p->command == 4) {
            state = D_800BF4C0;
            if (p->timer != p->timer / state[0xBB] * state[0xBB]) {
                continue;
            }
            if (p->timer > 0) {
                if (other[1] == 4 && ovl_19_func_800BA468(other, p) == one &&
                    ovl_19_func_800BA468(p, other) == one) {
                    choice = ovl_19_func_800BA2D4();
                    origin = (s16 *)actorAt(base, choice);
                    other = base;
                    if (choice == 0) {
                        other = state + 0x2C;
                    }
                    angle = (s16)(origin[2] + ovl_19_func_800BA5B4(origin, other));
                    ovl_19_func_800BAC40((Ovl19Func800BAC40Arg *)origin, 9, (s16)(angle - angle / 4 * 4), state[0xC8]);
                    *(s32 *)(origin + 6) = one;
                    ovl_19_func_800BAC40((Ovl19Func800BAC40Arg *)other, 10, other[2], state[0xC9]);
                    *(s32 *)(other + 6) = 0;
                    continue;
                }
            } else {
                if (ovl_19_func_800BA468(p, other) == one) {
                    angle = (s16)(p->angle + ovl_19_func_800BA5B4(p, other));
                    ovl_19_func_800BAC40((Ovl19Func800BAC40Arg *)p, 6, (s16)(angle - angle / 4 * 4), state[0xC7]);
                    p->flag = one;
                    command = 7;
                    if (ovl_19_func_800BA73C((Ovl19Func800BA73CArg *)other) == one) {
                        command = 8;
                    }
                    ovl_19_func_800BAC40((Ovl19Func800BAC40Arg *)other, command, other[2], state[0xC7]);
                    state[0x101] = func_8001FABC(0x3C);
                } else {
                    ovl_19_func_800BAC40((Ovl19Func800BAC40Arg *)p, 5, p->angle, state[0xC6]);
                    p->flag = 0;
                }
                continue;
            }
        }
        if (ovl_19_func_800BA750(p) == one) {
            command = ovl_19_func_800BA33C((u8 *)p);
            switch (command) {
                case 0:
                case 1:
                    angle = (s16)func_80012A34(4);
                    break;
                case 3:
                    angle = view[10];
                    break;
                case 2:
                    if (func_80012A34(101) <= p->value * 100 / 255) {
                        angle = (s16)(view[10] + ovl_19_func_800BA5B4(p, other));
                        angle = (s16)(angle - angle / 4 * 4);
                    } else {
                        angle = p->angle;
                    }
                    break;
            }
            ovl_19_func_800BAC40((Ovl19Func800BAC40Arg *)p, command, angle, D_800BF4C0[0xBC]);
        }
        }
    } while (view += 36, q = base, ++i < 2);
}
#endif
