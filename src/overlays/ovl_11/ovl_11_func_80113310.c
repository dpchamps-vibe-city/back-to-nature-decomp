#include "common.h"
#include "game_types.h"

s32 func_80012A34(s32 arg0);
s32 ovl_11_func_801136D0(s32 *arg0, u16 *arg1, u16 *arg2);
s32 ovl_11_func_80113208(s16 arg0, s16 arg1, s16 arg2);
s32 ovl_11_func_80113A20(s32 arg0);
s32 ovl_11_func_800CB540(u16 *arg0);

#define GRID ((Ovl11GridState *)D_8012D110)

s32 ovl_11_func_80113310(Ovl11CE034Object *arg0) {
    s32 position[4];
    struct {
        s16 x;
        s16 y;
    } cell;
    struct {
        u16 id;
        u16 value;
        u16 count;
    } item;
    s16 *offset;

    offset = (s16 *)(D_801281F0 + arg0->unk38 * 4);
    position[0] = arg0->unk100 + offset[0];
    position[1] = arg0->unk104;
    position[2] = arg0->unk108 + offset[1];
    if (ovl_11_func_801136D0(position, (u16 *)&cell.x, (u16 *)&cell.y) != 0) {
        return 0;
    }
    if (GRID->grid[(s16)GRID->count][cell.x][cell.y] != 0) {
        return 0;
    }
    if (cell.x == 0 || cell.y == 0 || cell.x == 9 || cell.y == 9) {
        ovl_11_func_80113208(cell.x, cell.y, 1);
        GRID->grid[(s16)GRID->count][cell.x][cell.y] = 1;
        return 0;
    }
    if ((s16)GRID->count != 0) {
        if (GRID->grid[(s16)GRID->count - 1][cell.x][cell.y] == 2) {
            return 0;
        }
        if (cell.x >= GRID->centerX - 2 &&
            cell.x <= GRID->centerX + 2 &&
            cell.y >= GRID->centerY - 2 &&
            cell.y <= GRID->centerY + 2) {
            ovl_11_func_80113208(cell.x, cell.y, 1);
            GRID->grid[(s16)GRID->count][cell.x][cell.y] = 1;
            return 0;
        }
    }
    if (GRID->probability > func_80012A34(100) &&
        GRID->found[(s16)GRID->count] == 0) {
        GRID->grid[(s16)GRID->count][cell.x][cell.y] = 2;
        GRID->found[(s16)GRID->count] = 1;
        ovl_11_func_80113208(cell.x, cell.y, 2);
        GRID->lastX = cell.x;
        GRID->lastY = cell.y;
    } else {
        ovl_11_func_80113208(cell.x, cell.y, 1);
        GRID->grid[(s16)GRID->count][cell.x][cell.y] = 1;
        item.count = 1;
        item.value = 0;
        item.id = ovl_11_func_80113A20(((Ovl11TriggerWorkView *)&D_8007AFF0)->field_25474);
        if ((s16)item.id >= 0x186) {
            item.id = 0x10E;
        }
        if ((s16)item.id < 0) {
            item.id = 0x10F;
        }
        if ((s16)item.id != 0) {
            ovl_11_func_800CB540((u16 *)&item);
            GRID->probability += 2;
            return 1;
        }
        GRID->probability++;
    }
    return 0;
}
