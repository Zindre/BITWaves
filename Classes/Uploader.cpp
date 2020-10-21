//
//  Uploader.cpp
//  BITWaves
//
//  Created by Sindre Sørensen on 20/10/2020.
//

#include "Uploader.hpp"
//#include "stdio.h"
USING_NS_CC;
//using namespace network;


Uploader::Uploader(){
    bounceAndShare = nullptr;
}

Uploader::Uploader(BounceAndShare* bas) {
    bounceAndShare = bas;
}

std::tuple<char*, int> Uploader::file_to_base64(std::string path) {
    FILE * file = std::fopen(path.c_str(), "r+");
    if (file == NULL) return;
    fseek(file, 0, SEEK_END);
    long int size = ftell(file);
    fclose(file);
    // Reading data to array of unsigned chars
    file = std::fopen(path.c_str(), "r+");
    unsigned char * in = (unsigned char *) malloc(size);
    long int bytes_read = fread(in, sizeof(unsigned char), size, file);
    fclose(file);
    char* out;
    unsigned int outlength = cocos2d::base64Encode(in, (int) size, &out);
    return {out, outlength};
}

void Uploader::notifyUser(char message[]) {
    
    if (bounceAndShare) {
        bounceAndShare->showPrompt(message);
    }
    else {
        log("Upload status: %s", message);
    }
}

void Uploader::onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response) {
    char message[50];
    if (!response)
    {
        sprintf(message, "Feil: Ingen respons ved forsøk på opplasting.");
        notifyUser(message);
        return;
    }
    long responseCode = response->getResponseCode();
    if (responseCode == 200 or responseCode == 302  ) {
        sprintf(message, "Lydklippet er lastet opp");
    }
    else {
        sprintf(message,"Feil ved opplasting.");
        log("Feil ved opplasting, kode %lu", response->getResponseCode());
    }
    notifyUser(message);
    
}

void Uploader::upload_bounce_file(std::string sourcePath, std::string destinationPath) {
    char message[256] = "Laster opp fil til BIT20.";
    notifyUser(message);
    char* datab64;
    unsigned int datab64size;
    std::tie (datab64, datab64size) = Uploader::file_to_base64(sourcePath);
    char *stringpayload = new char[datab64size + 6];
    sprintf(stringpayload, "data=%s", datab64);
    
    const char *url_base = "https://script.google.com/macros/s/AKfycbzajQuBAyi8T_xY--iqoP5Ns1nqhmmzd2RvtKgTy8rkGQ3xl1d7/exec?mimetype=audio/wav&filename=";
    size_t filenameLen = std::strlen(destinationPath.c_str());
    char *url_filenameParam = new char[filenameLen + 1];
    std::strcpy(url_filenameParam, destinationPath.c_str());
    const char *url_suffix= ".wav";
    size_t urllen = strlen(url_base) + strlen(url_filenameParam) + strlen(url_suffix) + 1;
    char *URL = new char[urllen];
    std::strcpy(URL, url_base);
    std::strcat(URL, url_filenameParam);
    std::strcat(URL, url_suffix);
    //log("URL: %s", URL);
    network::HttpRequest* request = new (std :: nothrow) network::HttpRequest();
    //log("Payload: %s", stringpayload);
    request->setRequestType(network::HttpRequest::Type::POST);
    request->setUrl(URL);
    request->setRequestData(stringpayload, datab64size);
    // This setter crashes the main thread when ran:
    //request->setResponseCallback([] (network::HttpClient *sender, network::HttpResponse *response) {
    //    log("Upload completed");
    //});
    request->setResponseCallback(CC_CALLBACK_2(Uploader::onHttpRequestCompleted, this));
    network::HttpClient::getInstance()->send(request);
 }
