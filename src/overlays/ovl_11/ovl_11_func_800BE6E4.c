#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"

s32 ovl_11_func_8011E090(VECTOR *, u16 *, u16 *);
s32 ovl_11_func_800D7EF8(s32 *, u16 *, u16 *);

s32 ovl_11_func_800BE6E4(s32 **arg0, s32 arg1) {
    VECTOR pos;
    u16 coords[2];
    s16 row;
    s16 col;
    s32 rowOffset;
    u8 *columnBase;
    u8 *mapBase;
    s16 temp_a0_2;
    s16 temp_t3;
    s32 *temp_a0;
    s32 *temp_v0_2;
    s32 *temp_v1_2;
    s32 temp_a3;
    s32 var_s0;
    s32 var_v0;
    u16 temp_a0_3;
    u16 temp_a1;
    u16 temp_v0;
    u8 temp_t2;
    Ovl11TilePools *temp_a2;
    Ovl11TileItem *temp_v1;
    s8 group;
    void *var_t0;

    temp_a0 = *arg0;
    pos.vx = temp_a0[0];
    pos.vy = *(s32 *) ((u8 *) temp_a0 + 4);
    pos.vz = *(s32 *) ((u8 *) temp_a0 + 8);
    temp_a0_2 = ((s16 *) arg0[1])[1];
    var_s0 = (temp_a0_2 != 1) * 2;
    if (temp_a0_2 == 6) {
        var_s0 = 1;
    }
    var_v0 = 0;
    if (var_s0 != 2) {
        if (var_s0 != 0) {
            if (ovl_11_func_8011E090((VECTOR *) &pos.vx, &coords[0], &coords[1]) != 0) {
                return 0;
            }
            goto block_7;
        }
        var_v0 = 0;
        if (ovl_11_func_800D7EF8(&pos.vx, &coords[0], &coords[1]) == 0) {
block_7:
            row = coords[1];
            col = coords[0];
            rowOffset = row * 0x168;
            arg1 = col * sizeof(Ovl11D124Entry);
            mapBase = (u8 *) gMainAreaGrid;
            columnBase = mapBase + arg1;
            var_t0 = (void *) (rowOffset + (s32) columnBase);
            if (var_s0 != 0) {
                s32 alternateRowOffset;
                u8 *alternateBase;
                u8 *alternateColumnBase;

                alternateRowOffset = row * 0x38;
                alternateBase = mapBase + 0x2328;
                alternateColumnBase = alternateBase + arg1;
                var_t0 = (void *) (alternateRowOffset + (s32) alternateColumnBase);
            }
            if ((*(u16 *) var_t0) == 0x167) {
                return 0;
            }
            {
                temp_a2 = (Ovl11TilePools *) (mapBase - 0x55C4);
                temp_a1 = (*(s16 *) var_t0);
                temp_t2 = *((u8 *) var_t0 + 4);
                temp_a3 = (u16) temp_a1 * 0x28;
                temp_v1 = (Ovl11TileItem *) (temp_a3 + (s32) temp_a2->items);
                temp_t3 = temp_v1->variants[temp_t2];
                temp_v0 = temp_a2->properties[temp_t3][2];
                group = temp_v1->group;
                switch (temp_v0) {
                case 0:
                    *(*(s32 **) ((u8 *) arg0 + 0x10)) = 0;
                    *(*(void ***) ((u8 *) arg0 + 8)) = var_t0;
                    return 0;
                case 1:
                case 2:
                    *(*(s32 **) ((u8 *) arg0 + 0x10)) = (*(s32 *) ((u8 *) arg0 + 0x18)) | 0x4000;
                    if ((u16) temp_a1 == 0x64) {
                        temp_v0_2 = *(s32 **) ((u8 *) arg0 + 0x10);
                        *temp_v0_2 &= ~0x4000;
                    }
                    temp_a0_3 = ((Ovl11TileItem *) (temp_a3 + (s32) temp_a2->items))->flags;
                    switch (temp_a0_3) {
                    case 0x4040:
                        if (temp_a2->groups[group][1] != temp_t2) {
                            break;
                        }
                    case 0x100:
                    case 0x8000:
                    case 0x8010:
                        temp_v1_2 = *(s32 **) ((u8 *) arg0 + 0x10);
                        *temp_v1_2 |= 0x2000;
                        break;
                    }
                    var_v0 = 2;
                    *(*(void ***) ((u8 *) arg0 + 8)) = var_t0;
                    if (D_8006C860[temp_t3][2] == 1) {
                        return var_v0;
                    }
                    return 1;
                default:
                    break;
                }
                goto block_27;
            }
block_27:
            var_v0 = 0;
            /* Duplicate return node #28. Try simplifying control flow for better match */
            return var_v0;
        }
        /* Duplicate return node #28. Try simplifying control flow for better match */
        return var_v0;
    }
    return var_v0;
}
