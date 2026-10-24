#include "common.h"
#include "game_types.h"

s32 GetVal8005E5B4(void);
void SetVal8005E334(s32 arg0);
void SetVal8005E2BC(s32 arg0);
s32 func_8002261C(s32 arg0, s32 arg1);
void func_80022738(void);
s32 ovl_11_func_800D6014(u16 arg0);
s32 ovl_11_func_8011CF40(s16 arg0, u16 *arg1);
void ovl_11_func_8011D098(s32 arg0, s32 arg1, s32 arg2);
void ovl_11_func_8011D0B4(void);
s16 ovl_11_func_8011D98C(StructD548 x);
s16 ovl_11_func_8011D9B4(StructD548 x);
void ovl_11_func_8011DD48(ovl_11_DD48_arg0 *arg0, s32 arg1, s16 arg2, s16 arg3, s16 arg4);

s32 ovl_11_func_8011BAFC(s32 *arg0, s32 arg1, s32 arg2, Ovl11BAFCParams arg3,
                       ovl_11_DD48_arg0 *arg8) {
    s32 result;
    StructD548 *record;

    if ((u32)(*arg0 - arg1) >= 6U) {
        return -1;
    }
    record = arg3.record;
    result = 0;
    switch (*arg0 - arg1) {
    case 0:
        ovl_11_func_8011D098(3, 0, arg1);
        break;
    case 1:
        if (GetVal8005E5B4() == 2) {
            ovl_11_func_8011D0B4();
        }
        break;
    case 2:
        if (GetVal8005E5B4() == 5) {
            ovl_11_func_8011D0B4();
        }
        break;
    case 3:
        SetVal8005E334(0);
        SetVal8005E2BC(0);
        func_8002261C(2, 0x4D);
        ovl_11_func_8011DD48(arg8, 0, 0,
            (s16)ovl_11_func_800D6014((u16)ovl_11_func_8011D98C(*record)), 0);
        D_8012D52C = arg2;
        ovl_11_func_8011D098(2, 4, arg1);
        break;
    case 4:
        switch (ovl_11_func_8011CF40(0x63, (u16 *)&arg8->field_8)) {
        case 0:
            break;
        case 1:
            result = 1;
            func_80022738();
            break;
        case 2:
            func_80022738();
            func_8002261C(2, ovl_11_func_8011D9B4(*record));
            ovl_11_func_8011D098(1, 3, arg1);
            break;
        case 3:
            func_80022738();
            func_8002261C(arg3.field_C, arg3.field_E);
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
