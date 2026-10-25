#include "common.h"
#include "psyq/memory.h"
#include "ovl11_selection_message.h"

void func_80021E60(s32 arg0);
void ovl_11_func_800BD5A0(void);
void ovl_11_func_800D6628(void);
void ovl_11_func_800BFEA4(void);
void ovl_11_func_800BFF00(void);
void ovl_11_func_800C044C(s16 arg0);
/* These two sprite-loader wrappers use their matched definition signatures;
 * the forwarded arg2/arg3 of func_80015704 are unused debug-hook parameters. */
void ovl_11_func_800BD168(void);
void ovl_11_func_800C9BB8(void);
void ovl_11_func_800BD538(void);
s32 ovl_11_func_800CDE3C(u8 *arg0);
void ovl_11_func_800BD488(s32 arg0);
void ovl_11_func_800DC990(void);
void ovl_11_func_800DCECC(void);
void ovl_11_func_800F6630(void);
void ovl_11_func_800F42A8(u16 arg0, s32 arg1);
void ovl_11_func_800C09D0(void);
void ovl_11_func_800CBEF8(void);
void ovl_11_func_80107F18(void);
void func_800226F0(void);
void ovl_11_func_80111EF0(void);
void ovl_11_func_80121318(void);
void ovl_11_func_800FFA28(void);
void ovl_11_func_800CFD9C(void);
s32 ovl_11_func_800DE76C(void);
s32 ovl_11_func_80109310(s32 *arg0);
void ovl_11_func_800BD668(void);
void ovl_11_func_800F1F48(s32 arg0);
void func_8001AF70(u16 arg0, u16 arg1);
void ovl_11_func_800FF96C(void);
void ovl_11_func_800BD3C4(s32 arg0);

void ovl_11_func_80104A90(void) {
    Ovl11AreaInitView *base;
    Ovl11FarInitView *far;
    s32 value;

    far = (Ovl11FarInitView *)&D_8007AFF0;
    far->field_25484 = 0xFFFF;
    far->field_25488 = 0xFFFF;
    far->field_247A8 = -1;
    far->field_247A6 = -1;
    far->field_2479C = -1;
    far->buffer = &D_800A0368;
    far->field_25498 = -1;
    far->field_25390 = 0;
    far->field_2549C = -1;
    far->data = D_8009F78C;
    base = (Ovl11AreaInitView *)D_8006C838;
    base->flags = (s32) (base->flags & 0xFF7FFFFF);
    value = base->field_44CE * 4;
    value += 0x50;
    far->field_2547C = base->field_44BA + value;
    memset(&D_800A0368, 0, 0x10);
    base->field_5214 = 0;
    func_80021E60(1);
    ovl_11_func_800BD5A0();
    ovl_11_func_800D6628();
    ovl_11_func_800BFEA4();
    ovl_11_func_800BFF00();
    ovl_11_func_800C044C(base->field_E4C8);
    ovl_11_func_800BD168();
    ovl_11_func_800BD3C4(0);
    ovl_11_func_800C9BB8();
    ovl_11_func_800BD538();
    ovl_11_func_800CDE3C(base->field_51C8);
    ovl_11_func_800BD488((s32) base->field_44BA);
    ovl_11_func_800DC990();
    ovl_11_func_800DCECC();
    ovl_11_func_800F6630();
    ovl_11_func_800F42A8((u16) base->field_44BA, 1);
    ovl_11_func_800C09D0();
    ovl_11_func_800CBEF8();
    ovl_11_func_80107F18();
    ovl_11_func_800F6630();
    Ovl11ResetSelectionMessage();
    func_800226F0();
    ovl_11_func_80111EF0();
    ovl_11_func_80121318();
    ovl_11_func_800FFA28();
    ovl_11_func_800CFD9C();
    ovl_11_func_800DE76C();
    ovl_11_func_80109310((s32 *) &D_800759E4);
    ovl_11_func_800BD668();
    ovl_11_func_800F1F48(gGraphicsHeapBase + 0x4290);
    base->field_4488 = 1;
    D_801287F4 = 0xA;
    far->field_2548C = 0;
    base->field_4438 = -1;
    far->field_254A0 = 0xC;
    base->field_4450 = (s32) (base->field_4450 | 0x40);
    if (base->field_44C0 >= 7) {
        func_8001AF70(0xADU, 1U);
    }
    ovl_11_func_800FF96C();
}
