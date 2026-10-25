#include "common.h"
#include "screen_fade.h"

void ovl_11_func_80102A10(void);
void ovl_11_func_80102B6C(u16 arg0, s16 arg1);
void func_80021B90(s32 arg0);
s16 ovl_11_func_80102BCC(u16 *outId, u16 *outValue);
void ovl_11_func_801033D4(void);
void ovl_11_func_80103424(void);
void ovl_11_func_80103480(void);
void ovl_11_func_801034D0(void);
void ovl_11_func_80103548(void);

void ovl_11_func_8010289C(void) {
    u16 id;
    s16 value;
    s16 index;
    s16 selection;

    if (IsScreenFadeComplete() != 0) {
        index = D_801273E6;
        Ovl11_D_801273E7 = 0;
        ovl_11_func_80102A10();
        selection = index;
        if (index != 0xFF) {
            id = ((Ovl11SelectionRecordArray *)D_8006C838)->records[index].id;
            value = (s16)((Ovl11SelectionRecordArray *)D_8006C838)->records[index].value;
            ovl_11_func_80102B6C(id, value);
            Ovl11_D_801273DC = ovl_11_func_801033D4;
            func_80021B90(0x16);
            D_801273E4_state = 8;
            return;
        }
        index = ovl_11_func_80102BCC(&id, (u16 *)&value);
        ovl_11_func_80102B6C(id, value);
        func_80021B90(0x16);
        D_801273E4_state = 8;
        if (index == 0xFE) {
            Ovl11_D_801273DC = ovl_11_func_80103480;
        } else if (index == selection) {
            Ovl11_D_801273DC = ovl_11_func_80103424;
        } else {
            if (value == (s16)((Ovl11SelectionRecordArray *)D_8006C838)->records[index].value) {
                Ovl11_D_801273DC = ovl_11_func_801033D4;
            } else if (value < (s16)((Ovl11SelectionRecordArray *)D_8006C838)->records[index].value) {
                Ovl11_D_801273DC = ovl_11_func_80103548;
            } else {
                Ovl11_D_801273DC = ovl_11_func_801034D0;
            }
        }
    }
}
