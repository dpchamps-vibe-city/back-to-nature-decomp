#ifndef OVL11_AREA_SETUP_HOOK_H
#define OVL11_AREA_SETUP_HOOK_H

#include "common.h"

/* Area-scene setup extension point. Retail implementation is a no-op:
 * no inputs, return value, state, drawing, or resource lifetime effects.
 * Setup paths retain the original out-of-line call; this does not initialize
 * resources itself and is not a substitute for area_scene_load.
 */
void Ovl11AreaSetupHook(void);

#endif
