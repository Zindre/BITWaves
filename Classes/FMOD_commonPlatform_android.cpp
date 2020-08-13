#include <string.h>
#include <jni.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <cstdlib>

#include "FMOD_commonPlatform.h"
#include "Functions.h"


std::vector<char *> gPathList;

const char *Common_MediaPath_Rec( unsigned int whatSoundObject ) {

    char *filePath = (char *)calloc(256, sizeof(char));

    strcat(filePath, "/data/data/com.stianremvik.bitwaves2/files/record");
    std::string str = to_string( whatSoundObject );
    char const *myChar = str.c_str();
    strcat(filePath, myChar );
    strcat(filePath, ".wav" );
    gPathList.push_back(filePath);

    return filePath;
}

const char *Common_MediaPath_Bounce() {
    char *filePath = (char *)calloc(256, sizeof(char));

    strcat(filePath, "/storage/self/primary/Documents/bounce.wav");
    gPathList.push_back(filePath);

    return filePath;
}