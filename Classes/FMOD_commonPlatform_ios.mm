#import <Foundation/Foundation.h>

#import "FMOD_commonPlatform.h"


const char *Common_MediaPath_Rec( unsigned int whatSoundObject ) {
    return [[NSString stringWithFormat:@"%@/record%d.wav", [NSSearchPathForDirectoriesInDomains( NSDocumentDirectory, NSUserDomainMask, YES ) objectAtIndex:0], whatSoundObject] UTF8String];
}

const char *Common_MediaPath_Bounce() {
    return [[NSString stringWithFormat:@"%@/bounce.wav", [NSSearchPathForDirectoriesInDomains( NSDocumentDirectory, NSUserDomainMask, YES ) objectAtIndex:0]] UTF8String];
}

