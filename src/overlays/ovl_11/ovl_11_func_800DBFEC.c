#include "common.h"
#include "game_types.h"
#include "projected_primitives.h"
#include "ovl11_background_color_cycle.h"

void ovl_11_func_800BCF54(s32 arg0, s32 arg1);
void ovl_11_func_800DB00C(Ovl11SceneCoordinates *arg0, Ovl11SceneCoordinates *arg1);
void func_8001E340(Ovl11SceneGrid **arg0);
void ovl_11_func_800DC114(s32 arg0, s32 arg1, s32 arg2);
void ovl_11_func_80111D48(void);
void ovl_11_func_80111A60(void);

void ovl_11_func_800DBFEC(u16 arg0, u16 arg1, s32 arg2) {
    u8 *base = (u8 *)&D_8007AFF0;
    struct Ovl11SceneHeader *header;

    *(Ovl25SetupStatePair *)(base + 0x25378) =
        *(Ovl25SetupStatePair *)(base + 0x2479C);
    *(s32 *)(base + 0x2537C) = *(s32 *)(base + 0x247A0);
    *(u16 *)(base + 0x25380) = *(u16 *)(base + 0x247A4);
    *(u16 *)(base + 0x25382) = *(u16 *)(base + 0x247A6);
    *(u16 *)(base + 0x25384) = *(u16 *)(base + 0x247A8);
    *(u16 *)(base + 0x25386) = *(u16 *)(base + 0x247AA);
    ovl_11_func_800BCF54(arg0, arg2 & 0xFFFF);
    InitProjectedPrimitiveSource(gOvl11SceneGeometry, (u8 *)D_8007AFF0 + 0x2000);
    ovl_11_func_800DB00C(&D_800A00C4, &D_800A00CC);
    func_8001E340((Ovl11SceneGrid **)gOvl11SceneGeometry);
    ovl_11_func_800DC114(0, 0, 0);
    header = *(struct Ovl11SceneHeader **)(base + 0x25388);
    switch (header->kind) {
    case 15:
        if (header->id == 0xE5) {
            ovl_11_func_80111D48();
            return;
        }
        ovl_11_func_80111A60();
        return;
    case 44:
        Ovl11ResetBackgroundColorCycle();
        return;
    }
}
