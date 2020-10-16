
#pragma once

#include <stdio.h>
#include "cocos2d.h"
#include "Functions.h"
#include "Konstants.h"
#include "HelpOverlay.h"

#define kBombBlowTime 1.0f


using namespace cocos2d;

class MainMenu {

public:
    
    MainMenu( cocos2d::Layer *layer, unsigned int whatScene );
    
    void updateBorder();
    
    cocos2d::Sprite *soundSquare[kNumOfSoundObjects];
    cocos2d::Sprite *waveForm[kNumOfSoundObjects];
    cocos2d::DrawNode *border[kNumOfSoundObjects];
    cocos2d::Sprite *instrumentArea;
    cocos2d::Sprite *midLine;
    cocos2d::ParticleSystemQuad *blowFlame;
    cocos2d::ParticleSystemQuad *blowSmoke;
    
    cocos2d::Sprite *buttons_black[kNumOfButtons];
    cocos2d::Sprite *buttons_gray[kNumOfButtons];
    cocos2d::Sprite *buttons_image[kNumOfButtons];
    cocos2d::Sprite *bombAnim;
    
    unsigned int getWhatScene();
    void setActiveSoundObject( unsigned int activeSoundObject );
    unsigned int getActiveSoundObject();
    void setIsLoopOn( bool loopIsOn );
    bool getIsLoopOn();
    float getPlayHeadStartPosX();
    float getPlayHeadEndPosX();
    float getInstrumentAreaWidth();
    void bombTimer( float dt );
    void resetBomb();
    bool bombIsPressed();
    bool removeAllSeqRects();
    void setRemoveAllSeqRects( bool removeAllSeqRects );
    void setBombIsPressed( bool bombIsPressed );
    void animateBomb();
    void setBounceWavIsOn( bool bounceWavIsOn );
    bool bounceWavIsOn();
    void setTouchHasBegun( bool touchHasBegun, unsigned int whatButton );
    bool getTouchHasBegun( unsigned int whatButton );
    void abortWithTouchMove( cocos2d::Vec2 touchPos );
    void setStartPos( cocos2d::Vec2 touchPos );
    //std::string getCurrentProjectName();
    //void setCurrentProjectName( std::string textFieldString );
    void updateCurrentProjectNameLabel( std::string currentProjectName );
    
    //cocos2d::Sprite *helpOverlay;
    bool helpOverlayIsVisible;
    HelpOverlay *helpOverlay;
    
private:
    
    void layoutOnLeft();
    
    
    cocos2d::Vec2 origin;
    cocos2d::Size visibleSize;
    unsigned int whatScene;
    unsigned int activeSoundObject;
    bool loopIsOn;
    float playHead_startPosX;
    float playHead_endPosX;
    float fBombTimer;
    bool bBombIsPressed;
    bool bRemoveAllSeqRects;
    float padding;
    float timeBetweenFrames;
    bool bBounceWavIsOn;
    bool buttons_touchHasBegun[kNumOfButtons];
    cocos2d::Vec2 startPos;
    cocos2d::Sprite *blackBack[kNumOfButtons];
    //std::string currentProjectName;
    cocos2d::Label *label_currentProjectName;
    
    
    
};
