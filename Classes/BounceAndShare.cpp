#include "BounceAndShare.hpp"

using namespace cocos2d;

BounceAndShare::BounceAndShare( cocos2d::Layer *layer ) {
    
    visibleSize = Director::getInstance()->getSafeAreaRect().size;
    origin = Director::getInstance()->getSafeAreaRect().origin;
    
    background = Sprite::create( "square1px.png" );
    background->setTextureRect( Rect( 0, 0, visibleSize.width * 0.6, visibleSize.height * 0.6 ) );
    background->setPosition( Vec2( origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.5 ) );
    layer->addChild( background, kLayer_BounceAndShare );
    
    closeCross = Sprite::create( "closeCross.png" );
    closeCross->setPosition( Vec2( background->getPosition().x, background->getPosition().y ) );
    layer->addChild( closeCross, kLayer_BounceAndShare );
    
    _isShowing = false;
    
    hide();
}

void BounceAndShare::show() {
    background->setVisible( true );
    closeCross->setVisible( true );
}

void BounceAndShare::hide() {
    background->setVisible( false );
    closeCross->setVisible( false );
}

bool BounceAndShare::isShowing() {
    return _isShowing;
}
