/*
 * Scripted camera-target positioning. The mover keeps full-word XYZ between
 * calls and publishes signed halfwords to the shared camera work area.
 * The companion flag service is also used by other scene commands.
 */
#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/abs.h"
#include "ovl11_camera_position_control.h"

s32 Ovl11MoveCameraTarget(s16 speedCode, s32 height, s32 x, s32 z) {
    s16 y = height;
    VECTOR delta;
    s16 selector;
    s32 divisor;
    s16 stepSize;
    s32 complete;
    s32 horizontalDistance;
    s32 distance;
    s32 *position;

    selector = speedCode;
    gOvl11SelectedPositionRecord = -1;
    ((struct struct_8006C838_view *)D_8006C838)->field_0C |= 0x400000;
    Ovl11SetPositionControlFlags(0, 0, 0, 0);
    stepSize = 0x18;
    selector += 2;
    switch (selector) {
    case 1: {
        u8 *cameraWork = (u8 *)&D_8007AFF0;
        *(s16 *)(cameraWork + 0x253B6) = y;
        *(s16 *)(cameraWork + 0x253B4) = x;
        *(s16 *)(cameraWork + 0x253B8) = z;
        return 1;
    }
    case 0:
        stepSize = 5;
        break;
    case 2:
        stepSize = 0x18;
        break;
    case 3:
        stepSize = 0x24;
        break;
    case 4:
        stepSize = 0x30;
        break;
    case 5:
        stepSize = 0x48;
        break;
    }
    if (gOvl11CameraMoveActive == 0) {
        u8 *cameraWork = (u8 *)&D_8007AFF0;
        gOvl11CameraMoveActive = 1;
        gOvl11CameraMovePosition[0] = *(s16 *)(cameraWork + 0x253B4);
        gOvl11CameraMovePosition[1] = *(s16 *)(cameraWork + 0x253B6);
        gOvl11CameraMovePosition[2] = *(s16 *)(cameraWork + 0x253B8);
    }
    position = gOvl11CameraMovePosition;
    delta.vy = abs(y - position[1]);
    delta.vx = abs(x - gOvl11CameraMovePosition[0]);
    delta.vz = abs(z - position[2]);
    horizontalDistance = SquareRoot0(delta.vx * delta.vx + delta.vz * delta.vz);
    distance = horizontalDistance < delta.vy ? delta.vy : horizontalDistance;
    divisor = distance / stepSize;
    if (divisor != 0) {
        goto interpolate;
    }
    {
        complete = 0;
        if (position[2] == z && gOvl11CameraMovePosition[0] == x &&
            position[1] == y) {
            gOvl11CameraMoveActive = 0;
            complete = 1;
        } else {
            u8 *cameraWork = (u8 *)&D_8007AFF0;
            /* Arrival is acknowledged on the NEXT call, not this final snap. */
            complete = 0;
            gOvl11CameraMovePosition[2] = z;
            gOvl11CameraMovePosition[0] = x;
            gOvl11CameraMovePosition[1] = y;
            *(s16 *)(cameraWork + 0x253B8) = (s16)(u16)gOvl11CameraMovePosition[2];
            *(s16 *)(cameraWork + 0x253B4) = (s16)(u16)gOvl11CameraMovePosition[0];
            *(s16 *)(cameraWork + 0x253B6) = (s16)(u16)gOvl11CameraMovePosition[1];
        }
    }
    goto done;
interpolate:
    {
        gOvl11CameraMovePosition[2] += (z - gOvl11CameraMovePosition[2]) / divisor;
        gOvl11CameraMovePosition[0] += (x - gOvl11CameraMovePosition[0]) / divisor;
        gOvl11CameraMovePosition[1] += (y - gOvl11CameraMovePosition[1]) / divisor;
        complete = 0;
        {
            u8 *cameraWork = (u8 *)&D_8007AFF0;
            *(s16 *)(cameraWork + 0x253B8) = (s16)(u16)gOvl11CameraMovePosition[2];
            *(s16 *)(cameraWork + 0x253B4) = (s16)(u16)gOvl11CameraMovePosition[0];
            *(s16 *)(cameraWork + 0x253B6) = (s16)(u16)gOvl11CameraMovePosition[1];
        }
    }
done:
    return complete;
}

s32 Ovl11SetPositionControlFlags(s16 release, s16 keepFlag2, s32 unused2, s32 unused3) {
    if (release != 0) {
        char *work = (char *)&D_8006C838;
        *(s32 *)(work + 0x5234) &= 0xFF7FFBFF;
        *(s32 *)(work + 0x000C) &= 0xFFBFFFFF;
        if (*(s16 *)(work + 0x52C6) != 1 && *(s16 *)(work + 0x52C6) != 6) {
            *(u16 *)(work + 0x51FE) |= 0x0008;
        } else {
            char *clearWork = (char *)&D_8006C838;
            *(u16 *)(clearWork + 0x51FE) &= 0xFFF7;
        }
    } else {
        char *setWork = (char *)&D_8006C838;
        *(s32 *)(setWork + 0x5234) |= 0x00800000;
    }
    if (keepFlag2 == 0) {
        char *tailWork = (char *)&D_8006C838;
        *(s32 *)(tailWork + 0x5234) &= 0xFFFFFFFD;
    }
    return 1;
}
