#include "common.h"
#include "game_types.h"

s32 ovl_11_func_800D5810(s16 arg0);
void ovl_11_func_800CB618(void);
void ovl_11_func_800C9D98(void);
s32 ovl_11_func_800C9DE8(s32 arg0);
s32 ovl_11_func_800C7270(Recon_ovl_11_func_800C7270_A0View *arg0, s32 arg1, s32 arg2, s32 arg3);
void ovl_11_func_800D7348(u16 *dst, u16 *src);
s32 func_8001FABC(s16 arg0);

void ovl_11_func_800C8E98(Recon_ovl_11_func_800C7270_A0View *arg0) {
    s16 (*entries)[3];
    s32 empty;
    u16 item;
    u32 i;
    s16 *selected;

    empty = 1;
    entries = (s16 (*)[3])((u8 *)arg0 + 0xC0);
    for (i = 0; i < 8U; i++) {
        if (entries[i][0] != 0) {
            empty = 0;
        }
    }
    item = *(u16 *)((u8 *)arg0 + 0x8A);
    if ((empty == 0) || (item != 0)) {
        selected = (s16 *)((u8 *)arg0 + 0x8A);
        if (*selected != 0) {
            if (ovl_11_func_800D5810(*selected) == 1) {
                ovl_11_func_800CB618();
            }
        } else {
            arg0->unk6C &= 0xE7FFFFFF;
            ovl_11_func_800C9D98();
            if ((ovl_11_func_800C9DE8(0) != -1) && (*(s16 *)((u8 *)arg0 + 0x8A) != 0)) {
                ovl_11_func_800C7270(arg0, 0xA, *(u16 *)((u8 *)arg0 + 0x38) + 0xB, -2);
                ovl_11_func_800D7348(&arg0->unkF0, (u16 *)selected);
                arg0->unk6C |= 0x8000;
                func_8001FABC(7);
            }
        }
    }
}
