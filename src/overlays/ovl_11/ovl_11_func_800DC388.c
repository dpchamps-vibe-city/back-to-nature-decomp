#include "common.h"
#include "ovl11_clock_calendar.h"

s32 ovl_11_func_800E3D88(void);
s32 ovl_11_func_800DD274(void);
s32 func_8001AF44(u32 arg0);

s32 ovl_11_func_800DC388(s32 arg0) {
    s32 id;

    id = arg0;
    if (ovl_11_func_800E3D88() == 0x12D) {
        return gCalendarSeason + 0xBC;
    }
    if (id == 0) {
        id = ovl_11_func_800DD274();
    }
    if (id == 0x50) {
        s32 variant = ((struct Ovl11MessageState *)D_8006C838)->variant50 * 4 + 0x50;
        id = ((struct Ovl11MessageState *)D_8006C838)->base + variant;
    }
    if (id == 0x70) {
        id = ((struct Ovl11MessageState *)D_8006C838)->variant70 * 4 + ((struct Ovl11MessageState *)D_8006C838)->base + 0x70;
    }
    if (id == 0x60) {
        id = ((struct Ovl11MessageState *)D_8006C838)->variant60 * 4 + ((struct Ovl11MessageState *)D_8006C838)->base + 0x60;
    }
    if (((struct Ovl11SpriteWork *)&D_8007AFF0)->header[0] == 0x43) {
        if ((func_8001AF44(0x44U) == 0) && (((struct Ovl11SpriteWork *)&D_8007AFF0)->header[1] == 0x44)) {
            id = gCalendarSeason + 0x48;
        }
        if (func_8001AF44(0x42U) == 1) {
            id = gCalendarSeason + 0x48;
        }
    }
    if (((struct Ovl11SpriteWork *)&D_8007AFF0)->header[0] == 0x37) {
        if ((func_8001AF44(0x44U) == 0) && (((struct Ovl11SpriteWork *)&D_8007AFF0)->header[1] == 0x38)) {
            id = gCalendarSeason + 0x44;
        }
        if (func_8001AF44(0x43U) == 1) {
            id = gCalendarSeason + 0x44;
        }
    }
    return Ovl11RemapCalendarValue((s16)id);
}
