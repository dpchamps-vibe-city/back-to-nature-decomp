#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"

void func_8001AF70(u16 arg0, u16 arg1);
void ovl_11_func_8010941C(s32 arg0);
s32 ovl_11_func_800E6EE0(s16 arg0, s16 arg1, s32 arg2, s32 arg3);

s32 ovl_11_func_800EC064(s16 arg0, s16 arg1, s32 arg2, s32 arg3) {
    s16 index;
    s32 i;
    s32 offset;
    s32 *flags;
    s32 *otherFlags;
    char *base;
    char *otherBase;
    struct_800759E4 *object;

    if (arg3 != 0) {
        index = *(s16 *)&gOvl11ValueSlots[arg1];
    } else {
        index = arg1;
    }
    object = NULL;
    switch (arg0) {
    case 0:
        object = (struct_800759E4 *)&D_80075AD4;
        break;
    case 1:
    case 2:
        if (arg2 == 0) {
            i = 0;
            base = (char *)D_8006C838;
            base += 0x81F0;
            flags = (s32 *)base;
            do {
                if (flags[i * 0x2E] & 0x400) {
                    offset = (s16)i;
                    object = (struct_800759E4 *)((char *)&D_800749F4 + offset * 0xB8);
                    break;
                }
                i++;
            } while (i < 20);
            if (object == NULL) {
                return 1;
            }
        } else {
            object = (struct_800759E4 *)((char *)&D_800749F4 + index * 0xB8);
        }
        break;
    case 3:
        if (arg2 == 0) {
            i = 0;
            otherFlags = &D_80074320;
            for (; i < 10; i++) {
                if (otherFlags[i * 0x2D] & 0x400) {
                    otherBase = (char *)(otherFlags - 0xD);
                    offset = (s16)i;
                    object = (struct_800759E4 *)(offset * 0xB4 + (s32)otherBase);
                    break;
                }
            }
            if (object == NULL) {
                return 1;
            }
        } else {
            object = (struct_800759E4 *)((char *)&D_800742EC + index * 0xB4);
        }
        break;
    case 4:
        object = &D_800759E4;
        break;
    default:
        return 0;
    }
    switch (arg2) {
    case 0:
        object->f34 &= ~0x400;
        break;
    case 1:
        object->f34 |= 0x400;
        break;
    case 2:
        func_8001AF70(0x74, 0);
        ovl_11_func_8010941C(0);
        break;
    case 3:
        ovl_11_func_800E6EE0(0x106, 1, 0, 1);
        ((struct Ovl11PoolResetView *)D_8006C838)->field_91E0 &= ~0x20000;
        func_8001AF70(0x74, 1);
        ovl_11_func_8010941C(1);
        break;
    }
    return 1;
}
