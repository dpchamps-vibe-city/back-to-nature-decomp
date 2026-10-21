#include "common.h"
#include "game_types.h"
#include "psyq/abs.h"

s32 ovl_11_func_800C3828(void);
s32 ovl_11_func_800BDB88(s32 *arg0, s32 arg1);
u16 ovl_11_func_8010734C(s32 id);
void ovl_11_func_800C2044(Ovl11C3B78Arg0 *arg0, s32 arg1, void *arg2);

void ovl_11_func_800C2678(Ovl11C3B78Arg0 *arg0) {
    s32 fixedX;
    s32 fixedZ;
    s32 eventBase;
    s32 distance;
    s32 deltaX;
    s32 deltaZ;
    u16 command;
    Ovl11C3B78Arg1 *event;

    eventBase = ovl_11_func_800C3828();
    fixedX = *(s32 *)((u8 *)arg0 + 0x40) + arg0->unk50;
    *(s32 *)((u8 *)arg0 + 0x40) = fixedX;
    fixedZ = *(s32 *)((u8 *)arg0 + 0x48) + arg0->unk58;
    *(s32 *)((u8 *)arg0 + 0x48) = fixedZ;
    event = (Ovl11C3B78Arg1 *)(eventBase +
        (*(s16 *)((u8 *)arg0 + 0xE) * 0xCC) + (arg0->unkC * 0xC));
    arg0->unk30 = fixedX / 0x10000;
    arg0->unk38 = fixedZ / 0x10000;
    if ((*(u16 *)((u8 *)arg0 + 0x1E) & 0x100) && !(D_80122F88 & 1)) {
        ovl_11_func_800BDB88(&arg0->unk30, 0xC000);
    }
    command = event->unk0;
    switch (command) {
    case 3:
    case 0x11:
    case 0x12:
        if ((*(u16 *)((u8 *)arg0 + 0x1E) & 2) &&
            (*(u16 *)((u8 *)arg0 + 0x1A) == 5) &&
            (ovl_11_func_8010734C((s16)event->unk2) == 3)) {
            ovl_11_func_800C2044(arg0, eventBase, event);
            *(s16 *)((u8 *)arg0 + 0x12) = 0x14;
            *(u16 *)((u8 *)arg0 + 0x1E) &= 0xFFFC;
        }
        break;
    default:
        distance = 0x40;
        if (D_80125F24[*(s16 *)((u8 *)arg0 + 0x2E)] == 0x1000) {
            distance = 0x10;
        }
        if (*(u16 *)((u8 *)arg0 + 0x1E) & 0x4000) {
            distance = 0x10;
        }
        deltaX = abs(event->unk6 - arg0->unk30);
        if (deltaX < distance) {
            deltaZ = abs(event->unkA - arg0->unk38);
            if (deltaZ < distance) {
                ovl_11_func_800C2044(arg0, eventBase, event);
            }
        }
    }
}
