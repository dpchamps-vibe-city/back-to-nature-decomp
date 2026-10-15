#include "common.h"
#include "game_types.h"

s32 func_800226A4(void);
void func_800226D8(s32 arg0);
void func_80017B18(s32 arg0);
s32 func_8002261C(s32 arg0, s32 arg1);
void func_80022738(void);
void ovl_11_func_8011D098(s32 arg0, s32 arg1, s32 arg2);
void ovl_11_func_8011D0B4(void);
s32 ovl_11_func_8011D734(Ovl11D548Arg *arg0);
s16 ovl_11_func_8011D9B4(StructD548 x);

s32 ovl_11_func_8011B840(s32 *arg0, s32 arg1, Ovl11D8012D7B8 arg2) {
    s32 result;
    StructD548 *record;
    s16 startKind;
    s16 startId;
    s16 cancelKind;
    s16 cancelId;
    s32 state;

    if ((u32)(*arg0 - arg1) >= 6U) {
        return -1;
    }
    result = 0;
    record = (StructD548 *)arg2.unk0;
    startKind = arg2.unk4;
    startId = arg2.unk6;
    cancelKind = arg2.unk8;
    cancelId = arg2.unkA;
    state = arg2.unk10;
    switch (*arg0 - arg1) {
    case 0:
        if (startKind != -1) {
            func_8002261C(startKind, startId);
            ovl_11_func_8011D098(1, 3, arg1);
        } else {
            ovl_11_func_8011D098(3, 0, arg1);
        }
        break;
    case 1:
        if (func_800226A4() == 2) {
            ovl_11_func_8011D0B4();
        }
        break;
    case 2:
        if (func_800226A4() == 5) {
            ovl_11_func_8011D0B4();
        }
        break;
    case 3:
        func_800226D8(0);
        func_80017B18(0);
        if (D_8012D52C == 7) {
            D_8012D52C = state;
            if (func_8002261C(2, 0x5F) == 0) {
                ovl_11_func_8011D098(2, 4, arg1);
            }
        } else {
            D_8012D52C = state;
            if (func_8002261C(2, 0x46) == 0) {
                ovl_11_func_8011D098(2, 4, arg1);
            }
        }
        break;
    case 4:
        switch (ovl_11_func_8011D734((Ovl11D548Arg *)record)) {
        case 0:
            break;
        case 1:
            result = 1;
            func_80022738();
            break;
        case 2:
            if (ovl_11_func_8011D9B4(*record) != -1) {
                func_80022738();
                func_8002261C(2, ovl_11_func_8011D9B4(*record));
                ovl_11_func_8011D098(1, 3, arg1);
            }
            break;
        case 3:
            func_80022738();
            func_8002261C(cancelKind, cancelId);
            ovl_11_func_8011D098(1, 5, arg1);
            break;
        }
        break;
    case 5:
        result = 3;
        break;
    }
    return result;
}
