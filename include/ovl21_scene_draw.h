#ifndef OVL21_SCENE_DRAW_H
#define OVL21_SCENE_DRAW_H

#include "common.h"

/* Requires initialized scene records/sprite banks and the active render
 * context; the primary offset selector must be in 0..5 (unchecked). Draws
 * six record sprites, each active slot in their three-slot
 * arrays, then the primary sprite at the selected offset-table index.
 * Record state 2 with the source's animation-step bit set also draws the
 * shared marker with a -350 Y-coordinate offset. Drawing may
 * select/advance animations and modifies packet/ordering-table state. */
void Ovl21DrawSceneSprites(void);

/* Draws the separate centered frame sprite. Not part of the traversal above. */
void Ovl21DrawSceneFrame(void);

#endif
