//
//  Uploader.hpp
//  BITWaves
//
//  Created by Sindre Sørensen on 10/10/2020.
//
#include "cocos2d.h"
#include "Functions.h"
#include "stdio.h"
#include "base64.h"
#include "network/HttpClient.h"
#include "tuple"


#ifndef Uploader_hpp
#define Uploader_hpp
#endif /* Uploader_hpp */

//@interface Uploader:NSObject {
//}
//-(void) upload_bounce_file:path destinationPath:destinationPath;
//@end
class Uploader {

    public:
        void upload_bounce_file(std::string sourcePath, std::string destinationPath);
        void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);

    private:
        std::tuple<char**, int> file_to_base64(std::string path);

};
