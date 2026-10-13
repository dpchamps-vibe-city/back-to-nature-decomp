#include "common.h"
#include "game_types.h"
#include "psyq/memory.h"

u16 *ovl_11_func_800CE744(s32 arg0, s32 arg1);
void ovl_11_func_800CFD9C(void);
void ovl_11_func_8010C3C4(void);

void ovl_11_func_800CE96C(void) {
    s32 i;
    u32 j;
    u16 *object;
    struct Ovl11PoolResetView *p;
    struct Ovl11PoolResetView *first;
    struct Ovl11PoolResetView *p2;
    struct Ovl11PoolResetView *p3;
    struct Ovl11PoolResetView *p4;
    struct Ovl11PoolResetView *p5;
    memset(&D_800742EC, 0, 0x708);
    p = (struct Ovl11PoolResetView *) ((u8 *) &D_800742EC - 0x7AB4);
    for (i = 0; i < 10; i++) {
        first = (struct Ovl11PoolResetView *) ((u8 *) p + i * 0xB4);
        first->records_7AB4[0].type = 0;
        first->records_7AB4[0].id = 0xFFFF;
    }
    memset(&D_800749F4, 0, 0xE60);
    for (i = 0; i < 20; i++) {
        p2 = (struct Ovl11PoolResetView *) ((u8 *) D_8006C838 + i * 0xB8);
        p2->records_81BC[0].type = 0;
        p2->records_81BC[0].id = 0xFFFF;
    }
    for (i = 0; i < 20; i++) {
        p3 = (struct Ovl11PoolResetView *) ((u8 *) D_8006C838 + i * 0xB8);
        p3->records_81BC[0].type = 0;
        p3->records_81BC[0].id = 0xFFFF;
    }
    memset(D_80075854_reset, 0, 0x18E);
    for (i = 98; i >= 0; i--) {
        ((struct Ovl11PoolResetView *) D_8006C838)->cells_901C[i][0] = 0;
    }
    memset(D_80075BC4_reset, 0, 0x420);
    for (i = 0; i < 6; i++) {
        p4 = (struct Ovl11PoolResetView *) ((u8 *) D_8006C838 + i * 0xB0);
        p4->records_938C[0].type = 0;
        p4->records_938C[0].id = 0xFFFF;
    }
    for (i = 0; i < 3; i++) {
        p5 = (struct Ovl11PoolResetView *) ((u8 *) D_8006C838 + i * 0xB4);
        p5->records_97AC[0].type = 0;
        p5->records_97AC[0].id = 0xFFFF;
    }
    ((struct Ovl11PoolResetView *) D_8006C838)->field_91AC = 0;
    ((struct Ovl11PoolResetView *) D_8006C838)->field_91E0 = 0;
    object = ovl_11_func_800CE744(0x107, 0);
    *(s32 *) ((u8 *) object + 0x38) = -0x708;
    *(s32 *) ((u8 *) object + 0x3C) = 0;
    *(s32 *) ((u8 *) object + 0x40) = 0xC1C;
    *(s16 *) ((u8 *) object + 0x2E) = 0;
    *(s16 *) ((u8 *) object + 0x30) = 1;
    memset(&D_80075AD4, 0, 0xF0);
    ((struct Ovl11PoolResetView *) D_8006C838)->field_929C = 0;
    ((struct Ovl11PoolResetView *) D_8006C838)->field_92A0 = 0xFFFF;
    ovl_11_func_800CFD9C();
    for (j = 0; j < 3U; j++) {
        (&D_80128D60)[j] = 0x12C;
    }
    D_80128D68 = -1;
    for (i = 0; i < 4; i++) {
        ((struct Ovl11PoolResetView *) D_8006C838)->pairs_99C8[i][0] = -1;
        ((struct Ovl11PoolResetView *) D_8006C838)->pairs_99C8[i][1] = -1;
    }
    ovl_11_func_8010C3C4();
    ((struct Ovl11PoolResetView *) D_8006C838)->field_99D8 = 0;
    ((struct Ovl11PoolResetView *) D_8006C838)->field_99E6 = 0;
    ((struct Ovl11PoolResetView *) D_8006C838)->flags &= ~8;
}
