#include "common.h"
#include "ovl15_save_checksums.h"
#include "psyq/memory.h"
#include "ovl15_save_image.h"

extern u8 D_80053350[];
extern s8 D_80137584;
extern s16 D_8013758E;

typedef struct {
    char pad_0[0x34];
    s32 field_34;
    s16 field_38;
    s16 field_3A;
} D_801376E0Entry;

extern D_801376E0Entry D_801376E0[];

void func_8001AC10(u32 *arg0, s32 arg1, s32 arg2);

void ovl_15_func_80131CDC(void) {
    s32 temp_a1;

    temp_a1 = gActiveRenderContext->field_D8;
    func_8001AC10((u32 *)(temp_a1 + 0x18), temp_a1 + 0x14, gTextBankOffsets[0] + (s32)D_80053350);
    if (Ovl15VerifySaveChecksums() == -1) {
        D_80137584 = 0xE;
    }
    else if (D_801376E0[D_8013758E].field_34 != 0x14) {
        D_80137584 = 0xE;
    }
    else {
        Ovl15RemoveSaveChecksumSlots();
        D_80137584 = 0x10;
    }
}
