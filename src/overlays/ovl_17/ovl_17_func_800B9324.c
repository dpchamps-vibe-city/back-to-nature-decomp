#include "common.h"
#include "ovl17_record_threshold_state.h"
#include "ovl17_record_decision_tests.h"

void ovl_17_func_800B9324(void) {
    s32 i;
    s32 ready;
    s32 group;
    s32 recordOffset;
    s32 entryOffset;
    s32 column;
    u8 *table;
    u8 *work;
    u8 *settings;
    u8 *resultBase;
    u8 *cursor;

    i = 0;
    work = D_80074838;
    /* D_800BD870 is the work area's record alias at +0x28. */
    table = (u8 *)((u32)gOvl17AnimatedRecordBase - 0x28);
    resultBase = table + 0x30;
    recordOffset = 0;
    cursor = resultBase;
    do {
        if (i != 2 || *(s16 *)(work + 0x6514) != 3) {
            settings = gOvl17RecordWorkBase;
            if (*(s32 *)cursor == 1) {
                *(s32 *)cursor = 0;
            } else {
                group = Ovl17EvaluateRecordThresholdState(i);
                ready = Ovl17IsRecordDecisionReady(i);
                if (Ovl17IsRecordAboveDecisionThreshold(i, *(s16 *)(settings + 0x16)) != 0) {
                    column = 0;
                } else if (Ovl17IsRecordAboveDecisionThreshold(i, *(s16 *)(settings + 0x18)) != 0) {
                    column = 2;
                } else {
                    column = 4;
                }
                if (ready == 0) {
                    column++;
                }
                entryOffset = column * 2;
                entryOffset += group * 0xC;
                *(s32 *)(recordOffset + (u32)resultBase) =
                    Ovl17RollAboveDecisionThreshold(*(s16 *)(table + entryOffset + 0x208));
            }
        }
        recordOffset += 0x50;
        i++;
        cursor += 0x50;
    } while (i < 6);
}
