#ifndef CAMERA_OFFSET_H
#define CAMERA_OFFSET_H

#include "common.h"

/* Initialize radius=800, height=600, yaw=0; compute and back up the offset,
 * zero its origin, clear pending changes, and set state=2 / initialized=1.
 * Called during engine startup; these are world-coordinate values, not pixels. */
void InitCameraOffsetState(void);

/* Store cylindrical parameters without recomputing the vector. Any change
 * marks the state dirty and resets its change-state word. Radius and height
 * use the same integer coordinate units as the output; yaw uses 4096/turn. */
void SetCameraOffsetParameters(s32 radius, s32 height, s32 yaw);

/* Copy three s32 origin coordinates to the supplied address (legacy s32 ABI).
 * The destination must have room for all three words. */
void CopyCameraOffsetOrigin(s32 destination);

/* Clear only the state byte, leaving parameters and vectors untouched. */
void ClearCameraOffsetState(void);

/* Write x=cos(yaw)*radius, y=-height, z=sin(yaw)*radius to three s32 words.
 * Yaw wraps modulo 4096. The Q12 trig products are shifted arithmetically;
 * input magnitudes must keep the signed products within s32 range. */
void ComputeCylindricalOffset(s32 *out, s32 radius, s32 height, s32 yaw);

#endif
