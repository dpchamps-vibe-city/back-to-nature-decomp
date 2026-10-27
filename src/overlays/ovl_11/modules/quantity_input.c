/* Pad-driven quantity editing, independent of prompt rendering and storage.
 * The caller supplies the live quantity. Directional repeat takes priority
 * over button actions; wrapping still runs on confirmation/cancellation.
 * Membership and evidence-limited boundaries: notes/file-groupings.md. */
#include "common.h"
#include "game_types.h"
#include "sound_commands.h"
#include "ovl11_quantity_input.h"

s32 Ovl11ProcessQuantityInput(s16 limit, u16 *quantity) {
    s32 repeated;
    s32 pressed;
    s32 action;
    u16 adjusted;

    action = 0;
    repeated = gPad0Input->repeatButtons;
    pressed = gPad0Input->pressedButtons;
    if (repeated & 0x2000) {
        adjusted = *quantity + 1;
        *quantity = adjusted;
        PlaySoundEffect(5);
    } else if (repeated & 0x8000) {
        adjusted = *quantity - 1;
        *quantity = adjusted;
        PlaySoundEffect(5);
    } else if (repeated & 0x1000) {
        adjusted = *quantity + 10;
        *quantity = adjusted;
        PlaySoundEffect(5);
    } else if (repeated & 0x4000) {
        adjusted = *quantity - 10;
        *quantity = adjusted;
        PlaySoundEffect(5);
    } else if (pressed & 0x40) {
        action = 1;
        PlaySoundEffect(0);
    } else if (pressed & 0x10) {
        action = 2;
        PlaySoundEffect(0);
    } else if (pressed & 0x20) {
        action = 3;
        PlaySoundEffect(1);
    }
    /* Halfword truncation and signed tests are intentional: an underflow
     * wraps to the upper limit, rather than saturating at one. */
    if (limit < (s16) *quantity) {
        *quantity = 1;
    }
    if ((s16) *quantity <= 0) {
        *quantity = (u16) limit;
    }
    return action;
}
