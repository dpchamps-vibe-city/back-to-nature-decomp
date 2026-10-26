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

s32 ovl_11_func_800D6014(u16 arg0);
void ovl_11_func_8011D400(s32 *arg0);
s32 ovl_11_func_800E6EE0(s16 arg0, s16 arg1, s32 arg2, s32 arg3);

s32 ovl_11_func_80113CD0(void) {
    s32 flags;
    Ovl11D8012D538 *header;
    u8 *p;
    void **q;
    s16 *a;
    s8 *c;
    s16 *b;

    Ovl11RefreshProgressStatus();
    header = &gOvl11QuantityPrompt;
    header->unk0 = (s32) D_80051E92 + gTextBankOffsets[1];
    *(s32 *) ((u8 *) header + 4) = (s32) D_80051E92 - 0x678 + gTextBankOffsets[1];
    flags = gProgressDerivedFlags;
    header->unkA = ovl_11_func_800D6014(0x68U);
    header->unk8 = 1;
    ovl_11_func_8011D400((s32 *) gOvl11PrimarySelectionRecord);

    p = gOvl11PrimarySelectionRecord;
    *(s8 *) (p + 0x5A) = 0xF;
    *(s8 *) (p + 0x5B) = 0x15;
    *(s16 *) (p + 0x5C) = 0;
    q = (void **) p;
    a = (s16 *) ((u8 *) q + 0x28);
    c = (s8 *) ((u8 *) q + 0x3C);
    b = (s16 *) ((u8 *) q + 0x46);
    *q = (void *) ((s32) D_80051E92 + gTextBankOffsets[1]);
    *(s16 *) ((u8 *) q + 0x28) = 0x68;
    *(s8 *) ((u8 *) q + 0x3C) = 0;
    *(s16 *) ((u8 *) q + 0x46) = 0x63;
    q++;
    a++;
    c++;
    b++;
    if (flags & 0x200000) {
        *q = (void *) ((s32) D_80051E92 + 0xE + gTextBankOffsets[1]);
    } else {
        *q = NULL;
    }
    q++;
    *a++ = 0x1F;
    *c++ = 0;
    *b++ = 0x65;
    if (flags & 0x400000) {
        *q = (void *) ((s32) D_80051EB8 + gTextBankOffsets[1]);
    } else {
        *q = NULL;
    }
    q++;
    *a++ = 0x20;
    *c++ = 0;
    *b++ = 0x66;
    *q++ = (void *) ((s32) D_80051ED0 + gTextBankOffsets[1]);
    *a++ = 0x161;
    *c++ = 0;
    *b++ = 0x67;
    *q++ = (void *) ((s32) D_80051ED0 + 0x10 + gTextBankOffsets[1]);
    *a++ = 0x165;
    *c++ = 0;
    *b++ = 0x68;
    *q++ = (void *) ((s32) D_80051ED0 + 0x24 + gTextBankOffsets[1]);
    *a++ = 0;
    *c++ = 3;
    *b++ = 0x69;
    *q++ = (void *) ((s32) D_80051ED0 + 0x36 + gTextBankOffsets[1]);
    *a++ = 0;
    *c++ = 3;
    *b++ = 0x6A;
    if (flags & 0x4000) {
        *q = (void *) ((s32) D_80051ED0 - 0x5E + gTextBankOffsets[1]);
    } else {
        *q = NULL;
    }
    q++;
    *a++ = 0x1E;
    *c++ = 0;
    *b++ = 0x64;
    if (ovl_11_func_800E6EE0(0x1D, 0, 2, 0) != 1) {
        *q = (void *) ((s32) D_80051F1C + gTextBankOffsets[1]);
    } else {
        *q = NULL;
    }
    *a = 0x1D;
    *c = 0;
    *b = 0x281;
    gOvl11InteractionDrawMode = 0;
    return 1;
}
