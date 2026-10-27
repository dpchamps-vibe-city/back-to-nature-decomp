/* 13x13 byte-grid transforms, original ovl_11:800D6488-800D6628.
 * Pure coordinate remapping; this service neither owns nor looks up grids.
 * The traversal order is observable for overlapping buffers. The tag scanner
 * uses a separate stack destination. See ovl11_grid_transform.h. */

#include "common.h"
#include "ovl11_grid_transform.h"

void Ovl11TransformGridCells(u8 mode, u8 dst[][13], u8 src[][13]) {
    s32 row;
    s32 column;

    switch (mode) {
    case 0:
        for (row = 0; row < 13; row++) {
            for (column = 12; column >= 0; column--) {
                dst[row][column] = src[column][row];
            }
        }
        break;
    case 1:
        for (row = 0; row < 13; row++) {
            for (column = 12; column >= 0; column--) {
                dst[row][12 - column] = src[row][column];
            }
        }
        break;
    case 2:
        for (row = 12; row >= 0; row--) {
            for (column = 0; column < 13; column++) {
                dst[12 - row][column] = src[column][row];
            }
        }
        break;
    case 3:
        for (row = 0; row < 13; row++) {
            for (column = 0; column < 13; column++) {
                dst[row][column] = src[row][column];
            }
        }
        break;
    }
}
