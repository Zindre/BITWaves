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
    bool buttonTouchHasBegun( int whatButton );
    void setButtonTouchHasBegun( bool touchHasBegun, int whatButton );
    void abortWithTouchMove( cocos2d::Vec2 touchPos );
    void setTouchStartPos( cocos2d::Vec2 touchPos );
    
    cocos2d::Sprite *closeCross;
    cocos2d::Sprite *buttonBg[kBounceAndShare_Buttons_NumOf];
    cocos2d::Label *label_buttons[kBounceAndShare_Buttons_NumOf];
    cocos2d::Sprite *background;
    
private:
    
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    bool _buttonTouchHasBegun[kBounceAndShare_Buttons_NumOf];
    cocos2d::Vec2 _touchStartPos;
    
};
