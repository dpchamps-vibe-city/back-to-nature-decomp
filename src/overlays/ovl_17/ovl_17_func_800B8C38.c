#include "common.h"

void ovl_17_func_800BA808(s32 arg0);
void ovl_17_func_800B91B8(void);
void func_80017A08(s32 arg0, s32 arg1);
s32 func_8002261C(s32 arg0, s32 arg1);
/* GetVal8005E5B4, named by its original overlay import. */
s32 GetVal8005E5B4(void);
void func_8001FAE8(s32 arg0);

void ovl_17_func_800B8C38(void) {
    s32 i;
    s32 selected;
    s32 data;
    s32 name;
    s32 entry;
    s32 chosen;
    u8 *base;
    u8 *state;
    u8 *current;

    ovl_17_func_800BA808(0x800000);
    selected = 0;
    ovl_17_func_800B91B8();
    for (i = 0; i < 6; i++) {
        base = D_800BD848;
        entry = ((Ovl17DisplaySetup *)base)->entries[i];
        if (entry == 2) {
            selected = i;
        } else {
            data = (s32)D_800556A8;
            func_80017A08((u16)i, *(u16 *)(base + entry * 0x50 + 0x40) + data);
        }
    }
    if (((Ovl17ModeView *)D_8006C838)->mode == 3) {
        name = (s32)&D_80075AD8;
    } else {
        current = D_800BD848;
        chosen = ((Ovl17DisplaySetup *)current)->entries[selected];
        name = (s32)D_800556A8 + *(u16 *)(current + chosen * 0x50 + 0x40);
    }
    func_80017A08((u16)selected, name);
    func_8002261C(4, 6);
    if (GetVal8005E5B4() == 2) {
        state = D_800BD848;
        func_8001FAE8(*(s16 *)(state + 0x2CA));
        state[4] = 9;
    }
}
