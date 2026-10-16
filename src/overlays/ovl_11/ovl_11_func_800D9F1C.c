#include "common.h"
#include "game_types.h"

s32 ovl_11_func_800F5888(u16 *arg0, s32 *arg1);
s32 ovl_11_func_800D9EFC(s32 arg0, s32 arg1, s32 arg2, s32 *arg3, s32 *arg4);
void func_80015EE8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5);

void ovl_11_func_800D9F1C(void) {
    s16 pos[4];
    s32 projected[4];
    s32 sprite;
    s32 texture;
    s32 y;
    s32 x;
    u16 kind;
    u8 value;
    u8 *map;

    for (y = 0; y < 7; y++) {
        for (x = 0; x < 7; x++) {
            pos[0] = x * 400 - 1250;
            pos[1] = -100;
            pos[2] = 850 - y * 400;
            if (ovl_11_func_800F5888((u16 *)pos, projected) != 0) {
                sprite = 0;
                texture = 0;
                map = (u8 *)D_8006C838;
                kind = ((Ovl11D9F1CMapView *)map)->cells[y * 7 + x].kind;
                value = ((Ovl11D9F1CMapView *)(map + (y * 7 + x) * 8))->cells[0].value;
                switch (kind) {
                case 0x3F:
                    texture = 3;
                    break;
                case 0x175:
                    sprite = 1;
                    texture = 3;
                    break;
                case 0x3A:
                    texture = 0;
                    break;
                case 0x3E:
                    texture = 2;
                    break;
                case 0x22:
                    if (!ovl_11_func_800D9EFC(value, value + 2, 4, &sprite, &texture)) continue;
                    break;
                case 0x23:
                    if (!ovl_11_func_800D9EFC(value, value + 2, 5, &sprite, &texture)) continue;
                    break;
                case 0x24:
                    if (!ovl_11_func_800D9EFC(value, value + 2, 6, &sprite, &texture)) continue;
                    break;
                case 0x25:
                    if (!ovl_11_func_800D9EFC(value, value + 2, 7, &sprite, &texture)) continue;
                    break;
                case 0x26:
                    if (!ovl_11_func_800D9EFC(value, value + 2, 8, &sprite, &texture)) continue;
                    break;
                case 0x27:
                    if (!ovl_11_func_800D9EFC(value, value + 2, 9, &sprite, &texture)) continue;
                    break;
                case 0x28:
                    if (!ovl_11_func_800D9EFC(value, value + 2, 10, &sprite, &texture)) continue;
                    break;
                case 0x29:
                    if (!ovl_11_func_800D9EFC(value, value + 2, 11, &sprite, &texture)) continue;
                    break;
                case 0x2A:
                    if (!ovl_11_func_800D9EFC(value, value + 2, 12, &sprite, &texture)) continue;
                    break;
                case 0x2B:
                    if (!ovl_11_func_800D9EFC(value, value + 2, 13, &sprite, &texture)) continue;
                    break;
                case 0x2C:
                    if (!ovl_11_func_800D9EFC(value, value + 2, 14, &sprite, &texture)) continue;
                    break;
                case 0x2D:
                    if (!ovl_11_func_800D9EFC(value, value + 2, 15, &sprite, &texture)) continue;
                    break;
                case 0x2E:
                    if (!ovl_11_func_800D9EFC(value, value + 2, 16, &sprite, &texture)) continue;
                    break;
                case 0x2F:
                    if (!ovl_11_func_800D9EFC(value, value + 2, 17, &sprite, &texture)) continue;
                    break;
                case 0x30:
                    if (!ovl_11_func_800D9EFC(value, value + 2, 18, &sprite, &texture)) continue;
                    break;
                case 0x31:
                    if (!ovl_11_func_800D9EFC(value, value + 2, 20, &sprite, &texture)) continue;
                    break;
                case 0x32:
                    if (!ovl_11_func_800D9EFC(value, value + 2, 21, &sprite, &texture)) continue;
                    break;
                case 0x178:
                    if (!ovl_11_func_800D9EFC(value, value + 2, 22, &sprite, &texture)) continue;
                    break;
                case 0x179:
                    if (!ovl_11_func_800D9EFC(value, value + 2, 27, &sprite, &texture)) continue;
                    break;
                case 0x34:
                    if (!ovl_11_func_800D9EFC(value, value + 2, 23, &sprite, &texture)) continue;
                    break;
                case 0x35:
                    if (!ovl_11_func_800D9EFC(value, value + 2, 24, &sprite, &texture)) continue;
                    break;
                default:
                    continue;
                }
                func_80015EE8(D_8005E3C0->field_120 + ((projected[2] >> 2) * 4),
                    (s32)(&D_80070400 + texture), (u8)sprite, 0,
                    (s16)projected[0], (s16)projected[1]);
            }
        }
    }
}
