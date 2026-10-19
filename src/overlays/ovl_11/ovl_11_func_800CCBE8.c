#include "common.h"
#include "game_types.h"

void ovl_11_func_800CBD78(Ovl11C7010Object *arg0);

void ovl_11_func_800CCBE8(Ovl11CC4E0Object *arg0) {
    s16 temp_a0;

    temp_a0 = *(s16 *) ((u8 *) arg0 + 2);
    (*(s16 *) ((u8 *) arg0 + 2)) = 0;
    ((struct struct_8006C838_800E8F78 *) D_8006C838)->field_5234 &= ~0x40;
    if (temp_a0 == 0x186) {
        D_8012DB78.x = *(u16 *) ((u8 *) arg0 + 0x24);
        D_8012DB78.y = (*(u16 *) ((u8 *) arg0 + 0x28)) - 0xA;
        D_8012DB78.z = *(u16 *) ((u8 *) arg0 + 0x2C);
        return;
    }
    ovl_11_func_800CBD78((Ovl11C7010Object *) ((u8 *) D_8006C838 + 0x51C8));
}
