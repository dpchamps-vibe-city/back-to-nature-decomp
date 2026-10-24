#include "common.h"
#include "game_types.h"

s32 GetVal8005E5B4(void);
s32 func_8002261C(s32 arg0, s32 arg1);
void func_80022738(void);
s32 ovl_11_func_800F2354(s32 arg0, s32 arg1);
s32 ovl_11_func_8011B840(s32 *arg0, s32 arg1, Ovl11D8012D7B8 arg2);
s32 ovl_11_func_8011BAFC(s32 *arg0, s32 arg1, s32 arg2, Ovl11BAFCParams arg3, ovl_11_DD48_arg0 *arg8);
s32 ovl_11_func_8011D06C(s32 arg0);
void ovl_11_func_8011D084(s32 arg0, s32 arg1);
void ovl_11_func_8011D0B4(void);
s16 ovl_11_func_8011D98C(StructD548 x);
s16 ovl_11_func_8011D9DC(s16 id, s32 mode);
s32 ovl_11_func_8011DAD4(s16 id, s16 amount, s32 mode);

s32 ovl_11_func_8011C6CC(s16 *arg0, s16 *arg1, s16 *arg2, Ovl11D8012D7B8 arg3) {
    s32 result;
    s16 startKind;
    s16 startId;
    s16 nextKind;
    s16 nextId;
    s16 cancelKind;
    s16 cancelId;
    s32 message;
    Ovl11D8012D538 *recordState;
    Ovl11D8012D538 *secondState;

    result = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_522C = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5230 = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 |= 0x800000;
    startKind = arg0[0];
    startId = arg0[1];
    nextKind = arg1[0];
    nextId = arg1[1];
    cancelKind = arg2[0];
    cancelId = arg2[1];
    switch (D_8012D524) {
    case 0:
        ovl_11_func_8011D084(4, 0);
        break;
    case 1:
        if (GetVal8005E5B4() == 2) {
            ovl_11_func_8011D0B4();
        }
        break;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
        switch (ovl_11_func_8011B840(&D_8012D524, 4, arg3)) {
        case 0:
            break;
        case 1:
            if (ovl_11_func_8011D9DC(ovl_11_func_8011D98C(*(StructD548 *)D_8012D548), 0) == 0) {
                func_80022738();
                func_8002261C(2, 0x54);
                ovl_11_func_8011D084(1, 2);
            } else {
                D_8012D540 = 1;
                func_8002261C(startKind, startId);
                ovl_11_func_8011D084(1, 11);
            }
            break;
        case 3:
            ovl_11_func_8011D084(2, 0);
            break;
        }
        break;
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
        recordState = &D_8012D538;
        switch (ovl_11_func_8011BAFC(&D_8012D524, 11, 5, *(Ovl11BAFCParams *)&arg3, (ovl_11_DD48_arg0 *)recordState)) {
        case 0:
            break;
        case 1:
            if (ovl_11_func_8011D06C(recordState->unk8 * recordState->unkA) == 1) {
                if (ovl_11_func_8011D9DC(ovl_11_func_8011D98C(*(StructD548 *)D_8012D548), 0) < recordState->unk8) {
                    func_8002261C(2, 0xC1);
                    ovl_11_func_8011D084(1, 11);
                } else {
                    secondState = &D_8012D538;
                    if (ovl_11_func_8011D9DC(ovl_11_func_8011D98C(*(StructD548 *)D_8012D548), 1) < secondState->unk8) {
                        message = 0x207;
                        if (D_8012D520 == 14) {
                            message = 0x261;
                        }
                        func_8002261C(3, message);
                        ovl_11_func_8011D084(1, 3);
                    } else {
                        D_8012D52C = 4;
                        func_8002261C(nextKind, nextId);
                        ovl_11_func_8011D084(1, 7);
                    }
                    ovl_11_func_800F2354(-(D_8012D538.unk8 * D_8012D538.unkA), 1);
                    ovl_11_func_8011DAD4(ovl_11_func_8011D98C(*(StructD548 *)D_8012D548), D_8012D538.unk8, 0);
                }
            } else {
                func_8002261C(2, 0x4F);
                ovl_11_func_8011D084(1, 11);
            }
            break;
        case 3:
            D_8012D52C = 4;
            func_8002261C(cancelKind, cancelId);
            ovl_11_func_8011D084(1, 7);
            break;
        }
        break;
    case 3:
        func_8002261C(nextKind, nextId);
        ovl_11_func_8011D084(1, 7);
        break;
    case 2:
        ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 &= ~0x800000;
        result = 1;
        break;
    }
    return result;
}
