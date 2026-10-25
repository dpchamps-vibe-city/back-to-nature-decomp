#include "common.h"
#include "game_types.h"
#include "aged_item_pool.h"
#include "sound_commands.h"
#include "message_window.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "game_flags.h"
#include "ovl11_interaction_id_predicates.h"

s32 ovl_11_func_800BF3F4(void);
/* This caller supplies two arguments; the callee's trailing debug arguments
 * do not participate in sprite initialization. */
s32 ovl_11_func_800C79F8(Vec3 *arg0);
void ovl_11_func_800F0FB0(s32 arg0);

s32 ovl_11_func_800CC4E0(Ovl11CC4E0Object *arg0, Vec3 *arg1) {
    s32 temp_v0;
    s32 temp_v0_2;
    u16 temp_a0;
    char *far_base;
    char *main_base;

    temp_v0 = ovl_11_func_800BF3F4();
    if (temp_v0 != 0) {
        InitSpriteSourceData(&D_80128C80, (SpriteDataHeader *) &gSharedSpriteData);
        SelectSpriteAnimation((ObjectState *) &D_80128C80, 0x17);
        PlaySoundEffect(0x10);
        arg0->unk6 = 3;
        arg0->unkC = 0;
        if (temp_v0 == 2) {
            temp_a0 = *(u16 *) ((u8 *) arg0 + 2);
            if (((u32) (temp_a0 - 0xA1) < 3U) && (Ovl11AddAgedItem((s32) (s16) temp_a0) == 0)) {
                RequestMessageWindow(3, 0x382);
            }
            if ((s16) (*(u16 *) ((u8 *) arg0 + 2)) == 0x84) {
                Ovl11QueueAgedItemUpdate();
            }
        } else {
            far_base = (char *)&D_8007AFF0;
            if (((*(Ovl11CC4E0Head **) (far_base + 0x25388))->unk2 == 0x2C) && ((s16) (*(u16 *) ((u8 *) arg0 + 2)) == 0xA3)) {
                SetGameFlag(0x5EU, 1U);
            }
            if ((temp_v0 != 3) && ((s16) (*(u16 *) ((u8 *) arg0 + 2)) == 0x186)) {
                D_8012DB80 = 1;
            }
        }
        temp_v0_2 = ovl_11_func_800C79F8(arg1);
        if (temp_v0_2 == 0) {
            goto success;
        }
        switch (temp_v0_2) {
        case 38:
            if (Ovl11IsInteractionItemSetA((s16) (*(u16 *) ((u8 *) arg0 + 2))) != 0) {
                arg0->unk6 = 4;
            }
            break;
        case 44:
            main_base = (char *)D_8006C838;
            (*(u16 *) (main_base + 0x51F0)) = (u16) (*(u16 *) ((u8 *) arg0 + 2));
            if ((s16) (*(u16 *) ((u8 *) arg0 + 2)) == 0xA4) {
                (*(u16 *) (main_base + 0x51F0)) = (u16) (*(u16 *) (main_base + 0x5254));
            }
            ovl_11_func_800F0FB0(4);
            break;
        default:
            return 1;
        }
success:
        return 1;
    } else {
        return 0;
    }
}
