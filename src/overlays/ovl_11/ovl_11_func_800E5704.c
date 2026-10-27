#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "ovl11_state_field_access.h"
#include "ovl11_direction_vector.h"

s32 ovl_11_func_800E5704(s16 arg0, s16 arg1, s32 arg2, s32 arg3) {
    s16 index;
    s32 changed;
    u8 *ptr;
    u8 *other;

    ptr = Ovl11ResolveStateField(0x22, (s32) arg0, NULL);
    other = Ovl11ResolveStateField(0x23, (s32) arg0, NULL);
    index = arg1 + 2;
    changed = 0;
    switch (index) {
    case 2:
        if (arg2 >= (*(s32 *) ((u8 *) ptr + 8))) {
            (*(s32 *) ((u8 *) ptr + 8)) = arg2;
            changed = 1;
        }
        break;
    case 3:
        if (arg2 >= (*(s32 *) ((u8 *) ptr + 0))) {
            (*(s32 *) ((u8 *) ptr + 0)) = arg2;
            changed = 1;
        }
        break;
    case 4:
        if ((*(s32 *) ((u8 *) ptr + 8)) >= arg2) {
            (*(s32 *) ((u8 *) ptr + 8)) = arg2;
            changed = 1;
        }
        break;
    case 5:
        if ((*(s32 *) ((u8 *) ptr + 0)) >= arg2) {
            (*(s32 *) ((u8 *) ptr + 0)) = arg2;
            changed = 1;
        }
        break;
    case 1:
        if (arg2 >= (*(s32 *) ((u8 *) ptr + 4))) {
            (*(s32 *) ((u8 *) ptr + 4)) = arg2;
            changed = 1;
        }
        break;
    case 0:
        if ((*(s32 *) ((u8 *) ptr + 4)) >= arg2) {
            (*(s32 *) ((u8 *) ptr + 4)) = arg2;
            changed = 1;
        }
        break;
    }
    if (changed != 0) {
        Ovl11SetDirectionVector(4, (Recon800D0408A1View *) other, 0);
        return 1;
    }
    Ovl11SetDirectionVector((u16)arg1, (Recon800D0408A1View *) other, arg3 * 2);
    return 0;
}
