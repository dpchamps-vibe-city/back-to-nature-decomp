/*
 * Backdrop staging and eight-source sprite pool, ovl_27:800B9324..800BA4C4.
 * Pool objects: 800C4A80+0x30*i; six effect counters: 800C4A2C+2*i.
 * 800C4BD0 supplies all fixed/tiled backdrop draws; 800C4BA0 is initialized
 * but not drawn here. The loader copies the shared header to 800C4C00.
 * The interior text-bank label helper is retained with the complete run.
 * See ovl27_backdrop_sprite_pool.h and notes/file-groupings.md for contracts,
 * membership, weaker label-helper provenance and evidence-limited boundaries.
 */
#include "common.h"
#include "ovl27_backdrop_sprite_pool.h"
#include "game_types.h"
#include "random.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "sprite_render.h"
#include "psyq/memory.h"
#include "cd_loader.h"
#include "sprite_upload.h"
#include "beveled_panel.h"

void Ovl27DrawBackdropAnim30(void) {
    DrawSpriteFrameRects(gActiveRenderContext->field_D8 + 0x88, ((s32)(&D_800C4BD0)), 0x1E, 0, 0, 0);
}

void Ovl27DrawBackdropAnim32(s16 y) {
    DrawSpriteFrameRects(gActiveRenderContext->field_D8 + 0x88, ((s32)(&D_800C4BD0)), 0x20, 0, 0, y);
}

void Ovl27DrawBackdropAnim31(s16 y) {
    DrawSpriteFrameRects(gActiveRenderContext->field_D8 + 0x88, ((s32)(&D_800C4BD0)), 0x1F, 0, 0, y);
}

void Ovl27DrawBackdropAnim33(s16 x) {
    DrawSpriteFrameRects(gActiveRenderContext->field_D8 + 0x88, ((s32)(&D_800C4BD0)), 0x21, 0, x, 0);
}

void Ovl27DrawTiledBackdrop(void) {
    s16 firstPage;
    s16 firstRow1Y;
    s16 firstRow2Y;
    s16 secondRow1Y;
    s16 secondRow2Y;
    s16 firstSway;
    s16 firstX2;
    s16 secondSway;
    s16 secondX2;
    s16 firstX1;
    s16 secondX1;
    s16 firstX0;
    s16 secondX0;
    s16 rowY;
    s32 orderingTable;
    s16 firstPhaseWrap;
    s16 secondPage;
    s16 secondPhaseWrap;
    s32 nextFirstPhase;
    s32 nextSecondPhase;
    s32 firstOffset1;
    s32 firstOffset2;
    s32 secondOffset1;
    s32 secondOffset2;

    /* Two explicitly ordered 3x3 layers. The narrow temporaries and separate
     * offset births preserve the original halfword truncation/dataflow. */
    firstPage = D_800C4A4C / 240;
    rowY = (firstPage * 0xF0) - (u16)D_800C4A4C;
    firstSway = 0x140 - (firstPage * 0x50);
    orderingTable = gActiveRenderContext->field_D8 + 0x90;
    firstSway += ccos((D_800C4A4E << 12) / 120) * 12 / 4096;
    firstX0 = rowY - firstSway;
    DrawSpriteFrameRects(orderingTable, (s32)D_800C4BD0, 0x25, 0, (s16)(s32)firstX0, (s16)(s32)rowY);
    firstOffset1 = firstSway - 0x140;
    firstX1 = rowY - firstOffset1;
    DrawSpriteFrameRects(orderingTable, (s32)D_800C4BD0, 0x25, 0, (s16)(s32)firstX1, (s16)(s32)rowY);
    firstOffset2 = firstSway - 0x280;
    firstX2 = rowY - firstOffset2;
    DrawSpriteFrameRects(orderingTable, (s32)D_800C4BD0, 0x25, 0, (s16)(s32)firstX2, (s16)(s32)rowY);
    firstRow1Y = rowY + 0xF0;
    DrawSpriteFrameRects(orderingTable, (s32)D_800C4BD0, 0x25, 0, (s16)(s32)firstX0, (s16)(s32)firstRow1Y);
    DrawSpriteFrameRects(orderingTable, (s32)D_800C4BD0, 0x25, 0, (s16)(s32)firstX1, (s16)(s32)firstRow1Y);
    DrawSpriteFrameRects(orderingTable, (s32)D_800C4BD0, 0x25, 0, (s16)(s32)firstX2, (s16)(s32)firstRow1Y);
    firstRow2Y = rowY + 0x1E0;
    DrawSpriteFrameRects(orderingTable, (s32)D_800C4BD0, 0x25, 0, (s16)(s32)firstX0, (s16)(s32)firstRow2Y);
    DrawSpriteFrameRects(orderingTable, (s32)D_800C4BD0, 0x25, 0, (s16)(s32)firstX1, (s16)(s32)firstRow2Y);
    DrawSpriteFrameRects(orderingTable, (s32)D_800C4BD0, 0x25, 0, (s16)(s32)firstX2, (s16)(s32)firstRow2Y);
    if (D_8006C840 & 1) {
        nextFirstPhase = (u16)D_800C4A4C;
        nextFirstPhase++;
        firstPhaseWrap = (s16)nextFirstPhase / 960;
        nextFirstPhase -= firstPhaseWrap * 960;
        D_800C4A4C = nextFirstPhase;
    }
    secondPage = D_800C4A4E / 240;
    rowY = (u16)D_800C4A4E - secondPage * 240;
    secondSway = 0x140 - secondPage * 0x50;
    secondSway += csin((D_800C4A4E << 12) / 120) * 12 / 4096;
    secondX0 = rowY + secondSway;
    DrawSpriteFrameRects(orderingTable, (s32)D_800C4BD0, 0x25, 0, (s16)(s32)secondX0, (s16)(s32)rowY);
    secondOffset1 = secondSway - 0x140;
    secondX1 = rowY + secondOffset1;
    DrawSpriteFrameRects(orderingTable, (s32)D_800C4BD0, 0x25, 0, (s16)(s32)secondX1, (s16)(s32)rowY);
    secondOffset2 = secondSway - 0x280;
    secondX2 = rowY + secondOffset2;
    DrawSpriteFrameRects(orderingTable, (s32)D_800C4BD0, 0x25, 0, (s16)(s32)secondX2, (s16)(s32)rowY);
    secondRow1Y = rowY - 0xF0;
    DrawSpriteFrameRects(orderingTable, (s32)D_800C4BD0, 0x25, 0, (s16)(s32)secondX0, (s16)(s32)secondRow1Y);
    DrawSpriteFrameRects(orderingTable, (s32)D_800C4BD0, 0x25, 0, (s16)(s32)secondX1, (s16)(s32)secondRow1Y);
    DrawSpriteFrameRects(orderingTable, (s32)D_800C4BD0, 0x25, 0, (s16)(s32)secondX2, (s16)(s32)secondRow1Y);
    secondRow2Y = rowY - 0x1E0;
    DrawSpriteFrameRects(orderingTable, (s32)D_800C4BD0, 0x25, 0, (s16)(s32)secondX0, (s16)(s32)secondRow2Y);
    DrawSpriteFrameRects(orderingTable, (s32)D_800C4BD0, 0x25, 0, (s16)(s32)secondX1, (s16)(s32)secondRow2Y);
    DrawSpriteFrameRects(orderingTable, (s32)D_800C4BD0, 0x25, 0, (s16)(s32)secondX2, (s16)(s32)secondRow2Y);
    nextSecondPhase = (u16)D_800C4A4E;
    nextSecondPhase++;
    secondPhaseWrap = (s16)nextSecondPhase / 960;
    nextSecondPhase -= secondPhaseWrap * 960;
    orderingTable = gActiveRenderContext->field_D8 + 0x94;
    D_800C4A4E = nextSecondPhase;
    DrawSpriteFrameRects(orderingTable, (s32)D_800C4BD0, 0x1D, 0, 0, 0);
}

void Ovl27UpdateEffectSlot0(void) {
    s32 nextPhase;
    s16 wraps;
    s16 animation;

    nextPhase = (u16)gOvl27EffectSlot0Ticks + 1;
    wraps = (s16)nextPhase / 1800;
    gOvl27EffectSlot0Ticks = nextPhase - wraps * 1800;
    DrawSpriteAnimationQuadsAndUpdate(gActiveRenderContext->field_D8 + 0x8C, (SpriteSourceData *)D_800C4A80, 0x11A, 0xAD);
    if (gOvl27EffectSlot0Ticks >= 0x5A) {
        gOvl27EffectSlot0Ticks = 0;
        animation = Rand(0xA);
        if (animation == (animation / 2) * 2) {
            animation -= 1;
        }
        if (animation < 0) {
            animation = 0;
        }
        SelectSpriteAnimation((ObjectState *)D_800C4A80, animation);
    }
}

void Ovl27UpdateEffectSlot1(void) {
    s32 nextPhase;
    s16 wraps;
    s16 animation;

    nextPhase = (u16)gOvl27EffectSlot1Ticks + 1;
    wraps = (s16)nextPhase / 1800;
    gOvl27EffectSlot1Ticks = nextPhase - wraps * 1800;
    DrawSpriteAnimationQuadsAndUpdate(gActiveRenderContext->field_D8 + 0x8C, (SpriteSourceData *)D_800C4AB0, 0x21, 0x92);
    if (gOvl27EffectSlot1Ticks >= 0x3C) {
        gOvl27EffectSlot1Ticks = 0;
        animation = Rand(9) + 10;
        if (animation == (animation / 2) * 2) {
            animation -= 1;
        }
        if (animation < 10) {
            animation = 10;
        }
        SelectSpriteAnimation((ObjectState *)D_800C4AB0, animation);
    }
}

void Ovl27UpdateEffectSlot2(void) {
    s32 nextPhase;
    s16 wraps;
    s16 animation;

    nextPhase = (u16)gOvl27EffectSlot2Ticks + 1;
    wraps = (s16)nextPhase / 1800;
    gOvl27EffectSlot2Ticks = nextPhase - wraps * 1800;
    DrawSpriteAnimationQuadsAndUpdate(gActiveRenderContext->field_D8 + 0x8C, (SpriteSourceData *)D_800C4AE0, 0x33, 0xB6);
    if (gOvl27EffectSlot2Ticks >= 0x3C) {
        gOvl27EffectSlot2Ticks = 0;
        animation = Rand(3) + 0x13;
        if (animation == 0x13) {
            gOvl27EffectSlot2Ticks = 0x1E;
        }
        SelectSpriteAnimation((ObjectState *)D_800C4AE0, animation);
    }
}

void Ovl27UpdateEffectSlot3(void);
void Ovl27UpdateEffectSlot4(void);
void Ovl27UpdateEffectSlot5(void);

void Ovl27UpdateEffectSlots3To5(void) {
    Ovl27UpdateEffectSlot3();
    Ovl27UpdateEffectSlot4();
    Ovl27UpdateEffectSlot5();
}

void Ovl27UpdateEffectSlot3(void) {
    s32 nextPhase;
    s16 wraps;
    s32 orderingTable;

    nextPhase = (u16)gOvl27EffectSlot3Ticks + 1;
    wraps = (s16)nextPhase / 1800;
    gOvl27EffectSlot3Ticks = nextPhase - wraps * 1800;
    orderingTable = gActiveRenderContext->field_D8 + 0x8C;
    DrawSpriteAnimationQuadsAndUpdate(orderingTable, (SpriteSourceData *)D_800C4B10, 0x127, 0xCD);
    DrawSpriteAnimationQuadsAndUpdate(orderingTable, (SpriteSourceData *)D_800C4B10, 0x102, 0x8A);
    DrawSpriteAnimationQuadsAndUpdate(orderingTable, (SpriteSourceData *)D_800C4B10, 0x118, 0x79);
    if (gOvl27EffectSlot3Ticks >= 0xF) {
        gOvl27EffectSlot3Ticks = 0;
        SelectSpriteAnimation((ObjectState *)D_800C4B10, Rand(2) + 0x16);
    }
}

void Ovl27UpdateEffectSlot4(void) {
    s32 nextPhase;
    s16 wraps;
    s32 orderingTable;

    nextPhase = (u16)gOvl27EffectSlot4Ticks + 1;
    wraps = (s16)nextPhase / 1800;
    gOvl27EffectSlot4Ticks = nextPhase - wraps * 1800;
    orderingTable = gActiveRenderContext->field_D8 + 0x8C;
    DrawSpriteAnimationQuadsAndUpdate(orderingTable, (SpriteSourceData *)D_800C4B40, 0x3F, 0x95);
    DrawSpriteAnimationQuadsAndUpdate(orderingTable, (SpriteSourceData *)D_800C4B40, 0xA, 0xBC);
    DrawSpriteAnimationQuadsAndUpdate(orderingTable, (SpriteSourceData *)D_800C4B40, 9, 0xD6);
    DrawSpriteAnimationQuadsAndUpdate(orderingTable, (SpriteSourceData *)D_800C4B40, 0x127, 0x34);
    DrawSpriteAnimationQuadsAndUpdate(orderingTable, (SpriteSourceData *)D_800C4B40, 0x126, 0x1E);
    if (gOvl27EffectSlot4Ticks >= 0x14) {
        gOvl27EffectSlot4Ticks = 0;
        SelectSpriteAnimation((ObjectState *)D_800C4B40, Rand(2) + 0x18);
    }
}

void Ovl27UpdateEffectSlot5(void) {
    s32 nextPhase;
    s16 wraps;
    s32 orderingTable;

    nextPhase = (u16)gOvl27EffectSlot5Ticks + 1;
    wraps = (s16)nextPhase / 1800;
    gOvl27EffectSlot5Ticks = nextPhase - wraps * 1800;
    orderingTable = gActiveRenderContext->field_D8 + 0x8C;
    DrawSpriteAnimationQuadsAndUpdate(orderingTable, (SpriteSourceData *)D_800C4B70, 0x17, 0x1E);
    DrawSpriteAnimationQuadsAndUpdate(orderingTable, (SpriteSourceData *)D_800C4B70, 0x6, 0x6D);
    DrawSpriteAnimationQuadsAndUpdate(orderingTable, (SpriteSourceData *)D_800C4B70, 0x24, 0xDF);
    if (gOvl27EffectSlot5Ticks >= 0x1E) {
        gOvl27EffectSlot5Ticks = 0;
        SelectSpriteAnimation((ObjectState *)D_800C4B70, Rand(2) + 0x1A);
    }
}

void Ovl27DrawBackdropAnim36(void) {
    s32 y;
    s32 x;

    x = 0x68;
    if ((gOvl27TitleAnimationTicks % 30) < 0xF) {
        x = 0x64;
    }
    y = 0xB3;
    if (gOvl27TitleSelection == 0) {
        y = 0x9B;
    }
    DrawSpriteFrameQuads(gActiveRenderContext->field_D8 + 0x88, (s32)D_800C4BD0, 0x24, 0, (s16)x, (s16)y, 0x1000, 0, 0U);
}

void Ovl27DrawBackdropAnim34(void) {
    DrawSpriteFrameRects(gActiveRenderContext->field_D8 + 0x88, ((s32)(&D_800C4BD0)), 0x22, 0, 0, 0);
}

void Ovl27DrawBackdropAnim35(void) {
    DrawSpriteFrameRects(gActiveRenderContext->field_D8 + 0x88, ((s32)(&D_800C4BD0)), 0x23, 0, 0, 0);
}

void func_80017B3C(s32 orderingTable, s32 text, s32 x, s32 y);

void Ovl27DrawTextBankLabel(s16 index) {
    s32 text;

    text = (s32)&D_8005175C + (*gTextBankOffsets + D_800C4A50[index]);
    DrawMenuPanel((u32 *)(gActiveRenderContext->field_D8 + 4), 1, 0x10, 0x10, 0x78, 0x14);
    func_80017B3C(gActiveRenderContext->field_D8, text, 0x14, 0x14);
}

void Ovl27BeginBackdropStageLoad(void) {
    DrawSync(0);
    ClearOTagR((unsigned long *)gActiveRenderContext->field_120, 0x800);
    CdReadArchiveBytes(0, 0x2000, 0x23000, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 1);
}

s32 Ovl27PollBackdropSpritePoolLoad(void) {
    if (CdReadArchiveBytes(0, 0x2000, 0x23000, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 0) != 0) {
        UploadSpriteImages((u8 *)(gGraphicsHeapBase + 0x4FAC));
        memcpy(D_800C4C00, (void *)(gGraphicsHeapBase + 0x4290), 0xD1C);
        InitSpriteSourceData((SpriteSourceData *)D_800C4A80, (SpriteDataHeader *)D_800C4C00);
        InitSpriteSourceData((SpriteSourceData *)D_800C4AB0, (SpriteDataHeader *)D_800C4C00);
        InitSpriteSourceData((SpriteSourceData *)D_800C4AE0, (SpriteDataHeader *)D_800C4C00);
        InitSpriteSourceData((SpriteSourceData *)D_800C4B10, (SpriteDataHeader *)D_800C4C00);
        InitSpriteSourceData((SpriteSourceData *)D_800C4B40, (SpriteDataHeader *)D_800C4C00);
        InitSpriteSourceData((SpriteSourceData *)D_800C4B70, (SpriteDataHeader *)D_800C4C00);
        InitSpriteSourceData((SpriteSourceData *)D_800C4BA0, (SpriteDataHeader *)D_800C4C00);
        InitSpriteSourceData((SpriteSourceData *)D_800C4BD0, (SpriteDataHeader *)D_800C4C00);
        SelectSpriteAnimation((ObjectState *)D_800C4A80, 0);
        SelectSpriteAnimation((ObjectState *)&D_800C4AB0, 0xA);
        SelectSpriteAnimation((ObjectState *)&D_800C4AE0, 0x13);
        SelectSpriteAnimation((ObjectState *)&D_800C4B10, 0x16);
        SelectSpriteAnimation((ObjectState *)&D_800C4B40, 0x18);
        SelectSpriteAnimation((ObjectState *)&D_800C4B70, 0x1A);
        SelectSpriteAnimation((ObjectState *)&D_800C4BA0, 0x1C);
        SelectSpriteAnimation((ObjectState *)D_800C4BD0, 0x1D);
        return 1;
    }
    return 0;
}
