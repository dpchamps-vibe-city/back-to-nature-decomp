#ifndef MAP_TEXTURE_CAMERA_H
#define MAP_TEXTURE_CAMERA_H

/* Farm-overlay map camera, using the existing scene/far-work-area state.
 * Calls are frame-driven and non-reentrant; they allocate no storage.
 *
 * Enter selects camera mode 0x17 and resets only the phase halfword. It does
 * not initialize the retained pan coordinates or vertical step/accumulator.
 * Step updates the vertical accumulator and adds its low halfword to the
 * camera y offset only while mode 0x17 is active, then subtracts 4 from the
 * step. These are engine-coordinate increments per call, not elapsed time.
 *
 * Update runs setup, a seasonal 25x45 texture refresh, pad-controlled x/z pan
 * (200 coordinate units per call), and fade-gated exit. Bounds gate each pan
 * step rather than clamp arbitrary initial values. Exit restores the normal
 * camera and sets the existing scene refresh bit. Scene/grid state must
 * already be initialized; texture refresh assumes non-null grid entries.
 */
void Ovl11EnterMapCameraMode(void);
void Ovl11StepMapCameraVerticalOffset(void);
void Ovl11UpdateMapCameraMode(void);

#endif
