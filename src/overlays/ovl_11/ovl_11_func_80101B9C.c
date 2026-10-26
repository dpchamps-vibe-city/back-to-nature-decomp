#include "common.h"
#include "sound_commands.h"
#include "message_window.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "beveled_panel.h"
#include "psyq/libgpu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "psyq/abs.h"
#include "psyq/libgs.h"
#include "psyq/libsnd.h"
#include "psyq/libapi.h"
#include "ovl11_item_table_queries.h"

void func_80017B3C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void func_8001AAF4(s16 arg0, s16 arg1, s32 arg2, s16 arg3, s16 arg4);
void SetVal8005E2BC(s32 arg0);
void func_800248B0(s32 arg0, s16 arg1, s16 arg2);
void ovl_11_func_80102368(void);
void ovl_11_func_801024D8(void);
void ovl_11_func_80102268(s8 arg0, s8 arg1);
void ovl_11_func_801022E8(void);
s32 ovl_11_func_801027D4(s8 arg0, s8 arg1);
void *ovl_11_func_80102844(s8 arg0, s8 arg1);
void ovl_11_func_80100FFC(void);

void ovl_11_func_80101B9C(void) {
    s32 ui;
    s32 text;
    u32 *panels;
    s16 empty;
    s32 i;

    ui = gActiveRenderContext->field_D8;
    panels = (u32 *)(ui + 0x74);
    text = ui + 0x70;
    DrawMenuPanel(panels, 1, 0x16, 0x10, 0x10E, 0x4A);
    DrawMenuPanel(panels, 1, 0x16, 0x5A, 0x10E, 0x4A);
    func_80017B3C(text, (s32)(D_800539E8 + gTextBankOffsets[0]), 0x21, 0x20);
    if (D_801273EA_table == 0) {
        func_80017B3C(text, (s32)(D_800539E8 + 0x16 + gTextBankOffsets[0]), 0x21, 0x6A);
    } else {
        func_80017B3C(text, (s32)(D_800539E8 + 0x28 + gTextBankOffsets[0]), 0x21, 0x6A);
        func_8001AAF4(D_801273EA_table, 1, text, 0x2A, 0x78);
        func_80017B3C(text, (s32)(D_800539E8 + 0x36 + gTextBankOffsets[0]), 0x33, 0x78);
        func_8001AAF4(8, 1, text, 0x3C, 0x78);
    }
    ovl_11_func_80102368();
    ovl_11_func_801024D8();
    empty = 8;
    for (i = 0; i < 8; i++) {
        if (D_8012CF00_ids[i] == 0) {
            empty = i;
            break;
        }
    }
    if (D_801273E4_state == 6) {
        ovl_11_func_80102268(Ovl11_D_801273E7, D_801273EA_table);
    } else if (D_801273E4_state == 7) {
        ovl_11_func_801022E8();
    } else {
        SetMessagePortraitEnabled(0);
        SetVal8005E2BC(0);
        RequestMessageWindow(3, 0x3BE);
        if (IsMessageWindowStatusFinal()) {
            switch (((u32 *)gPad0Input)[0]) {
            case 0x1000:
            case 0x4000:
                Ovl11_D_801273E7 = Ovl11_D_801273E7 >= 4 ?
                    Ovl11_D_801273E7 - 4 : Ovl11_D_801273E7 + 4;
                PlaySoundEffect(5);
                break;
            case 0x2000:
                if (Ovl11_D_801273E7 == 3 || Ovl11_D_801273E7 == 7) {
                    D_801273EA_table++;
                }
                Ovl11_D_801273E7++;
                if (Ovl11_D_801273E7 == Ovl11_D_801273E7 / 4 * 4) {
                    Ovl11_D_801273E7 -= 4;
                }
                PlaySoundEffect(5);
                break;
            case 0x8000:
                if (Ovl11_D_801273E7 == 0 || Ovl11_D_801273E7 == 4) {
                    D_801273EA_table--;
                }
                Ovl11_D_801273E7--;
                if (Ovl11_D_801273E7 == 3 || Ovl11_D_801273E7 == -1) {
                    Ovl11_D_801273E7 += 4;
                }
                PlaySoundEffect(5);
                break;
            }
            if (((u32 *)gPad0Input)[0] & 0xA) {
                D_801273EA_table++;
                PlaySoundEffect(5);
            }
            if (((u32 *)gPad0Input)[0] & 5) {
                D_801273EA_table--;
                PlaySoundEffect(5);
            }
            if (D_801273EA_table < 0) {
                D_801273EA_table = 8;
            }
            if (D_801273EA_table > 8) {
                D_801273EA_table = 0;
            }
        }
        func_800248B0(gActiveRenderContext->field_D8 + 0x54,
                     (Ovl11_D_801273E7 - Ovl11_D_801273E7 / 4 * 4) * 34 + 0x87,
                     Ovl11_D_801273E7 / 4 * 34 + 0x67);
        if (IsMessageWindowStatusFinal()) {
            if (((u32 *)gPad0Input)[2] & 0x800) {
                for (i = 0; i < 8; i++) {
                    if (D_8012CF00_ids[i] != 0) {
                        CloseMessageWindow();
                        PlaySoundEffect(0);
                        D_801273E4_state = 7;
                        return;
                    }
                }
                PlaySoundEffect(2);
            } else if (((u32 *)gPad0Input)[2] & 0x10) {
                if (*(s16 *)ovl_11_func_80102844(D_801273EA_table, Ovl11_D_801273E7) != 0) {
                    CloseMessageWindow();
                    PlaySoundEffect(0);
                    D_801273E4_state = 6;
                } else {
                    PlaySoundEffect(2);
                }
            } else if (((u32 *)gPad0Input)[2] & 0x20) {
                PlaySoundEffect(1);
                if (*(u16 *)&D_801273F0 != 0) {
                    for (i = 0; i < 8; i++) {
                        D_8012CF00_ids[i] = 0;
                    }
                } else {
                    CloseMessageWindow();
                    Ovl11_D_801273DC = ovl_11_func_80100FFC;
                }
            } else if (((u32 *)gPad0Input)[2] & 0x40) {
                if (*(s16 *)ovl_11_func_80102844(D_801273EA_table, Ovl11_D_801273E7) == 0) {
                    PlaySoundEffect(2);
                    return;
                }
                if (ovl_11_func_801027D4(D_801273EA_table, Ovl11_D_801273E7) == 1) {
                    for (i = 0; i < 8; i++) {
                        if (D_8012CF00_ids[i] == *(s16 *)ovl_11_func_80102844(D_801273EA_table, Ovl11_D_801273E7)) {
                            PlaySoundEffect(1);
                            D_8012CF00_ids[i] = 0;
                            break;
                        }
                    }
                } else {
                    if (*(s16 *)ovl_11_func_80102844(D_801273EA_table, Ovl11_D_801273E7) != 0) {
                        if (empty < 8 && Ovl11GetItemType(*(s16 *)ovl_11_func_80102844(D_801273EA_table, Ovl11_D_801273E7)) != 1) {
                            PlaySoundEffect(0);
                            D_8012CF00_ids[empty] = *(s16 *)ovl_11_func_80102844(D_801273EA_table, Ovl11_D_801273E7);
                        } else {
                            PlaySoundEffect(2);
                        }
                    } else {
                        PlaySoundEffect(2);
                    }
                }
            }
        }
    }
}
