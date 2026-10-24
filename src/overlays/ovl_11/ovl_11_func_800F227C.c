/* For each of the 37 records, find the first of its 30 entries (8-byte
 * stride at +0xE4) whose key is at least arg0, store that index in unk22 and
 * copy the entry's three fields into unkE, unk2C and unk2E. A key of 0x168
 * leaves every record unchanged. */
#include "common.h"

void ovl_11_func_800F227C(s16 arg0) {
    struct_80076220 *rec;
    struct_80076220_entry *en;
    u8 *b1;
    u16 *b2;
    u16 *b3;
    u8 *p1;
    u16 *p2;
    u16 *p3;
    s16 temp;
    s32 i;
    s32 j;

    if (arg0 != 0x168) {
        rec = &D_80076220;
        for (j = 0; j < 37; j++, rec++) {
            en = (struct_80076220_entry *)((u8 *)rec + 0xE4);
            for (i = 0; i < 30; i++, en++) {
                temp = en->unk0;
                if (temp < arg0) {
                    continue;
                }
                if (arg0 < temp) {
                    rec->unk22 = (i < 0) ? 0 : i;
                } else {
                    rec->unk22 = i;
                }
                rec->unkC = 0;
                b1 = (u8 *)rec + 0xE6;
                p1 = (void *)((u8 *)b1 + (u32)rec->unk22 * 8);
                rec->unkE = *p1;
                b2 = (u16 *)((u8 *)rec + 0xE8);
                p2 = (void *)((u8 *)b2 + (u32)rec->unk22 * 8);
                rec->unk2C = *p2;
                b3 = (u16 *)((u8 *)rec + 0xEA);
                p3 = (void *)((u8 *)b3 + (u32)rec->unk22 * 8);
                rec->unk2E = *p3;
                break;
            }
        }
    }
}
