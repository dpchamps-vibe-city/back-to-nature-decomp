#ifndef OVL11_OBJECT_PROGRAM_INSTALL_H
#define OVL11_OBJECT_PROGRAM_INSTALL_H

#include "common.h"

/* Install a resident object's tagged archive, passed through the existing
 * 32-bit address ABI. Copies its program entry (at most 0xCE0 bytes), uploads
 * image entries, then moves sprite data immediately below asset-scratch end.
 * The archive must remain valid throughout the call; scratch space and entry
 * offsets are not bounds-checked. The installed data must survive its users.
 *
 * Archive flag 0x20 selects VRAM X offset 0 instead of 384 and sets work flag
 * 0x10; Y offset is 0. Rebuilds sprite-offset/source state and clears eligible
 * state objects when sprite data is nonempty. Shared installation state makes
 * this non-reentrant. The program record is copied even with no sprite data.
 *
 * This and ovl11_active_record_sprites.h expose the install/render sides of
 * object_sprite_resources. Entity entries tagged 0x800 claim the first free
 * source among ten slots and write its selector to the parallel spawn record.
 * A full source pool silently skips installation. Call the entity-source
 * reset in ovl11_object_load_resets.h before loading a replacement archive;
 * this installer does not itself reset the pool or allocate its side records.
 */
void Ovl11InstallObjectProgramArchive(s32 archiveAddress);

/* Copy each of 37 preceding 0x30-byte sprite sources into the current source
 * at stride 0x1D4. Object id 0x82 also reselects each script sprite source.
 * Requires the resident record bank and current-object pointer initialized.
 */
void Ovl11CopyPreviousObjectSpriteSources(void);

#endif
