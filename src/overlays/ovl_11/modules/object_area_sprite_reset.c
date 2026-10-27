/*
 * object_area_sprite_reset — reconstructed translation unit.
 *
 * Clear the supplied object flag 0x1000, reinitialize the shared area sprite
 * source, and reset the global primary-object request. This adapter does not
 * allocate or initialize the supplied object.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "ovl11_object_area_sprite_reset.h"
#include "ovl11_primary_object_requests.h"
#include "ovl11_area_resource_load.h"

void Ovl11ResetObjectAreaSpriteSource(Recon_ovl_11_func_800CD08C_A0View *object) {
    object->unk6C = object->unk6C & -0x1001;
    Ovl11InitAreaSpriteSource();
    Ovl11ResetPrimaryObjectRequest();
}
