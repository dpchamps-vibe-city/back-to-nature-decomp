#ifndef OVL11_ACTIVE_RECORD_SPRITES_H
#define OVL11_ACTIVE_RECORD_SPRITES_H

#include "common.h"

/* Requires the loaded area's record pools, sprite sources, entity side table,
 * active render context and GTE scene state to be initialized. No allocation
 * or bounds checking. Coordinates retain their existing raw units; projection
 * uses their unsigned low halfwords, adjusted by (+150,-100,-150), and depth
 * selects an ordering-table entry through (depth>>2)*4.
 *
 * Work flag 0x00800000 suppresses BOTH scan drivers completely. The distinct
 * 0x08000000 pause flag suppresses movement and ordinary animation stepping,
 * not drawing; codes 19D/1BC..1BF still animate in the active-record path.
 */

/* Visit all 37 slots (stride 0x1D4); require both slot flags 0x18000000.
 * Tick the timed effect, integrate nonzero displacement with surface-height
 * resolution, draw the sprite/effect/optional icon, and advance animation.
 * Selected records can move five OT entries forward when game flag 0x72 is on.
 * Animation can advance even when projection culls a record.
 */
void Ovl11TickActiveRecordSprites(void);

/* Walk ten parallel entity/source slots (0x18/0x30 strides), drawing and
 * stepping those whose entity +4 bit 0 is set. Then draw the current-object
 * sprite if its shared +4476 gate is zero. The latter draw does not step a
 * source. These routines submit packets and are not side-effect-free queries.
 */
void Ovl11DrawAttachmentRecordSprites(void);

#endif
