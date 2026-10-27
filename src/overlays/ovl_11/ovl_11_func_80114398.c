#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "psyq/abs.h"
#include "psyq/libgs.h"
#include "ovl11_progress_status.h"
#include "ovl11_selection_record_reset.h"

s32 ovl_11_func_80114398(void) {
    u8 *p;
    void **q;
    s16 *a;
    s8 *c;
    s16 *b;

    Ovl11RefreshProgressStatus();
    p = gOvl11PrimarySelectionRecord;
    Ovl11ClearSelectionRows((s32 *) p);
    *(s8 *) (p + 0x5A) = 0xC;
    *(s8 *) (p + 0x5B) = 0x17;
    if (((struct Ovl11RecordFlagsView *) D_8006C838)->field_4A86 & 0x10) {
        *(s16 *) (p + 0x5C) = 1;
    } else {
        *(s16 *) (p + 0x5C) = 0;
    }

    q = (void **) gOvl11PrimarySelectionRecord;
    a = (s16 *) ((u8 *) q + 0x28);
    c = (s8 *) ((u8 *) q + 0x3C);
    b = (s16 *) ((u8 *) q + 0x46);
    if (((struct Ovl11RecordFlagsView *) D_8006C838)->field_4A86 & 0x10) {
        *q = NULL;
    } else {
        *q = (void *) ((s32) D_80051CF4 + gTextBankOffsets[1]);
    }
    q++;
    *a++ = 0x67;
    *c++ = 2;
    *b++ = -1;
    if (((struct Ovl11RecordFlagsView *) D_8006C838)->field_4A86 & 0x10) {
        *q = (void *) ((s32) D_80051D0C + gTextBankOffsets[1]);
    } else {
        *q = NULL;
    }
    q++;
    *a++ = 0x185;
    *c++ = 2;
    *b++ = -1;
    gOvl11InteractionDrawMode = 0;
    *q++ = (void *) ((s32) D_80051C7A + gTextBankOffsets[1]);
    *a++ = 0xF7;
    *c++ = 2;
    *b++ = -1;
    *q++ = (void *) ((s32) D_80051C7A + 0x12 + gTextBankOffsets[1]);
    *a++ = 0xF8;
    *c++ = 2;
    *b++ = -1;
    *q++ = (void *) ((s32) D_80051C7A + 0x24 + gTextBankOffsets[1]);
    *a++ = 0xF9;
    *c++ = 2;
    *b++ = -1;
    *q++ = (void *) ((s32) D_80051C7A + 0x36 + gTextBankOffsets[1]);
    *a++ = 0x9E;
    *c++ = 2;
    *b++ = -1;
    *q++ = (void *) ((s32) D_80051C7A + 0x50 + gTextBankOffsets[1]);
    *a++ = 0x9C;
    *c++ = 2;
    *b++ = -1;
    *q++ = (void *) ((s32) D_80051C7A + 0x6A + gTextBankOffsets[1]);
    *a++ = 0x5C;
    *c++ = 2;
    *b++ = -1;
    *q = (void *) ((s32) D_80051C7A + 0xA8 + gTextBankOffsets[1]);
    *a = 0x60;
    *c = 2;
    *b = -1;
    return 1;
}
