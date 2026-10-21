#include "common.h"

void ovl_11_func_800F4CA0(void);

void ovl_11_func_800F4BB4(void) {
    if (((struct struct_8006C838_button *)D_8006C838)->field_44D6 == 0) {
        return;
    }

    switch (((struct struct_8006C838_button *)D_8006C838)->field_44D6) {
    case 0x51:
    case 0x55:
        if (((struct struct_8006C838_button *)D_8006C838)->field_44BA != 0) {
            ovl_11_func_800F4CA0();
        }
        break;
    case 0x52:
        if (((struct struct_8006C838_button *)D_8006C838)->field_44BA != 1) {
            ovl_11_func_800F4CA0();
        }
        break;
    case 0x53:
    case 0x54:
        if (((struct struct_8006C838_button *)D_8006C838)->field_44BA != 2) {
            ovl_11_func_800F4CA0();
        }
        break;
    }

    ((struct struct_8006C838_button *)D_8006C838)->field_51DE -= 5;
    if (((struct struct_8006C838_button *)D_8006C838)->field_51DE < 0) {
        ((struct struct_8006C838_button *)D_8006C838)->field_51DE = 0;
    }
}
