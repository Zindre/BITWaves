
#include "Circle.hpp"

Circle::Circle( cocos2d::Layer *layer, cocos2d::Vec2 startPos, unsigned int activeSoundObject, int spectrum ) {
    
    //visibleSize = Director::getInstance()->getVisibleSize();
    visibleSize = Director::getInstance()->getSafeAreaRect().size;
    //origin = Director::getInstance()->getVisibleOrigin();
    origin = Director::getInstance()->getSafeAreaRect().origin;
    
    opacity = 255;
    
    float startScale = scaleValue( startPos.y, visibleSize.height, 0, 0.05f, 0.3f, true );
    
    if ( spectrum > kSpectrum_MinValue ) {
        willGrow = true;
        scaleSize = scaleValue( spectrum, kSpectrum_MinValue, kSpectrum_MaxValue, 0.0f, 0.5f, true );
        scaleSize = scaleSize + startScale;
    } else {
        willGrow = false;
        scaleSize = 0.05f;
        scaleSize = scaleSize + startScale;
    }
    
    
        
    destroy = false;
    this->startPos = startPos;
    scaleIncValue = scaleValue( startPos.y, 0, visibleSize.height, 0.0025f, 0.005f, true );
    opacityDecValue = scaleValue( startPos.y, 0, visibleSize.height, 2, 6, true );
    
    sprite = Sprite::create( "circle.png" );
    sprite->setPosition( startPos );
    sprite->setScale( scaleSize );
    layer->addChild( sprite, kLayer_Circles );
    
    switch ( activeSoundObject ) {
        case 0:
            sprite->setColor( color_soundObject0() );
            color = Color4F (color_soundObject0() );
            break;
        case 1:
            sprite->setColor( color_soundObject1() );
            color = Color4F (color_soundObject1() );
            break;
        case 2:
            sprite->setColor( color_soundObject2() );
            color = Color4F (color_soundObject2() );
            break;
        case 3:
            sprite->setColor( color_soundObject3() );
            color = Color4F (color_soundObject3() );
            break;
        case 4:
            sprite->setColor( color_soundObject4() );
            color = Color4F (color_soundObject4() );
            break;
        case 5:
            sprite->setColor( color_soundObject5() );
            color = Color4F (color_soundObject5() );
            break;
        case 6:
            sprite->setColor( color_soundObject6() );
            color = Color4F (color_soundObject6() );
            break;
            
        default:
            break;
    }
    

    


}

void Circle::update( float dt ) {
    
    opacity -= opacityDecValue;
    sprite->setOpacity( opacity );
    //id_label->setOpacity( opacity );
    
    if ( willGrow ) {
        scaleSize += scaleIncValue;
    }
    sprite->setScale( scaleSize );
    
    //drawNode->clear();
    //cocos2d::Color4F newColor = Color4F( color.r, color.g, color.b, opacity );
    //drawNode->drawCircle( startPos, scaleSize, 0, 30, false, 1.0f, 1.0f, newColor );
    
    if ( opacity <= 0 ) {
        destroy = true;
    }
    
}
