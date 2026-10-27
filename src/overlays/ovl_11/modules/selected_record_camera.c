/*
 * selected_record_camera — reconstructed translation unit.
 *
 * Follow the selected object-position record with the scene camera target
 * when the object-program follow flag is enabled. Preserve the low-halfword
 * coordinate copies and the two sentinel selections.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_scene_value_commands.h"
#include "ovl11_selected_record_camera.h"

void Ovl11FollowSelectedRecordCamera(void) {
    u16 *recordCoordinates;
    char *cameraWork;

    if ((D_8006C844 & 0x400000) &&
        (Ovl11GetSelectedPositionRecord() != -2) &&
        (Ovl11GetSelectedPositionRecord() != -1)) {
        recordCoordinates = (u16 *)((char *)gObjectPositionRecordCoordinates +
            Ovl11GetSelectedPositionRecord() * 0x1D4);
        cameraWork = (char *)&D_8007AFF0;
        /* Each source component is a word; the camera keeps its low half. */
        *(s16 *)(cameraWork + 0x253B4) = recordCoordinates[0];
        *(s16 *)(cameraWork + 0x253B6) = recordCoordinates[2];
        *(s16 *)(cameraWork + 0x253B8) = recordCoordinates[4];
    }
}
