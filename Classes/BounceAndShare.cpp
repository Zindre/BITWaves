#include "BounceAndShare.hpp"

using namespace cocos2d;

BounceAndShare::BounceAndShare( cocos2d::Layer *layer ) {
    
    visibleSize = Director::getInstance()->getSafeAreaRect().size;
    origin = Director::getInstance()->getSafeAreaRect().origin;
    
    background = Sprite::create( "square1px.png" );
    background->setTextureRect( Rect( 0, 0, visibleSize.width * 0.6, visibleSize.height * 0.4 ) );
    background->setPosition( Vec2( origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.5 ) );
    layer->addChild( background, kLayer_BounceAndShare );
    
    Size bgSize = background->getBoundingBox().size;
    
    closeCross = Sprite::create( "closeCross.png" );
    closeCross->setPosition( Vec2( background->getPosition().x - (bgSize.width/2) + closeCross->getBoundingBox().size.width, background->getPosition().y + (bgSize.height/2) - closeCross->getBoundingBox().size.height ) );
    layer->addChild( closeCross, kLayer_BounceAndShare );
    
    
    
    for ( int i = 0; i < kBounceAndShare_Buttons_NumOf; i++ ) {
        buttonBg[i] = Sprite::create( "buttonBg.png" );
        buttonBg[i]->setColor( Color3B::BLACK );
        layer->addChild( buttonBg[i], kLayer_BounceAndShare );
        
        label_buttons[i] = Label::createWithTTF( "", "fonts/arial.ttf", kProjectHandling_FontSize_Buttons );
        layer->addChild( label_buttons[i], kLayer_BounceAndShare );
        
        _buttonTouchHasBegun[i] = false;
    }
    
    Size buttonBgSize = buttonBg[0]->getBoundingBox().size;
    buttonBg[kBounceAndShare_Buttons_Index_StartBounce]->setPosition( Vec2( background->getPosition().x - (buttonBgSize.width/1.5), background->getPosition().y ) );
    label_buttons[kBounceAndShare_Buttons_Index_StartBounce]->setPosition( buttonBg[kBounceAndShare_Buttons_Index_StartBounce]->getPosition() );
    buttonBg[kBounceAndShare_Buttons_Index_Share]->setPosition( Vec2( background->getPosition().x + (buttonBgSize.width/1.5), background->getPosition().y ) );
    label_buttons[kBounceAndShare_Buttons_Index_Share]->setPosition( buttonBg[kBounceAndShare_Buttons_Index_Share]->getPosition() );

    
    label_buttons[kBounceAndShare_Buttons_Index_StartBounce]->setString( "Lag lydfil" );
    label_buttons[kBounceAndShare_Buttons_Index_Share]->setString( "Dele med BIT20" );
    
    
    
    
    
    hide();
    
}

void BounceAndShare::show() {
    background->setVisible( true );
    closeCross->setVisible( true );
    for ( int i = 0; i < kBounceAndShare_Buttons_NumOf; i++ ) {
        buttonBg[i]->setVisible( true );
        label_buttons[i]->setVisible( true );
    }
}

void BounceAndShare::hide() {
    background->setVisible( false );
    closeCross->setVisible( false );
    for ( int i = 0; i < kBounceAndShare_Buttons_NumOf; i++ ) {
        buttonBg[i]->setVisible( false );
        label_buttons[i]->setVisible( false );
    }
}

bool BounceAndShare::buttonTouchHasBegun( int whatButton ) {
    return _buttonTouchHasBegun[whatButton];
}

void BounceAndShare::setTouchStartPos( Vec2 touchPos ) {
    _touchStartPos = touchPos;
}

void BounceAndShare::setButtonTouchHasBegun( bool touchHasBegun, int whatButton ) {
    
    _buttonTouchHasBegun[whatButton] = touchHasBegun;
    
    if ( touchHasBegun ) {
        buttonBg[whatButton]->setScale( 1.1 );
        label_buttons[whatButton]->setScale( 1.1 );
    } else {
        buttonBg[whatButton]->setScale( 1.0 );
        label_buttons[whatButton]->setScale( 1.0 );
    }
    
}

void BounceAndShare::abortWithTouchMove( Vec2 touchPos ) {
    
    Vec2 stopPos = touchPos;
    float distX = _touchStartPos.x - stopPos.x;
    float distY = _touchStartPos.y - stopPos.y;
    float touchMoveTolerance = visibleSize.width * 0.05f;
    
    float offsetAbs = abs( distX ) + abs( distY );
    
    if ( offsetAbs > touchMoveTolerance ) {
        for ( int i = 0; i < kBounceAndShare_Buttons_NumOf; i++ ) {
            _buttonTouchHasBegun[i] = false;
            buttonBg[i]->setScale( 1.0 );
            label_buttons[i]->setScale( 1.0 );
        }
    }
        
}
