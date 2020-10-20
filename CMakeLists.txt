#/****************************************************************************
# Copyright (c) 2013-2014 cocos2d-x.org
# Copyright (c) 2015-2017 Chukong Technologies Inc.
#
# http://www.cocos2d-x.org
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
# ****************************************************************************/

cmake_minimum_required(VERSION 3.6)

set(APP_NAME BITWaves)

project(${APP_NAME})

if(XCODE)
    if(NOT DEFINED CMAKE_XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET)
        SET (CMAKE_XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET 8.0)
    endif()
endif()

if(NOT DEFINED BUILD_ENGINE_DONE) # to test BITWaves into root project
    set(COCOS2DX_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cocos2d)
    set(CMAKE_MODULE_PATH ${COCOS2DX_ROOT_PATH}/cmake/Modules/)

    include(CocosBuildSet)
    add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos ${ENGINE_BINARY_PATH}/cocos/core)
endif()

# record sources, headers, resources...
set(GAME_SOURCE)
set(GAME_HEADER)

set(GAME_RES_FOLDER
    "${CMAKE_CURRENT_SOURCE_DIR}/Resources"
    )
if(APPLE OR WINDOWS)
    cocos_mark_multi_resources(common_res_files RES_TO "Resources" FOLDERS ${GAME_RES_FOLDER})
endif()

# add cross-platforms source files and header files 
if(ANDROID)
list(APPEND GAME_SOURCE
        Classes/AppDelegate.cpp
        Classes/Circle.cpp
        Classes/Functions.cpp
        Classes/InstrumentScene.cpp
        Classes/MainMenu.cpp
        Classes/SeqSoundRect.cpp
        Classes/SequencerScene.cpp
        Classes/FMOD_commonPlatform_android.cpp
        Classes/FMODAudioEngine.cpp
        Classes/CircleEmitter.cpp
        Classes/HelpOverlay.cpp

     )
list(APPEND GAME_HEADER
        Classes/AppDelegate.h
        Classes/Circle.hpp
        Classes/Functions.h
        Classes/InstrumentScene.h
        Classes/MainMenu.hpp
        Classes/SeqSoundRect.hpp
        Classes/SequencerScene.h
        Classes/FMOD_commonPlatform.h
        Classes/FMODAudioEngine.h
        Classes/CircleEmitter.hpp
        Classes/HelpOverlay.h
        Classes/Konstants.h
     )
endif()

if(IOS)
list(APPEND GAME_SOURCE
        Classes/AppDelegate.cpp
        Classes/Circle.cpp
        Classes/Functions.cpp
        Classes/InstrumentScene.cpp
        Classes/MainMenu.cpp
        Classes/SeqSoundRect.cpp
        Classes/SequencerScene.cpp
        Classes/FMOD_commonPlatform_ios.mm
        Classes/FMODAudioEngine.cpp
        Classes/CircleEmitter.cpp
        Classes/HelpOverlay.cpp
        Classes/BounceAndShare.cpp
        Classes/ProjectHandling.cpp
        Classes/ProjectNamesLabel.cpp
        Classes/Uploader.cpp
)
list(APPEND GAME_HEADER
        Classes/AppDelegate.h
        Classes/Circle.hpp
        Classes/Functions.h
        Classes/InstrumentScene.h
        Classes/MainMenu.hpp
        Classes/SeqSoundRect.hpp
        Classes/SequencerScene.h
        Classes/FMOD_commonPlatform.h
        Classes/FMODAudioEngine.h
        Classes/CircleEmitter.hpp
        Classes/HelpOverlay.h
        Classes/Konstants.h
        Classes/BounceAndShare.hpp
        Classes/ProjectHandling.hpp
        Classes/ProjectNamesLabel.hpp
        Classes/Uploader.hpp
     )
endif()

if(ANDROID)
    # change APP_NAME to the share library name for Android, it's value depend on AndroidManifest.xml
    set(APP_NAME MyGame)
    list(APPEND GAME_SOURCE
         proj.android/app/jni/hellocpp/main.cpp
         )
elseif(LINUX)
    list(APPEND GAME_SOURCE
         proj.linux/main.cpp
         )
elseif(WINDOWS)
    list(APPEND GAME_HEADER
         proj.win32/main.h
         proj.win32/resource.h
         )
    list(APPEND GAME_SOURCE
         proj.win32/main.cpp
         proj.win32/game.rc
         ${common_res_files}
         )
elseif(APPLE)
    if(IOS)
        list(APPEND GAME_HEADER
             proj.ios_mac/ios/AppController.h
             proj.ios_mac/ios/RootViewController.h
             )
        set(APP_UI_RES
            proj.ios_mac/ios/LaunchScreen.storyboard
            proj.ios_mac/ios/LaunchScreenBackground.png
            proj.ios_mac/ios/Images.xcassets
            )
        list(APPEND GAME_SOURCE
             proj.ios_mac/ios/main.m
             proj.ios_mac/ios/AppController.mm
             proj.ios_mac/ios/RootViewController.mm
             proj.ios_mac/ios/Prefix.pch
             ${APP_UI_RES}
             )
    elseif(MACOSX)
        set(APP_UI_RES
            proj.ios_mac/mac/Icon.icns
            proj.ios_mac/mac/Info.plist
            )
        list(APPEND GAME_SOURCE
             proj.ios_mac/mac/main.cpp
             proj.ios_mac/mac/Prefix.pch
             ${APP_UI_RES}
             )
    endif()
    list(APPEND GAME_SOURCE ${common_res_files})
endif()

# mark app complie info and libs info
set(all_code_files
    ${GAME_HEADER}
    ${GAME_SOURCE}
    )
if(NOT ANDROID)
    add_executable(${APP_NAME} ${all_code_files})
else()
    add_library(${APP_NAME} SHARED ${all_code_files})
    add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos/platform/android ${ENGINE_BINARY_PATH}/cocos/platform)
    target_link_libraries(${APP_NAME} -Wl,--whole-archive cpp_android_spec -Wl,--no-whole-archive)

    add_library(fmodLib1 SHARED IMPORTED)
    set_property(TARGET fmodLib1 PROPERTY IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/fmodLib/android/lowlevel/${ANDROID_ABI}/libfmod.so")
    target_link_libraries(${APP_NAME} fmodLib1)

    add_library(fmodLib2 SHARED IMPORTED)
    set_property(TARGET fmodLib2 PROPERTY IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/fmodLib/android/lowlevel/${ANDROID_ABI}/libfmodL.so")
    target_link_libraries(${APP_NAME} fmodLib2)

    add_library(fmodLib3 SHARED IMPORTED)
    set_property(TARGET fmodLib3 PROPERTY IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/fmodLib/android/studio/${ANDROID_ABI}/libfmodstudio.so")
    target_link_libraries(${APP_NAME} fmodLib3)

    add_library(fmodLib4 SHARED IMPORTED)
    set_property(TARGET fmodLib4 PROPERTY IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/fmodLib/android/studio/${ANDROID_ABI}/libfmodstudioL.so")
    target_link_libraries(${APP_NAME} fmodLib4)

endif()

target_link_libraries(${APP_NAME} cocos2d)
target_include_directories(${APP_NAME}
        PRIVATE Classes
        PRIVATE ${COCOS2DX_ROOT_PATH}/cocos/audio/include/

        # FMOD
        if(ANDROID)
            PRIVATE fmodClasses/android/lowlevel
            PRIVATE fmodClasses/android/studio
        endif()
        
)

# mark app resources
setup_cocos_app_config(${APP_NAME})
if(APPLE)
    set_target_properties(${APP_NAME} PROPERTIES RESOURCE "${APP_UI_RES}")

    if(MACOSX)
        set_xcode_property(${APP_NAME} INFOPLIST_FILE "${CMAKE_CURRENT_SOURCE_DIR}/proj.ios_mac/mac/Info.plist")
    elseif(IOS)
        set_xcode_property(${APP_NAME} INFOPLIST_FILE "${CMAKE_CURRENT_SOURCE_DIR}/proj.ios_mac/ios/Info.plist")
        set_xcode_property(${APP_NAME} ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon")
    endif()

    # For code-signing, set the DEVELOPMENT_TEAM:
    #set_xcode_property(${APP_NAME} DEVELOPMENT_TEAM "GRLXXXX2K9")
elseif(WINDOWS)
    cocos_copy_target_dll(${APP_NAME})
endif()

if(LINUX OR WINDOWS)
    cocos_get_resource_path(APP_RES_DIR ${APP_NAME})
    cocos_copy_target_res(${APP_NAME} LINK_TO ${APP_RES_DIR} FOLDERS ${GAME_RES_FOLDER})
endif()

#add_subdirectory(NFHTTP)
#target_link_libraries(${APP_NAME} NFHTTP)

add_library(fmodLib SHARED IMPORTED)
set_property(TARGET fmodLib PROPERTY IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/fmodLib/ios/release/libfmod_iphoneos.a")
target_link_libraries(${APP_NAME} fmodLib)
