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

s32 ovl_11_func_800DF3BC(u16 state, u16 variant, u16 *animation, u16 *frame);
void ovl_11_func_800D03B4(void *position, void *delta, s32 keepDelta);

void Ovl11DrawTaggedRecordSprite(u8 *record) {
    u16 animation;
    u16 frame;
    u16 state;

    ovl_11_func_800DF3BC(*(u16 *)(record + 0x24), *(u16 *)(record + 0x22), &animation, &frame);
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
    ovl_11_func_800D03B4((void *)(recordAddress + 0x38),
                       (void *)(recordAddress + 0x48), 0);
}
