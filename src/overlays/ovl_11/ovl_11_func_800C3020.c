#include "common.h"
#include "game_types.h"

void ovl_11_func_800C2678(Ovl11C3B78Arg0 *arg0);
void ovl_11_func_800C2C74(Ovl11C3B78Arg0 *arg0);

s32 ovl_11_func_800C3020(Ovl11C3B78Arg0 *arg0) {
    switch (*(u16 *)((u8 *)arg0 + 0x1A)) {
    case 2:
    case 5:
    case 6:
        if (!(*(u16 *)((u8 *)arg0 + 0x1E) & 4)) {
            if (*(s16 *)((u8 *)arg0 + 0x10) == 0) {
                ovl_11_func_800C2678(arg0);
            } else {
                if (--*(s16 *)((u8 *)arg0 + 0x10) == 0) {
                    ovl_11_func_800C2C74(arg0);
                }
            }
        }
        break;
    }
    return 1;
}
