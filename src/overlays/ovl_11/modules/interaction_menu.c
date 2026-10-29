/*
 * Interaction-menu lifecycle and item-slot display, ovl_11:800F6680–800F9D3C.
 *
 * Starts reset the private grid, indicator and texture-stream state. The
 * frame service waits for rotation/messages, fills the texture cache, then
 * drives slot selection, record moves and cache/near-pane texture exchange.
 * Cache activity marks protect this frame's draws from eviction; they are
 * distinct from occupancy and from the eighteen displayed near-pane IDs.
 *
 * The sequence, record-slot, texture-stream/cache and slot-decoration headers
 * publish separate caller-facing operations of this one non-reentrant unit.
 * CD/message-buffer, graphics-heap scratch, VRAM and packet storage must be
 * initialized by the caller. No storage allocation or transactional rollback
 * is provided. Membership and outer-boundary evidence: notes/file-groupings.md.
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
#include "ovl11_menu_exit_flags.h"
#include "cd_loader.h"
#include "integer_power.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "packet_work.h"
#include "beveled_panel.h"
#include "psyq/libgpu.h"
#include "psyq/libsnd.h"
#include "psyq/libgs.h"
#include "psyq/libapi.h"
#include "ovl11_slot_sprite_cache.h"
#include "psyq/abs.h"
#include "ovl11_cell_grid_lookup.h"
#include "sprite_source.h"
#include "sprite_upload.h"
#include "sprite_render.h"
#include "ovl11_item_table_queries.h"
#include "ovl11_item_stack_clear.h"
#include "ovl11_slot_decorations.h"
#include "ui_sprites.h"
#include "text_engine.h"

/* In-unit helpers retain external linkage and the original ABI. Only the
 * sequence, cache, streaming and decoration services have outside callers. */
void Ovl11UpdateMenuGridScreen(void);
s16 *Ovl11GetItemSlotRecord(s32 slot, s32 page);
s32 Ovl11GetItemSlotLevel(s32 slot);
void Ovl11ReorderSelectionTextures(u16 *firstId, s32 firstPane,
                                  u16 *secondId, s32 secondPane,
                                  s32 releaseOutgoing);
void Ovl11ReleaseUnusedSelectionTextures(void);
void Ovl11DrawTextureSpriteInCell(s32 *ot, s16 textureIndex, s16 x, s16 y);
void Ovl11DrawScaledTextureQuad(s32 *ot, s16 textureIndex, s16 centreX,
                              s16 centreY, s32 scale4096);

void Ovl11ResetInteractionSequence(void) {
    gOvl11InteractionPhase = 0xFF;
    gOvl11MenuGridCursorSlot = 0;
    gOvl11MenuGridSavedCursorSlot = 0;
    gOvl11MenuGridSelectedSlot = 0xFF;
    gOvl11MenuGridPageIndex = 0;
    gOvl11MenuGridSelectedPageIndex = 0;
    gOvl11MenuGridMessageState = 0xFF;
    gOvl11MenuGridIndicatorFrame = 0;
    gOvl11MenuGridIndicatorTicks = 0;
    gOvl11TextureStreamCursor = 0;
    Ovl11ResetTextureCellCache();
}

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

void Ovl11StartMessageInteraction(void) {
    Ovl11ResetInteractionSequence();
    ((struct struct_8006C838_view *)&D_8006C838)->field_0C |= 0x20000;
    PlaySoundEffect(3);
    gOvl11InteractionPhase = 4;
    SetMessagePortraitEnabled(0);
    SetTextInputEnabled(0);
    RequestMessageWindow(3, 0x37A);
}

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
                SetTextInputEnabled(0);
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
                    Ovl11PostMenuExitFlags();
                    ((struct struct_8006C838_view *)&D_8006C838)->field_0C |= 0x40000;
                    ((struct struct_8006C838_800E8F78 *)&D_8006C838)->field_5234 |= 0x2000;
                    SetGameFlag(0x71, 0);
                    SetGameFlag(0x70, 0);
                }
            } else {
                Ovl11ClearTextureCellActivity();
                savedPalette = GetTextPalette();
                SetTextPalette(3);
                Ovl11UpdateMenuGridScreen();
                SetTextPalette(savedPalette);
            }
        }
    }
}

/*
 * Synchronous single-ID path of the shared texture cache. Activity is a
 * per-pass retention mark: misses discard the highest-index inactive ID,
 * then fill the lowest-index free slot. The batch streamer and packet
 * builders below operate on these same residency records.
 * Membership and boundaries: notes/file-groupings.md.
 */
/* Clear occupancy and activity without erasing texture geometry. */
void Ovl11ResetTextureCellCache(void) {
    s32 i;

    for (i = 0; i <= 0x47; i++) {
        gOvl11TextureCache[i].active = 0;
        gOvl11TextureCache[i].id = 0;
    }
}

void Ovl11ClearTextureCellActivity(void) {
    s32 i;

    i = 0x47;
    do {
        gOvl11TextureCache[i].active = 0;
        i--;
    } while (i >= 0);
}

/* Uses the last piece of animation 0's frame. Unwritten geometry and activity
 * remain stale, unlike the batch installer's whole-record clear. */
void Ovl11LoadTextureCellRecord(u16 textureId) {
    SpriteSourceData sprite;
    SpriteSourceData *source;
    s32 i;
    s32 id;
    s32 baseX;
    s32 lastPieceOffset;
    s16 uploadY;
    s32 row;
    SpriteTableRef *frame;
    SpriteFramePiece *piece;
    SpriteTextureRect *texture;

    id = textureId & 0xFFFF;
    for (i = 0; i < 0x48; i++) {
        if (gOvl11TextureCache[i].id == 0) {
            break;
        }
    }
    if (i != 0x48) {
        CdReadDiscSectorsBlocking(0, (id << 11) + 0x03C19000, 0x800, 0, (s32)&gMessageWindowReadBuffer);
        source = &sprite;
        InitSpriteSourceData(source, (SpriteDataHeader *)&gMessageWindowReadBuffer);
        frame = (SpriteTableRef *)source->field_20 + *(u16 *)(source->field_2C + ((SpriteTableRef *)source->field_28)->offset);
        lastPieceOffset = frame->count * 12 - 12;
        piece = (SpriteFramePiece *)(source->field_24 + frame->offset);
        piece = (SpriteFramePiece *)((u8 *)piece + lastPieceOffset);
        texture = (SpriteTextureRect *)source->field_1C + piece->index;
        gOvl11TextureCache[i].id = id;
        gOvl11TextureCache[i].width = texture->width * 4;
        gOvl11TextureCache[i].height = texture->height;
        gOvl11TextureCache[i].x = (i % 8) * 8 + (baseX = (i / 24) * 64 + 0x140);
        gOvl11TextureCache[i].y = ((i % 24) / 8) * 64;
        gOvl11TextureCache[i].clutX = (i % 4) * 16 + baseX;
        gOvl11TextureCache[i].clutY = (row = (i % 24) / 4) + 0xC0;
        uploadY = row - 0x3E;
        UploadSpriteImagesAtOffsets((u8 *)&gMessageWindowReadBuffer + 0x100,
            (s16)((u16)gOvl11TextureCache[i].x - 0x140),
            gOvl11TextureCache[i].y,
            (s16)(gOvl11TextureCache[i].clutX - 0x140),
            uploadY);
    }
}

/* Hit-only retry path: ID zero submits the separate shared-sprite fallback. */
void Ovl11DrawCachedTextureCell(s32 *ot, u16 textureId, s16 x, s16 y) {
    s32 i;
    s32 id;

    id = textureId & 0xFFFF;
    if (id == 0) {
        Ovl11DrawEmptySlot((s32) (ot + 1), x, y);
        return;
    }
    for (i = 0; i < 0x48; i++) {
        if (gOvl11TextureCache[i].id == id) {
            gOvl11TextureCache[i].active = 1;
            Ovl11DrawTextureSpriteInCell(ot, i, x, y);
            return;
        }
    }
}

void Ovl11DrawTextureCell(s32 *ot, u16 textureId, s16 x, s16 y) {
    s32 i;
    u16 id;

    id = textureId & 0xFFFF;
    if (id == 0) {
        Ovl11DrawEmptySlot((s32)(ot + 1), x, y);
        return;
    }
    for (i = 0; i < 0x48; i++) {
        if (gOvl11TextureCache[i].id == id) {
            gOvl11TextureCache[i].active = 1;
            Ovl11DrawTextureSpriteInCell(ot, i, x, y);
            return;
        }
    }
    for (i = 0x47; i >= 0; i--) {
        if (gOvl11TextureCache[i].active == 0) {
            gOvl11TextureCache[i].id = 0;
            Ovl11LoadTextureCellRecord(id);
            Ovl11DrawCachedTextureCell(ot, id, x, y);
            return;
        }
    }
}

/* Retained original quad variant (no external retail caller). ID zero draws
 * nothing. Scale is divided by 4096 before multiplying the cached dimensions. */
void Ovl11DrawScaledTextureCell(s32 *ot, u16 textureId, s16 x, s16 y, s32 scale4096) {
    s32 i;
    u16 id;

    id = textureId & 0xFFFF;
    if (id != 0) {
        for (i = 0; i < 0x48; i++) {
            if (gOvl11TextureCache[i].id == id) {
                gOvl11TextureCache[i].active = 1;
                Ovl11DrawScaledTextureQuad(ot, i, x, y, scale4096);
                return;
            }
        }
        for (i = 0x47; i >= 0; i--) {
            if (gOvl11TextureCache[i].active == 0) {
                gOvl11TextureCache[i].id = 0;
                Ovl11LoadTextureCellRecord(id);
                Ovl11DrawScaledTextureCell(ot, id, x, y, scale4096);
                return;
            }
        }
    }
}

/* Menu-grid decorations, ovl_11:800F6F40–800F71DC.
 * These services reuse the resident shared sprite source: selected-cell
 * marker, empty-cell fallback, quantity label, and a cell-relative cursor.
 * They submit packets but do not advance selection or alter item records.
 * Membership and evidence-limited boundaries: notes/file-groupings.md. */
void Ovl11DrawSlotCursor(s32 orderingTable, s16 x, s16 y) {
    DrawUiSprite0BFrame1(orderingTable, (s16)(x + 12), (s16)(y + 24));
}

void Ovl11DrawSelectedSlotMarker(s32 orderingTable, s16 x, s16 y) {
    SelectSpriteAnimation((ObjectState *)&gSharedSpriteSource, 0xB);
    SelectSpriteAnimationFrame((ObjectState *)&gSharedSpriteSource, 0);
    DrawSpriteFrameQuads(orderingTable, (s32)&gSharedSpriteSource,
        gSharedSpriteSource.field_4, gSharedSpriteSource.field_5,
        x, y, 0x1000, 0, 0);
}

void Ovl11DrawEmptySlot(s32 orderingTable, s16 x, s16 y) {
    SelectSpriteAnimation((ObjectState *)&gSharedSpriteSource, 0xB);
    SelectSpriteAnimationFrame((ObjectState *)&gSharedSpriteSource, 2);
    DrawSpriteFrameRects(orderingTable, (s32)&gSharedSpriteSource,
        gSharedSpriteSource.field_4, gSharedSpriteSource.field_5, x, y);
}

void Ovl11DrawSlotQuantity(s32 orderingTable, s16 x, s16 y, s32 quantity) {
    s32 tens;

    SelectSpriteAnimation((ObjectState *)&gSharedSpriteSource, 0xA);
    x += 2;
    y += 0x16;
    SelectSpriteAnimationFrame((ObjectState *)&gSharedSpriteSource, 0xA);
    DrawSpriteFrameRects(orderingTable, (s32)&gSharedSpriteSource,
        gSharedSpriteSource.field_4, gSharedSpriteSource.field_5, x, y);

    /* Always advance over the tens position, even when it is blank. */
    x += 0xA;
    tens = quantity / 10;
    if (tens != 0) {
        SelectSpriteAnimationFrame((ObjectState *)&gSharedSpriteSource, tens & 0xFF);
        DrawSpriteFrameRects(orderingTable, (s32)&gSharedSpriteSource,
            gSharedSpriteSource.field_4, gSharedSpriteSource.field_5, x, y);
    }

    x += 0xA;
    SelectSpriteAnimationFrame((ObjectState *)&gSharedSpriteSource, (quantity % 10) & 0xFF);
    DrawSpriteFrameRects(orderingTable, (s32)&gSharedSpriteSource,
        gSharedSpriteSource.field_4, gSharedSpriteSource.field_5, x, y);
}

/*
 * Batch sprite-texture streaming into the shared 72-slot texture cache.
 *
 * Each CD read covers 80 consecutive 0x800-byte sprite records. Installation
 * selects IDs from the work-area request lists, resolves up to two frames per
 * sprite, and uploads their image/CLUT data to the cache slot's VRAM region.
 * The halfword cursor encodes both the next batch ID and read/poll phase.
 * See ovl11_texture_stream.h for the public service contract.
 */

/* Internal helpers retain their existing linkage and ABI. The start result is
 * the loader's buffer pointer returned as a word; the stream ignores it.
 * Preserve the low-halfword sign extension before scaling the ID. */
s32 Ovl11StartTextureBatchRead(s32 firstId) {
    return CdReadArchiveBytes(0, (firstId << 0x10 >> 5) + 0x3C19000,
        0x28000, gGraphicsHeapBase + 0x4290, 1, 1);
}

/* 0 = installed, 1 = read still pending, -1 = no free cache record.
 * There is no rollback on exhaustion: earlier records stay installed.
 *
 * Keep the empty loop between source initialization and the second-frame
 * temporaries, indexed request traversal, and frames-before-anim declaration
 * order. Together they preserve the call-boundary lifetimes and spill layout
 * of this loop under the retail compiler. */
s32 Ovl11InstallTextureBatch(s16 firstId, s16 requestMode) {
    SpriteSourceData sprite;
    s32 first;
    s32 count;
    SpriteSourceData *src;
    s32 mode;
    s32 requestIndex;
    s32 slot;
    s32 row;
    s16 uploadY;
    u16 id;
    SpriteFramePiece *entries;
    SpriteTableRef *ref;
    u16 *frames;
    SpriteTableRef *anim;
    u16 *frame;
    SpriteFramePiece *piece;
    SpriteFramePiece *piece2;
    SpriteTextureRect *tex;
    SpriteTextureRect *tex2;

    first = firstId;
    if (CdReadArchiveBytes(0, (firstId << 11) + 0x03C19000, 0x28000,
            (u8 *)gGraphicsHeapBase + 0x4290, 1, 0) != 0) {
        if (requestMode >= 0) {
            mode = 3;
            if ((u16)requestMode < 4U) {
                mode = requestMode;
            }
            goto selected;
failed:
            return -1;
        } else {
            mode = 0;
        }
selected:
        count = gOvl11TextureRequestCounts[mode];
        for (requestIndex = 0; requestIndex < count; requestIndex++) {
            switch (mode) {
            case 0:
                id = ((Ovl11TextureRequests *)D_8006C838)->lists[1][requestIndex][0];
                break;
            case 1:
                if (requestIndex < 0x40) {
                    id = ((Ovl11TextureRequests *)D_8006C838)->lists[0][requestIndex][0];
                } else {
                    id = gOvl11ExtraTextureRequests[requestIndex - 0x40];
                }
                break;
            default:
                id = ((Ovl11TextureRequests *)D_8006C838)->lists[2][requestIndex][0];
                break;
            }
            if (id == 0 || id < first || id >= first + 0x50) {
                continue;
            }
            tex = NULL;
            piece = NULL;
            InitSpriteSourceData(&sprite, (SpriteDataHeader *)((u8 *)gGraphicsHeapBase
                + (((id - first) << 11) + 0x4290)));
            do { } while (0);
            src = &sprite;
            tex2 = NULL;
            piece2 = NULL;
            anim = (SpriteTableRef *)src->field_28;
            frames = (u16 *)src->field_2C;
            frame = (u16 *)((u8 *)frames + anim[0].offset);
            if (*frame <= 0xFFFD) {
                ref = (SpriteTableRef *)src->field_20 + *frame;
                entries = (SpriteFramePiece *)(src->field_24 + ref->offset);
                piece = &entries[ref->count - 1];
                tex = (SpriteTextureRect *)src->field_1C + piece->index;
            }
            frame = (u16 *)((u8 *)frames + anim[1].offset);
            if (*frame <= 0xFFFD) {
                ref = (SpriteTableRef *)src->field_20 + *frame;
                entries = (SpriteFramePiece *)(src->field_24 + ref->offset);
                piece2 = &entries[ref->count - 1];
                tex2 = (SpriteTextureRect *)src->field_1C + piece2->index;
            }
            for (slot = 0; slot < 0x48; slot++) {
                if (gOvl11TextureCache[slot].id == 0) {
                    break;
                }
            }
            if (slot == 0x48) {
                goto failed;
            }
            memset(&gOvl11TextureCache[slot], 0, 0x22);
            gOvl11TextureCache[slot].id = id;
            gOvl11TextureCache[slot].x = (slot % 8) * 8 + (s16)((slot / 24) * 64 + 0x140);
            gOvl11TextureCache[slot].y = ((slot % 24) / 8) * 64;
            if (tex != NULL) {
                gOvl11TextureCache[slot].width = tex->width * 4;
                gOvl11TextureCache[slot].height = tex->height;
                gOvl11TextureCache[slot].pieceX = piece->rest[1];
                gOvl11TextureCache[slot].pieceY = piece->rest[2];
            }
            if (piece2 != NULL) {
                gOvl11TextureCache[slot].x2 = (slot % 8) * 8 + (s16)((slot / 24) * 64 + 0x140);
                gOvl11TextureCache[slot].y2 = ((slot % 24) / 8) * 64 + 0x20;
            }
            if (tex2 != NULL) {
                gOvl11TextureCache[slot].width2 = tex2->width * 4;
                gOvl11TextureCache[slot].height2 = tex2->height;
                gOvl11TextureCache[slot].pieceX2 = piece2->rest[1];
                gOvl11TextureCache[slot].pieceY2 = piece2->rest[2];
            }
            gOvl11TextureCache[slot].clutX = (slot % 4) * 16 + ((slot / 24) * 64 + 0x140);
            gOvl11TextureCache[slot].clutY = (row = (slot % 24) / 4) + 0xC0;
            uploadY = row - 0x3E;
            UploadSpriteImagesAtOffsets((u8 *)gGraphicsHeapBase + (((id - first) << 11) + 0x4390),
                (s16)((u16)gOvl11TextureCache[slot].x - 0x140),
                gOvl11TextureCache[slot].y,
                (s16)(gOvl11TextureCache[slot].clutX - 0x140), uploadY);
        }
        return 0;
    }
    return 1;
}

s32 Ovl11ServiceTextureStream(s16 resetOrStep) {
    s32 stepResult;

    if (resetOrStep == -1) {
        gOvl11TextureStreamCursor = 0;
        return 0;
    } else {
        stepResult = Ovl11StepTextureStream(1);
        return ((s16)stepResult);
    }
}

s32 Ovl11StepTextureStream(s32 requestMode) {
    s32 result;

    /* Below 0x400 the cursor is a first ID; adding 0x400 marks an active
     * read. A successful install removes that marker and advances by 80. */
    if (gOvl11TextureStreamCursor < 0x400) {
        Ovl11StartTextureBatchRead(gOvl11TextureStreamCursor);
        gOvl11TextureStreamCursor += 0x400;
        return 1;
    }
    if (Ovl11InstallTextureBatch(gOvl11TextureStreamCursor - 0x400, requestMode) == 0) {
        gOvl11TextureStreamCursor -= 0x3B0;
        result = gOvl11TextureStreamCursor < 0x13F;
    } else {
        result = 1;
    }
    return result;
}

/*
 * Phase-selected item-slot screen, ovl_11:800F787C-800F8CF8.
 *
 * The frame driver owns the input -> record operation -> navigation/message
 * sequence and the ordering-table layers used by its seven view helpers.
 * Slot 0 is the selected record, 1..8 the snapshot row, and 9..16 the visible
 * eight-cell page. The same layout selector controls the snapshot count and
 * the navigation links. Records are three halfwords: ID, attribute, quantity.
 * Texture ownership changes before moves/swaps; stack merges leave quantities
 * above 99 in the source. The layout index must be 0..2 and the slot/text/
 * sprite resources initialized; the frame service supplies those lifetimes.
 *
 * All view callers are inside this unit; helpers retain external linkage.
 * Membership and evidence-limited outer boundaries: notes/file-groupings.md.
 */

s32 Ovl11HandleMenuGridInput(void);
void Ovl11UpdateMenuGridNavigation(s32 slotCount);
void Ovl11UpdateMenuGridMessage(s32 textDisplay, s32 panelDisplay, s32 slot, s32 page);
void Ovl11DrawMenuGridSlots(s32 *orderingTable, s32 firstSlot, s32 count, s16 *records);
void Ovl11DrawMenuGridSlot(s32 *orderingTable, s32 slot, s16 *record);
void Ovl11DrawMenuGridPageCount(s32 textDisplay, s32 slotCount);
void Ovl11DrawMenuGridPanels(u32 *panelOrderingTable, s32 textDisplay,
                           s32 topTextAddress, s32 bottomTextAddress);

void Ovl11UpdateMenuGridScreen(void) {
    s32 firstText;
    s32 secondText;
    s32 message;
    s32 items;
    s32 selected;
    s32 snapshot;
    s32 object;
    s32 text;
    u32 *panel;
    s32 *display;

    SetTextSpacing(0, 2);
    if (IsMessageWindowStatusFinal() && (gPad0Input->pressedButtons & 0x800)) {
        gOvl11InteractionPhase = 0xFE;
        CloseMessageWindow();
        PlaySoundEffect(3);
        return;
    }
    firstText = gOvl11InteractionPhase;
    if (firstText == 0) {
        firstText = 0x448;
        secondText = 0x466;
        message = 0x3AD;
        items = (s32)D_8006C838 + 0x468A;
        selected = (s32)D_8006C838 + 0x524C;
        snapshot = (s32)D_8006C838 + 0x5258;
    } else if (firstText == 2) {
        firstText = 0x454;
        secondText = 0x476;
        message = 0x3AE;
        items = (s32)D_8006C838 + 0x450A;
        selected = (s32)D_8006C838 + 0x5252;
        snapshot = (s32)D_8006C838 + 0x5288;
    } else if (firstText == 1) {
        firstText = 0x454;
        secondText = 0x484;
        message = 0x3AF;
        items = (s32)D_8006C838 + 0x480A;
        selected = (s32)D_8006C838 + 0x5252;
        snapshot = (s32)D_8006C838 + 0x5288;
    } else {
        return;
    }
    firstText = (s32)((u8 *)D_8005175C + (gTextBankOffsets[0] + firstText));
    secondText = (s32)((u8 *)D_8005175C + (gTextBankOffsets[0] + secondText));
    object = gActiveRenderContext->field_D8;
    panel = (u32 *)(object + 0x68);
    text = object + 0x64;
    display = (s32 *)(object + 0x5C);
    Ovl11DrawMenuGridPanels(panel, text, firstText, secondText);
    Ovl11DrawMenuGridPageCount(text, 0x40);
    Ovl11HandleMenuGridInput();
    Ovl11DrawMenuGridSlot(display, 0, (s16 *)selected);
    Ovl11DrawMenuGridSlots(display, 1, pow_int(2, gMenuGridLayoutIndex + 1), (s16 *)snapshot);
    Ovl11UpdateMenuGridNavigation(0x40);
    items += gOvl11MenuGridPageIndex * 48;
    Ovl11DrawMenuGridSlots(display, 9, 8, (s16 *)items);
    Ovl11UpdateMenuGridMessage(text, (s32)panel, gOvl11MenuGridCursorSlot, gOvl11MenuGridPageIndex);
    if (gOvl11MenuGridMessageState == 0xFF) {
        SetMessagePortraitEnabled(0);
        SetTextInputEnabled(0);
        RequestMessageWindow(3, message);
    }
}

s32 Ovl11ApplySlotRecordOperation(s32 sourceSlot, s32 destinationSlot,
                                s32 sourcePage, s32 destinationPage);
void Ovl11CompactItemSlots(s16 *arg0, s16 arg1);

s32 Ovl11HandleMenuGridInput(void) {
    s16 temp_s0;
    s32 temp_s0_2;
    s32 temp_v1;

    temp_s0 = gOvl11MenuGridMessageState;
    if ((temp_s0 == 0xFF) && (IsMessageWindowStatusFinal() != 0)) {
        temp_v1 = gPad0Input->pressedButtons;
        if (temp_v1 & 0x10) {
            if (*Ovl11GetItemSlotRecord((s32) gOvl11MenuGridCursorSlot, (s32) gOvl11MenuGridPageIndex) != 0) {
                CloseMessageWindow();
                PlaySoundEffect(0);
                gOvl11MenuGridMessageState = 0;
                goto block_32;
            }
            PlaySoundEffect(2);
            return 0;
        }
        if (temp_v1 & 0x20) {
            if (gOvl11MenuGridSelectedSlot == temp_s0) {
                CloseMessageWindow();
                gOvl11InteractionPhase = 0xFE;
                PlaySoundEffect(3);
                return 0;
            }
            PlaySoundEffect(1);
            gOvl11MenuGridSelectedSlot = temp_s0;
            goto block_32;
        }
        if (temp_v1 & 0x40) {
            if (gOvl11MenuGridSelectedSlot == temp_s0) {
                PlaySoundEffect(0);
                gOvl11MenuGridSelectedSlot = (s16) (u16) gOvl11MenuGridCursorSlot;
                gOvl11MenuGridSelectedPageIndex = (s16) (u16) gOvl11MenuGridPageIndex;
                goto block_32;
            }
            gOvl11MenuGridSavedCursorSlot = (u16) gOvl11MenuGridCursorSlot;
            if ((gOvl11MenuGridSelectedSlot == (s16) (u16) gOvl11MenuGridCursorSlot) && ((gOvl11MenuGridSelectedPageIndex == gOvl11MenuGridPageIndex) || (gOvl11MenuGridSelectedSlot < 9))) {
                gOvl11MenuGridCursorSlot = 0;
            }
            Ovl11ReleaseUnusedSelectionTextures();
            temp_s0_2 = Ovl11ApplySlotRecordOperation((s32) gOvl11MenuGridSelectedSlot, (s32) gOvl11MenuGridCursorSlot, (s32) gOvl11MenuGridSelectedPageIndex, (s32) gOvl11MenuGridPageIndex);
            Ovl11ReleaseUnusedSelectionTextures();
            gOvl11MenuGridCursorSlot = (s16) gOvl11MenuGridSavedCursorSlot;
            if (temp_s0_2 == 1) {
                PlaySoundEffect(0);
                gOvl11MenuGridSelectedSlot = 0xFF;
                return 1;
            }
            PlaySoundEffect(2);
            return 0;
        }
        if (temp_v1 & 0x100) {
            if (gOvl11MenuGridSelectedSlot == temp_s0) {
                PlaySoundEffect(0);
                switch (gOvl11InteractionPhase) {
                case 0:
                    Ovl11CompactItemSlots((s16 *) &gItemOverflowSlotBase, 0x40);
                    break;
                case 2:
                    Ovl11CompactItemSlots((s16 *) D_80070D42_slots, 0x40);
                    break;
                case 1:
                    Ovl11CompactItemSlots((s16 *) D_80071042, 0x40);
                    break;
                }
                goto block_32;
            }
            PlaySoundEffect(2);
            goto block_32;
        }
        goto block_32;
    }
block_32:
    return 0;
}

s32 Ovl11TakeSlotRecord(u16 *arg0, s32 arg1, u16 *arg2, s32 arg3);
s32 Ovl11SwapSlotRecords(u16 *arg0, s32 arg1, u16 *arg2, s32 arg3);
s32 Ovl11MergeSlotRecords(u16 *arg0, s32 arg1, u16 *arg2, s32 arg3);

/* Empty endpoints move a record; different IDs swap; equal IDs try to merge.
 * Selecting the same slot/page is a no-op, unlike two different empty slots. */
s32 Ovl11ApplySlotRecordOperation(s32 sourceSlot, s32 destinationSlot,
                                s32 sourcePage, s32 destinationPage) {
    s32 result;
    s16 *destination;
    s16 *source;

    if (sourceSlot != destinationSlot || sourcePage != destinationPage) {
        source = Ovl11GetItemSlotRecord(sourceSlot, sourcePage);
        destination = Ovl11GetItemSlotRecord(destinationSlot, destinationPage);
        if (*source == 0 && *destination == 0) {
            return 1;
        }
        if (*destination == 0) {
            return Ovl11TakeSlotRecord((u16 *)source, sourceSlot, (u16 *)destination, destinationSlot);
        }
        if (*source == 0) {
            return Ovl11TakeSlotRecord((u16 *)destination, destinationSlot, (u16 *)source, sourceSlot);
        }
        if (*source != *destination) {
            return Ovl11SwapSlotRecords((u16 *)source, sourceSlot, (u16 *)destination, destinationSlot);
        }
        result = Ovl11MergeSlotRecords((u16 *)source, sourceSlot, (u16 *)destination, destinationSlot);
        return result;
    }
    return 0;
}

void Ovl11SwapCopyRecords(Ovl11CopyRecord *arg0, Ovl11CopyRecord *arg1);
void Ovl11CopySlotRecord(Ovl11CopyRecord *arg0, Ovl11CopyRecord *arg1);

/* Selected/snapshot destinations split one unit for type zero and move
 * whole records for types one/two. Paged destinations instead move the whole
 * record, requiring a negative status and a phase-compatible ID. */
s32 Ovl11TakeSlotRecord(u16 *source, s32 sourceSlot, u16 *destination, s32 destinationSlot) {
    s32 destinationLevel;
    s32 result;
    s8 itemType;

    destinationLevel = Ovl11GetItemSlotLevel(destinationSlot);
    if (destinationLevel != 1) {
        if (destinationLevel < 2) {
            if (destinationLevel != 0) {
                return 0;
            }
            goto move_or_split;
        }
        result = 0;
        if (destinationLevel == 2) {
            result = 0;
            if (Ovl11IsItemStatusNegative((s16) source[0]) != 0) {
                result = 0;
                if (gOvl11InteractionPhase != Ovl11ClassifyItemStatus((s16) source[0])) {
                    return 0;
                }
                Ovl11ReorderSelectionTextures(source, sourceSlot, destination, destinationSlot, 0);
                Ovl11SwapCopyRecords((Ovl11CopyRecord *) source, (Ovl11CopyRecord *) destination);
                return 1;
            }
        }
        return result;
    }
    result = 0;
    if (Ovl11IsItemStatusNegative((s16) source[0]) != 0) {
move_or_split:
        itemType = Ovl11GetItemType((s16) source[0]);
        if (itemType != 0) {
            if (itemType >= 0) {
                result = 0;
                if (itemType < 3) {
                    Ovl11ReorderSelectionTextures(source, sourceSlot, destination, destinationSlot, 0);
                    Ovl11SwapCopyRecords((Ovl11CopyRecord *) source, (Ovl11CopyRecord *) destination);
                    return 1;
                }
                return result;
            }
            result = 0;
            return result;
        }
        Ovl11ReorderSelectionTextures(source, sourceSlot, destination, destinationSlot, 0);
        Ovl11CopySlotRecord((Ovl11CopyRecord *) source, (Ovl11CopyRecord *) destination);
        (*(s16 *) ((u8 *) destination + 4)) = 1;
        result = 1;
        if (--((s16 *) source)[2] <= 0) {
            Ovl11ClearItemStackEntry((s16 *) source);
            return 1;
        }
        return result;
    }
    return result;
}

s32 Ovl11SwapSlotRecords(u16 *arg0, s32 arg1, u16 *arg2, s32 arg3) {
    switch (Ovl11GetItemSlotLevel(arg3)) {
    case 2:
        if (Ovl11IsItemStatusNegative((s16) arg0[0]) == 0) {
            return 0;
        }
        if (gOvl11InteractionPhase != Ovl11ClassifyItemStatus((s16) arg0[0])) {
            return 0;
        }
        break;
    case 1:
        if (Ovl11IsItemStatusNegative((s16) arg0[0]) == 0) {
            return 0;
        }
    case 0:
        if (Ovl11GetItemType((s16) arg0[0]) == 0 && ((s16 *) arg0)[2] >= 2) {
            return 0;
        }
        break;
    }
    switch (Ovl11GetItemSlotLevel(arg1)) {
    case 2:
        if (Ovl11IsItemStatusNegative((s16) arg2[0]) == 0) {
            return 0;
        }
        if (gOvl11InteractionPhase != Ovl11ClassifyItemStatus((s16) arg2[0])) {
            return 0;
        }
        Ovl11ReorderSelectionTextures(arg0, arg1, arg2, arg3, 1);
        Ovl11SwapCopyRecords((Ovl11CopyRecord *) arg0, (Ovl11CopyRecord *) arg2);
        return 1;
    case 1:
        if (Ovl11IsItemStatusNegative((s16) arg2[0]) == 0) {
            return 0;
        }
    case 0:
        if (Ovl11GetItemType((s16) arg2[0]) != 0 || ((s16 *) arg2)[2] < 2) {
            Ovl11ReorderSelectionTextures(arg0, arg1, arg2, arg3, 1);
            Ovl11SwapCopyRecords((Ovl11CopyRecord *) arg0, (Ovl11CopyRecord *) arg2);
            return 1;
        }
        break;
    }
    return 0;
}

void Ovl11AccumulateItemStackCount(u16 *arg0, u16 *arg1);

s32 Ovl11MergeSlotRecords(u16 *arg0, s32 arg1, u16 *arg2, s32 arg3) {
    s32 temp_v0;

    temp_v0 = Ovl11GetItemSlotLevel(arg3);
    if (temp_v0 >= 0) {
        if (temp_v0 >= 2) {
            if (temp_v0 != 2) {
                return 0;
            }
            goto block_5;
        }
        if (Ovl11GetItemType((s16) *arg0) == 2) {
block_5:
            if (Ovl11GetItemType((s16) *arg0) != 1) {
                Ovl11AccumulateItemStackCount(arg0, arg2);
                return 1;
            }
            goto block_7;
        }
        return 0;
    }
block_7:
    return 0;
}

/* Fill each hole from the next occupied slot, preserving record order.
 * Explicit backedges preserve the original's index-based scan: expressing
 * either as a C loop lets this compiler introduce a six-byte pointer walker.
 * Keep the two index terms in the inner address (rather than caching 6*i). */
void Ovl11CompactItemSlots(s16 *slots, s16 count) {
    s32 slotIndex;
    s32 scanIndex;
    s32 remaining;
    Ovl11CopyRecord *record;
    Ovl11CopyRecord *empty;

    slotIndex = 0;
    if (count > 0) {
next_slot:
        record = (Ovl11CopyRecord *)(slots + slotIndex * 3);
        if (record->field_0 == 0) {
            scanIndex = 0;
            empty = record;
            remaining = count - slotIndex;
find_source:
            if (scanIndex < remaining) {
                record = (Ovl11CopyRecord *)(slots + (slotIndex * 3) + (scanIndex * 3));
                scanIndex += 1;
                if (record->field_0 == 0) {
                    goto find_source;
                }
                *empty = *record;
                memset(record, 0, 6);
            }
        }
        slotIndex += 1;
        if (slotIndex < count) {
            goto next_slot;
        }
    }
}

/* Mutable ID/attribute/count record: slot 0 is selected, 1..8 snapshot,
 * and >=9 the eight-cell page. Indices and phase are not validated. */
s16 *Ovl11GetItemSlotRecord(s32 column, s32 row) {
    Ovl11ItemState *base;
    Ovl11ItemState *selected;
    Ovl11ItemState *start;
    s32 offset;
    s32 index;
    s32 rowOffset;

    rowOffset = row * 8 - 9;
    index = rowOffset + column;
    if (gOvl11InteractionPhase == 0) {
        base = (Ovl11ItemState *)&D_80071A84;
        offset = column << 1;
        offset += column;
        offset <<= 1;
        start = base + 1;
    } else {
        base = &Ovl11Item_80071A8A;
        offset = column << 1;
        offset += column;
        offset <<= 1;
        start = base + 8;
    }
    do {
        selected = (Ovl11ItemState *)(offset + (u32)start);
    } while (0);
    offset = Ovl11GetItemSlotLevel(column);
    switch (offset) {
    case 0:
        return (s16 *)base;
    case 1:
        return (s16 *)selected;
    default:
        switch (gOvl11InteractionPhase) {
        case 0:
            return (s16 *)((Ovl11ItemState *)&gItemOverflowSlotBase + index);
        default:
            return (s16 *)(D_80071042 + index);
        case 2:
            return (s16 *)(D_80070D42_slots[0] + index);
        }
    }
}

/* Nonnegative slots classify as selected (0), snapshot (1), or paged (2). */
s32 Ovl11GetItemSlotLevel(s32 slot) {
    if (slot == 0) {
        return 0;
    }
    if (slot < 9) {
        return 1;
    }
    return 2;
}

void Ovl11AccumulateItemStackCount(u16 *source, u16 *destination) {
    s32 sum;

    sum = destination[2] + source[2];
    destination[2] = sum;
    if ((s16)sum >= 100) {
        destination[2] = 99;
        source[2] = sum - 99;
    } else {
        Ovl11ClearItemStackEntry((s16 *)source);
    }
}

void Ovl11SwapCopyRecords(Ovl11CopyRecord *left, Ovl11CopyRecord *right) {
    Ovl11CopyRecord temp;

    temp = *left;
    *left = *right;
    *right = temp;
}

void Ovl11CopySlotRecord(Ovl11CopyRecord *source, Ovl11CopyRecord *destination) {
    *destination = *source;
}

/* Navigation and drawing share all 17 rows of the private geometry table.
 * The split halfword initialization of delta preserves the original word
 * scratch construction; page deltas are applied only in message state FF. */

void Ovl11UpdateMenuGridNavigation(s32 slotCount) {
    union { s32 value; u16 halves[2]; } delta;
    s32 previous;
    s32 buttons;
    s32 pageButtons;
    s32 direction;
    s32 state;

    delta.halves[0] = 0;
    delta.halves[1] = 0;
    if (IsMessageWindowStatusFinal() != 0) {
        buttons = gPad0Input->repeatButtons;
        if (buttons & 0xF000) {
            if (buttons & 0x1000) {
                direction = delta.value;
            } else if (buttons & 0x4000) {
                direction = 1;
            } else if (buttons & 0x8000) {
                if (gOvl11MenuGridCursorSlot == 9 || gOvl11MenuGridCursorSlot == 13) {
                    delta.value = slotCount / 8 - 1;
                }
                direction = 2;
            } else {
                if (gOvl11MenuGridCursorSlot == 12 || gOvl11MenuGridCursorSlot == 16) {
                    delta.value = 1;
                }
                direction = 3;
            }
            if (gOvl11MenuGridMessageState == 255) {
                gOvl11MenuGridCursorSlot = gOvl11MenuGridNavigation[gOvl11MenuGridCursorSlot].links[direction][gMenuGridLayoutIndex];
                PlaySoundEffect(5);
            }
        }
        if (gOvl11MenuGridMessageState == 255) {
            pageButtons = gPad0Input->repeatButtons;
            if (pageButtons & 5) {
                PlaySoundEffect(5);
                previous = delta.value - 1;
                delta.value = previous + slotCount / 8;
            } else if (pageButtons & 10) {
                PlaySoundEffect(5);
                delta.value++;
            }
        }
    }
    if (gOvl11MenuGridMessageState == 255) {
        gOvl11MenuGridPageIndex = (u16)gOvl11MenuGridPageIndex + delta.value - ((gOvl11MenuGridPageIndex + delta.value) / (slotCount / 8)) * (slotCount / 8);
    }
    if (gOvl11MenuGridSelectedSlot != 255) {
        state = Ovl11GetItemSlotLevel(gOvl11MenuGridSelectedSlot);
        if (state >= 0) {
            if (state < 2 || (state == 2 && gOvl11MenuGridPageIndex == gOvl11MenuGridSelectedPageIndex)) {
                Ovl11DrawSelectedSlotMarker(gActiveRenderContext->field_D8 + 0x54,
                    gOvl11MenuGridNavigation[gOvl11MenuGridSelectedSlot].x, gOvl11MenuGridNavigation[gOvl11MenuGridSelectedSlot].y);
            }
        }
    }
    Ovl11DrawSlotCursor(gActiveRenderContext->field_D8 + 0x50,
        gOvl11MenuGridNavigation[gOvl11MenuGridCursorSlot].x, gOvl11MenuGridNavigation[gOvl11MenuGridCursorSlot].y);
}

/* The two display arguments are retained but unused by this message path.
 * A4 records use the mapped item ID +1000, not the raw record ID. */
void Ovl11UpdateMenuGridMessage(s32 textDisplay, s32 panelDisplay, s32 slot, s32 page) {
    s16 *record;
    s32 message;

    if (gOvl11MenuGridMessageState != 255) {
        record = Ovl11GetItemSlotRecord(slot, page);
        if (record[0] == 0xA4) {
            message = Ovl11GetRecordItemId(record) + 1000;
        } else {
            message = record[0];
        }
        RequestMessageWindow(3, message);
        if (GetMessageWindowStatus() == 2) {
            gOvl11MenuGridMessageState = 255;
            CloseMessageWindow();
        }
    }
}

void Ovl11DrawMenuGridSlots(s32 *orderingTable, s32 firstSlot, s32 count, s16 *records) {
    s32 slot;

    for (slot = firstSlot; slot < firstSlot + count; slot++) {
        Ovl11DrawMenuGridSlot(orderingTable, slot, records);
        records += 3;
    }
}

void Ovl11DrawMenuGridSlot(s32 *orderingTable, s32 slot, s16 *record) {
    if (record[0] == 0) {
        Ovl11DrawEmptySlot((s32) (orderingTable + 1), gOvl11MenuGridNavigation[slot].x, gOvl11MenuGridNavigation[slot].y);
        return;
    }
    if (slot < 9) {
        Ovl11DrawSlotSprite(record[0], 0, orderingTable, gOvl11MenuGridNavigation[slot].x, gOvl11MenuGridNavigation[slot].y, 8);
    } else {
        Ovl11DrawTextureCell(orderingTable, (u16) record[0], gOvl11MenuGridNavigation[slot].x, gOvl11MenuGridNavigation[slot].y);
    }
    switch (Ovl11GetItemType(record[0])) {
    case 2:
        Ovl11DrawSlotQuantity(gActiveRenderContext->field_D8 + 0x58, gOvl11MenuGridNavigation[slot].x, gOvl11MenuGridNavigation[slot].y, record[2]);
        return;
    case 0:
        if (Ovl11GetItemSlotLevel(slot) == 2) {
            Ovl11DrawSlotQuantity(gActiveRenderContext->field_D8 + 0x58, gOvl11MenuGridNavigation[slot].x, gOvl11MenuGridNavigation[slot].y, record[2]);
        }
        /* fallthrough */
    case 1:
        return;
    }
}

/* Shared source is left on animation C after the two indicator draws. */
void Ovl11DrawMenuGridIndicators(s32 textDisplay) {
    s32 nextFrame;
    s32 wraps;

    SelectSpriteAnimation((ObjectState *) &gSharedSpriteSource, 0xD);
    SelectSpriteAnimationFrame((ObjectState *) &gSharedSpriteSource, gOvl11MenuGridIndicatorFrame);
    DrawSpriteFrameRects(textDisplay, (s32) &gSharedSpriteSource, (s32) (*(u8 *) ((u8 *) &gSharedSpriteSource + 4)), (s32) (*(u8 *) ((u8 *) &gSharedSpriteSource + 5)), 0x7D, 0x90);
    SelectSpriteAnimation((ObjectState *) &gSharedSpriteSource, 0xC);
    SelectSpriteAnimationFrame((ObjectState *) &gSharedSpriteSource, gOvl11MenuGridIndicatorFrame);
    DrawSpriteFrameRects(textDisplay, (s32) &gSharedSpriteSource, (s32) (*(u8 *) ((u8 *) &gSharedSpriteSource + 4)), (s32) (*(u8 *) ((u8 *) &gSharedSpriteSource + 5)), 0x115, 0x90);
    gOvl11MenuGridIndicatorTicks++;
    if (gOvl11MenuGridIndicatorTicks >= 9) {
        gOvl11MenuGridIndicatorTicks = 0;
        nextFrame = (s16) gOvl11MenuGridIndicatorFrame + 1;
        wraps = nextFrame / 4;
        gOvl11MenuGridIndicatorFrame = gOvl11MenuGridIndicatorFrame - (wraps * 4 - 1);
    }
}

/* One-based page / (slotCount/8), using shared FFFF-terminated digit scratch.
 * slotCount/8 must be nonzero; the screen driver supplies 64 slots. */
void Ovl11DrawMenuGridPageCount(s32 textDisplay, s32 slotCount) {
    s32 firstDisplay;
    s16 *end;

    end = gOvl11MenuGridPageText;
    end = WriteDecimalText(gOvl11MenuGridPageIndex + 1, end, 1);
    firstDisplay = textDisplay;
    *(u16 *)end = 0xFFFF;
    end = gOvl11MenuGridPageText;
    DrawTextLine(firstDisplay, (s32)end, 0x28, 0x6E);
    DrawTextLine(textDisplay, (s32)((u8 *)D_80051BF0 + gTextBankOffsets[0]), 0x35, 0x6E);
    *(u16 *)WriteDecimalText(slotCount / 8, gOvl11MenuGridPageText, 1) = 0xFFFF;
    DrawTextLine(textDisplay, (s32)gOvl11MenuGridPageText, 0x42, 0x6E);
}

void Ovl11DrawMenuGridPanels(u32 *panelOrderingTable, s32 textDisplay,
                           s32 topTextAddress, s32 bottomTextAddress) {
    DrawMenuPanel(panelOrderingTable, 1, 0x19, 0x10, 0x69, 0x4A);
    DrawMenuPanel(panelOrderingTable, 1, 0x79, 0x10, 0xAE, 0x4A);
    DrawMenuPanel(panelOrderingTable, 1, 0x19, 0x5A, 0x10E, 0x4A);
    Ovl11DrawMenuGridIndicators(textDisplay);
    DrawTextLine(textDisplay, topTextAddress, 0x1D, 0x14);
    DrawTextLine(textDisplay, bottomTextAddress, 0x1D, 0x5E);
}

/* Draw existing texture-cache records without loading or reclaiming them.
 * The sprite path commits SPRT/DR_MODE packets to packet_work; the quad path
 * advances the active context's POLY_FT4 cursor. Ownership stays external.
 * Membership/boundary evidence: notes/file-groupings.md.
 * Cache indices and packet capacity are unchecked. The sprite path centers
 * dimensions below 32 in the supplied pixel cell without cropping larger
 * ones; the quad path uses the supplied centre and integer scale multiple. */
void Ovl11DrawTextureSpriteInCell(s32 *ot, s16 textureIndex, s16 x, s16 y) {
    SPRT *packet;
    s32 textureX;
    s32 textureY;

    Ovl11DrawEmptySlot((s32) (ot + 1), x, y);
    if (gOvl11TextureCache[textureIndex].width < 32) {
        x += (32 - gOvl11TextureCache[textureIndex].width) / 2;
    }
    if (gOvl11TextureCache[textureIndex].height < 32) {
        y += (32 - gOvl11TextureCache[textureIndex].height) / 2;
    }
    packet = (SPRT *) ReservePacketWorkBytes(0);
    setSprt(packet);
    /* Keep signed CLUT coordinate loads in this SDK-function path. The
     * sibling quad uses the SDK macro and the record's unsigned fields. */
    packet->clut = GetClut(*(s16 *) &gOvl11TextureCache[textureIndex].clutX,
                          *(s16 *) &gOvl11TextureCache[textureIndex].clutY);
    setRGB0(packet, 128, 128, 128);
    setXY0(packet, x, y);
    setSemiTrans(packet, 0);
    setWH(packet, gOvl11TextureCache[textureIndex].width, gOvl11TextureCache[textureIndex].height);
    setUV0(packet, (gOvl11TextureCache[textureIndex].x - 320) * 4, gOvl11TextureCache[textureIndex].y);
    addPrim(ot, packet);
    packet++;
    setDrawTPage((DR_MODE *) packet, 1, 1,
        (textureY = gOvl11TextureCache[textureIndex].y,
         textureX = gOvl11TextureCache[textureIndex].x,
         getTPage(0, 0, textureX & 0x3C0, textureY)));
    addPrim(ot, packet);
    SetPacketWorkCursor((s32) ((DR_MODE *) packet + 1));
}

void Ovl11DrawScaledTextureQuad(s32 *ot, s16 textureIndex, s16 centreX, s16 centreY, s32 scale4096) {
    POLY_FT4 *packet;
    s16 width;
    s16 height;
    s16 left;
    s16 top;
    s32 textureX;
    s32 textureY;
    s32 page;

    width = gOvl11TextureCache[textureIndex].width;
    height = gOvl11TextureCache[textureIndex].height;
    /* Division happens BEFORE multiplication: only integer scale multiples
     * survive, unlike a usual Q12 multiply-then-shift. */
    width *= scale4096 / 4096;
    height *= scale4096 / 4096;
    packet = (POLY_FT4 *) gActiveRenderContext->field_118;
    setPolyFT4(packet);
    packet->clut = getClut(gOvl11TextureCache[textureIndex].clutX, gOvl11TextureCache[textureIndex].clutY);
    setRGB0(packet, 128, 128, 128);
    setSemiTrans(packet, 0);
    textureX = gOvl11TextureCache[textureIndex].x;
    textureY = gOvl11TextureCache[textureIndex].y;
    page = getTPage(0, 0, textureX & 0x3C0, textureY & 0x100);
    packet->tpage = page | getTPage(0, 0, 0, (u16) gOvl11TextureCache[textureIndex].y & 0x200);
    setUV4(packet,
        gOvl11TextureCache[textureIndex].x, gOvl11TextureCache[textureIndex].y,
        gOvl11TextureCache[textureIndex].x + gOvl11TextureCache[textureIndex].width, gOvl11TextureCache[textureIndex].y,
        gOvl11TextureCache[textureIndex].x, gOvl11TextureCache[textureIndex].y + gOvl11TextureCache[textureIndex].height,
        gOvl11TextureCache[textureIndex].x + gOvl11TextureCache[textureIndex].width, gOvl11TextureCache[textureIndex].y + gOvl11TextureCache[textureIndex].height);
    left = centreX - width / 2;
    top = centreY - height / 2;
    setXYWH(packet, left, top, width, height);
    addPrim(ot, packet);
    gActiveRenderContext->field_118 = (s32) (packet + 1);
}

/* Selection-slot texture exchange. The slot operations above invoke this
 * before changing record IDs. Only a paged/near crossing transfers images.
 * releaseOutgoing == 1 reconciles and releases the outgoing near slot.
 * Shared heap scratch +4290/+4A90, RECT, VRAM and slot/cache state must be
 * ready. Synchronous GPU transfers move 8x32/8x64 images and 16x1 CLUTs.
 * Early returns are not transactional: missing/full paths may retain prior
 * mutations. Release compares the eighteen displayed IDs as a multiset,
 * so duplicate records retain distinct near-pane residents. */

void Ovl11TransferSelectionTexture(s32 outgoingId, s32 incomingId, s32 releaseOutgoing);

void Ovl11ReorderSelectionTextures(u16 *firstId, s32 firstPane, u16 *secondId,
                          s32 secondPane, s32 releaseOutgoing) {
    s32 secondCategory;

    secondCategory = Ovl11GetItemSlotLevel(secondPane);
    if (Ovl11GetItemSlotLevel(firstPane) == 2) {
        if (secondCategory != 2) {
            Ovl11TransferSelectionTexture((s32) *secondId, (s32) *firstId, releaseOutgoing);
        }
    } else if (secondCategory == 2) {
        Ovl11TransferSelectionTexture((s32) *firstId, (s32) *secondId, releaseOutgoing);
    }
}

void Ovl11ReleaseUnusedSelectionTextures(void);

#define imageRect (*(RECT *)gOvl11TextureTransferRect)
#define TRANSFER_SLOTS (((Ovl11FarSlotTableView *)&D_8007AFF0)->slots)
#define records (((Ovl11FarRecordTableView *)&D_8007AFF0)->records)
#define cache gOvl11TextureCache
#define cacheImage(i) (*(Ovl11A04B8Entry *)&cache[i])

void Ovl11TransferSelectionTexture(s32 outgoingId, s32 incomingId, s32 releaseOutgoing) {
    Ovl11TextureRecord backup;
    s32 oldRow;
    s16 row;
    s32 slot;
    s16 index;
    s16 special;
    s32 position;
    s32 column;
    s16 clutColumn;
    s16 page;
    s16 clutPage;
    s32 pageX;
    s32 clutY;

    outgoingId &= 0xFFFF;
    incomingId &= 0xFFFF;
    if (releaseOutgoing == 1) {
        Ovl11ReleaseUnusedSelectionTextures();
    }
    special = 0;
    position = 0;
    if (outgoingId != 0) {
        oldRow = (s16)Ovl11ClassifyItemFlags(outgoingId) != 0;
        for (slot = 0; slot < 9; slot = (s16)(slot + 1)) {
            if (TRANSFER_SLOTS[oldRow][slot] == outgoingId) {
                break;
            }
        }
        if (slot == 9) {
            return;
        }
        if (records[oldRow][slot].unk18 >= 0x3F0) {
            special = 1;
        }
        imageRect.w = 8;
        imageRect.x = records[oldRow][slot].unk0;
        imageRect.y = records[oldRow][slot].unk2;
        imageRect.h = oldRow == 0 ? 0x20 : 0x40;
        if (special == 1) {
            imageRect.x += 0x3C0;
        } else {
            imageRect.x += 0x340;
        }
        StoreImage(&imageRect, (u_long *)(gGraphicsHeapBase + 0x4290));
        DrawSync(0);
        imageRect.w = 0x10;
        imageRect.h = 1;
        imageRect.x = records[oldRow][slot].unk18;
        imageRect.y = records[oldRow][slot].unk1A;
        StoreImage(&imageRect, (u_long *)(gGraphicsHeapBase + 0x4A90));
        DrawSync(0);
        backup.active = 1;
        backup.id = outgoingId;
        *(Ovl11A04B8Entry *)&backup = records[oldRow][slot];
        if (releaseOutgoing == 1) {
            Ovl11RemoveSlotSprite(outgoingId);
        }
    }
    if (incomingId != 0) {
        row = (s16)Ovl11ClassifyItemFlags(incomingId) != 0;
        for (index = 0; index < 72; index++) {
            if (cache[index].id == incomingId) {
                break;
            }
        }
        if (index == 72) {
            return;
        }
        slot = (s16)Ovl11InsertSlotSpriteId(incomingId, row);
        if (slot == -1) {
            return;
        }
        special = Ovl11CanUseSharedSlotClut(row);
        TRANSFER_SLOTS[row][slot] = incomingId;
        records[row][slot] = cacheImage(index);
        imageRect.w = 8;
        imageRect.x = cache[index].x;
        imageRect.y = cache[index].y;
        imageRect.h = row == 0 ? 0x20 : 0x40;
        if (special == 1) {
            records[row][slot].unk0 = row * 8 + 0x28;
            records[row][slot].unk2 = 0xBD;
            records[row][slot].unk4 = records[row][slot].unk0;
            records[row][slot].unk6 = records[row][slot].unk2 + records[row][slot].unkA;
            MoveImage2(&imageRect, row * 8 + 0x3E8, 0xBD);
        } else {
            position = (s16)Ovl11FindFreeSlotTextureRank(row);
            records[row][slot].unk0 = position * 8;
            records[row][slot].unk2 = row * 32 + 0x9B;
            records[row][slot].unk4 = records[row][slot].unk0;
            records[row][slot].unk6 = records[row][slot].unk2 + records[row][slot].unkA;
            MoveImage2(&imageRect, position * 8 + 0x340, row * 32 + 0x9B);
        }
        imageRect.w = 0x10;
        imageRect.x = cache[index].clutX;
        imageRect.h = 1;
        imageRect.y = cache[index].clutY;
        if (special == 1) {
            records[row][slot].unk18 = 0x3F0;
            records[row][slot].unk1A = row + 0xFE;
            MoveImage2(&imageRect, 0x3F0, row + 0xFE);
        } else {
            records[row][slot].unk18 = (position - (s16)(position / 4) * 4) * 16 + 0x340;
            records[row][slot].unk1A = position / 4 + (clutY = row * 2 + 0xFB);
            MoveImage2(&imageRect, (position - (s16)(position / 4) * 4) * 16 + 0x340, (s16)(position / 4) + clutY);
        }
    }
    if (outgoingId != 0) {
        for (index = 0; index < 72; index++) {
            if (cache[index].id == outgoingId) {
                return;
            }
        }
        oldRow = (s16)Ovl11ClassifyItemFlags(outgoingId) != 0;
        for (index = 0; index < 72; index++) {
            if (cache[index].id == 0) {
                break;
            }
        }
        if (index == 72) {
            return;
        }
        column = (s16)(index / 8);
        column = (index - column * 8) * 8;
        page = index / 24;
        pageX = (page << 6) + 0x140;
        backup.x = column + pageX;
        backup.y = ((index - page * 24) / 8) << 6;
        imageRect.x = backup.x;
        imageRect.y = backup.y;
        imageRect.w = 8;
        imageRect.h = oldRow == 0 ? 0x20 : 0x40;
        LoadImage(&imageRect, (u_long *)(gGraphicsHeapBase + 0x4290));
        DrawSync(0);
        clutColumn = index / 4;
        clutPage = index / 24;
        backup.clutX = (index - clutColumn * 4) * 16 + ((clutPage << 6) + 0x140);
        backup.clutY = (index - clutPage * 24) / 4 + 0xC0;
        imageRect.x = backup.clutX;
        imageRect.y = backup.clutY;
        imageRect.w = 0x10;
        imageRect.h = 1;
        LoadImage(&imageRect, (u_long *)(gGraphicsHeapBase + 0x4A90));
        DrawSync(0);
        cache[index].id = outgoingId;
        cacheImage(index) = *(Ovl11A04B8Entry *)&backup;
    }
}

#undef imageRect
#undef TRANSFER_SLOTS
#undef records
#undef cache
#undef cacheImage

void Ovl11ReleaseUnusedSelectionTextures(void) {
    Ovl11Slots524CView *base;
    char *far_base;
    s16 sp10[36];
    s16 *src;
    s16 *q;
    s16 temp_v1;
    s16 var_a0_2;
    s32 var_a1_2;
    s32 var_v1;

    base = (Ovl11Slots524CView *) D_8006C838;
    for (var_v1 = 0; var_v1 < 0x12; var_v1++) {
        if (var_v1 < 8) {
            sp10[var_v1] = base->slots[var_v1 + 2].unk0;
        } else if (var_v1 == 8) {
            sp10[var_v1] = ((Ovl11Slots524CView *) D_8006C838)->slots[0].unk0;
        } else if (var_v1 < 17) {
            sp10[var_v1] = base->slots[var_v1 + 1].unk0;
        } else if (var_v1 == 17) {
            sp10[var_v1] = ((Ovl11Slots524CView *) D_8006C838)->slots[1].unk0;
        }
    }
    var_v1 = 0;
    far_base = (char *) D_8009AFF0;
    src = (s16 *) (far_base + 0x54A4);
    do {
        q = src + var_v1;
        var_a0_2 = *q;
        if (var_a0_2 != 0) {
            for (var_a1_2 = 0; var_a1_2 < 0x12; var_a1_2++) {
                temp_v1 = sp10[var_a1_2];
                if (temp_v1 != 0 && var_a0_2 == temp_v1) {
                    var_a0_2 = 0;
                    sp10[var_a1_2] = 0;
                    break;
                }
            }
            if (var_a0_2 != 0) {
                Ovl11RemoveSlotSprite(var_a0_2);
            }
        }
        var_v1 += 1;
    } while (var_v1 < 0x12);
}
