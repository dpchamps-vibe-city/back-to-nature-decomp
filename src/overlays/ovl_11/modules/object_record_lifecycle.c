/* State-object allocation, position advance and sprite submission.
 * The six resident 0xB0 records are shared with reset, collision and scan
 * services; this unit neither defines their storage nor owns their updater.
 * Membership and evidence-limited edges: notes/file-groupings.md. */

#include "ovl11_object_record_lifecycle.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "ovl11_object_render.h"
#include "ovl11_object_record_init.h"
#include "ovl11_object_state_dispatch.h"
#include "ovl11_direction_vector.h"

s32 ovl_11_func_800D3424(u16 state, u16 variant, u16 *animation, u16 *frame);

void Ovl11DrawStateObjectRecord(u8 *record) {
    u16 animation;
    u16 frame;

    ovl_11_func_800D3424(*(u16 *)(record + 0x24), *(u16 *)(record + 0x22),
                        &animation, &frame);
    if (animation != record[0x7C]) {
        SelectSpriteAnimation((ObjectState *)(record + 0x78), (u8)animation);
    }
    if (frame != 0 && frame != record[0x7D]) {
        SelectSpriteAnimationFrame((ObjectState *)(record + 0x78), (u8)frame);
    }
    SetSpriteAnimationUpdateLatch(0);
    Ovl11DrawObjectWithMarker(record);
    if (*(u16 *)(record + 0x24) == 0) {
        *(u16 *)(record + 0x7A) |= 0x100;
    }
}

void Ovl11AdvanceStateObjectPosition(s32 recordAddress) {
    Ovl11AccumulateDirectionVector(recordAddress + 0x38, recordAddress + 0x48, 0);
}

/* The destination begins at +1A, not a word boundary. Packed assignment
 * retains the original unaligned pair copy rather than assuming alignment. */
typedef struct __attribute__((packed)) {
    s32 first;
    s32 second;
} Ovl11UnalignedStatePair;

Ovl11ObjectInitRecord *Ovl11AllocateStateObjectRecord(s32 id) {
    UnkStruct80075BC4 *record;
    UnkStruct80075BC4 *scan;
    UnkStruct80075BC4 *next;
    s32 slot;
    char *work;
    char *farState;
    u16 selector;

    slot = 0;
    record = gStateObjectPool;
    if (gStateObjectPool[0].unk0 != 0) {
        scan = gStateObjectPool;
        next = gStateObjectPool;
scan_next:
        scan += 1;
        slot += 1;
        next += 1;
        if (slot < 6) {
            record = next;
            if (scan->unk0 != 0) {
                goto scan_next;
            }
        }
    }
    if (slot == 6) {
        return 0;
    }
    Ovl11InitObjectRecord((Ovl11ObjectInitRecord *)record, (s16) id);
    work = (char *)&D_8006C838;
    *(Ovl11UnalignedStatePair *)((u8 *)record + 0x1A) =
        *(Ovl11UnalignedStatePair *)(work + 0x44B8);
    farState = (char *)&D_8007AFF0;
    selector = *(u16 *)(farState + 0x25476);
    *(s16 *)((u8 *)record + 0xAC) = 0xFF;
    *(u16 *)((u8 *)record + 0x30) = selector;
    if (record->unk0 == 0x157) {
        Ovl11EnterObjectState((u16 *)record, 4);
    }
    if (record->unk0 == 0x15B) {
        Ovl11EnterObjectState((u16 *)record, 0x12);
    }
    return (Ovl11ObjectInitRecord *)record;
}
