#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_upload.h"
#include "message_buffer_access.h"

s32 D_8005E45C;
s16 D_8005E468;
u8 *D_8005E46C;

void func_80019FC4(s16 arg0) {
    if ((D_8005E468 != arg0) && (D_8005E46C != 0)) {
        UploadSpriteImages(D_8005E46C);
        ClearMessageReadBufferBusy();
        D_8005E468 = arg0;
        D_8005E45C = 0;
    }
}
