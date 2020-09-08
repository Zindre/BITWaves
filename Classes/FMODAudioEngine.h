#pragma once

#include "fmod.hpp"
#include "cocos2d.h"
#include "Konstants.h"
#include "Functions.h"
#include "FMOD_commonPlatform.h"



using namespace cocos2d;


namespace FMODAudioEngine {
    
    void update();
    void playSound( unsigned int whatSoundObject );
    void setPitch( float pitch, int channelid );
    void recordStart( std::string currentProjectName, unsigned int whatSoundObject );
    void recordStop( unsigned int whatSoundObject );
    void shutDown();
    void createRecordSound( unsigned int whatSoundObject );
    void loadSoundFromDisk( std::string currentProjectName, unsigned int whatSoundObject );
    void writeWavHeader( FILE *fileBuffer, FMOD::Sound *sound, int length );
    void writeToDisk( unsigned int whatSoundObject );
    unsigned int getSoundLength( unsigned int whatSoundObject );
    unsigned int getChannelPos();
    bool isRecording();
    void stopSound();
    void fadeOut( int channelid, float fadeOutTime );
    int getChannelID();
    void START_outputToWaveWriter();
    void STOP_outputToWaveWriter();
    bool hasRecordWav(  unsigned int whatSoundObject );
    float getSpectrum( int channelID );
    bool channelisPlaying( int channelid );
    bool isBouncing();
    int numOfChannelsPlaying();
    unsigned int getRecPlayChannel();
    void delayPlayWhenRec();
    bool systemIsInitialized();
    
}
