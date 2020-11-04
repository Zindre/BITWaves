#pragma once

#include <stdio.h>
#include "cocos2d.h"
#include "Konstants.h"
#include "Functions.h"
#include "Circle.hpp"
#include "FMODAudioEngine.h"


using namespace cocos2d;

class CircleEmitter {
    
public:
    CircleEmitter( cocos2d::Layer *layer, unsigned int touchID, cocos2d::Vec2 startPos, unsigned int activeSoundObject );
    
    void update( float dt );
    bool destroy();
    unsigned int getChannelID();
    unsigned int getTouchID();
    void setPos( cocos2d::Vec2 touchPos );
    void setTouchHasEnded( bool touchHasEnded );
    bool touchHasEnded();
    float getPitch( cocos2d::Vec2 touchPos );
    void setPitch( cocos2d::Vec2 touchPos );
    void fadeOut();
    
    //cocos2d::Label *id_label;
    
    
private:
    
    //void updateLabel( bool channelisPlaying, float spectrum, cocos2d::Vec2 touchPos );
    
    
    std::vector<Circle> circle;
    cocos2d::Layer *layer;
    cocos2d::Vec2 safeAreaOrigin;
    cocos2d::Size safeAreaRect;
    float spawnTimer;
    unsigned int touchID;
    bool bDestroy;
    unsigned int channelID;
    unsigned int whatSoundObject;
    cocos2d::Vec2 pos;
    bool bTouchHasEnded;
    bool soundIsDone;
    bool hasStartedFadeOut;


    
};

