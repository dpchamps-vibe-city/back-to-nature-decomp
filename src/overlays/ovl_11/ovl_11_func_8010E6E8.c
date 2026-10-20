#include "common.h"
#include "game_types.h"

s32 func_800225B8(void);
s32 func_8002261C(s32 arg0, s32 arg1);
s32 ovl_11_func_80110658(s16 *arg0, s16 *arg1, s32 arg2, s32 arg3, s32 arg4);
s32 ovl_11_func_80110944(s16 area, u16 mode);

s32 ovl_11_func_8010E6E8(Ovl11E6E8Object *obj) {
    s16 x;
    s16 y;
    s32 result;
    s32 facing;
    u16 direction;
    s32 first;
    s32 second;
    s32 message;
    s32 number;

    result = 0;
    ((Ovl11E6E8State *) D_8006C838)->field_51EE = 1;
    ((Ovl11E6E8State *) D_8006C838)->field_522C = 0;
    ((Ovl11E6E8State *) D_8006C838)->field_5230 = 0;
    ((Ovl11E6E8State *) D_8006C838)->field_5234 |= 0x800400;
    direction = ((Ovl11E6E8State *) D_8006C838)->field_5200;
    if (direction < 2U) {
        facing = direction;
        facing += 2;
    } else {
        facing = direction;
        facing -= 2;
    }
    obj->direction = facing;
    switch (obj->state) {
    case 0:
        obj->flags |= 0x800;
        switch (func_800225B8()) {
        case 0:
            break;
        case 1:
            obj->state = 2;
            if (ovl_11_func_80110658(&x, &y, 1, 1, 1) != 0 &&
                ovl_11_func_80110658(&x, &y, 0, 1, 1) != 0) {
                obj->state = 4;
            }
            obj->attributes |= 0x41;
            break;
        case 2:
            obj->state = 2;
            if (ovl_11_func_80110658(&x, &y, 1, 0, 0) != 0 &&
                ovl_11_func_80110658(&x, &y, 0, 0, 0) != 0) {
                obj->state = 4;
            }
            obj->attributes |= 0x42;
            break;
        case 3:
            obj->state = 2;
            first = ovl_11_func_80110944(1, 0xF);
            if (first == -1) {
                second = ovl_11_func_80110944(4, 0xF);
                if (second == first && ovl_11_func_80110944(5, 0xF) == second) {
                    obj->state = 4;
                }
            }
            obj->attributes |= 0x44;
            break;
        case 4:
            obj->state = 5;
            break;
        }
        break;
    case 2:
        message = D_80127D78[obj->index];
        number = message + 3;
        if (obj->index != 0) {
            number = message + 2;
        }
        func_8002261C(2, number);
        obj->state = 3;
        break;
    case 3:
        switch (func_800225B8()) {
        case 0:
            break;
        case 1:
            obj->choice = 2;
            obj->state = -1;
            func_8002261C(2, obj->index + 0x26C);
            break;
        case 2:
            obj->choice = 4;
            obj->state = -1;
            func_8002261C(2, obj->index + 0x26C);
            break;
        case 3:
            obj->choice = 8;
            obj->state = -1;
            func_8002261C(2, obj->index + 0x26C);
            break;
        case 4:
            obj->state = 5;
            break;
        }
        break;
    case 4:
        func_8002261C(2, 0x149);
        obj->state = 1;
        break;
    case 5:
        message = D_80127D78[obj->index];
        number = message + 4;
        if (obj->index != 0) {
            number = message + 3;
        }
        func_8002261C(2, number);
        obj->state = 1;
        break;
    case 1:
        obj->state = -1;
        obj->attributes &= ~0x47;
        /* fall through */
    case -1:
        ((Ovl11E6E8State *) D_8006C838)->field_51EE = 0;
        ((Ovl11E6E8State *) D_8006C838)->field_5234 &= ~0x400;
        ((Ovl11E6E8State *) D_8006C838)->field_5234 &= ~0x800000;
        result = 1;
        obj->timer = 0x12C;
        obj->flags &= ~0x800;
        break;
    }
    return result;
}
