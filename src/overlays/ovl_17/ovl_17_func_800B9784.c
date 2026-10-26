#include "common.h"

void ovl_17_func_800B9784(void) {
    s32 selected;
    s32 scaled;
    s32 position;
    s32 positionPixels;
    s32 origin;
    u16 value;
    u8 *base;
    u8 *rows;

    if (((Ovl17ModeView *)D_8006C838)->mode == 3) {
        selected = 2;
    } else {
        selected = ((Ovl17RecordWorkInitView *)gOvl17RecordWorkBase)->selectionIndices[0];
    }
    base = gOvl17RecordWorkBase;
    scaled = selected * 0x50;
    rows = base + 0x28;
    value = *(u16 *)(base + scaled + 0x2C);
    ((Ovl17RecordWorkInitView *)base)->targetScrollSpeed = value;
    selected = *(s32 *)(rows + scaled + 0x10);
    position = selected;
    positionPixels = position / 0x1000;
    origin = ((Ovl17RecordWorkInitView *)base)->scrollPosition;
    if (positionPixels > origin / 0x1000 + 0x50) {
        ((Ovl17RecordWorkInitView *)base)->scrollSpeed += ((Ovl17RecordWorkInitView *)base)->scrollSpeed / 8;
        return;
    }
    if (position / 0x1000 < origin / 0x1000 - 0x3C) {
        ((Ovl17RecordWorkInitView *)base)->scrollSpeed -= ((Ovl17RecordWorkInitView *)base)->scrollSpeed / 8;
        return;
    }
    ((Ovl17RecordWorkInitView *)base)->scrollSpeed = value;
}
