
#include "SeqSoundRect.hpp"

SeqSoundRect::SeqSoundRect( Layer *layer, Vec2 startPos, unsigned int whatSoundObject, unsigned int soundLengthMS, float instrumentAreaWidth ) {

    //visibleSize = Director::getInstance()->getVisibleSize();
    visibleSize = Director::getInstance()->getSafeAreaRect().size;
    //origin = Director::getInstance()->getVisibleOrigin();
    origin = Director::getInstance()->getSafeAreaRect().origin;
    
    this->whatSoundObject = whatSoundObject;
    this->layer = layer;
    this->soundLengthMS = soundLengthMS;

    FileUtils *fileUtils = FileUtils::getInstance();
    std::string dirPath = fileUtils->getWritablePath();
    std::string imageString = (dirPath + "waveForm" + to_string( whatSoundObject ) + ".png" );
    
    height = visibleSize.height * 0.1f;
    log( "sound length MS: %d", soundLengthMS );
    normalWidth = scaleValue( soundLengthMS, 0, kSequenceLengthInMS, 0, instrumentAreaWidth, true );
    sprite = Sprite::create( imageString );
    sprite->setContentSize( Size( normalWidth, height ) );
    
    sprite->setPosition( startPos );
    sprite->setOpacity( kSeqSoundRect_Opacity_Low );
    sprite->setAnchorPoint( Vec2( 0.5f, 0.5f ) );
    layer->addChild( sprite, kLayer_SeqSoundRect_Over );
    
    touchArea = Sprite::create( "square1px.png" );
    touchArea->setTextureRect( Rect( 0, 0, visibleSize.width * 0.05f, sprite->getBoundingBox().size.height ) );
    touchArea->setPosition( sprite->getPosition() );
    touchArea->setOpacity( 50 );
    touchArea->setVisible( false );
    layer->addChild( touchArea, kLayer_SeqSoundRect_Over );
    
    collisionSprite = Sprite::create( "square1px.png" );
    collisionSprite->setTextureRect( Rect( 0, 0, 1, 1 ) );
    collisionSprite->setVisible( false );
    setCollisionSpritePos();
    layer->addChild( collisionSprite, kLayer_SeqSoundRect_Over );
    
    switch ( whatSoundObject ) {
        case 0:
            sprite->setColor( color_soundObject0() );
            break;
        case 1:
            sprite->setColor( color_soundObject1() );
            break;
        case 2:
            sprite->setColor( color_soundObject2() );
            break;
        case 3:
            sprite->setColor( color_soundObject3() );
            break;
        case 4:
            sprite->setColor( color_soundObject4() );
            break;
        case 5:
            sprite->setColor( color_soundObject5() );
            break;
        case 6:
            sprite->setColor( color_soundObject6() );
            break;
            
        default:
            break;
    }
    
    border = DrawNode::create();
    border->setVisible( true );
    border->setOpacity( kSeqSoundRect_Opacity_Low );
    layer->addChild( border, kLayer_SeqSoundRect_Over );
    updateBorder();
    
    
    poff = ParticleSystemQuad::create( "poff.plist" );
    cocos2d::Sprite *particleSprite = Sprite::create( "poff.png" );
    poff->setTexture( particleSprite->getTexture() );
    poff->setPosition( sprite->getPosition() );
    poff->stopSystem();
    poff->setAutoRemoveOnFinish( true );
    layer->addChild( poff, kLayer_SeqSoundRect_Over );
    
    
    bIsPlaying = false;
    isHeld = false;
    offsetPos = sprite->getPosition();
    
}

bool SeqSoundRect::isPlaying() {
    return bIsPlaying;
}

void SeqSoundRect::setIsPlaying( bool isPlaying ) {
    bIsPlaying = isPlaying;
}


unsigned int SeqSoundRect::getWhatSoundObject() {
    return whatSoundObject;
}

void SeqSoundRect::setIsHeld( bool isHeld ) {
    this->isHeld = isHeld;
}

bool SeqSoundRect::getIsHeld() {
    return isHeld;
}

void SeqSoundRect::bounceWhenHit() {
    auto getBig = ScaleTo::create( 0.1, 1.02 );
    auto getSmall = ScaleTo::create( 0.1, 0.98 );
    auto getNormal = ScaleTo::create( 0.7, 1 );
    auto elasticGetNormal = EaseElasticOut::create( getNormal->clone() );
    auto seqScale = Sequence::create( getBig, getSmall, elasticGetNormal, NULL );
    
    auto getLight = FadeTo::create( 0.1, 255 );
    auto getDark = FadeTo::create( 0.1, 100 );
    auto seqFade = Sequence::create( getLight, getDark, NULL );
    
    auto both = Spawn::create( seqScale, seqFade, NULL );
    
    sprite->runAction( both );
    border->runAction( both->clone() );
}

void SeqSoundRect::setWidth() {
    float pitch = scaleValue( sprite->getPosition().y, visibleSize.height * kSequencer_MinYpos_height_multiplier, visibleSize.height * kMidLine_Height_Multiplier, kPitchMin, kPitchMax, true );
    float multiplier = 1/pitch;
    float stretchedWidth = normalWidth * multiplier;
    sprite->setContentSize( Size( stretchedWidth, height ) );
}

void SeqSoundRect::updateBorder() {
    border->clear();
    border->drawRect( Vec2( sprite->getPosition().x - sprite->getBoundingBox().size.width/2, sprite->getPosition().y + sprite->getBoundingBox().size.height/2 ),
                     Vec2( sprite->getPosition().x + sprite->getBoundingBox().size.width/2, sprite->getPosition().y - sprite->getBoundingBox().size.height/2 ),
                     Color4F( Color4B( sprite->getColor() ) ) );

}

void SeqSoundRect::setCollisionSpritePos() {
    collisionSprite->setPosition( Vec2( sprite->getPosition().x - (sprite->getBoundingBox().size.width/2.0f), sprite->getPosition().y ) );
}

void SeqSoundRect::changeLayerToUnder() {
    sprite->setLocalZOrder( kLayer_SeqSoundRect_Under );
    border->setLocalZOrder( kLayer_SeqSoundRect_Under );
}

void SeqSoundRect::setOffsetPos( cocos2d::Vec2 touchBeganPos ) {
    offsetPos = touchBeganPos - sprite->getPosition();
}

cocos2d::Vec2 SeqSoundRect::getOffsetPos() {
    return offsetPos;
}
