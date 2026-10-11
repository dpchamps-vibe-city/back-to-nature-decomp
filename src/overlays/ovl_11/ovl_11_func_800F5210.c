#include "common.h"
#include "game_types.h"

typedef struct {
    /* 0x00 */ s16 field_00;
    /* 0x02 */ u16 field_02;
    /* 0x04 */ char pad_04[0x18 - 0x04];
} Entry_F5210; /* 0x18 */

typedef struct {
    /* 0x0000 */ char pad_0000[0x44BA];
    /* 0x44BA */ s16 field_44BA;
    /* 0x44BC */ char pad_44BC[0x44D4 - 0x44BC];
    /* 0x44D4 */ u16 field_44D4;
    /* 0x44D6 */ char pad_44D6[0x51C0 - 0x44D6];
    /* 0x51C0 */ s32 field_51C0;
    /* 0x51C4 */ char pad_51C4[0xDDB4 - 0x51C4];
    /* 0xDDB4 */ Entry_F5210 *field_DDB4;
} View_F5210;

void ovl_11_func_800F5210(void) {
    Entry_F5210 *p = (Entry_F5210 *)((u8 *)((View_F5210 *)D_8006C838)->field_DDB4 + 2);
    s16 type;
    u16 flags;
    s32 i;

    for (i = 0; i < 13; i++) {
        type = p->field_00;
        switch (type) {
        case 0x64:
            if (!(((View_F5210 *)D_8006C838)->field_51C0 & 0x20)) {
                p->field_02 &= 0xFFFE;
            } else {
                p->field_02 |= 1;
            }
            break;
        case 0x21:
            if (!(((View_F5210 *)D_8006C838)->field_51C0 & 0x100)) {
                p->field_02 &= 0xFFFE;
            } else {
                p->field_02 |= 1;
            }
            break;
        case 0x128:
            if ((((View_F5210 *)D_8006C838)->field_44BA != 0) && (((View_F5210 *)D_8006C838)->field_44D4 == 0)) {
                p->field_02 &= 0xFFFE;
            } else {
                p->field_02 |= 1;
            }
            break;
        case 0x129:
            if ((((View_F5210 *)D_8006C838)->field_44BA != 1) && (((View_F5210 *)D_8006C838)->field_44D4 == 0)) {
                p->field_02 &= 0xFFFE;
            } else {
                p->field_02 |= 1;
            }
            break;
        case 0x12A:
            if ((((View_F5210 *)D_8006C838)->field_44BA != 2) && (((View_F5210 *)D_8006C838)->field_44D4 == 0)) {
                p->field_02 &= 0xFFFE;
            } else {
                p->field_02 |= 1;
            }
            break;
        }
        flags = p->field_02;
        if (flags & 0x8000) {
            if (((View_F5210 *)D_8006C838)->field_51C0 & 0x40) {
                p->field_00 = 0x6D;
                p->field_02 = flags | 1;
            } else if (((View_F5210 *)D_8006C838)->field_51C0 & 0x80) {
                p->field_00 = 0x6E;
                p->field_02 = flags | 1;
            } else {
                p->field_02 = flags & 0xFFFE;
            }
        }
        p++;
    }
}
