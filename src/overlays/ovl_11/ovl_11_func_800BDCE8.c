#include "common.h"

typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
} Ovl11Func800BDCE8Vec;

typedef struct {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ s32 unk4;
    /* 0x8 */ s32 unk8;
} Ovl11Func800BDCE8Arg1;

void ovl_11_func_800F0FB0(s32 arg0);
void ovl_11_func_800BD358(void);

void *ovl_11_func_800BDCE8(u8 *arg0, Ovl11Func800BDCE8Arg1 *arg1, s32 arg2) {
    u32 i;
    s32 off;
    u8 *p;
    u8 *ret;
    Ovl11Func800BDCE8Vec lo;
    Ovl11Func800BDCE8Vec hi;
    u16 flags;
    u16 flags2;
    char *base;

    for (i = 0, base = (char *)&D_8006C838, ret = arg0 + 0x10, p = arg0, off = 0;
         i < 0x32; ret += 0x2C, p += 0x2C, i++, off += 0x2C) {
        /* The second address is written as a foldable spelling of `off` so the
         * original base-and-copy pair survives: both loads read arg0 + off, one
         * through the recomputed temporary. */
        flags2 = *(u16 *)(arg0 + (off * 2 - off) + 0x12);
        flags = *(u16 *)(arg0 + off + 0x10);
        if (flags2 != 0xFFFF && !(flags & 0x6008)) {
            lo = *(Ovl11Func800BDCE8Vec *)(p + 0x1E);
            hi = *(Ovl11Func800BDCE8Vec *)(p + 0x24);
            if (lo.x < arg1->unk0 && arg1->unk0 < hi.x &&
                lo.z < arg1->unk8 && arg1->unk8 < hi.z) {
                if (flags & 0x100) {
                    if (arg2 != 0) {
                        ovl_11_func_800F0FB0(7);
                    }
                    continue;
                }
                if (flags & 0x80) {
                    if (arg2 != 0) {
                        *(u16 *)(base + 0x51FE) |= 4;
                    }
                    continue;
                }
                if (flags2 != 0xFFFE) {
                    ovl_11_func_800BD358();
                }
                return ret;
            }
        }
    }
    return 0;
}
