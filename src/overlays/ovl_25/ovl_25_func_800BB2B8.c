#include "common.h"

void func_80014BCC(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void func_8001719C(u8 *arg0);
void func_8001BFA8(void *arg0, void *arg1);
void func_8001E340(void **arg0);
void func_8001E334(s32 arg0);
void *memcpy(void *dest, const void *src, u32 n);

/* Streams two CD-sector runs from the D_800BBED0 position table, copies the
 * 0xBDC-byte D_8007AFF0 work area into D_8009F78C, then publishes that copy
 * and a 0x39 value into the D_8007AFF0+0x25388/0x2547C far-buffer slots. The
 * empty memory barrier keeps the BFA8 argument loads after the copy, which is
 * what places the copy source in $a0. */
void ovl_25_func_800BB2B8(void) {
    s32 *p;
    u8 *dst;
    char *far_base;

    p = D_800BBED0;
    func_80014BCC(0, p[0], p[1] - p[0], 0, D_8007AFF0);
    func_80014BCC(0, p[1], p[2] - p[1], 0, D_8005E3B0 + 0x4290);
    func_8001719C((u8 *)(D_8005E3B0 + 0x4290));
    memcpy(&D_8009F78C, (void *)D_8007AFF0, 0xBDC);
    dst = &D_800C0454;
    __asm__("" : : : "memory");
    func_8001BFA8(dst, (void *)(D_8007AFF0 + 0x2000));
    far_base = (char *)&D_8007AFF0;
    *(s32 *)(far_base + 0x2547C) = 0x39;
    *(s32 *)(far_base + 0x25388) = (s32)&D_8009F78C;
    func_8001E340((void **)dst);
    func_8001E334(0xC8);
}
