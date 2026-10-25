#include "common.h"
#include "asset_scratch.h"
#include "screen_fade.h"
#include "cd_loader.h"

/* The callee's four-byte flag aggregates use the scalar word calling ABI. */
void ovl_11_func_800E499C(s32);
s32 ovl_11_func_800EEBC8();
void ovl_11_func_800E54C8(void);

s32 ovl_11_func_800E4330(void) {
    s16 index;
    s32 offset;
    s32 size;
    u8 *data;
    u8 *base;

    index = Ovl11CurrentObjectId[1];
    offset = D_801240D8[index];
    size = D_801240D8[index + 1] - offset;
    do {
        data = CdReadArchiveBytes(12, offset, size, (u8 *)GetAssetScratchStart(), 0, 0);
        if (data == 0 && IsScreenFadeComplete() != 0) {
            return 0;
        }
    } while (data == 0);
    ovl_11_func_800E499C((s32)data);
    Ovl11RecordCursor = D_8006E910;
    D_801291B0 = D_8006E910;
    D_80129224 = 0;
    ovl_11_func_800EEBC8(0, 0, 0, 0);
    base = (u8 *)D_8006E910 - 0x20D8;
    ovl_11_func_800E54C8();
    *(s16 *)(base + 0x4438) = -1;
    *(s32 *)(base + 0x7A74) = 1;
    return 0;
}
