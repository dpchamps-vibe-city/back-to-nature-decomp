/*
 * Overlay-19 two-column value labels and sprite markers.
 *
 * The sprite helper is genuinely nested: the original parent supplies GCC's
 * static chain before every call. Keep that definition inside the parent,
 * rather than emulating its incoming register in a separate file. Membership
 * and boundaries are documented in notes/file-groupings.md.
 */
#include "common.h"
#include "game_types.h"
#include "ovl19_value_table_display.h"

void func_80015840(ObjectState *sprite, u8 animation);
void func_8001585C(ObjectState *sprite, u8 frame);
void func_80015EE8(s32 orderingTable, s32 sprite, s32 animation, s32 frame, s16 x, s16 y);
u32 func_80017A64(void);
void func_80017A48(u32 setting);
s32 ovl_19_func_800BADAC(s16 value);
void func_80017B3C(s32 orderingTable, s32 text, s32 x, s32 y);

void Ovl19DrawValueTable(void) {
    void Ovl19DrawValueTableSpriteState(s32 animation, s32 frame) {
        u8 *display;

        func_80015840((ObjectState *)&gOvl19UiSprite, animation & 0xFF);
        func_8001585C((ObjectState *)&gOvl19UiSprite, frame & 0xFF);
        display = gOvl19UiSprite - 0x1A0;
        func_80015EE8(gActiveRenderContext->field_D8 + 0x14, (s32)&gOvl19UiSprite,
                      display[0x1A4], display[0x1A5], 0, 0);
    }
    u32 savedTextSetting;
    u8 *sprite;
    u8 *display = gOvl19UiSprite;
    s32 displayAddress;
    u8 *values;
    s32 *firstLabel;
    u8 *labelText;
    s32 firstRowY;
    s32 rowYFixed;
    s32 nextColumn;
    s32 moreColumns;
    s32 textOffset;
    s32 textAddress;
    /* Existing authorized binding: keeps the initial zero below the sprite
     * draw and preserves loop/delay-slot allocation. Not original-source
     * evidence; see notes/asm-exception-evidence.md (ovl_19:800BB108). */
    register s32 row asm("$16");
    u8 *counts;
    s32 column;

    func_80015840((ObjectState *)display, 0);
    sprite = display;
    display -= 0x1A0;
    displayAddress = (s32)display;
    func_80015EE8(gActiveRenderContext->field_D8 + 0x18, (s32)sprite, display[0x1A4], display[0x1A5], 0, 0);
    column = 0;
    row = 0;
    savedTextSetting = func_80017A64();
    func_80017A48(1);
    firstLabel = &D_8007AE58;
    labelText = (u8 *)gOvl19ValueLabelText;
    do {
        nextColumn = column + 1;
        rowYFixed = 60 << 16;
        firstRowY = 57;
        values = (u8 *)(displayAddress + 0x160);
        do {
            if (column == 0 && row == 0) {
                textAddress = *firstLabel;
            } else {
                textOffset = ovl_19_func_800BADAC(((s16 *)values)[column * 4 + row]);
                textAddress = textOffset + (s32)labelText;
            }
            func_80017B3C(gActiveRenderContext->field_D8 + 0xC, textAddress, (s16)(column * 200 + 21),
                          row > 0 ? rowYFixed >> 16 : (s16)firstRowY);
            firstRowY += 45;
            rowYFixed += 45 << 16;
            row++;
        } while (row < 4);
        moreColumns = nextColumn < 2;
        column = nextColumn;
        row = 0;
    } while (moreColumns);
    func_80017A48(savedTextSetting);
    counts = (u8 *)D_800BF4C0;
    if (((s16 *)counts)[4] > 0) {
        Ovl19DrawValueTableSpriteState(1, 0);
        Ovl19DrawValueTableSpriteState(4, 0);
        Ovl19DrawValueTableSpriteState(5, 0);
        Ovl19DrawValueTableSpriteState(7, 0);
    }
    if (((s16 *)counts)[4] >= 2) {
        Ovl19DrawValueTableSpriteState(1, 1);
        Ovl19DrawValueTableSpriteState(7, 1);
    }
}
