#include "common.h"
#include "ovl11_halfword_triplets.h"

void ovl_11_func_800D666C(s16 arg0);
s32 ovl_11_func_800CB4E0(void);
void ovl_11_func_8010941C(s32 arg0);
void ovl_11_func_800F4CAC(void);
void ovl_11_func_800F4CEC(void);
s32 ovl_11_func_800CB540(u16 *arg0);
s32 ovl_11_func_800D6A94(s16 arg0);

s32 ovl_11_func_800E8250(s16 arg0, s16 arg1) {
    s16 *slot;
    s32 i;
    u16 saved;

    if (arg0 == 0) {
        Ovl11CopyHalfwordTriplet((u16 *)&D_80129500, (u16 *)&Ovl11Item_80071A8A);
        *(Ovl11ItemSnapshot *)D_80129510 = *(Ovl11ItemSnapshot *)((u8 *)&Ovl11Item_80071A8A + 0x36);
        if (((struct Ovl11E8250State *)D_8006C838)->item.id != 0) {
            ((struct Ovl11E8250State *)D_8006C838)->flags &= 0xE7FF7FFF;
            ovl_11_func_800D666C(((struct Ovl11E8250State *)D_8006C838)->item.id);
            Ovl11ClearHalfwordTriplet((s16 *)&((struct Ovl11E8250State *)D_8006C838)->item);
            ovl_11_func_800CB4E0();
        }
        slot = (s16 *)&((struct Ovl11E8250State *)D_8006C838)->slots;
        for (i = 0; i < 8; i++) {
            ovl_11_func_800D666C(*slot);
            Ovl11ClearHalfwordTriplet(slot);
            slot += 3;
        }
        ovl_11_func_800CB4E0();
    } else {
        saved = ((struct Ovl11E8250State *)D_8006C838)->item.id;
        if (D_80129500.id != 0) {
            if (saved != 0) {
                ((struct Ovl11E8250State *)D_8006C838)->flags &= 0xE7FF7FFF;
                ovl_11_func_800D666C(((struct Ovl11E8250State *)D_8006C838)->item.id);
                Ovl11ClearHalfwordTriplet((s16 *)&((struct Ovl11E8250State *)D_8006C838)->item);
                ovl_11_func_800CB4E0();
                switch (saved) {
                case 0x106:
                case 0x107:
                    ((struct Ovl11E8250State *)D_8006C838)->effectFlags &= 0xFFFDFFFF;
                    ovl_11_func_8010941C(0);
                    break;
                case 0x65:
                    ((struct Ovl11E8250State *)D_8006C838)->effect[0xE] |= 1;
                    ovl_11_func_800F4CAC();
                    break;
                case 0x64:
                    ((struct Ovl11E8250State *)D_8006C838)->effect[2] |= 1;
                    ovl_11_func_800F4CEC();
                    break;
                }
            }
            if ((arg1 == 0) || (((ItemData *)((struct Ovl11E8250State *)D_8006C838)->items)[D_80129500.id].u0.flags & 0x100)) {
                ovl_11_func_800CB540((u16 *)&D_80129500);
            } else {
                ((struct Ovl11E8250State *)D_8006C838)->item = D_80129500;
                ovl_11_func_800D6A94(((struct Ovl11E8250State *)D_8006C838)->item.id);
                ((struct Ovl11E8250State *)D_8006C838)->flags = (((struct Ovl11E8250State *)D_8006C838)->flags | 0x10008000) & ~0x4000;
                ovl_11_func_800CB4E0();
            }
        }
        Ovl11Items_80071AC0 = *(Ovl11ItemSnapshot *)D_80129510;
        slot = (s16 *)&Ovl11Items_80071AC0;
        for (i = 0; i < 8;) {
            ovl_11_func_800D6A94(*slot);
            i++;
            slot += 3;
        }
    }
    return 1;
}
