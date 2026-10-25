#include "common.h"
#include "sound_commands.h"

s32 GetVal8005E5B4(void);
s32 func_800226B0(void);
void func_800226F0(void);
void ovl_11_func_800BCF28(void);
void ovl_11_func_800FF96C(void);
void func_80022580(u32 *arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5);
void func_80017B3C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void SetVal8005E334(s32 arg0);
void SetVal8005E2BC(s32 arg0);
s32 func_8002261C(s32 arg0, s32 arg1);
void func_800248B0(s32 arg0, s16 arg1, s16 arg2);
void func_80022738(void);

void ovl_11_func_800FED0C(void) {
    s32 ready;
    u32 state;
    s32 buttons;
    s16 selection;
    u8 *base;

    if (D_801273B0 == 0x30) {
        if (GetVal8005E5B4() == 2) {
            base = (u8 *)D_8006C838;
            *(Ovl11MenuSettings *)(base + 0xE4CC) = D_8012CEE8_menu;
            func_800226F0();
            D_801273B0 = 0;
            ovl_11_func_800BCF28();
            *(u32 *)(base + 0xC) |= 0x40000;
            ovl_11_func_800FF96C();
        }
        return;
    }
    ready = func_800226B0();
    if (ready == 1 && (*(s32 *)((u8 *)gPad0Input + 8) & 0x800)) {
        D_801273B0 = 0x30;
        PlaySoundEffect(3);
        func_80022738();
        return;
    }
    state = D_801273B0;
    if (state != 1) {
        D_801273C0[(state >> 4) - 1]();
    } else {
        func_80022580((u32 *)(gActiveRenderContext->field_D8 + 0x64), 1, 0x64, 0x4A, 0x82, 0x22);
        func_80017B3C(gActiveRenderContext->field_D8 + 0x60, (s32)(D_80053AE8 + D_80054BBC[0]), 0x70, 0x4E);
        SetVal8005E334(0);
        SetVal8005E2BC(0);
        func_8002261C(3, 0x3B0);
        func_800248B0(gActiveRenderContext->field_D8 + 0x5C, 0x6C, (s16)(D_801273B4_selection[0] * 14 + 0x54));
        if (ready == 1) {
            buttons = *(s32 *)((u8 *)gPad0Input + 8);
            if (buttons & 0x20) {
                D_801273B0 = 0x30;
                PlaySoundEffect(3);
                func_80022738();
            } else if (buttons & 0x40) {
                func_80022738();
                PlaySoundEffect(0);
                D_801273B0 = (D_801273B4_selection[0] + 1) << 4;
            } else {
                buttons = *(s32 *)((u8 *)gPad0Input);
                if (buttons & 0x4000) {
                    PlaySoundEffect(5);
                    D_801273B4_selection[0]++;
                } else if (buttons & 0x1000) {
                    D_801273B4_selection[0]++;
                    PlaySoundEffect(5);
                }
                selection = D_801273B4_selection[0] / 2;
                D_801273B4_selection[0] -= selection * 2;
            }
        }
    }
}
