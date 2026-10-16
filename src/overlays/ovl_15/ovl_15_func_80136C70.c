#include "common.h"
#include "psyq/memory.h"

void func_80022580(u32 *, s32, s32, s32, s16, s16);
void func_80017A08(s32, s32);
void func_80017B3C(s32, s32, s32, s32);
void ovl_15_func_8013703C(s16, s16 *, s32);
u16 *func_8001AA7C(s32, u16 *);

void ovl_15_func_80136C70(s16 slot, s16 mode) {
    s16 *dayText;
    s16 *seasonText;
    s16 y;
    s16 status;
    s32 offset;
    s32 i;
    u16 glyph;
    struct Ovl15SaveHeader *header;
    void *headers;

    y = slot * 0x31 + 0x14;
    func_80022580((u32 *)(D_8005E3C0->field_D8 + 0x18), 1, 0x30, (s32) (s16) (slot * 0x31 + 0x10), 0xE4, 0x30);
    func_80017A08(1, (s32) ((void *) ((u8 *) &D_801375A8 + (slot * 4))));
    func_80017B3C(D_8005E3C0->field_D8 + 0x14, (s32) ((void *) ((u8 *) &D_80051D8E + *D_80054BBC)), 0x41, (s32) y);
    status = D_80137820[slot];
    switch (status) {
    case 1:
        func_80017B3C(D_8005E3C0->field_D8 + 0x14, (s32)((void *) ((u8 *) &D_80051D8E - 0x13A)) + D_80054BBC[0], 0x41, (s16)(y + 0x15));
        return;
    case 2:
        if (mode == 0) {
            func_80017B3C(D_8005E3C0->field_D8 + 0x14, (s32)((void *) ((u8 *) &D_80051D8E - 0x13A)) + D_80054BBC[0], 0x41, (s16)(y + 0x15));
        } else if (mode == 1) {
            func_80017B3C(D_8005E3C0->field_D8 + 0x14, (s32)((void *) ((u8 *) &D_80051D8E - 0x128)) + D_80054BBC[0], 0x41, (s16)(y + 0x15));
        }
        break;
    case 3:
        if (mode == 0) {
            func_80017B3C(D_8005E3C0->field_D8 + 0x14, (s32)((void *) ((u8 *) &D_80051D8E - 0x13A)) + D_80054BBC[0], 0x41, (s16)(y + 0x15));
        } else if (mode == 1) {
            func_80017B3C(D_8005E3C0->field_D8 + 0x14, (s32)((void *) ((u8 *) &D_80051D8E - 0xFC)) + D_80054BBC[0], 0x41, (s16)(y + 0x15));
        }
        break;
    case 0:
        for (i = 0; i < 8; i++) {
            glyph = D_801376E0_headers[slot].name_8.units[i];
            if ((glyph >= 0xE01U) && (glyph != 0xFFD)) {
                D_801376E0_headers[slot].name_8.units[i] = 0xFFFF;
            }
        }
        for (i = 0; i < 8; i++) {
            glyph = D_801376E0_headers[slot].name_1A.units[i];
            if ((glyph >= 0xE01U) && (glyph != 0xFFD)) {
                D_801376E0_headers[slot].name_1A.units[i] = 0xFFFF;
            }
        }
        y += 0xE;
        memset(D_80140EE0, 0xFF, 0x28);
        headers = D_801376E0_headers;
        offset = slot * sizeof(struct Ovl15SaveHeader);
        header = (void *)((u8 *)headers + offset);
        ovl_15_func_8013703C((s16) (((u16 *)&header->words)[0] + 1), D_80140EE0, 2);
        func_80017A08(2, (s32) D_80140EE0);
        seasonText = D_80140EE0 + 5;
        *(s16 *)func_8001AA7C((s32) ((s16 *)&header->words)[1], (u16 *) seasonText) = -1;
        func_80017A08(3, (s32) seasonText);
        dayText = D_80140EE0 + 0xD;
        ovl_15_func_8013703C((s16) (((u16 *)&header->words)[2] + 1), dayText, 2);
        func_80017A08(4, (s32) dayText);
        header->name_8.units[8] = 0xFFFF;
        func_80017A08(5, (s32)D_801376E0_headers[slot].name_8.units);
        func_80017B3C(D_8005E3C0->field_D8 + 0x14, (s32) ((void *) ((u8 *) &D_80051D9E + *D_80054BBC)), 0x41, (s32) y);
        y += 0xE;
        func_80017A08(6, (s32)D_801376E0_headers[slot].name_1A.units);
        header->name_1A.units[8] = 0xFFFF;
        func_80017B3C(D_8005E3C0->field_D8 + 0x14, (s32) ((void *) ((u8 *) ((void *) ((u8 *) &D_80051D9E + 0x1C)) + *D_80054BBC)), 0x41, (s32) y);
        return;
    }
}
