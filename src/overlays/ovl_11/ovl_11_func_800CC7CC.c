#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "sound_commands.h"
#include "message_window.h"
#include "game_flags.h"
#include "ovl11_paired_item_ids.h"
#include "psyq/abs.h"
#include "psyq/memory.h"
#include "psyq/libgpu.h"
#include "ovl11_object_events.h"

s32 ovl_11_func_800CE550(Vec3 *arg0);
unsigned long long ovl_11_func_800BF450(void);
void ovl_11_func_800CCBE8(Ovl11CC4E0Object *arg0);
s32 ovl_11_func_800CB540(u16 *arg0);
s32 ovl_11_func_800CC4E0(Ovl11CC4E0Object *arg0, Vec3 *arg1);
Ovl11CC7CCSpawn *ovl_11_func_800F4994(Vec3 *arg0);
s32 ovl_11_func_800F3E44(s16 arg0);
s32 ovl_11_func_800BF3D0(void);
s32 ovl_11_func_800F3BCC(u16 arg0);
void ovl_11_func_800F43CC(s32 arg0, VECTOR position, u16 arg5, s32 arg6);
s32 ovl_11_func_80109188(Ovl11D04D4Obj *arg0, s32 arg1);
s32 ovl_11_func_800CC6A8(Ovl11CC7CCObject *arg0);

s32 ovl_11_func_800CC7CC(Ovl11CC7CCObject *arg0) {
    VECTOR next;
    Ovl11CC7CCRequest request;
    VECTOR position;
    Ovl11CC7CCSpawn *spawn;
    Ovl11CC7CCSpawn *saved;
    char *base;
    s32 high;
    s32 result;
    s32 kind;
    s32 *offset;

    high = 0;
    next.vx = arg0->position.vx + arg0->velocity.vx / 4096;
    next.vy = arg0->position.vy;
    next.vz = arg0->position.vz + arg0->velocity.vz / 4096;
    ovl_11_func_800CE550((Vec3 *)&next);
    if (arg0->position.vy >= 5001) {
        high = 1;
    }
    if (high == 0 && ((s32)ovl_11_func_800BF450() != 0 || next.vy < arg0->position.vy - 100)) {
        arg0->direction = (arg0->direction + 2) & 3;
        offset = (s32 *)(D_80123130 + arg0->direction * 8);
        arg0->velocity.vx = offset[0] * 2;
        arg0->velocity.vz = offset[1] * 2;
        arg0->position.vx += arg0->velocity.vx / 4096;
        arg0->position.vz += arg0->velocity.vz / 4096;
        if (arg0->unk2 != 0x65) {
            arg0->velocity.vx = 0;
            arg0->velocity.vz = 0;
        }
        return 0;
    }
    if (arg0->position.vy > arg0->height || high != 0) {
        if (arg0->unk2 == 0x65) {
            base = (char *)D_8006C838;
            saved = &(*(Ovl11CC7CCSpawnOwner **)(base + 0x8000 + 0x5DD4))->spawn;
            saved->id = *(u16 *)(base + 0x52C6);
            saved->position = arg0->position;
        }
        kind = Ovl11IsUpperPairedItemId(arg0->unk2);
        if (kind == 1) {
            request.count = kind;
            request.id = Ovl11MapToLowerPairedItemId(arg0->unk2);
            ovl_11_func_800CCBE8((Ovl11CC4E0Object *)arg0);
            ovl_11_func_800CB540(&request.id);
            return 1;
        }
        if (ovl_11_func_800CC4E0((Ovl11CC4E0Object *)arg0, (Vec3 *)&next) != 0) {
            return 0;
        }
        if (arg0->unk2 == 0x186) {
            goto remove;
        }
        {
            spawn = ovl_11_func_800F4994((Vec3 *)&next);
            if (spawn != NULL) {
                spawn->flags |= 0x100;
                if (D_8006C858[arg0->unk2].field_12 != 0 && ovl_11_func_800F3E44(arg0->unk2) != 0) {
                    RequestMessageWindow(3, 0x383);
                }
remove:
                ovl_11_func_800CCBE8((Ovl11CC4E0Object *)arg0);
                return 1;
            }
            if (ovl_11_func_800BF3D0() != 0 && D_8006C858[arg0->unk2].field_12 != 0) {
                ovl_11_func_800F3BCC(arg0->unk2);
                PlaySoundEffect(0x13);
                ovl_11_func_800CCBE8((Ovl11CC4E0Object *)arg0);
                SetGameFlag(7, 1);
                return 1;
            }
            if (arg0->unk2 == 0x65) {
                position.vx = arg0->position.vx;
                position.vy = arg0->position.vy;
                position.vz = arg0->position.vz;
                ovl_11_func_800CCBE8((Ovl11CC4E0Object *)arg0);
                ovl_11_func_800F43CC(0x65, position, D_80071AFE, 0);
                ovl_11_func_80109188((Ovl11D04D4Obj *)&D_800759E4, 7);
                return 1;
            }
            result = ovl_11_func_800CC6A8(arg0);
            if (result != 2) {
                return result;
            }
            Ovl11CountDownObjectEventCounters();
            {
                s32 r;
                r = 0;
                arg0->unk6 = 1;
                arg0->unk4 = 1;
                arg0->unkC = 0;
                return r;
            }
        }
    }
    return 2;
}
