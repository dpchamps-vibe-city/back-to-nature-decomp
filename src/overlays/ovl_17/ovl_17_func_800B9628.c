#include "common.h"

void ovl_17_func_800B9628(void) {
    Ovl17SelView *v;
    Ovl17Rec *records;
    s16 best;
    s32 i;
    s32 j;
    s16 *selection;
    s32 selectionIndex;
    Ovl17SelView *lookup;
    s16 *cursor;

    v = (Ovl17SelView *) D_800BD848;
    if (v->count == 0) {
        v->sel[0] = -1;
    }
    i = 0;
    records = v->recs;
    for (; i < 6; i++) {
        if (records[i].field0 > 0x7FFFFF) {
            j = 0;
            if (v->sel[0] != i) {
                lookup = (Ovl17SelView *)D_800BD848;
                cursor = lookup->sel;
                do {
                    j++;
                    cursor++;
                    if (j >= 6) {
                        break;
                    }
                } while (*cursor != i);
            }
            if (j == 6) {
                selection = v->sel;
                selectionIndex = v->count;
                *(s16 *)((u32)(selectionIndex * 2) + (u32)selection) = i;
                v->count = v->count + 1;
            }
        }
    }
    {
        u8 *b;
        s32 *p;
        u8 *active;
        u8 *destination;

        b = D_800BD848;
        if (*(s16 *) (b + 0x2D4) == 0) {
            best = 0;
            i = 0;
            active = D_800BD848;
            p = (s32 *) (active + 0x38);
            for (; i < 6; i++) {
                for (j = 0; j < 6; j++) {
                    b = active + 0x38 + j * 0x50;
                    if (*(s32 *) ((u8 *) p + best * 0x50) < *(s32 *)b) {
                        best = j;
                    }
                }
            }
            destination = D_800BD848;
            *(s16 *) (destination + 0x2BC) = best;
        }
    }
}
