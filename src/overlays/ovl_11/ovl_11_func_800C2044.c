#include "common.h"

INCLUDE_ASM("build/ovl_11/asm/nonmatchings/ovl_11_func_800C2044", ovl_11_func_800C2044);


/* PARKED by /auto_decompilation_loop on 2026-10-21T04:53:28.055Z.
 * Reason: escalation-exhausted.
 * Escalation reached: gpt-6.1-sol.
 * The best non-matching attempt is preserved verbatim below, disabled.
 * Findings and the decision needed: notes/human-needed-approvals/ovl_11_func_800C2044.md
 */

#if 0
/* Best non-matching attempt, preserved for the next session. */
#include "common.h"
#include "game_types.h"

s32 ovl_11_func_800C3EE8(Ovl11AnimationObject *, u16 *);
void ovl_11_func_800C3F6C(Ovl11C3F6CObjectView *, u16 *);
void ovl_11_func_800C2A98(Ovl11EventObject *);
s32 ovl_11_func_800C3838(Ovl11C3B78Arg0 *, s32, s32);
void ovl_11_func_800C32C8(Ovl11AnimationObject *, s32, s32, s32);
void ovl_11_func_800C3B78(Ovl11C3B78Arg0 *, Ovl11C3B78Arg1 *);
void ovl_11_func_800C2C74(Ovl11C3B78Arg0 *);
void ovl_11_func_800C1F94(Ovl11AnimationObject *, u16 *);

#define H(off) (*(s16 *) ((u8 *) arg0 + (off)))
#define U(off) (*(u16 *) ((u8 *) arg0 + (off)))
#define W(off) (*(s32 *) ((u8 *) arg0 + (off)))
#define CURRENT() ((Ovl11C3B78Arg1 *) (arg1 + arg0->unkE * 0xCC + arg0->unkC * 0xC))

void ovl_11_func_800C2044(Ovl11C3B78Arg0 *arg0, s32 arg1, Ovl11C3B78Arg1 *event) {
    u16 command;
    s32 index;
    s32 work;
    s32 next;
    s32 z;
    u16 count;

    arg0->unk30 = event->unk6;
    count = (u16)arg0->unkC;
    z = event->unkA;
    next = count + 1;
    arg0->unkC = next;
    index = arg0->unkC;
    arg0->unk50 = 0;
    arg0->unk54 = 0;
    arg0->unk58 = 0;
    arg0->unk38 = z;
    if (index == 17) {
        arg0->unkC = 16;
        H(0x1A) = 0;
        arg0->unk6 = event->unk4;
        ovl_11_func_800C32C8((Ovl11AnimationObject *) arg0, 1, 0, -2);
        return;
    }
    event = (Ovl11C3B78Arg1 *) (arg1 + arg0->unkE * 0xCC + index * 0xC);
    if (D_80128B68 == 0) {
        command = event->unk0;
        if (command == 3 || command == 9 || command == 17 || command == 18) {
            arg0->unkC++;
            event = CURRENT();
        }
    }
    switch (event->unk0) {
    case 0x8000:
        arg0->unkC = event->unk2;
        event = CURRENT();
        U(0x1E) &= 0xBFFF;
        if (event->unk0 != 5) {
            if (event->unk0 != 2) {
                goto process;
            }
            goto set_position;
        } else {
        U(0x1E) |= 0x4000;
set_position:
        arg0->unk30 = event->unk6;
        arg0->unk34 = event->unk8;
        arg0->unk38 = event->unkA;
        arg0->unk40 = event->unk6 << 16;
        arg0->unk44 = event->unk8 << 16;
        arg0->unk48 = event->unkA << 16;
        arg0->unkC++;
        event = CURRENT();
        if (D_80128B68 != 0) {
            H(0x10) = event->unk2;
        }
        }
        break;
    case 16:
        arg0->unkC++;
        event = CURRENT();
        arg0->unk30 = event->unk6;
        arg0->unk34 = event->unk8;
        arg0->unk38 = event->unkA;
        arg0->unk40 = event->unk6 << 16;
        arg0->unk44 = event->unk8 << 16;
        arg0->unk48 = event->unkA << 16;
        break;
    case 18:
        U(0x20) |= 0x1000;
    case 17:
    case 3:
        if (!(U(0x1E) & 0x1000)) {
            ovl_11_func_800C3EE8((Ovl11AnimationObject *) arg0, (u16 *) event);
            return;
        }
        arg0->unkC++;
        event = CURRENT();
        arg0->unk30 = event->unk6;
        arg0->unk34 = event->unk8;
        arg0->unk38 = event->unkA;
        arg0->unk40 = event->unk6 << 16;
        arg0->unk44 = event->unk8 << 16;
        arg0->unk48 = event->unkA << 16;
        if (D_80128B68 != 0) {
            H(0x10) = event->unk2;
        }
        break;
    case 9:
        ovl_11_func_800C3F6C((Ovl11C3F6CObjectView *) arg0, (u16 *) event);
        return;
    case 4:
        work = ((Ovl11EventObject *)arg0)->entryIndex + 1;
        ((Ovl11EventObject *)arg0)->entryIndex = work;
        ovl_11_func_800C2A98((Ovl11EventObject *) arg0);
        work = (s32) ((u8 *) arg0 + 0xE4);
        U(0x2C) = ((Ovl11EventObject *)arg0)->entries[((Ovl11EventObject *)arg0)->entryIndex].data[1];
        U(0x2E) = ((Ovl11EventObject *)arg0)->entries[((Ovl11EventObject *)arg0)->entryIndex].data[2];
        ((Ovl11EventEntry *)work)[((Ovl11EventObject *)arg0)->entryIndex].time = ((struct Ovl11CommandTimeState *) D_8006C838)->time;
        if (H(0x2E) != ((struct Ovl11SpriteWork *) &D_8007AFF0)->header[1]) {
            U(0x20) &= 0xDFFF;
            U(0x1E) &= 0xFEFF;
            ((Ovl11EventEntry *)work)[((Ovl11EventObject *)arg0)->entryIndex].time -= U(0xA);
            if (D_80125F24[H(0x2E)] != 0) {
                H(0xA) = 0;
            }
            return;
        }
        ((Ovl11EventEntry *)work)[((Ovl11EventObject *)arg0)->entryIndex].time = ((struct Ovl11CommandTimeState *) D_8006C838)->time;
        ovl_11_func_800C3838(arg0, D_80128B68, 0);
        if (D_80128B68 != 0) {
            H(0x10) = event->unk2;
        }
        if (H(0x10) != 0) {
            ovl_11_func_800C32C8((Ovl11AnimationObject *) arg0, 2, H(6), -2);
        }
        break;
    case 1:
    case 6:
        if (D_80128B68 != 0) {
            H(0x10) = event->unk2;
        }
        break;
    case 0:
    case 0xFFFF:
        arg0->unk6 = ((u16 *)event)[-4];
        ovl_11_func_800C32C8((Ovl11AnimationObject *) arg0, 1, 0, -2);
        return;
    }
process:
    if (H(0x10) == 0) {
        ovl_11_func_800C3B78(arg0, event);
        ovl_11_func_800C2C74(arg0);
        ovl_11_func_800C32C8((Ovl11AnimationObject *) arg0, 2, H(6) + 1, -2);
    }
    ovl_11_func_800C1F94((Ovl11AnimationObject *) arg0, (u16 *) event);
}
#endif
