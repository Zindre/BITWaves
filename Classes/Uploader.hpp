//
//  Uploader.hpp
//  BITWaves
//
//  Created by Sindre Sørensen on 10/10/2020.
//
#ifndef Uploader_hpp
#define Uploader_hpp
#endif /* Uploader_hpp */

#include "cocos2d.h"
#include "Functions.h"
#include "stdio.h"
#include "base64.h"
#include "network/HttpClient.h"
#include "tuple"
#include "BounceAndShare.hpp"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

class Uploader {
    

    
    public:
        Uploader();
        Uploader(BounceAndShare* bounceAndShare);
        void upload_bounce_file(std::string sourcePath, std::string destinationPath);
        void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
        BounceAndShare* bounceAndShare;
        static const char* URL_BASE;

    private:
        std::pair<char*, int> file_to_base64(std::string path);
        void notifyUser(char* message);

};
