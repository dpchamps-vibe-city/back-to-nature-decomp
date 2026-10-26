/*
 * map_grid_cells — reconstructed translation unit.
 *
 * Two-layer map-cell services: checked access, randomized matching,
 * neighborhood texture variants and refresh, kind-pair normalization and
 * cell-position effects. Public contracts: ovl11_map_grid_cells.h.
 * Layer 0 is the 45x25 main grid, layer 1 the 7x7 alternate grid; the random
 * search deliberately uses the inverse selector (nonzero means main).
 * The nine-bit mask uses row-major neighbor order. Preserve the original
 * ordered tests and fallback comparisons, including apparently unreachable
 * comparisons; simplifying them is not part of reconstructing this module.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "ovl11_sprite_effect_pool.h"
#include "random.h"

#include "ovl11_map_grid_cells.h"
#include "ovl11_cell_grid_lookup.h"

void Ovl11ApplyCellNeighborhoodVariants(s16 row, s16 column, s16 layer,
                                        s16 useCountFallback, s16 skipTextureDispatch) {
    s32 sp10;
    s32 sp14;
    s32 sp18;
    s32 sp1C;
    Ovl11GridBounds *sp20;
    s32 sp24;
    s32 sp28;
    s16 *var_s2;
    Ovl11D124Entry *temp_v0;
    s16 temp_v0_2;
    s32 temp_v1;
    s32 var_a0;
    u32 var_s1;
    s32 var_s4;
    s32 var_v0;
    s32 var_v0_2;
    s32 var_v1;
    s32 var_v1_2;
    s32 bit;
    s32 base;
    s32 outerX;
    s32 outerY;
    s32 (**setter)(s16, s16, s16);

    sp10 = (s32) row;
    sp14 = (s32) column;
    sp18 = (s32) useCountFallback;
    sp1C = (s32) skipTextureDispatch;
    sp20 = &gOvl11CellGridLayerBounds[layer];
    if ((useCountFallback == 0) && (sp1C == 0)) {
        gOvl11CellTextureSetters[layer](column, sp10, 5);
    }
    temp_v0 = Ovl11GetMapGridCell((s32) layer, (s16) sp14, (s16) sp10);
    if (temp_v0 != NULL) {
        sp24 = layer * 4;
        temp_v0->unk6 = (u16) temp_v0->unk6 & 0xFFF0;
        var_v1 = 0;
        do {
            outerX = sp14 + gOvl11CellNeighborOffsets[(var_v1 * 2) | 1];
            gOvl11CellProbeColumn = (u16) outerX;
            outerY = sp10 + gOvl11CellNeighborOffsets[var_v1 * 2];
            gOvl11CellProbeRow = (u16) outerY;
            var_s1 = 0;
            for (var_s4 = 0; var_s4 < 9; var_s4++) {
            temp_v1 = (s16) (gOvl11CellProbeColumn + gOvl11CellNeighborOffsets[var_s4 * 2 + 1]);
            temp_v0_2 = gOvl11CellProbeRow + gOvl11CellNeighborOffsets[var_s4 * 2];
            if ((temp_v1 >= 0) && (temp_v0_2 >= 0) && (temp_v1 < sp20->width) && (temp_v0_2 < sp20->height)) {
                temp_v0 = Ovl11GetMapGridCell((s32) layer, temp_v1, temp_v0_2);
                if ((temp_v0 != NULL) && ((u16) temp_v0->unk0 == 0x36) && (temp_v0->unk4 != 0)) {
                    var_s1 |= 1 << var_s4;
                }
            }
            }
            gOvl11CellProbeVariant = -1;
            var_a0 = -1;
            if ((var_s1 & 0x1B8) == 0x1B8) {
                temp_v0_2 = 6;
                gOvl11CellProbeVariant = temp_v0_2;
            }
            if (gOvl11CellProbeVariant == var_a0) {
                if (((var_s1 & 0xDB) == 0xDB) && ((var_s1 & 0x24) != 0x24) && ((var_s1 & 0x120) != 0x120)) {
                    gOvl11CellProbeVariant = 2;
                }
                if (gOvl11CellProbeVariant == -1) {
                    if ((var_s1 & 0x1BB) == 0x1BB) {
                        gOvl11CellProbeVariant = 6;
                    }
                    if (gOvl11CellProbeVariant == -1) {
                        var_v1_2 = var_s1 & 0xFE;
                        if (var_v1_2 == 0xFE) {
                            var_v1_2 = 6;
                            gOvl11CellProbeVariant = var_v1_2;
                        }
                        if (gOvl11CellProbeVariant == -1) {
                            if (((var_s1 & 0x3F) == 0x3F) && ((var_s1 & 0xC0) != 0xC0) && ((var_s1 & 0x180) != 0x180)) {
                                gOvl11CellProbeVariant = 4;
                            }
                            if (gOvl11CellProbeVariant == -1) {
                                if (((var_s1 & 0x1B6) == 0x1B6) && ((var_s1 & 9) != 9) && ((var_s1 & 0x48) != 0x48)) {
                                    var_a0 = 6;
                                    gOvl11CellProbeVariant = var_a0;
                                }
                                if (gOvl11CellProbeVariant == -1) {
                                    if (((var_s1 & 0x1B) == 0x1B) && ((var_s1 & 0xC0) != 0xC0) && ((var_s1 & 0x24) != 0x24)) {
                                        gOvl11CellProbeVariant = 3;
                                    }
                                    if (gOvl11CellProbeVariant == -1) {
                                        if (((var_s1 & 0xD8) == 0xD8) && ((var_s1 & 3) != 3) && ((var_s1 & 0x120) != 0x120)) {
                                            gOvl11CellProbeVariant = 1;
                                        }
                                        if (gOvl11CellProbeVariant == -1) {
                                            if (((var_s1 & 0x36) == 0x36) && ((var_s1 & 9) != 9) && ((var_s1 & 0x180) != 0x180)) {
                                                gOvl11CellProbeVariant = 5;
                                            }
                                            if (gOvl11CellProbeVariant == -1) {
                                                if ((var_s1 & 0x1B0) == 0x1B0) {
                                                    if (((var_s1 & 0x48) != 0x48) && ((var_s1 & 6) != 6)) {
                                                        gOvl11CellProbeVariant = 7;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if ((var_s1 & 0x10) != 0) {
                if (sp18 != 0) {
                    var_a0 = 0;
                    bit = 0;
                    for (; bit < 9; bit++) {
                        temp_v1 = 1;
                        var_v0_2 = temp_v1 << bit;
                        if (var_s1 & var_v0_2) {
                            var_a0++;
                        }
                    }
                    if (gOvl11CellProbeVariant == -1) {
                        if (var_a0 == 2) {
                            gOvl11CellProbeVariant = 0;
                        }
                        if (gOvl11CellProbeVariant == -1) {
                            if (var_a0 == 3) {
                                gOvl11CellProbeVariant = 0;
                            }
                            if (gOvl11CellProbeVariant == -1) {
                                if ((u32) (var_a0 - 4) < 3U) {
                                    if ((var_s1 & 0x1A0) != 0x1A0) {
                                        gOvl11CellProbeVariant = 0;
                                    }
                                    if (gOvl11CellProbeVariant == -1) {
                                        if ((var_s1 & 0x26) != 0x1A0) {
                                            gOvl11CellProbeVariant = 0;
                                        }
                                        if (gOvl11CellProbeVariant == -1) {
                                            if ((var_s1 & 0x4A) != 0x4A) {
                                                gOvl11CellProbeVariant = 0;
                                            }
                                            if (gOvl11CellProbeVariant == -1) {
                                                if ((var_s1 & 0xB) != 0x1A) {
                                                    gOvl11CellProbeVariant = 0;
                                                }
                                                goto block_84;
                                            }
                                            goto block_85;
                                        }
                                        goto block_85;
                                    }
                                    goto block_85;
                                }
                                goto block_84;
                            }
                            goto block_85;
                        }
                        goto block_85;
                    }
                    goto block_85;
                }
            }
block_84:
            if (gOvl11CellProbeVariant != -1) {
block_85:
                temp_v0 = Ovl11GetMapGridCell((s32) layer, (s16) gOvl11CellProbeColumn, (s16) gOvl11CellProbeRow);
                if (temp_v0 != NULL) {
                    if (sp1C == 0) {
                        setter = &gOvl11CellTextureSetters[layer];
                        base = ((temp_v0->unk4 - 1) * 8) + 5;
                        temp_v0_2 = base + (u16) gOvl11CellProbeVariant;
                        (*setter)((s16) gOvl11CellProbeColumn, (s16) gOvl11CellProbeRow, temp_v0_2);
                    }
                    var_v0_2 = (u16) temp_v0->unk6 & 0xFFF0;
                    var_v0_2 |= (u16) gOvl11CellProbeVariant;
                    temp_v0->unk6 = var_v0_2;
                }
            }
            var_v1++;
        } while (var_v1 < 9);
    }
}

void Ovl11SpawnCellEffectSprite(u16 effectId, s16 layer, s16 column, s16 row) {
    VECTOR position;
    s32 value;
    s32 origin;
    s32 id;

    id = effectId;
    if (layer == 0) {
        value = column * 400 - 0x1068;
        position.vx = value;
        value = row * 400;
        origin = 0x578;
    } else {
        value = column * 400 - 0x578;
        position.vx = value;
        value = row * 400;
        origin = 0x3E8;
    }
    origin -= value;
    position.vy = 0;
    position.vz = origin;
    __asm__ volatile("" : : : "memory");
    id = (s16)id;
    Ovl11SpawnSpriteEffect(id, position);
}

void Ovl11RefreshCellTextureVariant(s16 row, s16 column, s32 layer) {
    Ovl11D124Entry *entry;
    u16 flags;
    s16 texture;

    entry = Ovl11GetMapGridCell(layer, column, row);
    if (entry != 0) {
        if ((u16)entry->unk0 == 0x36) {
            entry->unk4 = Ovl11ReadRecordTypeProperty04(entry->unk0);
            entry->unk5 = 0;
            flags = (u16)entry->unk6 | 0x8000;
            entry->unk6 = flags;
            if (entry->unk4 != 0) {
                texture = (((entry->unk4 - 1) * 8) | 5) + (flags & 15);
                gOvl11CellTextureSetters[layer](column, row, texture);
            }
        }
    }
}

s32 Ovl11FindRandomGridCell(s16 *columnOut, s16 *rowOut, s32 mainMap,
                           Ovl11D124Entry *key, s32 matchMask) {
    Ovl11GridCellSearchView *view;
    Ovl11D124Entry *base;
    s32 count;
    s32 width;
    s32 start;
    s32 matches;
    s32 i;

    if (mainMap != 0) {
        width = 0x2D;
        count = 0x465;
        base = &gMainAreaGrid[0][0];
    } else {
        view = (Ovl11GridCellSearchView *)D_8006C838;
        width = 7;
        if (view->alternateGridEnabled == 0) {
            return 0;
        }
        count = 0x31;
        base = view->alternateGrid;
    }

    start = Rand(count);
    i = start;
    matches = 0;
    do {
        if (((matchMask & 1) || ((u16)key->unk0 == (u16)base[i].unk0)) &&
            ((matchMask & 2) || ((u16)key->unk2 == (u16)base[i].unk2)) &&
            ((matchMask & 4) || (key->unk4 == base[i].unk4))) {
            *columnOut = i % width;
            *rowOut = i / width;
            matches++;
            if ((matchMask & 8) == 0) {
                return matches;
            }
        }
        if (++i >= count) {
            i = 0;
        }
    } while (i != start);
    return matches;
}

void Ovl11NormalizeCellKindPair(u16 *kinds) {
    u16 previousKind = kinds[1];

    if (previousKind == 0x169) {
        kinds[1] = 0x168U;
        if (kinds[0] == previousKind) {
            kinds[0] = 0x168U;
        }
    } else if (previousKind == 0x16B) {
        kinds[1] = 0x16AU;
    }
}

Ovl11D124Entry *Ovl11GetMapGridCell(s32 layer, s16 column, s16 row) {
    Ovl11D124Entry *base;
    s32 height;
    s32 width;
    Ovl11D124Entry *ret;

    ret = 0;
    if (layer == 0) {
        height = 0x19;
        width = 0x2D;
        base = &gMainAreaGrid[0][0];
    } else {
        height = 7;
        width = 7;
        base = &gAlternateAreaGrid[0][0];
    }
    if ((column >= 0) && (column < width) && (row >= 0) && (row < height)) {
        ret = base + (row * width + column);
    }
    return ret;
}
