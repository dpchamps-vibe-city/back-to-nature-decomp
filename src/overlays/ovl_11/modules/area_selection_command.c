/* Scripted area-selection requests. This posts mode 0x18 and a selection,
 * rather than loading the area synchronously. The area-mode sequence and
 * object archive pipeline consume that mode. See the public command header
 * and notes/file-groupings.md for the supported unit boundaries.
 */
#include "common.h"
#include "game_flags.h"
#include "ovl11_default_area_selection.h"
#include "ovl11_area_selection_command.h"

s32 Ovl11RequestAreaSelectionCommand(s16 selectionCode, s16 absoluteSelection,
                                    s32 flag2000, s32 flag4000) {
    s32 selection;

    switch (selectionCode) {
    case -1:
        /* Cancel the season offset so the relative path yields the default. */
        selection = Ovl11ResolveDefaultAreaSelection() - ((struct Ovl11AreaSelectionCommandWork *) D_8006C838)->season;
        break;
    case -2:
        selection = (((struct Ovl11AreaSelectionCommandWork *) D_8006C838)->variant * 4) + 0x50;
        break;
    case -4:
        if (((struct Ovl11AreaSelectionCommandWork *) D_8006C838)->status & 0x100000) {
            selection = 0x78;
        } else if (GetGameFlag(0x3AU) == 1) {
            selection = 0x74;
        } else {
            selection = 0x70;
        }
        break;
    case -3:
        if (((struct Ovl11AreaSelectionCommandWork *) D_8006C838)->status & 0x100) {
            selection = 0x6C;
        } else if (((struct Ovl11AreaSelectionCommandWork *) D_8006C838)->status & 0x200) {
            selection = 0x68;
        } else if (GetGameFlag(0x3CU) == 1) {
            selection = 0x64;
        } else {
            selection = 0x60;
        }
        break;
    default:
        selection = selectionCode;
        break;
    }
    if (absoluteSelection == 0) {
        ((Ovl11TriggerWorkView *) &D_8007AFF0)->field_25480 = (s32) (selection + ((struct Ovl11AreaSelectionCommandWork *) D_8006C838)->season);
    } else {
        ((Ovl11TriggerWorkView *) &D_8007AFF0)->field_25480 = (s32) selection;
    }
    ((Ovl11TriggerWorkView *) &D_8007AFF0)->field_254A0 = 0x18;
    if (flag2000 == 0) {
        ((struct Ovl11AreaSelectionCommandWork *) D_8006C838)->flags = (s32) (((struct Ovl11AreaSelectionCommandWork *) D_8006C838)->flags & ~0x2000);
    } else {
        ((struct Ovl11AreaSelectionCommandWork *) D_8006C838)->flags = (s32) (((struct Ovl11AreaSelectionCommandWork *) D_8006C838)->flags | 0x2000);
    }
    if (flag4000 != 0) {
        ((struct Ovl11AreaSelectionCommandWork *) D_8006C838)->flags = (s32) (((struct Ovl11AreaSelectionCommandWork *) D_8006C838)->flags | 0x4000);
    } else {
        ((struct Ovl11AreaSelectionCommandWork *) D_8006C838)->flags = (s32) (((struct Ovl11AreaSelectionCommandWork *) D_8006C838)->flags & ~0x4000);
    }
    return 1;
}
