#include "common.h"

#define RESTORE(type, destination, offset) \
    (*(type *)(destination) = *(type *)(D_801376D0 + (offset)))

void ovl_15_func_80135380(void)
{
    s32 i;
    u8 *record;

    D_801376D0 = D_80137AB0;
    RESTORE(Ovl15SaveBlockF0, ((Ovl15SaveRestoreRuntime *)D_8006C838)->block_DDCC, 0x213C);
    RESTORE(Ovl15SaveBlock30, ((Ovl15SaveRestoreRuntime *)D_8006C838)->block_DDD4, 0x222C);
    for (i = 0; i < 37; i++) {
        record = (u8 *)D_8006C838 + i * 0x1D4;
        *(Ovl15SaveBytes4 *)(record + 0x9A0C) = ((Ovl15SavedRecordList *)D_801376D0)->records[i].bytes;
        *(u16 *)(record + 0x99EA) = ((Ovl15SavedRecordList *)D_801376D0)->records[i].field_4;
        *(u16 *)(record + 0x99EC) = ((Ovl15SavedRecordList *)D_801376D0)->records[i].field_6;
        *(u16 *)(record + 0x9A06) = ((Ovl15SavedRecordList *)D_801376D0)->records[i].field_8;
        *(u16 *)(record + 0x9A08) = ((Ovl15SavedRecordList *)D_801376D0)->records[i].field_A;
        *(u16 *)(record + 0x9A0A) = ((Ovl15SavedRecordList *)D_801376D0)->records[i].field_C;
    }
    RESTORE(Ovl15SaveBlock1F34, &D_800742EC, 0);
    RESTORE(Ovl15SaveBlock18, D_8006C838, 0x225C);
    RESTORE(Ovl15SaveBlock8, (u8 *)D_8006C838 + 0x30, 0x2284);
    RESTORE(Ovl15SaveBlock94, (u8 *)D_8006C838 + 0x38, 0x228C);
    ((s32 *)&gScenePhase)[0] = *(s32 *)(D_801376D0 + 0x2320);
    ((s32 *)&gScenePhase)[1] = *(s32 *)(D_801376D0 + 0x2324);
    RESTORE(Ovl15SaveBlock14, (u8 *)&gScenePhase + 8, 0x2328);
    RESTORE(Ovl15SaveBlock14, (u8 *)&gScenePhase + 0x1C, 0x233C);
    ((s32 *)&gScenePhase)[12] = *(s32 *)(D_801376D0 + 0x2350);
    ((s32 *)&gScenePhase)[13] = *(s32 *)(D_801376D0 + 0x2354);
    ((s32 *)&gScenePhase)[14] = *(s32 *)(D_801376D0 + 0x2358);
    ((s32 *)&gScenePhase)[15] = *(s32 *)(D_801376D0 + 0x235C);
    ((s32 *)&gScenePhase)[16] = *(s32 *)(D_801376D0 + 0x2360);
    RESTORE(Ovl15SaveBlockCFC, (u8 *)&gScenePhase + 0x44, 0x2364);
    RESTORE(Ovl15SaveBlock3FC, &D_80071A00, 0x3060);
    RESTORE(Ovl15SaveBlock2328, D_80071DFC, 0x345C);
    RESTORE(Ovl15SaveBlock188, D_80074124, 0x5784);
    RESTORE(Ovl15SaveBlock40, &D_800742AC, 0x590C);
    RESTORE(Ovl15SaveBlock6C8, D_8007A638, 0x59C0);
    D_8007AD00[0] = *(s32 *)(D_801376D0 + 0x6088);
    RESTORE(Ovl15SaveBlockC, (u8 *)D_8007AD00 + 4, 0x608C);
    RESTORE(Ovl15SaveBlock3C, (u8 *)D_8007AD00 + 0x10, 0x6098);
    RESTORE(Ovl15SaveBlock108, D_8007AE78, 0x6200);
    RESTORE(Ovl15SaveBlock24, &gScreenColorRamp, 0x6308);
    RESTORE(Ovl15SaveBlock34, D_8007AFA4, 0x632C);
    D_8007AFD8[0] = *(s32 *)(D_801376D0 + 0x6360);
    D_8007AFD8[1] = *(s32 *)(D_801376D0 + 0x6364);
    D_8007AFD8[2] = *(s32 *)(D_801376D0 + 0x6368);
    RESTORE(Ovl15SaveBlockC, (u8 *)D_8007AFD8 + 0xC, 0x636C);
}

#undef RESTORE
