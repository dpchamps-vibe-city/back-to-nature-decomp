#ifndef OVL11_AREA_SELECTION_SETUP_H
#define OVL11_AREA_SELECTION_SETUP_H

/* ovl_11 must be resident. Selection IDs are raw area-resource indices, not
 * pointers. Both operations read shared area/calendar/event state and are
 * non-reentrant. Numeric variants retain their original meaning. */

/* Resolve 0 through the current default; expand IDs 0x50/0x60/0x70 through
 * resident variant fields, apply seasonal/event overrides, then the calendar
 * range remapper (with signed-halfword narrowing). The special 0x12D mode
 * returns season+0xBC directly. Does not clear the event flags. */
s32 Ovl11ResolveAreaSelection(s32 selection);

/* Resolve and install the current work-area selection. With restorePosition
 * nonzero and a selected record, copy its signed XYZ halfwords, restore the
 * grid position, then consume flags 0x42/0x43 to apply fixed XYZ overrides.
 * Coordinates remain in the resident area's native units (no conversion).
 * Only the low 16 bits of loadOptions are forwarded to the setup helper.
 * Updates normal/alternate selection history, installs loaded header/buffer
 * pointers, requests sprite/VRAM resources and rebuilds object events and
 * rotations. Resource reads are synchronous; when the low-half loadOptions
 * value is nonzero, the loader assumes the selected header is already staged. */
void Ovl11InstallAreaSelection(s32 restorePosition, s32 loadOptions);

#endif
