#ifndef OVL11_SCENE_HEADER_CAPTURE_H
#define OVL11_SCENE_HEADER_CAPTURE_H

#include "common.h"

/* Copy the bound scene header's first two halfwords to primary-object +0xFC.
 * For signed object codes 0x106/0x107 at +0x8A, also copy them to +0x4012;
 * otherwise leave that mirror untouched. The work-area header pointer must
 * already be bound and readable. Copies tolerate unaligned storage and do
 * not change the header, bind a new scene, or reset any other object field. */
void Ovl11CaptureSceneHeaderKey(void);

#endif
