#include "common.h"
#include "game_types.h"

s32 ovl_11_func_80109188(Ovl11PrimaryStageHandlerObj *arg0, s32 arg1);
s32 ovl_11_func_800DF010(Ovl11StageHandlerObj *arg0, s32 arg1);
s32 ovl_11_func_800E0C24(Ovl11StageHandlerObj *arg0, s32 arg1);
s32 ovl_11_func_8010B57C(u16 *arg0, s32 arg1);

#define CURRENT_STAGE (((struct Ovl11D2764StateView *)&D_8007AFF0)->field_25476)
#define MAIN_STAGE (((struct Ovl11D2764StageView *)D_8006C838)->field_91DC)
#define OTHER_STAGE (((struct Ovl11D2764StageView *)D_8006C838)->field_92CC)

void ovl_11_func_800D2764(s32 arg0) {
    Ovl11StageHandlerObj *obj;
    s32 i;

    switch (arg0) {
    case 0:
        if (CURRENT_STAGE == MAIN_STAGE) {
            ovl_11_func_80109188((Ovl11PrimaryStageHandlerObj *)&D_800759E4, 5);
        }
        for (i = 0; i < 20; i++) {
            obj = (Ovl11StageHandlerObj *)((u8 *)&D_800749F4 + i * 0xB8);
            if (*(u16 *)obj != 0) {
                if (obj->unk34 & 0x10000) {
                    if (CURRENT_STAGE == *(s16 *)((u8 *)obj + 0x30)) {
                        if (ovl_11_func_800DF010(obj, 0xB) == 0) {
                            D_80128D60 = 0;
                        }
                    }
                } else if (CURRENT_STAGE == *(s16 *)((u8 *)obj + 0x30)) {
                    if (ovl_11_func_800E0C24(obj, 0xB) == 0) {
                        D_80128D60 = 0;
                    }
                }
            }
        }
        return;
    case 1:
        if ((CURRENT_STAGE == MAIN_STAGE) &&
            (ovl_11_func_80109188((Ovl11PrimaryStageHandlerObj *)&D_800759E4, 4) == 0)) {
            D_80128D62 = 0;
            return;
        }
        return;
    case 2:
        if ((CURRENT_STAGE == OTHER_STAGE) &&
            (ovl_11_func_8010B57C(&D_80075AD4, 0x13) == 0)) {
            D_80128D64 = 0;
        }
        break;
    }
}
