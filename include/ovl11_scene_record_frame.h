#ifndef OVL11_SCENE_RECORD_FRAME_H
#define OVL11_SCENE_RECORD_FRAME_H

#include "common.h"

/* Unless shared-work flag 0x4000 suppresses the service, refresh record
 * enables, append the scene-record sprites to the active render context,
 * then service the current record's sprite Y-offset countdown. The order
 * matters: this frame draws with the old offset before its decay.
 * Requires resident dispatch lists/records, sprite resources, active ordering
 * tables and sufficient packet storage. This constructs packets, not a GPU
 * submit. Suppression skips all three operations; no completion result. */
void Ovl11ServiceSceneRecordFrame(void);

#endif
