/*
 * pending_stage_dispatch — reconstructed translation unit.
 *
 * Consume a pending mode when the readiness predicate permits, then dispatch
 * stage-matched object handlers and clear per-mode counters when those
 * handlers return zero. Counter initialization and advancement live elsewhere;
 * this unit only consumes the pending request and performs the handler calls.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "ovl11_pending_stage_dispatch.h"
#include "ovl11_tagged_record_handlers.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "psyq/abs.h"
#include "psyq/libgs.h"
#include "psyq/libsnd.h"
#include "psyq/libapi.h"
#include "ovl11_code160_record_handlers.h"

s32 ovl_11_func_800CBDFC(void);
void Ovl11DispatchStageMode(s32 mode);

void Ovl11ProcessPendingStageMode(void) {
    if ((gOvl11PendingStageMode != -1) && (ovl_11_func_800CBDFC() != 0)) {
        Ovl11DispatchStageMode((s32) gOvl11PendingStageMode);
        gOvl11PendingStageMode = -1;
    }
}

s32 ovl_11_func_80109188(Ovl11PrimaryStageHandlerObj *arg0, s32 arg1);
s32 ovl_11_func_8010B57C(u16 *arg0, s32 arg1);

#define CURRENT_STAGE (((struct Ovl11D2764StateView *)&D_8007AFF0)->field_25476)
#define MAIN_STAGE (((struct Ovl11D2764StageView *)D_8006C838)->field_91DC)
#define OTHER_STAGE (((struct Ovl11D2764StageView *)D_8006C838)->field_92CC)

/* Internal helper by use, not static linkage: preserve the original symbol.
 * Mode 0 scans all 20 slots even after a handler clears the shared counter. */
void Ovl11DispatchStageMode(s32 mode) {
    Ovl11StageHandlerObj *object;
    s32 slot;

    switch (mode) {
    case 0:
        if (CURRENT_STAGE == MAIN_STAGE) {
            ovl_11_func_80109188((Ovl11PrimaryStageHandlerObj *)&gStageMode1Record, 5);
        }
        for (slot = 0; slot < 20; slot++) {
            object = (Ovl11StageHandlerObj *)((u8 *)&gTaggedRecordBase + slot * 0xB8);
            if (*(u16 *)object != 0) {
                if (object->unk34 & 0x10000) {
                    if (CURRENT_STAGE == *(s16 *)((u8 *)object + 0x30)) {
                        if (Ovl11DispatchCode160Handler(object, 0xB) == 0) {
                            gOvl11Mode0DispatchCounter = 0;
                        }
                    }
                } else if (CURRENT_STAGE == *(s16 *)((u8 *)object + 0x30)) {
                    if (Ovl11DispatchTaggedRecordHandler(object, 0xB) == 0) {
                        gOvl11Mode0DispatchCounter = 0;
                    }
                }
            }
        }
        return;
    case 1:
        if ((CURRENT_STAGE == MAIN_STAGE) &&
            (ovl_11_func_80109188((Ovl11PrimaryStageHandlerObj *)&gStageMode1Record, 4) == 0)) {
            gOvl11Mode1DispatchCounter = 0;
            return;
        }
        return;
    case 2:
        if ((CURRENT_STAGE == OTHER_STAGE) &&
            (ovl_11_func_8010B57C(&gStageMode2Record, 0x13) == 0)) {
            gOvl11Mode2DispatchCounter = 0;
        }
        break;
    }
}
