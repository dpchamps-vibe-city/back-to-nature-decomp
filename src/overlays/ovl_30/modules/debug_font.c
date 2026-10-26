/* SDK dump-font setup used when entering the debug menu.
 * The adjacent page modules use their own row/descriptor protocols; this
 * initializer touches neither. See notes/file-groupings.md for the bounded
 * organization evidence and ovl30_debug_font.h for display/VRAM constraints. */

#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "ovl30_debug_font.h"

/* Shared text-origin setter; also clears the two secondary pen offsets. */
void func_800183B8(s32 x, s32 y);

void Ovl30InitDebugFont(void) {
    s32 consoleId;

    FntLoad(0x2C0, 0x100);
    consoleId = FntOpen(0x18, 0, 0x100, 0xC8, 0, 0x200);
    SetDumpFnt(consoleId);
    func_800183B8(0x18, 0x18);
}
