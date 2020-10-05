#ifndef __SequencerScene_SCENE_H__
#define __SequencerScene_SCENE_H__

#include "cocos2d.h"
#include "FMODAudioEngine.h"
#include "Functions.h"
#include "MainMenu.hpp"
#include "InstrumentScene.h"
#include "SeqSoundRect.hpp"
#include "BounceAndShare.hpp"

class SequencerScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(SequencerScene);
    
    virtual void update( float dt );
    

    
private:
    
    void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
    void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
    void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
    void onTouchesCancelled(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
    
    void movePlayHead();
    void stopPlayHead();
    bool CheckBoxCollision( cocos2d::Sprite *box1, cocos2d::Sprite *box2 );
    void saveData();
    void loadData();
    void removeSeqSoundRect( unsigned long who );
    void keepSeqSoundRectInArea();
    void setPlayHeadHandlePos();
    void stopBounce();
    void resetWhenReachEnd();
    void resetWhenStop();

    
    MainMenu *mainMenu;
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    cocos2d::Sprite *playHead;
    cocos2d::Sprite *playHeadHandle;
    std::vector<SeqSoundRect> seqSoundRect;
    bool someoneIsHeld;
    std::vector<cocos2d::Vec2> savePos;
    std::vector<int> saveWhatSoundObject;
    bool playHeadIsMoving;
    BounceAndShare *bounceAndShare;
    unsigned int whatState;
    


};

#endif // __SequencerScene_SCENE_H__
