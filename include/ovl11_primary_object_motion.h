#ifndef OVL11_PRIMARY_OBJECT_MOTION_H
#define OVL11_PRIMARY_OBJECT_MOTION_H

#include "common.h"

/* Integrate the shared primary object's signed XYZ position by its velocity
 * (world units per invocation), resolving surface height and side collisions.
 * The optional probe can change object state, dispatch flags, velocity and the
 * five-update hold counter. Entry to special-motion bit 40000000 sets Y
 * velocity to -80. While active, apply table X/Z increments, add 16 to Y
 * velocity per update up to 128, and clear on contact (with actuator request).
 * Object flag 1 suppresses table acceleration and gravity updates.
 * Collision tolerance is restored to 200.
 * Publish camera-work halfwords unless blocked by work flag 400000, clear X/Z
 * velocity, optionally clamp, project, and update the camera screen offset.
 * Requires initialized object, surface/collision state, camera and render data;
 * direction indexes the four-entry tables without bounds checking.
 * No allocation. The projection matrix stack is restored. */
void Ovl11UpdatePrimaryObjectMotion(void);

#endif
