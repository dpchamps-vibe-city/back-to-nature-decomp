#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "sound_commands.h"
#include "psyq/libgte.h"
#include "ovl11_cell_actions.h"
#include "psyq/libgpu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "psyq/abs.h"
#include "psyq/libgs.h"
#include "psyq/libsnd.h"
#include "psyq/libapi.h"
#include "ovl11_item_table_queries.h"

typedef struct {
    char pad0[2];
    s16 unk2;
    u16 unk4;
} ReconA0View;

typedef struct {
    s16 unk0;
    char pad2[2];
    u16 unk4;
} F413CArg1;

typedef struct {
    char pad0[2];
    s16 unk2;
    u16 unk4;
} F413CArg0;

typedef struct {
    char pad0[2];
    s16 unk2;
    u16 unk4;
} CEEF8Arg0;

typedef struct {
    char pad0[0x50];
    Ovl11CellActionEntry *unk50;
    s32 unk54;
} Func800C7878Arg0;

s16 ovl_11_func_800F4240(ReconA0View *arg0);
s32 ovl_11_func_800F413C(F413CArg0 *arg0, F413CArg1 *arg1);
s32 ovl_11_func_800CEEF8(CEEF8Arg0 *arg0);

extern Ovl11CellActionEntry *gOvl11InteractionHitRecord;
extern s32 gOvl11InteractionHitFlags;

s32 ovl_11_func_800C7878(Func800C7878Arg0 *arg0, F413CArg1 *arg1) {
    s32 temp_v0;
    u32 temp_v1;

    arg0->unk50 = gOvl11InteractionHitRecord;
    arg0->unk54 = gOvl11InteractionHitFlags;
    temp_v1 = gOvl11InteractionHitFlags & 0x8928;
    switch (temp_v1) {                              /* irregular */
    case 0x8000:
        temp_v0 = Ovl11ItemHasFlag8100(ovl_11_func_800F4240((ReconA0View *) gOvl11InteractionHitRecord));
        if ((temp_v0 == 1) && (ovl_11_func_800F413C((F413CArg0 *) gOvl11InteractionHitRecord, arg1) == temp_v0)) {
            PlaySoundEffect(7);
        }
        break;
    case 0x100:
        PlaySoundEffect(7);
        if (ovl_11_func_800CEEF8((CEEF8Arg0 *) arg0->unk50) != 2) {
            arg1->unk0 = arg0->unk50->u0.unk0;
        } else {
            arg1->unk0 = 0x65;
        }
        (*(s16 *) ((u8 *) arg1 + 2)) = 0;
        arg1->unk4 = 1;
        break;
    case 0x800:
    case 0x8:
        PlaySoundEffect(7);
        arg1->unk0 = arg0->unk50->u0.unk0;
        (*(s16 *) ((u8 *) arg1 + 2)) = 0;
        arg1->unk4 = 1;
        ovl_11_func_800CEEF8((CEEF8Arg0 *) arg0->unk50);
        break;
    case 0x20:
        PlaySoundEffect(7);
        arg1->unk0 = Ovl11ResolveCellItemState(arg0->unk50);
        (*(s16 *) ((u8 *) arg1 + 2)) = 0;
        arg1->unk4 = 1;
        break;
    }
    gOvl11InteractionHitFlags = 0;
    gOvl11InteractionHitRecord = NULL;
    return 0;
}
