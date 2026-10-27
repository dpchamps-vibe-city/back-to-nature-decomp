#ifndef OVL11_SELECTED_RECORD_CAMERA_H
#define OVL11_SELECTED_RECORD_CAMERA_H

#include "common.h"

/* If shared-work +0x0C bit 0x400000 is set and the selected position record
 * is neither -2 nor -1, copy that record's XYZ low halfwords into the scene
 * camera target at far-work +0x253B4/+0x253B6/+0x253B8. The record stride is
 * 0x1D4 bytes; other indices are unchecked and must identify a valid record.
 * Coordinate scale is unchanged (no conversion or interpolation). This
 * updates the target only, not the camera eye, matrices or record itself.
 * Otherwise it leaves the previous target intact. */
void Ovl11FollowSelectedRecordCamera(void);

#endif
