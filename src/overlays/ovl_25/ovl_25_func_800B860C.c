#include "common.h"
#include "sound_commands.h"
#include "message_window.h"

extern s16 D_800BFE44;
extern u8 gOvl25SequenceCyclePhase;
extern s16 gOvl25SequenceAnchor4[4];

void ovl_25_func_800B93E4(void);
void SetVal8005E2BC(s32 arg0);
void ovl_25_func_800B9B00(s16 *arg0, s16 arg1, s16 arg2);
void ovl_25_func_800B8340(void);

void ovl_25_func_800B860C(void) {
    char *ref;
    char *base;
    char *p;

    ovl_25_func_800B93E4();
    ref = (char *)&D_800BFE44;
    if (*(s16 *)(ref + 0x4CC) == 0) {
        CloseMessageWindow();
        SetVal8005E2BC(0);
        SetMessagePortraitEnabled(0);
        RequestMessageWindow(4, 0x35);
        ovl_25_func_800B9B00((s16 *)(ref + 0x2E0), 9, 0x3B);
    }
    *(u16 *)(ref + 0x4CC) = *(u16 *)(ref + 0x4CC) + 1;
    if (*(s16 *)(ref + 0x4CC) >= 0x3C) {
        ovl_25_func_800B8340();
        *(s16 *)(ref + 0x4C8) = PlaySoundEffect(0x38);
        CloseMessageWindow();
        base = (char *)&D_8007AFF0;
        p = base + 0x20000;
        D_800BFE44 = 2;
        *(u16 *)(ref + 0x4CC) = 0;
        *(u16 *)(ref + 0xC) = 0;
        *(u16 *)(p + 0x53B6) = 0;
        *(u16 *)(p + 0x53B4) = *(u16 *)(ref + 0x20);
        *(u16 *)(p + 0x53B8) = *(u16 *)(ref + 0x24);
        ovl_25_func_800B9B00((s16 *)(ref + 0x2E0), 0xA, 0);
        gOvl25SequenceCyclePhase = 1;
        gOvl25SequenceAnchor4[0] = 0x85A;
        gOvl25SequenceAnchor4[1] = 0;
        gOvl25SequenceAnchor4[2] = -0x30A;
    }
}
