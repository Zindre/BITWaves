#ifndef __InstrumentScene_SCENE_H__
#define __InstrumentScene_SCENE_H__

#include "cocos2d.h"
#include "FMODAudioEngine.h"
#include "Functions.h"
#include "MainMenu.hpp"
#include "SequencerScene.h"
#include "CircleEmitter.hpp"

#define kMinimumRecordTime 0.1f

class InstrumentScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(InstrumentScene);
    
    virtual void update( float dt );
    
    
private:

    
    void onTouchesBegan(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event* event);
    void onTouchesMoved(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event* event);
    void onTouchesEnded(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event* event);
    void onTouchesCancelled(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
    
    void stopRecording();
    void drawWaveForm();
    void captureWaveform();
    void clearWaveForm( float dt );

    MainMenu *mainMenu;
    std::vector<CircleEmitter> circleEmitter;
    
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    float recordTimer;
    bool touchIsDown;
    unsigned int playingSoundObject[kTouch_NumOfTouchesAllowed];
    cocos2d::DrawNode *waveFormRect;
    float waveFormPosX;
    float clearWaveFormTimer;
    bool bClearWaveForm;
    bool hasUpdatedWaveSprite;
    bool recIsLocked;
    bool recIsFinalizing;


    
};

#endif // __InstrumentScene_SCENE_H__
