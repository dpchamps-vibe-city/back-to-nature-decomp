#ifndef OVL11_SCENE_HEADER_KEY_H
#define OVL11_SCENE_HEADER_KEY_H

#include "common.h"

/* Return 0 when both halfwords of key match the first two halfwords of the
 * currently bound scene header, 1 otherwise. The key must address two valid
 * signed halfwords and the work-area header pointer must already be bound.
 * No pointer/null checks, coordinate conversion or state changes occur.
 * The key is read only; its original non-const pointer ABI is retained. */
s32 Ovl11SceneHeaderKeyDiffers(s16 *key);

#endif
