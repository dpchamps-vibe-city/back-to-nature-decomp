/*
 * Four-slot additive fullscreen flash sequencer. The slots hold one timer
 * and three RGB words; only the red word gates drawing and color decay.
 * The private RGB seed is copied, never modified, on every call.
 */

#include "common.h"
#include "screen_primitives.h"
#include "sound_commands.h"
#include "random.h"
#include "ovl11_color_flash_command.h"

s32 Ovl11UpdateColorFlashCommand(s16 resetDelay, s16 firstSlot) {
    struct Ovl11ColorTriple colors = gOvl11ColorFlashBaseRGB;
    s16 channel;
    s32 redSlot;
    s32 blueSlot;
    s32 greenSlot;
    s32 blue;
    s32 green;
    s32 *ot;
    s32 *slots;
    s16 noise;
    s32 slot;

    if (resetDelay == 0) {
        gOvl11ValueSlots[firstSlot] += 5;
        switch (gOvl11ValueSlots[firstSlot]) {
        case 0:
            PlaySoundEffect(0x46);
            break;
        case 300:
            channel = Rand(3);
            colors.values[channel] = 255;
            channel = Rand(3);
            colors.values[channel] = 255;
            slots = gOvl11ValueSlots;
            for (channel = 0; channel < 3; channel++) {
                slot = firstSlot + channel + 1;
                noise = Rand(32);
                slots[slot] = colors.values[channel];
                slots[slot] -= noise;
            }
            break;
        case 330:
            PlaySoundEffect(0x47);
            break;
        default:
            if (gOvl11ValueSlots[firstSlot] >= 360) {
                for (channel = 0; channel < 3; channel++) {
                    colors.values[channel] = 160;
                }
                redSlot = firstSlot + 1;
                gOvl11ValueSlots[redSlot] = 0;
                resetDelay = 1;
            }
            break;
        }
        redSlot = firstSlot + 1;
        if (gOvl11ValueSlots[redSlot] != 0) {
            ot = (s32 *)(gActiveRenderContext->field_D8 + 0x14);
            blueSlot = firstSlot + 3;
            blue = *(u8 *)&gOvl11ValueSlots[blueSlot];
            greenSlot = firstSlot + 2;
            green = *(u16 *)&gOvl11ValueSlots[greenSlot];
            DrawScreenFlatRectWithBlendMode(ot, 0, 0, 320, 240,
                blue | (green << 8) | (*(u8 *)&gOvl11ValueSlots[redSlot] << 16), 1, 1);
            for (channel = 1; channel < 4; channel++) {
                gOvl11ValueSlots[firstSlot + channel] -= 15;
                if (gOvl11ValueSlots[firstSlot + channel] < 0) {
                    gOvl11ValueSlots[firstSlot + channel] = 0;
                }
            }
        }
    }
    if (resetDelay != 0) {
        gOvl11ValueSlots[firstSlot] = -((s16)Rand(80) * 5) - 200;
    }
    return 1;
}
