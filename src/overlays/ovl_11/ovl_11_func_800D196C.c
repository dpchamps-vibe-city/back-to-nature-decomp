#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"

s32 ovl_11_func_800D1CFC(Recon_ovl_11_func_800D2594_A0View *arg0);
s32 ovl_11_func_800D1C18(s32 arg0, s32 arg1, s32 arg2);
void ovl_11_func_800D0408(s16 arg0, Recon800D0408A1View *arg1, s32 arg2);
s32 ovl_11_func_800D2DB8(Ovl11Func800BDCE8Arg1 *arg0);
s32 ovl_11_func_800BE208(s32 *arg0, Ovl11CollisionTag arg1, u16 arg2, u16 arg3, void **arg4, s32 *arg5, s32 arg6, s32 arg7);
s32 ovl_11_func_800D24FC(Recon_ovl_11_func_800D2594_A0View *arg0, VECTOR position, s16 arg5, s32 arg6);

s32 ovl_11_func_800D196C(Recon_ovl_11_func_800D2594_A0View *arg0, u16 arg1, s32 arg2) {
    VECTOR position;
    void *hit;
    s32 hitType;
    s32 result;
    s32 extra;
    s16 direction;
    u16 width;
    u16 height;

    result = 4;
    extra = 0;
    if (arg0->unk34 & 0x2000) {
        extra = ovl_11_func_800D1CFC(arg0);
    }
    direction = *(s16 *)((u8 *)arg0 + 0x22);
    if (arg1 == 7) {
        result = ovl_11_func_800D1C18((u16)direction, 0, extra);
    } else if (arg1 == 5) {
        result = ovl_11_func_800D1C18((u16)direction, 1, extra);
    } else if (arg1 == 3) {
        result = ovl_11_func_800D1C18((u16)direction, 2, extra);
    } else if (arg1 == 5) {
        result = ovl_11_func_800D1C18((u16)direction, 1, extra);
    } else if (arg1 == 1) {
        result = *(u16 *)((u8 *)arg0 + 0x22);
    } else if (arg1 == 6) {
        result = ovl_11_func_800D1C18((u16)direction, 3, extra);
    } else if (arg1 == 4) {
        s32 turn;
        direction--;
        turn = 3;
        if (direction >= 0) {
            turn = (u16)direction;
        }
        result = turn;
    } else if (arg1 == 2) {
        s32 turn;
        direction++;
        turn = 0;
        if (direction < 4) {
            turn = (u16)direction;
        }
        result = turn;
    } else {
        direction = direction < 2 ? direction + 2 : direction - 2;
        ovl_11_func_800D0408(direction, (Recon800D0408A1View *)&position, 400);
        position.vx += *(s32 *)((u8 *)arg0 + 0x38);
        position.vz += *(s32 *)((u8 *)arg0 + 0x40);
        width = *(u16 *)((u8 *)&D_8006C858[*(u16 *)arg0] + 0x24);
        height = *(u16 *)((u8 *)&D_8006C858[*(u16 *)arg0] + 0x26);
        if (arg0->unk34 & 0x800000) {
            arg2 &= ~0x20;
        }
        if (arg0->unk34 & 0x200000) {
            if (ovl_11_func_800D2DB8((Ovl11Func800BDCE8Arg1 *)&position)) {
                result = 4;
            } else {
                s32 blocked;
                blocked = ovl_11_func_800BE208((s32 *)&position, ((Ovl11CEFCCObject *)arg0)->collision, width, height, &hit, &hitType, (s32)arg0, arg2);
                result = (u16)direction;
                if (blocked) {
                    result = 4;
                }
            }
        } else {
            s32 original;
            s32 blocked;
            blocked = ovl_11_func_800BE208((s32 *)&position, ((Ovl11CEFCCObject *)arg0)->collision, width, height, &hit, &hitType, (s32)arg0, arg2);
            result = (u16)direction;
            original = result;
            if (blocked) {
                result = 4;
            }
            if (arg0->unk34 & 0x800000) {
                if (ovl_11_func_800D24FC(arg0, position, direction, 0) != 1) {
                    result = original;
                }
            }
        }
    }
    return result;
}
