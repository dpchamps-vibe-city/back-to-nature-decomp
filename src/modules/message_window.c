/*
 * Archive-backed message window, EXE 800225B8..8002301C.
 *
 * A one-deep request queue drives read, open, text, close and finish handlers.
 * The dispatch phase is the byte at work-area +CC; the separately published
 * status and choice come from the text engine. The 0x2000-byte record contains
 * an optional sprite prefix and text at +0x1800. Portrait selection uses the
 * text engine's current selector and a seven-bucket value lookup.
 *
 * State is global and non-reentrant. The active render context and text engine
 * are clients, not owned here: their small pointers remain absolute accesses.
 * Public contracts and update-count units are in message_window.h; original
 * membership/boundary evidence is in notes/file-groupings.md.
 */
#include "common.h"
#include "game_types.h"
#include "cd_loader.h"
#include "message_window.h"

/* Tentative definitions retain the original $gp ownership. The saved flag-3
 * word is stored whole but only its low halfword is restored. The archive
 * selector is stored as a word and interpreted unsigned by the lookup. */
s32 gMessageWindowChoice;
s32 gMessageWindowEntry;
s32 gMessageWindowArchive;
s32 gMessageWindowStatus;
s32 gMessageWindowQueuedArchive;
s32 gMessageWindowQueuedEntry;
s32 gMessagePortraitEnabled;
s32 gMessageWindowSavedWorkFlags;
s32 gMessageWindowAnimationStep;
s32 gMessageWindowSavedFlag3;
s32 *gMessageWindowText;

/* Helpers keep external linkage; co-location alone does not prove static. */
s32 FinishMessageWindow(void);
s16 GetMessageWindowArchiveId(void);
void DrawMessagePortrait(void);
s32 GetMessagePortraitValueBucket(s32 selector);
s32 AnimateMessageWindowRect(s32 packet, s32 unused, s16 x, s16 y,
    s16 width, s16 height, s32 *step, s32 heightSteps, s32 opening);

void SetVal8005E2BC(s32 value);
void func_80022580(u32 *packet, s32 style, s32 x, s32 y, s16 width, s16 height);
s32 func_8001AF44(u32 slot);
void func_8001AF70(u16 slot, u16 value);
void func_800179E8(void);
void func_80015704(SpriteSourceData *out, SpriteDataHeader *header);
void func_80022008(void);
void func_80017A38(s16 x, s16 y);
u32 func_80017A64(void);
void func_80017A48(u32 mode);
s32 func_80017BC8(s32 *packet, u16 *text, s16 x, s16 y, s16 width, s16 height);
void func_80015840(ObjectState *obj, u8 state);
void func_80015C50(s32 packet, SpriteSourceData *source, s16 x, s16 y);
s16 func_80017B30(void);
void func_8001719C(u8 *data);
void func_80017C3C(s32 slot);

s32 GetMessageWindowChoice(void) {
    return gMessageWindowChoice;
}

s32 UpdateMessageWindow(void) {
    struct struct_8006C838_view *work;
    s32 phase;
    s32 (*handler)(void *);

    work = (struct struct_8006C838_view *)&D_8006C838;
    phase = work->field_CC;
    if (phase == 0) {
        return 1;
    }
    handler = gMessageWindowHandlers[phase];
    return handler(work);
}

s32 RequestMessageWindow(s32 archive, s32 entry) {
    s32 rejected;
    struct struct_8006C838 *work;

    rejected = 1;
    work = D_8006C838;
    if (((u8 *)work)[0xCC] == 0) {
        ((u8 *)work)[0xCC] = 1U;
        rejected = 0;
        gMessageWindowArchive = archive;
        gMessageWindowEntry = entry;
        gMessageWindowStatus = 0;
    }
    if (((u32)(((u8 *)work)[0xCC] - 5) < 2U) &&
        ((gMessageWindowArchive != archive) || (gMessageWindowEntry != entry)) &&
        (gMessageWindowQueuedArchive == -1)) {
        rejected = 0;
        gMessageWindowQueuedArchive = archive;
        gMessageWindowQueuedEntry = entry;
    }
    return rejected;
}

s32 GetMessageWindowStatus(void) {
    return gMessageWindowStatus;
}

s32 IsMessageWindowStatusFinal(void) {
    s32 status;
    s32 final;

    status = gMessageWindowStatus;
    final = 0;
    if (status == 2) {
        final = 1;
    } else if (status == 5) {
        final = 1;
    }
    return final;
}

void SetMessagePortraitEnabled(s32 enabled) {
    gMessagePortraitEnabled = enabled;
}

s32 GetMessagePortraitEnabled(void) {
    return gMessagePortraitEnabled;
}

void ResetMessageWindow(void) {
    gMessageWindowSavedWorkFlags = 0;
    FinishMessageWindow();
    gMessageWindowPhase = 0;
    SetVal8005E2BC(1);
    SetMessagePortraitEnabled(1);
    gMessageWindowStatus = 2;
    gMessageWindowQueuedArchive = -1;
}

void CloseMessageWindow(void) {
    char *work;

    SetVal8005E2BC(1);
    SetMessagePortraitEnabled(1);
    work = (char *)&D_8006C838;
    if (*(u8 *)(work + 0xCC) == 4) {
        *(u8 *)(work + 0xCC) = 5;
        gMessageWindowAnimationStep = 0;
        gMessageWindowStatus = 0xF;
    }
}

void SetMessageWindowSavedWorkFlags(s32 flags) {
    gMessageWindowSavedWorkFlags = flags;
}

/* Expand horizontally for six steps, then vertically for heightSteps;
 * closing runs the progression backwards. Q12 halves round toward zero.
 * The retry skips sub-three-pixel heights, so one update can consume more
 * than one step. Preserve the two arithmetic stages and the minimum height. */
s32 AnimateMessageWindowRect(s32 packet, s32 unused, s16 x, s16 y,
    s16 width, s16 height, s32 *step, s32 heightSteps, s32 opening) {
    s32 heightHalfStepQ12;
    s32 widthHalfStepQ12;
    s32 halfHeightQ12;
    s32 drawX;
    s32 drawY;
    s32 drawWidth;
    s32 drawHeight;
    s32 product;
    s32 closingProgress;
    s32 progress;
    s32 lastStep;
    s32 savedStep;

retry:
    drawY = y + (height / 2);
    drawX = x + (width / 2);
    halfHeightQ12 = height << 11;
    heightHalfStepQ12 = halfHeightQ12 / heightSteps;
    widthHalfStepQ12 = (width << 11) / 6;
    if (opening == 1) {
        progress = *step;
        savedStep = *step;
    } else {
        closingProgress = *step - 6;
        progress = heightSteps - closingProgress;
        savedStep = *step;
    }
    if (progress < 6) {
        product = widthHalfStepQ12 * progress;
        if (product < 0) {
            progress = product + 0xFFF;
        } else {
            progress = product;
        }
        progress >>= 12;
        drawX -= progress;
        drawHeight = 4;
        drawWidth = progress * 2;
    } else {
        product = heightHalfStepQ12 * (progress - 6);
        drawX = x;
        if (product < 0) {
            progress = product + 0xFFF;
        } else {
            progress = product;
        }
        product = progress >> 12;
        drawHeight = product;
        drawY -= drawHeight;
        drawHeight = drawHeight * 2;
        drawWidth = width;
        if (drawHeight < 3) {
            *step += 1;
            goto retry;
        }
        if (drawHeight < 4) {
            drawHeight = 4;
        }
    }
    lastStep = heightSteps + 6;
    if (savedStep >= lastStep) {
        if (opening == 1) {
            func_80022580(packet, 0, x, y, (s32)width, (s32)height);
        }
        *step = lastStep;
        return 1;
    }
    func_80022580(packet, 0, (s16)drawX, (s16)drawY, (s32)drawWidth, (s32)drawHeight);
    *step += 1;
    return 0;
}

s32 BeginMessageWindowRead(void) {
    struct struct_8006C838_view *work;
    s32 flags;

    gMessageWindowSavedFlag3 = func_8001AF44(3);
    func_8001AF70(3, 1);
    work = (struct struct_8006C838_view *)D_8006C838;
    flags = work->field_0C;
    gMessageWindowStatus = 0xE;
    gMessageWindowSavedWorkFlags = flags;
    flags |= 0x08000000;
    work->field_0C = flags;
    CdReadArchiveBytes(GetMessageWindowArchiveId(), gMessageWindowEntry << 13,
        0x2000, (u8 *)work + 0xD8, 0, 1);
    gMessageWindowAnimationStep = 0;
    work->field_CC = 2;
    return 0;
}

s32 PollMessageWindowRead(void) {
    u8 *loadedRecord;
    s32 opened;
    s8 nextPhase;

    loadedRecord = CdReadArchiveBytes(GetMessageWindowArchiveId(),
        gMessageWindowEntry << 13, 0x2000, (u8 *)&gMessageWindowReadBuffer, 0, 0);
    opened = AnimateMessageWindowRect(gActiveRenderContext->field_D8 + 8,
        0, 0x1A, 0xA4, 0x10B, 0x3E, &gMessageWindowAnimationStep, 0xC, 1);
    if (loadedRecord != 0) {
        LoadMessageWindowSprite((s32 *)loadedRecord);
        gMessageWindowText = (s32 *)((u8 *)&gMessageWindowReadBuffer + 0x1800);
        func_800179E8();
        nextPhase = 3;
        if (opened == 1) {
            nextPhase = 4;
        }
        ((u8 *)&gMessageWindowReadBuffer)[-0xC] = nextPhase;
        func_80015704((SpriteSourceData *)&gMessagePortraitSource,
            (SpriteDataHeader *)&gSharedSpriteData);
        func_80022008();
    }
    return 0;
}

s16 GetMessageWindowArchiveId(void) {
    u32 archive;

    archive = gMessageWindowArchive;
    if (archive >= 5U) {
        archive = 0;
    }
    return gMessageWindowArchiveIds[archive];
}

s32 OpenMessageWindow(void) {
    if (AnimateMessageWindowRect(gActiveRenderContext->field_D8 + 8,
        0, 0x1A, 0xA4, 0x10B, 0x3E, &gMessageWindowAnimationStep, 0xC, 1) == 1) {
        gMessageWindowPhase = 4;
    }
    return 0;
}

s32 ProcessMessageWindowText(void) {
    s32 drawPortrait;
    u32 savedTextMode;

    func_80017A38(0, 2);
    drawPortrait = 0;
    savedTextMode = func_80017A64();
    func_80017A48(3);
    gMessageWindowStatus = func_80017BC8((s32 *)(gActiveRenderContext->field_D8 + 8),
        (u16 *)gMessageWindowText, 0x1E, 0xA8, 0x107, 0x3A);
    func_80017A48(savedTextMode);
    func_80017A38(0, 0);
    gMessageWindowChoice = 0;
    switch (gMessageWindowStatus) {
    case 0:
    case 6:
    case 7:
    case 8:
    case 9:
        break;
    case 2:
        ((struct struct_8006C838_view *)&D_8006C838)->field_CC = 5;
        gMessageWindowStatus = 0xF;
        break;
    case 1:
    case 3:
    case 4:
    case 5:
        drawPortrait = 1;
        break;
    case 10:
        ((struct struct_8006C838_view *)&D_8006C838)->field_CC = 5;
        gMessageWindowChoice = 1;
        gMessageWindowStatus = 0xF;
        break;
    case 11:
        ((struct struct_8006C838_view *)&D_8006C838)->field_CC = 5;
        gMessageWindowChoice = 2;
        gMessageWindowStatus = 0xF;
        break;
    case 12:
        ((struct struct_8006C838_view *)&D_8006C838)->field_CC = 5;
        gMessageWindowChoice = 3;
        gMessageWindowStatus = 0xF;
        break;
    case 13:
        ((struct struct_8006C838_view *)&D_8006C838)->field_CC = 5;
        gMessageWindowChoice = 4;
        gMessageWindowStatus = 0xF;
        break;
    }
    if (((struct struct_8006C838_view *)&D_8006C838)->field_CC != 6) {
        func_80022580((u32 *)(gActiveRenderContext->field_D8 + 0xC),
            0, 0x1A, 0xA4, 0x10B, 0x3E);
        if (drawPortrait != 0) {
            if (gMessagePortraitEnabled == 1) {
                DrawMessagePortrait();
            }
        }
    }
    if (((struct struct_8006C838_view *)&D_8006C838)->field_CC == 5) {
        gMessageWindowAnimationStep = 0;
    }
    return 0;
}

s32 ShrinkMessageWindow(void) {
    if (AnimateMessageWindowRect(gActiveRenderContext->field_D8 + 8,
        0, 0x1A, 0xA4, 0x10B, 0x3E, &gMessageWindowAnimationStep, 6, 0) == 1) {
        gMessageWindowPhase = 6;
        if (gMessageWindowQueuedArchive == -1) {
            gMessageWindowStatus = 2;
        }
    }
    return 0;
}

s32 FinishMessageWindow(void) {
    func_8001AF70(3, (u16)gMessageWindowSavedFlag3);
    if (gMessageWindowSavedWorkFlags & 0x08000000) {
        ((struct struct_8006C838_view *)&D_8006C838)->field_0C |= 0x08000000;
    } else {
        ((struct struct_8006C838_view *)&D_8006C838)->field_0C &= 0xF7FFFFFF;
    }
    ((struct struct_8006C838_view *)&D_8006C838)->field_CC = 0;
    if (gMessageWindowQueuedArchive != -1) {
        RequestMessageWindow(gMessageWindowQueuedArchive, gMessageWindowQueuedEntry);
        gMessageWindowQueuedArchive = -1;
    } else {
        gMessageWindowStatus = 2;
    }
    return 1;
}

void DrawMessagePortrait(void) {
    s32 selector;
    s32 bucket;

    selector = func_80017B30();
    bucket = 7;
    if (selector != 0) {
        bucket = GetMessagePortraitValueBucket(selector);
    }
    if ((u8)_gMessagePortraitSource[1] != bucket) {
        func_80015840((ObjectState *)&gMessagePortraitSource, bucket & 0xFF);
    }
    func_80015C50(gActiveRenderContext->field_D8 + 4,
        (SpriteSourceData *)&gMessagePortraitSource, 0x11D, 0xD9);
}

/* The lookup's gameplay meaning is not established. Keep the measured
 * thresholds and unchecked signed selector rather than naming a mood. */
s32 GetMessagePortraitValueBucket(s32 selector) {
    s32 index;
    char *base;
    s16 recordIndex;
    char *record;
    u16 value;

    index = (s16)selector;
    base = (char *)&D_8006C838;
    recordIndex = gMessagePortraitValueIndices[index];
    record = base + recordIndex * 468;
    value = *(u16 *)(record + 0x99EC);
    if (value < 5000) {
        return 0;
    }
    if (value < 20000) {
        return 1;
    }
    if (value < 30000) {
        return 2;
    }
    if (value < 40000) {
        return 3;
    }
    if (value < 50000) {
        return 4;
    }
    if (value < 60000) {
        return 5;
    }
    return 6;
}

void LoadMessageWindowSprite(s32 *record) {
    s32 textSlot;

    textSlot = *record;
    if (textSlot != -1) {
        func_8001719C((u8 *)record + 4);
        func_80017C3C(textSlot);
    }
}
