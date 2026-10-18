#include "common.h"
#include "game_types.h"

s32 ovl_11_func_80102DC4(void *arg0);
s32 ovl_11_func_80102E1C(Ovl11IdAdjustmentObject *arg0);
s32 ovl_11_func_80102F14(Ovl11IdAdjustmentObject *arg0, u16 *arg1);

s16 ovl_11_func_80102BCC(u16 *outId, u16 *outValue) {
    u16 savedIds[8];
    u16 value;
    Ovl11IdAdjustmentObject *object;
    s32 index;
    s32 i;
    u16 savedFlags;
    u16 id;

    for (i = 0; i < 8; i++) {
        savedIds[i] = D_8012CF00_ids[i];
    }
    value = 0;
    savedFlags = D_801273EE_flags;
    id = 0;
    for (index = 0; ((Ovl11IdAdjustmentObject *) D_80050B5C_objects)[index].id != 0xE2; index++) {
        for (i = 0; i < 8; i++) {
            D_8012CF00_ids[i] = savedIds[i];
        }
        object = (Ovl11IdAdjustmentObject *) D_80050B5C_objects + index;
        D_801273EE_flags = savedFlags;
        value = object->value;
        if (ovl_11_func_80102DC4(object) &&
            ovl_11_func_80102E1C(object) &&
            ovl_11_func_80102F14(object, &value)) {
            id = object->id;
            break;
        }
    }
    for (i = 0; i < 8; i++) {
        D_8012CF00_ids[i] = savedIds[i];
    }
    D_801273EE_flags = savedFlags;
    if (id == 0) {
        *outId = 0xE2;
        *outValue = 1;
        return 0xFE;
    }
    *outId = id;
    *outValue = value;
    for (index = 0; index < 0x50; index++) {
        if (((Ovl11SelectionRecords *) D_8006C838)->records[index][0] == id) {
            break;
        }
    }
    if (index == 0x50) {
        return 0xFF;
    }
    return (u8) index;
}
