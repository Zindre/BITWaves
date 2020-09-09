#include "FMODAudioEngine.h"
#include <assert.h>

#define ERRCHECK(result) assert(result == FMOD_OK)
#define DEVICE_INDEX (0)

#if defined(WIN32) || defined(__WATCOMC__) || defined(_WIN32) || defined(__WIN32__)
#define __PACKED                         /* dummy */
#else
#define __PACKED __attribute__((packed)) /* gcc packed */
#endif

#define LATENCY_MS      (50) /* Some devices will require higher latency to avoid glitches */

namespace FMODAudioEngine {
    
    FMOD::System *system = 0;
    FMOD::Sound *sound[kNumOfSoundObjects] = {0};
    FMOD::Channel *channel = 0;
    FMOD::ChannelGroup *recPlayChannelGroup = 0;
    FMOD::DSP *dsp = 0;
    FMOD::DSP *fftDsp[kNumOfChannels] = {0};

    
    FMOD_CREATESOUNDEXINFO exinfo;
    unsigned int datalength = 0;
    unsigned int soundlength = 0;
    bool bIsRecording = false;
    FILE *fileBuffer;
    unsigned int whatSoundObjectToWrite = 0;
    bool bHasRecordWav[kNumOfSoundObjects] = {0};
    bool bIsBouncing = false;
    unsigned int recPlayChannel = 0;
    unsigned int samplesRecorded = 0;
    unsigned int desiredLatency = 0;
    unsigned int adjustedLatency = 0;
    unsigned int recordPos = 0;
    static unsigned int lastRecordPos = 0;
    unsigned int recordDelta = 0;
    float spectrumValue[kNumOfChannels] = {0};
    bool isPlayingWhileRec = false;
    

    FMOD::System *getLowLevelSystem() {
    
        if ( system == 0 ) {
            
            FMOD_RESULT result = FMOD::System_Create( &system );
            ERRCHECK( result );

            if ( CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID ) {
                result = system->setOutput( FMOD_OUTPUTTYPE_OPENSL );
                ERRCHECK( result );
            }
            
            
            result = system->init( kNumOfChannels, FMOD_INIT_NORMAL, NULL );
            ERRCHECK(result);
            
            log( "FMOD init" );
            
            
            int numDrivers = 0;
            result = system->getRecordNumDrivers( NULL, &numDrivers );
            ERRCHECK(result);
            if ( numDrivers == 0 ) {
                log( "No recording devices found/plugged in!" );
            } else {
                log( "Num of recording devices: %d", numDrivers );
            }
            
            
            // Moved out to instument and sequencer classes
            /*for ( int i = 0; i < kNumOfSoundObjects; i++ ) {
                loadSoundFromDisk( i );
            }*/
        
            //result = FMOD::Debug_Initialize( FMOD_DEBUG_LEVEL_LOG );
            //ERRCHECK( result );
            
            result = system->createChannelGroup( "Rec play channel group", &recPlayChannelGroup );
            ERRCHECK( result );
            
            for ( int i = 0; i < kNumOfChannels; i++ ) {
                result = system->createDSPByType( FMOD_DSP_TYPE_FFT, &fftDsp[i] );
                ERRCHECK( result );
            }

            
        
            
        }

        return system;
    }

        
    void update() {
        
        FMOD::System *system = getLowLevelSystem();
        FMOD_RESULT result = system->update();
        ERRCHECK( result );
        
        writeToDisk( whatSoundObjectToWrite );

        if ( isRecording() ) {
            FMODAudioEngine::delayPlayWhenRec();
        }
    }

    void playSound( unsigned int whatSoundObject ) {
        
        if ( bHasRecordWav[whatSoundObject] ) {
            FMOD_RESULT result;
            
            bool paused = false;
            result = system->playSound( sound[whatSoundObject], 0, paused, &channel );
            ERRCHECK( result );
    
        }
    }
    
    bool channelisPlaying( int channelid ) {
        FMOD_RESULT result;
        
        result = system->getChannel( channelid, &channel );
        ERRCHECK( result );
        
        bool bIsPlaying = false;
        result = channel->isPlaying( &bIsPlaying );
        if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN)) {
            ERRCHECK(result);
        }
        
        if ( !bIsPlaying ) {
            return false;
        } else {
            return true;
        }
    }
    
    int getChannelID() {
        bool bIsPlaying = false;
        FMOD_RESULT result = channel->isPlaying( &bIsPlaying );
        if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN)) {
            ERRCHECK(result);
        }
        if ( !bIsPlaying ) {
            return 0;
        }
        
        int index;
        result = channel->getIndex( &index );
        ERRCHECK( result );
        return index;
    }
    
    void setPitch( float pitch, int channelid ) {
        
        FMOD_RESULT result = system->getChannel( channelid, &channel );
        ERRCHECK( result );
        
        bool bIsPlaying = false;
        result = channel->isPlaying( &bIsPlaying );
        if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN)) {
            ERRCHECK(result);
        }
        if ( !bIsPlaying ) {
            return;
        }

        //log( "channel: %d - pitch: %f", channelid, pitch );
        
        result = channel->setPitch( pitch );
        ERRCHECK( result );

    }
    
    void fadeOut( int channelid, float fadeOutTime ) {
        
        FMOD_RESULT result;
        
        result = system->getChannel( channelid, &channel );
        ERRCHECK( result );
        
        bool isPlaying = false;
        result = channel->isPlaying( &isPlaying );
        if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN)) {
            ERRCHECK(result);
        }
        if ( !isPlaying ) {
            return;
        }
        
        unsigned long long dspclock = 0;
        int rate = 0;
        
        result = system->getSoftwareFormat( &rate, 0, 0 );
        ERRCHECK( result );
        
        result = channel->getDSP( FMOD_CHANNELCONTROL_DSP_TAIL, &dsp );
        ERRCHECK( result );
    
        result = channel->getDSPClock( 0, &dspclock );
        ERRCHECK( result );
        result = channel->addFadePoint( dspclock, 1.0f );
        ERRCHECK( result );
        result = channel->addFadePoint( dspclock + (rate * fadeOutTime), 0.0f );
        ERRCHECK( result );
        bool stop = true;
        result = channel->setDelay( 0, dspclock + (rate * fadeOutTime), stop );
        ERRCHECK( result );
        
    }
    
    void stopSound() {
        
        for( int i = 0; i < kNumOfChannels; i++ ) {
            FMOD_RESULT result = system->getChannel( i, &channel );
            
            if( result == FMOD_OK && channel ) {
                channel->stop();
            }
        }
    }
    
    unsigned int getSoundLength( unsigned int whatSoundObject ) {
        
        if ( bHasRecordWav[whatSoundObject] ) {
            unsigned int length;
            FMOD_RESULT result = sound[whatSoundObject]->getLength( &length, FMOD_TIMEUNIT_MS );
            ERRCHECK( result );
            return length;
        } else {
            return 0;
        }
    }
    
    unsigned int getChannelPos() {
        
        bool bIsPlaying = false;
        FMOD_RESULT result = channel->isPlaying( &bIsPlaying );
        if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN)) {
            ERRCHECK(result);
        }
        if ( !bIsPlaying ) {
            return 0;
        }
        
        unsigned int pos = 0;
        result = channel->getPosition( &pos, FMOD_TIMEUNIT_MS );
        ERRCHECK( result );
        
        return pos;
    }
    
    void createRecordSound( unsigned int whatSoundObject ) {
        
        int nativeRate = 0;
        int nativeChannels = 0;
        FMOD_RESULT result = system->getRecordDriverInfo( DEVICE_INDEX, NULL, 0, NULL, &nativeRate, NULL, &nativeChannels, NULL );
        ERRCHECK(result);
        
        log( "Native sampling rate: %d", nativeRate );
        
        exinfo                  = {0};
        exinfo.cbsize           = sizeof(FMOD_CREATESOUNDEXINFO);
        exinfo.numchannels      = nativeChannels;
        exinfo.format           = FMOD_SOUND_FORMAT_PCM16;
        exinfo.defaultfrequency = nativeRate;
        exinfo.length           = exinfo.defaultfrequency * sizeof(short) * exinfo.numchannels * kRecordMaxLengthSeconds;
        
        desiredLatency = (nativeRate * LATENCY_MS) / 1000;
        adjustedLatency = desiredLatency;
        
        result = system->createSound( 0, FMOD_DEFAULT | FMOD_LOOP_OFF | FMOD_OPENUSER, &exinfo, &sound[whatSoundObject] );
        ERRCHECK( result );
        
    }

    void loadSoundFromDisk( std::string currentProjectName, unsigned int whatSoundObject ) {
        
        //std::string recFile = Common_MediaPath_Rec( whatSoundObject );
        
        FileUtils *fileUtils = FileUtils::getInstance();
        std::string writablePath = fileUtils->getWritablePath();
        std::string recFile = "record" + to_string( whatSoundObject ) + ".wav";
        std::string projectFolder = currentProjectName;
        std::string recFileFullPath = writablePath + projectFolder + "/" + recFile;
        log( "load sound from disk - rec file full path: %s", recFileFullPath.c_str() );
        
        
        if ( ! fileUtils->isFileExist( recFileFullPath ) ) {
            
            bHasRecordWav[whatSoundObject] = false;
            log( "record%d.wav has content: %d", whatSoundObject, bHasRecordWav[whatSoundObject] );

        } else {
            log( "found record%d.wav", whatSoundObject );
            const char *cString = recFileFullPath.c_str();
            
            FMOD_RESULT result = system->createSound( cString, FMOD_DEFAULT | FMOD_LOOP_OFF, 0, &sound[whatSoundObject] );
            ERRCHECK( result );
            
            bHasRecordWav[whatSoundObject] = true;
            log( "record%d.wav has content: %d", whatSoundObject, bHasRecordWav[whatSoundObject] );
        }

    }
    
    void recordStart( std::string currentProjectName, unsigned int whatSoundObject ) {
        
        datalength = 0;
        soundlength = 0;
        samplesRecorded = 0;
        desiredLatency = 0;
        adjustedLatency = 0;
        recordPos = 0;
        lastRecordPos = 0;
        recordDelta = 0;
        
        whatSoundObjectToWrite = whatSoundObject;

        createRecordSound( whatSoundObject );
        
        FMOD_RESULT result = system->recordStart( DEVICE_INDEX, sound[whatSoundObject], false );
        ERRCHECK( result );
        
        bIsRecording = true;
        
        
        
        //std::string recFile = Common_MediaPath_Rec( whatSoundObject );
        
        FileUtils *fileUtils = FileUtils::getInstance();
        std::string writablePath = fileUtils->getWritablePath();
        std::string projectFolder = currentProjectName;
        std::string recFile = "record" + to_string( whatSoundObject ) + ".wav";
        std::string recFileFullPath = writablePath + projectFolder + "/" + recFile;

        log( "record start - rec file full path: %s", recFileFullPath.c_str() );

        fileBuffer = fopen( recFileFullPath.c_str(), "wb" );
        if ( ! fileBuffer ) {
            printf( "ERROR : could not open record.wav for writing.\n" );
        }
     
        // Write out the wav header.  As we don't know the length yet it will be 0.
        writeWavHeader( fileBuffer, sound[whatSoundObject], datalength );
        
        result = sound[whatSoundObject]->getLength( &soundlength, FMOD_TIMEUNIT_PCM );
        ERRCHECK( result );
        
    }
    
    void delayPlayWhenRec() {
        
        FMOD_RESULT result = system->getRecordPosition( DEVICE_INDEX, &recordPos );
        if ( result != FMOD_ERR_RECORD_DISCONNECTED ) {
            ERRCHECK( result );
        }
        
        recordDelta = (recordPos >= lastRecordPos) ? (recordPos - lastRecordPos) : (recordPos + soundlength - lastRecordPos);
        lastRecordPos = recordPos;
        samplesRecorded += recordDelta;
        
        static unsigned int minRecordDelta = (unsigned int)-1;
        if ( recordDelta && (recordDelta < minRecordDelta ) ) {
            minRecordDelta = recordDelta; /* Smallest driver granularity seen so far */
            adjustedLatency = (recordDelta <= desiredLatency) ? desiredLatency : recordDelta; /* Adjust our latency if driver granularity is high */
        }
        
        /*
         Delay playback until our desired latency is reached.
         */
        if ( ! isPlayingWhileRec && samplesRecorded >= adjustedLatency) {
            log( "play" );
            result = system->playSound( sound[whatSoundObjectToWrite], 0, false, &channel );
            ERRCHECK( result );
            
            int index;
            result = channel->getIndex( &index );
            ERRCHECK( result );
            recPlayChannel = index;
            
            result = channel->setChannelGroup( recPlayChannelGroup );
            ERRCHECK( result );
            
            result = recPlayChannelGroup->setVolume( 0.0f );
            ERRCHECK( result );
            
            isPlayingWhileRec = true;
        }
    }

    void recordStop( unsigned int whatSoundObject ) {
        
        FMOD_RESULT result = system->recordStop( DEVICE_INDEX );
        ERRCHECK( result );
        
     
        // Write back the wav header now that we know its length.
        writeWavHeader( fileBuffer, sound[whatSoundObject], datalength );
        
        fclose( fileBuffer );
        
        bIsRecording = false;
        isPlayingWhileRec = false;
        
        // Moved out to instument and sequencer classes
        //loadSoundFromDisk( whatSoundObject );

    }

    
    float getSpectrum( int channelID ) {
        
        FMOD_RESULT result;
        
        result = system->getChannel( channelID, &channel );
        ERRCHECK( result );
        
        bool isPlaying;
        result = channel->isPlaying( &isPlaying );
        if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN)) {
            ERRCHECK(result);
        }
        if ( !isPlaying ) {
            return 0.0f;
        }
        
        result = channel->addDSP( FMOD_CHANNELCONTROL_DSP_TAIL, fftDsp[channelID] );
        ERRCHECK( result );
        

        result = fftDsp[channelID]->getParameterFloat( FMOD_DSP_FFT_DOMINANT_FREQ, &spectrumValue[channelID], 0, 0 );
        ERRCHECK( result );
        
        return spectrumValue[channelID];
        
    }

    void shutDown() {
        
        FMOD_RESULT result;
        for ( int i = 0; i < kNumOfSoundObjects; i++ ) {
            result = sound[i]->release();
            ERRCHECK( result );
        }

        result = system->release();
        ERRCHECK( result );
    }

    void writeWavHeader( FILE *fileBuffer, FMOD::Sound *sound, int length ) {
        
        int             channels, bits;
        float           rate;
        
        if ( !sound ) {
            return;
        }
        
        fseek( fileBuffer, 0, SEEK_SET );
        
        sound->getFormat  ( 0, 0, &channels, &bits );
        sound->getDefaults( &rate, 0 );
        
        {

            
            // WAV Structures
            typedef struct
            {
                signed char id[4];
                int         size;
            } RiffChunk;
            
            struct
            {
                RiffChunk       chunk           __PACKED;
                unsigned short  wFormatTag      __PACKED;    // format type
                unsigned short  nChannels       __PACKED;    // number of channels (i.e. mono, stereo...)
                unsigned int    nSamplesPerSec  __PACKED;    // sample rate
                unsigned int    nAvgBytesPerSec __PACKED;    // for buffer estimation
                unsigned short  nBlockAlign     __PACKED;    // block size of data
                unsigned short  wBitsPerSample  __PACKED;    // number of bits per sample of mono data
            } FmtChunk  = { {{'f','m','t',' '}, sizeof(FmtChunk) - sizeof(RiffChunk) }, 1, static_cast<unsigned short>(channels), static_cast<unsigned int>((int)rate), static_cast<unsigned int>((int)rate * channels * bits / 8), static_cast<unsigned short>(1 * channels * bits / 8), static_cast<unsigned short>(bits) };
            
            struct
            {
                RiffChunk   chunk;
            } DataChunk = { {{'d','a','t','a'}, length } };
            
            struct
            {
                RiffChunk   chunk;
                signed char rifftype[4];
            } WavHeader = { {{'R','I','F','F'}, static_cast<int>(sizeof(FmtChunk) + sizeof(RiffChunk) + length) }, {'W','A','V','E'} };

            
     
            // Write out the WAV header.
            fwrite(&WavHeader, sizeof(WavHeader), 1, fileBuffer);
            fwrite(&FmtChunk, sizeof(FmtChunk), 1, fileBuffer);
            fwrite(&DataChunk, sizeof(DataChunk), 1, fileBuffer);
        }
    }

    void writeToDisk( unsigned int whatSoundObject ) {
        
        static unsigned int lastrecordpos = 0;
        unsigned int recordpos = 0;
        
        if ( bIsRecording ) {
            FMOD_RESULT result = system->getRecordPosition( DEVICE_INDEX, &recordpos );
            ERRCHECK( result );
        }
        
        if ( recordpos != lastrecordpos ) {
            
            void *ptr1, *ptr2;
            int blocklength;
            unsigned int len1, len2;
            
            blocklength = (int)recordpos - (int)lastrecordpos;
            
            if ( blocklength < 0 ) {
                blocklength += soundlength;
            }
            
            // Lock the sound to get access to the raw data.
            sound[whatSoundObject]->lock( lastrecordpos * exinfo.numchannels * 2, blocklength * exinfo.numchannels * 2, &ptr1, &ptr2, &len1, &len2 );
            /// exinfo.numchannels * 2 = stereo 16bit.  1 sample = 4 bytes.
            
            
            // Write it to disk.
            if ( ptr1 && len1 ) {
                datalength += fwrite( ptr1, 1, len1, fileBuffer );
            }
            if ( ptr2 && len2 ) {
                datalength += fwrite( ptr2, 1, len2, fileBuffer );
            }
            
            //log( "write to disk: %d", datalength );
            
    
            // Unlock the sound to allow FMOD to use it again.
            sound[whatSoundObject]->unlock( ptr1, ptr2, len1, len2 );
        }
        
        lastrecordpos = recordpos;
        
        //printf("%-23s. Record buffer pos = %6d : Record time = %02d:%02d\r", (timeGetTime() / 500) & 1 ? "Recording to record.wav" : "", recordpos, datalength / exinfo.defaultfrequency / exinfo.numchannels / 2 / 60, (datalength / exinfo.defaultfrequency / exinfo.numchannels / 2) % 60);
        
    }
    
    bool isRecording() {
        return bIsRecording;
    }

    
    void START_outputToWaveWriter() {
        
        bIsBouncing = true;
        
        //char cDest[200] = {0};
        //[[NSString stringWithFormat:@"%@/bounce.wav", [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0]] getCString:cDest maxLength:200 encoding:NSASCIIStringEncoding];
        std::string str = Common_MediaPath_Bounce();
        char *cstr = new char[str.length() + 1];
        strcpy(cstr, str.c_str());
        // do stuff

        
        FMOD_RESULT result;
        
        result = system->close();
        ERRCHECK( result );
        
        result = system->setOutput( FMOD_OUTPUTTYPE_WAVWRITER );
        ERRCHECK( result );
        
        result = system->init( kNumOfChannels, FMOD_INIT_NORMAL, cstr );
        ERRCHECK(result);

        delete [] cstr;
        
        // Moved out to sequencer class
        /*for ( int i = 0; i < kNumOfSoundObjects; i++ ) {
            loadSoundFromDisk( i );
        }*/
        
    }
    
    void STOP_outputToWaveWriter() {
        FMOD_RESULT result;
        
        result = system->close();
        ERRCHECK( result );
        
        result = system->release();
        ERRCHECK( result );
        
        result = FMOD::System_Create( &system );
        ERRCHECK( result );
        
        result = system->init( kNumOfChannels, FMOD_INIT_NORMAL, NULL );
        ERRCHECK(result);
        
        result = system->createChannelGroup( "Rec play channel group", &recPlayChannelGroup );
        ERRCHECK( result );
        
        for ( int i = 0; i < kNumOfChannels; i++ ) {
            result = system->createDSPByType( FMOD_DSP_TYPE_FFT, &fftDsp[i] );
            ERRCHECK( result );
        }
        
        // Moved out to sequencer class
        /*for ( int i = 0; i < kNumOfSoundObjects; i++ ) {
            loadSoundFromDisk( i );
        }*/

        bIsBouncing = false;
    }
    
    bool isBouncing() {
        return bIsBouncing;
    }
    
    bool hasRecordWav( unsigned int whatSoundObject ) {
        return bHasRecordWav[whatSoundObject];
    }
    
    int numOfChannelsPlaying() {
        FMOD_RESULT result;
        int channels = 0;
        result = system->getChannelsPlaying( &channels );
        return channels;
    }
    
    unsigned int getRecPlayChannel() {
        return recPlayChannel;
    }

    bool systemIsInitialized() {
        if ( system != nullptr ) {
            return true;
        } else {
            return false;
        }
    }
    
}
