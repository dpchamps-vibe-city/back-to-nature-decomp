#ifndef OVL11_SCENE_SETUP_INSTALL_H
#define OVL11_SCENE_SETUP_INSTALL_H

#include "common.h"

/* Set the shared scene-collision vertical tolerance to 200 world units.
 * Separate from installation; does not load or render anything. */
void Ovl11SetSceneCollisionTolerance(void);

/* Copy the pending setup fields into installed work state, load the indexed
 * area resources, initialize projected geometry, install coordinate clamps,
 * enable collision cells and clear both render-context RGB values. The second
 * argument is unused; only low 16 bits of alreadyLoaded reach the loader.
 * Header kind 15 selects triplet reset (ID E5) or time-of-day fade; kind 44
 * resets the background color cycle. Other kinds add no follow-up.
 * Requires initialized loader/heap/staging/render data and valid resourceIndex;
 * inherits the loader's synchronous I/O, unchecked capacities and retry loops. */
void Ovl11InstallSceneSetup(u16 resourceIndex, u16 unused, s32 alreadyLoaded);

/* Store RGB modulo 256 in BOTH render contexts and mark each color flag 1.
 * Does not submit GPU commands, switch contexts or wait for drawing. */
void Ovl11SetRenderContextColor(s32 red, s32 green, s32 blue);

#endif
