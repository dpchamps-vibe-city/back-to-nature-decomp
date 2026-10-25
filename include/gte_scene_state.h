#ifndef GTE_SCENE_STATE_H
#define GTE_SCENE_STATE_H

#include "common.h"

/*
 * Persistent camera, fog and lighting configuration for the GTE. Coordinate
 * values use the caller's world units; view rotation uses SDK 0x1000 fixed-point
 * unit vectors. These setters retain the s32 ABI and perform no validation.
 */

/* exe:0x8001B530. Set projection center (160,120) pixels and screen distance
 * 1000; set eye (0,0,4096), roll 2048 and target (0,0,-4096). Clear light and
 * color matrices and initialize the separate default matrix to identity.
 * Does not initialize the fog/color bytes or load a new camera view. */
void InitGteSceneState(void);

/* exe:0x8001B5DC. Rebuild/load the view unless a matrix override is installed,
 * then apply SetFogNear(near,1000), background/far RGB and color/light matrices.
 * Overwrites GTE registers; not a state push/pop. */
void ApplyGteSceneState(void);

/* exe:0x8001B9F8 / 0x8001BA40. Store eye/roll or target and clear the matrix
 * override. Roll uses the SDK angle convention (4096 units per turn). The
 * builder recomputes on every apply while no override is installed. */
void SetCameraEyeAndRoll(s32 x, s32 y, s32 z, s32 rollAngle);
void SetCameraTarget(s32 x, s32 y, s32 z);

/* exe:0x8001BF74 / 0x8001BF88. Store only the low byte of each RGB component;
 * registers are not changed until ApplyGteSceneState. Back RGB is shifted
 * left four bits when loaded into the GTE. Far RGB goes through SetFarColor. */
void SetSceneBackColor(s32 red, s32 green, s32 blue);
void SetSceneFarColor(s32 red, s32 green, s32 blue);

/* exe:0x8001BF9C. Store the raw SDK fog-near argument; apply is deferred. */
void SetSceneFogNear(s32 distance);

#endif
