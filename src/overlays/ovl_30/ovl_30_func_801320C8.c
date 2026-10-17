#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"

s32 func_80013394(void);
void ovl_30_func_80133454(Ovl30ValueMenuEntry *arg0, s32 arg1);
void ovl_30_func_80133974(u8 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

s32 ovl_30_func_801320C8(s32 arg0) {
    Ovl30ValueMenuEntry entries[29] = {
        {(u8 *)&Ovl30_RecordsA638[D_801349B4].field_AE, 0, 0xFF, 2, -1, D_8012ECE8},
        {(u8 *)&Ovl30_RecordsA638[D_801349B4].field_B0, 0, 0xFF, 2, -1, D_8012ECF4},
        {(u8 *)&Ovl30_RecordsA638[D_801349B4].field_B2, 0, 0xFF, 2, -1, D_8012ED00},
        {(u8 *)&Ovl30_RecordsA638[D_801349B4].field_B4, 0, 0x1F, 2, -1, D_8012ED0C},
        {(u8 *)&Ovl30_RecordsA638[D_801349B4].field_B6, 0, 0, 2, -2, D_8012ED18},
        {(u8 *)&Ovl30_RecordsA638[D_801349B4].field_B8, 0, 0, 2, -2, D_8012E91C},
        {(u8 *)&Ovl30_RecordsA638[D_801349B4].field_BA, 0, 0, 2, -2, D_8012ED24},
        {(u8 *)&Ovl30_RecordsA638[D_801349B4].affection, 0, 0xFF, 2, -1, D_8012E874},
        {(u8 *)&Ovl30_RecordsA638[D_801349B4].gestation, 0, 0x63, 2, -2, D_8012E8D4},
        {(u8 *)&Ovl30_RecordsA638[D_801349B4].action, 0, 0, 2, -2, D_8012E8E0},
        {(u8 *)&Ovl30_RecordsA638[D_801349B4].process_no, 0, 0, 2, -2, D_8012E8EC},
        {(u8 *)&Ovl30_RecordsA638[D_801349B4].process_id, 0, 0, 2, -2, D_8012E8F8},
        {(u8 *)&Ovl30_RecordsA638[D_801349B4].process_time, 0, 0, 2, -2, D_8012E904},
        {(u8 *)&Ovl30_RecordsA638[D_801349B4].elapsed[1], 0, 0, 2, -2, D_8012E910},
        {(u8 *)&Ovl30_RecordsA638[D_801349B4].flags, 0, 0, 4, -2, D_8012E91C},
        {(u8 *)&Ovl30_RecordsA638[D_801349B4].position[0], 0, 0, 4, -2, D_8012E928},
        {(u8 *)&Ovl30_RecordsA638[D_801349B4].position[1], 0, 0, 4, -2, D_8012E934},
        {(u8 *)&Ovl30_RecordsA638[D_801349B4].position[2], 0, 0, 4, -2, D_8012E940},
        {(u8 *)&Ovl30_RecordsA638[D_801349B4].speed[0], 0, 0, 4, -2, D_8012E94C},
        {(u8 *)&Ovl30_RecordsA638[D_801349B4].speed[1], 0, 0, 4, -2, D_8012E958},
        {(u8 *)&Ovl30_RecordsA638[D_801349B4].speed[2], 0, 0, 4, -2, D_8012E964},
        {(u8 *)&Ovl30_RecordsA638[D_801349B4].target[0], 0, 0, 4, -2, D_8012E970},
        {(u8 *)&Ovl30_RecordsA638[D_801349B4].target[1], 0, 0, 4, -2, D_8012E97C},
        {(u8 *)&Ovl30_RecordsA638[D_801349B4].target[2], 0, 0, 4, -2, D_8012E988},
        {(D_8007A638 - 0x4426), 0, 999, 2, -1, D_8012ED30},
        {(D_8007A638 - 0x4424), 0, 999, 2, -1, D_8012ED3C},
        {(D_8007A638 - 0x4422), 0, 999, 2, -1, D_8012ED48},
        {(D_8007A638 - 0x4420), 0, 999, 2, -1, D_8012ED54},
        {(D_8007A638 - 0x441E), 0, 999, 2, -1, D_8012ED60}
    };
    s32 buttons;

    if (func_80013394() != 0) {
        KanjiFntPrint(D_8012ED6C, D_801349B4);
        D_80134014 = arg0;
        ovl_30_func_80133454(entries, 29);
        arg0 = D_80134014;
        buttons = *(s32 *)D_8005E3A8;
        if (buttons & 4) {
            ovl_30_func_80133974((u8 *)&D_801349B4, 4, 6, 0, -1);
            return arg0;
        }
        if (buttons & 8) {
            ovl_30_func_80133974((u8 *)&D_801349B4, 4, 6, 0, 1);
        }
    }
    return arg0;
}
