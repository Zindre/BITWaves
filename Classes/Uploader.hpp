//
//  Uploader.hpp
//  BITWaves
//
//  Created by Sindre Sørensen on 10/10/2020.
//
#include <stdio.h>
#include <string>

#ifndef Uploader_hpp
#define Uploader_hpp
#endif /* Uploader_hpp */

//@interface Uploader:NSObject {
//}
//-(void) upload_bounce_file:path destinationPath:destinationPath;
//@end
namespace Uploader {

//void c_upload_bounce_file(std::string &sourcePath, std::string &destinationPath);
void upload_bounce_file(std::string sourcePath, std::string destinationPath);

}
