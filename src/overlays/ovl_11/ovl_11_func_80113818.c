#include "common.h"
#include "screen_fade.h"

s32 ovl_11_func_80118B84(s32 arg0, s16 arg1, s16 arg2);
void func_8001AC10(u32 *arg0, s32 arg1, s32 arg2);
void func_8001AF70(u16 arg0, u16 arg1);
void ovl_11_func_800CD670(s16 arg0);
void ovl_11_func_80112D10(void);

void ovl_11_func_80113818(void) {
    Ovl11TriggerWorkView *work;
    s16 mode;
    s32 object;
    s32 result;
    s32 dataOffset;

    if (((Ovl11GridState *)D_8012D110)->flags & 0xC000) {
        dataOffset = 0;
        if (((Ovl11GridState *)D_8012D110)->flags & 0x4000) {
            dataOffset = 0x34;
        }
        result = ovl_11_func_80118B84(D_8005E3C0->field_D8 + 0x3C, 0x20, 0x20);
        object = D_8005E3C0->field_D8;
        func_8001AC10((u32 *)(object + 0x3C), object + 0x28,
                      (s32)(D_80128148 + dataOffset));
        if (result == 1) {
            work = (Ovl11TriggerWorkView *)&D_8007AFF0;
            work->field_254A0 = 5;
            BeginScreenFadeFeedback(0xA);
            if (((Ovl11GridState *)D_8012D110)->flags & 0x8000) {
                work->field_25480++;
                (*(u16 *)D_8012D110)++;
                func_8001AF70(0x44, 1);
                ((Ovl11GridState *)D_8012D110)->flags |= 0x2000;
            } else {
                mode = work->field_25474;
                if (mode == 0x37) {
                    work->field_25480 = 0x44;
                    func_8001AF70(0x43, 1);
                }
                if (work->field_25474 == 0x43) {
                    work->field_25480 = 0x48;
                    func_8001AF70(0x42, 1);
                }
                ovl_11_func_80112D10();
            }
            ((Ovl11GridState *)D_8012D110)->flags &= ~0xC000;
            ((struct struct_8006C838_view *)D_8006C838)->field_0C &= ~0x10000000;
            ((struct struct_8006C838_view *)D_8006C838)->field_0C &= ~0x08000000;
            func_8001AF70(3, 0);
        } else if (result == 2) {
            ((Ovl11GridState *)D_8012D110)->flags &= ~0xC000;
            ((struct struct_8006C838_view *)D_8006C838)->field_0C &= ~0x10000000;
            ((struct struct_8006C838_view *)D_8006C838)->field_0C &= ~0x08000000;
            func_8001AF70(3, 0);
            ovl_11_func_800CD670(0xA);
        }
    }
}
