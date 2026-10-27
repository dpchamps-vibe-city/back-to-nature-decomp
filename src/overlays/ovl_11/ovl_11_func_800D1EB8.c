#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "ovl11_timed_sprite_effect.h"
#include "ovl11_tagged_record_presentation.h"
#include "ovl11_tagged_record_handlers.h"
#include "ovl11_small_record_handlers.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "psyq/abs.h"
#include "psyq/libgs.h"
#include "psyq/libsnd.h"
#include "psyq/libapi.h"
#include "ovl11_code160_record_handlers.h"

void ovl_11_func_800D1EB8(void) {
    Recon_ovl_11_func_8010CE80_A0View *entry;
    s32 *entryB4;
    s32 flags;
    s32 update_mask;
    s32 i;
    u8 *base;
    u8 *far_base;
    u8 *base2;
    u8 *far_base2;

    for (i = 0; i < 20; i++) {
        entry = (Recon_ovl_11_func_8010CE80_A0View *)((u8 *)&gTaggedRecordBase + i * 0xB8);
        base = (u8 *)D_8006C838;
        far_base = D_8009AFF0;
        if (*(u16 *)entry != 0 && (flags = entry->unk34, !(flags & 0x400))) {
            if (flags & 0x10000) {
                if (!(*(s32 *)(base + 0xC) & 0x08000000)) {
                    if (!(*(s32 *)(base + 0xC) & 0x40000000)) {
                        if (*(s16 *)(far_base + 0x5476) == *(s16 *)((u8 *)entry + 0x30)) {
                            Ovl11StepTaggedRecordHandler(entry);
                            Ovl11ApplyTaggedRecordDisplacement((s32)entry);
                        }
                    } else {
                        Ovl11StepTaggedRecordHandler(entry);
                        Ovl11ApplyTaggedRecordDisplacement((s32)entry);
                    }
                    Ovl11TickTimedSpriteEffect((Ov11Timer *)((u8 *)entry + 0xA8));
                }
                if (*(s16 *)(far_base + 0x5476) == *(s16 *)((u8 *)entry + 0x30)) {
                    Ovl11SelectAndDrawTaggedRecordSprite((u8 *)entry);
                } else {
                    Ovl11ResetTimedSpriteEffect((s16 *)((u8 *)entry + 0xA8));
                }
            } else {
                if (!(*(s32 *)(base + 0xC) & 0x08000000)) {
                    if (!(*(s32 *)(base + 0xC) & 0x40000000)) {
                        if (*(s16 *)(far_base + 0x5476) == *(s16 *)((u8 *)entry + 0x30)) {
                            Ovl11TickCode160Record((UnkStruct800DF4F0 *)entry);
                            Ovl11AdvanceTaggedRecordPosition((s32)entry);
                        }
                    } else {
                        Ovl11TickCode160Record((UnkStruct800DF4F0 *)entry);
                        Ovl11AdvanceTaggedRecordPosition((s32)entry);
                    }
                    Ovl11TickTimedSpriteEffect((Ov11Timer *)((u8 *)entry + 0xA8));
                }
                if (!(entry->unk34 & 0x20000)) {
                    if (*(s16 *)(far_base + 0x5476) != *(s16 *)((u8 *)entry + 0x30)) {
                        Ovl11ResetTimedSpriteEffect((s16 *)((u8 *)entry + 0xA8));
                    } else {
                        Ovl11DrawTaggedRecordSprite((u8 *)entry);
                    }
                }
            }
        }
    }
    for (i = 0; i < 10; i++) {
        entryB4 = (s32 *)((u8 *)&gSmallTaggedRecordBase + i * 0xB4);
        far_base2 = D_8009AFF0;
        base2 = (u8 *)(&gSmallTaggedRecordBase - 0x1EAD);
        if (*(u16 *)entryB4 != 0) {
            Ovl11StepSmallRecordHandler((s16 *)entryB4);
            update_mask = 0x20000;
            if (*(s16 *)(far_base2 + 0x5476) != *(s16 *)((u8 *)entryB4 + 0x30)) {
                Ovl11ResetTimedSpriteEffect((s16 *)((u8 *)&gSmallTaggedRecordBase + i * 0xB4 + 0xA8));
            } else {
                if (!(update_mask & *(s32 *)((u8 *)entryB4 + 0x34))) {
                    if (!(*(s32 *)(base2 + 0xC) & 0x08000000)) {
                        Ovl11ApplySmallRecordDisplacement((s32)entryB4);
                    }
                    if (!(*(s32 *)((u8 *)entryB4 + 0x34) & 0x02000000)) {
                        Ovl11SelectAndDrawSmallRecordSprite((u8 *)entryB4);
                        Ovl11TickTimedSpriteEffect((Ov11Timer *)((u8 *)&gSmallTaggedRecordBase + i * 0xB4 + 0xA8));
                    }
                }
            }
        }
    }
}
