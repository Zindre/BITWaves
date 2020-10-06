#include "SequencerScene.h"
#include <iomanip>

USING_NS_CC;

Scene* SequencerScene::createScene() {
    
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = SequencerScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SequencerScene::init() {
    
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    //visibleSize = Director::getInstance()->getVisibleSize();
    visibleSize = Director::getInstance()->getSafeAreaRect().size;
    //origin = Director::getInstance()->getVisibleOrigin();
    origin = Director::getInstance()->getSafeAreaRect().origin;
    
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2( SequencerScene::onTouchesBegan, this );
    listener->onTouchesMoved = CC_CALLBACK_2( SequencerScene::onTouchesMoved, this );
    listener->onTouchesEnded = CC_CALLBACK_2( SequencerScene::onTouchesEnded, this );
    dispatcher->addEventListenerWithSceneGraphPriority( listener, this );

    mainMenu = new MainMenu( this, kScene_Sequencer );
    
    playHead = Sprite::create( "square1px.png" );
    playHead->setTextureRect( Rect( 0, 0, 1, visibleSize.height ) );
    playHead->setAnchorPoint( Vec2( 0.5f, 0.5f ) );
    playHead->setPosition( Vec2( mainMenu->getPlayHeadStartPosX(), visibleSize.height * 0.5f + origin.y ) );
    this->addChild( playHead, kLayer_PlayHead );
    someoneIsHeld = false;
    playHeadIsMoving = false;
    playHeadIsPressed = false;
    
    playHeadHandle = Sprite::create( "playHeadHandle@2x.png" );
    setPlayHeadHandlePos();
    this->addChild( playHeadHandle, kLayer_PlayHead );
    whatState = kSequencerScene_State_Normal;
    
    
    for ( int i = 0; i < kNumOfSoundObjects; i++ ) {
        FMODAudioEngine::loadSoundFromDisk( mainMenu->getCurrentProjectName(), i );
    }

    
    // ONLY FOR DEBUGGING
    /*UserDefault::getInstance()->deleteValueForKey( "current_posX" );
    UserDefault::getInstance()->deleteValueForKey( "current_posY" );
    UserDefault::getInstance()->deleteValueForKey( "current_whatSound" );*/
    
    bounceAndShare = new BounceAndShare( this );
    

    loadData();
    
    keepSeqSoundRectInArea();

    
    
    this->scheduleUpdate();
    
    return true;
}

void SequencerScene::update( float dt ) {
    
    FMODAudioEngine::update();
    
    for ( int i = 0; i < seqSoundRect.size(); i++ ) {
        if ( ! seqSoundRect[i].isPlaying() ) {
            if ( ! seqSoundRect[i].getIsHeld() ) {
                
                if ( whatState == kSequencerScene_State_PlayHeadIsMoving || whatState == kSequencerScene_State_IsBouncing ) {
                    
                    if ( CheckBoxCollision( seqSoundRect[i].collisionSprite, playHead ) ) {
                        FMODAudioEngine::playSound( seqSoundRect[i].getWhatSoundObject() );
                        float pitch  = scaleValue( seqSoundRect[i].sprite->getPosition().y, visibleSize.height * kSequencer_MinYpos_height_multiplier, visibleSize.height * kMidLine_Height_Multiplier, kPitchMin, kPitchMax, true );
                        log( "fmod pitch: %f", pitch );
                        //log( "pitch UPDATE: %f", pitch );
                        FMODAudioEngine::setPitch( pitch, FMODAudioEngine::getChannelID() );
                        seqSoundRect[i].setIsPlaying( true );
                        //seqSoundRect[i].bounceWhenHit();
                        seqSoundRect[i].sprite->setOpacity( kSeqSoundRect_Opacity_High );
                        seqSoundRect[i].border->setOpacity( kSeqSoundRect_Opacity_High );
                    }
                    
                }
            }
        }
        
        if ( seqSoundRect[i].isPlaying() ) {
            if ( ! CheckBoxCollision( seqSoundRect[i].sprite, playHead ) && ! CheckBoxCollision( seqSoundRect[i].collisionSprite, playHead ) ) {
                seqSoundRect[i].setIsPlaying( false );
                seqSoundRect[i].sprite->setOpacity( kSeqSoundRect_Opacity_Low );
                seqSoundRect[i].border->setOpacity( kSeqSoundRect_Opacity_Low );
            }
        }
    }
    
    /*if ( FMODAudioEngine::getChannelPos() > 0 ) {
        log( "channel pos: %d", FMODAudioEngine::getChannelPos() );
    }*/
    
    if ( whatState == kSequencerScene_State_BombIsPressed ) {
        
        mainMenu->bombTimer( dt );
    
        // Remove all
        if ( mainMenu->removeAllSeqRects() ) {
            for ( unsigned long i = seqSoundRect.size(); i-- > 0; ) {
                seqSoundRect[i].poff->resetSystem();
                removeSeqSoundRect( i );
                mainMenu->setRemoveAllSeqRects( false );
            }
        }
    }
}

void SequencerScene::onTouchesBegan( const std::vector<Touch*>& touches, Event* event ) {
    
    for ( auto &touch : touches ) {
        if ( touch != nullptr ) {
            if ( touch->getID() == 0 ) {
            
                if ( whatState == kSequencerScene_State_Normal ) {
                    
                    // MAIN MENU
                    mainMenu->setStartPos( touch->getLocation() );
                    
                    // BACK BUTTON
                    if ( mainMenu->buttons_image[kButtons_ArrayNum_Stop]->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                        mainMenu->setTouchHasBegun( true, kButtons_ArrayNum_Stop );
                        mainMenu->buttons_image[kButtons_ArrayNum_Stop]->setScale( kButtons_ScaleValue );
                    }
                    
                    // BOMB BUTTON
                    if ( mainMenu->buttons_image[kButtons_ArrayNum_Bomb]->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                        //mainMenu->setBombIsPressed( true );
                        mainMenu->animateBomb();
                        mainMenu->buttons_image[kButtons_ArrayNum_Bomb]->setScale( kButtons_ScaleValue );
                        whatState = kSequencerScene_State_BombIsPressed;
                    }
                    
                    // MIC BUTTON
                    if ( mainMenu->buttons_image[kButtons_ArrayNum_Mic]->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                        mainMenu->setTouchHasBegun( true, kButtons_ArrayNum_Mic );
                        mainMenu->buttons_image[kButtons_ArrayNum_Mic]->setScale( kButtons_ScaleValue );
                    }
                    
                    // PLAY BUTTON
                    if ( mainMenu->buttons_image[kButtons_ArrayNum_Play]->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                        mainMenu->setTouchHasBegun( true, kButtons_ArrayNum_Play );
                        mainMenu->buttons_image[kButtons_ArrayNum_Play]->setScale( kButtons_ScaleValue );
                    }
                    
                    // HELP BUTTON
                    if ( mainMenu->buttons_image[kButtons_ArrayNum_Help]->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                        mainMenu->buttons_image[kButtons_ArrayNum_Help]->setScale( kButtons_ScaleValue );
                        mainMenu->setTouchHasBegun( true, kButtons_ArrayNum_Help );
                    }
                    
                    // BOUNCE AND SHARE BUTTON
                    if ( mainMenu->buttons_image[kButtons_ArrayNum_Bounce]->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                        mainMenu->setTouchHasBegun( true, kButtons_ArrayNum_Bounce );
                        mainMenu->buttons_image[kButtons_ArrayNum_Bounce]->setScale( kButtons_ScaleValue );
                    }
                    
                } else if ( whatState == kSequencerScene_State_PlayHeadIsMoving ) {
                    
                    // STOP BUTTON
                    if ( mainMenu->buttons_image[kButtons_ArrayNum_Stop]->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                        mainMenu->setTouchHasBegun( true, kButtons_ArrayNum_Stop );
                        mainMenu->buttons_image[kButtons_ArrayNum_Stop]->setScale( kButtons_ScaleValue );
                    }
                    
                } else if ( whatState == kSequencerScene_State_BounceAndShare ) {
                    
                    bounceAndShare->setTouchStartPos( touch->getLocation() );
                    
                    // CLOSE CROSS
                    if ( bounceAndShare->closeCross->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                        bounceAndShare->hide();
                        whatState = kSequencerScene_State_Normal;
                    }
            
                }
                
                if ( whatState == kSequencerScene_State_PlayHeadIsMoving || whatState == kSequencerScene_State_Normal ) {
                    
                    // LOOP BUTTON
                    if ( mainMenu->buttons_image[kButtons_ArrayNum_Loop]->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                        mainMenu->setTouchHasBegun( true, kButtons_ArrayNum_Loop );
                        mainMenu->buttons_image[kButtons_ArrayNum_Loop]->setScale( kButtons_ScaleValue );
                    }
                    
                    // RIGHT SOUND SELECT MENU
                    for ( int i = 0; i < kNumOfSoundObjects; i++ ) {
                        int whatSoundObject = i;
                        if ( FMODAudioEngine::hasRecordWav( whatSoundObject ) ) {
                            if ( mainMenu->soundSquare[whatSoundObject]->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                                seqSoundRect.push_back( SeqSoundRect( this, touch->getLocation(), whatSoundObject, FMODAudioEngine::getSoundLength( whatSoundObject ), mainMenu->getInstrumentAreaWidth(), mainMenu->getCurrentProjectName() ) );
                                savePos.push_back( Vec2( 0, 0 ) );
                                saveWhatSoundObject.push_back( whatSoundObject );
                            }
                        }
                    }
                    
                    // SOUND BOXES ON CANVAS
                    for ( unsigned long i = seqSoundRect.size(); i-- > 0; ) {
                        if ( ! seqSoundRect[i].getIsHeld() && ! someoneIsHeld ) {
                            if ( seqSoundRect[i].sprite->getBoundingBox().containsPoint( touch->getLocation() ) || seqSoundRect[i].touchArea->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                                seqSoundRect[i].setIsHeld( true );
                                someoneIsHeld = true;
                                seqSoundRect[i].setOffsetPos( touch->getLocation() );
                            }
                        }
                    }
                    
                }
                
                if ( whatState == kSequencerScene_State_BounceAndShare || whatState == kSequencerScene_State_IsBouncing ) {
                
                    // START BOUNCE BUTTON
                    if ( bounceAndShare->buttonBg[kBounceAndShare_Buttons_Index_StartBounce]->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                        bounceAndShare->setButtonTouchHasBegun( true, kBounceAndShare_Buttons_Index_StartBounce );
                    }
                    
                }
                
                if ( whatState == kSequencerScene_State_Normal || whatState == kSequencerScene_State_BounceAndShare ) {
                    
                    // PLAY HEAD HANDLE
                    if ( playHeadHandle->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                        playHeadIsPressed = true;
                    }
                    
                }
                
                
                
            }
        

        }
    }
    
}

void SequencerScene::onTouchesMoved( const std::vector<Touch*> &touches, Event* event ) {
    
    for ( auto touch : touches ) {
        if ( touch != nullptr ) {
            if ( touch->getID() == 0 ) {
            
                if ( whatState == kSequencerScene_State_Normal ) {
                    
                    mainMenu->abortWithTouchMove( touch->getLocation() );
                    
                } else if ( whatState == kSequencerScene_State_BounceAndShare ) {
                    
                    bounceAndShare->abortWithTouchMove( touch->getLocation() );
                    
                }
                
                if ( whatState == kSequencerScene_State_Normal || whatState == kSequencerScene_State_BounceAndShare ) {
                    
                    if ( playHeadIsPressed ) {
                        playHeadHandle->setPositionX( touch->getLocation().x );
                        playHead->setPositionX( playHeadHandle->getPosition().x );
                        
                        if ( playHeadHandle->getPosition().x <= mainMenu->getPlayHeadStartPosX() ) {
                            playHeadHandle->setPositionX( mainMenu->getPlayHeadStartPosX() );
                            playHead->setPositionX( playHeadHandle->getPosition().x );
                        }
                        if ( playHeadHandle->getPosition().x >= mainMenu->getPlayHeadEndPosX() ) {
                            playHeadHandle->setPositionX( mainMenu->getPlayHeadEndPosX() );
                            playHead->setPositionX( playHeadHandle->getPosition().x );
                        }
                    }
                    
                }
                
                if ( whatState == kSequencerScene_State_Normal || whatState == kSequencerScene_State_PlayHeadIsMoving ) {
                    
                    // SOUND BOXES ON CANVAS
                    for ( int i = 0; i < seqSoundRect.size(); i++ ) {
                        if ( seqSoundRect[i].getIsHeld() ) {
                            seqSoundRect[i].sprite->setPositionY( touch->getLocation().y - seqSoundRect[i].getOffsetPos().y );
                            seqSoundRect[i].sprite->setPositionX( touch->getLocation().x - seqSoundRect[i].getOffsetPos().x );
                            seqSoundRect[i].setWidth();
                            seqSoundRect[i].poff->setPosition( seqSoundRect[i].sprite->getPosition() );
                            seqSoundRect[i].touchArea->setPosition( seqSoundRect[i].sprite->getPosition() );
                            seqSoundRect[i].updateBorder();
                            savePos[i] = seqSoundRect[i].sprite->getPosition();
                            keepSeqSoundRectInArea();
                            seqSoundRect[i].setCollisionSpritePos();
                        }
                    }
                    
                }
                
                
            }
            
            
        }
    }
}

void SequencerScene::onTouchesEnded( const std::vector<Touch*> &touches, Event* event ) {
    
    for ( auto touch : touches ) {
        if ( touch != nullptr ) {
            if ( touch->getID() == 0 ) {
            
                if ( whatState == kSequencerScene_State_Normal ) {
                    
                    // MIC BUTTON
                    if ( mainMenu->buttons_image[kButtons_ArrayNum_Mic]->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                        if ( mainMenu->getTouchHasBegun( kButtons_ArrayNum_Mic ) ) {
                            FMODAudioEngine::stopSound();
                            saveData();
                            auto scene = InstrumentScene::createScene();
                            Director::getInstance()->replaceScene( scene );
                        }
                    }
                    
                    // PLAY BUTTON
                    if ( mainMenu->buttons_image[kButtons_ArrayNum_Play]->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                        if ( mainMenu->getTouchHasBegun( kButtons_ArrayNum_Play ) ) {
                            FMODAudioEngine::stopSound();
                            movePlayHead();
                            playHeadHandle->setVisible( false );
                            mainMenu->buttons_image[kButtons_ArrayNum_Stop]->setTexture( "stopButtonDark.png" );
                            whatState = kSequencerScene_State_PlayHeadIsMoving;
                        }
                    }
                    
                    // BOUNCE AND SHARE BUTTON
                    if ( mainMenu->buttons_image[kButtons_ArrayNum_Bounce]->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                        if ( mainMenu->getTouchHasBegun( kButtons_ArrayNum_Bounce ) ) {
                            bounceAndShare->show();
                            whatState = kSequencerScene_State_BounceAndShare;
                        }
                    }
                    
                    // HELP BUTTON
                    if ( mainMenu->buttons_image[kButtons_ArrayNum_Help]->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                        if ( mainMenu->getTouchHasBegun( kButtons_ArrayNum_Help ) ) {
                            mainMenu->helpOverlay->show();
                            whatState = kSequencerScene_State_HelpOverlay;
                        }
                    }
                    
                    // BACK BUTTON
                    if ( mainMenu->buttons_image[kButtons_ArrayNum_Stop]->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                        if ( mainMenu->getTouchHasBegun( kButtons_ArrayNum_Stop ) ) {
                            playHead->setPosition( Vec2( mainMenu->getPlayHeadStartPosX(), visibleSize.height * 0.5 + origin.y ) );
                            playHeadHandle->setPositionX( playHead->getPosition().x );
                        }
                    }
          
                } else if ( whatState == kSequencerScene_State_HelpOverlay ) {
                    
                    mainMenu->helpOverlay->hide();
                    whatState = kSequencerScene_State_Normal;
                    
                } else if ( whatState == kSequencerScene_State_PlayHeadIsMoving ) {
                    
                    // STOP BUTTON
                    if ( mainMenu->buttons_image[kButtons_ArrayNum_Stop]->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                        if ( mainMenu->getTouchHasBegun( kButtons_ArrayNum_Stop ) ) {
                            resetWhenStop();
                        }
                    }
            
                } else if ( whatState == kSequencerScene_State_BombIsPressed ) {
                    
                    mainMenu->resetBomb();
                    whatState = kSequencerScene_State_Normal;
                    
                }
                
                if ( whatState == kSequencerScene_State_PlayHeadIsMoving || whatState == kSequencerScene_State_Normal ) {
                    
                    // LOOP BUTTON
                    if ( mainMenu->buttons_image[kButtons_ArrayNum_Loop]->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                        if ( mainMenu->getTouchHasBegun( kButtons_ArrayNum_Loop ) ) {
                            if ( ! mainMenu->getIsLoopOn() ) {
                                mainMenu->setIsLoopOn( true );
                                mainMenu->buttons_image[kButtons_ArrayNum_Loop]->setColor( Color3B::YELLOW );
                            } else {
                                mainMenu->setIsLoopOn( false );
                                mainMenu->buttons_image[kButtons_ArrayNum_Loop]->setColor( Color3B::WHITE );
                            }
                        }
                    }
                    
                    // SOUND BOXES ON CANVAS
                    if ( someoneIsHeld ) {
                        keepSeqSoundRectInArea();
                        saveData();
                    }
                    for ( int i = 0; i < seqSoundRect.size(); i++ ) {
                        seqSoundRect[i].setIsHeld( false );
                        seqSoundRect[i].changeLayerToUnder();
                    }
                    someoneIsHeld = false;
                    
                    // REMOVE ONE
                    for ( unsigned long i = 0; i < seqSoundRect.size(); i++ ) {
                        if ( seqSoundRect[i].sprite->getPosition().x > mainMenu->instrumentArea->getPosition().x ) {
                            seqSoundRect[i].poff->resetSystem();
                            removeSeqSoundRect( i );
                        }
                    }
                    
                }
                
                if ( whatState == kSequencerScene_State_BounceAndShare || whatState == kSequencerScene_State_IsBouncing ) {
                    
                    if ( bounceAndShare->buttonTouchHasBegun( kBounceAndShare_Buttons_Index_StartBounce ) ) {
                        
                        if ( mainMenu->bounceWavIsOn() ) {
                            stopBounce();
                        } else {
                            mainMenu->setBounceWavIsOn( true );
                            //mainMenu->buttons_image[kButtons_ArrayNum_Bounce]->setColor( Color3B::RED );
                            FMODAudioEngine::stopSound();
                            FMODAudioEngine::START_outputToWaveWriter( mainMenu->getCurrentProjectName() );
                            for ( int i = 0; i < kNumOfSoundObjects; i++ ) {
                                FMODAudioEngine::loadSoundFromDisk( mainMenu->getCurrentProjectName(), i );
                            }
                            movePlayHead();
                            playHeadHandle->setVisible( false );
                            mainMenu->buttons_image[kButtons_ArrayNum_Play]->setColor( Color3B( kButtons_GrayedOutValue, kButtons_GrayedOutValue, kButtons_GrayedOutValue ) );
                            mainMenu->buttons_image[kButtons_ArrayNum_Stop]->setColor( Color3B( kButtons_GrayedOutValue, kButtons_GrayedOutValue, kButtons_GrayedOutValue ) );
                            mainMenu->buttons_image[kButtons_ArrayNum_Loop]->setColor( Color3B( kButtons_GrayedOutValue, kButtons_GrayedOutValue, kButtons_GrayedOutValue ) );
                            mainMenu->buttons_image[kButtons_ArrayNum_Bomb]->setColor( Color3B( kButtons_GrayedOutValue, kButtons_GrayedOutValue, kButtons_GrayedOutValue ) );
                            mainMenu->buttons_image[kButtons_ArrayNum_Mic]->setColor( Color3B( kButtons_GrayedOutValue, kButtons_GrayedOutValue, kButtons_GrayedOutValue ) );
                            mainMenu->buttons_image[kButtons_ArrayNum_Help]->setColor( Color3B( kButtons_GrayedOutValue, kButtons_GrayedOutValue, kButtons_GrayedOutValue ) );
                            whatState = kSequencerScene_State_IsBouncing;
                            bounceAndShare->label_buttons[kBounceAndShare_Buttons_Index_StartBounce]->setString( "Stop" );
                            bounceAndShare->buttonBg[kBounceAndShare_Buttons_Index_StartBounce]->setColor( Color3B::RED );
                        }
                        
                    }
                    
                    // Touch bagan to false
                    for ( int i = 0; i < kBounceAndShare_Buttons_NumOf; i++ ) {
                        bounceAndShare->setButtonTouchHasBegun( false, i );
                    }
                    
                }
                
            }
            
            
            
            
            
            mainMenu->buttons_image[kButtons_ArrayNum_Play]->setScale( 1.0f );
            mainMenu->buttons_image[kButtons_ArrayNum_Stop]->setScale( 1.0f );
            mainMenu->buttons_image[kButtons_ArrayNum_Loop]->setScale( 1.0f );
            mainMenu->buttons_image[kButtons_ArrayNum_Bounce]->setScale( 1.0f );
            mainMenu->buttons_image[kButtons_ArrayNum_Mic]->setScale( 1.0f );
            mainMenu->buttons_image[kButtons_ArrayNum_Bomb]->setScale( 1.0f );
            mainMenu->buttons_image[kButtons_ArrayNum_Help]->setScale( 1.0f );
            mainMenu->setTouchHasBegun( false, kButtons_ArrayNum_Mic );
            mainMenu->setTouchHasBegun( false, kButtons_ArrayNum_Play );
            mainMenu->setTouchHasBegun( false, kButtons_ArrayNum_Stop );
            mainMenu->setTouchHasBegun( false, kButtons_ArrayNum_Loop );
            mainMenu->setTouchHasBegun( false, kButtons_ArrayNum_Bounce );
            mainMenu->bombAnim->setVisible( false );
            playHeadIsPressed = false;
            
            
        }
    }
}

void SequencerScene::removeSeqSoundRect( unsigned long who ) {

    this->removeChild( seqSoundRect[who].sprite );
    this->removeChild( seqSoundRect[who].touchArea );
    this->removeChild( seqSoundRect[who].border );
    this->removeChild( seqSoundRect[who].collisionSprite );
    savePos.erase( savePos.begin() + who );
    saveWhatSoundObject.erase( saveWhatSoundObject.begin() + who );
    seqSoundRect.erase( seqSoundRect.begin() + who );
    
    UserDefault::getInstance()->deleteValueForKey( "current_posX" );
    UserDefault::getInstance()->deleteValueForKey( "current_posY" );
    UserDefault::getInstance()->deleteValueForKey( "current_whatSound" );
    
}

void SequencerScene::onTouchesCancelled( const std::vector<Touch*>& touches, Event  *event ) {
    onTouchesEnded( touches, event );
}

void SequencerScene::movePlayHead() {
    
    playHead->stopAllActions();
    
    int fullLengthTimeInSec = kSequenceLengthInMS/1000;
    float fullLengthInPx = mainMenu->getInstrumentAreaWidth();
    log( "full length in px: %f", fullLengthInPx );
    
    float speed = fullLengthInPx / fullLengthTimeInSec;
    log( "speed: %f", speed );
    
    float distance = playHead->getPosition().getDistance( Vec2( mainMenu->getPlayHeadEndPosX(), playHead->getPosition().y ) );
    log( "distance: %f", distance );

    float duration = distance / speed;
    log( "duration: %f", duration );
    
    auto moveRight = MoveTo::create( duration, Vec2( mainMenu->getPlayHeadEndPosX(), visibleSize.height * 0.5 + origin.y ) );
    
    auto stopBounce = CallFunc::create( CC_CALLBACK_0( SequencerScene::stopBounce, this ) );
    auto seqBounce = Sequence::create( moveRight, stopBounce, NULL );
    
    auto resetWhenReachEnd = CallFunc::create( CC_CALLBACK_0( SequencerScene::resetWhenReachEnd, this ) );
    auto seqNormalPlay = Sequence::create( moveRight, resetWhenReachEnd, NULL );
    
    seqNormalPlay->setTag( kTag_MoveRight );
    seqBounce->setTag( kTag_MoveRight );
    
    if ( mainMenu->bounceWavIsOn() ) {
        playHead->runAction( seqBounce );
    } else {
        playHead->runAction( seqNormalPlay );
    }

}

void SequencerScene::stopBounce() {
    mainMenu->setBounceWavIsOn( false );
    //mainMenu->buttons_image[kButtons_ArrayNum_Bounce]->setColor( Color3B::WHITE );
    FMODAudioEngine::stopSound();
    stopPlayHead();
    playHeadHandle->setVisible( true );
    setPlayHeadHandlePos();
    FMODAudioEngine::STOP_outputToWaveWriter();
    for ( int i = 0; i < kNumOfSoundObjects; i++ ) {
        FMODAudioEngine::loadSoundFromDisk( mainMenu->getCurrentProjectName(), i );
    }
    mainMenu->buttons_image[kButtons_ArrayNum_Play]->setColor( Color3B::WHITE );
    mainMenu->buttons_image[kButtons_ArrayNum_Stop]->setColor( Color3B::WHITE );
    mainMenu->buttons_image[kButtons_ArrayNum_Loop]->setColor( Color3B::WHITE );
    mainMenu->buttons_image[kButtons_ArrayNum_Bomb]->setColor( Color3B::WHITE );
    mainMenu->buttons_image[kButtons_ArrayNum_Mic]->setColor( Color3B::WHITE );
    mainMenu->buttons_image[kButtons_ArrayNum_Help]->setColor( Color3B::WHITE );
    
    //saveToIcloud();
    
    whatState = kSequencerScene_State_BounceAndShare;
    bounceAndShare->label_buttons[kBounceAndShare_Buttons_Index_StartBounce]->setString( "Lag lydfil" );
    bounceAndShare->buttonBg[kBounceAndShare_Buttons_Index_StartBounce]->setColor( Color3B::BLACK );
}

void SequencerScene::resetWhenStop() {
    FMODAudioEngine::stopSound();
    for ( int i = 0; i < seqSoundRect.size(); i++ ) {
        seqSoundRect[i].setIsPlaying( false );
        seqSoundRect[i].sprite->setOpacity( kSeqSoundRect_Opacity_Low );
        seqSoundRect[i].border->setOpacity( kSeqSoundRect_Opacity_Low );
    }
    mainMenu->buttons_image[kButtons_ArrayNum_Stop]->setTexture( "backButton.png" );
    playHeadHandle->setVisible( true );
    setPlayHeadHandlePos();
    stopPlayHead();
    whatState = kSequencerScene_State_Normal;
}

void SequencerScene::resetWhenReachEnd() {
    FMODAudioEngine::stopSound();
    for ( int i = 0; i < seqSoundRect.size(); i++ ) {
        seqSoundRect[i].setIsPlaying( false );
        seqSoundRect[i].sprite->setOpacity( kSeqSoundRect_Opacity_Low );
        seqSoundRect[i].border->setOpacity( kSeqSoundRect_Opacity_Low );
    }
    if ( mainMenu->getIsLoopOn() ) {
        playHead->setPosition( Vec2( mainMenu->getPlayHeadStartPosX(), visibleSize.height * 0.5 + origin.y ) );
        playHeadHandle->setPositionX( playHead->getPosition().x );
        movePlayHead();
    } else {
        mainMenu->buttons_image[kButtons_ArrayNum_Stop]->setTexture( "backButton.png" );
        playHeadHandle->setVisible( true );
        setPlayHeadHandlePos();
        stopPlayHead();
        whatState = kSequencerScene_State_Normal;
    }

}

void SequencerScene::stopPlayHead() {
    playHead->stopActionByTag( kTag_MoveRight );
}

bool SequencerScene::CheckBoxCollision( Sprite *box1, Sprite *box2 ) {
    
    Rect box1rect = box1->getBoundingBox();
    Rect box2rect = box2->getBoundingBox();
    
    if ( box1rect.intersectsRect( box2rect ) ) {
        return true;
    } else {
        return false;
    }
}

void SequencerScene::saveData() {
    
    // ----------------------------------------------------------------------------------
    if ( savePos.size() != 0 ) {
        Data data_pos_X;
        std::vector<float> pos_X;
        
        for ( int i = 0; i < savePos.size(); i++ ) {
            pos_X.push_back( savePos[i].x );
        }
        
        for ( int i = 0; i < pos_X.size(); i++ ) {
            log( "sequencer save pos X: %f", pos_X[i] );
        }
        
        data_pos_X.copy((unsigned char*) pos_X.data(), pos_X.size() * sizeof(float));
        UserDefault::getInstance()->setDataForKey( "current_posX", data_pos_X );
    }
    // ----------------------------------------------------------------------------------
    
    
    // ----------------------------------------------------------------------------------
    if ( savePos.size() != 0 ) {
        Data data_pos_Y;
        std::vector<float> pos_Y;
        
        for ( int i = 0; i < savePos.size(); i++ ) {
            pos_Y.push_back( savePos[i].y );
        }
        
        for ( int i = 0; i < pos_Y.size(); i++ ) {
            log( "sequencer save pos Y: %f", pos_Y[i] );
        }
        
        data_pos_Y.copy((unsigned char*) pos_Y.data(), pos_Y.size() * sizeof(float));
        UserDefault::getInstance()->setDataForKey( "current_posY", data_pos_Y );
    }
    // ----------------------------------------------------------------------------------
    
    
    // ----------------------------------------------------------------------------------
    if ( saveWhatSoundObject.size() != 0 ) {
        Data data_whatSound;
        std::vector<int> whatSound;
        
        for ( int i = 0; i < saveWhatSoundObject.size(); i++ ) {
            whatSound.push_back( saveWhatSoundObject[i] );
        }
        
        for ( int i = 0; i < whatSound.size(); i++ ) {
            log( "sequencer save whatSound: %i", whatSound[i] );
        }
        
        data_whatSound.copy((unsigned char*) whatSound.data(), whatSound.size() * sizeof(int));
        UserDefault::getInstance()->setDataForKey( "current_whatSound", data_whatSound );
    }
    // ----------------------------------------------------------------------------------
    
    
}

void SequencerScene::loadData() {

    // ----------------------------------------------------------------------------------
    Data data_pos_X = UserDefault::getInstance()->getDataForKey( "current_posX" );
    float*  buffer_X = (float*) data_pos_X.getBytes();
    ssize_t length_X = data_pos_X.getSize() / sizeof(float);
    
    std::ostringstream ss_X;
    ss_X << std::setprecision(2) << std::fixed;
    for (int i = 0; i < length_X; i++) {
        ss_X << buffer_X[i] << " ";
    }
    CCLOG("sequencer load %s is %s", "current_posX", ss_X.str().c_str());
    // ----------------------------------------------------------------------------------
    
    
    // ----------------------------------------------------------------------------------
    Data data_pos_Y = UserDefault::getInstance()->getDataForKey( "current_posY" );
    float*  buffer_Y = (float*) data_pos_Y.getBytes();
    ssize_t length_Y = data_pos_Y.getSize() / sizeof(float);
    
    std::ostringstream ss_Y;
    ss_Y << std::setprecision(2) << std::fixed;
    for (int i = 0; i < length_Y; i++) {
        ss_Y << buffer_Y[i] << " ";
    }
    CCLOG("sequencer load %s is %s", "current_posY", ss_Y.str().c_str());
    // ----------------------------------------------------------------------------------
    
    
    // ----------------------------------------------------------------------------------
    Data data_whatSound = UserDefault::getInstance()->getDataForKey( "current_whatSound" );
    int* buffer_whatSound = (int*) data_whatSound.getBytes();
    ssize_t length_whatSound = data_whatSound.getSize() / sizeof(int);
    
    std::ostringstream ss_whatSound;
    ss_whatSound << std::setprecision(2) << std::fixed;
    for ( int i = 0; i < length_whatSound; i++ ) {
        ss_whatSound << buffer_whatSound[i] << " ";
    }
    log( "sequencer load %s is %s", "current_whatSound", ss_whatSound.str().c_str() );
    // ----------------------------------------------------------------------------------
    
    for ( int i = 0; i < length_X; i++ ) {
        seqSoundRect.push_back( SeqSoundRect( this, Vec2( buffer_X[i], buffer_Y[i] ), buffer_whatSound[i], FMODAudioEngine::getSoundLength( buffer_whatSound[i] ), mainMenu->getInstrumentAreaWidth(), mainMenu->getCurrentProjectName() ) );
        savePos.push_back( Vec2( buffer_X[i], buffer_Y[i] ) );
        saveWhatSoundObject.push_back( buffer_whatSound[i] );
    }
    
    for ( int i = 0; i < seqSoundRect.size(); i++ ) {
        seqSoundRect[i].setWidth();
        seqSoundRect[i].updateBorder();
        seqSoundRect[i].setCollisionSpritePos();
        seqSoundRect[i].changeLayerToUnder();
    }
}

void SequencerScene::keepSeqSoundRectInArea() {
    
    for ( int i = 0; i < seqSoundRect.size(); i++ ) {

        float minPosY = (visibleSize.height * kSequencer_MinYpos_height_multiplier) + origin.y;
        if ( seqSoundRect[i].sprite->getPositionY() <= minPosY ) {
            seqSoundRect[i].sprite->setPositionY( minPosY );
            seqSoundRect[i].updateBorder();
        }
        
        if ( seqSoundRect[i].sprite->getPositionX() - (seqSoundRect[i].sprite->getBoundingBox().size.width/2.0f) <= mainMenu->getPlayHeadStartPosX() ) {
            seqSoundRect[i].sprite->setPositionX( mainMenu->getPlayHeadStartPosX() + (seqSoundRect[i].sprite->getBoundingBox().size.width/2.0f) + 1 );
            seqSoundRect[i].updateBorder();
        }
        
        float maxPosY = (visibleSize.height - (seqSoundRect[i].sprite->getBoundingBox().size.height/2.0f) ) + origin.y;
        if ( seqSoundRect[i].sprite->getPositionY() >=  maxPosY ) {
            seqSoundRect[i].sprite->setPositionY( maxPosY );
            seqSoundRect[i].updateBorder();
        }
        
        seqSoundRect[i].touchArea->setPosition( seqSoundRect[i].sprite->getPosition() );
        seqSoundRect[i].poff->setPosition( seqSoundRect[i].sprite->getPosition() );
        seqSoundRect[i].setCollisionSpritePos();
    }
}

void SequencerScene::setPlayHeadHandlePos() {
    playHeadHandle->setPosition( Vec2( playHead->getPosition().x, playHead->getPosition().y + (playHead->getBoundingBox().size.height/2.0f) - (playHeadHandle->getBoundingBox().size.height/2.0f) ) );
}
