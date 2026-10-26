#ifndef OVL11_AREA_RESOURCE_LOAD_H
#define OVL11_AREA_RESOURCE_LOAD_H

#include "common.h"

/* Overlay-11 area loading. Initialize the CD archive index, graphics heap,
 * staging header and render context first. All offsets and copy lengths are
 * bytes unless explicitly described as word/halfword counts. The synchronous
 * CD service reads whole sectors and retries errors indefinitely. Index and
 * destination capacity are unchecked. Do not overlap these calls with another
 * user of the CD request state or graphics-heap staging at base+0x4290.
 * Uploads submit GPU commands; only the entries documented below wait first.
 */

/* 800BCF54: descriptor stride is four words, with the next record's first
 * word also serving as this record's final boundary. Nonzero alreadyLoaded
 * skips only the first read, not its 0xBDC-byte copy or later bank uploads.
 * Updates the retained first-read length and scene setup fields; game flag
 * 9B additionally loads/expands the palette triplet. */
void Ovl11LoadAreaResourceSet(s32 index, s32 alreadyLoaded);

/* 800BD160/800BD5A0: retained empty hooks called by area-entry services. */
void Ovl11AreaResourceEntryHook(void);
void Ovl11AreaSpriteBankHook(void);

/* 800BD168: initialize the area sprite source from staging header word 1;
 * its record address is header[1]+0xBE08. */
void Ovl11InitAreaSpriteSource(void);

/* 800BD1BC: stage boundary[index..index+1], copy all bytes to 8007F7F8. */
void Ovl11LoadCopiedSpriteData(s32 index);
/* 800BD238: only area modes 1/3; wait for GPU, clear 2048 OT entries, copy
 * A20 bytes for index 0 or 838 otherwise. Mode 3 uploads with X offset -256. */
void Ovl11LoadSceneSpriteData(s32 index);
/* 800BD358: write -1 to the retained resource-index marker. */
void Ovl11ClearResourceIndex(void);
/* 800BD374: start/poll descriptor's first subrange into staging. restart=1
 * starts; subsequent polls use 0. Return 0 pending, nonzero complete. */
s32 Ovl11ReadAreaResource(s32 index, s32 restart);

/* 800BD3C4/488: read indexed archive boundaries; copy the prefix length from
 * the corresponding sprite-offset table, then upload its trailing images.
 * The second uses offset-table index+2 and a different installed buffer. */
void Ovl11LoadIndexedSpriteSet(s32 index);
void Ovl11LoadOffsetSpriteSet(s32 index);
/* 800BD538: fixed bank, copy 0x540 words, upload the following images. */
void Ovl11LoadFixedAreaSpriteBank(void);
/* 800BD5A8: -1 is a no-op; otherwise install the record prefix and images,
 * then invoke the existing index-specific record setup service. */
void Ovl11LoadIndexedSpriteRecords(s32 index);
/* 800BD668: stage fixed archive range 74800/A800; no upload or copy. */
void Ovl11StageFixedSpriteBank(void);

/* 800BD6A8: -1 is a no-op. Selector E expands day/hour, setting flags 3F/40
 * for the later hour bands; 3E6 expands season/variant (season -1 is a no-op).
 * Other selectors directly index the tables. Nonzero waitForGpu waits before
 * the read. Seasonal variants upload a second image block. */
void Ovl11LoadCalendarSpriteSet(s16 selector, s16 waitForGpu);
/* 800BD8DC/9D4: load 3800/1800-byte indexed banks to 8007BFF8. The large
 * form also retains the signed selector. No index validation or GPU wait. */
void Ovl11LoadLargeIndexedSpriteBank(s16 index);
void Ovl11LoadSmallIndexedSpriteBank(s32 index);
/* 800BDA20: fixed archive range 3D51000/6800, then submit image uploads. */
void Ovl11LoadFixedSpriteImages(void);
/* 800BDA70: wait for GPU, stage in the inactive render context's bank buffer,
 * copy 770 bytes to the installed bank and upload its trailing images. */
void Ovl11LoadRenderContextSpriteBank(void);

#endif
