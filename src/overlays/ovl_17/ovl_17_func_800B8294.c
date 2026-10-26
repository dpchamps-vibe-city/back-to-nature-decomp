#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "ovl17_record_work_init.h"

void ovl_17_func_800B90F8(void);

void ovl_17_func_800B8294(void) {
    s16 temp_v0;
    Ovl11CardView *work;
    s16 *var_a2;
    s16 *cards;
    s16 var_a1;
    s32 var_a3;
    s32 offset;
    u8 *var_a0;
    u8 *var_a1_3;
    s32 *names;

    Ovl17ResetCardRows();
    work = (Ovl11CardView *)D_8006C838;
    var_a1 = 2;
    if (((Ovl17ModeView *)D_8006C838)->mode != 3) {
        var_a1 = ((Ovl17ModeView *)D_8006C838)->mode;
    }
    names = D_800BB534;
    offset = var_a1 * 0x54;
    cards = (s16 *)((u8 *)work + 0xE516);
    var_a2 = (s16 *)((u8 *)cards + offset);
    var_a0 = gOvl17RowSlotTargets;
    for (var_a3 = 0; var_a3 < 6; var_a3++) {
        temp_v0 = *var_a2;
        var_a2 += 7;
        ((Ovl17RowsView *)gOvl17RecordWorkBase)->rows[var_a3].data = var_a0;
        var_a0 += 6;
        ((Ovl17RowsView *)gOvl17RecordWorkBase)->rows[var_a3].flags = 0;
        ((Ovl17RowsView *)gOvl17RecordWorkBase)->rows[var_a3].nameOffset = names[temp_v0];
    }
    if (((Ovl17ModeView *)D_8006C838)->mode == 3) {
        var_a1_3 = gOvl17SpriteHeaderVariantA;
        ((Ovl17RowsView *)gOvl17RecordWorkBase)->rows[2].flags |= 1;
    } else {
        var_a1_3 = gOvl17SpriteHeaderVariantB;
    }
    InitSpriteSourceData((SpriteSourceData *)D_800BD930, (SpriteDataHeader *) var_a1_3);
    (*(s16 *) (D_800BD930 + 0x1E4)) = -1;
    ovl_17_func_800B90F8();
}
