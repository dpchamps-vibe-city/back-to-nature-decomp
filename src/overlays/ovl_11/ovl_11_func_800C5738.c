#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"

void ovl_11_func_800CE034(Ovl11CE034Object *arg0, Ovl11CE034Position *arg1, s32 arg2);
s32 ovl_11_func_800BDB88(s32 *arg0, s32 arg1);
s32 ovl_11_func_800CB114(Ovl11CB114Object *arg0, s32 *arg1, s16 arg2);

void ovl_11_func_800C5738(Ovl11C498CObject *arg0) {
    VECTOR position;
    s32 result;
    u16 direction;
    u32 i;

    direction = arg0->direction;
    for (i = 0; i < 4U; i++) {
        arg0->direction = i;
        position = *(VECTOR *)arg0->position;
        ovl_11_func_800CE034((Ovl11CE034Object *)arg0,
                           (Ovl11CE034Position *)&position, 2);
        result = ovl_11_func_800BDB88((s32 *)&position, 0xC080);
        if ((result | ovl_11_func_800CB114((Ovl11CB114Object *)arg0,
                                        (s32 *)&position, 0x18)) == 0) {
            *(VECTOR *)arg0->position = position;
            return;
        }
    }
    arg0->direction = direction;
}
