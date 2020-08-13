#pragma once

#include <stdio.h>
#include "cocos2d.h"
#include "Konstants.h"
#include "Functions.h"

using namespace cocos2d;

class Circle {

public:
    Circle( cocos2d::Layer *layer, cocos2d::Vec2 startPos, unsigned int activeSoundObject, int spectrum );
    
    void update( float dt );
    
    cocos2d::Sprite *sprite;
    
    bool destroy;
    
    
    
private:
    
    
    
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    
    
    float scaleSize;
    float opacity;
    cocos2d::Vec2 startPos;
    cocos2d::Color4F color;
    bool willGrow;
    float scaleIncValue;
    int opacityDecValue;
    

};
