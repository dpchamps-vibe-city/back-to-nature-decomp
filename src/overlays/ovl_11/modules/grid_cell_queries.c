/*
 * Main/alternate-grid classification and directional cell queries.
 * The near/far probe shares its classifier with the local grid-query protocol;
 * the collision leaf uses the existing collision-query layout. The final
 * service submits action 0 for every main-grid cell, in row-major order.
 * Membership and evidence-limited boundaries: notes/file-groupings.md.
 */
#include "common.h"
#include "game_types.h"
#include "ovl11_grid_cell_queries.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "ovl11_cell_actions.h"

s32 ovl_11_func_8011E090(VECTOR *pos, u16 *colOut, u16 *rowOut);
s32 ovl_11_func_800D7EF8(s32 *pos, u16 *colOut, u16 *rowOut);
s32 Ovl11ClassifyGridCell(s16 col, s16 row, s32 map);

s32 Ovl11CheckDirectionalCellTransition(s32 *position, s32 direction) {
    VECTOR nearPos;
    VECTOR farPos;
    u16 nearCol;
    u16 nearRow;
    u16 farCol;
    u16 farRow;
    s16 *delta;
    s32 farLookupResult;
    s32 map;
    s32 result;

    map = (((struct Ovl11D2764StateView *) &D_8007AFF0)->field_25476 != 1) * 2;
    if (((struct Ovl11D2764StateView *) &D_8007AFF0)->field_25476 == 6) {
        map = 1;
    }
    if (map != 2) {
        delta = (s16 *) (gOvl11AreaGridCellOffsets + direction * 4);
        nearPos.vx = position[0] + delta[0];
        nearPos.vy = position[1];
        nearPos.vz = position[2] + delta[1];
        farPos.vx = position[0] + delta[0] * 2;
        farPos.vy = position[1];
        farPos.vz = position[2] + delta[1] * 2;
        if (map != 0) {
            result = 0;
            if (ovl_11_func_8011E090(&nearPos, &nearCol, &nearRow) == 0) {
                farLookupResult = ovl_11_func_8011E090(&farPos, &farCol, &farRow);
                goto check_cells;
            }
            return result;
        }
        result = 0;
        if ((ovl_11_func_800D7EF8(&nearPos.vx, &nearCol, &nearRow) == 0) &&
            (((s16) nearCol != 0x2C) || (result = 0, direction != 3))) {
            farLookupResult = ovl_11_func_800D7EF8(&farPos.vx, &farCol, &farRow);
check_cells:
            if (Ovl11ClassifyGridCell((s16) nearCol, (s16) nearRow, map) == 1) {
                if (farLookupResult == 0) {
                    if (Ovl11ClassifyGridCell((s16) farCol, (s16) farRow, map) == 2) {
                        return 1;
                    }
                } else {
                    return 1;
                }
            }
            return 0;
        }
        return result;
    }
    return 0;
}

/* Local classifier, still externally linked: 0=other, 1=property 1,
 * 2=empty tag or property 0. Coordinates are already validated by callers. */
s32 Ovl11ClassifyGridCell(s16 col, s16 row, s32 map) {
    Ovl11D124Entry *entry;
    Ovl11TilePools *pools;
    u16 property;

    if (map != 0) {
        entry = &gAlternateAreaGrid[row][col];
    } else {
        entry = &gMainAreaGrid[row][col];
    }
    if ((u16) entry->unk0 == 0x167) {
        return 2;
    }
    pools = (Ovl11TilePools *) D_8006C838;
    property = pools->properties[pools->items[(u16) entry->unk0].variants[entry->unk4]][2];
    switch (property) {
    case 0:
        return 2;
    case 1:
        return 1;
    case 2:
        return 0;
    }
    return 0;
}

s32 Ovl11IsSpecialCollisionCell(Ovl11CollisionQuery *query) {
    s32 point[3];
    u16 col;
    u16 row;
    s32 *position;
    s32 map;
    s32 otherRegion;
    u16 *entry;
    s32 tag;

    position = query->position;
    point[0] = position[0];
    point[1] = position[1];
    point[2] = position[2];
    otherRegion = (query->object->field_2 != 1);
    map = otherRegion * 2;
    if (map == 2) {
        return 0;
    }
    if (ovl_11_func_800D7EF8(point, &col, &row) == 0) {
        if (map != 0) {
            entry = (u16 *) &gAlternateAreaGrid[(s16) row][(s16) col];
        } else {
            entry = (u16 *) &gMainAreaGrid[(s16) row][(s16) col];
        }
        tag = *entry;
        /* Keep the decision tree: unsigned tag load, signed word tests. */
        if (tag != 0x3F) {
            if (tag < 0x40) {
                if (tag < 0x3E) {
                    if (tag < 0x3B) {
                        goto miss;
                    }
                    goto hit;
                }
                goto miss;
            }
            if (tag < 0x175) {
                if (tag < 0x16C) {
                    goto miss;
                }
                goto hit;
            }
            goto miss;
        }
hit:
        return 1;
miss:
        return 0;
    }
    return 0;
}

void Ovl11ProcessMainGridCells(void) {
    u32 row;
    u32 col;

    for (row = 0; row < 0x19U; row++) {
        for (col = 0; col < 0x2DU; col++) {
            Ovl11TryPromoteCellPendingKind(0, (s16) col, (s16) row, 1);
        }
    }
}
