#include "common.h"
#include "screen_fade.h"
#include "cd_audio.h"
#include "sprite_grid.h"
#include "message_window.h"
#include "psyq/memory.h"
#include "ovl11_texture_stream.h"

u32 func_80017A64(void);
void func_80017A48(u32 arg0);
void func_80017A38(s16 arg0, s16 arg1);
void ovl_11_func_800F69FC(void);

typedef void (*Ovl11StateFunc)(void);
extern Ovl11StateFunc D_801273DC;
extern Ovl11StateFunc D_801273E0;
extern u8 D_801273E4;
extern u8 D_801273E7;
extern u8 D_801273E8;
extern u8 D_801273E9;
extern u8 D_801273EA;

void ovl_11_func_800FFF7C(void) {
    s32 temp_a0;
    u32 temp_s0;

    temp_a0 = gActiveRenderContext->field_D8 + 0x78;
    if (D_801273E4 != 0) {
        if (D_801273E4 == 1) {
            DrawScrollingSpriteGrid(temp_a0);
            if ((IsMessageWindowStatusFinal() != 0) && (Ovl11ServiceTextureStream(0) == 0)) {
                CloseMessageWindow();
                D_801273E4 = 3;
            }
        } else {
            DrawScrollingSpriteGridInContext(temp_a0);
            if (D_801273E4 == 8) {
                if (GetCdAudioState() == 0) {
                    D_801273E4 = 3;
                    BeginScreenFadeDecrease(0xA, 0, 2);
                }
            } else {
                if (D_801273E0 != D_801273DC) {
                    D_801273EA = 0;
                    D_801273E9 = 0;
                    D_801273E8 = 0;
                    D_801273E7 = 0;
                }
                temp_s0 = func_80017A64();
                func_80017A48(3U);
                func_80017A38(1, 2);
                ovl_11_func_800F69FC();
                D_801273E0 = D_801273DC;
                D_801273DC();
                func_80017A48(temp_s0);
                func_80017A38(0, 2);
            }
        }
    }
}
