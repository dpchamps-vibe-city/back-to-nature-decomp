/*
 * primary_object_input_override — reconstructed translation unit.
 *
 * Begin a timed primary-object direction override, then replace held/pressed
 * masks from its saved direction while active.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "ovl11_primary_object_input_override.h"
#include "game_types.h"
#include "ovl11_primary_object_state_change.h"

void Ovl11BeginPrimaryObjectInputOverride(s16 ticks) {
    u8 *base = (u8 *)&gPrimaryObjectBase;

    if (*(s32 *)(base + 0x6C) & 0x40000000) {
        return;
    }
    if (*(s32 *)(base + 0x6C) & 8) {
        return;
    }
    *(s16 *)(base + 0x2E) = ticks;
    *(s32 *)(base + 0x6C) |= 0xA;
    *(u16 *)(base + 0x30) = *(u16 *)(base + 0x38);
    Ovl11ChangePrimaryObjectState((Ovl11C7010Object *)base, 2,
        *(u16 *)(base + 0x38) + 1, -2);
    *(s32 *)(base + 0x6C) &= ~0x20000000;
}

void Ovl11ApplyPrimaryObjectInputOverride(void *object) {
    if (*(u32 *)((u8 *)object + 0x6C) & 8) {
        gOvl11PrimaryPressedButtons = 0;
        gOvl11PrimaryHeldButtons = gOvl11ForcedDirectionButtons[*(s16 *)((u8 *)object + 0x30)];
    }
}
