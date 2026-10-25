#include "common.h"
#include "game_types.h"
#include "screen_fade.h"
#include "message_window.h"

void ovl_21_func_800B8320(void);
void ovl_21_func_800B9798(s16 arg0, s16 arg1);
void ovl_21_func_800BA4C0(void);

void ovl_21_func_800B859C(void) {
    s32 ready;
    s32 state;

    ovl_21_func_800B8320();
    ovl_21_func_800B9798(0, 0);
    ovl_21_func_800B9798(1, 0);
    ovl_21_func_800BA4C0();
    ready = IsScreenFadeComplete();
    if (ready == 1) {
        if ((s16) D_800C0448_init.records[0].unk2 != 0) {
            RequestMessageWindow(4, 0x29);
            state = GetMessageWindowStatus();
            if (state == 2) {
                if (GetMessageWindowChoice() == ready) {
                    D_800C0448[0] = state;
                } else {
                    D_800C0448[0] = ready;
                }
            }
        } else {
            D_800C0448[0] = ready;
        }
    }
}
