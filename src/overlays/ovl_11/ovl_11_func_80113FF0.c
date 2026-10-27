#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "psyq/abs.h"
#include "psyq/libgs.h"
#include "ovl11_progress_status.h"
#include "ovl11_cell_grid_lookup.h"
#include "ovl11_selection_record_reset.h"


s32 ovl_11_func_80113FF0(void) {
    s32 temp_s3;
    u8 *p;
    void **q;
    s16 *a;
    s8 *c;
    s16 *b;

    Ovl11RefreshProgressStatus();
    gOvl11QuantityPrompt.unk0 = (s32) ((void *) ((u8 *) &D_80051E26 + gTextBankOffsets[1]));
    *(s32 *) ((u8 *) &gOvl11QuantityPrompt + 4) = (s32) D_80051E26 - 0x60C + gTextBankOffsets[1];
    temp_s3 = gProgressDerivedFlags;
    gOvl11QuantityPrompt.unkA = Ovl11GetItemPrice(0x69U);
    gOvl11QuantityPrompt.unk8 = 1;
    Ovl11ClearSelectionRows((s32 *) gOvl11PrimarySelectionRecord);

    p = gOvl11PrimarySelectionRecord;
    *(s8 *) (p + 0x5A) = 0xF;
    *(s8 *) (p + 0x5B) = 0x15;
    *(s16 *) (p + 0x5C) = 0;

    q = (void **) p;
    a = (s16 *) ((u8 *) q + 0x28);
    c = (s8 *) ((u8 *) q + 0x3C);
    b = (s16 *) ((u8 *) q + 0x46);

    *q = (void *) ((s32) &D_80051E26 + gTextBankOffsets[1]);
    *(s16 *) ((u8 *) q + 0x28) = 0x69;
    *(s8 *) ((u8 *) q + 0x3C) = 0;
    *(s16 *) ((u8 *) q + 0x46) = 0x47;
    q++;
    a++;
    c++;
    b++;
    *q++ = (void *) ((s32) &D_80051E26 + 0x1A + gTextBankOffsets[1]);
    *a++ = 0x109;
    *c++ = 0;
    *b++ = 0x4A;
    *q++ = (void *) ((s32) &D_80051E26 + 0x32 + gTextBankOffsets[1]);
    *a++ = 0;
    *c++ = 3;
    *b++ = 0x49;
    if (temp_s3 & 0x4000) {
        *q = (void *) ((s32) &D_80051E26 + 0x4C + gTextBankOffsets[1]);
    } else {
        *q = NULL;
    }
    *a = 0x1E;
    *c = 0;
    *b = 0x48;
    gOvl11InteractionDrawMode = 0;
    return 1;
}
