#include "common.h"
#include "psyq/memory.h"
#include "cd_loader.h"
#include "projected_primitives.h"

void func_8001719C(u8 *arg0);
void func_8001E340(void **arg0);
void SetVal8005E51C(s32 arg0);

/* dst is name-bound to D_800C0DD8 before the copy so the address is
 * materialised in the aligned/unaligned copy arms; the empty memory
 * barrier keeps the BFA8 argument loads after the copy, which is what
 * places the copy source in $a0. */
void ovl_21_func_800BB8B8(void) {
    s32 *p;
    u8 *dst;

    p = D_800BC894;
    CdReadDiscSectorsBlocking(0, p[0], p[1] - p[0], 0, D_8007AFF0);
    CdReadDiscSectorsBlocking(0, p[1], p[2] - p[1], 0, gGraphicsHeapBase + 0x4290);
    func_8001719C((u8 *)(gGraphicsHeapBase + 0x4290));
    memcpy(&D_8009F78C, (void *)D_8007AFF0, 0xBDC);
    dst = &gOvl21SceneGeometry;
    __asm__("" : : : "memory");
    InitProjectedPrimitiveSource(dst, (void *)(D_8007AFF0 + 0x2000));
    func_8001E340((void **)dst);
    SetVal8005E51C(0xC8);
}
