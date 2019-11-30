#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <switch.h>

#include "util.h"
#include "touch.h"
#include "menu.h"
#include "unzip.h"
#include "download.h"

//#define DEBUG                                              // enable for nxlink debug

int appInit()
{
    Result rc;
    if (R_FAILED(rc = socketInitializeDefault()))           // for curl / nxlink.
        printf("socketInitializeDefault() failed: 0x%x.\n\n", rc);

    #ifdef DEBUG
    if (R_FAILED(rc = nxlinkStdio()))                       // redirect all printout to console window.
        printf("nxlinkStdio() failed: 0x%x.\n\n", rc);
    #endif

    if (R_FAILED(rc = plInitialize()))                      // for shared fonts.
        printf("plInitialize() failed: 0x%x.\n\n", rc);

    if (R_FAILED(rc = romfsInit()))                         // load textures from app.
        printf("romfsInit() failed: 0x%x.\n\n", rc);

    sdlInit();                                              // int all of sdl and start loading textures.

    romfsExit();                                            // exit romfs after loading sdl as we no longer need it.

    return 0;
}

void appExit()
{
    sdlExit();
    socketExit();
    plExit();
    splExit();
    setsysExit();
}

int main(int argc, char **argv)
{
    // init stuff.
    appInit();
    mkdir(APP_PATH, 0777);
    chdir(ROOT);

    // set the cursor position to 0.
    short cursor = 0;

    // touch variables.
    int touch_lock = OFF;
    u32 tch = 0;
    touchPosition touch;
	
    AppletType at = appletGetAppletType();
    if (at != AppletType_Application && at != AppletType_SystemApplication) 
	{
		drawText(fntSmall, 25,50, SDL_GetColour(white), "You are running Applet Mode! Downloading and extracting large files (like firmware) will fail with an out of memory error! Run hbmenu via title override or an NSP forwarder.");
		
		drawText(fntSmall, 10,350, SDL_GetColour(white), "Note that writing to an exFAT formatted SD can cause corruption. Always use FAT32 with homebrew.");
		drawImageScale(error_icon, 450, 410, 256, 256);
		updateRenderer();

		sleep(10);
		appExit();
		return 0;
    }
    // muh loooooop
    while(appletMainLoop())
    {
        // scan for button / touch input each frame.
        hidScanInput();
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        hidTouchRead(&touch, tch);
        u32 touch_count = hidTouchCount();

        // main menu display
        printOptionList(cursor);

        // move cursor down...
        if (kDown & KEY_DOWN)
        {
            if (cursor == CURSOR_LIST_MAX) cursor = 0;
            else cursor++;
        }

        // move cursor up...
        if (kDown & KEY_UP)
        {
            if (cursor == 0) cursor = CURSOR_LIST_MAX;
            else cursor--;
        }

        // select option
        if (kDown & KEY_A || (touch_lock == OFF && touch.px > 530 && touch.px < 1200 && touch.py > FIRST_LINE - HIGHLIGHT_BOX_MIN && touch.py < (NEWLINE * CURSOR_LIST_MAX) + FIRST_LINE + HIGHLIGHT_BOX_MAX))
        {
            // check if the user used touch to enter this option.
            if (touch_lock == OFF && touch_count > 0)
                cursor = touch_cursor(touch.px, touch.py);

            switch (cursor)
            {
            case UP_LATEST:
				downloadFirmware(LATEST_URL, DL_OUTPUT, cursor);
                break;
            /*
            case UP_APP:
                if (yesNoBox(cursor, 390, 250, "Update App?") == YES)
                    update_app();
            */
            case UP_STABLE:
                downloadFirmware(STABLE_URL, DL_OUTPUT, cursor);
                break;
            }
        }

        // exit...
        if (kDown & KEY_PLUS || (touch.px > 1145 && touch.px < SCREEN_W && touch.py > 675 && touch.py < SCREEN_H))
            break;

        // lock touch if the user has already touched the screen (touch tap).
        if (touch_count > 0) touch_lock = ON;
        else touch_lock = OFF;

        // display render buffer
        updateRenderer();
    }

    // cleanup then exit
    appExit();
    return 0;
}
