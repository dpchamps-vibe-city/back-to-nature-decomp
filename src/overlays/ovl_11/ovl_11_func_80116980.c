#include "common.h"
#include "game_types.h"

void func_80015114(u32 *, s16, s16, s16, s16, s32, s16);
void func_80022580(u32 *, s32, s32, s32, s16, s16);
void func_80017B3C(s32, s32, s32, s32);
s32 ovl_11_func_80117178(s16, s16, s16);
s32 func_8001FABC(s16);
void func_80024770(s32, s16, s16);
void func_800247C0(s32, s16, s16);
void func_800248B0(s32, s16, s16);
void func_8001AAF4(s16, s16, s32, s16, s16);
void ovl_11_func_80116F4C(Ovl11Func80116F4CArg *, s32, s16, s16);

s32 ovl_11_func_80116980(s16 arg0, s16 arg1, s16 arg2) {
    s32 input_mode;
    s32 result;
    Ovl11Func80116F4CArg *record;
    u16 terminator;
    s32 row_y;
    s16 icon;
    s16 kind;
    s32 ui;
    s32 row_offset;
    s32 next_index;
    s32 record_index;
    s32 header_ot;
    s32 row_ot;
    s32 count;
    s32 repeat_buttons;
    s32 pressed_buttons;
    s32 i;
    s32 prefix_value;
    s32 last_index;
    u8 *name;
    s32 text_index1;
    s32 text_index2;
    s32 text_index3;
    s32 text_index4;
    s32 next_row;

    input_mode = (s32) arg2;
    if (arg2 == -1) {
        D_8012D518 = 0;
        D_8012D51A = 0;
        return -2;
    }
    ui = D_8005E3C0->field_D8;
    header_ot = ui + 0x38;
    row_ot = ui + 0x40;
    kind = arg0 >= 0 ? (arg0 < 3 ? arg0 : 2) : 0;
    result = -1;
    func_80015114((u32 *)(D_8005E3C0->field_D8 + 0x48), 0, 0, *(s16 *)&HWD0, *(s16 *)&VWD0, 0, 1);
    i = 0;
    func_80022580((u32 *)(D_8005E3C0->field_D8 + 0x44), 1, 0x18, 0x20, 0x110, 0x74);
    func_80022580((u32 *)(D_8005E3C0->field_D8 + 0x3C), 1, 0x50, 0x10, 0xA0, 0x20);
    for (;;) {
        prefix_value = ((Ovl11MenuGlobals *)D_8006C838)->text[i];
        terminator = prefix_value;
        if (terminator == 0xFFFF) {
            name = &D_800535C6[*D_80054BBC];
            (&D_8012D608)[i] = ((u16 *)name)[0];
            text_index1 = i + 1;
            (&D_8012D608)[text_index1] = ((u16 *)name)[1];
            text_index2 = i + 2;
            (&D_8012D608)[text_index2] = ((u16 *)name)[2];
            text_index3 = i + 3;
            (&D_8012D608)[text_index3] = ((u16 *)name)[3];
            text_index4 = i + 4;
            (&D_8012D608)[text_index4] = terminator;
            break;
        }
        (&D_8012D608)[i] = prefix_value;
        i++;
    }
    func_80017B3C(header_ot, (s32) &D_8012D608, 0x52, 0x1A);
    D_8012823C[kind](header_ot, 0xB0, 0x12);
    func_80017B3C(header_ot, (s32) &(D_800535C6 + 0x3E)[*D_80054BBC], 0xC8, 0x1A);
    count = ovl_11_func_80117178(kind, arg1, -1);
    if (input_mode == 0) {
        repeat_buttons = *(s32 *) ((u8 *) D_8005E3A8 + 0);
        if (repeat_buttons & 0x1000) {
            func_8001FABC(5);
            D_8012D518--;
        } else if (repeat_buttons & 0x4000) {
            func_8001FABC(5);
            D_8012D518++;
        }
        last_index = count - 1;
        if ((s16) D_8012D518 < 0) {
            D_8012D518 = last_index;
        }
        if ((s32) last_index < (s16) D_8012D518) {
            D_8012D518 = 0;
        }
        if (count >= 5) {
            if ((D_8012D51A + 3) < (s16) D_8012D518) {
                D_8012D51A = D_8012D518 - 3;
            }
            if ((s16) D_8012D518 < D_8012D51A) {
                D_8012D51A = (s16) D_8012D518;
            }
        }
    }
    if (D_8012D51A > 0) {
        func_80024770(row_ot, 0x120, 0x30);
    }
    if ((D_8012D51A + 4) < count) {
        func_800247C0(row_ot, 0x120, 0x80);
    }
    func_800248B0(row_ot, 0x20, (s16) ((((s16) D_8012D518 - D_8012D51A) * 0x18) + 0x3C));
    for (i = 0; i < 4 && (next_row = i + 1, count >= D_8012D51A + next_row); i++) {
        row_offset = i * 0x18;
        next_index = i + 1;
        record_index = ovl_11_func_80117178(kind, arg1, (s16) ((u16) D_8012D51A + i + 1));
        func_8001AAF4((s16) ((u16) D_8012D51A + next_index), 2, row_ot, 0x20, (s16) (s32) (s16) (row_offset + 0x36));
        row_y = row_offset + 0x30;
        if (kind == 0) {
            record = (Ovl11Func80116F4CArg *) ((u8 *) &D_800742EC + (record_index * 0xB4));
        } else {
            record = (Ovl11Func80116F4CArg *) ((u8 *) &D_800749F4 + (record_index * 0xB8));
        }
        icon = kind;
        if (kind == 2) {
            if (((Ovl11MenuGlobals *) ((u8 *) D_8006C838 + record_index * 0xB8))->status < 7U) {
                icon = 3;
            }
        }
        D_8012823C[icon](row_ot, 0x40, row_y);
        ovl_11_func_80116F4C(record, row_ot, 0x58, row_y);
    }
    if (input_mode == 0) {
        pressed_buttons = *(s32 *) ((u8 *) D_8005E3A8 + 8);
        if (pressed_buttons & 0x20) {
            func_8001FABC(1);
            result = -2;
        } else if (pressed_buttons & 0x40) {
            func_8001FABC(0);
            result = ovl_11_func_80117178(kind, arg1, (s16) (D_8012D518 + 1));
        }
    } else {
        result = ovl_11_func_80117178(kind, arg1, (s16) (D_8012D518 + 1));
    }
    return result;
}
