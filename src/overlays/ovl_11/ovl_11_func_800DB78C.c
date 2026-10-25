#include "common.h"
#include "gte_scene_state.h"

void ovl_11_func_800DB78C(void) {
    char *far_base = (char *)&D_8007AFF0;

    SetCameraEyeAndRoll(*(s16 *)(far_base + 0x253AC) + *(s16 *)(far_base + 0x253B4),
                  *(s16 *)(far_base + 0x253AE),
                  *(s16 *)(far_base + 0x253B0) + *(s16 *)(far_base + 0x253B8),
                  0);
    SetCameraTarget(*(s16 *)(far_base + 0x253B4),
                  *(s16 *)(far_base + 0x253B6),
                  *(s16 *)(far_base + 0x253B8));
}
