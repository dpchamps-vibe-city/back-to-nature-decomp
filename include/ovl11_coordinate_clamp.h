#ifndef OVL11_COORDINATE_CLAMP_H
#define OVL11_COORDINATE_CLAMP_H

#include "common.h"
#include "game_types.h"

/* Reset to x=[-13000,-8000], z=[2500,4500], stored y=[0,440]. */
void Ovl11ResetCoordinateClamp(void);

/* Copy the first xyz triple of each corner; the first supplies minX/maxZ,
 * the second maxX/minZ. Store both y values without applying them. No bound
 * validation or reordering is performed. Coordinates use scene units. */
void Ovl11SetCoordinateClamp(Ovl11SceneCoordinates *firstCorner,
                             Ovl11SceneCoordinates *secondCorner);

/* Clamp only the first triple's x and z in place. Leave y and every other
 * halfword untouched. Ordered comparisons are significant for inverted
 * bounds: minX, maxZ, maxX, minZ are applied in that order. */
void Ovl11ClampCoordinates(Ovl11SceneCoordinates *position);

#endif
