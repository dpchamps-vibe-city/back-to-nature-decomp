#include "common.h"
#include "game_types.h"
#include "ovl11_spawn_record_access.h"
#include "ovl11_record_position_set.h"

s32 ovl_11_func_80109974(Ovl11SetFieldsView *arg0, Ovl11PaddedVec3 position, s16 arg5, s32 arg6) {
    Ovl11PaddedVec3 found;
    s16 kind;
    s32 result;
    s32 minX;
    s32 maxX;
    s32 minZ;
    s32 maxZ;

    result = 0;
    minX = position.vx - arg6;
    maxX = position.vx + arg6;
    minZ = position.vz - arg6;
    maxZ = position.vz + arg6;
    if (Ovl11ReadSpawnRecord(0x65, &found.vx, &kind, 0) == 0) {
        return 0;
    }
    if (arg5 != kind) {
        return 0;
    }
    if ((minX < found.vx) && (found.vx < maxX) &&
        (minZ < found.vz) && (found.vz < maxZ)) {
        Ovl11SetRecordPosition(arg0, found);
        result = 1;
    }
    return result;
}
