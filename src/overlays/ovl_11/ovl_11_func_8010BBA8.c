#include "common.h"
#include "game_types.h"
#include "message_window.h"
#include "ovl11_cd_audio_request.h"

typedef struct {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ char pad_02[0x30 - 0x02];
    /* 0x30 */ s16 unk30;
} Struct_8010BBA8;

s32 ovl_11_func_8010BBA8(Struct_8010BBA8 *arg0) {
    char *far_base;

    if (arg0->unk0 == 0) {
        return -1;
    }
    if (D_80075AEA < 9) {
        RequestMessageWindow(2, 0x258);
    } else if (D_80075AEA >= 0xC9) {
        RequestMessageWindow(2, 0x259);
    } else {
        RequestMessageWindow(2, 0x258);
    }
    far_base = (char *)&D_8007AFF0;
    if (*(s16 *)(far_base + 0x25476) == arg0->unk30) {
        Ovl11RequestCdAudioTrack(0xD);
    }
    return -1;
}
