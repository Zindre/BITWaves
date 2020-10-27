
#include "CircleEmitter.hpp"

CircleEmitter::CircleEmitter( Layer *layer, unsigned int touchID, Vec2 startPos, unsigned int activeSoundObject ) {
    
    this->layer = layer;
    this->touchID = touchID;
    
    whatSoundObject = activeSoundObject;
    
    visibleSize = Director::getInstance()->getSafeAreaRect().size;
    //visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getSafeAreaRect().origin;
    //origin = Director::getInstance()->getVisibleOrigin();
    
    spawnTimer = 0.0f;
    bDestroy = false;
    pos = Vec2( startPos );
    bTouchHasEnded = false;
    soundIsDone = false;
    hasStartedFadeOut = false;
    
    /*id_label = Label::createWithTTF( "touchID: " + std::to_string( touchID ) + "\nchannelID: " + std::to_string( channelID ), "fonts/arial.ttf", 6 );
    id_label->setPosition( startPos );
    id_label->setVisible( true );
    id_label->setOpacity( 255 );
    layer->addChild( id_label, 100 );*/
    
    FMODAudioEngine::playSound( whatSoundObject );
    this->channelID = FMODAudioEngine::getChannelID();
    FMODAudioEngine::setPitch( getPitch( startPos ), channelID );
    
}

void CircleEmitter::update( float dt ) {
    
    if ( ! soundIsDone ) {
        if ( ! FMODAudioEngine::channelisPlaying( channelID ) ) {
            soundIsDone = true;
        }
    }
    
    if ( soundIsDone && circle.size() == 0 ) {
        bDestroy = true;
    }
    
    //updateLabel( FMODAudioEngine::channelisPlaying( channelID ), FMODAudioEngine::getSpectrum( channelID ), pos );
    
    if ( ! soundIsDone ) {
        spawnTimer += dt;
        if ( spawnTimer > 0.1f ) {
            circle.push_back( Circle( layer, pos, whatSoundObject, FMODAudioEngine::getSpectrum( channelID ) ) );
            spawnTimer = 0.0f;
        }
    }
    
    for ( int i = 0; i < circle.size(); i++ ) {
        circle[i].update( dt );
        if ( circle[i].destroy ) {
            layer->removeChild( circle[i].sprite );
            circle.erase( circle.begin() + i );
        }
    }

    
}

unsigned int CircleEmitter::getChannelID() {
    return channelID;
}

bool CircleEmitter::destroy() {
    return bDestroy;
}

unsigned int CircleEmitter::getTouchID() {
    return touchID;
}

/*void CircleEmitter::updateLabel( bool channelisPlaying, float spectrum, Vec2 touchPos ) {
    id_label->setString( "touchID: " + std::to_string( touchID ) +
                        "\nthis->channelID: " + std::to_string( channelID ) +
                        "\nFMOD Channel ID: " + std::to_string( FMODAudioEngine::getChannelID() ) +
                        "\nchPly: " + std::to_string( channelisPlaying ) +
                        "\nspectrum: " + std::to_string ( spectrum ) +
                        "\ntouchPosX: " + std::to_string ( touchPos.x ) +
                        "\ntouchPosY: " + std::to_string ( touchPos.y ) +
                        "\nobject: " + std::to_string ( whatSoundObject ) );
}*/

void CircleEmitter::setPos( Vec2 touchPos ) {
    pos = touchPos;
}

void CircleEmitter::setTouchHasEnded( bool touchHasEnded ) {
    bTouchHasEnded = touchHasEnded;
}

bool CircleEmitter::touchHasEnded() {
    return bTouchHasEnded;
}

float CircleEmitter::getPitch( Vec2 touchPos ) {
    
    float pitch;
    float midLinePosY = (visibleSize.height * kMidLine_Height_Multiplier) + origin.y;
    
    if ( touchPos.y < midLinePosY ) {
        pitch = scaleValue( touchPos.y, 0, midLinePosY, kPitchMin, kPitchMax, true );
    } else {
        pitch = kPitchMax;
    }
    
    return pitch;
    
}

void CircleEmitter::setPitch( Vec2 touchPos ) {
    FMODAudioEngine::setPitch( getPitch( touchPos ), channelID );
}

void CircleEmitter::fadeOut() {
    
    if ( ! hasStartedFadeOut ) {
        float soundSquareHeight = visibleSize.height / kNumOfSoundObjects;
        float fadeTime = scaleValue( pos.x, soundSquareHeight, visibleSize.width - soundSquareHeight, 0.1f, 2.0f, true );
        
        if ( ! FMODAudioEngine::isRecording() ) {
            if ( FMODAudioEngine::hasRecordWav( whatSoundObject ) ) {
                FMODAudioEngine::fadeOut( channelID, fadeTime );
            }
        }
        
        hasStartedFadeOut = true;
    }
}

 
