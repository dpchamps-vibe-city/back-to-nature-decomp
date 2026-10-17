#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"

void ovl_11_func_800F9BE4(void);
s32 ovl_11_func_800D5750(s16 arg0);
void ovl_11_func_800D666C(s16 arg0);
s32 ovl_11_func_800D6730(s16 value, s16 row);
s32 ovl_11_func_800D678C(s16 arg0);
s32 ovl_11_func_800D67F4(s16 arg0);

#define imageRect (*(RECT *)D_80129638)
#define slots (((Ovl11FarSlotTableView *)&D_8007AFF0)->slots)
#define records (((Ovl11FarRecordTableView *)&D_8007AFF0)->records)
#define cache D_80129648_textures
#define cacheImage(i) (*(Ovl11A04B8Entry *)&cache[i])

void ovl_11_func_800F9210(s32 arg0, s32 arg1, s32 arg2) {
    Ovl11TextureRecord backup;
    s32 oldRow;
    s16 row;
    s32 slot;
    s16 index;
    s16 special;
    s32 position;
    s32 column;
    s16 clutColumn;
    s16 page;
    s16 clutPage;
    s32 pageX;
    s32 clutY;

    arg0 &= 0xFFFF;
    arg1 &= 0xFFFF;
    if (arg2 == 1) {
        ovl_11_func_800F9BE4();
    }
    special = 0;
    position = 0;
    if (arg0 != 0) {
        oldRow = (s16)ovl_11_func_800D5750(arg0) != 0;
        for (slot = 0; slot < 9; slot = (s16)(slot + 1)) {
            if (slots[oldRow][slot] == arg0) {
                break;
            }
        }
        if (slot == 9) {
            return;
        }
        if (records[oldRow][slot].unk18 >= 0x3F0) {
            special = 1;
        }
        imageRect.w = 8;
        imageRect.x = records[oldRow][slot].unk0;
        imageRect.y = records[oldRow][slot].unk2;
        imageRect.h = oldRow == 0 ? 0x20 : 0x40;
        if (special == 1) {
            imageRect.x += 0x3C0;
        } else {
            imageRect.x += 0x340;
        }
        StoreImage(&imageRect, (u_long *)(D_8005E3B0 + 0x4290));
        DrawSync(0);
        imageRect.w = 0x10;
        imageRect.h = 1;
        imageRect.x = records[oldRow][slot].unk18;
        imageRect.y = records[oldRow][slot].unk1A;
        StoreImage(&imageRect, (u_long *)(D_8005E3B0 + 0x4A90));
        DrawSync(0);
        backup.active = 1;
        backup.id = arg0;
        *(Ovl11A04B8Entry *)&backup = records[oldRow][slot];
        if (arg2 == 1) {
            ovl_11_func_800D666C(arg0);
        }
    }
    if (arg1 != 0) {
        row = (s16)ovl_11_func_800D5750(arg1) != 0;
        for (index = 0; index < 72; index++) {
            if (cache[index].id == arg1) {
                break;
            }
        }
        if (index == 72) {
            return;
        }
        slot = (s16)ovl_11_func_800D6730(arg1, row);
        if (slot == -1) {
            return;
        }
        special = ovl_11_func_800D678C(row);
        slots[row][slot] = arg1;
        records[row][slot] = cacheImage(index);
        imageRect.w = 8;
        imageRect.x = cache[index].x;
        imageRect.y = cache[index].y;
        imageRect.h = row == 0 ? 0x20 : 0x40;
        if (special == 1) {
            records[row][slot].unk0 = row * 8 + 0x28;
            records[row][slot].unk2 = 0xBD;
            records[row][slot].unk4 = records[row][slot].unk0;
            records[row][slot].unk6 = records[row][slot].unk2 + records[row][slot].unkA;
            MoveImage2(&imageRect, row * 8 + 0x3E8, 0xBD);
        } else {
            position = (s16)ovl_11_func_800D67F4(row);
            records[row][slot].unk0 = position * 8;
            records[row][slot].unk2 = row * 32 + 0x9B;
            records[row][slot].unk4 = records[row][slot].unk0;
            records[row][slot].unk6 = records[row][slot].unk2 + records[row][slot].unkA;
            MoveImage2(&imageRect, position * 8 + 0x340, row * 32 + 0x9B);
        }
        imageRect.w = 0x10;
        imageRect.x = cache[index].clutX;
        imageRect.h = 1;
        imageRect.y = cache[index].clutY;
        if (special == 1) {
            records[row][slot].unk18 = 0x3F0;
            records[row][slot].unk1A = row + 0xFE;
            MoveImage2(&imageRect, 0x3F0, row + 0xFE);
        } else {
            records[row][slot].unk18 = (position - (s16)(position / 4) * 4) * 16 + 0x340;
            records[row][slot].unk1A = position / 4 + (clutY = row * 2 + 0xFB);
            MoveImage2(&imageRect, (position - (s16)(position / 4) * 4) * 16 + 0x340, (s16)(position / 4) + clutY);
        }
    }
    if (arg0 != 0) {
        for (index = 0; index < 72; index++) {
            if (cache[index].id == arg0) {
                return;
            }
        }
        oldRow = (s16)ovl_11_func_800D5750(arg0) != 0;
        for (index = 0; index < 72; index++) {
            if (cache[index].id == 0) {
                break;
            }
        }
        if (index == 72) {
            return;
        }
        column = (s16)(index / 8);
        column = (index - column * 8) * 8;
        page = index / 24;
        pageX = (page << 6) + 0x140;
        backup.x = column + pageX;
        backup.y = ((index - page * 24) / 8) << 6;
        imageRect.x = backup.x;
        imageRect.y = backup.y;
        imageRect.w = 8;
        imageRect.h = oldRow == 0 ? 0x20 : 0x40;
        LoadImage(&imageRect, (u_long *)(D_8005E3B0 + 0x4290));
        DrawSync(0);
        clutColumn = index / 4;
        clutPage = index / 24;
        backup.clutX = (index - clutColumn * 4) * 16 + ((clutPage << 6) + 0x140);
        backup.clutY = (index - clutPage * 24) / 4 + 0xC0;
        imageRect.x = backup.clutX;
        imageRect.y = backup.clutY;
        imageRect.w = 0x10;
        imageRect.h = 1;
        LoadImage(&imageRect, (u_long *)(D_8005E3B0 + 0x4A90));
        DrawSync(0);
        cache[index].id = arg0;
        cacheImage(index) = *(Ovl11A04B8Entry *)&backup;
    }
}
