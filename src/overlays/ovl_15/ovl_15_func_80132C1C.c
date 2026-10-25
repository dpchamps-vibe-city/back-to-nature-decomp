#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/memory.h"
#include "ovl15_row_placement.h"

/* Callee scans the selected pool and writes the free record index. */
void *ovl_15_func_8013345C(void *arg0, s16 arg1, s16 *arg2);

void ovl_15_func_80132C1C(void) {
    s16 index;
    s16 sourceIndex;
    s16 kind;
    s32 i;
    u8 *buffer;

    buffer = D_80142550;
    index = 0;
    for (i = 0; i < 5; i++) {
        kind = Ovl15Slots[1][i].unk0;
        sourceIndex = Ovl15Slots[1][i].unk2;
        if (kind != -1) {
            switch (kind) {
            case 0:
                ((Ovl15SaveRecordStates *)D_801376D0)->records_B4[sourceIndex].state = 0;
                Ovl15PlaceRowXSlot((struct ovl_15_80132A8C_data *)(D_801376D0 + sourceIndex * 0xB4), sourceIndex);
                memcpy(buffer, D_801376D0 + sourceIndex * 0xB4, 0xB4);
                Ovl15Slots[1][i].unk4 = (s32)buffer;
                buffer += 0xB4;
                memset(D_801376D0 + sourceIndex * 0xB4, 0, 0xB4);
                break;
            case 1:
            case 2:
                ((Ovl15SaveRecordStates *)D_801376D0)->records_B8[sourceIndex].state = 0;
                Ovl15PlaceRowZSlot((Unk80132B44 *)(D_801376D0 + 0x708 + sourceIndex * 0xB8), sourceIndex);
                memcpy(buffer, D_801376D0 + 0x708 + sourceIndex * 0xB8, 0xB8);
                Ovl15Slots[1][i].unk4 = (s32)buffer;
                buffer += 0xB8;
                memset(D_801376D0 + 0x708 + sourceIndex * 0xB8, 0, 0xB8);
                break;
            }
        }
    }
    for (i = 0; i < 5; i++) {
        kind = Ovl15Slots[0][i].unk0;
        sourceIndex = Ovl15Slots[0][i].unk2;
        if (kind != -1) {
            switch (kind) {
            case 0:
                buffer = ovl_15_func_8013345C(D_801376D0, 0, &index);
                ((Ovl15TransferRuntimeStates *)D_8006C838)->pool.records_B4[sourceIndex].state = 0;
                Ovl15PlaceRowXSlot((struct ovl_15_80132A8C_data *)((u8 *)D_8006C838 + 0x7AB4 + sourceIndex * 0xB4), index);
                memcpy(buffer, (u8 *)D_8006C838 + 0x7AB4 + sourceIndex * 0xB4, 0xB4);
                memset((u8 *)D_8006C838 + 0x7AB4 + sourceIndex * 0xB4, 0, 0xB4);
                break;
            case 1:
            case 2:
                buffer = ovl_15_func_8013345C(D_801376D0, 1, &index);
                ((Ovl15TransferRuntimeStates *)D_8006C838)->pool.records_B8[sourceIndex].state = 0;
                Ovl15PlaceRowZSlot((Unk80132B44 *)((u8 *)D_8006C838 + 0x81BC + sourceIndex * 0xB8), index);
                memcpy(buffer, (u8 *)D_8006C838 + 0x81BC + sourceIndex * 0xB8, 0xB8);
                memset(&((Ovl15TransferRuntimeRecords *)D_8006C838)->records_B8[sourceIndex], 0, 0xB8);
                break;
            }
        }
    }
}
