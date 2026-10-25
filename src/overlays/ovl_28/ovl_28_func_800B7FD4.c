#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libsnd.h"
#include "psyq/libgpu.h"
#include "master_volume.h"
#include "sound_driver.h"
#include "sound_commands.h"


short SsSeqOpen (unsigned long *, short);
void SsSetMono (void);
void SsSetStereo (void);
void SsStart (void);
void SsUtReverbOn (void);
void ovl_28_func_800B8C94 (void);
void ovl_28_func_800B8CE4 (void);
s32 ovl_28_func_800B8D48 (void);
void StepMasterVolumeRamp (void);
void func_8001719C (u8 *arg0);
void func_80015704(SpriteSourceData *out, SpriteDataHeader *header);
void func_80015840(ObjectState *obj, u8 arg1);

void ovl_28_func_800B7FD4(void) {
    s32 *base;

    switch (D_800B961C) {                           /* irregular */
    case 0x0:
        D_800B961C = 0xA;
        return;
    case 0xA:
        ovl_28_func_800B8C94();
        D_800B961C = (u16) D_800B961C + 1;
        return;
    case 0xB:
        UpdateMasterVolumeAndSoundTransfer();
        if (IsSoundTransferComplete() != 0) {
            BeginMasterVolumeFadeIn(0xA);
            D_800B961C = (u16) D_800B961C + 1;
        }
        return;
    case 0xC:
        if (GetMasterVolumeRampState() == 0) {
            D_800B961C = 0x14;
            return;
        }
        break;
    case 0x14:
        ovl_28_func_800B8CE4();
        D_800B961C = (u16) D_800B961C + 1;
        return;
    case 0x15:
        if (ovl_28_func_800B8D48() != 0) {
            D_800B961C = 0xFF;
            return;
        }
        break;
    case 0xFF:
        D_800B961C = 0;
        OpenSoundSequencesAndStart();
        PlaySoundSequenceWithReverb(0, 0, 0x23, 0x7F);
        base = (s32 *)&D_8006C838;
        base[0x1122] = base[0x1122] + 1;
        break;
    }
}
