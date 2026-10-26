/* Primary-object frame submission and its companion/saved-record variants.
 * The frame path projects into shared scratch, orders the two sprite parts,
 * then selects an indirect saved record or the default holder. Animation
 * synchronization is serviced separately by the object's state handler.
 * Membership and evidence-limited boundaries: notes/file-groupings.md. */

#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_render.h"
#include "sprite_source.h"
#include "point_projection.h"
#include "ovl11_timed_sprite_effect.h"
#include "ovl11_object_render.h"
#include "ovl11_primary_object_draw.h"

void ovl_11_func_800D6F78(s16 arg0, s16 arg1, s32 *arg2, s16 arg3, s16 arg4, s32 arg5);

void Ovl11DrawPrimaryObjectCompanion(Ovl11C7010Object *object, s32 depth, s32 *screenPosition) {
    s16 spriteId;
    s32 mode;
    s32 four;
    s32 flags;
    s32 modeCopy;
    s32 direction;
    s32 offsetGroup;
    s32 selector;
    u16 *offset;

    flags = object->unk6C;
    if (flags & 0x4000) {
        return;
    }
    if (flags & 0x8000) {
        spriteId = object->itemIndex;
        offsetGroup = 0;
        if (!(flags & 0x18000000)) {
            spriteId = *(s16 *) ((u8 *) object + 0xF0);
        }
        if (flags & 0x10000000) {
            offsetGroup = 8;
        }
        selector = object->unk3C;
        four = 4;
        mode = selector & 0xFFFF;
        modeCopy = selector;
        selector = four;
        if (mode == selector) {
            offsetGroup = 0x10;
        }
        if ((mode == 3) || (mode == 0x31)) {
            offsetGroup = 0x18;
        }
        direction = object->unk38;
        if (((u16) modeCopy == 0x15) || ((u16) modeCopy == 0x32)) {
            direction = 0;
        }
        offset = &gOvl11CompanionScreenOffsets[direction * 2 + offsetGroup];
        ovl_11_func_800D6F78(spriteId, 0, (s32 *) (gActiveRenderContext->field_120 + ((depth >> 2) * 4)), (s16) ((*(u16 *) ((u8 *) screenPosition + 0)) + offset[0]), (s16) ((*(u16 *) ((u8 *) screenPosition + 4)) + offset[1]), 2);
    }
}

POLY_FT4 *BuildSpriteEntryPackets(s32 *ot, POLY_FT4 *packets,
                       SpriteEntrySourceView *source, s16 x, s16 y,
                       u16 flags, s32 total, s32 textureBase,
                       s16 substitute, s16 substituteFrom, s16 substituteTo);

void func_80016054(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 arg4, s16 arg5, s16 arg6, s32 arg7, s32 arg8, u16 arg9);

void Ovl11DrawPrimaryObjectSprite(Recon_ovl_11_func_800CD08C_A0View *arg0, s32 arg1, s32 *arg2) {
    SpriteSourceData *temp_s0;
    s16 var_t2;
    s16 var_t4;
    s16 var_t5;
    s32 temp_a3;
    s32 temp_v1;

    temp_a3 = gActiveRenderContext->field_120 + ((arg1 >> 2) * 4);
    if (arg0->unk6C & 0x100000) {
        temp_s0 = (SpriteSourceData *) ((u8 *) arg0 + 0x290);
        func_80016054(temp_a3, gActiveRenderContext->field_118, (s32) temp_s0, (s32) (*(u8 *) ((u8 *) arg0 + 0x294)), (u8) (s32) (*(u8 *) ((u8 *) arg0 + 0x295)), (s16) (s32) (*(s16 *) ((u8 *) arg2 + 0)), (s16) (s32) (*(s16 *) ((u8 *) arg2 + 4)), *(s32 *) ((u8 *) arg0 + 0x298), 0, 0x40U);
        UpdateSpriteAnimation(temp_s0);
        return;
    }
    var_t2 = -1;
    var_t4 = -1;
    temp_v1 = *(u16 *) ((u8 *) arg0 + 0x3C);
    var_t5 = -1;
    if (((u16) temp_v1 == 0x17) || ((u16) temp_v1 == 0x19) || ((u16) temp_v1 == 0x1B) || ((u16) temp_v1 == 0x1D) || ((u16) temp_v1 == 0x1F)) {
        if ((u32) ((*(u8 *) ((u8 *) arg0 + 0x264)) - 0x48) < 0x64U) {
            if (temp_v1 == 0x1F) {
                temp_v1 = (s32) gOvl11Mode1FSpriteSubstituteIds;
                var_t2 = ((s16 *) temp_v1)[*(s16 *) ((u8 *) arg0 + 0x86)];
            } else {
                var_t2 = gOvl11PrimarySpriteSubstituteIds[*(s16 *) ((u8 *) arg0 + 0x86)];
                var_t5 = gOvl11PrimarySpriteSubstituteValues[*(s16 *) ((u8 *) arg0 + 0x86)];
                temp_v1 = (s16) (*(u16 *) ((u8 *) arg0 + 0x18)) / 20;
                var_t4 = gOvl11PrimarySpriteSubstituteValues[(s16) temp_v1];
            }
        }
    }
    BuildSpriteEntryPackets((s32 *) temp_a3, (POLY_FT4 *) gActiveRenderContext->field_118, (SpriteSourceData *) ((u8 *) arg0 + 0x260), *(s16 *) ((u8 *) arg2 + 0), (s16) (s32) (*(s16 *) ((u8 *) arg2 + 4)), 0x40U, D_8005E3BC, (s32) ((void *) ((u8 *) &gOvl11PrimarySpriteTextureBanks + (gActiveRenderContextIndex * 0x78))), (s16) (s32) var_t2, (s16) (s32) var_t4, (s16) (s32) var_t5);
    if (!(((struct struct_8006C838_view *) D_8006C838)->field_0C & 0x08000000)) {
        UpdateSpriteAnimation((SpriteSourceData *) ((u8 *) D_8006C838 + 0x5428));
    }
}

s32 ovl_11_func_800DD1D0 (s16 *arg0);
u32 ovl_11_func_800CDEEC (u8 *arg0);
void ovl_11_func_800C9E90 (s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void Ovl11CopyAndDrawSavedObject (Ovl11DrawState *arg0);
void ovl_11_func_8010C1C0 (Ovl11DrawState *arg0);
void ovl_11_func_8010C2EC (u8 *arg0);
u32 ovl_11_func_800CDFAC (u8 *arg0);

/* The area-frame caller supplies a trailing 1 in a1; it is unread here. */
void Ovl11DrawPrimaryObjectFrame(Ovl11SpritePositionView *arg0, s32 unused) {
    s32 var_a0;
    s32 var_s3;
    u8 *object;

    object = (u8 *) &gPrimaryObjectBase;

    if (ovl_11_func_800DD1D0((s16 *) (object + 0xFC)) == 0) {
        gOvl11PrimaryDrawWorldPoint[0] = arg0->field_0 + 0x96;
        gOvl11PrimaryDrawWorldPoint[1] = arg0->field_4 - 0x64;
        gOvl11PrimaryDrawWorldPoint[2] = arg0->field_8 - 0x96;
        var_s3 = ProjectPointToScreen(gOvl11PrimaryDrawScreenPoint, (SVECTOR *) gOvl11PrimaryDrawWorldPoint);
        if ((*(s16 *) (object + 0x32)) != 0) {
            ovl_11_func_800CDEEC(object);
        }
        if (var_s3 != 0) {
            if ((*(u16 *) (object + 0x36)) & 4) {
                DrawSpriteFrameRects(gActiveRenderContext->field_120 + 0xA0, (s32) &gSharedSpriteSource, 0xE, ((u32) (*(u32 *) (object + -0x51C0)) >> 2) & 1, (s16) (s32) (*(s16 *) ((u8 *) gOvl11PrimaryDrawScreenPoint + 0)), (s16) (s32) (s16) ((*(u16 *) ((u8 *) gOvl11PrimaryDrawScreenPoint + 4)) - 0x32));
            }
            Ovl11DrawTimedSpriteEffect((s16 *) (object + 0x2CC), gActiveRenderContext->field_120 + ((var_s3 >> 2) * 4), *(s16 *) ((u8 *) gOvl11PrimaryDrawScreenPoint + 0), (s16) ((*(u16 *) ((u8 *) gOvl11PrimaryDrawScreenPoint + 4)) - 0x28));
            if ((*(u16 *) (object + 0x38) == 0) ||
                (*(u16 *) (object + 0x38) == 3) ||
                (*(s32 *) (object + 0x6C) & 0x10000000) ||
                (*(u16 *) (object + 0x3C) == 0x15) ||
                (*(u16 *) (object + 0x3C) == 4) ||
                (*(u16 *) (object + 0x3C) == 0x32) ||
                (*(u8 *) (object + 0x264) == 5)) {
                Ovl11DrawPrimaryObjectCompanion((Ovl11C7010Object *) object, var_s3, gOvl11PrimaryDrawScreenPoint);
                Ovl11DrawPrimaryObjectSprite((Recon_ovl_11_func_800CD08C_A0View *) object, var_s3, gOvl11PrimaryDrawScreenPoint);
            } else {
                Ovl11DrawPrimaryObjectSprite((Recon_ovl_11_func_800CD08C_A0View *) object, var_s3, gOvl11PrimaryDrawScreenPoint);
                Ovl11DrawPrimaryObjectCompanion((Ovl11C7010Object *) object, var_s3, gOvl11PrimaryDrawScreenPoint);
            }
            var_a0 = 0;
            if ((*(u16 *) (object + 0x3C)) == 0x4A) {
                var_a0 = (*(u8 *) (object + 0x264)) == 1;
            }
            if ((*(s32 *) (object + 0x6C)) & 0x40000000) {
                var_a0 = 1;
            }
            if (var_a0 != 0) {
                gOvl11PrimaryDrawWorldPoint[0] = arg0->field_0 + 0x96;
                gOvl11PrimaryDrawWorldPoint[1] = -0x64;
                gOvl11PrimaryDrawWorldPoint[2] = arg0->field_8 - 0x96;
                var_s3 = ProjectPointToScreen(gOvl11PrimaryDrawScreenPoint, (SVECTOR *) gOvl11PrimaryDrawWorldPoint);
                if (var_s3 == 0) {
                    return;
                }
            }
            {
                if (!(*(s32 *) (object + 0x6C) & 0x100)) {
                    ovl_11_func_800C9E90(gActiveRenderContext->field_120 + ((var_s3 >> 2) * 4),
                        gOvl11PrimaryDrawScreenPoint[0], gOvl11PrimaryDrawScreenPoint[1], 0);
                }
                if (*(s32 *) (object + 0x6C) & 0x100) {
                    if ((*(s32 *) (object + 0x6C)) & 0x80) {
                        Ovl11CopyAndDrawSavedObject((Ovl11DrawState *) object);
                    } else {
                        ovl_11_func_8010C1C0((Ovl11DrawState *) object);
                        ovl_11_func_8010C2EC((u8 *) &D_80075AD4);
                    }
                }
                if ((D_80123154 != 0) && (*((s16 *) ((u8 *) &D_80125F24 + ((*(s16 *) (object + 0xFE)) * 2))) == 0)) {
                    ovl_11_func_800CDFAC(object);
                }
            }
        }
    }
}

void Ovl11CopyAndDrawSavedObject(Ovl11DrawState *drawState) {
    u8 *saved;

    saved = Ovl11SavedInteractionObject;
    *(s32 *) (saved + 0x38) = drawState->unk100;
    *(s32 *) (saved + 0x3C) = drawState->unk104;
    *(s32 *) (saved + 0x40) = drawState->unk108;
    *(s32 *) (saved + 0x44) = drawState->unk10C;
    Ovl11DrawObjectWithMarker(saved);
}

void Ovl11SyncSavedObjectAnimation(Ovl11C7010Object *object) {
    ObjectState *spriteState;
    u8 *saved;

    saved = Ovl11SavedInteractionObject;
    /* Direction is stored as a halfword but animation consumes its low byte. */
    *(u16 *) (saved + 0x22) = object->unk38;
    if (object->unk3C == 1) {
        SelectSpriteAnimation((ObjectState *) ((u8 *) saved + 0x78), 0);
    }
    spriteState = (ObjectState *) ((u8 *) saved + 0x78);
    if (object->unk3C == 2) {
        SelectSpriteAnimation(spriteState, (saved[0x22] + 1) & 0xFF);
    }
    if (object->unk3C == 4) {
        SelectSpriteAnimation(spriteState, (saved[0x22] + 1) & 0xFF);
    }
    if (object->sprite.field_4 == (object->unk38 + 0x3F)) {
        SelectSpriteAnimation(spriteState, (saved[0x22] + 1) & 0xFF);
        return;
    }
    if (GetSpriteFrameControlHighBit((SpriteSourceData *) spriteState) != 0) {
        saved[0x7D] = saved[0x22];
    }
}
