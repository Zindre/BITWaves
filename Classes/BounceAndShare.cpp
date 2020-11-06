#include "BounceAndShare.hpp"

using namespace cocos2d;

BounceAndShare::BounceAndShare( cocos2d::Layer *layer ) {
    
    safeAreaRect = Director::getInstance()->getSafeAreaRect().size;
    safeAreaOrigin = Director::getInstance()->getSafeAreaRect().origin;
    
    background = Sprite::create( "square1px.png" );
    background->setTextureRect( Rect( 0, 0, safeAreaRect.width * 0.6, safeAreaRect.height * 0.8 ) );
    background->setPosition( Vec2( safeAreaOrigin.x + safeAreaRect.width * 0.5, safeAreaOrigin.y + safeAreaRect.height * 0.5 ) );
    background->setColor( Color3B::WHITE );
    layer->addChild( background, kLayer_BounceAndShare );
    
    closeCross = Sprite::create( "closeCross.png" );
    layer->addChild( closeCross, kLayer_BounceAndShare );
    
    Size bgSize = background->getBoundingBox().size;
    
    label_bounceSuccess = Label::createWithTTF( "Lagring av lydfilen var vellykket!", "fonts/arial.ttf", kFontSize_BigText );
    label_bounceSuccess->setColor( Color3B::BLACK );
    layer->addChild( label_bounceSuccess, kLayer_BounceAndShare );
    
    float padding = label_bounceSuccess->getBoundingBox().size.height;
    float lineHeightSmallText = kFontSize_SmallText * 6;
    
    label_whereToFind = Label::createWithTTF( "Lydfilen «bounce.wav» finner du i appen «Filer» under BITWaves\nog katalogen til dette prosjektet.", "fonts/arial.ttf", kFontSize_SmallText );
    label_whereToFind->setColor( Color3B::BLACK );
    label_whereToFind->setAlignment( TextHAlignment::CENTER );
    label_whereToFind->setLineHeight( lineHeightSmallText );
    layer->addChild( label_whereToFind, kLayer_BounceAndShare );
    
    label_wantToShare = Label::createWithTTF( "Vil du dele lydfilen med BIT20 Ensemble?", "fonts/arial.ttf", kFontSize_BigText );
    label_wantToShare->setColor( Color3B::BLACK );
    layer->addChild( label_wantToShare, kLayer_BounceAndShare );
    
    label_useRights = Label::createWithTTF( "Ved å dele lydfilen med BIT20\ngir du samtidig all bruksrett til BIT20.", "fonts/arial.ttf", kFontSize_SmallText );
    label_useRights->setColor( Color3B::BLACK );
    label_useRights->setAlignment( TextHAlignment::CENTER );
    label_useRights->setLineHeight( lineHeightSmallText );
    layer->addChild( label_useRights, kLayer_BounceAndShare );

    label_webLink = Label::createWithTTF( "Hør din egen og andre sine komposisjoner her!", "fonts/arial.ttf", kFontSize_SmallText );
    label_webLink->setColor( Color3B::BLACK );
    label_webLink->enableUnderline();
    layer->addChild( label_webLink, kLayer_BounceAndShare_Prompt );
    
    
    for ( int i = 0; i < kBounceAndShare_Buttons_NumOf; i++ ) {
        buttonBg[i] = Sprite::create( "buttonBg.png" );
        buttonBg[i]->setColor( Color3B::BLACK );
        layer->addChild( buttonBg[i], kLayer_BounceAndShare );
        
        label_buttons[i] = Label::createWithTTF( "", "fonts/arial.ttf", kFontSize_Buttons );
        layer->addChild( label_buttons[i], kLayer_BounceAndShare );
        
        _buttonTouchHasBegun[i] = false;
    }
    
    Size buttonBgSize = buttonBg[0]->getBoundingBox().size;
    label_buttons[kBounceAndShare_Buttons_Index_Share]->setString( "Dele med BIT20" );
    
    float bgHeight = label_bounceSuccess->getBoundingBox().size.height + padding + label_whereToFind->getBoundingBox().size.height + padding + label_wantToShare->getBoundingBox().size.height + padding + label_useRights->getBoundingBox().size.height + label_webLink->getBoundingBox().size.height + padding + buttonBgSize.height + (padding * 4);
    background->setTextureRect( Rect( 0, 0, safeAreaRect.width * 0.6, bgHeight ) );
    
    
    // SET POSITIONS
    closeCross->setPosition( Vec2( background->getPosition().x - (bgSize.width/2) + closeCross->getBoundingBox().size.width, background->getPosition().y + (bgHeight/2) - closeCross->getBoundingBox().size.height ) );
    label_bounceSuccess->setPosition( Vec2( background->getPosition().x, closeCross->getPosition().y - closeCross->getBoundingBox().size.height ) );
    label_whereToFind->setPosition( Vec2( background->getPosition().x, label_bounceSuccess->getPosition().y - label_bounceSuccess->getBoundingBox().size.height - padding ) );
    label_wantToShare->setPosition( Vec2( background->getPosition().x, label_whereToFind->getPosition().y - label_whereToFind->getBoundingBox().size.height - padding ) );
    label_useRights->setPosition( Vec2( background->getPosition().x, label_wantToShare->getPosition().y - label_wantToShare->getBoundingBox().size.height - padding ) );
    label_webLink->setPosition( Vec2( background->getPosition().x, label_useRights->getPosition().y - label_useRights->getBoundingBox().size.height ) );
    buttonBg[kBounceAndShare_Buttons_Index_Share]->setPosition( Vec2( background->getPosition().x, label_webLink->getPosition().y - label_webLink->getBoundingBox().size.height - (padding * 2) ) );
    label_buttons[kBounceAndShare_Buttons_Index_Share]->setPosition( buttonBg[kBounceAndShare_Buttons_Index_Share]->getPosition() );
    
    
    
    
    promptBg = Sprite::create( "square1px.png" );
    promptBg->setTextureRect( Rect( 0, 0, background->getBoundingBox().size.width * 0.8, background->getBoundingBox().size.height * 0.6 ) );
    promptBg->setPosition( Vec2( background->getPosition().x, background->getPosition().y ) );
    promptBg->setVisible( false );
    layer->addChild( promptBg, kLayer_BounceAndShare );
    
    label_prompt = Label::createWithTTF( "Dette er en beskjed til bruker", "fonts/arial.ttf", kFontSize_BigText );
    label_prompt->setPosition( Vec2( promptBg->getPosition().x, promptBg->getPosition().y + (promptBg->getBoundingBox().size.height * 0.25) ) );
    label_prompt->setColor( Color3B::BLACK );
    label_prompt->setVisible( false );
    label_prompt->setMaxLineWidth( promptBg->getBoundingBox().size.width * 0.8 );
    float lineHeight = kFontSize_BigText * 6;
    label_prompt->setLineHeight( lineHeight );
    label_prompt->setAlignment( TextHAlignment::CENTER );
    layer->addChild( label_prompt, kLayer_BounceAndShare );
    
    buttonBg[kBounceAndShare_Buttons_Index_PromptConfirm]->setPosition( Vec2( promptBg->getPosition().x, promptBg->getPosition().y - buttonBgSize.height ) );
    label_buttons[kBounceAndShare_Buttons_Index_PromptConfirm]->setPosition( buttonBg[kBounceAndShare_Buttons_Index_PromptConfirm]->getPosition() );
    label_buttons[kBounceAndShare_Buttons_Index_PromptConfirm]->setString( "Ok" );
    buttonBg[kBounceAndShare_Buttons_Index_PromptConfirm]->setLocalZOrder( kLayer_BounceAndShare_Prompt );
    label_buttons[kBounceAndShare_Buttons_Index_PromptConfirm]->setLocalZOrder( kLayer_BounceAndShare_Prompt );
    
    uploadAnim = Sprite::create( "uploadAnim.png" );
    uploadAnim->setPosition( promptBg->getPosition() );
    uploadAnim->setVisible( false );
    layer->addChild( uploadAnim, kLayer_BounceAndShare_Prompt );
    
    
    
    
    hideAll();
    
}

void BounceAndShare::show() {
    background->setVisible( true );
    closeCross->setVisible( true );
    for ( int i = 0; i < kBounceAndShare_Buttons_NumOf; i++ ) {
        buttonBg[i]->setVisible( true );
        label_buttons[i]->setVisible( true );
    }
    buttonBg[kBounceAndShare_Buttons_Index_PromptConfirm]->setVisible( false );
    label_buttons[kBounceAndShare_Buttons_Index_PromptConfirm]->setVisible( false );
    label_bounceSuccess->setVisible( true );
    label_whereToFind->setVisible( true );
    label_wantToShare->setVisible( true );
    label_useRights->setVisible( true );
    label_webLink->setVisible( true );
    label_webLink->setString( "Hør komposisjoner delt med BIT20 her!" );
    label_webLink->setPosition( Vec2( background->getPosition().x, label_useRights->getPosition().y - label_useRights->getBoundingBox().size.height ) );
}

void BounceAndShare::hideAll() {
    background->setVisible( false );
    closeCross->setVisible( false );
    for ( int i = 0; i < kBounceAndShare_Buttons_NumOf; i++ ) {
        buttonBg[i]->setVisible( false );
        label_buttons[i]->setVisible( false );
    }
    label_bounceSuccess->setVisible( false );
    label_whereToFind->setVisible( false );
    label_wantToShare->setVisible( false );
    label_useRights->setVisible( false );
    label_webLink->setVisible( false );
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
    float touchMoveTolerance = safeAreaRect.width * 0.05f;
    
    float offsetAbs = abs( distX ) + abs( distY );
    
    if ( offsetAbs > touchMoveTolerance ) {
        for ( int i = 0; i < kBounceAndShare_Buttons_NumOf; i++ ) {
            _buttonTouchHasBegun[i] = false;
            buttonBg[i]->setScale( 1.0 );
            label_buttons[i]->setScale( 1.0 );
        }
    }
        
}

void BounceAndShare::showPrompt( std::string message ) {
    log("Message at bns: %s", message.c_str());
    promptBg->setVisible( true );
    label_prompt->setVisible( true );
    label_prompt->setString( message );
    if ( message.compare( "Laster opp fil til BIT20..." ) == 0 ) {
        buttonBg[kBounceAndShare_Buttons_Index_PromptConfirm]->setVisible( false );
        label_buttons[kBounceAndShare_Buttons_Index_PromptConfirm]->setVisible( false );
        uploadAnim->setVisible( true );
        auto rotate = RotateBy::create( 1, 360 );
        auto repeatRotate = RepeatForever::create( rotate );
        uploadAnim->runAction( repeatRotate );
    } else if ( message.compare( "Vennligst lagre prosjektet før du laster opp" ) == 0 ) {
        buttonBg[kBounceAndShare_Buttons_Index_PromptConfirm]->setVisible( true );
        label_buttons[kBounceAndShare_Buttons_Index_PromptConfirm]->setVisible( true );
        uploadAnim->setVisible( false );
        uploadAnim->stopAllActions();
    } else {
        buttonBg[kBounceAndShare_Buttons_Index_PromptConfirm]->setVisible( true );
        label_buttons[kBounceAndShare_Buttons_Index_PromptConfirm]->setVisible( true );
        uploadAnim->setVisible( false );
        uploadAnim->stopAllActions();
        label_webLink->setVisible( true );
        label_webLink->setString( "Hør din egen og andre sine komposisjoner her!" );
        label_webLink->setPosition( Vec2( label_prompt->getPosition().x, label_prompt->getPosition().y - (label_prompt->getBoundingBox().size.height * 1.5) ) );
    }
}

void BounceAndShare::hidePrompt() {
    promptBg->setVisible( false );
    label_prompt->setVisible( false );
    buttonBg[kBounceAndShare_Buttons_Index_PromptConfirm]->setVisible( false );
    label_buttons[kBounceAndShare_Buttons_Index_PromptConfirm]->setVisible( false );
    uploadAnim->setVisible( false );
    uploadAnim->stopAllActions();
    label_webLink->setVisible( false );
}

void BounceAndShare::hideBounceWindow() {
    background->setVisible( false );
    closeCross->setVisible( false );
    buttonBg[kBounceAndShare_Buttons_Index_Share]->setVisible( false );
    label_buttons[kBounceAndShare_Buttons_Index_Share]->setVisible( false );
    label_bounceSuccess->setVisible( false );
    label_whereToFind->setVisible( false );
    label_wantToShare->setVisible( false );
    label_useRights->setVisible( false );
    label_webLink->setVisible( false );
}

void BounceAndShare::openWebLink() {
    Application::getInstance()->openURL( "https://drive.google.com/drive/folders/11xHq_TCO1xAIWQjC_1YD2pKzgaaVjnzw" );
}
