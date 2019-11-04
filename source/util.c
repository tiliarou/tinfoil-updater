#include <stdio.h>
#include <string.h>
#include <switch.h>

#include "util.h"
#include "menu.h"
#include "unzip.h"
#include "download.h"

#define TEMP_FILE               "/switch/tinfoil-updater/temp"

int updateTinfoil(char *url, char *output, int mode)
{
    if (!downloadFile(url, TEMP_FILE))
    {
        unzip(TEMP_FILE, mode);
        errorBox(400, 250, "      Update complete!");
    }
    return 1;
}

void update_app()
{
    // download new nro as a tempfile.
    if (!downloadFile(APP_URL, TEMP_FILE))
    {
        // remove current nro file.
        remove(APP_OUTPUT);
        // remove nro from /switch/.
        remove(OLD_APP_PATH);
        // rename the downloaded temp_file with the correct nro name.
        rename(TEMP_FILE, APP_OUTPUT);
        // using errorBox as a message window on this occasion. 
        errorBox(400, 250, "      Update complete!\nRestart app to take effect.");
    }
}
