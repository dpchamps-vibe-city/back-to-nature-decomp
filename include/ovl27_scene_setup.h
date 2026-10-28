#ifndef OVL27_SCENE_SETUP_H
#define OVL27_SCENE_SETUP_H
#include "common.h"

/* Title-overlay scene entry, once per update. gScenePhase must index the
 * seven-entry resident phase table (unchecked). The setup phases reset local
 * stages, preserve the work selector across InitStateMemory when needed,
 * initialize title work records, load backdrop/shared/image/page resources,
 * and wait for screen/audio transitions before handing control to the menu.
 * Requires initialized render, CD, sound and message services. Uses shared
 * overlay/engine state and is not reentrant. */
void Ovl27DispatchTitleScenePhase(void);

#endif
