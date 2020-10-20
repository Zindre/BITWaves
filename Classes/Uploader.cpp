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


std::tuple<char**, int> Uploader::file_to_base64(std::string path) {
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
    char** out;
    int outlength = cocos2d::base64Encode(in, (int) size, out);
    return {out, outlength};
}

void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response) {
    log("Hei");
}

void Uploader::upload_bounce_file(std::string sourcePath, std::string destinationPath) {
    log("Saving destinationPath: %s", destinationPath.c_str());
    char** datab64;
    int datab64size;
    std::tie (datab64, datab64size) = Uploader::file_to_base64(sourcePath);
    char *stringpayload = new char[datab64size + 6];
    sprintf(stringpayload, "data=%s", *datab64);
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
    network::HttpClient::getInstance()->send(request);
 }
