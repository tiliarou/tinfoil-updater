#ifndef _DOWNLOAD_H_
#define _DOWNLOAD_H_

#define LATEST_URL         "https://www.underprivlidged.xyz/packs/OFWlatest.zip" // currently 9.0.1 as of November 29, 2019
#define STABLE_URL         "https://www.underprivlidged.xyz/packs/OFWstable.zip" // currently 8.1.0 as of November 29, 2019

int downloadFile(const char *url, const char *output);

#endif
