#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "ovl11_collision_query_budget.h"

/* Overlay symbol name for the matched SetVal8005E2E8 definition. */
void SetVal8005E2E8(s32 arg0);
s32 func_8001E4C0(void *arg0, BoundsStruct_8001E878 *arg1);
s32 func_8001E38C(void);

s32 ovl_11_func_800BDB88(s32 *arg0, s32 arg1) {
    void *object1;
    s32 result1;
    void *object2;
    s32 result2;
    s32 found;
    s32 height;
    s32 ret;

    if (arg1 == 0xC080 &&
        ((Ovl11CollisionWorkView *)&D_8007AFF0)->collision.field_2 == 1 &&
        Ovl11QueryCollisionUnbudgeted(arg0,
            ((Ovl11CollisionWorkView *)&D_8007AFF0)->collision,
            0, 0, &object1, &result1, 0, 0x10) == 0) {
        if (arg0[1] < 0) {
            ret = 1;
        } else {
            arg0[1] = 0;
            ret = 0;
        }
    } else if ((arg1 == 0xC080 || arg1 == 0xE080) &&
        Ovl11QueryCollisionUnbudgeted(arg0,
            ((Ovl11CollisionWorkView *)&D_8007AFF0)->collision,
            0, 0, &object2, &result2, 0, 0x20010) == 0 &&
        arg0[1] >= 0) {
        arg0[1] = 0;
        ret = 0;
    } else {
        SetVal8005E2E8(arg1);
        found = func_8001E4C0(D_80128818, (BoundsStruct_8001E878 *)arg0);
        height = func_8001E38C();
        if (found == 0) {
            ret = 1;
        } else {
            arg0[1] = height;
            ret = 0;
        }
    }
    return ret;
}
