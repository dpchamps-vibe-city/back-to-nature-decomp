/*
 * Kanji scene-selection menu, ovl_08:800B8014..800B8400.
 *
 * The original three-word callback table drives initialization, a wait for
 * sound transfer, then the interactive page. The phase, selected row and
 * 32-entry scene/label table are overlay data, not new C initializers.
 * See ovl08_scene_select_menu.h for the public per-frame entry and effects;
 * notes/file-groupings.md records membership and the unresolved right edge.
 */
#include "common.h"
#include "ovl08_scene_select_menu.h"
#include "screen_fade.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "card_deal.h"
#include "master_volume.h"
#include "scene_selection.h"

void Ovl08UpdateSceneSelectMenu(void) {
    s32 *handlers = &gOvl08SceneSelectHandlers;

    /* The original dispatcher passes the table address. Its three handlers
     * ignore that argument; keep the word-table ABI rather than replacing
     * the stored callbacks or adding parameters to the handlers. */
    ((void (*)(s32 *))handlers[gOvl08SceneSelectPhase])(handlers);
}

void Ovl08InitSceneSelectMenu(void) {
    u8 *drawEnvironment;

    gSceneSubphase = 0;
    KanjiFntClose();
    KanjiFntOpen(8, 0, 0x140, 0xF0, 0x1C0, 0, 0x140, 0xFF, 0, 0xFF);
    BeginScreenFadeDecrease(1, 0, 2);
    drawEnvironment = (u8 *)&gRenderContexts[0];
    drawEnvironment[0x19] = 0x40;
    drawEnvironment[0x1A] = 0x40;
    drawEnvironment[0x1B] = 0x80;
    drawEnvironment += 0x134;
    drawEnvironment[0x19] = 0x40;
    drawEnvironment[0x1A] = 0x40;
    drawEnvironment[0x1B] = 0x80;
    DealCardGameTable();
    gOvl08SceneSelectPhase += 1;
}

void Ovl08WaitSceneSelectSoundTransfer(void) {
    if (IsSoundTransferComplete() != 0) {
        gOvl08SceneSelectPhase += 1;
    }
}

/* Only this existing word view is needed: preserve the other game-state
 * fields and set, rather than replace, the selection-specific flag. */
typedef struct {
    char pad_000[0x10];
    s32 selectionFlags;
} SceneSelectGameFlagsView;

void Ovl08DrawSceneSelectMenu(void) {
    SomeStruct *input;
    s32 page;
    s32 row;

    KanjiFntPrint(&gOvl08SceneSelectHeading);
    KanjiFntPrint(&gOvl08SceneSelectSubheading);

    page = gOvl08SceneSelectCursor / 6;
    row = page * 6;

    while (row < (gOvl08SceneSelectCursor / 6) * 6 + 6) {
        if ((u32)row < 0x20U) {
            KanjiFntPrint(&gOvl08SceneSelectRowFormat,
                          (row == gOvl08SceneSelectCursor) ? &gOvl08SceneSelectCursorMarker : &gOvl08SceneSelectBlankMarker, row,
                          gOvl08SceneSelectEntries[row].labelAddress);
        } else {
            KanjiFntPrint(&gOvl08SceneSelectEmptyRow);
        }
        row += 1;
    }

    KanjiFntPrint(&gOvl08SceneSelectInstructions);

    if (IsScreenFadeComplete() != 0) {
        input = (SomeStruct *)gPad0Input;
        if (input->field_0x0 & 0x1000) {
            gOvl08SceneSelectCursor -= 1;
            if (gOvl08SceneSelectCursor < 0) {
                gOvl08SceneSelectCursor = 0x1F;
            }
        } else if (input->field_0x0 & 0x4000) {
            gOvl08SceneSelectCursor += 1;
            if ((u32)gOvl08SceneSelectCursor >= 0x20U) {
                gOvl08SceneSelectCursor = 0;
            }
        } else if (input->field_0x0 & 0x8000) {
            gOvl08SceneSelectCursor -= 6;
            if (gOvl08SceneSelectCursor < 0) {
                gOvl08SceneSelectCursor = 0;
            }
        } else if (input->field_0x0 & 0x2000) {
            gOvl08SceneSelectCursor += 6;
            if ((u32)gOvl08SceneSelectCursor >= 0x20U) {
                gOvl08SceneSelectCursor = 0x1F;
            }
        } else if (input->field_0x8 & 0x20) {
            s32 sceneId = gOvl08SceneSelectEntries[gOvl08SceneSelectCursor].sceneId;
            if (sceneId != -1) {
                RequestSceneChange(sceneId);
                switch (gOvl08SceneSelectCursor) {
                case 13:
                    ((SceneSelectGameFlagsView *)&D_8006C838)->selectionFlags |= 0x80000;
                    break;
                case 14:
                    ((SceneSelectGameFlagsView *)&D_8006C838)->selectionFlags |= 0x40000;
                    break;
                case 15:
                    ((SceneSelectGameFlagsView *)&D_8006C838)->selectionFlags |= 0x20000;
                    break;
                case 16:
                    ((SceneSelectGameFlagsView *)&D_8006C838)->selectionFlags |= 0x10000;
                    break;
                }
            }
        }
        KanjiFntFlush(-1);
    }
}
