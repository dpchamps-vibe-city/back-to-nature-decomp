#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"


void ovl_11_func_800C02B0 (void);
s32 ovl_11_func_800C0834 (s16 arg0);
void ovl_11_func_800C0010 (s32 arg0, s32 arg1);
void ovl_11_func_800BFF74 (void);
void func_8001316C (u32 arg0, u32 arg1, u32 arg2);
void ovl_11_func_800C009C (u8 *arg0);

void ovl_11_func_800BFD1C(void) {
    s32 temp_v0;
    u16 temp_a1;
    u8 *base;
    u8 *work;
    u8 *tail;

    temp_a1 = ((u16 *) ((struct Ovl11SpriteWork *) &D_8007AFF0)->header)[4];
    if (!(temp_a1 & 0x800) && (((struct Ovl11SpriteWork *) &D_8007AFF0)->header[1] != 6)) {
        if (!(temp_a1 & 0x1000)) {
            ovl_11_func_800C02B0();
        }
        base = (u8 *) D_8006C838;
        if ((*(s32 *) (base + 0xC)) & 0x02000000) {
            work = base + 0x8000;
            temp_v0 = ovl_11_func_800C0834(*(s16 *) (base + 0x44C0));
            if ((*(s32 *) (work + 0x6768)) != temp_v0) {
                ovl_11_func_800C0010(0x100, (s32) &D_80128A80[temp_v0 * 3]);
                (*(s32 *) (work + 0x6768)) = temp_v0;
            }
        }
        tail = (u8 *) D_8006C838;
        if (!((*(s32 *) (tail + 0xC)) & 0x10200000) && !(((u16 *) ((struct Ovl11SpriteWork *) &D_8007AFF0)->header)[4] & 0x1000)) {
            ovl_11_func_800BFF74();
        }
    }
}
