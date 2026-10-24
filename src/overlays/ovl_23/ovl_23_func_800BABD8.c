#include "common.h"
#include "game_types.h"

void func_80015840(ObjectState *obj, u8 arg1);
void func_8001585C(ObjectState *obj, u8 arg1);
void func_80015EE8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5);

void ovl_23_func_800BABD8(void) {
    s16 x;
    s16 tileX64;
    s16 tileX76;
    s32 position;
    s32 screenWidth;
    s32 lowerOffset;
    s32 upperOffset;
    u8 *base;
    u8 *tiles64;
    u8 *tiles76;
    u8 *endBase;
    u8 *loopBase;
    ObjectState *sprite;
    ObjectState *sprite64;
    ObjectState *sprite76;
    ObjectState *endSprite;
    ObjectState *loopSprite;

    sprite64 = (ObjectState *)D_800BFC30;
    func_80015840(sprite64, 0);
    func_8001585C(sprite64, 0);
    tiles64 = (u8 *)sprite64 - 0x3B4;
    position = *(s32 *)(tiles64 + 0x10);
    tileX64 = position / 4096 - (position / 262144) * 64 - 64;
    if (tileX64 < HWD0 + 64) {
        do {
            s32 packet = D_8005E3C0->field_D8 + 0xD8;
            loopSprite = sprite64;
            loopBase = tiles64;
            func_80015EE8(packet, (s32)loopSprite,
                         loopBase[0x3B8], loopBase[0x3B9], tileX64, 0);
            tileX64 += 64;
        } while (tileX64 < HWD0 + 64);
    }
    sprite76 = (ObjectState *)D_800BFC30;
    func_80015840(sprite76, 1);
    tiles76 = (u8 *)sprite76 - 0x3B4;
    tileX76 = (*(s32 *)(tiles76 + 0x10) / 4096 * 3) -
        (*(s32 *)(tiles76 + 0x10) / 4096 * 3 / 76) * 76 - 76;
    if (tileX76 < HWD0 + 76) {
        do {
            s32 packet = D_8005E3C0->field_D8 + 0xCC;
            loopSprite = sprite76;
            loopBase = tiles76;
            func_80015EE8(packet, (s32)loopSprite,
                         loopBase[0x3B8], loopBase[0x3B9], tileX76, 0);
            tileX76 += 76;
        } while (tileX76 < HWD0 + 76);
    }
    base = Ovl23_D_800BF87C_draw;
    screenWidth = HWD0;
    if (-screenWidth / 2 - 32 < *(s32 *)(base + 0x10) / 4096 &&
        *(s32 *)(base + 0x10) / 4096 < screenWidth / 2 + 32) {
        sprite = (ObjectState *)(base + 0x3B4);
        func_80015840(sprite, 0);
        func_8001585C(sprite, 1);
        x = HWD0 / 2 + *(s32 *)(base + 0x10) / 4096;
        func_80015EE8(D_8005E3C0->field_D8 + 0xD4, (s32)sprite,
                     base[0x3B8], base[0x3B9], x - 32, 0);
        func_80015840(sprite, 2);
        x = HWD0 / 2 + *(s32 *)(base + 0x10) / 4096 + 64;
        func_80015EE8(D_8005E3C0->field_D8 + 0xD4, (s32)sprite,
                     base[0x3B8], base[0x3B9], x - 32, 0);
    }
    endBase = Ovl23_D_800BF87C_draw;
    screenWidth = HWD0;
    if (-screenWidth / 2 + (lowerOffset = ((Ovl23D87CView39C *)endBase)->unk39C - 32) <
        *(s32 *)(endBase + 0x10) / 4096 &&
        *(s32 *)(endBase + 0x10) / 4096 <
        screenWidth / 2 + (upperOffset = ((Ovl23D87CView39C *)endBase)->unk39C + 32)) {
        endSprite = (ObjectState *)(endBase + 0x3B4);
        func_80015840(endSprite, 0);
        func_8001585C(endSprite, 1);
        x = HWD0 / 2 + *(s32 *)(endBase + 0x10) / 4096 -
            ((Ovl23D87CView39C *)endBase)->unk39C;
        func_80015EE8(D_8005E3C0->field_D8 + 0xD4, (s32)endSprite,
                     endBase[0x3B8], endBase[0x3B9], x - 32, 0);
    }
}
