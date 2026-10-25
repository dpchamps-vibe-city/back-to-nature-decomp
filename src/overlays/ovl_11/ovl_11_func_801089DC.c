#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_upload.h"

void ovl_11_func_800BD9D4(s32 arg0);

void ovl_11_func_801089DC(void) {
    ovl_11_func_800BD9D4(0x12);
    UploadSpriteImagesAtOffsets(((u8 *)(((s32)(&D_8007BFF8)))), D_8012D060.unk0, D_8012D060.unk2, D_8012D060.unk4, D_8012D060.unk6);
}
