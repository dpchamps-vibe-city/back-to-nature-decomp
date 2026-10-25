#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "screen_fade.h"
#include "scene_selection.h"

void ovl_30_func_8012F030(s32 arg0);
void ovl_30_func_8012FE94(void);
void func_8001FE00(s32 arg0);
s32 func_8001FABC(s16 arg0);
void func_8001FE34(s32 arg0);
void func_8001AF70(u16 arg0, u16 arg1);
void func_8002194C(void);

#define clamp_selection(expression) \
    ({ s32 value = (expression); \
       if ((u32)value >= 18) value = 17; \
       value; })

static inline s32 clamp_nonnegative(s32 value) {
    if (value < 0) {
        value = 0;
    }
    return value;
}

void ovl_30_func_8012F084(void) {
    s32 (*temp_v0_2)();
    s32 temp_v0_3;
    s32 temp_v1;
    s32 temp_v1_2;
    s32 var_v0;
    u32 temp_v0;
    u32 var_a1;
    s32 state = D_80134B0C;
    s32 *index;

    auto void copy_display(s32 arg0) __asm__("ovl_30_func_8012F030");

    switch (state) {
    case 0:
        D_80134B0C = 2;
        D_80134AD8 = 1;
        D_80134ADC = 0;
        D_80134AE0 = 0;
        D_80134B18 = D_80133FC0[0];
        D_80133FBC = 0;
        copy_display(0);
        break;
    case 2:
        D_80134B0C = 3;
        KanjiFntClose();
        KanjiFntOpen(8, 0x10, 0x140, 0xF0, 0x1C0, 0, 0x140, 0xFF, 0, 0xFF);
        FntFlush(-1);
        ovl_30_func_8012FE94();
        BeginScreenFadeDecrease(0xA, 0, 2);
        func_8001FE00(0xA);
        break;
    case 3:
        temp_v1 = *(s32 *) ((u8 *) gPad0Input + 8);
        if (temp_v1 & 2) {
            index = &D_80134ADC;
            D_80134AE0 = 0;
            D_80134B10 = -1;
            *index = clamp_selection(*index += 1);
            func_8001FABC(4);
        } else if (temp_v1 & 1) {
            temp_v1_2 = --D_80134ADC;
            D_80134AE0 = 0;
            D_80134B10 = -1;
            D_80134ADC = clamp_nonnegative(temp_v1_2);
            func_8001FABC(4);
        }
        temp_v0_2 = D_80133FC0[D_80134ADC];
        D_80134B18 = temp_v0_2;
        D_80134AE0 = temp_v0_2(D_80134AE0);
        temp_v0_3 = IsScreenFadeComplete();
        if (temp_v0_3 == 1) {
            if (D_80134AD8 == temp_v0_3) {
                if ((*(s32 *) ((u8 *) gPad0Input + 8)) & 0x800) {
                    BeginScreenFadeIncrease(0xA, 0, 2);
                    func_8001FE34(0xA);
                    D_80134AD8 = 0;
                }
            } else {
                D_80134B0C = 4;
            }
        }
        KanjiFntFlush(0);
        break;
    case 4: {
    auto void copy_display(s32 arg0) __asm__("ovl_30_func_8012F030");
    void copy_display(s32 arg0) {
        u_long *src;

        src = (u_long *)(D_8005E3B0 + 0x4290);
        if (arg0 == 0) {
            StoreImage((RECT *)Ovl30_DisplayRect, src);
        } else {
            LoadImage((RECT *)Ovl30_DisplayRect, src);
        }
        DrawSync(0);
    }
        copy_display(1);
        func_8001AF70(0x9CU, 0U);
        index = &D_80134B14;
        if (*index != ((struct Ovl30SettingsView *)&D_8007AFF0)->selected) {
            func_8001AF70(0x9CU, 1U);
            D_8005E32C = *index;
        }
        D_80134B0C = 0;
        BeginScreenFadeDecrease(0xA, 0, 2);
        func_8001FE00(0xA);
        ((struct struct_8006C838_view *)D_8006C838)->field_0C &= 0xF7FFFFFF;
        RequestSceneChange(2);
        func_8001AF70(0x9BU, 0U);
        if (D_80134008 != 0) {
            func_8001AF70(0x9BU, 1U);
        }
        break;
    }
    }
    func_8002194C();
}
