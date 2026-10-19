#include "common.h"
#include "game_types.h"

s32 ovl_11_func_80107528(s32 arg0, u16 arg1);

s32 ovl_11_func_80107480(Ovl11GuardRecord *arg0) {
    u8 first;
    u8 second;
    u16 firstValue;
    u16 secondValue;
    s32 firstResult;
    s32 secondResult;
    s32 result;

    first = arg0->unk0;
    second = arg0->unk1;
    firstValue = arg0->unk2;
    secondValue = arg0->unk4;
    if (D_8012D038 == -1U || D_8012D038 == firstValue) {
        firstResult = ovl_11_func_80107528(first, firstValue);
        secondResult = ovl_11_func_80107528(second, secondValue);
        if (firstResult == 1) {
            if (second == 0xFF) {
                return arg0->unk6;
            }
            result = -1;
            if (secondResult == firstResult) {
                result = arg0->unk6;
            }
            return result;
        }
    }
    return -1;
}
