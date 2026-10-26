#ifndef OVL25_SCENE_DISPLAY_PIPELINE_H
#define OVL25_SCENE_DISPLAY_PIPELINE_H

#include "common.h"

/* Camera service: initialize once, then update offset, apply GTE camera and
 * draw in that order each frame. Positions are signed scene coordinates;
 * yaw uses the SDK's 4096-unit full turn. Update preserves the GTE matrix
 * stack; Apply changes the engine camera, Draw constructs geometry packets.
 * Shared far-work camera fields and descriptor make this non-reentrant. */
void Ovl25InitSceneCamera(void);
void Ovl25UpdateSceneCameraOffset(void);
void Ovl25ApplySceneCamera(void);
void Ovl25DrawSceneGeometry(void);

/* Ordered setup stages. Sprite reads block with no timeout; they DrawSync,
 * clear 0x800 OT entries and reuse gGraphicsHeapBase+0x4290 as scratch.
 * Keep enough scratch for the largest 0x124D0-byte read. Retained 0x13DC/
 * 0x1C88-byte bank copies back the initialized sources after scratch reuse.
 * Geometry loads two unchecked disc-sector runs, publishes the 0xBDC-byte
 * area-header copy to shared far work, initializes gOvl25SceneGeometry and
 * sets collision tolerance to 200. Sound only queues requests; the outer
 * setup dispatcher waits for transfer completion. Exclusive CD/heap/OT use
 * is required; these stages are not safe during active scene drawing. */
void Ovl25LoadSharedSpriteArchive(void);
void Ovl25UploadCommonSpriteImages(void);
void Ovl25LoadObjectSpriteSet(void);
void Ovl25LoadSceneGeometry(void);
void Ovl25QueueSceneSoundLoads(void);

#endif
