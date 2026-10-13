#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"

s32 ovl_11_func_800D7EF8(s32 *, u16 *, u16 *);
s32 ovl_11_func_800BE6E4(s32 **, s32);

s32 ovl_11_func_800BDF98(VECTOR position, Ovl11CollisionTag arg4,
                       s16 arg5, s32 arg6) {
    Ovl11CollisionQuery query;
    u16 col;
    u16 row;
    void *tile;
    void **tilePtr = &tile;
    s32 flags;
    s32 result;
    s32 checkedResult;

    query.position = &position.vx;
    query.object = &arg4;
    query.tile = (void **) &tilePtr;
    query.flags = &flags;
    query.field_C = 0;
    query.field_14 = 0;
    ovl_11_func_800D7EF8(&position.vx, &col, &row);
    if ((s16) col == 0x2C && arg5 == 3) {
        return 1;
    }
    checkedResult = ovl_11_func_800BE6E4((s32 **) &query, 0);
    result = checkedResult;
    if (checkedResult == 2) {
        if (arg6 == 0) {
            switch (arg5) {
            case 0:
                position.vz -= 0x190;
                break;
            case 1:
                position.vx -= 0x190;
                break;
            case 2:
                position.vz += 0x190;
                break;
            case 3:
                position.vx += 0x190;
                break;
            }
            if (ovl_11_func_800BDF98(position, arg4, arg5, 1) == 0) {
                return 2;
            }
            result = 1;
        } else {
            return 2;
        }
    }
    return result;
}
