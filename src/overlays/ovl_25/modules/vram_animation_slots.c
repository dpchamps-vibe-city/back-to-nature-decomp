/*
 * Eight VRAM animation slots shared with the executable and other overlays.
 * Descriptors live at asset-header+0x8A8; key bit 0x8000 selects sequence
 * playback, otherwise the image scrolls vertically in two-pixel steps.
 * Source/destination X and width are VRAM words, Y and height are scanlines.
 *
 * This overlay's six-entry unit is 800BB4B4..800BB970. It uses the same
 * 0x16-byte slots and 0x12-byte descriptors as ovl_11, not a new copy of
 * the state. The duplicate local struct views have been reconciled to those
 * canonical layouts; duration/destination signedness is preserved.
 * Membership and conservative boundary evidence: notes/file-groupings.md.
 */

#include "common.h"
#include "psyq/memory.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "ovl25_vram_animation_slots.h"

void Ovl25InitVramAnimationSlots(void) {
    char *work;
    u32 index;

    /* The primary storage is zeroed, not marked free. The eight free-key
     * sentinels go into the separate work-area array. */
    memset(&gVramAnimationStoragePrefix, 0, 0xB4);
    work = (char *)&D_8007AFF0;
    for (index = 0; index < 8; index++) {
        *(u16 *)(work + 0x253C0 + index * 0x16) = 0xFFFF;
    }
}

void Ovl25AcquireVramAnimationSlot(Ovl11VramAnimationDescriptor *descriptor);

void Ovl25RequestVramAnimations(s32 tableAddress) {
    Ovl11VramAnimationDescriptor *descriptor;
    Ovl11VramAnimationDescriptor *request;
    u32 index;
    u32 emptyKey;

    index = 0;
    emptyKey = 0xFFFF;
    descriptor = (Ovl11VramAnimationDescriptor *)(tableAddress + 0x8A8);
    request = descriptor;
    do {
        if (descriptor->key != emptyKey) {
            Ovl25AcquireVramAnimationSlot(request);
        }
        request += 1;
        index += 1;
        descriptor += 1;
    } while (index < 8U);
}

void Ovl25StepVramAnimationSequence(Ovl11AnimationSlot *arg0);
void Ovl25StepVramAnimationScroll(Ovl11AnimationSlot *arg0);

void Ovl25UpdateVramAnimations(void) {
    s32 index;
    Ovl11AnimationSlot *slot;
    u16 key;

    slot = (Ovl11AnimationSlot *) &gVramAnimationSlotHead;
    for (index = 0; index < 8; index++, slot++) {
        key = slot->key;
        if ((key != 0xFFFF) && !(((struct struct_8006C838_view *) D_8006C838)->field_0C & 0x08000000)) {
            if (key & 0x8000) {
                Ovl25StepVramAnimationSequence(slot);
            } else {
                Ovl25StepVramAnimationScroll(slot);
            }
        }
    }
}

void Ovl25StepVramAnimationSequence(Ovl11AnimationSlot *slot) {
    RECT source;
    s32 tableAddress;
    s32 descriptorOffset;
    Ovl11VramAnimationDescriptor *descriptor;
    char *work;
    char *reloadWork;

    source.x = slot->sourceX;
    source.y = slot->sourceY;
    source.w = slot->width;
    source.h = slot->height;
    MoveImage(&source, slot->destX, slot->destY);
    work = (char *)&D_8007AFF0;
    /* Mode 0x2C tiles flagged images into two additional word-X positions. */
    if ((*(s16 *)(work + 0x25476) == 0x2C) && (slot->key & 1)) {
        MoveImage(&source, slot->destX + 8, slot->destY);
        MoveImage(&source, slot->destX + 0x10, slot->destY);
    }
    slot->step += 1;
    if (slot->step >= slot->duration) {
        reloadWork = (char *)&D_8007AFF0;
        tableAddress = *(s32 *)(reloadWork + 0x25388);
        descriptorOffset = slot->nextOrDelay * 0x12 + 0x8A8;
        descriptor = (Ovl11VramAnimationDescriptor *)(tableAddress + descriptorOffset);
        slot->duration = descriptor->duration;
        slot->nextOrDelay = descriptor->nextIndex;
        slot->destX = descriptor->destX;
        slot->destY = descriptor->destY;
        slot->sourceX = descriptor->sourceX;
        slot->sourceY = descriptor->sourceY;
        slot->width = descriptor->width;
        slot->height = descriptor->height;
        slot->step = 0;
    }
}

void Ovl25StepVramAnimationScroll(Ovl11AnimationSlot *slot) {
    RECT source;
    u16 step;

    source.x = slot->sourceX;
    source.y = slot->sourceY;
    source.w = slot->width;
    source.h = slot->height - slot->step;
    MoveImage(&source, slot->destX, slot->destY + slot->step);
    step = slot->step;
    if (step != 0) {
        source.h = step;
        source.x = slot->sourceX;
        source.y = (slot->sourceY + slot->height) - step;
        source.w = slot->width;
        MoveImage(&source, slot->destX, slot->destY);
    }
    if (slot->scrollTick == slot->nextOrDelay) {
        slot->step += 2;
        slot->scrollTick = 0;
        if (slot->step >= (s16) slot->height) {
            slot->step = 0;
        }
    } else {
        slot->scrollTick += 1;
    }
}

/* Duplicate keys and a full pool are silent no-ops. */
void Ovl25AcquireVramAnimationSlot(Ovl11VramAnimationDescriptor *descriptor) {
    s32 index;
    Ovl11AnimationSlot *slot;

    slot = (Ovl11AnimationSlot *)&gVramAnimationSlotHead;
    for (index = 0; index < 8; index++, slot++) {
        if (descriptor->key == slot->key) {
            return;
        }
    }
    slot = (Ovl11AnimationSlot *)&gVramAnimationSlotHead;
    for (index = 0; index < 8; index++, slot++) {
        if (slot->key == 0xFFFF) {
            memset(slot, 0, 0x16);
            slot->key = descriptor->key;
            slot->duration = descriptor->duration;
            slot->nextOrDelay = descriptor->nextIndex;
            slot->destX = descriptor->destX;
            slot->destY = descriptor->destY;
            slot->sourceX = descriptor->sourceX;
            slot->sourceY = descriptor->sourceY;
            slot->width = descriptor->width;
            slot->height = descriptor->height;
            return;
        }
    }
}
