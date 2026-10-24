#include "common.h"
#include "game_types.h"

s32 ovl_11_func_800F44B4(u16 arg0, s32 *arg1, s16 *arg2, s32 arg3);
void ovl_11_func_800D05D0(Ovl11SetFieldsView *arg0, Ovl11PaddedVec3 v);

s32 ovl_11_func_80109974(Ovl11SetFieldsView *arg0, Ovl11PaddedVec3 position, s16 arg5, s32 arg6) {
    Ovl11PaddedVec3 found;
    s16 kind;
    s32 result;
    s32 minX;
    s32 maxX;
    s32 minZ;
    s32 maxZ;

    result = 0;
    minX = position.field_0 - arg6;
    maxX = position.field_0 + arg6;
    minZ = position.field_8 - arg6;
    maxZ = position.field_8 + arg6;
    if (ovl_11_func_800F44B4(0x65, &found.field_0, &kind, 0) == 0) {
        return 0;
    }
    if (arg5 != kind) {
        return 0;
    }
    if ((minX < found.field_0) && (found.field_0 < maxX) &&
        (minZ < found.field_8) && (found.field_8 < maxZ)) {
        ovl_11_func_800D05D0(arg0, found);
        result = 1;
    }
    return result;
}
