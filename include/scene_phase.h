#ifndef SCENE_PHASE_H
#define SCENE_PHASE_H

#include "common.h"

/* exe:80021FD0. Clear both shared scene-dispatch words (gScenePhase and
 * gSceneSubphase) to zero. The main loop calls this after loading selected
 * overlays, before dispatching their first update. No overlay-private state,
 * callback, resource or other part of game state is reset. */
void ResetSceneDispatchState(void);

#endif
