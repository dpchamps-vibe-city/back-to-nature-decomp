/* Retail area-scene preparation hook (exe:80021D64).
 * Ovl11PrepareAreaScene calls this before examining the shared work flags.
 * It has no retail effects and does not initialize the work storage.
 */
#include "area_scene_prepare_hook.h"

void AreaScenePrepareHook(void) {
    /* Preserve the original empty hook's sixteen-byte stack allocation. */
    char unusedFrame[16];
}
