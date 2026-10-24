#include "common.h"
#include "game_types.h"

s32 ovl_11_func_800CB4E0(void);
void ovl_11_func_800C9D98(void);
s32 func_8001FABC(s16 arg0);
void ovl_11_func_800C70CC(Ovl11C7010Object *arg0, s32 arg1, s32 arg2, s32 arg3);

void ovl_11_func_800CB618(void) {
    s32 index;
    Ovl11C7010Object *obj = (Ovl11C7010Object *) &D_80071A00;
    Ovl11CB618RecordView *record;

    if (!(obj->unk6C & 0x10000000)) {
        ovl_11_func_800CB4E0();
        return;
    }
    ovl_11_func_800C9D98();
    index = *(s16 *) ((u8 *) obj + 0x20) - 1;
    record = (Ovl11CB618RecordView *) obj;
    if (record->records[index][0] == 0) {
        func_8001FABC(8);
        ovl_11_func_800C70CC(obj, 0xCU, obj->unk38 + 0x1D, -2);
        obj->unk6C &= ~0x8000;
    }
}
