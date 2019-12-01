#include <unistd.h>
#include <switch.h>

#include "menu.h"
#include "touch.h"
#include "util.h"
#include "download.h"

#define APP_VERSION "Firmware Downloader 0.1.0"

void refreshScreen()
{
    clearRenderer();

    // app version.
    drawText(fntMedium, 40, 40, SDL_GetColour(white), APP_VERSION);

    //drawText(fntMedium, 120, 225, SDL_GetColour(white), "Menu Here"); // menu options
    drawButton(fntButton, BUTTON_A, 970, 672, SDL_GetColour(white));
    drawText(fntSmall, 1010, 675, SDL_GetColour(white), "Select");
    drawButton(fntButton, BUTTON_PLUS, 1145, 672, SDL_GetColour(white));
    drawText(fntSmall, 1185, 675, SDL_GetColour(white), "Exit");
}

void printOptionList(int cursor)
{
    refreshScreen();

    char *option_list[]      = {    "Download latest Official Firmware files", \
                                    "Download legacy Official Firmware files" };

    char *description_list[] = {    "This app extracts OFW files to sd:/OFW/ for installation with ChoiDojourNX.", \
                                    "This app extracts OFW files to sd:/OFW/ for installation with ChoiDojourNX." };

    SDL_Texture *textureArray[] = { down_icon, app_icon };

    for (int i=0, nl=0; i < (CURSOR_LIST_MAX+1); i++, nl+=NEWLINE)
    {
        if (cursor != i) drawText(fntSmall, 550, FIRST_LINE+nl, SDL_GetColour(white), option_list[i]);
        else
        {
            // icon for the option selected.
            drawImage(textureArray[i], 125, 250);
            // highlight box.
            drawShape(SDL_GetColour(dark_blue), 530, (FIRST_LINE + nl - HIGHLIGHT_BOX_MIN), 700, HIGHLIGHT_BOX_MAX);
            // option text.
            drawText(fntSmall, 550, FIRST_LINE+nl, SDL_GetColour(jordy_blue), option_list[i]);
            // description.
			drawText(fntSmall, 15,605, SDL_GetColour(white), "Note that writing to an exFAT SD card can cause corruption. Be sure to use FAT32.");
            drawText(fntSmall, 15, 540, SDL_GetColour(white), description_list[i]);
        }
    }
}

void popUpBox(TTF_Font *font, int x, int y, SDL_Colour colour, char *text)
{
    // outline. box
    drawShape(SDL_GetColour(black), (SCREEN_W/4)-5, (SCREEN_H/4)-5, (SCREEN_W/2)+10, (SCREEN_H/2)+10);
    // popup box.
    drawShape(SDL_GetColour(dark_blue), SCREEN_W/4, SCREEN_H/4, SCREEN_W/2, SCREEN_H/2);
    // text to draw.
    drawText(font, x, y, colour, text);
}

int yesNoBox(int mode, int x, int y, char *question)
{
    printOptionList(mode);
    popUpBox(fntMedium, x, y, SDL_GetColour(white), question);
    // highlight box.
    drawShape(SDL_GetColour(faint_blue), 380, 410, 175, 65);
    drawShape(SDL_GetColour(faint_blue), 700, 410, 190, 65);
    // option text.
    drawButton(fntButtonBig, BUTTON_B, 410, 425, SDL_GetColour(white));
    drawText(fntMedium, 455, 425, SDL_GetColour(white), "No");
    drawButton(fntButtonBig, BUTTON_A, 725, 425, SDL_GetColour(white));
    drawText(fntMedium, 770, 425, SDL_GetColour(white), "Yes");

    updateRenderer();

    int res = 0;
    int touch_lock = OFF;
    touchPosition touch;
    u32 tch = 0;
    u32 touch_count = hidTouchCount();

    // check if the user is already touching the screen.
    if (touch_count > 0) touch_lock = ON;

    while (1)
    {
        hidScanInput();
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        hidTouchRead(&touch, tch);
        touch_count = hidTouchCount();

        if (touch_count == 0) touch_lock = OFF;

        if (touch_count > 0 && touch_lock == OFF)
            res = touch_yes_no_option(touch.px, touch.py);

        if (kDown & KEY_A || res == YES)
            return YES;

        if (kDown & KEY_B || res == NO)
            return NO;
    }
}

void errorBox(int x, int y, char *errorText)
{
    popUpBox(fntMedium, x, y, SDL_GetColour(white), errorText);
    drawImageScale(error_icon, 570, 340, 128, 128);
    updateRenderer();

    sleep(5);
}
