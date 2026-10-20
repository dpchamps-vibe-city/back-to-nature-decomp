#include "common.h"

void ovl_11_func_800E44D4(void);
void ovl_11_func_800E4568(void);
void ovl_11_func_800E4428(void);

s32 ovl_11_func_800E40CC(void) {
    Ovl11E910Entry *record;

    if (((Ovl11FarInitView *)&D_8007AFF0)->field_254A0 == 0x18) {
        return 0;
    }
    if (!(((struct Ovl11E4280State *)D_8006C838)->flags_C & 0x10000000)) {
        for (;;) {
            if (Ovl11RecordCursor->unk0 == 0xFF) {
                ((struct Ovl11E4280State *)D_8006C838)->selector = 2;
                break;
            }
            record = Ovl11RecordCursor;
            if (D_800B975C[record->unk0](
                    *(s16 *)((u8 *)record + 4),
                    *(s16 *)((u8 *)record + 6),
                    *(s32 *)((u8 *)record + 8),
                    *(s32 *)((u8 *)record + 0xC)) == 1) {
                if (Ovl11RecordCursor->unk1 == 0xFF) {
                    Ovl11RecordCursor++;
                    D_80129224++;
                } else {
                    Ovl11RecordCursor = &D_801291B0[Ovl11RecordCursor->unk1];
                    D_80129224 = Ovl11RecordCursor->unk1;
                }
            } else {
                if (Ovl11RecordCursor->unk2 == 0xFF) {
                    Ovl11RecordCursor++;
                    D_80129224++;
                } else {
                    Ovl11RecordCursor = &D_801291B0[Ovl11RecordCursor->unk2];
                    D_80129224 = Ovl11RecordCursor->unk2;
                }
            }
            if (record->unk3[0] & 1) {
                break;
            }
        }
    }
    ovl_11_func_800E44D4();
    ovl_11_func_800E4568();
    ovl_11_func_800E4428();
    return 0;
}
