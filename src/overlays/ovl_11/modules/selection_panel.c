/*
 * selection_panel — reconstructed translation unit.
 *
 * Selection panel lifecycle with private phase/lane state, directional lane
 * links, seasonal sprite resources and VRAM save/restore. The two external
 * frame/input entries are documented in selection_panel.h. The seven helpers
 * remain externally linked; grouping them does not establish static linkage.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "selection_panel.h"
#include "map_texture_camera.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "screen_fade.h"
#include "sound_commands.h"
#include "message_window.h"
#include "sprite_render.h"
#include "beveled_panel.h"
#include "cd_loader.h"
#include "sprite_source.h"
#include "sprite_upload.h"
#include "ovl11_menu_exit_flags.h"
#include "ui_sprites.h"

void Ovl11ResetSelectionPanelState(void) {
    gOvl11SelectionPanelState = 0;
    gOvl11SelectionPanelLane = 0;
}

void Ovl11OpenSelectionPanel(void) {
    Ovl11ResetSelectionPanelState();
    gOvl11SelectionPanelState = 1;
}

u32 func_80017A64(void);
void func_80017A48(u32 arg0);
void Ovl11ReloadSelectionPanelSprites(void);
void Ovl11ProcessSelectionPanelInput(void);

void Ovl11UpdateSelectionPanel(void) {
    u32 savedTextMode;

    if (gOvl11SelectionPanelState == 1) {
        BeginScreenFadeIncrease(10, 0, 2);
        gOvl11SelectionPanelState = 2;
    } else if (gOvl11SelectionPanelState == 2) {
        if (IsScreenFadeComplete() == 1) {
            gOvl11SelectionPanelState = 3;
            ((struct struct_8006C838_view *)D_8006C838)->field_0C |= 0x20000;
        }
    } else if (gOvl11SelectionPanelState == 3) {
        if (D_8006C844 & 0x800000) {
            StoreImage((RECT *)gOvl11SelectionPanelSaveRect, (u_long *)(gGraphicsHeapBase + 0x12A90));
            DrawSync(0);
            gOvl11SelectionPanelState = 4;
        }
    } else if (gOvl11SelectionPanelState == 4) {
        Ovl11ReloadSelectionPanelSprites();
        gOvl11SelectionPanelState = 5;
        BeginScreenFadeDecrease(10, 0, 2);
    } else if (gOvl11SelectionPanelState == 5) {
        DrawMenuPanel((u32 *)(gActiveRenderContext->field_D8 + 0x68), 1, 0, 0, 320, 176);
        DrawSpriteAnimationQuadsAndUpdate(gActiveRenderContext->field_D8 + 0x64, (SpriteSourceData *)gOvl11SelectionPanelSprites, (s16)(HWD0 / 2), (s16)(VWD0 / 2));
        if (IsScreenFadeComplete() == 1) {
            gOvl11SelectionPanelState = 6;
        }
    } else if (gOvl11SelectionPanelState == 8) {
        DrawMenuPanel((u32 *)(gActiveRenderContext->field_D8 + 0x68), 1, 0, 0, 320, 176);
        DrawSpriteAnimationQuadsAndUpdate(gActiveRenderContext->field_D8 + 0x64, (SpriteSourceData *)gOvl11SelectionPanelSprites, (s16)(HWD0 / 2), (s16)(VWD0 / 2));
        if (GetMessageWindowStatus() == 2) {
            BeginScreenFadeIncrease(10, 0, 2);
            gOvl11SelectionPanelState = 9;
        }
    } else if (gOvl11SelectionPanelState == 9) {
        DrawMenuPanel((u32 *)(gActiveRenderContext->field_D8 + 0x68), 1, 0, 0, 320, 176);
        DrawSpriteAnimationQuadsAndUpdate(gActiveRenderContext->field_D8 + 0x64, (SpriteSourceData *)gOvl11SelectionPanelSprites, (s16)(HWD0 / 2), (s16)(VWD0 / 2));
        if (IsScreenFadeComplete() == 1) {
            LoadImage((RECT *)gOvl11SelectionPanelSaveRect, (u_long *)(gGraphicsHeapBase + 0x12A90));
            DrawSync(0);
            ((struct struct_8006C838_view *)D_8006C838)->field_0C |= 0x40000;
            BeginScreenFadeDecrease(10, 0, 2);
            gOvl11SelectionPanelState = 10;
        }
    } else if (gOvl11SelectionPanelState == 10) {
        if (IsScreenFadeComplete() == 1) {
            Ovl11PostMenuExitFlags();
            gOvl11SelectionPanelState = 0;
        }
    } else if (gOvl11SelectionPanelState == 11) {
        DrawMenuPanel((u32 *)(gActiveRenderContext->field_D8 + 0x68), 1, 0, 0, 320, 176);
        DrawSpriteAnimationQuadsAndUpdate(gActiveRenderContext->field_D8 + 0x64, (SpriteSourceData *)gOvl11SelectionPanelSprites, (s16)(HWD0 / 2), (s16)(VWD0 / 2));
        if (IsScreenFadeComplete() == 1) {
            gOvl11SelectionPanelState = 12;
            Ovl11EnterMapCameraMode();
        }
    } else if (gOvl11SelectionPanelState == 12) {
        Ovl11ReloadSelectionPanelSprites();
        gOvl11SelectionPanelState = 5;
    } else if (gOvl11SelectionPanelState != 0) {
        savedTextMode = func_80017A64();
        func_80017A48(3);
        Ovl11ProcessSelectionPanelInput();
        func_80017A48(savedTextMode);
    }
}

void func_80017A38(s16 arg0, s16 arg1);
void Ovl11DrawSelectionPanel(void);
void SetVal8005E2BC(s32 arg0);
void Ovl11ConfirmSelectionPanel(void);

void Ovl11ProcessSelectionPanelInput(void) {
    s32 direction;
    u32 buttons;

    func_80017A38(0, 2);
    direction = 0;
    if (gOvl11SelectionPanelState != 7 && IsMessageWindowStatusFinal()) {
        buttons = ((u32 *)gPad0Input)[0] & 0xF000;
        if (buttons) {
            s32 offset;
            u8 *entry;

            switch (buttons) {
            case 0x1000:
                direction = 0;
                break;
            case 0x4000:
                direction = 1;
                break;
            case 0x8000:
                direction = 2;
                break;
            case 0x2000:
                direction = 3;
                break;
            }
            offset = direction + gOvl11SelectionPanelLane * 8;
            entry = gOvl11SelectionPanelLanes;
            /* Preserve the original base/index order of the address add. */
            entry -= -offset;
            gOvl11SelectionPanelLane = entry[4];
            PlaySoundEffect(5);
        }
    }
    Ovl11DrawSelectionPanel();
    if (gOvl11SelectionPanelState == 7) {
        if (GetMessageWindowStatus() == 2) {
            gOvl11SelectionPanelState = 6;
            CloseMessageWindow();
        }
    } else {
        SetMessagePortraitEnabled(0);
        SetVal8005E2BC(0);
        if (((struct Ovl11D2764StateView *)&D_8007AFF0)->field_25476 == 1) {
            RequestMessageWindow(3, 0x3AB);
        } else {
            RequestMessageWindow(3, 0x3B1);
        }
    }
    if (gOvl11SelectionPanelState != 7 && IsMessageWindowStatusFinal()) {
        u32 pressed;

        pressed = ((u32 *)gPad0Input)[2];
        if (pressed & 0x120) {
            if (IsMessageWindowStatusFinal()) {
                Ovl11ConfirmSelectionPanel();
            }
        } else if (pressed & 0x50) {
            PlaySoundEffect(0);
            gOvl11SelectionPanelState = 7;
            CloseMessageWindow();
            RequestMessageWindow(3, gOvl11SelectionPanelLane + 0x385);
        } else if (pressed & 0x800) {
            if (((struct Ovl11D2764StateView *)&D_8007AFF0)->field_25476 == 1) {
                PlaySoundEffect(0);
                BeginScreenFadeIncrease(10, 0, 2);
                gOvl11SelectionPanelState = 11;
            } else {
                PlaySoundEffect(2);
            }
        }
    }
}

void Ovl11ConfirmSelectionPanel(void) {
    CloseMessageWindow();
    PlaySoundEffect(3);
    gOvl11SelectionPanelState = 8;
}

void Ovl11LoadSelectionPanelSprites(s16 arg0);

void Ovl11ReloadSelectionPanelSprites(void) {
    PlaySoundEffect(3);
    Ovl11ResetSelectionPanelState();
    gOvl11SelectionPanelState = 6;
    Ovl11LoadSelectionPanelSprites(gCalendarSeason);
}

s32 Ovl11FindSelectionLaneSlot(s16 arg0);

void Ovl11DrawSelectionPanel(void) {
    s32 idx;
    u8 *gs;

    DrawMenuPanel((u32 *)(gActiveRenderContext->field_D8 + 0x68), 1, 0, 0, 0x140, 0xB0);
    DrawSpriteAnimationQuadsAndUpdate(gActiveRenderContext->field_D8 + 0x64, (SpriteSourceData *)&gOvl11SelectionPanelSprites[0], (s16)(HWD0 / 2), (s16)(VWD0 / 2));
    gs = (u8 *)D_8006C838;
    idx = Ovl11FindSelectionLaneSlot(*(s16 *)(gs + 0x52C6));
    DrawSpriteAnimationQuadsAndUpdate(gActiveRenderContext->field_D8 + 0x5C, (SpriteSourceData *)&gOvl11SelectionPanelSprites[1],
                  (s16)(*(u16 *)(gOvl11SelectionPanelLanes + idx * 8) - 0x10),
                  *(s16 *)(gOvl11SelectionPanelLanes + 2 + idx * 8));
    idx = Ovl11FindSelectionLaneSlot(*(s16 *)(gs + 0x91DC));
    DrawSpriteAnimationQuadsAndUpdate(gActiveRenderContext->field_D8 + 0x5C, (SpriteSourceData *)&gOvl11SelectionPanelSprites[2],
                  (s16)(*(u16 *)(gOvl11SelectionPanelLanes + idx * 8) + 0x10),
                  *(s16 *)(gOvl11SelectionPanelLanes + 2 + idx * 8));
    DrawUiSprite0BFrame1(gActiveRenderContext->field_D8 + 0x58,
                  *(s16 *)(gOvl11SelectionPanelLanes + gOvl11SelectionPanelLane * 8),
                  *(s16 *)(gOvl11SelectionPanelLanes + 2 + gOvl11SelectionPanelLane * 8));
}

s32 Ovl11FindSelectionLaneSlot(s16 id) {
    s32 byteOffset;

    for (byteOffset = 0; gOvl11SelectionLaneSlots[byteOffset] != 0xFF; byteOffset += 2) {
        if (id == gOvl11SelectionLaneSlots[byteOffset]) {
            return gOvl11SelectionLaneSlots[byteOffset + 1];
        }
    }
    return -1;
}

void Ovl11LoadSelectionPanelSprites(s16 season) {
    s32 archiveOffset;
    struct Ovl11SelectionPanelSpriteStorage *thirdSprite;
    struct Ovl11SelectionPanelSpriteStorage *secondSprite;

    DrawSync(0);
    ClearOTagR((u_long *)gActiveRenderContext->field_120, 0x800);
    season = season < 0 ? 0 : (season > 3 ? 3 : season);
    archiveOffset = (season * 0xE800) + 0x03D17000;
    CdReadArchiveBytes(0, archiveOffset, 0xE800, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 1);
    do {
    } while (CdReadArchiveBytes(0, archiveOffset, 0xE800, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 0) == 0);
    UploadSpriteImages((u8 *)(gGraphicsHeapBase + 0x4690));
    gOvl11SelectionPanelSpriteBlock = *(Ovl11SelectionPanelSpriteBlock *)(gGraphicsHeapBase + 0x4290);
    InitSpriteSourceData((SpriteSourceData *)gOvl11SelectionPanelSprites, (SpriteDataHeader *)&gOvl11SelectionPanelSpriteBlock);
    secondSprite = gOvl11SelectionPanelSprites + 1;
    InitSpriteSourceData((SpriteSourceData *)secondSprite, (SpriteDataHeader *)&gOvl11SelectionPanelSpriteBlock);
    thirdSprite = gOvl11SelectionPanelSprites + 2;
    InitSpriteSourceData((SpriteSourceData *)thirdSprite, (SpriteDataHeader *)&gOvl11SelectionPanelSpriteBlock);
    SelectSpriteAnimation((ObjectState *)gOvl11SelectionPanelSprites, 0);
    SelectSpriteAnimation((ObjectState *)secondSprite, 1);
    SelectSpriteAnimation((ObjectState *)thirdSprite, 2);
    SelectSpriteAnimationFrame((ObjectState *)thirdSprite, 1);
}
