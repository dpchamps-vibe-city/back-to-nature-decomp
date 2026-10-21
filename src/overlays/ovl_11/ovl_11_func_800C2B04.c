#include "common.h"
#include "game_types.h"

void ovl_11_func_800C2A98(Ovl11EventObject *arg0);
s32 ovl_11_func_800DD1D0(s16 *arg0);
s32 ovl_11_func_800C28CC(s16 arg0, SpriteSourceData *arg1);
s32 ovl_11_func_800C3838(Ovl11EventObject *arg0, s16 arg1, s32 arg2);

void ovl_11_func_800C2B04(Ovl11EventObject *arg0) {
    s16 oldEvent;
    s32 time;
    s32 nextTime;
    s32 ready;
    s32 changed;
    u16 flags;

    if (((Ovl11C2B04GlobalState *) D_8006C838)->flags & 0x100000) {
        if (arg0->entryIndex != 0x1D) {
            time = ((Ovl11C2B04GlobalState *) D_8006C838)->time;
            nextTime = arg0->entries[arg0->entryIndex + 1].time + *(s16 *) ((u8 *) arg0 + 0xA);
            ready = 0;
            if (nextTime < 0x168) {
                time -= 0x5A0;
            }
            if (time >= nextTime) {
                ready = 1;
            }
            if (ready != 0) {
                oldEvent = *(s16 *) ((u8 *) arg0 + 0x2E);
                changed = 0;
                if (arg0->flags & 0x100) {
                    arg0->flags20 |= 0x2000;
                } else {
                    arg0->flags20 &= 0xDFFF;
                }
                arg0->entryIndex++;
                ovl_11_func_800C2A98(arg0);
                if (oldEvent != *(s16 *) ((u8 *) arg0 + 0x2E)) {
                    changed = 1;
                }
                if (ovl_11_func_800DD1D0((s16 *) ((u8 *) arg0 + 0x2C)) == 0) {
                    if (!(arg0->flags & 0x100)) {
                        if (ovl_11_func_800C28CC(*(s16 *) arg0, (SpriteSourceData *) ((u8 *) arg0 + 0x80)) != 0) {
                            *(s16 *) ((u8 *) arg0 + 0x10) = 0;
                            goto advance;
                        }
                    } else {
advance:
                        flags = arg0->flags;
                        arg0->flags20 &= 0xEFFF;
                        if (!(flags & 0x1000) && changed != 0) {
                            arg0->flags = flags | 0x100;
                        }
                        arg0->field12 = 0x14;
                        ovl_11_func_800C3838(arg0, D_80128B68, 0);
                    }
                }
            }
        }
    }
}
