#ifndef OVL11_SCENE_CAMERA_LIGHTING_H
#define OVL11_SCENE_CAMERA_LIGHTING_H

#include "common.h"

/* Overlay-11 scene camera, lighting and projection. World coordinates are
 * integer scene units, yaw uses 4096 units per turn. Camera state occupies
 * the secondary work area rooted at D_8007AFF0.
 *
 * Startup separately resets orbit inputs and eye/target vectors. Each frame,
 * update -> apply -> draw uses geometry previously loaded by scene setup.
 * The update preserves the GTE matrix stack and truncates output to signed
 * halfwords. These entry points belong to overlay 11, not another overlay
 * sharing the same RAM addresses. */
void Ovl11UpdateSceneCameraOffset(void);
/* Install eye/target in engine camera state with zero roll. */
void Ovl11ApplySceneCamera(void);
/* Default distance=-9000, height=-4500, yaw=-512. Leaves vectors untouched. */
void Ovl11ResetSceneCameraOrbit(void);
/* Three flat lights: first RGB 208, others black, each direction (100,100,100).
 * Ambient is Q12 (2048,2048,2048), light mode 0. Updates libgs lighting state. */
void Ovl11InitSceneLighting(void);
/* Set libgs and GTE projection distance to 1000. */
void Ovl11SetSceneProjection(void);
/* Cull and emit loaded gOvl11SceneGeometry with the current eye/target. */
void Ovl11DrawSceneGeometry(void);
/* Clear both computed eye-offset and target triples, not orbit parameters. */
void Ovl11ClearSceneCameraVectors(void);

#endif
