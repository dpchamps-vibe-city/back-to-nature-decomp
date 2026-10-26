#include "common.h"
#include "sprite_grid.h"
#include "sound_commands.h"
#include "ovl11_scene_rotation_ramps.h"
#include "message_window.h"
#include "psyq/memory.h"
#include "ovl11_texture_stream.h"
#include "game_flags.h"
#include "ovl11_texture_cell_cache.h"

void SetVal8005E2BC(s32 arg0);
void ovl_11_func_800BCF28(void);
u32 func_80017A64(void);
void func_80017A48(u32 arg0);
void ovl_11_func_800F787C(void);

void ovl_11_func_800F67E0(void) {
    s32 (*check)(void);
    s32 id;
    s32 next;
    u32 saved;

    if (D_80126E40 != 0xFF) {
        if (D_80126E40 == 6 || D_80126E40 == 7) {
            if (D_80126E40 == 6) {
                check = Ovl11IsSceneZRotationRampHeld;
                id = 0x378;
                next = 3;
            } else {
                check = Ovl11IsSceneYRotationRampHeld;
                id = 0x379;
                next = 5;
            }
            if (check()) {
                ((struct struct_8006C838_view *)&D_8006C838)->field_0C |= 0x20000;
                PlaySoundEffect(3);
                SetMessagePortraitEnabled(0);
                SetVal8005E2BC(0);
                RequestMessageWindow(3, id);
                D_80126E40 = next;
            }
        } else {
            DrawScrollingSpriteGrid(gActiveRenderContext->field_D8 + 0x6C);
            if (D_80126E40 == 3 || D_80126E40 == 5 || D_80126E40 == 4) {
                if (D_80126E40 == 3) {
                    id = 0;
                    next = 0;
                } else if (D_80126E40 == 5) {
                    id = 1;
                    next = 2;
                } else {
                    id = 2;
                    next = 1;
                }
                if (IsMessageWindowStatusFinal()) {
                    if (Ovl11StepTextureStream(id) == 0) {
                        CloseMessageWindow();
                        D_80126E40 = next;
                    }
                }
            } else if (D_80126E40 == 0xFE) {
                if (GetMessageWindowStatus() == 2) {
                    ResetMessageWindow();
                    D_80126E40 = 0xFF;
                    ovl_11_func_800BCF28();
                    ((struct struct_8006C838_view *)&D_8006C838)->field_0C |= 0x40000;
                    ((struct struct_8006C838_800E8F78 *)&D_8006C838)->field_5234 |= 0x2000;
                    SetGameFlag(0x71, 0);
                    SetGameFlag(0x70, 0);
                }
            } else {
                Ovl11ClearTextureCellActivity();
                saved = func_80017A64();
                func_80017A48(3);
                ovl_11_func_800F787C();
                func_80017A48(saved);
            }
        }
    }
}
