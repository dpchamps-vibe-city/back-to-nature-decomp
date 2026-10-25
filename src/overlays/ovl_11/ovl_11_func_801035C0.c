#include "common.h"
#include "game_flags.h"

s16 ovl_11_func_80102BCC(u16 *outId, u16 *outValue);

void ovl_11_func_801035C0(void) {
    u16 id;
    u16 value;
    s32 index;
    s32 i;
    u8 *base;
    s32 offset;
    u16 *dest;

    index = ovl_11_func_80102BCC(&id, &value);
    if (index == 0xFF) {
        for (i = 0; i < 0x50; i++) {
            if (((Ovl11SelectionRecords *)D_8006C838)->records[i][0] == 0) {
                index = (s16)i;
                break;
            }
        }
    }
    base = (u8 *)D_8006C838;
    offset = index * 0x16;
    dest = (u16 *)((u8 *)((Ovl11SelectionRecordArray *)D_8006C838)->records[0].ids + offset);
    i = 0;
    ((Ovl11SelectionRecordArray *)base)->records[index].id = id;
    ((Ovl11SelectionRecordArray *)base)->records[index].value = value;
    ((Ovl11SelectionRecordArray *)base)->records[index].flags = D_801273EE_flags;
    do {
        *dest++ = D_8012CF00_ids[i];
        i++;
    } while (i < 8);
    switch ((s16)id) {
    case 0xD6:
        SetGameFlag(0x5B, 1);
        break;
    case 0xD7:
        SetGameFlag(0x5C, 1);
        break;
    case 0xD1:
        SetGameFlag(0x5D, 1);
        break;
    }
}
