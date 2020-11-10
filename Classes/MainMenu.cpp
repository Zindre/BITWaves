

#include "MainMenu.hpp"

MainMenu::MainMenu( Layer *layer, unsigned int whatScene ) {
    
    safeAreaRect = Director::getInstance()->getSafeAreaRect().size;
    safeAreaOrigin = Director::getInstance()->getSafeAreaRect().origin;
    
    this->whatScene = whatScene;
    
    if ( CC_TARGET_PLATFORM == CC_PLATFORM_IOS ) {
        if ( projectHandlingIsActivatedInIosPreferences() ) {
            log( "project handling is activated" );
        } else {
            log( "project handling is NOT activated" );
        }
        
        if ( bounceAndShareIsActivatedInIosPreferences() ) {
            log( "bounce and share is activated" );
        } else {
            log( "bounce and share is NOT activated" );
        }
    }

    
    std::string currentProjectName = UserDefault::getInstance()->getStringForKey( "currentProjectName" );
    
    log( "Main Menu - current project name: %s", currentProjectName.c_str() );
    
    label_currentProjectName = Label::createWithTTF( currentProjectName.c_str(), "fonts/arial.ttf", 9 );
    label_currentProjectName->setPosition( Vec2( safeAreaRect.width * 0.5 + safeAreaOrigin.x, safeAreaRect.height * 0.95 + safeAreaOrigin.y ) );
    label_currentProjectName->setColor( Color3B( 50, 50, 50 ) );
    layer->addChild( label_currentProjectName, kLayer_CurrentProjectName );
    
    FileUtils *fileUtils = FileUtils::getInstance();
    std::string writablePath = fileUtils->getWritablePath();
    std::string projectFolder = writablePath + currentProjectName;
    if ( ! fileUtils->isDirectoryExist( projectFolder ) ) {
        fileUtils->createDirectory( projectFolder );
    }
    
    
    padding = safeAreaRect.height * 0.05;
    
    float soundSquareHeight = safeAreaRect.height / kNumOfSoundObjects;
    float instrumentAreaWidth = safeAreaRect.width - (soundSquareHeight * 2.0f);
    
    for ( int i = 0; i < kNumOfButtons; i++ ) {
        buttons_black[i] = Sprite::create( "square1px.png" );
        buttons_black[i]->setAnchorPoint( Vec2( 0.0f, 1.0f ) );
        buttons_black[i]->setTextureRect( Rect( 0, 0, soundSquareHeight, soundSquareHeight ) );
        buttons_black[i]->setPosition( Vec2( safeAreaOrigin.x, safeAreaRect.height - (buttons_black[i]->getBoundingBox().size.height * i) + safeAreaOrigin.y ) );
        buttons_black[i]->setColor( Color3B( 0, 0, 0 ) );
        layer->addChild( buttons_black[i], kLayer_MainMenu );
        
        buttons_gray[i] = Sprite::create( "square1px.png" );
        buttons_gray[i]->setTextureRect( Rect( 0, 0, soundSquareHeight - (safeAreaRect.height * 0.005f), soundSquareHeight - (safeAreaRect.height * 0.005f) ) );
        buttons_gray[i]->setPosition( Vec2( buttons_black[i]->getPosition().x + (buttons_black[i]->getBoundingBox().size.width/2.0f), buttons_black[i]->getPosition().y - (buttons_black[i]->getBoundingBox().size.height/2.0f) ) );
        if ( whatScene == kScene_Instrument ) {
            buttons_gray[i]->setColor( Color3B( 20, 20, 20 ) );
        } else {
            buttons_gray[i]->setColor( Color3B( 100, 100, 100 ) );
        }
        
        layer->addChild( buttons_gray[i], kLayer_MainMenu );
        
        buttons_image[i] = Sprite::create();
        buttons_image[i]->setPosition( Vec2( buttons_black[i]->getPosition().x + (buttons_black[i]->getBoundingBox().size.width/2.0f), buttons_black[i]->getPosition().y - (buttons_black[i]->getBoundingBox().size.height/2.0f) ) );
        layer->addChild( buttons_image[i], kLayer_MainMenu );
        
        buttons_touchHasBegun[i] = false;
    }
    
    
    
    if ( whatScene == kScene_Instrument ) {
        buttons_image[kButtons_ArrayNum_Rec]->setTexture( "recButton.png" );
        buttons_image[kButtons_ArrayNum_Seq]->setTexture( "sequencerButton.png" );
        buttons_image[kButtons_ArrayNum_Stop]->setVisible( false );
        buttons_image[kButtons_ArrayNum_Stop]->setTexture( "stopButton.png" );
        buttons_image[kButtons_ArrayNum_Lock]->setTexture( "lockButton.png" );
        buttons_image[kButtons_ArrayNum_Projects]->setTexture( "projectsButton.png" );
        buttons_image[kButtons_ArrayNum_Help]->setTexture( "helpButton.png" );
    } else {
        buttons_image[kButtons_ArrayNum_Mic]->setTexture( "micButton.png" );
        buttons_image[kButtons_ArrayNum_Play]->setTexture( "playButton.png" );
        buttons_image[kButtons_ArrayNum_Loop]->setTexture( "loopButton.png" );
        buttons_image[kButtons_ArrayNum_Bounce]->setTexture( "shareButton.png" );
        buttons_image[kButtons_ArrayNum_Bomb]->setTexture( "bombButton.png" );
        buttons_image[kButtons_ArrayNum_Stop]->setTexture( "backButton.png" );
        buttons_image[kButtons_ArrayNum_Help]->setTexture( "helpButtonDark.png" );
    }


    if ( CC_TARGET_PLATFORM == CC_PLATFORM_IOS ) {
        if ( whatScene == kScene_Instrument ) {
            if ( ! projectHandlingIsActivatedInIosPreferences() ) {
                buttons_image[kButtons_ArrayNum_Projects]->setVisible( false );
            }
        }
        if ( whatScene == kScene_Sequencer ) {
            if ( ! bounceAndShareIsActivatedInIosPreferences() ) {
                buttons_image[kButtons_ArrayNum_Bounce]->setVisible( false );
            }
        }
    }
    
    
    for ( int i = 0; i < kNumOfSoundObjects; i++ ) {
        
        blackBack[i] = Sprite::create( "square1px.png" );
        blackBack[i]->setTextureRect( Rect( 0, 0, soundSquareHeight, soundSquareHeight ) );
        blackBack[i]->setAnchorPoint( Vec2( 1.0f, 1.0f ) );
        blackBack[i]->setPosition( Vec2( safeAreaRect.width + safeAreaOrigin.x, (safeAreaRect.height - (blackBack[i]->getBoundingBox().size.height ) * i ) + safeAreaOrigin.y ) );
        blackBack[i]->setColor( Color3B::BLACK );
        layer->addChild( blackBack[i], kLayer_MainMenu );
        
        soundSquare[i] = Sprite::create( "square1px.png" );
        soundSquare[i]->setTextureRect( Rect( 0, 0, soundSquareHeight, soundSquareHeight ) );
        soundSquare[i]->setAnchorPoint( Vec2( 1.0f, 1.0f ) );
        soundSquare[i]->setPosition( Vec2( safeAreaRect.width + safeAreaOrigin.x, (safeAreaRect.height - (soundSquare[i]->getBoundingBox().size.height ) * i ) + safeAreaOrigin.y ) );
        layer->addChild( soundSquare[i], kLayer_MainMenu );
    
        std::string imageFile = "waveForm" + to_string( i ) + ".png";
        std::string projectFolder = currentProjectName;
        std::string imageFileFullPath = writablePath + projectFolder + "/" + imageFile;
        std::string imageFileInProjectFolder = projectFolder + "/" + imageFile;
        log( "MainMenu constructor - image file full path: %s", imageFileFullPath.c_str() );
        
        if ( fileUtils->isFileExist( imageFileFullPath ) ) {
            Director::getInstance()->getTextureCache()->removeTexture( Sprite::create( imageFileFullPath )->getTexture() );
            log( "remove from cache: %s", imageFileFullPath.c_str() );
        }
        
        if ( fileUtils->isFileExist( imageFileFullPath ) ) {
            log( "waveForm%d.png exists", i );
            waveForm[i] = Sprite::create( imageFileFullPath );
        } else {
            log( "waveForm%d.png does NOT exist. Creating!", i );

            RenderTexture *rend = RenderTexture::create( 1.0f, 1.0f, PixelFormat::RGBA8888 );
            rend->retain();
            rend->setKeepMatrix( true );
            
            rend->beginWithClear( 0.0f, 0.0f, 0.0f, 0.0f );
            layer->visit();
            rend->end();
            
            rend->saveToFile( imageFileInProjectFolder );
            
            waveForm[i] = Sprite::create();
        }
        
        waveForm[i]->setAnchorPoint( Vec2( 0.0f, 0.5f ) );
        waveForm[i]->setPosition( Vec2( soundSquare[i]->getPosition().x - soundSquare[i]->getBoundingBox().size.width, soundSquare[i]->getPosition().y - (soundSquare[i]->getBoundingBox().size.height/2.0f) ) );
        layer->addChild( waveForm[i], kLayer_MainMenu );
        
        float scaleSize = soundSquare[i]->getBoundingBox().size.width / instrumentAreaWidth;
        waveForm[i]->setScale( scaleSize, scaleSize * 4.0f );

        

    }
    
    soundSquare[0]->setColor( color_soundObject0() );
    soundSquare[1]->setColor( color_soundObject1() );
    soundSquare[2]->setColor( color_soundObject2() );
    soundSquare[3]->setColor( color_soundObject3() );
    soundSquare[4]->setColor( color_soundObject4() );
    soundSquare[5]->setColor( color_soundObject5() );
    soundSquare[6]->setColor( color_soundObject6() );
    
    for ( int i = 0; i < kNumOfSoundObjects; i++ ) {
        waveForm[i]->setColor( soundSquare[i]->getColor() );
    }
    
    for ( int i = 0; i < kNumOfSoundObjects; i++ ) {
        border[i] = DrawNode::create();
        layer->addChild( border[i], kLayer_MainMenu );
    }

    activeSoundObject = 0;
    setActiveSoundObject( activeSoundObject );
    
    instrumentArea = Sprite::create( "square1px.png" );
    instrumentArea->setTextureRect( Rect( 0.0f, 0.0f, instrumentAreaWidth, safeAreaRect.height ) );
    instrumentArea->setAnchorPoint( Vec2( 1.0f, 1.0f ) );
    instrumentArea->setPosition( Vec2( soundSquare[0]->getPosition().x - soundSquare[0]->getBoundingBox().size.width, soundSquare[0]->getPosition().y ) );
    instrumentArea->setVisible( false );
    layer->addChild( instrumentArea, kLayer_InstrumentArea );

    
    midLine = Sprite::create( "square1px.png" );
    midLine->setTextureRect( Rect( 0.0f, 0.0f, safeAreaRect.width - soundSquare[0]->getBoundingBox().size.width, 1 ) );
    midLine->setPosition( Vec2( safeAreaOrigin.x, safeAreaRect.height * kMidLine_Height_Multiplier + safeAreaOrigin.y ) );
    midLine->setAnchorPoint( Vec2( 0.0f, 0.5f ) );
    midLine->setOpacity( 20 );
    layer->addChild( midLine, kLayer_Midline );
    
    
    bombAnim = Sprite::create( "Bomb1.png" );
    bombAnim->setPosition( Vec2( safeAreaRect.width * 0.5f + safeAreaOrigin.x, safeAreaRect.height * 0.5f + safeAreaOrigin.y ) );
    bombAnim->setVisible( false );
    layer->addChild( bombAnim, kLayer_BombAnim );
    fBombTimer = 0;
    bBombIsPressed = false;
    bRemoveAllSeqRects = false;
    timeBetweenFrames = 0;
    blowSmoke = ParticleSystemQuad::create( "blowExplosion_smoke.plist" );
    blowSmoke->setPosition( bombAnim->getPosition() );
    blowSmoke->stopSystem();
    layer->addChild( blowSmoke, kLayer_BombAnim );
    blowFlame = ParticleSystemQuad::create( "blowExplosion_flame.plist" );
    blowFlame->setPosition( bombAnim->getPosition() );
    blowFlame->stopSystem();
    layer->addChild( blowFlame, kLayer_BombAnim );
    

    
    bBounceWavIsOn = false;
    playHead_startPosX = instrumentArea->getPosition().x - instrumentArea->getBoundingBox().size.width - 1;
    playHead_endPosX = safeAreaRect.width - soundSquare[0]->getBoundingBox().size.width + safeAreaOrigin.x;
    loopIsOn = false;
    
    
    /*if ( CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID ) {
        helpOverlay = Sprite::create( "helpOverlay_Android@2x.png" );
    } else {
        helpOverlay = Sprite::create( "helpOverlay@2x.png" );
    }
    helpOverlay->setPosition( Vec2( safeAreaRect.width * 0.5f + safeAreaOrigin.x, safeAreaRect.height * 0.5f + safeAreaOrigin.y ) );
    layer->addChild( helpOverlay, kLayer_HelpOverlay );
    */

    
    helpOverlay = new HelpOverlay( layer, whatScene );
    
    
    if ( UserDefault::getInstance()->getBoolForKey( "helpOverlayHasShownOnFirstBoot" ) ) {
        helpOverlayIsVisible = false;
        //helpOverlay->setVisible( false );
        helpOverlay->hide();
    } else {
        helpOverlayIsVisible = true;
        //helpOverlay->setVisible( true );
        helpOverlay->show();
    }
    
    startPos = cocos2d::Vec2( 0.0f, 0.0f );
    
    
    // To cover long seqRects that goes beyond right menu outside safe rect
    cocos2d::Size visSize = Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 orig = Director::getInstance()->getVisibleOrigin();
    
    cocos2d::Sprite *coverRight = Sprite::create( "square1px.png" );
    coverRight->setAnchorPoint( Vec2( 0.0, 0.5 ) );
    coverRight->setTextureRect( Rect( 0, 0, blackBack[0]->getBoundingBox().size.width * 3, visSize.height ) );
    coverRight->setPosition( Vec2( blackBack[0]->getPosition().x, orig.y + (visSize.height/2) ) );
    coverRight->setColor( Color3B( 0, 0, 0 ) );
    layer->addChild( coverRight, kLayer_OutsideSafeAreaCover );
    
    // To cover the bottom area
    cocos2d::Sprite *coverBottom = Sprite::create( "square1px.png" );
    coverBottom->setAnchorPoint( Vec2( 0.5, 1.0 ) );
    coverBottom->setTextureRect( Rect( 0, 0, visSize.width, blackBack[0]->getBoundingBox().size.height * 3 ) );
    coverBottom->setPosition( Vec2( orig.x + (visSize.width/2), safeAreaOrigin.y ) );
    coverBottom->setColor( Color3B( 0, 0, 0 ) );
    layer->addChild( coverBottom, kLayer_OutsideSafeAreaCover );
    
    // To cover the left area
    cocos2d::Sprite *coverLeft = Sprite::create( "square1px.png" );
    coverLeft->setAnchorPoint( Vec2( 1.0, 1.0 ) );
    coverLeft->setTextureRect( Rect( 0, 0, blackBack[0]->getBoundingBox().size.width * 3, visSize.height ) );
    coverLeft->setPosition( Vec2( buttons_black[0]->getPosition().x, orig.y + visSize.height ) );
    coverLeft->setColor( Color3B( 0, 0, 0 ) );
    layer->addChild( coverLeft, kLayer_OutsideSafeAreaCover );
    
    
    
    


}

unsigned int MainMenu::getWhatScene() {
    return whatScene;
}

void MainMenu::setActiveSoundObject( unsigned int activeSoundObject ) {
    
    this->activeSoundObject = activeSoundObject;
    
    if ( whatScene == kScene_Instrument ) {
        for ( int i = 0; i < kNumOfSoundObjects; i++ ) {
            soundSquare[i]->setOpacity( 25 );
            waveForm[i]->setOpacity( 25 );
            border[i]->setOpacity( 40 );
            soundSquare[this->activeSoundObject]->setOpacity( 100 );
            waveForm[this->activeSoundObject]->setOpacity( 255 );
            border[this->activeSoundObject]->setOpacity( 255 );
        }
    } else {
        for ( int i = 0; i < kNumOfSoundObjects; i++ ) {
            soundSquare[i]->setOpacity( 25 );
            waveForm[i]->setOpacity( 255 );
            border[i]->setOpacity( 255 );
        }
    }

    updateBorder();
}

unsigned int MainMenu::getActiveSoundObject() {
    return activeSoundObject;
}

void MainMenu::setIsLoopOn( bool loopIsOn ) {
    this->loopIsOn = loopIsOn;
}

bool MainMenu::getIsLoopOn() {
    return loopIsOn;
}

float MainMenu::getPlayHeadStartPosX() {
    return playHead_startPosX;
}

float MainMenu::getPlayHeadEndPosX() {
    return playHead_endPosX;
}

float MainMenu::getInstrumentAreaWidth() {
    return instrumentArea->getBoundingBox().size.width;
}

void MainMenu::resetBomb() {
    bBombIsPressed = false;
    fBombTimer = 0;
    bRemoveAllSeqRects = false;
    bombAnim->stopAllActions();
}

void MainMenu::setBombIsPressed( bool bombIsPressed ) {
    bBombIsPressed = bombIsPressed;
}
bool MainMenu::bombIsPressed() {
    return bBombIsPressed;
}

void MainMenu::bombTimer( float dt ) {
    
    if ( !bRemoveAllSeqRects ) {
    
        fBombTimer += dt;
        //log( "bomb timer: %f", fBombTimer );
        if ( fBombTimer >= kBombBlowTime ) {
            bRemoveAllSeqRects = true;
            blowFlame->resetSystem();
            blowSmoke->resetSystem();
            bombAnim->setVisible( false );
            fBombTimer = 0;
        }
    }
}

bool MainMenu::removeAllSeqRects() {
    return bRemoveAllSeqRects;
}

void MainMenu::setRemoveAllSeqRects( bool removeAllSeqRects ) {
    bRemoveAllSeqRects = removeAllSeqRects;
}

void MainMenu::animateBomb() {
    
    bombAnim->setVisible( true );
    
    unsigned int numOfSpritesInAnim = 4;
    
    Vector<SpriteFrame*> animFrames( numOfSpritesInAnim + 1 );
    char str[100] = {0};
    for ( int i = 1; i < numOfSpritesInAnim + 1 ; i++ ) {
        
        sprintf( str, "Bomb%d.png", i );
        
        auto frame = SpriteFrame::create( str, Rect( 0, 0, bombAnim->getBoundingBox().size.width, bombAnim->getBoundingBox().size.height ) );
        animFrames.pushBack(frame);
    }
    
    timeBetweenFrames = kBombBlowTime/numOfSpritesInAnim;
    auto animation = Animation::createWithSpriteFrames( animFrames, timeBetweenFrames );
    auto animate = Animate::create( animation );
    bombAnim->runAction( animate );
}

void MainMenu::setBounceWavIsOn( bool bounceWavIsOn ) {
    bBounceWavIsOn = bounceWavIsOn;
}

bool MainMenu::bounceWavIsOn() {
    return bBounceWavIsOn;
}

void MainMenu::updateBorder() {
    for ( int i = 0; i < kNumOfSoundObjects; i++ ) {
        border[i]->clear();
        border[i]->drawRect( Vec2( soundSquare[i]->getPosition().x - soundSquare[i]->getBoundingBox().size.width, soundSquare[i]->getPosition().y - 1 ),
                             Vec2( soundSquare[i]->getPosition().x - 1, soundSquare[i]->getPosition().y - soundSquare[i]->getBoundingBox().size.height + 1 ),
                             Color4F( Color3B(soundSquare[i]->getColor()) ) );
    }
}

void MainMenu::setTouchHasBegun( bool touchHasBegun, unsigned int whatButton ) {
    buttons_touchHasBegun[whatButton] = touchHasBegun;
}

bool MainMenu::getTouchHasBegun( unsigned int whatButton ) {
    return buttons_touchHasBegun[whatButton];
}

void MainMenu::abortWithTouchMove( Vec2 touchPos ) {
    
    Vec2 stopPos = touchPos;
    float distX = startPos.x - stopPos.x;
    float distY = startPos.y - stopPos.y;
    //float touchMoveTolerance = safeAreaRect.width * 0.05f;
    float touchMoveTolerance = buttons_gray[0]->getBoundingBox().size.width/2;
    
    if ( abs( distX ) + abs( distY ) > touchMoveTolerance ) {
        for ( int i = 0; i < kNumOfButtons; i++ ) {
            buttons_touchHasBegun[i] = false;
            buttons_image[i]->setScale( 1.0f );
            if ( bBombIsPressed ) {
                bombAnim->setVisible( false );
                resetBomb();
            }
        }
    }
        
}

void MainMenu::setStartPos( cocos2d::Vec2 touchPos ) {
    startPos = touchPos;
}

/*std::string MainMenu::getCurrentProjectName() {
    return currentProjectName;
}*/

/*void MainMenu::setCurrentProjectName( std::string textFieldString ) {
    currentProjectName = textFieldString;
    label_currentProjectName->setString( currentProjectName );
    UserDefault::getInstance()->setStringForKey( "currentProjectName", currentProjectName );
}*/

void MainMenu::updateCurrentProjectNameLabel( std::string currentProjectName ) {
    label_currentProjectName->setString( currentProjectName );
}
