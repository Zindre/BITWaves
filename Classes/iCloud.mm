#include "iCloud.hpp"


void saveToIcloud() {
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    NSString *path = [documentsDirectory stringByAppendingPathComponent:@"bounce.wav"];
    NSURL* sourceURL = [[NSURL alloc] initFileURLWithPath:path];

    NSFileManager *fileManager = [NSFileManager defaultManager];
    if ([fileManager fileExistsAtPath:path]) {
        NSLog(@"File found!");
    }
    else {
        NSLog(@"File not found!");
    }



    NSURL *ubiq = [[NSFileManager defaultManager] URLForUbiquityContainerIdentifier:nil];

    if (ubiq) {
        NSLog(@"iCloud access at %@", ubiq);
        NSError* error = nil;
        NSURL *destinationURL = [[ubiq URLByAppendingPathComponent:@"Documents"] URLByAppendingPathComponent:@"bounce.wav"];
        [[NSFileManager defaultManager] setUbiquitous:YES
                                            itemAtURL:sourceURL
                                       destinationURL:destinationURL
                                                error:&error];

        if(error != nil) {
            NSLog(@"Error while uploading the file: %@", error);
        } else {
            NSLog(@"File ulpoaded successfully");
        }
        
    } else {
        NSLog(@"No iCloud access");
    }

}

/// iCloud ///
/*- (void)queryDidReceiveNotification:(NSNotification *)notification {
    NSArray *results = [metadataQuery results];
    
    for(NSMetadataItem *item in results) {
        NSString *filename = [item valueForAttribute:NSMetadataItemDisplayNameKey];
        NSNumber *filesize = [item valueForAttribute:NSMetadataItemFSSizeKey];
        NSDate *updated = [item valueForAttribute:NSMetadataItemFSContentChangeDateKey];
        NSLog(@"%@ (%@ bytes, updated %@)", filename, filesize, updated);
    }
}*/
/// iCloud End ///


/// iCloud ///
/*NSFileManager* fm = [NSFileManager defaultManager];
id currentiCloudToken = fm.ubiquityIdentityToken;

if ( currentiCloudToken ) {
    NSData *newTokenData =
    [NSKeyedArchiver archivedDataWithRootObject: currentiCloudToken];
    [[NSUserDefaults standardUserDefaults]
     setObject: newTokenData
     forKey: @"com.apple.bitwaves2.UbiquityIdentityToken"];
} else {
    [[NSUserDefaults standardUserDefaults]
     removeObjectForKey: @"com.apple.bitwaves2.UbiquityIdentityToken"];
}

[[NSNotificationCenter defaultCenter]
    addObserver: self
    selector: @selector (iCloudAccountAvailabilityChanged:)
    name: NSUbiquityIdentityDidChangeNotification
    object: nil];

if ( currentiCloudToken ) {
    UIAlertView *alert = [[UIAlertView alloc]
                          initWithTitle: @"Choose Storage Option"
                          message: @"Should documents be stored in iCloud and available on all your devices?"
                          delegate: self
                          cancelButtonTitle: @"Local Only"
                          otherButtonTitles: @"Use iCloud", nil];
    [alert show];
}




NSMetadataQuery *metadataQuery = [[NSMetadataQuery alloc] init];
[metadataQuery setPredicate:[NSPredicate predicateWithFormat:@"%K LIKE '*'", NSMetadataItemFSNameKey]];
[[NSNotificationCenter defaultCenter] addObserver:self
                                         selector:@selector(queryDidReceiveNotification:)
                                             name:NSMetadataQueryDidUpdateNotification
                                           object:metadataQuery];

NSArray *results = [metadataQuery results];

for(NSMetadataItem *item in results) {
    NSString *filename = [item valueForAttribute:NSMetadataItemDisplayNameKey];
    NSNumber *filesize = [item valueForAttribute:NSMetadataItemFSSizeKey];
    NSDate *updated = [item valueForAttribute:NSMetadataItemFSContentChangeDateKey];
    NSLog(@"%@ (%@ bytes, updated %@)", filename, filesize, updated);
}

[metadataQuery startQuery];




dispatch_async (dispatch_get_global_queue (DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^(void) {
    
    ubiquitousURL = [[NSFileManager defaultManager] URLForUbiquityContainerIdentifier:nil];
    
    if ( ubiquitousURL != nil) {
        // Your app can write to the iCloud container
        NSLog( @"Your app can write to %@", ubiquitousURL );
        
        
        NSString *localFile = [NSString stringWithFormat:@"file://%@/bounce.wav", [NSSearchPathForDirectoriesInDomains( NSDocumentDirectory, NSUserDomainMask, YES ) objectAtIndex:0]];
        NSURL *localBounceURL = [NSURL URLWithString: localFile];
        NSLog( @"local file url: %@", localBounceURL );
        
        NSURL *destinationURL = [ubiquitousURL URLByAppendingPathComponent:@"Documents/bounce.wav"];
        NSError *err = nil;
        [fm setUbiquitous:YES itemAtURL:localBounceURL destinationURL:destinationURL error:&err];
        
        dispatch_async (dispatch_get_main_queue (), ^(void) {
            // On the main thread, update UI and state as appropriate
        });
    }
});*/
/// iCloud End ///
