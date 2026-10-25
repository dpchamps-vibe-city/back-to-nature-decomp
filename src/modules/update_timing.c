/*
 * Shared update-timing mode. The paired byte accessors and their input-repeat,
 * CD-volume and feedback-fade consumers support this narrow reconstructed
 * interface within the boot family, not a proven original file boundary.
 * See update_timing.h for the distinct interpretations used by each consumer.
 */

#include "common.h"
#include "update_timing.h"

/* Tentative definition retains GP-relative access to the existing data byte.
 * The initial value (2) remains in the original .sdata, not a C initializer. */
u8 gUpdateTimingMode;

void SetUpdateTimingMode(u8 mode) {
    gUpdateTimingMode = mode;
}

u8 GetUpdateTimingMode(void) {
    return gUpdateTimingMode;
}
