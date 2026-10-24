#include "common.h"

s32 ovl_11_func_800C3548(s32 arg0);
/* Engine import of matched u32 Rand(s32), at 0x80012A34. */
u32 Rand(s32 arg0);
void func_8001AF70(u16 arg0, u16 arg1);

void ovl_11_func_800C1D68(void) {
    u32 i;
    struct_80076220 *p;
    s32 value;
    s32 count;
    s32 kind;
    s32 one;
    u8 *work;

    p = &D_80076220;
    i = 0;
    one = 1;
    work = (u8 *)p - 0x19E8;
    for (; i < 0x25; i++, p++) {
        kind = ovl_11_func_800C3548((s16)i);
        value = p->unk4;
        count = (s16)p->unk2;
        if (p->unk1E & 0x400) {
            if (kind == one) {
                value += 0xC8;
            } else {
                count++;
            }
        }
        switch (i) {
        case 0x19:
        case 0x1D:
        case 0x1E:
        case 0x1F:
        case 0x20:
        case 0x23:
            break;
        default:
            if (!(p->unk1E & 0x600) && Rand(10) == 0) {
                if (kind == one) {
                    value -= 0xC8;
                } else {
                    count--;
                }
            }
            break;
        }
        if (value > 0xFFFF) value = 0xFFFF;
        if (value < 0) value = 0;
        if (count > 0xFF) count = 0xFF;
        if (count < 0) count = 0;
        p->unk4 = value;
        p->unk2 = count;
        if (i == *(s16 *)(work + 0x6778)) {
            if (p->unk1E & 0x200) {
                func_8001AF70(0x4B, 1);
            } else {
                func_8001AF70(0x4B, 0);
            }
        }
        p->unk1E &= ~0x600;
    }
    if (((struct Ovl11RecordThresholds *)D_8006C838)->field_9F68 >= 0x5DD) {
        func_8001AF70(0xE0, 1);
    }
    if (((struct Ovl11RecordThresholds *)D_8006C838)->field_A6B8 >= 0x5DD) {
        func_8001AF70(0xE1, 1);
    }
    if (((struct Ovl11RecordThresholds *)D_8006C838)->field_AA60 >= 0x5DD) {
        func_8001AF70(0xE2, 1);
    }
    if (((struct Ovl11RecordThresholds *)D_8006C838)->field_B384 >= 0x5DD) {
        func_8001AF70(0xE3, 1);
    }
    if (((struct Ovl11RecordThresholds *)D_8006C838)->field_BE7C >= 0x5DD) {
        func_8001AF70(0xE4, 1);
    }
}
