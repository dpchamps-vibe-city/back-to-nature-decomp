/*
 * Motion and sprite steps for the tagged-record loop's ordinary-record arm.
 * The caller controls region/liveness gates and orders motion before drawing;
 * neither entry repeats those gates. The pool remains shared, not TU-owned.
 * Contract: ovl11_tagged_record_presentation.h.
 * Original ovl_11:800DE7C8-800DE8A4. Evidence: notes/file-groupings.md.
 */
#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "ovl11_object_render.h"
#include "ovl11_tagged_record_presentation.h"
#include "ovl11_direction_vector.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "psyq/abs.h"
#include "psyq/libgs.h"
#include "psyq/libsnd.h"
#include "psyq/libapi.h"
#include "ovl11_code160_record_handlers.h"

void Ovl11DrawTaggedRecordSprite(u8 *record) {
    u16 animation;
    u16 frame;
    u16 state;

    Ovl11ReadCode160SpriteDescriptor(*(u16 *)(record + 0x24), *(u16 *)(record + 0x22), &animation, &frame);
    if (animation != *(u8 *)(record + 0x7C)) {
        SelectSpriteAnimation((ObjectState *)(record + 0x78), (u8)animation);
    }
    if (frame != 0 && frame != *(u8 *)(record + 0x7D)) {
        SelectSpriteAnimationFrame((ObjectState *)(record + 0x78), (u8)frame);
    }
    Ovl11DrawObjectWithMarker(record);
    state = *(u16 *)(record + 0x24);
    if (state == 0 || state == 5) {
        *(u16 *)(record + 0x7A) = (u16)(*(u16 *)(record + 0x7A) | 0x100);
    }
}

void Ovl11AdvanceTaggedRecordPosition(s32 recordAddress) {
    Ovl11AccumulateDirectionVector((void *)(recordAddress + 0x38),
                       (void *)(recordAddress + 0x48), 0);
}
