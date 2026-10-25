#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "screen_fade.h"
#include "message_window.h"


void ovl_21_func_800BA4C0 (void);
void ovl_21_func_800B8320 (void);
void ovl_21_func_800B836C (void);
void ovl_21_func_800BA510 (void);
void ovl_21_func_800BA5D0 (void);
void ovl_21_func_800BA670 (void);

void ovl_21_func_800B94D4(void) {
    ovl_21_func_800BA4C0();
    if (IsScreenFadeComplete() == 1) {
        ovl_21_func_800B8320();
        BeginScreenFadeDecrease(0xA, 0, 2);
        RequestMessageWindow(4, 0x28);
        *D_800C0448 = 0xE;
    }
}
