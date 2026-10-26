#include "common.h"
#include "screen_fade.h"
#include "sound_commands.h"
#include "message_window.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "beveled_panel.h"

void ovl_17_func_800B91B8(void);
void ovl_17_func_800BA808(s32 arg0);
void func_80017A08(s32 arg0, s32 arg1);
void func_80017B3C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s16 ovl_17_func_800B9940(s32 arg0);
s16 *func_8001A970(s32 arg0, s16 *arg1, s32 arg2);

void ovl_17_func_800B8D88(void) {
    u32 *boxes;
    s32 text;
    u16 mode;
    s32 total;
    Ovl17SummaryView *work;

    boxes = (u32 *)(gActiveRenderContext->field_D8 + 0xC);
    text = gActiveRenderContext->field_D8 + 8;
    work = (Ovl17SummaryView *)D_8006C838;
    mode = 2;
    if (work->mode != 3) {
        mode = (u16)work->mode;
    }
    ovl_17_func_800B91B8();
    ovl_17_func_800BA808(0x800000);
    DrawScreenSubtractOverlay(0x40, 0x40, 0x40);
    func_80017A08(0, (s32)&D_800BB528[mode * 2]);
    DrawMenuPanel(boxes, 1, 0x28, 0x30, 0xF4, 0x72);
    func_80017B3C(text, (s32)D_800BB482, 0x3D, 0x32);
    total = ovl_17_func_800B9940(1);
    DrawMenuPanel(boxes, 1, 0xC3, 0x10, 0x6F, 0x1E);
    func_80017B3C(text, (s32)(D_800BB482 - 0x17), 0xC7, 0x12);
    *(u16 *)func_8001A970(work->value, (s16 *)D_800BD738, 7) = 0xFFFF;
    func_80017B3C(text, (s32)D_800BD738, 0xC5, 0x1E);
    RequestMessageWindow(4, 7);
    if (GetMessageWindowStatus() == 2) {
        if (total != 0) {
            Ovl17_D_800BD848.field_2C8 = PlaySoundEffect(0x3E);
        }
        Ovl17_D_800BD848.field_4 = 0xA;
    }
}
