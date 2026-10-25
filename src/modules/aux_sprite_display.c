/*
 * Resident auxiliary sprite display, exe:0x8001AFE0-0x8001B258.
 * Its source/header and OT/packet workspace are private to this group; the
 * GP-relative enable/render guards connect initialization, queries and present.
 * The display uses the main renderer's double-buffer contexts absolutely,
 * establishing a separate TU from game_loop. See notes/file-groupings.md.
 */
#include "common.h"
#include "aux_sprite_display.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/memory.h"
#include "sprite_source.h"
#include "sprite_upload.h"
#include "sprite_render.h"

void func_80012A14(void);
void func_800129F4(void);
void InitAuxSpriteDisplaySource(s32, s32, s32, s32, s32, s32);

/* Tentative definitions retain this unit's original GP-relative accesses. */
s32 gAuxSpriteDisplayEnabled;
s32 gAuxSpriteDisplayRendering;

void ResetAuxSpriteDisplay(void) {
    memset(&gAuxSpriteOrderingTables, 0, 0x1300);
    InitAuxSpriteDisplaySource(0, 0, 0, 0, 0, 0);
}

INCLUDE_ASM("build/asm/nonmatchings/modules/aux_sprite_display", InitAuxSpriteDisplaySource16);

/* The original signed-halfword wrapper and reset wrapper both pass six
 * values. All are unused: retain their material call arguments and ABI rather
 * than deleting them based only on this initializer's unread parameters. */
void InitAuxSpriteDisplaySource(s32 unused0, s32 unused1, s32 unused2,
                                 s32 unused3, s32 unused4, s32 unused5) {
    unsigned long *ot = (unsigned long *)&gAuxSpriteOrderingTables;
    SpriteSourceData *source;
    SpriteDataHeader *header;

    func_80012A14();
    header = (SpriteDataHeader *)&gAuxSpriteHeader;
    UploadSpriteImages((u8 *)(*(s32 *)((char *)header - 4) + (s32)header));
    source = (SpriteSourceData *)&gAuxSpriteSource;
    InitSpriteSourceData(source, header);
    ClearOTagR(ot, 0x20);
    ClearOTagR(ot + 0x20, 0x20);
    SelectSpriteAnimation((ObjectState *)source, 0);
    gAuxSpriteDisplayEnabled = 1;
}

void DisableAuxSpriteDisplay(void) {
    gAuxSpriteDisplayEnabled = 0;
}

s32 IsAuxSpriteDisplayEnabled(void) {
    return gAuxSpriteDisplayEnabled;
}

void PresentAuxSpriteDisplay(void) {
    char *tables;
    char *ot;
    char *polygonCursor;
    char *spriteCursor;
    struct_8005E3C0 *context;
    struct_8005E3C0 *contextBase;

    tables = (char *)&gAuxSpriteOrderingTables;
    if ((gAuxSpriteDisplayEnabled != 0) && (gAuxSpriteDisplayRendering != 1)) {
        gAuxSpriteDisplayRendering = 1;
        DrawSync(0);
        func_800129F4();
        DrawOTag((unsigned long *)(tables + (gActiveRenderContextIndex << 7) + 0x7C));

        contextBase = (struct_8005E3C0 *)&gRenderContexts;
        context = contextBase;
        if (gActiveRenderContext == contextBase) {
            context = (struct_8005E3C0 *)((char *)contextBase + 0x134);
        }
        gActiveRenderContext = context;
        gActiveRenderContextIndex = (context != contextBase);
        PutDispEnv((void *)((char *)context + 0x5C));
        PutDrawEnv((void *)gActiveRenderContext);

        ot = tables + (gActiveRenderContextIndex << 7);
        polygonCursor = (gActiveRenderContextIndex * 0x280) + 0x100 + tables;
        spriteCursor = (gActiveRenderContextIndex * 0x180) + 0x600 + tables;
        ClearOTagR((unsigned long *)ot, 0x20);
        BuildSpriteAnimationRectsAndUpdate((s32)ot, (s32)&polygonCursor, (s32)&spriteCursor,
                      (SpriteSourceData *)&gAuxSpriteSource, 0xA0, 0x78);
        gAuxSpriteDisplayRendering = 0;
    }
}
