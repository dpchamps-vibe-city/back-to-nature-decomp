#include "common.h"
#include "sound_commands.h"
#include "message_window.h"
#include "psyq/memory.h"
#include "ovl11_texture_stream.h"

extern s32 D_801273DC;
extern s8 D_801273E4;
extern s8 D_801273E5;

void ovl_11_func_800FFDCC(void);
void ovl_11_func_800F69D0(void);
void ovl_11_func_80100128(void);
void SetVal8005E2BC(s32 arg0);

void ovl_11_func_800FFE64(void) {
    ((struct struct_8006C838_view *)&D_8006C838)->field_0C |= 0x20000;
    ovl_11_func_800FFDCC();
    ovl_11_func_800F69D0();
    D_801273E4 = 1;
    D_801273E5 = 0;
    D_801273DC = (s32)ovl_11_func_80100128;
    PlaySoundEffect(3);
    Ovl11ServiceTextureStream(-1);
    SetMessagePortraitEnabled(0);
    SetVal8005E2BC(0);
    RequestMessageWindow(3, 0x3B5);
}
