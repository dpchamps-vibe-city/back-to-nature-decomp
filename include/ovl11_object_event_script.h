#ifndef OVL11_OBJECT_EVENT_SCRIPT_H
#define OVL11_OBJECT_EVENT_SCRIPT_H

#include "common.h"
#include "game_types.h"

/* ovl_11 must be resident. These are existing views of the same runtime
 * objects, not interchangeable allocations: callers provide a complete
 * 0x1D4-byte record, valid schedule/command indices and initialized tables.
 * No entry point validates indices or allocates storage.
 *
 * Schedules have thirty 8-byte entries at object +0xE4, selected by +0x22.
 * Commands have 17 twelve-byte entries per 0xCC-byte row, selected by the
 * object's +0xE row and +0xC command index. Position accumulators are 16.16;
 * signed division by 65536 truncates toward zero, not toward minus infinity.
 */

/* Find key in a u32 list prefixed by its element count. Returns the zero-based
 * index or -1. The signed s16 key retains its existing unsigned-word comparison. */
s32 Ovl11FindCountedWordIndex(s16 key, u32 *table);

/* Select the primary or fallback counted sprite bank and initialize source.
 * Returns 0 without initializing source when neither bank contains key, else 1.
 * Publishes the selected group and ONE-based item before special-group remap.
 * A found entry with group -1 deliberately never returns. */
s32 Ovl11SelectScriptSpriteSource(s16 key, SpriteSourceData *source);

/* Load the selected schedule's four-byte descriptor into +0x2C and rotate
 * the +0x14/+0x16 selectors. A changed selector clears flag 0x10 at +0x1E. */
void Ovl11LoadScheduledEvent(Ovl11EventObject *object);

/* Dispatch the currently indexed command and apply its animation operation. */
void Ovl11DispatchCurrentScriptCommand(Ovl11C3B78Arg0 *object);

/* Integrate one X/Z movement step and advance a satisfied command. */
void Ovl11MoveTowardScriptCommand(Ovl11C3B78Arg0 *object);

/* Run up to tickCount command/countdown ticks (negative means none), after
 * one collision/state pass. -2 and -3 are special countdown states. The loop
 * can stop early when command processing clears object flag 0x100.
 * Live-mode zero is used by time-of-day reconstruction, not normal frames. */
void Ovl11RunObjectScriptTicks(Ovl11C3B78Arg0 *object, s32 tickCount);

/* Increment the script tick and process all 37 global records unless state
 * word +0x10 has 0x02000000 set. Per-record fades/flags may suppress updates.
 * The shared tick is NOT a frame clock: replay ticks increment it as well. */
void Ovl11UpdateObjectEvents(void);

#endif
