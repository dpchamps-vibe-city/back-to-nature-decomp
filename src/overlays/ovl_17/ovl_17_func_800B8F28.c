#include "common.h"
#include "screen_fade.h"

void ovl_17_func_800B91B8(void);
void ovl_17_func_800BA808(s32 arg0);
void func_80022580(u32 *arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5);
void func_80017B3C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s16 ovl_17_func_800B9940(s32 arg0);
s16 *func_8001A970(s32 arg0, s16 *arg1, s32 arg2);
s32 func_8002261C(s32 arg0, s32 arg1);
/* GetVal8005E5B4, named by its original overlay import. */
s32 GetVal8005E5B4(void);

void ovl_17_func_800B8F28(void) {
    u32 *boxes;
    s32 text;
    s32 total;
    Ovl17SummaryUpperView *work;
    Ovl17SummaryResultView *base;

    boxes = (u32 *)(gActiveRenderContext->field_D8 + 0xC);
    text = gActiveRenderContext->field_D8 + 8;
    ovl_17_func_800B91B8();
    ovl_17_func_800BA808(0x800000);
    DrawScreenSubtractOverlay(0x40, 0x40, 0x40);
    func_80022580(boxes, 1, 0x28, 0x30, 0xF4, 0x72);
    func_80017B3C(text, (s32)D_800BB482, 0x3D, 0x32);
    total = ovl_17_func_800B9940(0);
    base = (Ovl17SummaryResultView *)D_8006C838;
    total += base->value;
    if (total > 99999) {
        total = 99999;
    }
    func_80022580(boxes, 1, 0xC3, 0x10, 0x6F, 0x1E);
    func_80017B3C(text, (s32)(D_800BB482 - 0x17), 0xC7, 0x12);
    *(u16 *)func_8001A970(total, (s16 *)D_800BD738, 7) = 0xFFFF;
    func_80017B3C(text, (s32)D_800BD738, 0xC5, 0x1E);
    work = (Ovl17SummaryUpperView *)((u8 *)base + 0x8000);
    if (work->mode == 3 && D_800BDB04 == 2) {
        work->enabled = 1;
    } else {
        work->enabled = 0;
    }
    ((Ovl17SummaryResultView *)D_8006C838)->total = total;
    func_8002261C(4, 8);
    if (GetVal8005E5B4() == 2) {
        ((Ovl17SummaryResultView *)D_8006C838)->state++;
    }
}
