#ifndef OVL11_RECORD_SPRITE_SOURCES_H
#define OVL11_RECORD_SPRITE_SOURCES_H

#include "common.h"

/* Initialize consecutive 0x30-byte records at gSpriteSourceBankBase using
 * gOvl11SpriteRecordSets[recordSet]: a signed word count followed by byte
 * offsets into gLoadedSpriteDataBlob. The caller must load that blob first
 * and supply a valid, unchecked table index and offsets. A count >=45 is
 * reduced to 44; a nonpositive count writes nothing. Records beyond the
 * chosen count are left intact. This does not load data or upload textures. */
void Ovl11InstallRecordSpriteSources(s32 recordSet);

#endif
