#include "common.h"
#include "psyq/memory.h"

#define SAVE(type, source, offset) \
    (*(type *)(D_801376D0 + (offset)) = *(type *)(source))

void ovl_15_func_80134C48(void)
{
    s32 i;
    u8 *record;

    D_801376D0 = D_80137AB0;
    SAVE(Ovl15SaveBlock1F34, &D_800742EC, 0);
    SAVE(Ovl15SaveBlock18, D_8006C838, 0x225C);
    ((Ovl15SaveBufferWords *)D_801376D0)->field_2274 = *(s32 *)((u8 *)D_8006C838 + 0x20);
    ((Ovl15SaveBufferWords *)D_801376D0)->field_2278 = *(s32 *)((u8 *)D_8006C838 + 0x24);
    ((Ovl15SaveBufferWords *)D_801376D0)->field_227C = *(s32 *)((u8 *)D_8006C838 + 0x28);
    ((Ovl15SaveBufferWords *)D_801376D0)->field_2280 = *(s32 *)((u8 *)D_8006C838 + 0x2C);
    SAVE(Ovl15SaveBlock8, (u8 *)D_8006C838 + 0x30, 0x2284);
    SAVE(Ovl15SaveBlock94, (u8 *)D_8006C838 + 0x38, 0x228C);
    ((Ovl15SaveBufferWords *)D_801376D0)->field_2320 = ((s32 *)&D_80070CC0)[0];
    ((Ovl15SaveBufferWords *)D_801376D0)->field_2324 = ((s32 *)&D_80070CC0)[1];
    SAVE(Ovl15SaveBlock14, (u8 *)&D_80070CC0 + 8, 0x2328);
    SAVE(Ovl15SaveBlock14, (u8 *)&D_80070CC0 + 0x1C, 0x233C);
    ((Ovl15SaveBufferWords *)D_801376D0)->field_2350 = ((s32 *)&D_80070CC0)[12];
    ((Ovl15SaveBufferWords *)D_801376D0)->field_2354 = ((s32 *)&D_80070CC0)[13];
    ((Ovl15SaveBufferWords *)D_801376D0)->field_2358 = ((s32 *)&D_80070CC0)[14];
    ((Ovl15SaveBufferWords *)D_801376D0)->field_235C = ((s32 *)&D_80070CC0)[15];
    ((Ovl15SaveBufferWords *)D_801376D0)->field_2360 = ((s32 *)&D_80070CC0)[16];
    SAVE(Ovl15SaveBlockCFC, (u8 *)&D_80070CC0 + 0x44, 0x2364);
    SAVE(Ovl15SaveBlock3FC, &D_80071A00, 0x3060);
    SAVE(Ovl15SaveBlock2328, D_80071DFC, 0x345C);
    SAVE(Ovl15SaveBlock188, D_80074124, 0x5784);
    SAVE(Ovl15SaveBlock40, &D_800742AC, 0x590C);
    SAVE(Ovl15SaveBlock74, D_8007A5C4, 0x594C);
    SAVE(Ovl15SaveBlock6C8, D_8007A638, 0x59C0);
    ((Ovl15SaveBufferWords *)D_801376D0)->field_6088 = D_8007AD00[0];
    SAVE(Ovl15SaveBlockC, (u8 *)D_8007AD00 + 4, 0x608C);
    SAVE(Ovl15SaveBlock3C, (u8 *)D_8007AD00 + 0x10, 0x6098);
    SAVE(Ovl15SaveBlock108, D_8007AE78, 0x6200);
    SAVE(Ovl15SaveBlock24, &gScreenColorRamp, 0x6308);
    SAVE(Ovl15SaveBlock34, D_8007AFA4, 0x632C);
    ((Ovl15SaveBufferWords *)D_801376D0)->field_6360 = D_8007AFD8[0];
    ((Ovl15SaveBufferWords *)D_801376D0)->field_6364 = D_8007AFD8[1];
    ((Ovl15SaveBufferWords *)D_801376D0)->field_6368 = D_8007AFD8[2];
    SAVE(Ovl15SaveBlockC, (u8 *)D_8007AFD8 + 0xC, 0x636C);
    for (i = 0; i < 37; i++) {
        record = (u8 *)D_8006C838 + i * 0x1D4;
        ((Ovl15SavedRecordList *)D_801376D0)->records[i].bytes = *(Ovl15SaveBytes4 *)(record + 0x9A0C);
        ((Ovl15SavedRecordList *)D_801376D0)->records[i].field_4 = *(u16 *)(record + 0x99EA);
        ((Ovl15SavedRecordList *)D_801376D0)->records[i].field_6 = *(u16 *)(record + 0x99EC);
        ((Ovl15SavedRecordList *)D_801376D0)->records[i].field_8 = *(u16 *)(record + 0x9A06);
        ((Ovl15SavedRecordList *)D_801376D0)->records[i].field_A = *(u16 *)(record + 0x9A08);
        ((Ovl15SavedRecordList *)D_801376D0)->records[i].field_C = *(u16 *)(record + 0x9A0A);
    }
    SAVE(Ovl15SaveBlockF0, ((Ovl15SaveRestoreRuntime *)D_8006C838)->block_DDCC, 0x213C);
    SAVE(Ovl15SaveBlock30, ((Ovl15SaveRestoreRuntime *)D_8006C838)->block_DDD4, 0x222C);
    for (i = 0; i < 10; i++) {
        ((Ovl15SaveRecordStates *)D_801376D0)->records_B4[i].state = 0;
    }
    for (i = 0; i < 20; i++) {
        ((Ovl15SaveRecordStates *)D_801376D0)->records_B8[i].state = 0;
    }
    memset(D_801376D0 + 0x1CF8, 0, 0x21C);
}

#undef SAVE
