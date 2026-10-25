#include "common.h"
#include "game_types.h"
#include "message_window.h"

void ovl_11_func_800FA410(s32 arg0, s32 arg1, s16 arg2, s16 arg3, s16 arg4) {
    Recon_ovl_11_func_800FA31C_D80126F8CEntry *p;
    s32 i;
    s16 var;

    if (D_80126F80 == 0xFF) {
        return;
    }
    var = 0x3DC;
    p = &D_80126F8C;
    i = 0x11;
    do {
        if (p->unk0 == arg3 && p->unk1 == arg4) {
            var = p->unk2;
        }
        i -= 1;
        p += 1;
    } while (i >= 0);
    if (((Ovl11Status5488View *)D_8006C838)->field_5488 == arg2 &&
        ((Ovl11Status5488View *)D_8006C838)->field_548A == arg3 &&
        ((Ovl11Status5488View *)D_8006C838)->field_548C == arg4) {
        var = 0x3DD;
    }
    RequestMessageWindow(3, var);
    if (GetMessageWindowStatus() == 2) {
        D_80126F80 = 0xFF;
        CloseMessageWindow();
    }
}
