/*
 * interaction_sequence — reconstructed translation unit.
 *
 * Interaction sequence entry, rotation-ramp waits, message service and
 * teardown.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_interaction_sequence.h"
#include "psyq/memory.h"
#include "ovl11_texture_cell_cache.h"
#include "game_types.h"
#include "pad_actuator_sequences.h"
#include "ovl11_texture_stream.h"
#include "game_flags.h"
#include "sprite_grid.h"
#include "sound_commands.h"
#include "ovl11_scene_rotation_ramps.h"
#include "message_window.h"

void Ovl11ResetInteractionSequence(void) {
    gOvl11InteractionPhase = 0xFF;
    D_80126E44 = 0;
    D_80126E46 = 0;
    D_80126E48 = 0xFF;
    D_80126E4A = 0;
    D_80126E4C = 0;
    D_80126E4E = 0xFF;
    gOvl11SharedSpriteFrame = 0;
    gOvl11SharedSpriteFrameTicks = 0;
    gOvl11TextureStreamCursor = 0;
    Ovl11ResetTextureCellCache();
}

void Ovl11ResetInteractionSequence(void);

void Ovl11StartSceneZRotationInteraction(void) {
    Ovl11ResetInteractionSequence();
    gOvl11InteractionPhase = 6;
    SetGameFlag(0x71, 1);
    RequestPadActuatorSequence(0, 2);
}

void Ovl11StartSceneYRotationInteraction(void) {
    Ovl11ResetInteractionSequence();
    gOvl11InteractionPhase = 7;
    SetGameFlag(0x70, 1);
    RequestPadActuatorSequence(0, 2);
}

void SetVal8005E2BC(s32 arg0);

void Ovl11StartMessageInteraction(void) {
    Ovl11ResetInteractionSequence();
    ((struct struct_8006C838_view *)&D_8006C838)->field_0C |= 0x20000;
    PlaySoundEffect(3);
    gOvl11InteractionPhase = 4;
    SetMessagePortraitEnabled(0);
    SetVal8005E2BC(0);
    RequestMessageWindow(3, 0x37A);
}

void ovl_11_func_800BCF28(void);
u32 func_80017A64(void);
void func_80017A48(u32 arg0);
void ovl_11_func_800F787C(void);

void Ovl11ServiceInteractionSequence(void) {
    /* id/next deliberately serve both ramp-message and texture phases. */
    s32 (*check)(void);
    s32 id;
    s32 next;
    u32 savedPalette;

    if (gOvl11InteractionPhase != 0xFF) {
        if (gOvl11InteractionPhase == 6 || gOvl11InteractionPhase == 7) {
            if (gOvl11InteractionPhase == 6) {
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
                gOvl11InteractionPhase = next;
            }
        } else {
            DrawScrollingSpriteGrid(gActiveRenderContext->field_D8 + 0x6C);
            if (gOvl11InteractionPhase == 3 || gOvl11InteractionPhase == 5 || gOvl11InteractionPhase == 4) {
                if (gOvl11InteractionPhase == 3) {
                    id = 0;
                    next = 0;
                } else if (gOvl11InteractionPhase == 5) {
                    id = 1;
                    next = 2;
                } else {
                    id = 2;
                    next = 1;
                }
                if (IsMessageWindowStatusFinal()) {
                    if (Ovl11StepTextureStream(id) == 0) {
                        CloseMessageWindow();
                        gOvl11InteractionPhase = next;
                    }
                }
            } else if (gOvl11InteractionPhase == 0xFE) {
                if (GetMessageWindowStatus() == 2) {
                    ResetMessageWindow();
                    gOvl11InteractionPhase = 0xFF;
                    ovl_11_func_800BCF28();
                    ((struct struct_8006C838_view *)&D_8006C838)->field_0C |= 0x40000;
                    ((struct struct_8006C838_800E8F78 *)&D_8006C838)->field_5234 |= 0x2000;
                    SetGameFlag(0x71, 0);
                    SetGameFlag(0x70, 0);
                }
            } else {
                Ovl11ClearTextureCellActivity();
                savedPalette = func_80017A64();
                func_80017A48(3);
                ovl_11_func_800F787C();
                func_80017A48(savedPalette);
            }
        }
    }
}
