#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"

void ovl_21_func_800BA9F8(void);
s32 func_8001FABC(s16 arg0);
void ovl_21_func_800BAA18(void);
void ovl_21_func_800BA4C0(void);
void ovl_21_func_800BA4F0(void);
s32 ovl_21_func_800B98CC(Ovl21Func800B98CCArg *arg0);
void ovl_21_func_800B9A20(void);
void ovl_21_func_800B9CA4(void);
void ovl_21_func_800B9F3C(void);
void ovl_21_func_800B990C(void);
void func_8001AC10(u32 *arg0, s32 arg1, s32 arg2);
s32 ovl_21_func_800BA7F0(s16 arg0);
void func_80022738(void);
void ovl_21_func_800BA300(void);

void ovl_21_func_800B8B3C(void) {
    s32 i;
    s32 temp;
    s32 text;

    if (*(u32 *)((u8 *)D_8005E3A8 + 8) & 0x800) {
        temp = ((Ovl21C0448View *)D_800C0448)->unk4 != 1;
        ((Ovl21C0448View *)D_800C0448)->unk4 = temp;
        if (temp == 1) {
            ovl_21_func_800BA9F8();
            ((Ovl21C0448View *)D_800C0448)->unk984 = func_8001FABC(0x36);
        } else {
            ovl_21_func_800BAA18();
        }
    }
    if (((Ovl21C0448View *)D_800C0448)->unk4 == 1) {
        ovl_21_func_800BA4C0();
        ovl_21_func_800BA4F0();
        return;
    }
    if (((UnkStruct800C0448 *)D_800C0448)->unk14 == 1) {
        switch (*(u32 *)((u8 *)D_8005E3A8 + 8)) {
        case 0x8000:
            ((UnkStruct800C0448 *)D_800C0448)->unk2C--;
            break;
        case 0x2000:
            ((UnkStruct800C0448 *)D_800C0448)->unk2C++;
            break;
        case 0x10:
            if (((u16 *)((UnkStruct800C0448 *)D_800C0448)->unk30)[1] & 0x100) {
                ((UnkStruct800C0448 *)D_800C0448)->unk18 = 6;
            }
            break;
        case 0x40:
            if (((u16 *)((UnkStruct800C0448 *)D_800C0448)->unk30)[1] & 0x100) {
                if (((UnkStruct800C0448 *)D_800C0448)->unk18 != 2) {
                    ((UnkStruct800C0448 *)D_800C0448)->unk18 = 2;
                } else {
                    ovl_21_func_800B98CC((Ovl21Func800B98CCArg *)((u8 *)D_800C0448 + 0x10));
                }
            }
            break;
        }
        if (((UnkStruct800C0448 *)D_800C0448)->unk2C < 3) {
            ((UnkStruct800C0448 *)D_800C0448)->unk2C = 5;
        }
        if (((UnkStruct800C0448 *)D_800C0448)->unk2C >= 6) {
            ((UnkStruct800C0448 *)D_800C0448)->unk2C = 3;
        }
    } else if (((Ovl21C0448View *)D_800C0448)->unk988 == 0 &&
               (*(u32 *)((u8 *)D_8005E3A8 + 8) & 0xA050)) {
        ((Ovl21C0448View *)D_800C0448)->unk984 = func_8001FABC(2);
        *(void **)((u8 *)D_800C0448 + 0xC) = D_800BCA78;
        ((Ovl21C0448View *)D_800C0448)->unk988 = 1;
    }
    if (((Ovl21C0448View *)D_800C0448)->unk8 ==
        ((Ovl21C0448View *)D_800C0448)->unk8 / ((Ovl21C0448View *)D_800C0448)->unk64E * ((Ovl21C0448View *)D_800C0448)->unk64E) {
        ovl_21_func_800B9A20();
    }
    if (((Ovl21C0448View *)D_800C0448)->unk8 ==
        ((Ovl21C0448View *)D_800C0448)->unk8 / ((Ovl21C0448View *)D_800C0448)->unk650 * ((Ovl21C0448View *)D_800C0448)->unk650) {
        ovl_21_func_800B9CA4();
    }
    for (i = 0; i < 6; i++) {
        if (((UnkStruct800C0448 *)D_800C0448)[i].unk14 == 1 &&
            ((UnkStruct800C0448 *)D_800C0448)[i].unk18 == 4 &&
            (((u16 *)((UnkStruct800C0448 *)D_800C0448)[i].unk30)[1] & 0x100)) {
            ((UnkStruct800C0448 *)D_800C0448)[i].unk18 = 2;
        }
    }
    if (((Ovl21C0448View *)D_800C0448)->unk8 ==
        ((Ovl21C0448View *)D_800C0448)->unk8 / ((Ovl21C0448View *)D_800C0448)->unk648 * ((Ovl21C0448View *)D_800C0448)->unk648) {
        ovl_21_func_800B9F3C();
    }
    ovl_21_func_800BA300();
    ovl_21_func_800B990C();
    for (i = 0; i < 6; i++) {
        if (((UnkStruct800C0448 *)D_800C0448)[i].unk18 == 7) {
            ((UnkStruct800C0448 *)D_800C0448)[i].unk2E++;
            if (((UnkStruct800C0448 *)D_800C0448)[i].unk2E >= 30) {
                ((UnkStruct800C0448 *)D_800C0448)[i].unk2E = 0;
                ((UnkStruct800C0448 *)D_800C0448)[i].unk18 = 9;
            }
        }
    }
    ovl_21_func_800BA4C0();
    if (((Ovl21C0448View *)D_800C0448)->unk988 != 0) {
        if (*(void **)((u8 *)D_800C0448 + 0xC) != NULL) {
            text = gActiveRenderContext->field_D8;
            func_8001AC10((u32 *)(text + 0xC), text + 8, (s32)((Ovl21C0448View *)D_800C0448)->unkC);
        }
        (((Ovl21C0448View *)D_800C0448)->unk988)++;
        if (((Ovl21C0448View *)D_800C0448)->unk988 >= 90) {
            ((Ovl21C0448View *)D_800C0448)->unk988 = 0;
        }
    }
    (((Ovl21C0448View *)D_800C0448)->unk8)--;
    if (((Ovl21C0448View *)D_800C0448)->unk8 <= 0 ||
        ovl_21_func_800BA7F0(0) == 0 || ovl_21_func_800BA7F0(1) == 0) {
        ((Ovl21C0448View *)D_800C0448)->unk986 = func_8001FABC(0x39);
        func_80022738();
        D_800C0448[0] = 8;
        ((Ovl21C0448View *)D_800C0448)->unk988 = 0;
    }
}
