/* Per-frame sequencing of record updates, pending stage mode, a clock-gated
 * code15B request, and deferred CD audio. Dependencies are public services,
 * not private helpers inferred merely from this call sequence. */

#include "common.h"
#include "cd_audio.h"
#include "ovl11_pending_stage_dispatch.h"
#include "ovl11_item_record_allocation.h"
#include "ovl11_area_frame_update_dispatch.h"
#include "ovl11_area_record_updates.h"

void Ovl11RunAreaFrameUpdates(void) {
    if (!(((struct Ovl11UpdateState *) D_8006C838)->field_10 & 0x2000)) {
        Ovl11UpdateStateObjectPool();
        Ovl11UpdateSmallRecordTriplet();
        Ovl11UpdateTaggedRecordPools();
        Ovl11UpdatePairedStageObjects();
        Ovl11ProcessPendingStageMode();
        if (((struct Ovl11UpdateWork *) &D_8007AFF0)->field_25476 == 1 &&
            ((struct Ovl11UpdateState *) D_8006C838)->field_44C0 >= 0x12 &&
            ((struct Ovl11UpdateState *) D_8006C838)->field_44C0 < 0x17 &&
            (((struct Ovl11UpdateState *) D_8006C838)->field_44F8 & 0x01800000) == 0x800000) {
            if (((struct Ovl11UpdateState *) D_8006C838)->field_44C0 == 0x12) {
                if (((struct Ovl11UpdateState *) D_8006C838)->field_44C2 >= 0xA) {
                    Ovl11AllocateItemRecord(0x15B, -1);
                }
            } else {
                Ovl11AllocateItemRecord(0x15B, -1);
            }
        }
        if (((struct Ovl11UpdateState *) D_8006C838)->pendingCdAudioTrack != 0 &&
            GetCdAudioState() == 0 &&
            ((struct Ovl11UpdateState *) D_8006C838)->field_CC == 0) {
            StartCdAudioTrack(((struct Ovl11UpdateState *) D_8006C838)->pendingCdAudioTrack);
            ((struct Ovl11UpdateState *) D_8006C838)->pendingCdAudioTrack = 0;
        }
    }
}
