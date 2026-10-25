#include "common.h"
#include "screen_fade.h"

void ovl_11_func_800BDA20(void);
void ovl_11_func_80121134(void);
void ovl_11_func_800DB7F0(void);
void ovl_11_func_800D78D0(void);

void ovl_11_func_80120A2C(void) {
    s32 upOffset;
    s32 downOffset;
    s32 leftOffset;
    s32 rightOffset;

    switch (((Ovl11MapCameraState *)&D_8012DB58)->unk0) {
    case 0:
        ((struct Ovl11MapCameraWork *)&D_8007AFF0)->field_25394 = -0xB6E;
        ((struct Ovl11MapCameraWork *)&D_8007AFF0)->field_25398 = -0x7454;
        ((struct Ovl11MapCameraWork *)&D_8007AFF0)->field_253A0 = 0;
        ((struct struct_8006C838_view *)D_8006C838)->field_0C |= 0x800000;
        ((Ovl11MapCameraState *)&D_8012DB58)->unk0++;
        return;
    case 1:
        ovl_11_func_800BDA20();
        ovl_11_func_80121134();
        BeginScreenFadeDecrease(10, 0, 2);
        ((Ovl11MapCameraState *)&D_8012DB58)->unk0++;
        ((struct struct_8006C838_view *)D_8006C838)->field_0C &= ~0x800000;
        return;
    case 2:
        if (((u32 *)D_8005E3A8)[1] & 0x1000) {
            upOffset = ((Ovl11MapCameraState *)&D_8012DB58)->unk14;
            if (upOffset < 400) {
                upOffset += 200;
            }
            ((Ovl11MapCameraState *)&D_8012DB58)->unk14 = upOffset;
        }
        if (((u32 *)D_8005E3A8)[1] & 0x4000) {
            downOffset = ((Ovl11MapCameraState *)&D_8012DB58)->unk14;
            if (downOffset > -7200) {
                downOffset -= 200;
            }
            ((Ovl11MapCameraState *)&D_8012DB58)->unk14 = downOffset;
        }
        if (((u32 *)D_8005E3A8)[1] & 0x8000) {
            leftOffset = ((Ovl11MapCameraState *)&D_8012DB58)->unkC;
            if (leftOffset > -800) {
                leftOffset -= 200;
            }
            ((Ovl11MapCameraState *)&D_8012DB58)->unkC = leftOffset;
        }
        if (((u32 *)D_8005E3A8)[1] & 0x2000) {
            rightOffset = ((Ovl11MapCameraState *)&D_8012DB58)->unkC;
            if (rightOffset < 10000) {
                rightOffset += 200;
            }
            ((Ovl11MapCameraState *)&D_8012DB58)->unkC = rightOffset;
        }
        ((struct Ovl11MapCameraWork *)&D_8007AFF0)->field_253B4 = ((Ovl11MapCameraState *)&D_8012DB58)->unkC;
        ((struct Ovl11MapCameraWork *)&D_8007AFF0)->field_253B8 = ((Ovl11MapCameraState *)&D_8012DB58)->unk14;
        if (((u32 *)D_8005E3A8)[2] & 0x20) {
            BeginScreenFadeIncrease(10, 0, 2);
            ((Ovl11MapCameraState *)&D_8012DB58)->unk0 = 3;
            return;
        }
        return;
    case 3:
        if (IsScreenFadeComplete() == 1) {
            BeginScreenFadeDecrease(10, 0, 2);
            ovl_11_func_800DB7F0();
            ovl_11_func_800D78D0();
            ((struct Ovl11MapCameraWork *)&D_8007AFF0)->field_2548C = 0;
            ((struct struct_8006C838_view *)D_8006C838)->field_0C |= 0x800000;
        }
        break;
    }
}
