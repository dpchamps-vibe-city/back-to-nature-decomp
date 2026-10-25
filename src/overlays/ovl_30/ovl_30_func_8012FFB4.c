#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "screen_fade.h"

u32 Rand(s32);
s32 func_8001FABC(s16);
void func_8001B2CC(s32, s32);
void func_8001B3CC(s32);
void func_8001AF70(u16, u16);
void ovl_30_func_80133454(Ovl30ValueMenuEntry *, s32);
u16 *ovl_11_func_800CE744(s32, s32);
s32 ovl_11_func_800D5750(s16);
s32 ovl_11_func_800D8320();
s32 ovl_11_func_800E6EAC();
s32 ovl_11_func_800E6F34();
s32 ovl_11_func_800E7088(s16, s16, s32, s32);
s32 ovl_11_func_800E99EC(s16, s16, s32, s32);
void ovl_11_func_800EF488(s16);
void ovl_11_func_800EF6E4();
s32 ovl_11_func_8010CE80(Recon_ovl_11_func_8010CE80_A0View *, s32);

#define ACTION ((Ovl30ActionState *)D_8006C838)
#define EVENT(index) ((Ovl30EventRecordView *)((u8 *)D_8006C838 + (index) * 0x1D4))

s32 ovl_30_func_8012FFB4(s32 arg0) {
    s32 id;
    u16 *object;
    Ovl30RecordF8 *entry;
    s32 i;
    s32 x;
    s32 y;
    s16 selected;
    s32 first;
    s32 second;

    if (IsScreenFadeComplete() != 0) {
        if (D_80134B10 < 0) {
            KanjiFntPrint(D_8012E48C);
            D_80134014 = arg0;
            ovl_30_func_80133454(&D_8013439C, 29);
            arg0 = D_80134014;
            KanjiFntPrint(D_8012E4AC, D_80134B10 < 0 ? D_8012E4C4 : D_8012E4C8);
            if (D_80134B10 > 0) {
                func_8001FABC(0);
            }
        }
        switch (D_80134B10) {
        case 0:
            func_8001B2CC(0, D_80134AFC);
            D_80134B10 = -1;
            break;
        case 1:
            func_8001B3CC(0);
            D_80134B10 = -1;
            break;
        case 2: id = 0x162; goto spawn;
        case 3: id = 0x161; goto spawn;
        case 4: id = 0x160; goto spawn;
        case 5: id = 0x165; goto spawn;
        case 6: id = 0x164; goto spawn;
        case 7: id = 0x109; goto spawn;
        case 8: id = 0x10A; goto spawn;
        case 9: id = 0x15E; goto spawn;
        case 10:
            id = 0x15F;
        spawn:
            object = ovl_11_func_800CE744(id, -1);
            D_80134B10 = -1;
            for (i = 0; i < 8; i++) {
                object[i + 2] = Rand(0x94);
            }
            object[10] = 0xFFFF;
            break;
        case 11:
            D_80134B10 = -1;
            ACTION->flagsC |= 0x400000;
            break;
        case 12:
            D_80134B10 = -1;
            ACTION->flagsC &= ~0x400000;
            break;
        case 13:
            object = ovl_11_func_800CE744(0x162, -1);
            ((Ovl30AnimalRecord *)object)->elapsed[1] = 1;
            ((Ovl30AnimalRecord *)object)->position[0] = -2000;
            ((Ovl30AnimalRecord *)object)->position[2] = 0;
            object = ovl_11_func_800CE744(0x161, -1);
            ((Ovl30AnimalRecord *)object)->elapsed[1] = 1;
            ((Ovl30AnimalRecord *)object)->position[0] = -1000;
            ((Ovl30AnimalRecord *)object)->position[2] = 0;
            object = ovl_11_func_800CE744(0x160, -1);
            ((Ovl30AnimalRecord *)object)->elapsed[1] = 1;
            ((Ovl30AnimalRecord *)object)->position[0] = 0;
            ((Ovl30AnimalRecord *)object)->position[2] = 0;
            object = ovl_11_func_800CE744(0x160, -1);
            ((Ovl30AnimalRecord *)object)->elapsed[1] = 1;
            ((Ovl30AnimalRecord *)object)->position[0] = 1000;
            ((Ovl30AnimalRecord *)object)->position[2] = 0;
            object = ovl_11_func_800CE744(0x164, -1);
            ((Ovl30AnimalRecord *)object)->elapsed[1] = 1;
            ((Ovl30AnimalRecord *)object)->position[0] = 2000;
            ((Ovl30AnimalRecord *)object)->position[2] = 0;
            object = ovl_11_func_800CE744(0x165, -1);
            ((Ovl30AnimalRecord *)object)->elapsed[1] = 1;
            ((Ovl30AnimalRecord *)object)->position[0] = 3000;
            ((Ovl30AnimalRecord *)object)->position[2] = 0;
            D_80134B10 = -1;
            break;
        case 14:
            for (y = 0; y < 25; y++) {
                for (x = 0; x < 45; x++) {
                    ovl_11_func_800D8320(0, (s16)x, (s16)y, 0);
                }
            }
            D_80134B10 = -1;
            break;
        case 15:
            D_80134B10 = -1;
            ACTION->flags5234 &= ~0x800400;
            break;
        case 16:
            id = (s16)ovl_11_func_800D5750((s16)D_80134B00);
            switch (id) {
            case 0:
                ovl_11_func_800E7088((s16)D_80134B00, 0, 1, 0);
                break;
            case 1:
                if (ovl_11_func_800E6EAC(1, -1, 0, 0) == 1) {
                    ovl_11_func_800E6F34((s16)D_80134B00, 0, 1, 0);
                } else {
                    ovl_11_func_800E6F34((s16)D_80134B00, 0, 0, 0);
                }
                break;
            }
            D_80134B10 = -1;
            break;
        case 17:
            entry = Ovl30_RecordsA638;
            for (id = 0; id < 7; id++, entry++) {
                entry->field_B8 |= 1;
                ovl_11_func_8010CE80((Recon_ovl_11_func_8010CE80_A0View *)entry, 11);
            }
            D_80134B10 = -1;
            break;
        case 18:
            entry = Ovl30_RecordsA638;
            for (id = 0; id < 7; id++, entry++) {
                entry->field_B8 |= 2;
                ovl_11_func_8010CE80((Recon_ovl_11_func_8010CE80_A0View *)entry, 11);
            }
            D_80134B10 = -1;
            break;
        case 19:
            entry = Ovl30_RecordsA638;
            for (id = 0; id < 7; id++, entry++) {
                entry->field_B8 |= 4;
                ovl_11_func_8010CE80((Recon_ovl_11_func_8010CE80_A0View *)entry, 11);
            }
            D_80134B10 = -1;
            break;
        case 20:
            ovl_11_func_800EF6E4(1, 0);
            D_80134B10 = -1;
            break;
        case 21:
            ovl_11_func_800EF6E4(1, 1);
            D_80134B10 = -1;
            break;
        case 22:
            ovl_11_func_800EF6E4(1, 2);
            D_80134B10 = -1;
            break;
        case 23:
            ovl_11_func_800EF6E4(1, 3);
            D_80134B10 = -1;
            break;
        case 24:
            ovl_11_func_800EF6E4(1, 4);
            D_80134B10 = -1;
            break;
        case 25:
            ovl_11_func_800EF488(-1);
            selected = ACTION->selected - 3;
            switch (selected) {
            case 0: id = 0xA4; break;
            case 4: id = 0xA3; break;
            case 6: id = 0xA5; break;
            case 11: id = 0xA6; break;
            case 17:
            default: id = 0xA2; break;
            }
            EVENT(ACTION->selected)->field_99EC = 0xFFFF;
            func_8001AF70(id + 0xFB, 1);
            ACTION->state44CE = 2;
            ACTION->state44D0 = 1;
            ACTION->state44D2 = 1;
            ACTION->random = Rand(3);
            D_80134B10 = -1;
            break;
        case 26:
            ovl_11_func_800EF6E4(0, (s16)D_80134B04);
            switch (D_80134B04) {
            case 0: first = 3; second = 19; break;
            case 1: first = 7; second = 11; break;
            case 2: first = 9; second = 8; break;
            case 3: first = 14; second = 5; break;
            case 4:
            default: first = 20; second = 25; break;
            }
            EVENT(second)->field_99EA = 0xFF;
            EVENT(first)->field_99EC = 0;
            D_80134B10 = -1;
            EVENT(second)->field_9A06 |= 0x400;
            break;
        case 27:
            ovl_11_func_800E99EC((s16)D_80134B08, 0, 0, 0);
            D_80134B10 = -1;
            break;
        }
    }
    return arg0;
}
