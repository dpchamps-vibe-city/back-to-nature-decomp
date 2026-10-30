#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libetc.h"
#include "pad_input_update.h"

/* Adjust the display origin with the D-pad until L2 is pressed. */
void func_80012AB0(s16 width, s16 height) {
    DISPENV display;

    SetDefDispEnv(&display, 0, 0, width, height);
    PutDispEnv(&display);
    gPad0Input->pressedButtons = 0;
    do {
        if (gPad0Input->heldButtons & PADLleft) {
            display.disp.x -= 8;
        }
        if (gPad0Input->heldButtons & PADLright) {
            display.disp.x += 8;
        }
        if (gPad0Input->heldButtons & PADLup) {
            display.disp.y -= 8;
        }
        if (gPad0Input->heldButtons & PADLdown) {
            display.disp.y += 8;
        }
        VSync(0);
        PollPadInputs();
        PutDispEnv(&display);
        if (gPad0Input->pressedButtons & PADRright) {
            SetDefDispEnv((DISPENV *)&gRenderDisplayEnvView[0],
                          display.disp.x, display.disp.y, 640, 480);
            SetDefDispEnv((DISPENV *)&gRenderDisplayEnvView[1],
                          display.disp.x, display.disp.y, 640, 480);
        }
        if (gPad0Input->pressedButtons & PADRdown) {
            SetDefDispEnv((DISPENV *)&gRenderDisplayEnvView[0],
                          0, 240, 320, 240);
            SetDefDispEnv((DISPENV *)&gRenderDisplayEnvView[1],
                          0, 0, 320, 240);
        }
    } while (!(gPad0Input->pressedButtons & PADL2));
}
