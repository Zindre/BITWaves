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

// This uploads to script.google.com, something like this:

//function doPost(e) {
//
//  var folder, folders = DriveApp.getFoldersByName("BITWaves");
//  Logger = BetterLog.useSpreadsheet('1NNU8RtoXNy35WjHUKRLafeWGP17TYRX5GwPHqNtjWlM');
//  Logger.log(folders);
//  Logger.log(e);
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

// And new json version:

//function doPost(e) {
//
//  var folder, folders = DriveApp.getFoldersByName("BITWaves");
//  Logger = BetterLog.useSpreadsheet('1NNU8RtoXNy35WjHUKRLafeWGP17TYRX5GwPHqNtjWlM');
//  Logger.log(folders);
//  Logger.log(e);
//  if (folders.hasNext()) {
//    folder = folders.next();
//  } else {
//    folder = DriveApp.createFolder("BITWaves");
//  }
//  var json = JSON.parse(e.postData.contents);
//  //Logger.log(e.postData.contents);
//  Logger.log(json);
//  var fileData = Utilities.base64Decode(json['data']);
//  var blob = Utilities.newBlob(fileData, "audio/wav", json['filename']);
//  Logger.log(blob);
//  var id = "Your File Download Link: https://drive.google.com/uc?id="+folder.createFile(blob).getId();
//  return ContentService.createTextOutput(id);
//}

Uploader::Uploader(){
    bounceAndShare = nullptr;
}

Uploader::Uploader(BounceAndShare* bas) {
    //* @param bas A client to receive notifications */
    bounceAndShare = bas;
}

std::tuple<char*, int> Uploader::file_to_base64(std::string path) {
    /** Converts a file to base64
     * @param path the path to a binary file
     * @return a tuple, a char*, a pointer to the output base64 string and an int, the length of the output string
     */
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
    /** Notifies the user about upload status
     * @param message a char* containing the message */
    if (bounceAndShare) {
        bounceAndShare->showPrompt(message);
    }
    else {
        log("Upload status: %s", message);
    }
}

void Uploader::onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response) {
    /** Callback called after upload is done */

    char message[50];
    if (!response)
    {
        sprintf(message, "Feil: Ingen respons ved forsøk på opplasting");
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
    /**
     * Uploads a file to a web service, currently Google Apps
     */
    char message[256] = "Laster opp fil til BIT20.";
    notifyUser(message);
    char* datab64;
    unsigned int datab64size;
    std::tie (datab64, datab64size) = Uploader::file_to_base64(sourcePath);
    const char *url_base = "https://script.google.com/macros/s/AKfycbzajQuBAyi8T_xY--iqoP5Ns1nqhmmzd2RvtKgTy8rkGQ3xl1d7/exec";
    rapidjson::Document jsonDoc;
    jsonDoc.SetObject();
    Value o(rapidjson::kObjectType);
    rapidjson::Value filename;
    filename.SetString(destinationPath.c_str(), jsonDoc.GetAllocator());
    jsonDoc.AddMember("filename", filename, jsonDoc.GetAllocator());
    rapidjson::Value data;
    data.SetString(datab64, jsonDoc.GetAllocator());
    jsonDoc.AddMember("data", data, jsonDoc.GetAllocator());
    rapidjson::StringBuffer sb;
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
    jsonDoc.Accept(writer);
    std::string jsonString = sb.GetString();
    network::HttpRequest* request = new (std :: nothrow) network::HttpRequest();
    log("Payload: %s", jsonString.c_str());
    request->setRequestType(network::HttpRequest::Type::POST);
    request->setUrl(url_base);
    request->setRequestData(jsonString.c_str(), jsonString.length());
    request->setResponseCallback(CC_CALLBACK_2(Uploader::onHttpRequestCompleted, this));
    network::HttpClient::getInstance()->send(request);
 }
