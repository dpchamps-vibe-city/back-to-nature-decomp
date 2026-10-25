#ifndef OVL19_SCENE_CAMERA_GEOMETRY_H
#define OVL19_SCENE_CAMERA_GEOMETRY_H

#include "common.h"

/* Overlay-19 scene camera and geometry. Coordinates use the scene's integer
 * world units; angles use 4096 units per turn. State lives in the secondary
 * work area rooted at D_8007AFF0 and is shared with the scene controllers.
 *
 * Frame order: update the rotated offset, apply the camera, then draw.
 * Geometry must first be loaded by the overlay's scene setup. */
void Ovl19InitSceneCamera(void);
/* Recompute the signed-halfword eye offset from distance, height and yaw.
 * Preserves the GTE matrix stack; computed coordinates truncate to 16 bits. */
void Ovl19UpdateSceneCameraOffset(void);
/* Install eye/target in the engine camera state, with zero roll. */
void Ovl19ApplySceneCamera(void);
/* Cull and emit geometry using the same eye/target as ApplySceneCamera. */
void Ovl19DrawSceneGeometry(void);
/* position[0..3] is an in/out signed-halfword record. Query the loaded scene
 * with tolerance 0xC000; replace Y on a hit, preserve X/Z/fourth component.
 * This also changes the engine's shared collision query/result state. */
void Ovl19AdjustPositionToSurface(s16 *position);

#endif
