/* Area archive reads and sprite-bank installation.
 * The descriptor/range tables are private to this load protocol; destinations
 * and graphics-heap staging are shared with other services. Reads overwrite
 * staging, so these operations must be serialized. See the public header for
 * byte/count units and notes/file-groupings.md for membership/boundaries. */
#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/memory.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "cd_loader.h"
#include "sprite_upload.h"
#include "game_flags.h"
#include "ovl11_guarded_sprite_bank.h"
#include "sprite_source.h"
#include "ovl11_block_copy.h"
#include "ovl11_area_resource_load.h"
#include "ovl11_record_sprite_sources.h"
#include "ovl11_condition_evaluation.h"

void Ovl11LoadPaletteBankTriplet(void);

void Ovl11LoadAreaResourceSet(s32 index, s32 alreadyLoaded) {
    s32 *record;
    s32 start;
    s32 imageStart;
    s32 bankStart;
    u8 *work;

    index *= 4;
    record = &gOvl11ResourceDescriptors + index;
    gLoadedAreaResourceBytes = record[1] - record[0];
    start = record[0];
    if (alreadyLoaded == 0) {
        CdReadDiscSectorsBlocking(0, start, gLoadedAreaResourceBytes, 0, D_8007AFF0);
    }
    memcpy(gLoadedAreaHeaderCopy, (void *)D_8007AFF0, 0xBDC);
    work = (u8 *)&D_8007AFF0;
    *(Ovl25SetupStatePair *)(work + 0x25474) =
        *(Ovl25SetupStatePair *)(work + 0x2479C);
    *(u8 **)(work + 0x25388) = gLoadedAreaHeaderCopy;
    if (GetGameFlag(0x9B) == 1) {
        Ovl11LoadPaletteBankTriplet();
    }
    imageStart = record[1];
    CdReadDiscSectorsBlocking(0, imageStart, record[2] - imageStart, 0, gGraphicsHeapBase + 0x4290);
    UploadSpriteImages((u8 *)(gGraphicsHeapBase + 0x4290));
    bankStart = record[2];
    CdReadDiscSectorsBlocking(0, bankStart, record[4] - bankStart, 0, gGraphicsHeapBase + 0x4290);
    Ovl11InstallConditionRecords((void *)(gGraphicsHeapBase + 0x4290));
    Ovl11UploadGuardedSpriteBank((u8 *)gGraphicsHeapBase + ((record[3] - record[2]) + 0x4290));
}

void Ovl11AreaResourceEntryHook(void) {
}

void Ovl11InitAreaSpriteSource(void) {
    s32 *header;

    header = (s32 *)&D_8007AFF0;
    InitSpriteSourceData((SpriteSourceData *)D_80071C60,
                        (SpriteDataHeader *)header[1]);
    SetSpriteSourceRecordAddress((SomeStruct *)D_80071C60, header[1] + 0xBE08);
}

void Ovl11LoadCopiedSpriteData(s32 index) {
    s32 *bounds;

    bounds = &gOvl11CopiedSpriteBoundaries[index];
    CdReadDiscSectorsBlocking(0, bounds[0], bounds[1] - bounds[0], 0, gGraphicsHeapBase + 0x4290);
    memcpy(&D_8007F7F8, (void *)(gGraphicsHeapBase + 0x4290), bounds[1] - bounds[0]);
}

void Ovl11LoadSceneSpriteData(s32 index) {
    s32 *bounds;
    s32 start;
    s32 copyBytes;
    char *work;
    char *state;

    work = (char *)&D_8007AFF0;
    state = work + 0x20000;
    if ((*(s16 *)(state + 0x5476) == 1) || (*(s16 *)(state + 0x5476) == 3)) {
        DrawSync(0);
        ClearOTagR((u_long *)gActiveRenderContext->field_120, 0x800);
        copyBytes = 0xA20;
        bounds = &gOvl11SceneSpriteBoundaries[index];
        start = bounds[0];
        CdReadDiscSectorsBlocking(0, start, bounds[1] - start, 0, gGraphicsHeapBase + 0x4290);
        if (index != 0) {
            copyBytes = 0x838;
        }
        memcpy(work + 8, (void *)(gGraphicsHeapBase + 0x4290), copyBytes);
        if (*(s16 *)(state + 0x5476) == 3) {
            UploadSpriteImagesAtOffsets((u8 *)(gGraphicsHeapBase + 0x4290 + copyBytes), -0x100, 0, 0, 0);
            return;
        }
        UploadSpriteImages((u8 *)(gGraphicsHeapBase + 0x4290 + copyBytes));
    }
}

void Ovl11ClearResourceIndex(void) {
    gOvl11ResourceIndex = -1;
}

void Ovl11UnusedAreaResourceHook(void) {
    int reserved[6]; /* Retained unused automatic storage in this original hook. */
}

s32 Ovl11ReadAreaResource(s32 index, s32 restart) {
    u8 *record;
    s32 start;

    record = (index * 0x10) + (u8 *)&gOvl11ResourceDescriptors;
    start = *(s32 *)record;
    return CdReadArchiveBytes(0, start, *(s32 *)(record + 4) - start,
                              (u8 *)D_8007AFF0, 1, restart) != 0;
}

void Ovl11LoadIndexedSpriteSet(s32 index) {
    s32 *bounds;
    s32 *offsets;
    s32 copyBytes;

    bounds = &gOvl11IndexedSpriteBoundaries[index];
    CdReadDiscSectorsBlocking(0, bounds[0], bounds[1] - bounds[0], 0, gGraphicsHeapBase + 0x4290);
    ((Ovl11LoadedItemView *)&D_8007AFF0)->loadedItem = index;
    offsets = D_80124FCC_tables[index];
    copyBytes = *offsets;
    copyBytes = offsets[copyBytes + 1];
    memcpy(D_8008F7F8_words, (void *)(gGraphicsHeapBase + 0x4290), copyBytes);
    UploadSpriteImages((u8 *)gGraphicsHeapBase + 0x4290 + copyBytes);
}

void Ovl11LoadOffsetSpriteSet(s32 index) {
    s32 *bounds;
    s32 *offsets;
    s32 imageOffset;

    bounds = &gOvl11OffsetSpriteBoundaries[index];
    offsets = D_80124FCC_tables[index + 2];
    imageOffset = *(s32 *)((u8 *)offsets + (*offsets * 4) + 4);
    CdReadDiscSectorsBlocking(0, bounds[0], bounds[1] - bounds[0], 0, gGraphicsHeapBase + 0x4290);
    memcpy(&gSeasonalObjectSpriteBankBase, (void *)(gGraphicsHeapBase + 0x4290), imageOffset);
    imageOffset = imageOffset + 0x4290;
    UploadSpriteImages((u8 *)(gGraphicsHeapBase + imageOffset));
}

void Ovl11LoadFixedAreaSpriteBank(void) {
    CdReadDiscSectorsBlocking(0, 0x0380B000, 0x5000, 0, gGraphicsHeapBase + 0x4290);
    Ovl11CopyWordBlock(&gFixedIdSpriteBankBase, gGraphicsHeapBase + 0x4290, 0x540U);
    UploadSpriteImages(gGraphicsHeapBase + 0x5790);
}

void Ovl11AreaSpriteBankHook(void) {
}

void Ovl11LoadIndexedSpriteRecords(s32 index) {
    s32 *bounds;
    s32 *offsets;
    s32 imageOffset;

    if (index != -1) {
        bounds = &gOvl11RecordSpriteBoundaries[index];
        offsets = gOvl11SpriteRecordSets[index];
        imageOffset = *(s32 *)((u8 *)offsets + (*offsets * 4) + 4);
        CdReadDiscSectorsBlocking(0, bounds[0], bounds[1] - bounds[0], 0, gGraphicsHeapBase + 0x4290);
        memcpy(&gLoadedSpriteDataBlob, (void *)(gGraphicsHeapBase + 0x4290), imageOffset);
        imageOffset = imageOffset + 0x4290;
        UploadSpriteImages((u8 *)(gGraphicsHeapBase + imageOffset));
        Ovl11InstallRecordSpriteSources(index);
    }
}

void Ovl11StageFixedSpriteBank(void) {
    CdReadDiscSectorsBlocking(0, 0x74800, 0xA800, 0, gGraphicsHeapBase + 0x4290);
}

void Ovl11LoadCalendarSpriteSet(s16 selector, s16 waitForGpu) {
    s16 index;
    s32 *offsets;
    s32 baseIndex;
    s32 wordOffset;
    s32 start;
    s32 imageOffset;
    s32 staging;
    s32 variantKind;
    s32 *bounds;

    index = selector;
    variantKind = 0;
    if (index != -1) {
        if (index == 0xE) {
            index = (s16) ((struct Ovl11SpriteSelectors *) D_8006C838)->hour / 6;
            if (index >= 3) {
                index = 2;
            }
            if (index == 1) {
                SetGameFlag(0x3FU, 1U);
            }
            if (index == 2) {
                SetGameFlag(0x40U, 1U);
            }
            variantKind = 1;
            index = index + (s16) ((struct Ovl11SpriteSelectors *) D_8006C838)->day * 3;
            index += 0xE;
        }
        if (index == 0x3E6) {
            if (((struct Ovl11SpriteSelectors *) D_8006C838)->season != -1) {
                index = ((struct Ovl11SpriteSelectors *) D_8006C838)->season * 3 + 0x23;
                baseIndex = index;
                baseIndex += (u16) ((struct Ovl11SpriteSelectors *) D_8006C838)->variant;
                index = baseIndex;
                variantKind = 2;
                goto load_selected;
            }
        } else {
load_selected:
            offsets = D_80125DC0[index];
            wordOffset = *offsets;
            imageOffset = offsets[wordOffset + 1];
            bounds = &gOvl11CalendarSpriteBoundaries[index];
            staging = gGraphicsHeapBase + 0x4290;
            if (waitForGpu != 0) {
                DrawSync(0);
            }
            start = bounds[0];
            CdReadDiscSectorsBlocking(0, start, bounds[1] - start, 0, staging);
            if (variantKind == 1) {
                memcpy(D_8008F7F8_words, (void *) staging, imageOffset);
            } else {
                memcpy(&gAreaSpriteWorkBase, (void *) staging, imageOffset);
            }
            UploadSpriteImages((u8 *) (staging + imageOffset));
            if (variantKind == 2) {
                offsets = D_80125DC0[index];
                wordOffset = *offsets;
                imageOffset = offsets[wordOffset + 2];
                UploadSpriteImages((u8 *) (staging + imageOffset));
            }
        }
    }
}

void Ovl11LoadLargeIndexedSpriteBank(s16 index) {
    gOvl11LoadedSpriteBankSelector = index;
    CdReadDiscSectorsBlocking(0, ((index << 3) - index << 11) + 0x3CB8800, 0x3800, 0, ((s32)(&gLoadedSpriteBankBuffer)));
}

void Ovl11LoadPaletteBankTriplet(void) {
    u16 *palette;

    CdReadDiscSectorsBlocking(0, 0x03D57800, 0x800, 0, gGraphicsHeapBase + 0x4290);
    palette = gGraphicsHeapBase + 0x4290;
    gOvl11PaletteBankPointer = palette;
    Ovl11CopyHalfwordBlock((u16 *) &gItemOverflowSlotBase, palette, 0xC0U);
    Ovl11CopyHalfwordBlock((u16 *) ((&gItemOverflowSlotBase + 0x60)), gGraphicsHeapBase + 0x4410, 0xC0U);
    Ovl11CopyHalfwordBlock((u16 *) ((&gItemOverflowSlotBase - 0x60)), gGraphicsHeapBase + 0x4590, 0xC0U);
}

void Ovl11LoadSmallIndexedSpriteBank(s32 index) {
    CdReadDiscSectorsBlocking(0, ((index << 1) + index << 11) + 0x3D5A800, 0x1800, 0, ((s32)(&gLoadedSpriteBankBuffer)));
}

void Ovl11LoadFixedSpriteImages(void) {
    CdReadDiscSectorsBlocking(0, 0x3D51000, 0x6800, 0, gGraphicsHeapBase + 0x4290);
    UploadSpriteImages(((void *)(gGraphicsHeapBase + 0x4290)));
}

void Ovl11LoadRenderContextSpriteBank(void) {
    s32 staging;

    DrawSync(0);
    if ((s32)gActiveRenderContext == (s32)&gRenderContexts) {
        staging = *(s32 *)((u8 *)&gRenderContexts + 0x258);
    } else {
        staging = *(s32 *)((u8 *)&gRenderContexts + 0x124);
    }
    CdReadDiscSectorsBlocking(0, 0x7F000, 0x4800, 0, staging);
    memcpy(&gAreaSpriteWorkBase, (void *)staging, 0x770);
    UploadSpriteImages((u8 *)staging + 0x770);
}
