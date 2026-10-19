#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libapi.h"

void func_8001AF70(u16 arg0, u16 arg1);
s32 func_8001205C(void);
s32 func_80012098(void);
void func_800121D4(void);
/* The callee's four-byte flag aggregates use the scalar word calling ABI. */
u8 *func_80014CBC(s32, s32, s32, u8 *, s32, s32);
void ovl_11_func_800E5230(void);
void ovl_11_func_800E5294(void);
u8 *ovl_11_func_800E54F8(u16 *arg0, s32 arg1);
void ovl_11_func_800E499C(s32 arg0);
s32 ovl_11_func_800E3978(s32 arg0);
void ovl_11_func_800E5524(u8 *arg0, u8 *arg1, s32 arg2);
void ovl_11_func_800E5078(void);
s32 ovl_11_func_800EEBC8();
void ovl_11_func_800E54C8(void);

s32 ovl_11_func_800E3DC8(void) {
    s16 id;
    s32 size;
    s32 offset;
    u8 *result;
    s32 needsRead;
    s32 useAsync;
    u8 *data;
    u8 *base;

    data = NULL;
    if (((struct Ovl11CurrentObjectView *)D_8006C838)->current[3] != 1) {
        func_8001AF70((u16)(((struct Ovl11CurrentObjectView *)D_8006C838)->current[1] + 0xFB), 1);
    }
    ((struct Ovl11CurrentObjectView *)D_8006C838)->current[2] = 0;
    ((struct Ovl11CurrentObjectView *)D_8006C838)->current[4] = 0;
    ((struct Ovl11CurrentObjectView *)D_8006C838)->current[5] = 0;
    id = ((struct Ovl11CurrentObjectView *)D_8006C838)->current[1];
    offset = D_801240D8[id];
    needsRead = 1;
    size = D_801240D8[id + 1] - offset;
    ovl_11_func_800E5230();
    ovl_11_func_800E5294();
    if ((u32)(id - 0x15F) < 0x32U) {
        data = ovl_11_func_800E54F8((u16 *)((u8 *)D_8006C838 + 0xD8), id);
        needsRead = data == NULL;
    }
    useAsync = 0;
    if (needsRead != 0) {
        if ((size < func_8001205C()) && (((Ovl11FarInitView *)&D_8007AFF0)->field_254A0 != 0xF)) {
            useAsync = id != 0x12D;
        }
        if (useAsync != 0) {
            func_80014CBC(0xC, offset, size, (u8 *)func_80012098(), 0, 1);
            ((struct Ovl11E4280State *)D_8006C838)->selector = 3;
            return 0;
        }
        DrawSync(0);
        if (D_8005E5E8[0].field_11C < size) {
            ClearOTagR((u_long *)D_8005E3C0->field_120, 0x800);
            data = (u8 *)D_8005E5E8[0].field_124;
            if (D_8005E5E8[0].field_11C * 2 < size) {
                func_800121D4();
            }
        } else if ((void *)D_8005E3C0 == (void *)D_8005E5E8) {
            data = (u8 *)D_8005E5E8[1].field_124;
        } else {
            data = (u8 *)D_8005E5E8[0].field_124;
        }
        func_80014CBC(0xC, offset, size, data, 0, 1);
        do {
            result = func_80014CBC(0xC, offset, size, data, 0, 0);
        } while (result == NULL);
        ovl_11_func_800E499C((s32)result);
    } else {
        ovl_11_func_800E5524((u8 *)D_8006C838 + 0x20D8, data, ovl_11_func_800E3978(id));
        if (ovl_11_func_800E3978(id) >= 0xCE1U) {
            SystemError(0x53, ((struct Ovl11CurrentObjectView *)D_8006C838)->current[1]);
        }
        ovl_11_func_800E5078();
    }
    D_801291B0 = Ovl11RecordCursor = D_8006E910;
    D_80129224 = 0;
    ovl_11_func_800EEBC8(0, 0, 0, 0);
    base = (u8 *)D_8006E910 - 0x20D8;
    ovl_11_func_800E54C8();
    ((struct struct_8006C838_800E3AA4 *)base)->field_4438 = -1;
    ((struct Ovl11E4280State *)base)->selector = 1;
    return 0;
}
