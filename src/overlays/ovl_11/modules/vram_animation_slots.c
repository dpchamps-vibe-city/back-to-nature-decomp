/* Eight VRAM rectangle-animation slots: descriptor sequences or wrapping
 * vertical scroll. State is shared engine RAM, not overlay-owned allocation.
 * Public contracts: ovl11_vram_animation_slots.h. Membership: file-groupings.md. */
#include "common.h"
#include "game_types.h"
#include "psyq/memory.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "game_flags.h"
#include "ovl11_vram_animation_slots.h"

void Ovl11InitVramAnimationSlots(void) {
    char *work;
    u32 index;

    memset(&gVramAnimationStoragePrefix, 0, 0xB4);
    work = (char *)&D_8007AFF0;
    for (index = 0; index < 8; index++) {
        *(u16 *)(work + 0x253C0 + index * 0x16) = 0xFFFF;
    }
}

void Ovl11AcquireVramAnimationSlot(Ovl11VramAnimationDescriptor *descriptor) {
    s32 index;
    Ovl11AnimationSlot *slot;

    slot = (Ovl11AnimationSlot *) &gVramAnimationSlotHead;
    for (index = 0; index < 8; index++, slot++) {
        if (descriptor->key == slot->key) {
            return;
        }
    }
    slot = (Ovl11AnimationSlot *) &gVramAnimationSlotHead;
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

/* Retained unused entry. Deliberately inspect the same head eight times:
 * the original never advances the pointer. Do not repair it into a scan. */
void Ovl11ReleaseVramAnimationSlot(s16 key) {
    u16 *head;
    s32 index;

    head = &gVramAnimationSlotHead;
    index = 0;
    do {
        if (*head == key) {
            memset(head, 0, 0x16);
            *head = 0xFFFF;
            break;
        }
        index++;
    } while (index < 8);
}

void Ovl11RequestVramAnimations(Ovl11SetupHeader *header) {
    Ovl11VramAnimationDescriptor *entry;
    Ovl11VramAnimationDescriptor *request;
    u32 index;
    char *base;
    u16 sentinel;

    if (header->unk2 == 0xF) {
        base = (char *) &D_8006C838;
        base += 0x8000;
        if ((u32) ((*(u16 *) (base + 0x64C8)) - 1) < 2U) {
            return;
        }
    }
    index = 0;
    sentinel = 0xFFFF;
    entry = (Ovl11VramAnimationDescriptor *) ((u8 *) header + 0x8A8);
    request = entry;
    do {
        if (entry->key != sentinel) {
            Ovl11AcquireVramAnimationSlot(request);
        }
        request += 1;
        index += 1;
        entry += 1;
    } while (index < 8U);
}

void Ovl11StepVramAnimationSequence(Ovl11AnimationSlot *slot) {
    RECT rect;
    Ovl11VramAnimationTable *table;
    char *work;
    char *currentWork;

    work = (char *)&D_8007AFF0;
    table = *(Ovl11VramAnimationTable **)(work + 0x25388);
    if ((table->kind != 0x5B) || (GetGameFlag(0x45U) != 0)) {
        rect.x = slot->sourceX;
        rect.y = slot->sourceY;
        rect.w = slot->width;
        rect.h = slot->height;
        MoveImage(&rect, slot->destX, slot->destY);
        if ((*(s16 *)(work + 0x25476) == 0x2C) && (slot->key & 1)) {
            MoveImage(&rect, slot->destX + 8, slot->destY);
            MoveImage(&rect, slot->destX + 0x10, slot->destY);
        }
        slot->step = slot->step + 1;
        if (slot->step >= slot->duration) {
            Ovl11VramAnimationDescriptor *next;

            currentWork = (char *)&D_8007AFF0;
            next = &(*(Ovl11VramAnimationTable **)(currentWork + 0x25388))->entries[slot->nextOrDelay];
            slot->duration = next->duration;
            slot->nextOrDelay = next->nextIndex;
            slot->destX = next->destX;
            slot->destY = next->destY;
            slot->sourceX = next->sourceX;
            slot->sourceY = next->sourceY;
            slot->width = next->width;
            slot->height = next->height;
            slot->step = 0;
        }
    }
}

void Ovl11StepVramAnimationScroll(Ovl11AnimationSlot *slot) {
    RECT rect;
    u16 rowOffset;
    u16 tick;

    rect.x = slot->sourceX;
    rect.y = slot->sourceY;
    rect.w = slot->width;
    rect.h = slot->height - slot->step;
    MoveImage(&rect, (s32) slot->destX, slot->destY + slot->step);
    rowOffset = slot->step;
    if (rowOffset != 0) {
        rect.h = rowOffset;
        rect.x = slot->sourceX;
        rect.y = (slot->sourceY + slot->height) - rowOffset;
        rect.w = slot->width;
        MoveImage(&rect, (s32) slot->destX, (s32) slot->destY);
    }
    tick = slot->scrollTick;
    if (tick == slot->nextOrDelay) {
        slot->step = slot->step + 2;
        slot->scrollTick = 0;
        if (slot->step >= (s16) slot->height) {
            slot->step = 0;
        }
    } else {
        slot->scrollTick = tick + 1;
    }
}

void Ovl11UpdateVramAnimations(void) {
    Ovl11AnimationSlot *slot;
    s32 index;
    u16 key;

    slot = (Ovl11AnimationSlot *) &gVramAnimationSlotHead;
    for (index = 0; index < 8; index++, slot++) {
        key = slot->key;
        if ((key != 0xFFFF) && !(((struct struct_8006C838_view *) D_8006C838)->field_0C & 0x08000000)) {
            if (key & 0x8000) {
                Ovl11StepVramAnimationSequence(slot);
            } else {
                Ovl11StepVramAnimationScroll(slot);
            }
        }
    }
}
