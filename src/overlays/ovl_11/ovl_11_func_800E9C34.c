#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libapi.h"
#include "psyq/memory.h"
#include "ovl11_object_archive_pipeline.h"

s32 ovl_11_func_800E9C34(s16 arg0) {
    Ovl11E910Entry *var_s0;
    s32 var_a0;
    Ovl11E910Entry *temp_v0;

    var_s0 = D_8006E910;
    temp_v0 = (Ovl11E910Entry *)Ovl11GetObjectProgramCursor();
    if (temp_v0->unk2 == 0xFF) {
        var_a0 = 0;
        if (temp_v0 != var_s0) {
            while (++var_a0 < 0xCE) {
                var_s0++;
                if (temp_v0 == var_s0) {
                    break;
                }
            }
        }
        if (var_a0 == 0xCE) {
            return 0;
        }
        gOvl11ValueSlots[arg0] = var_a0 + 1;
    } else {
        gOvl11ValueSlots[arg0] = temp_v0->unk2;
    }
    return 1;
}
