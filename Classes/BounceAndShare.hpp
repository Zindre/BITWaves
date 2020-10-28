#pragma once

#include <stdio.h>
#include "cocos2d.h"
#include "Konstants.h"
#include "Functions.h"

class BounceAndShare {
    
public:
    
    BounceAndShare( cocos2d::Layer *layer );
    
    void show();
    void hideAll();
    void hideBounceWindow();
    bool buttonTouchHasBegun( int whatButton );
    void setButtonTouchHasBegun( bool touchHasBegun, int whatButton );
    void abortWithTouchMove( cocos2d::Vec2 touchPos );
    void setTouchStartPos( cocos2d::Vec2 touchPos );
    void showPrompt( std::string message );
    void hidePrompt();
    void openWebLink();
    
    cocos2d::Sprite *closeCross;
    cocos2d::Sprite *buttonBg[kBounceAndShare_Buttons_NumOf];
    cocos2d::Label *label_buttons[kBounceAndShare_Buttons_NumOf];
    cocos2d::Sprite *background;
    cocos2d::Label *label_webLink;
    
private:
    
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    bool _buttonTouchHasBegun[kBounceAndShare_Buttons_NumOf];
    cocos2d::Vec2 _touchStartPos;
    cocos2d::Label *label_bounceSuccess;
    cocos2d::Label *label_whereToFind;
    cocos2d::Label *label_wantToShare;
    cocos2d::Label *label_useRights;
    cocos2d::Label *label_prompt;
    cocos2d::Sprite *promptBg;
    cocos2d::Sprite *uploadAnim;
    
    
};
