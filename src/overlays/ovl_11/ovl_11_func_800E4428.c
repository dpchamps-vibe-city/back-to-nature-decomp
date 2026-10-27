#include "common.h"
#include "ovl11_scene_value_commands.h"

extern s32 gObjectPositionRecordCoordinates[];

void ovl_11_func_800E4428(void) {
    u16 *p;
    char *far_base;

    if ((D_8006C844 & 0x400000) && (Ovl11GetSelectedPositionRecord() != -2) && (Ovl11GetSelectedPositionRecord() != -1)) {
        p = (u16 *)((char *)gObjectPositionRecordCoordinates + Ovl11GetSelectedPositionRecord() * 0x1D4);
        far_base = (char *)&D_8007AFF0;
        *(s16 *)(far_base + 0x253B4) = p[0];
        *(s16 *)(far_base + 0x253B6) = p[2];
        *(s16 *)(far_base + 0x253B8) = p[4];
    }
}
