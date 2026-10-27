#ifndef AREA_SCENE_PREPARE_HOOK_H
#define AREA_SCENE_PREPARE_HOOK_H

#include "common.h"

/* Retail no-op invoked first by Ovl11PrepareAreaScene, before it examines
 * the area work flags. No inputs, return value, or memory side effects.
 * It does not initialize, clear, or validate the area's shared work storage.
 */
void AreaScenePrepareHook(void);

#endif
