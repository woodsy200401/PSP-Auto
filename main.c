#include <pspkernel.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#include <stdio.h>

PSP_MODULE_INFO("PSP Auto", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

static int running = 1;

static int exit_callback(int arg1, int arg2, void *common) {
    (void)arg1; (void)arg2; (void)common;
    running = 0;
    return 0;
}

static int callback_thread(SceSize args, void *argp) {
    (void)args; (void)argp;
    int cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
    sceKernelRegisterExitCallback(cbid);
    sceKernelSleepThreadCB();
    return 0;
}

static void setup_callbacks(void) {
    int thid = sceKernelCreateThread("callback_thread", callback_thread,
                                     0x11, 0xFA0, 0, NULL);
    if (thid >= 0) sceKernelStartThread(thid, 0, NULL);
}

static const char *items[] = {"Navigation", "Music", "Phone", "Settings"};
static const int item_count = 4;

static void draw(int selected, int page) {
    int i;
    pspDebugScreenClear();
    pspDebugScreenSetXY(2, 1);
    pspDebugScreenPrintf("============================================");
    pspDebugScreenSetXY(2, 2);
    pspDebugScreenPrintf("                PSP AUTO 0.1                ");
    pspDebugScreenSetXY(2, 3);
    pspDebugScreenPrintf("============================================");

    if (page < 0) {
        pspDebugScreenSetXY(4, 6);
        pspDebugScreenPrintf("HOME - OFFLINE DEMONSTRATION");
        for (i = 0; i < item_count; ++i) {
            pspDebugScreenSetXY(7, 9 + i * 3);
            pspDebugScreenPrintf("%s  %s", selected == i ? ">" : " ", items[i]);
        }
        pspDebugScreenSetXY(3, 25);
        pspDebugScreenPrintf("UP/DOWN: select   X: open   START: exit");
    } else {
        pspDebugScreenSetXY(4, 7);
        pspDebugScreenPrintf("%s", items[page]);
        pspDebugScreenSetXY(4, 10);
        if (page == 0) pspDebugScreenPrintf("Turn directions: awaiting phone bridge");
        if (page == 1) pspDebugScreenPrintf("Media controls: awaiting phone bridge");
        if (page == 2) pspDebugScreenPrintf("Calls and alerts: awaiting phone bridge");
        if (page == 3) pspDebugScreenPrintf("Connection mode: offline");
        pspDebugScreenSetXY(3, 25);
        pspDebugScreenPrintf("O: back   START: exit");
    }
    pspDebugScreenSetXY(3, 30);
    pspDebugScreenPrintf("PSP Auto - homebrew prototype, not Android Auto");
}

int main(void) {
    SceCtrlData pad;
    unsigned int previous = 0;
    int selected = 0, page = -1;
    pspDebugScreenInit();
    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_DIGITAL);
    setup_callbacks();
    draw(selected, page);

    while (running) {
        unsigned int pressed;
        sceCtrlPeekBufferPositive(&pad, 1);
        pressed = pad.Buttons & ~previous;
        if (pressed & PSP_CTRL_START) break;
        if (page == -1) {
            if (pressed & PSP_CTRL_UP) {
                selected = (selected + item_count - 1) % item_count;
                draw(selected, page);
            }
            if (pressed & PSP_CTRL_DOWN) {
                selected = (selected + 1) % item_count;
                draw(selected, page);
            }
            if (pressed & PSP_CTRL_CROSS) {
                page = selected;
                draw(selected, page);
            }
        } else if (pressed & PSP_CTRL_CIRCLE) {
            page = -1;
            draw(selected, page);
        }
        previous = pad.Buttons;
        sceDisplayWaitVblankStart();
    }
    sceKernelExitGame();
    return 0;
}
