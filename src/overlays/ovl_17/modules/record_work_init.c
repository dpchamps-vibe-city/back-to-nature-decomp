/*
 * Record work-area initialization, ovl_17:800BA54C..800BA808.
 * The terminal scene-load phase seeds controls, roll thresholds and row
 * targets, then loads six display rows from the shared card table. Row reset
 * is also a public operation during the scene. No sprite archive is loaded
 * here; resource loading and rendering are separate services.
 */
#include "ovl17_record_work_init.h"
#include "ovl17_timer_hud.h"

#define RECORD_WORK OVL17_RECORD_WORK

void Ovl17InitRecordRollThresholds(void);
void Ovl17InitRowSlotTargets(void);

void Ovl17InitRecordWork(void) {
    s32 slot;

    RECORD_WORK.toggle = 0;
    RECORD_WORK.phase = 0;
    RECORD_WORK.tickPeriod = 6;
    RECORD_WORK.valueBase = 0x80;
    RECORD_WORK.valueBias = 0;
    RECORD_WORK.valueGain = 4;
    RECORD_WORK.field_E = 0x96;
    RECORD_WORK.field_10 = 1;
    RECORD_WORK.field_12 = 0x28;
    RECORD_WORK.field_14 = 0x12;
    RECORD_WORK.field_16 = 0xAA;
    RECORD_WORK.field_18 = 0x64;
    RECORD_WORK.tickCounter = 0;
    RECORD_WORK.scrollSpeed = 0;
    RECORD_WORK.targetScrollSpeed = 0;
    RECORD_WORK.scrollPosition = 0x40000;
    for (slot = 0; slot < 6; slot++) {
        RECORD_WORK.selectionIndices[slot] = -1;
    }
    RECORD_WORK.soundVoice = -1;
    RECORD_WORK.soundTriggerPercent = 0x32;
    RECORD_WORK.primaryValueThreshold = 0x400;
    RECORD_WORK.selectionCount = 0;
    Ovl17InitRecordRollThresholds();
    Ovl17InitRowSlotTargets();
    Ovl17ResetCardRows();
    Ovl17UpdateTimerHud(-1);
}

/* Expand the packed 3x6 byte thresholds to the work area's signed halves.
 * The decision hub uses these as inputs to its Rand(127) comparison. */
void Ovl17InitRecordRollThresholds(void) {
    s32 group;
    s32 column;
    u8 *packed;
    u8 *base;
    s16 *thresholds;

    group = 0;
    base = gOvl17RecordWorkBase;
    packed = gOvl17RecordRollThresholds;
    thresholds = (s16 *)(base + 0x208);
    for (; group < 3; group++) {
        for (column = 0; column < 6; column++) {
            thresholds[group * 6 + column] = packed[group * 6 + column];
        }
    }
}

/* Six triples with a fixed 0..5 permutation in the middle halfword.
 * Rows retain pointers to these triples; their ultimate reader is unknown. */
void Ovl17InitRowSlotTargets(void) {
    s16 *targets;
    s32 slot;
    s32 x;

    targets = (s16 *)gOvl17RowSlotTargets;
    targets[1] = 5;
    targets[4] = 0;
    targets[7] = 4;
    targets[10] = 3;
    targets[13] = 1;
    targets[16] = 2;
    for (slot = 0; slot < 6; slot++) {
        x = 0x4D + slot * 0x1A;
        targets[slot * 3] = x;
        targets[slot * 3 + 2] = 0x14 - slot;
    }
}

/* Mode 3 uses card row 2 and substitutes the special slot's x/y into display
 * row 2. Other modes directly index the card table without a bounds check. */
void Ovl17ResetCardRows(void) {
    u16 row;
    s32 slot;

    row = 2;
    if (((Ovl17ModeView *)D_8006C838)->mode != 3) {
        row = ((Ovl17ModeView *)D_8006C838)->mode;
    }
    for (slot = 0; slot < 6; slot++) {
        ((Ovl17RowsView *)gOvl17RecordWorkBase)->rows[slot].flags = 0;
        ((Ovl17RowsView *)gOvl17RecordWorkBase)->rows[slot].unk2A = 0xFF;
        ((Ovl17RowsView *)gOvl17RecordWorkBase)->rows[slot].unk2C = 0;
        ((Ovl17RowsView *)gOvl17RecordWorkBase)->rows[slot].unk2E = 0;
        ((Ovl17RowsView *)gOvl17RecordWorkBase)->rows[slot].unk30 = 0;
        ((Ovl17RowsView *)gOvl17RecordWorkBase)->rows[slot].x =
            ((Ovl17ModeView *)D_8006C838)->cards[row][slot].x;
        ((Ovl17RowsView *)gOvl17RecordWorkBase)->rows[slot].y =
            ((Ovl17ModeView *)D_8006C838)->cards[row][slot].y;
        ((Ovl17RowsView *)gOvl17RecordWorkBase)->rows[slot].attr[0] =
            ((Ovl17ModeView *)D_8006C838)->cards[row][slot].attr[0];
        ((Ovl17RowsView *)gOvl17RecordWorkBase)->rows[slot].attr[1] =
            ((Ovl17ModeView *)D_8006C838)->cards[row][slot].attr[1];
        ((Ovl17RowsView *)gOvl17RecordWorkBase)->rows[slot].unk38 = -0x20000;
        ((Ovl17RowsView *)gOvl17RecordWorkBase)->rows[slot].data = &gOvl17RowSlotTargets[slot * 6];
    }
    if (((Ovl17ModeView *)D_8006C838)->mode == 3) {
        ((Ovl17RowsView *)gOvl17RecordWorkBase)->rows[2].flags = 1;
        ((Ovl17RowsView *)gOvl17RecordWorkBase)->rows[2].x =
            ((Ovl17ModeView *)D_8006C838)->unk9348;
        ((Ovl17RowsView *)gOvl17RecordWorkBase)->rows[2].y =
            ((Ovl17ModeView *)D_8006C838)->unk92B2;
    }
}
