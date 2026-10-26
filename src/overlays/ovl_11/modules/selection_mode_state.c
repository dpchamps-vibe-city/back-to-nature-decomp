/*
 * selection_mode_state — reconstructed translation unit.
 *
 * Reset, entry variants and per-frame handler dispatch for the selection-mode
 * state block. gOvl11LastSelectionHandler is the private change-detection
 * latch; the installed handler and remaining work bytes are shared with the
 * mode's other controllers. Existing header views retain observed access
 * widths without new signed/unsigned aliases.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_selection_mode_state.h"
#include "sound_commands.h"
#include "screen_fade.h"
#include "cd_audio.h"
#include "sprite_grid.h"
#include "message_window.h"
#include "psyq/memory.h"
#include "ovl11_texture_stream.h"
#include "ovl11_texture_cell_cache.h"

void Ovl11ResetSelectionModeState(void) {
    s32 i;

    Ovl11_D_801273DC = 0;
    gOvl11LastSelectionHandler = 0;
    D_801273E4_state = 0;
    D_801273E5_origin = 0;
    D_801273E6 = 0xFF;
    Ovl11_D_801273E7 = 0;
    Ovl11_D_801273E8 = 0;
    D_801273E9_selection = 0;
    D_801273EA_table = 0;
    D_801273EB = 0;
    D_801273EE_flags = 0;
    for (i = 0; i < 8; i++) {
        D_8012CF00_ids[i] = 0;
    }
    D_801273F0 = 0;
    D_801273EC = 0;
    D_80127424 = 0;
}

void Ovl11ResetSelectionModeState(void);
void ovl_11_func_80100128(void);
void SetVal8005E2BC(s32 arg0);

void Ovl11EnterSelectionModeDefault(void) {
    ((struct struct_8006C838_view *)&D_8006C838)->field_0C |= 0x20000;
    Ovl11ResetSelectionModeState();
    Ovl11ResetTextureCellCache();
    D_801273E4_state = 1;
    D_801273E5_origin = 0;
    Ovl11_D_801273DC = ovl_11_func_80100128;
    PlaySoundEffect(3);
    Ovl11ServiceTextureStream(-1);
    SetMessagePortraitEnabled(0);
    SetVal8005E2BC(0);
    RequestMessageWindow(3, 0x3B5);
}

extern void ovl_11_func_8010021C(void);

void Ovl11EnterSelectionModeAlternate(void) {
    ((struct struct_8006C838_view *)&D_8006C838)->field_0C |= 0x20000;
    Ovl11ResetSelectionModeState();
    Ovl11ResetTextureCellCache();
    D_801273E4_state = 1;
    D_801273E5_origin = 1;
    Ovl11_D_801273DC = ovl_11_func_8010021C;
    PlaySoundEffect(3);
    Ovl11ServiceTextureStream(-1);
    SetMessagePortraitEnabled(0);
    SetVal8005E2BC(0);
    RequestMessageWindow(3, 0x3B5);
}

u32 func_80017A64(void);
void func_80017A48(u32 arg0);
void func_80017A38(s16 arg0, s16 arg1);

void Ovl11ServiceSelectionMode(void) {
    s32 orderingTable;
    u32 savedPalette;

    orderingTable = gActiveRenderContext->field_D8 + 0x78;
    if (D_801273E4_state != 0) {
        if (D_801273E4_state == 1) {
            DrawScrollingSpriteGrid(orderingTable);
            if ((IsMessageWindowStatusFinal() != 0) && (Ovl11ServiceTextureStream(0) == 0)) {
                CloseMessageWindow();
                D_801273E4_state = 3;
            }
        } else {
            DrawScrollingSpriteGridInContext(orderingTable);
            if (D_801273E4_state == 8) {
                if (GetCdAudioState() == 0) {
                    D_801273E4_state = 3;
                    BeginScreenFadeDecrease(0xA, 0, 2);
                }
            } else {
                if (gOvl11LastSelectionHandler != Ovl11_D_801273DC) {
                    D_801273EA_table = 0;
                    D_801273E9_selection = 0;
                    Ovl11_D_801273E8 = 0;
                    Ovl11_D_801273E7 = 0;
                }
                savedPalette = func_80017A64();
                func_80017A48(3U);
                func_80017A38(1, 2);
                Ovl11ClearTextureCellActivity();
                gOvl11LastSelectionHandler = Ovl11_D_801273DC;
                Ovl11_D_801273DC();
                func_80017A48(savedPalette);
                func_80017A38(0, 2);
            }
        }
    }
}
