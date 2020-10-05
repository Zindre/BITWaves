#pragma once

#include <stdio.h>
#include "cocos2d.h"
#include "Konstants.h"
#include "Functions.h"

class BounceAndShare {
    
public:
    
    BounceAndShare( cocos2d::Layer *layer );
    
    void show();
    void hide();
    bool isShowing();
    
    cocos2d::Sprite *closeCross;
    
private:
    
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    cocos2d::Sprite *background;
    bool _isShowing;
    
};
