#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/memory.h"
#include "ovl15_row_placement.h"

/* Callee scans the selected record pool and writes its index through arg2. */
void *ovl_15_func_8013345C(s32 *arg0, s16 arg1, s16 *arg2);

void ovl_15_func_801331C4(void) {
    s16 index;
    s32 i;
    void *dest;

    index = 0;
    for (i = 0; i < 5; i++) {
        if (Ovl15Slots[1][i].unk0 != -1) {
            switch (Ovl15Slots[1][i].unk0) {
            case 0:
                dest = ovl_15_func_8013345C(&D_800742EC, 0, &index);
                memcpy(dest, (void *)Ovl15Slots[1][i].unk4, 0xB4);
                Ovl15PlaceRowXSlot((struct ovl_15_80132A8C_data *)((u8 *)&D_800742EC + index * 0xB4), index);
                break;
            case 1:
            case 2:
                dest = ovl_15_func_8013345C(&D_800742EC, 1, &index);
                memcpy(dest, (void *)Ovl15Slots[1][i].unk4, 0xB8);
                Ovl15PlaceRowZSlot((Unk80132B44 *)((u8 *)&gTaggedRecordBase + index * 0xB8), index);
                break;
            }
        }
    }
}
