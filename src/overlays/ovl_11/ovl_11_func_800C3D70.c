#include "common.h"
#include "game_flags.h"
#include "psyq/memory.h"
#include "ovl11_object_event_pool.h"

void ovl_11_func_800BDA70(void);
void ovl_11_func_800BD6A8(s16 arg0, s16 arg1);
void ovl_11_func_800C19B0(void);

void ovl_11_func_800C3D70(void) {
    s16 temp_a1;

    if (!((u16)((struct Ovl11SpriteWork *)&D_8007AFF0)->header[4] & 0x1000) && (temp_a1 = ((struct Ovl11SpriteWork *)&D_8007AFF0)->header[1], (D_80125F24[temp_a1] != 0x1000))) {
        if (temp_a1 == 1) {
            if ((D_80070D02 == 0x438) && (GetGameFlag(0x40U) == 0)) {
                ovl_11_func_800BDA70();
                SetGameFlag(0x40U, 1U);
            }
        } else {
            if (((struct Ovl11CCCC0State *)D_8006C838)->angle == 0x2D0) {
                Ovl11ClearEventSlotTimeOrigins();
                if (GetGameFlag(0x3FU) == 0) {
                    if (!(((struct Ovl11CCCC0State *)D_8006C838)->flags & 0x10)) {
                        ovl_11_func_800BD6A8(0xE, 1);
                        ovl_11_func_800C19B0();
                    }
                    SetGameFlag(0x3FU, 1U);
                }
            }
            if (((struct Ovl11CCCC0State *)D_8006C838)->angle == 0x438) {
                Ovl11ClearEventSlotTimeOrigins();
                if (GetGameFlag(0x40U) == 0) {
                    if (!(((struct Ovl11CCCC0State *)D_8006C838)->flags & 0x10)) {
                        ovl_11_func_800BD6A8(0xE, 1);
                        ovl_11_func_800C19B0();
                    }
                    SetGameFlag(0x40U, 1U);
                }
            }
        }
    }
}
