#include "common.h"
#include "game_types.h"

void ovl_11_func_80112904(s16 arg0);
void func_8001AF70(u16 arg0, u16 arg1);

void ovl_11_func_800EF488(s16 arg0) {
    Ovl11EF488Record *record;
    u16 *source;
    u16 first;
    u16 second;

    first = 0;
    second = 0;
    source = D_80070CF0_halfwords;
    switch (arg0) {
    case -1:
        ovl_11_func_80112904(((Ovl11EF488SelectionView *) D_8006C838)->selected);
        switch (((Ovl11EF488SelectionView *) D_8006C838)->selected) {
        case 3:
            first = 0x1C;
            break;
        case 7:
            first = 0x1D;
            break;
        case 9:
            first = 0x1E;
            break;
        case 14:
            first = 0x1F;
            break;
        case 20:
        default:
            first = 0x20;
            break;
        }
        break;
    case 0:
        first = 0x21;
        second = 0x26;
        break;
    case 1:
        first = 0x22;
        second = 0x27;
        break;
    case 2:
        first = 0x23;
        second = 0x28;
        break;
    case 3:
        first = 0x24;
        second = 0x29;
        break;
    case 4:
        first = 0x25;
        second = 0x2A;
        break;
    }
    if (arg0 == -1) {
        record = &D_80071CC0_record;
    } else {
        record = &D_8007AD10_records[arg0];
    }
    record->field_0 = source[0];
    record->field_2 = source[1];
    record->field_4 = source[2];
    record->field_6 = source[3];
    func_8001AF70(first, 1);
    if (arg0 == -1) {
        func_8001AF70(0x1B, 0);
        func_8001AF70(0x41, 1);
        return;
    }
    func_8001AF70(second, 1);
    ((Ovl11EF488FlagView *) ((u8 *) D_8006C838 +
        ((Ovl11EF488RecordsView *) D_8006C838)->records[arg0].index_8 * 0x1D4))->flags |= 8;
    ((Ovl11EF488FlagView *) ((u8 *) D_8006C838 +
        ((Ovl11EF488RecordsView *) D_8006C838)->records[arg0].index_A * 0x1D4))->flags |= 8;
}
