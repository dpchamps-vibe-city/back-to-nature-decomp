/*
 * guarded_sprite_bank — reconstructed translation unit.
 *
 * Guarded sprite-bank archive reload and upload. The same game-flag/work-bit
 * gate protects both entry points; the upload x offset is 0 or 256 VRAM words
 * and stage 44 suppresses uploads.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "cd_loader.h"
#include "game_flags.h"
#include "ovl11_guarded_sprite_bank.h"

s32 ovl_11_func_800E351C(u8 *buffer, s16 xOffset, s16 yOffset);

s32 Ovl11ReloadGuardedSpriteBank(void) {
    s32 bufferAddress;

    if (GetGameFlag(2U) != 1) {
        if (D_8006C844 & 0x18000) {
            return 0;
        }
        DrawSync(0);
        ClearOTagR((u32 *) gActiveRenderContext->field_120, 0x800);
        bufferAddress = gSpriteBankArchiveBuffer;
        CdLoadArchiveRangeBlocking(0x1D, 0x1E, bufferAddress, bufferAddress);
        Ovl11UploadGuardedSpriteBank((u8 *) bufferAddress);
    }
    return 0;
}

void Ovl11UploadGuardedSpriteBank(u8 *buffer) {
    char *stageWork;
    s16 xOffset;

    if ((GetGameFlag(2U) != 1) && !(D_8006C844 & 0x18000)) {
        stageWork = (char *)&D_8007AFF0;
        if (*(s16 *)(stageWork + 0x25476) != 0x2C) {
            xOffset = ((*(u16 *)(*(s32 *)(stageWork + 0x25388) + 8) & 0x8000) == 0) << 8;
            if (*(s16 *)(stageWork + 0x25476) == 0x1B) {
                xOffset = 0;
            }
            ovl_11_func_800E351C(buffer, xOffset, 0);
            gOvl11SpriteBankVramXOffset = xOffset;
        }
    }
}
