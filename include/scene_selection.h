#ifndef SCENE_SELECTION_H
#define SCENE_SELECTION_H

#include "common.h"

/*
 * Scene IDs are unitless EXE dispatch selectors, not overlay numbers. The
 * entry loop dispatches IDs 0..20 and initially selects 13. Keep the s32 ABI;
 * requests are not range-checked (the dispatcher uses an unsigned bounds test).
 *
 * These are shared boot-family state, not an event queue. A request immediately
 * saves the old current ID as previous, installs sceneId, and clears the update
 * loop's run flag. It does not abort the calling callback or initialize/load
 * a scene: setup follows when the callback and update loop return to dispatch.
 * Repeated requests overwrite the pair, even if sceneId equals the current ID.
 * The update loop sets its run flag to 1 on entry, so requests are intended for
 * its callbacks, not as a persistent stop latch before entering that loop.
 */

/* Original identity: exe:0x80011EF0 (formerly func_80011EF0). */
void RequestSceneChange(s32 sceneId);

/* Read the ID saved by the most recent request; querying has no side effects.
 * Boot's direct initial selection does not establish a previous-scene value.
 * Original identity: exe:0x80011F08 (formerly GetVal8005E394). */
s32 GetPreviousSceneId(void);

/* Read the selected/requested ID, not proof that its setup has completed.
 * Original identity: exe:0x80011F14 (formerly GetVal8005E39C). */
s32 GetCurrentSceneId(void);

#endif
