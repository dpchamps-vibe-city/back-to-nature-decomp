#include "common.h"
#include "ovl11_pool_record_transition.h"

void ovl_11_func_800F5210(s16 arg0);
void ovl_11_func_800F508C(s16 arg0);
void ovl_11_func_8010941C(s32 arg0);

typedef struct {
    /* 0x0000 */ char pad_0000[0x44BA];
    /* 0x44BA */ s16 field_44BA;
    /* 0x44BC */ char pad_44BC[0x91DC - 0x44BC];
    /* 0x91DC */ s16 field_91DC;
    /* 0x91DE */ char pad_91DE[0xDDD4 - 0x91DE];
    /* 0xDDD4 */ s16 *field_DDD4;
} D8006C838_3FF4;

void ovl_11_func_800F3FF4(s16 arg0) {
    s16 temp;

    ovl_11_func_800F5210(arg0);
    ovl_11_func_800F508C(arg0);
    if (((D8006C838_3FF4 *)&D_8006C838)->field_44BA == 0) {
        temp = *((D8006C838_3FF4 *)&D_8006C838)->field_DDD4;
        if ((temp == 0xD) || ((u32)(temp - 0x38) < 0xBU)) {
            Ovl11PublishCurrentPoolRecord();
        }
        temp = ((D8006C838_3FF4 *)&D_8006C838)->field_91DC;
        if ((temp == 0xD) || ((u32)(temp - 0x38) < 0xBU)) {
            ovl_11_func_8010941C(0);
        }
    }
}
