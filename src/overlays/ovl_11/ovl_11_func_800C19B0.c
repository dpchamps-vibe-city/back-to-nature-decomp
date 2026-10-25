#include "common.h"
#include "game_types.h"
#include "psyq/abs.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "ovl11_object_event_script.h"

s32 ovl_11_func_800C3838(Ovl11C3B78Arg0 *arg0, s32 arg1, s32 arg2);

void ovl_11_func_800C19B0(void) {
    s16 temp_v1;
    u16 clearedFlags;
    s32 sentinel;
    s32 var_s1;
    s32 var_v1;
    s32 value;
    s32 time;
    struct Ovl11EventInitRecord *var_s0;
    struct Ovl11EventInitWork *work;

    var_s0 = (struct Ovl11EventInitRecord *) &gObjectEventSlotBase;
    ((struct Ovl11EventInitState *) D_8006C838)->fieldCB70 = 0x13;
    ((struct Ovl11EventInitState *) D_8006C838)->fieldCB72 = 0x28;
    gOvl11ObjectScriptLiveMode = 0;
    for (var_s1 = 0; var_s1 < 37; var_s1++, var_s0++) {
        sentinel = 0x36;
        var_s0->field10 = 0;
        var_s0->field30 = 0;
        var_s0->field34 = 0;
        var_s0->field38 = 0;
        clearedFlags = var_s0->flags & 0xBEBC;
        var_s0->flags = clearedFlags;
        work = (struct Ovl11EventInitWork *) D_8009AFF0;
        var_s0->flags20 &= 0xCFFF;
        temp_v1 = work->header[1];
        gOvl11ObjectScriptTick = 0;
        if ((temp_v1 != sentinel) && (var_s0->field2E == temp_v1) && (Ovl11SelectScriptSpriteSource(var_s0->id, (SpriteSourceData *) ((u8 *) var_s0 + 0x80)) != 0)) {
            if (var_s0->id == 0x1B) {
                var_s0->field2C = sentinel;
                var_s0->field2E = sentinel;
                continue;
            }
            if (ovl_11_func_800C3838((Ovl11C3B78Arg0 *) var_s0, gOvl11ObjectScriptLiveMode, 0) != 0) {
                var_s0->flags |= 0x100;
                Ovl11DispatchCurrentScriptCommand((Ovl11C3B78Arg0 *) var_s0);
                Ovl11LoadScheduledEvent((Ovl11EventObject *) var_s0);
            } else {
                value = var_s0->flags;
                value |= 0x100;
                var_s0->flags = value;
                var_v1 = (s32) D_8006C838;
                time = ((struct Ovl11EventInitState *) var_v1)->time;
                var_v1 = 0;
                if (time >= 0x168) {
                    value = var_s0->index;
                    var_v1 = time - (var_s0->entries[value].unk0 + var_s0->offset);
                }
                if (var_v1 >= 0) {
                    Ovl11RunObjectScriptTicks((Ovl11C3B78Arg0 *) var_s0, gOvl11ClockMinutePeriodTwice * var_v1 / 2);
                } else {
                    var_s0->flags &= 0xFEFF;
                }
                Ovl11LoadScheduledEvent((Ovl11EventObject *) var_s0);
            }
        }
    }
    gOvl11ObjectScriptLiveMode = 1;
}
