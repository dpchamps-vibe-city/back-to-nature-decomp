/* Main/player/pet data editors and diagnostic action dispatch. The original
 * Japanese headings identify these pages; their four descriptor tables are
 * consecutive in the same order as the callbacks. The widget owns cursor
 * updates and the selected-action channel. Public contract:
 * ovl30_value_action_pages.h; membership/boundaries: notes/file-groupings.md. */

#include "ovl30_value_action_pages.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "screen_fade.h"
#include "pad_actuator_sequences.h"
#include "sound_commands.h"
#include "random.h"
#include "game_flags.h"
#include "ovl30_record_menus.h"
#include "ovl11_cell_actions.h"
#include "ovl11_item_table_queries.h"
#include "ovl11_item_record_allocation.h"
#include "ovl11_object_record_state.h"
#include "ovl11_item_slot_commands.h"

s32 Ovl30EditMainData(s32 cursor) {
    s32 result;

    if (IsScreenFadeComplete() != 0) {
        KanjiFntPrint(gOvl30MainDataTextPool);
        gOvl30RecordMenuCursor = cursor;
        Ovl30DrawAndEditValueMenu(&gOvl30MainDataMenu, 0x16);
        result = gOvl30RecordMenuCursor;
    } else {
        result = cursor;
    }
    return result;
}

s32 Ovl30EditPlayerData(s32 cursor) {
    s32 result;

    if (IsScreenFadeComplete() != 0) {
        KanjiFntPrint(gOvl30PlayerAndActionTextPool);
        gOvl30RecordMenuCursor = cursor;
        Ovl30DrawAndEditValueMenu(&gOvl30PlayerDataMenu, 0x17);
        result = gOvl30RecordMenuCursor;
    } else {
        result = cursor;
    }
    return result;
}

s32 ovl_11_func_800E99EC(s16, s16, s32, s32);
void ovl_11_func_800EF488(s16);
void ovl_11_func_800EF6E4(s16 mode, s16 rivalIndex);

#define ACTION ((Ovl30ActionState *)D_8006C838)
#define EVENT(index) ((Ovl30EventRecordView *)((u8 *)D_8006C838 + (index) * 0x1D4))

s32 Ovl30DispatchDebugActions(s32 cursor) {
    s32 id;
    u16 *object;
    Ovl30RecordF8 *entry;
    s32 i;
    s32 x;
    s32 y;
    s16 selected;
    s32 first;
    s32 second;

    if (IsScreenFadeComplete() != 0) {
        if (gOvl30RecordMenuSelectedId < 0) {
            KanjiFntPrint(gOvl30ActionHeading);
            gOvl30RecordMenuCursor = cursor;
            Ovl30DrawAndEditValueMenu(&gOvl30ActionMenu, 29);
            cursor = gOvl30RecordMenuCursor;
            KanjiFntPrint(gOvl30ActionStatusFormat, gOvl30RecordMenuSelectedId < 0 ? gOvl30ActionOffText : gOvl30ActionOnText);
            if (gOvl30RecordMenuSelectedId > 0) {
                PlaySoundEffect(0);
            }
        }
        switch (gOvl30RecordMenuSelectedId) {
        case 0:
            RequestPadActuatorSequence(0, gOvl30DebugActuatorSequence);
            gOvl30RecordMenuSelectedId = -1;
            break;
        case 1:
            StopPadActuatorSequence(0);
            gOvl30RecordMenuSelectedId = -1;
            break;
        /* These cases share the eight-random-halfword initialization tail. */
        case 2: id = 0x162; goto spawn;
        case 3: id = 0x161; goto spawn;
        case 4: id = 0x160; goto spawn;
        case 5: id = 0x165; goto spawn;
        case 6: id = 0x164; goto spawn;
        case 7: id = 0x109; goto spawn;
        case 8: id = 0x10A; goto spawn;
        case 9: id = 0x15E; goto spawn;
        case 10:
            id = 0x15F;
        spawn:
            object = Ovl11AllocateItemRecord(id, -1);
            gOvl30RecordMenuSelectedId = -1;
            for (i = 0; i < 8; i++) {
                object[i + 2] = Rand(0x94);
            }
            object[10] = 0xFFFF;
            break;
        case 11:
            gOvl30RecordMenuSelectedId = -1;
            ACTION->flagsC |= 0x400000;
            break;
        case 12:
            gOvl30RecordMenuSelectedId = -1;
            ACTION->flagsC &= ~0x400000;
            break;
        case 13:
            object = Ovl11AllocateItemRecord(0x162, -1);
            ((Ovl30AnimalRecord *)object)->elapsed[1] = 1;
            ((Ovl30AnimalRecord *)object)->position[0] = -2000;
            ((Ovl30AnimalRecord *)object)->position[2] = 0;
            object = Ovl11AllocateItemRecord(0x161, -1);
            ((Ovl30AnimalRecord *)object)->elapsed[1] = 1;
            ((Ovl30AnimalRecord *)object)->position[0] = -1000;
            ((Ovl30AnimalRecord *)object)->position[2] = 0;
            object = Ovl11AllocateItemRecord(0x160, -1);
            ((Ovl30AnimalRecord *)object)->elapsed[1] = 1;
            ((Ovl30AnimalRecord *)object)->position[0] = 0;
            ((Ovl30AnimalRecord *)object)->position[2] = 0;
            object = Ovl11AllocateItemRecord(0x160, -1);
            ((Ovl30AnimalRecord *)object)->elapsed[1] = 1;
            ((Ovl30AnimalRecord *)object)->position[0] = 1000;
            ((Ovl30AnimalRecord *)object)->position[2] = 0;
            object = Ovl11AllocateItemRecord(0x164, -1);
            ((Ovl30AnimalRecord *)object)->elapsed[1] = 1;
            ((Ovl30AnimalRecord *)object)->position[0] = 2000;
            ((Ovl30AnimalRecord *)object)->position[2] = 0;
            object = Ovl11AllocateItemRecord(0x165, -1);
            ((Ovl30AnimalRecord *)object)->elapsed[1] = 1;
            ((Ovl30AnimalRecord *)object)->position[0] = 3000;
            ((Ovl30AnimalRecord *)object)->position[2] = 0;
            gOvl30RecordMenuSelectedId = -1;
            break;
        case 14:
            for (y = 0; y < 25; y++) {
                for (x = 0; x < 45; x++) {
                    Ovl11TryPromoteCellPendingKind(0, (s16)x, (s16)y, 0);
                }
            }
            gOvl30RecordMenuSelectedId = -1;
            break;
        case 15:
            gOvl30RecordMenuSelectedId = -1;
            ACTION->flags5234 &= ~0x800400;
            break;
        case 16:
            id = (s16)Ovl11ClassifyItemFlags((s16)gOvl30DebugItemId);
            switch (id) {
            case 0:
                Ovl11InsertHeldGridOrOverflowItem((s16)gOvl30DebugItemId, 0, 1, 0);
                break;
            case 1:
                if (Ovl11HasEmptyItemSlot(1, -1, 0, 0) == 1) {
                    Ovl11InsertSecondaryItemCommand((s16)gOvl30DebugItemId, 0, 1, 0);
                } else {
                    Ovl11InsertSecondaryItemCommand((s16)gOvl30DebugItemId, 0, 0, 0);
                }
                break;
            }
            gOvl30RecordMenuSelectedId = -1;
            break;
        case 17:
            entry = Ovl30_RecordsA638;
            for (id = 0; id < 7; id++, entry++) {
                entry->field_B8 |= 1;
                Ovl11SetObjectRecordMode((Recon_ovl_11_func_8010CE80_A0View *)entry, 11);
            }
            gOvl30RecordMenuSelectedId = -1;
            break;
        case 18:
            entry = Ovl30_RecordsA638;
            for (id = 0; id < 7; id++, entry++) {
                entry->field_B8 |= 2;
                Ovl11SetObjectRecordMode((Recon_ovl_11_func_8010CE80_A0View *)entry, 11);
            }
            gOvl30RecordMenuSelectedId = -1;
            break;
        case 19:
            entry = Ovl30_RecordsA638;
            for (id = 0; id < 7; id++, entry++) {
                entry->field_B8 |= 4;
                Ovl11SetObjectRecordMode((Recon_ovl_11_func_8010CE80_A0View *)entry, 11);
            }
            gOvl30RecordMenuSelectedId = -1;
            break;
        case 20:
            ovl_11_func_800EF6E4(1, 0);
            gOvl30RecordMenuSelectedId = -1;
            break;
        case 21:
            ovl_11_func_800EF6E4(1, 1);
            gOvl30RecordMenuSelectedId = -1;
            break;
        case 22:
            ovl_11_func_800EF6E4(1, 2);
            gOvl30RecordMenuSelectedId = -1;
            break;
        case 23:
            ovl_11_func_800EF6E4(1, 3);
            gOvl30RecordMenuSelectedId = -1;
            break;
        case 24:
            ovl_11_func_800EF6E4(1, 4);
            gOvl30RecordMenuSelectedId = -1;
            break;
        /* These diagnostic actions do not validate gameplay preconditions. */
        case 25:
            ovl_11_func_800EF488(-1);
            selected = ACTION->selected - 3;
            switch (selected) {
            case 0: id = 0xA4; break;
            case 4: id = 0xA3; break;
            case 6: id = 0xA5; break;
            case 11: id = 0xA6; break;
            case 17:
            default: id = 0xA2; break;
            }
            EVENT(ACTION->selected)->field_99EC = 0xFFFF;
            SetGameFlag(id + 0xFB, 1);
            ACTION->state44CE = 2;
            ACTION->state44D0 = 1;
            ACTION->state44D2 = 1;
            ACTION->random = Rand(3);
            gOvl30RecordMenuSelectedId = -1;
            break;
        case 26:
            ovl_11_func_800EF6E4(0, (s16)gOvl30DebugRivalIndex);
            switch (gOvl30DebugRivalIndex) {
            case 0: first = 3; second = 19; break;
            case 1: first = 7; second = 11; break;
            case 2: first = 9; second = 8; break;
            case 3: first = 14; second = 5; break;
            case 4:
            default: first = 20; second = 25; break;
            }
            EVENT(second)->field_99EA = 0xFF;
            EVENT(first)->field_99EC = 0;
            gOvl30RecordMenuSelectedId = -1;
            EVENT(second)->field_9A06 |= 0x400;
            break;
        case 27:
            ovl_11_func_800E99EC((s16)gOvl30DebugTimeAdvance, 0, 0, 0);
            gOvl30RecordMenuSelectedId = -1;
            break;
        }
    }
    return cursor;
}

s32 Ovl30EditPetData(s32 cursor) {
    s32 result;

    if (IsScreenFadeComplete() != 0) {
        KanjiFntPrint(gOvl30PetDataHeading);
        gOvl30RecordMenuCursor = cursor;
        Ovl30DrawAndEditValueMenu(&gOvl30PetDataMenu, 0x30);
        result = gOvl30RecordMenuCursor;
    } else {
        result = cursor;
    }
    return result;
}
