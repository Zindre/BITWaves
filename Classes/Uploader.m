//
//  Uploader.m
//  BITWaves
//
//  Created by Sindre Sørensen on 14/10/2020.
//

#import <Foundation/Foundation.h>
#import <Uploader.hpp>





@implementation Uploader
- (NSString*)file_to_base64:(NSString*)path {
    NSData *fileData = [NSData dataWithContentsOfFile:path];
    return [fileData base64EncodedStringWithOptions:0];
}

//https://stackoverflow.com/questions/3566516/simple-http-post-example-in-objective-c
- (void)upload_bounce_file:(NSString*)sourcePath destinationPath:(NSString*)destinationPath {
    //NSLog(@"File contents: %@", [self file_to_base64:path]);
    NSString *fileDataBase64 = [self file_to_base64:sourcePath];
    NSURL *URL = [NSURL URLWithString:@"https://script.google.com/macros/s/AKfycbzajQuBAyi8T_xY--iqoP5Ns1nqhmmzd2RvtKgTy8rkGQ3xl1d7/exec?mimetype=audio/wav&filename=piano_objc.wav"];
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:URL];
    NSString *nsstringpayload = [NSString stringWithFormat:@"data=%@", fileDataBase64];
    NSData *nsdatacontent = [nsstringpayload dataUsingEncoding:NSUTF8StringEncoding];
    dispatch_semaphore_t sema = dispatch_semaphore_create(0);
    [request setHTTPMethod:@"POST"];
    NSURLSession *session = [NSURLSession sharedSession];
    NSLog(@"Content: %@", nsdatacontent);
    NSLog(@"Request: %@", request);
    NSURLSessionUploadTask *uploadTask = [session uploadTaskWithRequest:request
                                                               fromData:nsdatacontent
                                                      completionHandler:
                                          ^(NSData *data, NSURLResponse *response, NSError *error) {
        NSLog(@"Response: %@", response);
        dispatch_semaphore_signal(sema);

    }];
    [uploadTask resume];
    NSLog(@"Uploadtask: %@", uploadTask);

    if (![NSThread isMainThread]) {
        dispatch_semaphore_wait(sema, DISPATCH_TIME_FOREVER);
    } else {
        while (dispatch_semaphore_wait(sema, DISPATCH_TIME_NOW)) {
            [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode beforeDate:[NSDate dateWithTimeIntervalSinceNow:0]];
        }
    }
}
@end
