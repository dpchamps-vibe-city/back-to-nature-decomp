#include "common.h"
#include "ovl11_clock_calendar.h"

s32 ovl_11_func_800BCB5C(void) {
    s16 status;
    u16 result;

    if (((struct Ovl11BCB5CState *) D_8006C838)->flags4450 & 0x80) {
        return 0xF;
    }
    if (((struct Ovl11BCB5CState *) D_8006C838)->flags4450 & 0x800) {
        return 0xC;
    }
    if (((struct Ovl11BCB5CState *) D_8006C838)->flags4450 & 0x100) {
        return 0xF;
    }
    if (((struct Ovl11BCB5CState *) D_8006C838)->flags4450 & 0x200) {
        return 0xC;
    }
    if (((struct Ovl11BCB5CState *) D_8006C838)->flags4450 & 0x400) {
        return 0xC;
    }
    if (((struct Ovl11BCB5CState *) D_8006C838)->flags0C & 0x2000) {
        return 0x1F;
    }
    status = ((struct Ovl11SpriteWork *) &D_8007AFF0)->header[7];
    if (status == 0x1D) {
        return 0x1D;
    }
    if (status == 0xE) {
        return 0xE;
    }
    if ((u32) ((u16) ((struct Ovl11BCB5CState *) D_8006C838)->fieldE4C8 - 3) < 2U) {
        return 0x17;
    }
    if (((struct Ovl11BCB5CState *) D_8006C838)->fieldE4C8 == 1) {
        return 0x16;
    }
    if (((struct Ovl11BCB5CState *) D_8006C838)->fieldE4C8 == 2) {
        return ((struct Ovl11BCB5CState *) D_8006C838)->field44BA + 1;
    }
    if (status == 0x18) {
        if (Ovl11ClassifyClockHour(((struct Ovl11BCB5CState *) D_8006C838)->field44C0) == 3) {
            return 0x18;
        }
        return 0x19;
    }
    if (Ovl11ClassifyClockHour(((struct Ovl11BCB5CState *) D_8006C838)->field44C0) == 3) {
        return 0x1C;
    }
    if (((struct Ovl11SpriteWork *) &D_8007AFF0)->header[7] == 0x3E7) {
        if (((struct Ovl11BCB5CState *) D_8006C838)->fieldE4C8 == 0) {
            return ((struct Ovl11BCB5CState *) D_8006C838)->field44BA + 1;
        }
    }
    result = (u16) ((struct Ovl11SpriteWork *) &D_8007AFF0)->header[7];
    if ((u32) (result - 0x1F) < 6U) {
        return (s16) result;
    }
    return 0x1B;
}
