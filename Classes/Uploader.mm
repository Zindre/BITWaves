//
//  Uploader.m
//  BITWaves
//
//  Created by Sindre Sørensen on 14/10/2020.
//

// The bounce file is uploaded to a Google Apps Script, which looks like this:

//
//function doPost(e) {
//  var folder, folders = DriveApp.getFoldersByName("BITWaves");
//  console.log(folders);
//  if (folders.hasNext()) {
//    folder = folders.next();
//  } else {
//    folder = DriveApp.createFolder("BITWaves");
//  }
//  var data = Utilities.base64Decode(e.postData.contents.split("data=")[1]);
//  var blob = Utilities.newBlob(data, e.parameter.mimetype, e.parameter.filename);
//  Logger.log(blob);
//  var id = "Your File Download Link: https://drive.google.com/uc?id="+folder.createFile(blob).getId();
//  return ContentService.createTextOutput(id);
//}
//

#import <Foundation/Foundation.h>
#import <Uploader.hpp>


#if defined __cplusplus
#include <string>

//void c_upload_bounce_file(std::string sourcePath, std::string destinationPath) {
//    return upload_bounce_file(sourcePath, destinationPath);
//}


#endif /* __cplusplus */


NSString* file_to_base64(NSString* path) {
    NSData *fileData = [NSData dataWithContentsOfFile:path];
    return [fileData base64EncodedStringWithOptions:0];
}

//https://stackoverflow.com/questions/3566516/simple-http-post-example-in-objective-c
void Uploader::upload_bounce_file(std::string sourcePath, std::string destinationPath) {
    NSString *sourcePathNSString = [NSString stringWithCString:sourcePath.c_str() encoding:[NSString defaultCStringEncoding]];
    NSString *destinationPathNSString = [NSString stringWithCString:destinationPath.c_str() encoding:[NSString defaultCStringEncoding]];
    //NSLog(@"File contents: %@", [self file_to_base64:path]);
    NSString *fileDataBase64 = file_to_base64(sourcePathNSString);
    NSLog(@"Saving destinationPath: %@", destinationPathNSString);
    NSURL *URL = [NSURL URLWithString:[NSString stringWithFormat:@"https://script.google.com/macros/s/AKfycbzajQuBAyi8T_xY--iqoP5Ns1nqhmmzd2RvtKgTy8rkGQ3xl1d7/exec?mimetype=audio/wav&filename=%@.wav", destinationPathNSString]];
    //NSLog(@"URL: %@", URL);
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:URL];
    NSString *nsstringpayload = [NSString stringWithFormat:@"data=%@", fileDataBase64];
    NSData *nsdatacontent = [nsstringpayload dataUsingEncoding:NSUTF8StringEncoding];
    dispatch_semaphore_t sema = dispatch_semaphore_create(0);
    [request setHTTPMethod:@"POST"];
    NSURLSession *session = [NSURLSession sharedSession];
    //NSLog(@"Content: %@", nsdatacontent);
    //NSLog(@"Request: %@", request);
    NSURLSessionUploadTask *uploadTask = [session uploadTaskWithRequest:request
                                                               fromData:nsdatacontent
                                                      completionHandler:
                                          ^(NSData *data, NSURLResponse *response, NSError *error) {
        //NSLog(@"Response: %@", response);
        dispatch_semaphore_signal(sema);
        if (error) {NSLog(@"Error: %@", error);}

    }];
    [uploadTask resume];
    //NSLog(@"Uploadtask: %@", uploadTask);
    if (![NSThread isMainThread]) {
        dispatch_semaphore_wait(sema, DISPATCH_TIME_FOREVER);
    } else {
        while (dispatch_semaphore_wait(sema, DISPATCH_TIME_NOW)) {
            [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode beforeDate:[NSDate dateWithTimeIntervalSinceNow:0]];
        }
    }
}

