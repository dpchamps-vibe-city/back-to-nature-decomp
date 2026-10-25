#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "screen_fade.h"

void ovl_30_func_80133454(Ovl30ValueMenuEntry *arg0, s32 arg1);
void ovl_30_func_80133974(u8 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

s32 ovl_30_func_801315E0(s32 arg0) {
    s32 *position = (s32 *)((u8 *)D_80076250 + Ovl30_ResidentSelection->resident * 0x1D4);
    u16 *flags = (u16 *)((u8 *)D_8007624C + Ovl30_ResidentSelection->resident * 0x1D4);
    s32 *destination = (s32 *)((u8 *)D_80076280 + Ovl30_ResidentSelection->resident * 0x1D4);
    s32 *destinationSpeed = (s32 *)((u8 *)D_80076290 + Ovl30_ResidentSelection->resident * 0x1D4);
    s16 *birth = (s16 *)((u8 *)D_80076244 + Ovl30_ResidentSelection->resident * 0x1D4);
    Ovl30ValueMenuEntry entries[32] = {
        {(u8 *)&(&gObjectEventSlotBase)[Ovl30_ResidentSelection->resident], 0, 0x27, 2, -2, D_8012EA00},
        {(u8 *)&(&gObjectEventSlotBase)[Ovl30_ResidentSelection->resident], 0, 1, 2, -2, D_8012EA14},
        {(u8 *)D_80076222 + Ovl30_ResidentSelection->resident * sizeof(struct_80076220), 0, 0xFF, 2, -1, D_8012EA28},
        {(u8 *)D_80076224 + Ovl30_ResidentSelection->resident * sizeof(struct_80076220), 0, 0xFFFF, 2, -1, D_8012EA3C},
        {(u8 *)&(&gObjectEventSlotBase)[Ovl30_ResidentSelection->resident], 0, 0x1F, 2, -2, D_8012EA50},
        {(u8 *)D_80076226 + Ovl30_ResidentSelection->resident * sizeof(struct_80076220), 0, 3, 2, -2, D_8012EA64},
        {(u8 *)D_80076228 + Ovl30_ResidentSelection->resident * sizeof(struct_80076220), 0, 0, 2, -2, D_8012EA78},
        {(u8 *)D_8007622A + Ovl30_ResidentSelection->resident * sizeof(struct_80076220), 0, 0, 2, -2, D_8012EA8C},
        {(u8 *)&(&gObjectEventSlotBase)[Ovl30_ResidentSelection->resident], 0, 0x1F, 2, -2, D_8012EA50},
        {(u8 *)D_8007622C + Ovl30_ResidentSelection->resident * sizeof(struct_80076220), 0, 0, 2, -2, D_8012EAA0},
        {(u8 *)D_8007623A + Ovl30_ResidentSelection->resident * sizeof(struct_80076220), 0, 0, 2, -2, D_8012EAB4},
        {(u8 *)D_8007623C + Ovl30_ResidentSelection->resident * sizeof(struct_80076220), 0, 0, 2, -2, D_8012EAC8},
        {(u8 *)D_80076242 + Ovl30_ResidentSelection->resident * sizeof(struct_80076220), 0, 0, 2, -2, D_8012EADC},
        {(u8 *)D_8007623E + Ovl30_ResidentSelection->resident * sizeof(struct_80076220), 0, 0, 2, -2, D_8012EAF0},
        {(u8 *)&flags[0], 0, 0, 2, -2, D_8012EB04},
        {(u8 *)&flags[1], 0, 0, 2, -2, D_8012EB18},
        {(u8 *)&position[0], 0, 0, 4, -2, D_8012EB2C},
        {(u8 *)&position[1], 0, 0, 4, -2, D_8012EB40},
        {(u8 *)&position[2], 0, 0, 4, -2, D_8012EB54},
        {(u8 *)D_80076270 + Ovl30_ResidentSelection->resident * sizeof(struct_80076220), 0, 0, 4, -2, D_8012EB68},
        {(u8 *)D_80076270 + Ovl30_ResidentSelection->resident * sizeof(struct_80076220), 0, 0, 4, -2, D_8012EB7C},
        {(u8 *)D_80076270 + Ovl30_ResidentSelection->resident * sizeof(struct_80076220), 0, 0, 4, -2, D_8012EB90},
        {(u8 *)&destination[0], 0, 0, 4, -2, D_8012EBA4},
        {(u8 *)&destination[1], 0, 0, 4, -2, D_8012EBB8},
        {(u8 *)&destination[2], 0, 0, 4, -2, D_8012EBCC},
        {(u8 *)&destinationSpeed[0], 0, 0, 4, -2, D_8012EBE0},
        {(u8 *)&destinationSpeed[1], 0, 0, 4, -2, D_8012EBF4},
        {(u8 *)&destinationSpeed[2], 0, 0, 4, -2, D_8012EC08},
        {(u8 *)&birth[0], 0, 0, 2, -2, D_8012EC1C},
        {(u8 *)&birth[1], 0, 0, 2, -2, D_8012EC30},
        {(u8 *)D_80076234 + Ovl30_ResidentSelection->resident * sizeof(struct_80076220), 0, 100, 2, -1, D_8012EC44},
        {(u8 *)D_80076236 + Ovl30_ResidentSelection->resident * sizeof(struct_80076220), 0, 100, 2, -1, D_8012EC58}
    };
    s32 buttons;

    if (IsScreenFadeComplete() != 0) {
        KanjiFntPrint(D_8012EC6C, Ovl30_ResidentSelection->resident);
        D_80134014 = arg0;
        ovl_30_func_80133454(entries, 32);
        arg0 = D_80134014;
        buttons = *(s32 *)gPad0Input;
        if (buttons & 4) {
            ovl_30_func_80133974((u8 *)&Ovl30_ResidentSelection->resident, 2, 0x24, 0, -1);
            return arg0;
        }
        if (buttons & 8) {
            ovl_30_func_80133974((u8 *)&Ovl30_ResidentSelection->resident, 2, 0x24, 0, 1);
        }
    }
    return arg0;
}
