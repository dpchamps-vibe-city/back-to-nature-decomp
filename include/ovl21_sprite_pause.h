#ifndef OVL21_SPRITE_PAUSE_H
#define OVL21_SPRITE_PAUSE_H

#include "common.h"

/* Requires the scene sprite sources to have been initialized. Sets/clears
 * animation-pause mask 0x4 on all sixteen primary sources and all eighteen
 * record-slot sources (six records, three slots each), including inactive
 * slots. Other flags, frames and counters are unchanged. Drawing is not
 * disabled: UpdateSpriteAnimation simply skips paused sources. */
void Ovl21PauseSceneSprites(void);
void Ovl21ResumeSceneSprites(void);

#endif
