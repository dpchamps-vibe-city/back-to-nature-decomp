#include "common.h"
#include "screen_primitives.h"
#include "sound_commands.h"
#include "random.h"

s32 ovl_11_func_800EE240(s16 arg0, s16 arg1) {
    struct Ovl11ColorTriple colors = D_800B9E38;
    s16 i;
    s32 next;
    s32 index;
    s32 index2;
    s32 blue;
    s32 green;
    s32 *packet;
    s32 *base;
    s16 noise;
    s32 slot;

    if (arg0 == 0) {
        gOvl11ValueSlots[arg1] += 5;
        switch (gOvl11ValueSlots[arg1]) {
        case 0:
            PlaySoundEffect(0x46);
            break;
        case 300:
            i = Rand(3);
            colors.values[i] = 255;
            i = Rand(3);
            colors.values[i] = 255;
            base = gOvl11ValueSlots;
            for (i = 0; i < 3; i++) {
                slot = arg1 + i + 1;
                noise = Rand(32);
                base[slot] = colors.values[i];
                base[slot] -= noise;
            }
            break;
        case 330:
            PlaySoundEffect(0x47);
            break;
        default:
            if (gOvl11ValueSlots[arg1] >= 360) {
                for (i = 0; i < 3; i++) {
                    colors.values[i] = 160;
                }
                next = arg1 + 1;
                gOvl11ValueSlots[next] = 0;
                arg0 = 1;
            }
            break;
        }
        next = arg1 + 1;
        if (gOvl11ValueSlots[next] != 0) {
            packet = (s32 *)(gActiveRenderContext->field_D8 + 0x14);
            index = arg1 + 3;
            blue = *(u8 *)&gOvl11ValueSlots[index];
            index2 = arg1 + 2;
            green = *(u16 *)&gOvl11ValueSlots[index2];
            DrawScreenFlatRectWithBlendMode(packet, 0, 0, 320, 240,
                blue | (green << 8) | (*(u8 *)&gOvl11ValueSlots[next] << 16), 1, 1);
            for (i = 1; i < 4; i++) {
                gOvl11ValueSlots[arg1 + i] -= 15;
                if (gOvl11ValueSlots[arg1 + i] < 0) {
                    gOvl11ValueSlots[arg1 + i] = 0;
                }
            }
        }
    }
    if (arg0 != 0) {
        gOvl11ValueSlots[arg1] = -((s16)Rand(80) * 5) - 200;
    }
    return 1;
}
