#ifndef _UTIL_H_
#define _UTIL_H_

#define ROOT                    "/"
#define APP_PATH                "/switch/tinfoil-updater/"
#define APP_OUTPUT              "/switch/tinfoil-updater/tinfoil-updater.nro"
#define OLD_APP_PATH            "/switch/tinfoil-updater.nro"
#define TINFOIL_OUTPUT          "/switch/tinfoil-updater/tinfoil.zip"
#define TEMP_FILE               "/switch/tinfoil-updater/temp"

#define YES                     10
#define NO                      20
#define ON                      1
#define OFF                     0

int updateTinfoil(char *url, char *output, int mode);                   // update tinfoil
void update_app();                                                      // update the app

#endif