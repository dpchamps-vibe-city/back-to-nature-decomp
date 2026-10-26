/* Scene-surface height resolution. The special 0xC080/0xE080 modes first
 * consult collision probes for a zero-height surface; all other paths use
 * the scene's polygon geometry and its last-hit plane. Contract and effects:
 * ovl11_surface_height.h. Original ovl_11:800BDB88–800BDCE0. */
#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "ovl11_collision_query_budget.h"
#include "ovl11_surface_height.h"

void SetVal8005E2E8(s32 mode);
s32 func_8001E4C0(void *geometry, BoundsStruct_8001E878 *position);
s32 func_8001E38C(void);

s32 Ovl11ResolveSurfaceHeight(s32 *position, s32 mode) {
    void *firstObject;
    s32 firstFlags;
    void *secondObject;
    s32 secondFlags;
    s32 found;
    s32 height;
    s32 result;

    if (mode == 0xC080 &&
        ((Ovl11CollisionWorkView *)&D_8007AFF0)->collision.field_2 == 1 &&
        Ovl11QueryCollisionUnbudgeted(position,
            ((Ovl11CollisionWorkView *)&D_8007AFF0)->collision,
            0, 0, &firstObject, &firstFlags, 0, 0x10) == 0) {
        if (position[1] < 0) {
            result = 1;
        } else {
            position[1] = 0;
            result = 0;
        }
    } else if ((mode == 0xC080 || mode == 0xE080) &&
        Ovl11QueryCollisionUnbudgeted(position,
            ((Ovl11CollisionWorkView *)&D_8007AFF0)->collision,
            0, 0, &secondObject, &secondFlags, 0, 0x20010) == 0 &&
        position[1] >= 0) {
        position[1] = 0;
        result = 0;
    } else {
        SetVal8005E2E8(mode);
        found = func_8001E4C0(gOvl11SceneGeometry, (BoundsStruct_8001E878 *)position);
        height = func_8001E38C();
        if (found == 0) {
            result = 1;
        } else {
            position[1] = height;
            result = 0;
        }
    }
    return result;
}
