#include "common.h"
#include "game_types.h"
#include "psyq/abs.h"
#include "psyq/memory.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "ovl11_object_events.h"

s32 ovl_11_func_800C7838(u8 *arg0) {
    Ovl11AnimationObject *temp_v0;

    temp_v0 = (Ovl11AnimationObject *)Ovl11TriggerNearbyObjectReactions(arg0);
    if (temp_v0 == 0) {
        return 0;
    } else {
        return temp_v0->field_1A != 2;
    }
}
