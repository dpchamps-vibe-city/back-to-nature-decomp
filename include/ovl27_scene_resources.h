#ifndef OVL27_SCENE_RESOURCES_H
#define OVL27_SCENE_RESOURCES_H

#include "common.h"

/*
 * Setup-dispatcher resource steps, ovl_27:0x800BA4C4..0x800BA914.
 * Requires an initialized archive index, active render context and graphics
 * heap. Not reentrant: reads share the CD byte-read channel and scratch.
 * Each Begin clears the active 2048-entry ordering table, then starts a read.
 * Poll returns 0 while pending and 1 when complete; do not interleave pairs.
 * Destinations must hold the sector-rounded transfer, not just payload bytes.
 */

/* Entry 0 bytes [0x2A800,0x73800), to D_8007AFF4; no upload here. */
void Ovl27BeginSceneDataRead(void);
s32 Ovl27PollSceneDataRead(void);

/* Entry 0 bytes [0x25000,0x2A800), to graphics heap +0x4290.
 * On completion upload images at +0x566C and copy the 0x13DC-byte header
 * bank at +0x4290 into gSharedSpriteData. */
void Ovl27BeginSharedSpriteRead(void);
s32 Ovl27PollSharedSpriteRead(void);

/* Entry 0 first 0x2000 bytes, to heap +0x4290; poll uploads its images. */
void Ovl27BeginImageBankRead(void);
s32 Ovl27PollImageBankRead(void);

/* Bind sound command buffer 0 to heap +0x4290 and queue 0/1000/26.
 * The dispatcher separately drives transfer completion and starts playback. */
void Ovl27QueueSceneSoundLoads(void);

/* Empty begin hook followed by synchronous page copy. Clamp the work-state
 * selector at +0x18 to 0..3, copy 4096 bytes from gOvl27PageSources to the
 * bound buffer at +0x1C plus 6, then replace the selector with the source id.
 * Buffer binding/capacity is a caller prerequisite. Copy always returns 1. */
void Ovl27BeginPageCopy(void);
s32 Ovl27CopySelectedPage(void);

#endif
