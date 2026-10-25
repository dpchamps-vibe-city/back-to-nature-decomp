#include "common.h"
#include "screen_fade.h"
#include "master_volume.h"

void ovl_25_func_800BAC9C(void);
void ovl_25_func_800BAF20(void);
void ovl_25_func_800BAFCC(void);
void ovl_25_func_800BB2B8(void);
void ovl_25_func_800BB46C(void);
s32 func_80020818(void);
s32 func_8001FB30(s16 arg0, s16 arg1, s16 arg2, s16 arg3);
void ovl_25_func_800BB4B4(void);
void ovl_25_func_800BB510(s32 arg0);
void ovl_25_func_800B82B4(void);
void ovl_25_func_800BAA5C(void);
void func_800226F0(void);

void ovl_25_func_800B7F3C(void) {
    char *far_base;
    s32 *base;

    switch (*(u8 *)D_800BCD20) {
    case 0:
        ovl_25_func_800BAC9C();
        (*(u8 *)D_800BCD20)++;
        break;
    case 1:
        ovl_25_func_800BAF20();
        (*(u8 *)D_800BCD20)++;
        break;
    case 2:
        ovl_25_func_800BAFCC();
        (*(u8 *)D_800BCD20)++;
        break;
    case 3:
        ovl_25_func_800BB2B8();
        (*(u8 *)D_800BCD20)++;
        break;
    case 4:
        ovl_25_func_800BB46C();
        (*(u8 *)D_800BCD20)++;
        break;
    case 5:
        if (IsSoundTransferComplete() != 0) {
            *(u8 *)D_800BCD20 = 6;
            func_80020818();
            func_8001FB30(0, 0, 0x12, 0x7F);
        }
        break;
    }
    if (*(u8 *)D_800BCD20 == 6) {
        ovl_25_func_800BB4B4();
        far_base = (char *)&D_8007AFF0;
        *(Ovl25SetupStatePair *)(far_base + 0x25474) =
            *(Ovl25SetupStatePair *)(far_base + 0x2479C);
        ovl_25_func_800BB510((s32)D_8009F78C);
        ovl_25_func_800B82B4();
        ovl_25_func_800BAA5C();
        func_800226F0();
        BeginScreenFadeDecrease(0xA, 0, 2);
        BeginMasterVolumeFadeIn(0xA);
        *(u8 *)D_800BCD20 = 0;
        base = (s32 *)&D_8006C838;
        base[0x1123]++;
    }
}
