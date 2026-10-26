#ifndef OVL11_RECORD_POOL_LIFECYCLE_H
#define OVL11_RECORD_POOL_LIFECYCLE_H

#include "common.h"

/* Reinitialize the resident 10x0xB4, 20x0xB8, 6x0xB0 and 3x0xB4 pools,
 * aged-item cells and the two singleton records. Empty records have type 0
 * and ID FFFF. Seeds record 107 at (-1800, 0, 3100) in native coordinates;
 * the allocator must succeed. Resets dispatch counters to 300, cancels the
 * pending stage mode, and enables pool passes by clearing work+4450 bit 8.
 * This destroys existing pool contents; it allocates no backing memory. */
void Ovl11ResetRecordPools(void);

/* Run the first control pass: ten small records, two distinct passes over
 * the same twenty large records, then six slot records. Work+4450 bit 8
 * suppresses the entire pass and is left unchanged. */
void Ovl11RunRecordPoolFirstPass(void);

/* Run the second update pass over the small/large pools and singleton/aged
 * records unless bit 8 suppresses it. Clear transient spawn flags and bit 8
 * even on a skipped pass, resync both placement rows, and refresh or empty
 * the three tail records according to work+E4CA. The activity counter at
 * +523C saturates at 15. Call after the first pass; ordering is significant.
 * Both passes operate on shared resident storage and are not reentrant. */
void Ovl11RunRecordPoolSecondPass(void);

#endif
