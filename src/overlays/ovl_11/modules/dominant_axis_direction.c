/* Stateless XZ direction classification, ovl_11:800F06D8–800F0734.
 * Shared by facing commands, pending animations and entity-pool selection;
 * it has no record-selection, animation or persistent-state side effects.
 * See ovl11_dominant_axis_direction.h for direction codes and tie behavior.
 * The adjacent pair commands and adjustment-table service share neither
 * this implementation's inputs nor private state; original object edges
 * remain evidence-limited under -G0 (notes/file-groupings.md).
 */
#include "ovl11_dominant_axis_direction.h"

s32 Ovl11GetDominantXZDirection(s32 *target, s32 *origin) {
    s32 deltaZ;
    s32 deltaX;
    s32 magnitudeZ;
    s32 magnitudeX;

    deltaX = target[0] - origin[0];
    deltaZ = target[2] - origin[2];
    magnitudeX = (deltaX < 0) ? -deltaX : deltaX;
    magnitudeZ = (deltaZ < 0) ? -deltaZ : deltaZ;
    if (magnitudeZ < magnitudeX) {
        return (deltaX > 0) ? 3 : 1;
    }
    return (deltaZ > 0) ? 2 : 0;
}
