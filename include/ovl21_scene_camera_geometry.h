#ifndef OVL21_SCENE_CAMERA_GEOMETRY_H
#define OVL21_SCENE_CAMERA_GEOMETRY_H

#include "common.h"

/* Overlay-21 camera/geometry interface. Coordinates are integer scene-world
 * units; yaw uses 4096 units per turn. State is shared with scene controllers
 * in the D_8007AFF0 work area. Do not use these overlay entry points while a
 * different overlay occupies their RAM slot.
 *
 * Initialize once, then update -> apply -> draw each frame. Scene setup must
 * load gOvl21SceneGeometry before drawing. The update preserves the GTE
 * matrix stack and truncates its results to signed-halfword coordinates. */
void Ovl21InitSceneCamera(void);
void Ovl21UpdateSceneCameraOffset(void);
/* Install eye/target and zero roll in the engine camera state. */
void Ovl21ApplySceneCamera(void);
/* Cull and emit the loaded geometry using the same eye/target. */
void Ovl21DrawSceneGeometry(void);

#endif
