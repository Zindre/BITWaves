

#ifndef seqSoundRect_hpp
#define seqSoundRect_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "Konstants.h"
#include "Functions.h"

using namespace cocos2d;

class SeqSoundRect {

public:
    SeqSoundRect( cocos2d::Layer *layer, cocos2d::Vec2 startPos, unsigned int whatSoundObject, unsigned int soundLengthMS, float instrumentAreaWidth, std::string currentProjectName );
    
    cocos2d::Sprite *sprite;
    cocos2d::DrawNode *border;
    cocos2d::Sprite *touchArea;
    cocos2d::ParticleSystemQuad *poff;
    cocos2d::Sprite *collisionSprite;
    
    bool isPlaying();
    void setIsPlaying( bool isPlaying );
    unsigned int getWhatSoundObject();
    void setIsHeld( bool isHeld );
    bool getIsHeld();
    void bounceWhenHit();
    void setWidth();
    void updateBorder();
    void setCollisionSpritePos();
    void changeLayerToUnder();
    void setOffsetPos( cocos2d::Vec2 touchBeganPos );
    cocos2d::Vec2 getOffsetPos();

    
    
    
private:
    
    
    
    cocos2d::Vec2 origin;
    cocos2d::Size visibleSize;
    
    bool bIsPlaying;
    int whatSoundObject;
    bool isHeld;
    cocos2d::Layer *layer;
    unsigned int soundLengthMS;
    float normalWidth;
    float height;
    cocos2d::Vec2 offsetPos;

    
    
};

#endif /* seqSoundRect_hpp */
