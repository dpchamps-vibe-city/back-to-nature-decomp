/* Private scene-coordinate bounds: reset, two-corner copy and x/z clamp.
 * Public contracts: ovl11_coordinate_clamp.h.
 * Membership and supported boundaries: notes/file-groupings.md. */
#include "ovl11_coordinate_clamp.h"

void Ovl11ResetCoordinateClamp(void) {
    gOvl11CoordinateClampBounds.x = -0x32C8;
    gOvl11CoordinateClampBounds.z = 0x1194;
    gOvl11CoordinateClampBounds.secondX = -0x1F40;
    gOvl11CoordinateClampBounds.y = 0;
    gOvl11CoordinateClampBounds.secondY = 0x1B8;
    gOvl11CoordinateClampBounds.secondZ = 0x9C4;
}

void Ovl11SetCoordinateClamp(Ovl11SceneCoordinates *firstCorner,
                             Ovl11SceneCoordinates *secondCorner) {
    gOvl11CoordinateClampBounds.x = firstCorner->x;
    gOvl11CoordinateClampBounds.y = firstCorner->y;
    gOvl11CoordinateClampBounds.z = firstCorner->z;
    gOvl11CoordinateClampBounds.secondX = secondCorner->x;
    gOvl11CoordinateClampBounds.secondY = secondCorner->y;
    gOvl11CoordinateClampBounds.secondZ = secondCorner->z;
}

void Ovl11ClampCoordinates(Ovl11SceneCoordinates *position) {
    if (position->x < gOvl11CoordinateClampBounds.x) {
        position->x = gOvl11CoordinateClampBounds.x;
    }
    if (position->z > gOvl11CoordinateClampBounds.z) {
        position->z = gOvl11CoordinateClampBounds.z;
    }
    if (position->x > gOvl11CoordinateClampBounds.secondX) {
        position->x = gOvl11CoordinateClampBounds.secondX;
    }
    if (position->z < gOvl11CoordinateClampBounds.secondZ) {
        position->z = gOvl11CoordinateClampBounds.secondZ;
    }
}
