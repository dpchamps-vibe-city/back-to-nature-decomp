#include "common.h"
#include "ovl15_save_checksums.h"
#include "game_types.h"
#include "psyq/memory.h"
#include "psyq/strings.h"
#include "ovl15_save_card_title.h"
#include "ovl15_save_image.h"
#include "ovl15_card_filename.h"
#include "ovl15_save_slot_header.h"

void func_8001AC10(u32 *arg0, s32 arg1, s32 arg2);

void ovl_15_func_8012FAE0(void) {
    s32 temp_a1;
    u32 *icons;
    u32 paletteOffset;

    temp_a1 = gActiveRenderContext->field_D8;
    icons = D_801375B4[gOvl15SelectedSlot];
    func_8001AC10((u32 *)(temp_a1 + 0x18), temp_a1 + 0x14,
                 (s32)(D_8005346A + gTextBankOffsets[0]));
    Ovl15BuildSaveSlotHeader(gOvl15SelectedSlot);
    Ovl15SerializeSaveImage();
    Ovl15InsertSaveChecksumSlots();
    Ovl15WriteSaveChecksums();
    Ovl15BuildCardFilename(Ovl15CardFilename, gOvl15SelectedSlot);
    memcpy(&Ovl15CardHeader, D_8012DEBC, 3);
    Ovl15CardHeader.type = 0x13;
    Ovl15CardHeader.blocks = 4;
    Ovl15BuildSaveCardTitle(gOvl15SelectedSlot);
    memset(Ovl15CardHeader.reserved, 0, sizeof(Ovl15CardHeader.reserved));
    paletteOffset = icons[0] + 0x14;
    Ovl15CardHeader.palette = *(Ovl15IconPalette *)(gOvl15CardIconResourceBuffer + paletteOffset);
    memcpy(gOvl15TransferBuffer, &Ovl15CardHeader, sizeof(Ovl15CardHeader));
    memcpy(gOvl15TransferBuffer + 0x80, (void *)(icons[0] + (u32)gOvl15CardIconResourceBuffer + 0x40), 0x80);
    memcpy(gOvl15TransferBuffer + 0x100, (void *)(icons[1] + (u32)gOvl15CardIconResourceBuffer + 0x40), 0x80);
    memcpy(gOvl15TransferBuffer + 0x180, (void *)(icons[2] + (u32)gOvl15CardIconResourceBuffer + 0x40), 0x80);
    gOvl15MenuState = 0x14;
}
