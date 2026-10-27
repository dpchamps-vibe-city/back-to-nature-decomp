/* Scene-mode selector for object commands. State changes are posted to the
 * shared mode word; this does not run the selected mode or wait for it.
 * No helper/private-state evidence joins the neighboring random-value or
 * timed-sprite commands. See notes/file-groupings.md for the service cuts.
 */
#include "common.h"
#include "ovl11_scene_mode_command.h"

s32 Ovl11SetSceneModeCommand(s16 mode) {
    ((Ovl11SceneModeCommandWork *)D_8006C838)->field_448C = 0;
    switch (mode) {
    case 0: {
        u8 *far = (u8 *)&D_8007AFF0;

        *(s32 *)(far + 0x254A0) = 0x12;
        break;
    }
    case 1: {
        u8 *far = (u8 *)&D_8007AFF0;

        *(s32 *)(far + 0x254A0) = 0x16;
        ((Ovl11SceneModeRenderContextView *)gRenderContexts)[0].field_19 = 0;
        ((Ovl11SceneModeRenderContextView *)gRenderContexts)[0].field_1A = 0;
        ((Ovl11SceneModeRenderContextView *)gRenderContexts)[0].field_1B = 0;
        ((Ovl11SceneModeRenderContextView *)gRenderContexts)[1].field_19 = 0;
        ((Ovl11SceneModeRenderContextView *)gRenderContexts)[1].field_1A = 0;
        ((Ovl11SceneModeRenderContextView *)gRenderContexts)[1].field_1B = 0;
        break;
    }
    case 2: {
        u8 *far = (u8 *)&D_8007AFF0;

        *(s32 *)(far + 0x254A0) = 0x13;
        break;
    }
    case 3: {
        u8 *far = (u8 *)&D_8007AFF0;

        *(s32 *)(far + 0x254A0) = 0x14;
        break;
    }
    case 4: {
        u8 *far = (u8 *)&D_8007AFF0;

        *(s32 *)(far + 0x254A0) = 0x15;
        break;
    }
    case 5: {
        u8 *far = (u8 *)&D_8007AFF0;

        *(s32 *)(far + 0x254A0) = 0x1F;
        ((Ovl11SceneModeCommandWork *)D_8006C838)->field_5234 |= 0x400;
        break;
    }
    }
    return 1;
}
