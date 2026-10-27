/* Probe the current area using the private five-tolerance search policy.
 * Only a successful Y is copied back. See ovl11_surface_height_probe.h. */
#include "common.h"
#include "game_types.h"
#include "ovl11_surface_height.h"
#include "surface_collision.h"
#include "ovl11_surface_height_probe.h"

s32 Ovl11ProbePositionHeight(Vec3 *position) {
    Vec3 probe;
    s32 result;
    u32 toleranceIndex;

    result = 1;
    toleranceIndex = 0;
    probe = *position;
    for (; toleranceIndex < 5; toleranceIndex++) {
        SetSurfaceHeightTolerance(gOvl11PositionHeightTolerances[toleranceIndex]);
        if (Ovl11ResolveSurfaceHeight(&probe.x, 0) == 0) {
            position->y = probe.y;
            result = 0;
            break;
        }
    }
    SetSurfaceHeightTolerance(0xC8);
    return result;
}
