#include "common.h"
#include "ovl11_clock_calendar.h"
#include "ovl11_clock_hud.h"
#include "game_flags.h"

typedef struct {
    /* 0x000 */ char pad_000[0x0C];
    /* 0x00C */ s32 field_0C;
    /* 0x010 */ char pad_010[0xE4C2];
    /* 0xE4D2 */ s16 field_E4D2;
} OvC188CView;

void ovl_11_func_800C188C(void) {
    char *far_base;
    s32 temp;

    if (Ovl11IsClockAdvanceActive() == 1) {
        Ovl11DisableClockHud();
        return;
    }
    if (((OvC188CView *)D_8006C838)->field_0C & 0x10000000) {
        return;
    }
    temp = GetGameFlag(2);
    if (temp == 1) {
        far_base = (char *)&D_8007AFF0;
        if (*(u16 *)(*(s32 *)(far_base + 0x25388) + 8) & 0x1000) {
            if (GetGameFlag(0xB0) == 1) {
                Ovl11DisableClockHud();
                return;
            }
            if (*(u16 *)(*(s32 *)(far_base + 0x25388) + 8) & 0x1000) {
                goto block_9;
            }
        }
        if (GetGameFlag(5) == 1) {
            Ovl11DisableClockHud();
            return;
        }
        if (GetGameFlag(4) == 1) {
            Ovl11EnableClockHud();
            return;
        }
    }
block_9:
    if (((OvC188CView *)D_8006C838)->field_E4D2 != 0) {
        Ovl11EnableClockHud();
        return;
    }
    Ovl11DisableClockHud();
}
