#ifndef OVL25_SCENE_PHASE_LIFECYCLE_H
#define OVL25_SCENE_PHASE_LIFECYCLE_H

#include "common.h"

/* Engine-installed overlay-25 frame callback. The shared work +448C word
 * indexes five phases: staged resource/sound loading, fade-in wait, mode
 * update, exit request and teardown. Indices are unchecked; scene entry must
 * initialize phase, asset-load step and mode selector before dispatch.
 *
 * Saves/restores text draw mode around phase dispatch (mode 3 while inside),
 * then clears shared work flag 0x08000000 when present. Loading advances one
 * stage per call and waits for sound transfer before scene/camera/message and
 * VRAM-animation initialization. The mode phase updates message windows,
 * camera/GTE/geometry, VRAM animation and the selected mode handler.
 *
 * Exit waits for both screen and master-volume fades, stops the saved effect
 * voice if valid and sequence 0, rewinds sound slots 2/1, marks phase FF,
 * requests resident scene 6, sets work flag 0x80000 and clears background RGB.
 * All services and shared buffers are mutable and non-reentrant. Timing is
 * update-call based; no elapsed-millisecond interpretation is assumed.
 */
void Ovl25DispatchScenePhase(void);

#endif
