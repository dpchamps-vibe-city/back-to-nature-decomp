#ifndef OVL21_SCENE_RESOURCES_H
#define OVL21_SCENE_RESOURCES_H

#include "common.h"

/* Scene-entry setup steps, called in this order by the overlay initializer.
 * The three archive steps wait for DrawSync, clear the active 0x800-entry OT
 * and busy-poll CD reads without a timeout. They overwrite graphics-heap
 * scratch at +0x4290 and upload textures. Requires a valid render context,
 * initialized graphics heap and enough work storage; no capacity checks.
 * Camera/drawing interfaces remain in ovl21_scene_camera_geometry.h. */

/* Archive 0, byte range +0x25000..+0x2A800: retain/mirror 0x13DC bytes,
 * upload the following image data and bind the primary scene sprite. */
void Ovl21LoadSceneSpriteArchive(void);

/* Archive 0, first 0x2000 bytes: upload the standalone texture chunk. */
void Ovl21UploadSceneSpriteChunk(void);

/* Archive 0x16, first 0x16188 bytes: retain 0x233C bytes, upload textures
 * and bind 15 UI sources using the offset table. Also bind three embedded
 * sources in each of six 0x108-byte records, all to offset-table entry zero.
 * The UI anchor is a 0x30-byte source lvalue, not a new array allocation. */
void Ovl21LoadUiAndObjectSprites(void);

/* Read two disc ranges, upload textures, copy 0xBDC bytes of scene data,
 * publish the geometry descriptor and enable its cells; set collision
 * tolerance to 0xC8. Must precede Ovl21DrawSceneGeometry and surface queries.
 * Shared work at D_8007AFF0 and graphics-heap scratch are overwritten. */
void Ovl21LoadSceneGeometry(void);

/* Rewind sound slots 2 and 1; queue resource IDs 0x3E7 and 0x12.
 * Queue-only: the caller must wait for IsSoundTransferComplete, then perform
 * its remaining scene setup and Ovl21InitSceneCamera before regular updates. */
void Ovl21QueueSceneSound(void);

#endif
