#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libcd.h"
#include "psyq/memory.h"
#include "psyq/libapi.h"
#include "psyq/libsnd.h"
#include "asset_scratch.h"
#include "screen_fade.h"
#include "cd_loader.h"
#include "gte_scene_state.h"
#include "aux_sprite_display.h"
#include "cd_audio.h"
#include "master_volume.h"
#include "sound_driver.h"
#include "sound_commands.h"
#include "ovl11_selection_message.h"
#include "ovl11_clock_calendar.h"
#include "ovl11_vertex_blend.h"
#include "message_window.h"
#include "sprite_source.h"
#include "random.h"
#include "sprite_upload.h"
#include "debug_break.h"
#include "render_packet_pool.h"
#include "ovl11_choice_messages.h"
#include "ovl11_sprite_effect_pool.h"
#include "game_flags.h"
#include "psyq/abs.h"
#include "ovl11_object_event_script.h"
#include "ovl11_object_event_pool.h"

typedef struct {
               s16 unk0;
               s16 unk2;
               u16 unk4;
               s16 pad6;
               s32 unk8;
               s32 unkC;
               s32 unk10;
               s32 unk14;
} M2C_5b7450153aff_Ov11_4390struct;

int CdControl (u_char com, u_char *param, u_char *result);
void CdFlush (void);
CdlLOC *CdIntToPos (int i, CdlLOC *p);
int CdRead (int sectors, u_long *buf, int mode);
void CdReadBreak (void);
int CdReadSync (int mode, u_char *result);
int CdSync (int mode, u_char *result);
void SsSetSerialVol (char, short, short);
int StoreImage (RECT *rect, u_long *p);
void SystemError (char, long);
void *memmove (unsigned char *, const unsigned char *, int);
void SetSpriteSourceHeaderAndRecord (SpriteSourceData *src, s32 header, s32 field_18);
void DecodeAndUploadSpriteImages (u8 *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s32 arg5);
s32 *func_80021FE4 (void);
s16 GetMessageWindowArchiveId (void);
void func_800179E8 (void);
void func_80022008 (void);
void ovl_11_func_800BD160 (void);
void ovl_11_func_800BD168 (void);
void ovl_11_func_800CD0C0 (s32 *arg0);
void ovl_11_func_800CCC5C (void);
void ovl_11_func_800F4114 (void);
void ovl_11_func_800F0FB0 (s32 arg0);
void ovl_11_func_800F1038 (u16 arg0, s32 arg1);
void ovl_11_func_800D3200 (s32 arg0);
s32 ovl_11_func_800E39B8 (s32 *arg0, u32 arg1);
void ovl_11_func_800E5230 (void);
void ClearActiveOrderingTables (void);
s32 ovl_11_func_800E3978 (s32 arg0);
s32 ovl_11_func_800EEBC8 (void);
void ovl_11_func_800E54C8 (void);
s32 ovl_11_func_800E4AEC (s32 arg0);
s32 ovl_11_func_800E4C30 (s32 arg0);
s32 ovl_11_func_800E4C84 (s32 arg0);
s32 ovl_11_func_800E4B6C (s32 arg0, s32 arg1);
void ovl_11_func_800E4BA4 (s32 arg0);
void ovl_11_func_800E4D08 (s32 arg0);
s32 ovl_11_func_800E4B58 (s32 arg0, s32 arg1);
void ovl_11_func_800F4390 (M2C_5b7450153aff_Ov11_4390struct *arg0, s32 arg1);
s32 ovl_11_func_800E63C8(s16 arg0, s16 arg1, s32 arg2, s32 arg3);
void ovl_11_func_80107DD0 (s16 *arg0);
s32 ovl_11_func_80112A84 (void);
s32 ovl_11_func_800FEB14 (void);
s32 ovl_11_func_800FEB68 (void);
s32 ovl_11_func_800FEBB0 (void);
s32 ovl_11_func_800FEAD4 (void);
s32 ovl_11_func_800F3C9C (u16 arg0);
void func_80021D64 (void);
void ovl_11_func_80104A80 (void);
void ovl_11_func_801047FC (void);
void ovl_11_func_800D6628 (void);
void ovl_11_func_800BFEA4 (void);
void ovl_11_func_800BFF00 (void);
void ovl_11_func_800DBFA0 (void);
void ovl_11_func_800DAFD4 (void);
void ovl_11_func_800DB0E4 (void);
void ovl_11_func_800DBFC4 (void);
void ovl_11_func_800D7B24 (void);
void ovl_11_func_800BD358 (void);
void ovl_11_func_800BD5A0 (void);
void ovl_11_func_800BD538 (void);
void ovl_11_func_800DC990 (void);
void ovl_11_func_800F6630 (void);
void ovl_11_func_800CBEF8 (void);
s32 *ovl_11_func_80111E38 (void);
void ovl_11_func_800F2508 (void);
void ovl_11_func_80104898 (void);
void ovl_11_func_800FFA28 (void);
void ovl_11_func_801128B4 (void);
void ovl_11_func_800BD668 (void);
void ovl_11_func_80104A58 (void);
void ovl_11_func_80104A88 (void);

void ovl_21_func_800BB59C(void) {
    DrawSync(0);
    ClearOTagR((u32 *) gActiveRenderContext->field_120, 0x800);
    CdReadArchiveBytes(0, 0, 0x2000, gGraphicsHeapBase + 0x4290, 1, 1);
    do {

    } while (CdReadArchiveBytes(0, 0, 0x2000, gGraphicsHeapBase + 0x4290, 1, 0) == 0);
    UploadSpriteImages(gGraphicsHeapBase + 0x4290);
}
