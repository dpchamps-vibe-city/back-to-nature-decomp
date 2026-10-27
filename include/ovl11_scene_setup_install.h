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

/* ovl_11:800DC148. Update the installed header's scene effects once per call.
 * Kind 15 steps the setup-selected triplet/fade, kind 44 steps its color cycle,
 * kinds 1/6 draw visible/fixed cell sprites; kind 2 sets work flag 0x10000.
 * Kind 1 consumes sound request 0x4000, choosing CD track 0x13 for a matching
 * inactive type-109 record, overridden by track 0x14 on flag 0x8000.
 * Modes 0xC/0x12 start track 0x15 once, guarded by flags 0x40000/0x80000.
 * Always step both animation services, both rotation ramps and the visibility
 * pair, even for other kinds. Requires installed scene/resources and ready
 * graphics/audio services; no clock tick, scene load or return value. */
void Ovl11UpdateSceneEffects(void);

#endif
