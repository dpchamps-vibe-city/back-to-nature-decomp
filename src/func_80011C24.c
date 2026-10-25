#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libetc.h"
#include "screen_fade.h"
#include "wide_screen_fade.h"
#include "update_timing.h"

/* GP-relative globals this TU defines (target accesses all via %gp_rel).
 * Tentative definitions so cc1 keeps them inside the -G8 small-data range. */
void (*D_8005E384)(void);
s32 D_8005E388;
s32 D_8005E38C;
s32 D_8005E390;
s32 gSceneUpdateLoopActive;
s32 gActiveRenderContextIndex;
s32 *gFrameOrderingTable;
struct_8005E3C0 *gActiveRenderContext;

typedef struct {
    char pad_000[0x8];
    s32 field_008;
} struct_8006C838_view_8;

extern void func_80011DB0(void);

/* Callee prototypes (from include/functions.h; declared locally to avoid
 * pulling the generated header). All are void — implicit-int declarations
 * would make cc1 emit call_value_internal1 (a $v0 set) for each void call,
 * stealing $v0 from the loads that follow them. */
void func_8001B258(void);
void func_80013B04(void);
void func_8001FD10(void);
void func_800129F4(void);
void func_800120C8(void);

s32 GetVal8005E2CC(void);

u32 Rand(s32 arg0);

void func_80011C24(void) {
    char *scene_ptr;
    s32 tmp;
    s32 saved;
    s32 *toggle_ptr;

    D_8005E390 = 0;
    D_8005E38C = 0;
    gSceneUpdateLoopActive = 1;
    SetUpdateTimingMode(2);
    do {
    } while (DrawSync(1) != 0);
    if (gSceneUpdateLoopActive == 1) {
        do {
            Rand(0xFFFF);
            func_8001B258();
            func_80013B04();
            func_8001FD10();
            if (GetVal8005E2CC() == 1) {
                D_8005E384();
                DrawSync(0);
                VSync(2);
            } else {
                func_800129F4();
                tmp = (s32)gActiveRenderContext;
                scene_ptr = (char *)&gRenderContexts;
                saved = (s32)scene_ptr;
                if ((s32)scene_ptr == tmp) {
                    scene_ptr += 0x134;
                }
                gActiveRenderContext = (struct_8005E3C0 *)scene_ptr;
                gActiveRenderContextIndex = (s32)((s32)scene_ptr != saved);
                toggle_ptr = (s32 *)&D_8005E5D8;
                if (gFrameOrderingTable == &D_8005E5D8) {
                    toggle_ptr = (s32 *)((char *)&D_8005E5D8 + 8);
                }
                gFrameOrderingTable = toggle_ptr;
                func_800120C8();
                UpdateAndDrawScreenFade();
                UpdateAndDrawWideScreenFade();
                D_8005E384();
                ((struct_8006C838_view_8 *)&D_8006C838)->field_008 += 1;
                D_8005E38C = VSync(1);
                DrawSync(0);
                D_8005E390 = VSync(1);
                D_8005E388 = (s32)func_80011DB0;
                VSync(2);
            }
        } while (gSceneUpdateLoopActive == 1);
    }
}
