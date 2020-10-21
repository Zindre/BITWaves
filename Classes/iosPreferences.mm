
#import <Foundation/Foundation.h>
#import "iosPreferences.h"

bool projectHandlingIsActivatedInIosPreferences() {
    return [[NSUserDefaults standardUserDefaults] boolForKey:@"preference_projectHandling"];
}

bool bounceAndShareIsActivatedInIosPreferences() {
    return [[NSUserDefaults standardUserDefaults] boolForKey:@"preference_bounceAndShare"];
}
