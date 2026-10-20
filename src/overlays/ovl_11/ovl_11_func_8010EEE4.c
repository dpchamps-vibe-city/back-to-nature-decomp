#include "common.h"
#include "game_types.h"

s32 ovl_11_func_80110188(Ovl11StateArg0 *arg0, s16 arg1, s32 *arg2, s16 arg3);
void ovl_11_func_800D05D0(Ovl11SetFieldsView *arg0, Ovl11PaddedVec3 v);
s32 ovl_11_func_8010FBC4(Ovl11SetFieldsView *arg0, s32 arg1);
s32 ovl_11_func_8010FEF8(Ovl11SetFieldsView *arg0, s32 arg1);
s32 ovl_11_func_8010F8E4(Ovl11StateArg0 *arg0, s32 arg1);
s32 ovl_11_func_8010CE80(Recon_ovl_11_func_8010CE80_A0View *arg0, s32 arg1);
s32 ovl_11_func_80110544(Ovl11E644Object *arg0, s16 arg1);
s16 ovl_11_func_80110944(s16 area, u16 mode);

s32 ovl_11_func_8010EEE4(Ovl11EEE4Object *arg0) {
    Ovl11PaddedVec3 position;
    s32 active;
    s32 result;
    s16 selected;
    Ovl11D55F8Object *base;

    /* This nested helper captures arg0. Its definition emits the original
     * 8010EE5C code immediately before this parent, including the static chain.
     * See notes/research/ovl_11_func_8010EE5C-grouping.md. */
    void ovl_11_func_8010EE5C(Ovl11SetFieldsView *object, s16 mode, s32 flag) {
        if (object->field_30 == mode) {
            *(s16 *)((u8 *)object + 0xBA) = -1;
            *(s16 *)((u8 *)object + 0x28) = 0;
            object->field_34 &= ~flag;
            *(u16 *)((u8 *)object + 0xB8) |= 0x200;
            return;
        }
        object->field_34 |= flag;
        ovl_11_func_80110544((Ovl11E644Object *)arg0, 0xE6);
    }

    active = 1;
    base = (Ovl11D55F8Object *)arg0;
    base->unk34 |= 0x800;
    if (base->unk28 == -1) {
        ovl_11_func_8010EE5C((Ovl11SetFieldsView *)base, 5, 0x04000000);
    } else if (base->unk28 == -2) {
        ovl_11_func_8010EE5C((Ovl11SetFieldsView *)base, 4, 0x20000000);
    } else if (base->unk28 == -3) {
        ovl_11_func_8010EE5C((Ovl11SetFieldsView *)base, 1, 0x04000000);
    } else if (base->unk28 == -4) {
        ovl_11_func_8010EE5C((Ovl11SetFieldsView *)base, 1, 0x20000000);
    } else {
        if (base->unk30 == 1) {
            if (!(*(u16 *)((u8 *)base + 0xB8) & 0x100)) {
                if (ovl_11_func_80110944(5, 1) != -1 && D_80070D3A != 0) {
                    base->unk28 = -1;
                    ovl_11_func_80110188((Ovl11StateArg0 *)arg0, 5, &position.field_0, base->unk30);
                    ovl_11_func_800D05D0((Ovl11SetFieldsView *)base, position);
                } else if ((selected = ovl_11_func_80110944(5, 6)) != -1) {
                    base->unk28 = -1;
                    ovl_11_func_80110188((Ovl11StateArg0 *)arg0, 5, &position.field_0, base->unk30);
                    ovl_11_func_800D05D0((Ovl11SetFieldsView *)base, position);
                } else if (ovl_11_func_80110944(4, 1) != selected && D_80070D40 != 0) {
                    base->unk28 = -2;
                    ovl_11_func_80110188((Ovl11StateArg0 *)arg0, 4, &position.field_0, base->unk30);
                    ovl_11_func_800D05D0((Ovl11SetFieldsView *)base, position);
                } else if (ovl_11_func_80110944(4, 8) != -1) {
                    base->unk28 = -2;
                    ovl_11_func_80110188((Ovl11StateArg0 *)arg0, 4, &position.field_0, base->unk30);
                    ovl_11_func_800D05D0((Ovl11SetFieldsView *)base, position);
                } else {
                    arg0->flags |= 0x100;
                }
                arg0->index = -1;
                arg0->flags |= 0x200;
            } else {
                base->unk34 &= 0xC3FFFFFF;
                if (*(u16 *)((u8 *)base + 0xB8) & 0x200) {
                    if (ovl_11_func_8010FBC4((Ovl11SetFieldsView *)base, 0) != 0) {
                        arg0->index = -1;
                        arg0->flags &= 0xFDFF;
                    }
                } else if (ovl_11_func_8010FEF8((Ovl11SetFieldsView *)base, 0) != 0) {
                    active = 0;
                }
            }
        } else if (base->unk30 == 5) {
            base->unk34 &= 0xC3FFFFFF;
            if (*(u16 *)((u8 *)base + 0xB8) & 0x200) {
                if (ovl_11_func_8010F8E4((Ovl11StateArg0 *)base, 1) != 0) {
                    arg0->index = -1;
                    arg0->flags &= 0xFDFF;
                }
            } else if (ovl_11_func_8010FBC4((Ovl11SetFieldsView *)base, 1) != 0) {
                if (ovl_11_func_80110944(4, 9) != -1) {
                    base->unk28 = -3;
                    ovl_11_func_80110188((Ovl11StateArg0 *)arg0, 5, &position.field_0, base->unk30);
                    ovl_11_func_800D05D0((Ovl11SetFieldsView *)base, position);
                } else if (ovl_11_func_80110944(1, 10) != -1) {
                    base->unk28 = -3;
                    arg0->flags |= 0x100;
                    ovl_11_func_80110188((Ovl11StateArg0 *)arg0, 5, &position.field_0, base->unk30);
                    ovl_11_func_800D05D0((Ovl11SetFieldsView *)base, position);
                } else {
                    active = 0;
                }
            }
        } else if (base->unk30 == 4) {
            base->unk34 &= 0xC3FFFFFF;
            if (*(u16 *)((u8 *)base + 0xB8) & 0x200) {
                if (ovl_11_func_8010F8E4((Ovl11StateArg0 *)base, 0) != 0) {
                    arg0->index = -1;
                    arg0->flags &= 0xFDFF;
                }
            } else if (ovl_11_func_8010FEF8((Ovl11SetFieldsView *)base, 0) != 0) {
                if (ovl_11_func_80110944(1, 10) != -1) {
                    base->unk28 = -4;
                    arg0->flags |= 0x100;
                    ovl_11_func_80110188((Ovl11StateArg0 *)arg0, 4, &position.field_0, base->unk30);
                    ovl_11_func_800D05D0((Ovl11SetFieldsView *)base, position);
                } else {
                    active = 0;
                }
            }
        } else {
            active = 0;
        }
    }
    if (active == 1) {
        result = 0;
        arg0->saved26 = base->unk26;
        arg0->saved28 = base->unk28;
        arg0->saved2A = base->unk2A;
    } else {
        result = 1;
        arg0->saved26 = 0;
        base->unk34 &= ~0x800;
        arg0->flags |= 0x40;
        ovl_11_func_8010CE80((Recon_ovl_11_func_8010CE80_A0View *)arg0, 7);
        base->unk34 |= 0x3C000000;
        arg0->flags &= 0xFCFF;
    }
    return result;
}
