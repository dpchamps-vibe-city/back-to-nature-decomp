#include "common.h"
#include "scene_selection.h"
#include "sound_commands.h"
#include "ovl11_choice_messages.h"

void ovl_11_func_800BD5A0(void);
void ovl_11_func_800BFEA4(void);
void ovl_11_func_800BFF00(void);
void ovl_11_func_800C044C(s16 arg0);
void ovl_11_func_800CBEF8(void);
void ovl_11_func_800F6630(void);
void ovl_11_func_800BFE3C(void);
extern s32 D_801287F4;

typedef struct {
    char pad_000[0xC];
    s32 field_0C;
    char pad_010[0x4438 - 0x10];
    s16 field_4438;
    char pad_443A[0x4450 - 0x443A];
    s32 field_4450;
    char pad_4454[0x4488 - 0x4454];
    s32 field_4488;
    char pad_448C[0xE4C8 - 0x448C];
    s16 field_E4C8;
} Ovl11Init801048A0View;

void ovl_11_func_801048A0(void) {
    Ovl11Init801048A0View *base;
    char *far;

    base = (Ovl11Init801048A0View *)&D_8006C838;
    far = (char *)&D_8007AFF0;

    base->field_0C &= 0xFF7FFFFF;
    *(s32 *)(far + 0x25484) = 0xFFFF;
    *(s32 *)(far + 0x25488) = 0xFFFF;

    ovl_11_func_800BD5A0();
    ovl_11_func_800BFEA4();
    ovl_11_func_800BFF00();
    ovl_11_func_800C044C(base->field_E4C8);
    ovl_11_func_800CBEF8();
    Ovl11ResetChoiceMessages();
    ovl_11_func_800F6630();

    base->field_4488 = 1;
    D_801287F4 = 10;
    *(s32 *)(far + 0x2548C) = 0;
    *(s16 *)(far + 0x247A8) = -1;
    *(s16 *)(far + 0x247A6) = -1;
    *(s16 *)(far + 0x2479C) = -1;
    *(s32 *)(far + 0x25498) = -1;
    *(s32 *)(far + 0x25390) = 0;
    *(s32 *)(far + 0x2549C) = -1;
    base->field_4438 = -1;
    QueueSoundLoadCommand(1000, 0);

    base->field_4450 &= ~0xF80;
    if (GetPreviousSceneId() == 3) {
        base->field_4450 |= 0x80;
        ovl_11_func_800BFE3C();
    }
    if (GetPreviousSceneId() == 4) {
        base->field_4450 |= 0x100;
        ovl_11_func_800BFE3C();
    }
    if (GetPreviousSceneId() == 5) {
        base->field_4450 |= 0x200;
        ovl_11_func_800BFE3C();
    }
    if (GetPreviousSceneId() == 0xB) {
        base->field_4450 |= 0x400;
        ovl_11_func_800BFE3C();
    }
    if (GetPreviousSceneId() == 0xC) {
        base->field_4450 |= 0x800;
        ovl_11_func_800BFE3C();
    }
}
