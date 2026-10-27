/*
 * Active-record and attached-entity sprite service (ovl_11:E44D4..E499C).
 * The two scan drivers share three projection/draw helpers, in original order.
 * All use the same low-halfword coordinate adjustment and depth-to-OT mapping.
 * Resource installation and the object archive interpreter are separate clients.
 * Pause bits have distinct effects; see ovl11_active_record_sprites.h.
 */
#include "common.h"
#include "game_types.h"
#include "ovl11_active_record_sprites.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "game_flags.h"
#include "ovl11_timed_sprite_effect.h"
#include "ovl11_projected_record_sprites.h"
#include "ovl11_surface_height.h"
#include "ovl11_shared_animation_icon.h"
#include "ovl11_active_record_sprite_packet.h"

void Ovl11UpdateRecordSprite(u8 *record);
void Ovl11DrawEntityRecordSprite(Ovl11EntitySpritePositionView *position,
                                SpriteSourceData *source);
void Ovl11DrawCurrentObjectSprite(void);
void func_80016054(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 arg4,
                  s16 arg5, s16 arg6, s32 arg7, s32 arg8, u16 arg9);

void Ovl11TickActiveRecordSprites(void) {
    s32 slotIndex;
    struct_80076220 *slot;

    if (!(D_8006C844 & 0x800000)) {
        for (slotIndex = 0; slotIndex < 37; slotIndex++) {
            slot = &(&gObjectEventSlotBase)[slotIndex];
            if ((*(s32 *)((u8 *)slot + 0x1C) & 0x18000000) == 0x18000000) {
                Ovl11TickTimedSpriteEffect((Ov11Timer *)((u8 *)slot + 0xE0));
                Ovl11UpdateRecordSprite(slot->unk0);
            }
        }
    }
}

/* Entity records and sprite sources are parallel arrays with different strides. */
void Ovl11DrawAttachmentRecordSprites(void) {
    u8 *work;
    u8 *entity;
    SpriteSourceData *source;
    s32 entityOffset;
    s32 remaining;

    if (!(D_8006C844 & 0x800000)) {
        work = D_80074838;
        source = (SpriteSourceData *)gOvl11EntitySpriteSources;
        entityOffset = 0;
        remaining = 9;
        do {
            entity = (u8 *)(*(s32 *)(work + 0x5DD0) + entityOffset);
            if (*(u16 *)(entity + 4) & 1) {
                Ovl11DrawEntityRecordSprite((Ovl11EntitySpritePositionView *)entity, source);
            }
            source += 1;
            remaining -= 1;
            entityOffset += 0x18;
        } while (remaining >= 0);
        Ovl11DrawCurrentObjectSprite();
    }
}

void Ovl11DrawCurrentObjectSprite(void) {
    struct struct_8006C838_800E4608 *state = (struct struct_8006C838_800E4608 *)D_8006C838;
    s32 screen[4];
    u16 box[3];

    if (!(state->field_0C & 0x800000) && (state->field_4476 == 0)) {
        box[0] = state->field_4478 + 0x96;
        box[1] = state->field_447C - 0x64;
        box[2] = state->field_4480 - 0x96;
        if (Ovl11ProjectRecordSprite(box, screen) != 0) {
            Ovl11DrawActiveRecordSpritePacket(gActiveRenderContext->field_120 + ((screen[2] >> 2) * 4),
                               (s16)screen[0], (s16)screen[1]);
        }
    }
}

void Ovl11UpdateRecordSprite(u8 *record) {
    s32 screen[4];
    u16 box[3];
    s32 ot;
    s32 paused;

    paused = ((Ovl11E46A8State *)D_8006C838)->flags & 0x08000000;
    if (!paused) {
        if (*(s32 *)(record + 0x70) || *(s32 *)(record + 0x74) ||
            *(s32 *)(record + 0x78)) {
            *(s32 *)(record + 0x60) += *(s32 *)(record + 0x70);
            *(s32 *)(record + 0x64) += *(s32 *)(record + 0x74);
            *(s32 *)(record + 0x68) += *(s32 *)(record + 0x78);
            Ovl11ResolveSurfaceHeight((s32 *)(record + 0x60), 0xC000);
        }
    }
    box[0] = *(u16 *)(record + 0x60) + 0x96;
    box[1] = *(u16 *)(record + 0x64) - 0x64;
    box[2] = *(u16 *)(record + 0x68) - 0x96;
    if (Ovl11ProjectRecordSprite(box, screen)) {
        ot = gActiveRenderContext->field_120 + ((screen[2] >> 2) * 4);
        if (GetGameFlag(0x72) && *(s16 *)record == ((Ovl11E46A8State *)D_8006C838)->selectedId) {
            ot -= 0x14;
        }
        SetSpriteAnimationUpdateLatch(0);
        func_80016054(ot, gActiveRenderContext->field_118, (s32)(record + 0xB0),
                      record[0xB4], record[0xB5], (s16)screen[0], (s16)screen[1],
                      *(s32 *)(record + 0xB8), 0, 0x40);
        Ovl11DrawTimedSpriteEffect(record + 0xE0, ot, (s16)screen[0],
                                  (s16)(screen[1] - 0x2D));
        if (!(*(u16 *)(record + 0x20) & 0x8000)) {
            Ovl11DrawSharedAnimationIcon(ot, screen[0], screen[1],
                                        (*(u16 *)(record + 0x20) >> 14) & 1);
        }
    }
    /* These record codes keep animating even while movement is paused. */
    if (!paused || ((Ovl11E46A8State *)D_8006C838)->current[1] == 0x19D ||
        ((Ovl11E46A8State *)D_8006C838)->current[1] == 0x1BC ||
        ((Ovl11E46A8State *)D_8006C838)->current[1] == 0x1BD ||
        ((Ovl11E46A8State *)D_8006C838)->current[1] == 0x1BE ||
        ((Ovl11E46A8State *)D_8006C838)->current[1] == 0x1BF) {
        UpdateSpriteAnimation((SpriteSourceData *)(record + 0xB0));
    }
}

void Ovl11DrawEntityRecordSprite(Ovl11EntitySpritePositionView *position,
                                SpriteSourceData *source) {
    s32 screen[4];
    u16 box[3];

    box[0] = position->xLow + 0x96;
    box[1] = position->yLow - 0x64;
    box[2] = position->zLow - 0x96;
    if (Ovl11ProjectRecordSprite(box, screen) != 0) {
        func_80016054(gActiveRenderContext->field_120 + ((screen[2] >> 2) * 4),
                      gActiveRenderContext->field_118, (s32)source, (s32)source->field_4,
                      (u8)(s32)source->field_5, (s16)screen[0],
                      (s16)screen[1], source->field_8, 0, 0x40);
    }
    /* Animation advances even when projection culled the entity. */
    if (!(D_8006C844 & 0x08000000)) {
        UpdateSpriteAnimation(source);
    }
}
