#ifndef OVL19_SCENE_RESOURCES_H
#define OVL19_SCENE_RESOURCES_H

#include "common.h"

/* Scene-entry setup steps, called in this order by the overlay initializer.
 * The three archive steps wait for DrawSync, clear the active 0x800-entry OT,
 * and busy-poll CD reads with no timeout. They overwrite graphics-heap scratch
 * starting at +0x4290 and upload texture data. Requires a valid active render
 * context and enough heap/work storage; no capacity checks are performed.
 * Camera update/draw/query interfaces remain in ovl19_scene_camera_geometry.h. */

/* Archive 0, byte range +0x25000..+0x2A800: retain/mirror 0x13DC bytes of
 * sprite data and initialize the scene's SpriteSourceData header. */
void Ovl19LoadSceneSpriteArchive(void);

/* Archive 0, first 0x2000 bytes: upload the standalone texture chunk. */
void Ovl19UploadSceneSpriteChunk(void);

/* Archive 0x14, first 0x1AE04 bytes: retain a 0x104C-byte sprite-data blob,
 * upload its textures, and bind UI, three actor and two aux-actor sources. */
void Ovl19LoadUiAndActorSprites(void);

/* Read two consecutive disc ranges from the scene position table, upload
 * textures, copy 0xBDC bytes of scene data, and initialize/enable the geometry
 * cells and collision tolerance (0xC8). Must precede geometry draw/query. */
void Ovl19LoadSceneGeometry(void);

/* Rewind sound slots 2 and 1; queue resource IDs 0x3E7 and 0xF.
 * This only queues loading. The caller waits for transfer completion before
 * opening sequences/starting playback; it must then initialize the camera. */
void Ovl19QueueSceneSound(void);

#endif
