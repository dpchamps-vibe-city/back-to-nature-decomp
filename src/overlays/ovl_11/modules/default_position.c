/*
 * default_position — reconstructed translation unit.
 *
 * Read a selected signed X/Z pair into a three-word position, forcing Y to
 * zero.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "ovl11_default_position.h"

void Ovl11ReadDefaultPosition(s32 *position) {
    s16 *pair;

    pair = &gOvl11DefaultPositionPairs[D_80070D06 * 2];
    position[0] = pair[0];
    position[2] = pair[1];
    position[1] = 0;
}
