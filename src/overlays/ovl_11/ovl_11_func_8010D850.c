#include "common.h"
#include "game_types.h"
#include "random.h"

s32 ovl_11_func_80110944(s16 area, u16 mode);
s32 ovl_11_func_800DF010(Ovl11ActionDispatchView *arg0, s32 arg1);
s32 ovl_11_func_800E0C24(Ovl11ActionDispatchView *arg0, s32 arg1);
s32 ovl_11_func_800F413C(Ovl11F413CArg0View *arg0, Ovl11F413CArg1View *arg1);
s32 ovl_11_func_800F3BCC(u16 arg0);
void ovl_11_func_80110E28(u8 *arg0);

s32 ovl_11_func_8010D850(u8 *arg0, s16 area) {
    s32 random;
    s32 failed;
    s32 fallbackFailed;
    s32 i;
    s32 count;
    s32 status;
    s32 result;
    s16 index;
    u8 *record;
    u8 *base;
    Ovl11F413CArg1View out;

    random = Rand(3);
    count = (u16)(*(u16 *)(arg0 + 0xB2) / 100U) + random;
    failed = 0;
    for (i = 0; i < count; i++) {
        index = ovl_11_func_80110944(area, 4);
        if (index == -1) {
            failed = 1;
            break;
        }
        record = (u8 *)&gTaggedRecordBase + index * 0xB8;
        *(u16 *)(record + 0xB6) = *(u16 *)(arg0 + 0xAC);
        if (!(*(s32 *)(record + 0x34) & 0x10000)) {
            ovl_11_func_800DF010((Ovl11ActionDispatchView *)record, 7);
        } else {
            ovl_11_func_800E0C24((Ovl11ActionDispatchView *)record, 8);
        }
        if (*(u16 *)(arg0 + 0xB6) == 0) {
            failed = 1;
        }
        break;
    }
    i = 0;
    if (failed) {
        fallbackFailed = 0;
        if (count > 0) {
            base = D_80074838;
            do {
                result = (s16)ovl_11_func_80110944(area, 8);
                status = -1;
                if (result != status) {
                    status = ovl_11_func_800F413C((Ovl11F413CArg0View *)(*(u8 **)(base + 0x5DCC) + result * 0x18), &out);
                    result = 1;
                    if (status == result) {
                        *(u16 *)(arg0 + 0xB6) = out.unk0;
                        continue;
                    }
                }
                fallbackFailed = 1;
                goto fallback_done;
            } while (++i < count);
        }
fallback_done:
        failed = fallbackFailed;
    }
    if (*(u16 *)(arg0 + 0xB6) != 0) {
        ovl_11_func_800F3BCC(*(u16 *)(arg0 + 0xB6));
        ovl_11_func_80110E28(arg0);
    }
    return failed;
}
