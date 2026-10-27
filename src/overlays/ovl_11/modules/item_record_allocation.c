/* Item-code routing across the existing record pools and aged-item queue.
 * Keep the shared state-object tail: this switch owns the original 219-word
 * dispatch table. Pool allocation and storage remain in their own services. */
#include "common.h"
#include "game_types.h"
#include "aged_item_pool.h"
#include "ovl11_code160_record_creation.h"
#include "ovl11_tagged_record_creation.h"
#include "ovl11_small_record_handlers.h"
#include "ovl11_object_record_lifecycle.h"
#include "ovl11_item_record_allocation.h"

struct_800759E4 *ovl_11_func_80108CD0(s32 itemCode, s32 mode);
u16 *ovl_11_func_8010B1C4(s32 itemCode);
void ovl_11_func_800D2D54(void *record);

u16 *Ovl11AllocateItemRecord(s32 itemCode, s32 mode) {
    s32 stateCode;
    s32 recordAddress;
    s32 resultAddress;

    recordAddress = 0;
    switch (itemCode) {
    case 0x160:
    case 0x161:
    case 0x162:
        recordAddress = Ovl11AllocateCode160Record(itemCode, mode, 1);
        break;
    case 0x163:
        stateCode = 0x162;
        goto allocate_state_object;
    case 0x164:
    case 0x165:
        recordAddress = Ovl11AllocateTaggedRecord(itemCode, mode, 1);
        break;
    case 0x166:
        stateCode = 0x165;
        goto allocate_state_object;
    case 0x109:
    case 0x10A:
        recordAddress = Ovl11AllocateSmallRecord(itemCode, mode, 1);
        break;
    case 0x108:
    case 0x10B:
    case 0x10C:
    case 0x10D:
    case 0x10E:
    case 0x10F:
    case 0x110:
    case 0x112:
    case 0x113:
    case 0x114:
    case 0x115:
    case 0x116:
    case 0x117:
    case 0x118:
    case 0x119:
    case 0x11A:
    case 0x153:
    case 0x154:
    case 0x155:
    case 0x156:
    case 0x157:
    case 0x159:
    case 0x15B:
    case 0x17A:
    case 0x17B:
        stateCode = itemCode;
allocate_state_object:
        recordAddress = Ovl11AllocateStateObjectRecord(stateCode);
        break;
    default:
        break;
    case 0x106:
    case 0x107:
        recordAddress = ovl_11_func_80108CD0(itemCode, 1);
        break;
    case 0x15E:
    case 0x15F:
        recordAddress = ovl_11_func_8010B1C4(itemCode);
        break;
    case 0xA1:
    case 0xA2:
    case 0xA3:
        Ovl11AddAgedItem(itemCode);
        break;
    }
    resultAddress = 0;
    if (recordAddress != 0) {
        ovl_11_func_800D2D54((void *)recordAddress);
        resultAddress = recordAddress;
    }
    return (u16 *)resultAddress;
}
