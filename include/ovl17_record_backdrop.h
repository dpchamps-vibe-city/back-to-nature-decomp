#ifndef OVL17_RECORD_BACKDROP_H
#define OVL17_RECORD_BACKDROP_H

#include "common.h"

/* Draw the record scene's 64/76-pixel tile strips, visible endpoint markers
 * and sentinel-terminated prop table. scrollQ12 is signed screen scroll in
 * 1/4096-pixel units; conversions truncate toward zero. Props use twice the
 * whole-pixel scroll and strict visibility bounds. Tiles/markers use the
 * original fixed 320-pixel layout; props use the active HWD0/VWD0 dimensions.
 * Requires initialized display/scene sprite sources and active render context.
 * Appends drawing packets and changes both sources' animation/frame selections;
 * does not advance the scroll or a frame timer. No bounds/capacity checks. */
void Ovl17DrawRecordBackdrop(s32 scrollQ12);

#endif
