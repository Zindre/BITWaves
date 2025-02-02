#include "InstrumentScene.h"

USING_NS_CC;

Scene* InstrumentScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = InstrumentScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool InstrumentScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    this->setName( "mainLayer" );
    
    safeAreaRect = Director::getInstance()->getSafeAreaRect().size;
    safeAreaOrigin = Director::getInstance()->getSafeAreaRect().origin;
    
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2( InstrumentScene::onTouchesBegan, this );
    listener->onTouchesMoved = CC_CALLBACK_2( InstrumentScene::onTouchesMoved, this );
    listener->onTouchesEnded = CC_CALLBACK_2( InstrumentScene::onTouchesEnded, this );
    dispatcher->addEventListenerWithSceneGraphPriority( listener, this );
    
    cocos2d::Sprite *background = Sprite::create( "square1px.png" );
    background->setTextureRect( Rect( 0, 0, safeAreaRect.width, safeAreaRect.height ) );
    background->setPosition( Vec2( safeAreaOrigin.x + safeAreaRect.width * 0.5, safeAreaOrigin.y + safeAreaRect.height * 0.5 ) );
    background->setColor( Color3B( 15, 15, 15 ) );
    this->addChild( background, kLayer_GrayBackground );

    
    for ( int i = 0; i < kTouch_NumOfTouchesAllowed; i++ ) {
        playingSoundObject[i] = 0;
    }


    projectHandling = new ProjectHandling( this );
    mainMenu = new MainMenu( this, kScene_Instrument );
    
    recordTimer = 0.0f;
    
    waveFormRect = DrawNode::create();
    this->addChild( waveFormRect, kLayer_RecordWaveForm );
    waveFormPosX = 0.0f;
    clearWaveFormTimer = 0.0f;
    bClearWaveForm = false;
    hasUpdatedWaveSprite = false;
    recIsLocked = true;
    recIsFinalizing = false;
    hasLoadedSoundFiles = false;
    
    mainMenu->buttons_image[kButtons_ArrayNum_Rec]->setOpacity( 100 );
    
    
    this->scheduleUpdate();
    
    return true;
}

void InstrumentScene::update( float dt ) {
    
    //log( "---update start---" );
    
    FMODAudioEngine::update();
    
    for ( int i = 0; i < circleEmitter.size(); i++ ) {
        
        circleEmitter[i].update( dt );
        
        if ( circleEmitter[i].destroy() ) {
            //this->removeChild( circleEmitter[i].id_label );
            circleEmitter.erase( circleEmitter.begin() + i );
        }
        
    }

    
    if ( FMODAudioEngine::isRecording() ) {
        
        recordTimer += dt;
    
        drawWaveForm();

        if ( recordTimer >= kRecordMaxLengthSeconds ) {
            stopRecording();
        }
    }
    
    if ( bClearWaveForm ) {
        clearWaveForm( dt );
    }
    
    
    if ( ! hasLoadedSoundFiles && FMODAudioEngine::systemIsInitialized() ) {
        for ( int i = 0; i < kNumOfSoundObjects; i++ ) {
            std::string currentProjectName = UserDefault::getInstance()->getStringForKey( "currentProjectName" );
            FMODAudioEngine::loadSoundFromDisk( currentProjectName, i );
        }
        hasLoadedSoundFiles = true;
    }

    
    if ( projectHandling->getState() == kProjectHandling_State_SaveOverlay ) {
        if ( projectHandling->textField_save->getCharCount() == 0 ) {
            if ( projectHandling->buttonBg[kButtons_ProjectHandling_Index_ConfirmSave]->getOpacity() == 255 ) {
                projectHandling->buttonBg[kButtons_ProjectHandling_Index_ConfirmSave]->setOpacity( kProjectHandling_Button_TransparantValue );
            }
        } else {
            if ( projectHandling->buttonBg[kButtons_ProjectHandling_Index_ConfirmSave]->getOpacity() == 100 ) {
                projectHandling->buttonBg[kButtons_ProjectHandling_Index_ConfirmSave]->setOpacity( 255 );
            }
        }
    }
    
    
    
}

void InstrumentScene::onTouchesBegan( const std::vector<Touch*>& touches, Event* event ) {
    

    
    for ( auto &touch : touches ) {
        
        if ( touch != nullptr ) {
            
            mainMenu->setStartPos( touch->getLocation() );
            
            if ( ! mainMenu->helpOverlayIsVisible ) {
                
                // PROJECT HANDLING
                if ( projectHandling->isShowing() ) {
                    
                    
                    projectHandling->setTouchStartPos( touch->getLocation() );
                    
                    // CLOSE CROSS
                    if ( projectHandling->closeCross->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                        projectHandling->hide();
                    }
                    
                    
                    // MAIN SCREEN //
                    if ( projectHandling->getState() == kProjectHandling_State_MainScreen ) {
                        
                        
                        // SAVE
                        if ( projectHandling->savingIsPossible() ) {
                            if ( projectHandling->buttonBg[kButtons_ProjectHandling_Index_Save]->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                                projectHandling->setButtonTouchHasBegun( true, kButtons_ProjectHandling_Index_Save );
                            }
                        }
                        
                        // BROWSE
                        if ( projectHandling->buttonBg[kButtons_ProjectHandling_Index_Browse]->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                            projectHandling->setButtonTouchHasBegun( true, kButtons_ProjectHandling_Index_Browse );
                            
                        }
                        
                        // NEW
                        if ( projectHandling->buttonBg[kButtons_ProjectHandling_Index_New]->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                            projectHandling->setButtonTouchHasBegun( true, kButtons_ProjectHandling_Index_New );
                            
                        }
                        
                        
                    // SAVE OVERLAY
                    } else if ( projectHandling->getState() == kProjectHandling_State_SaveOverlay ) {
                        
                        // TEXT FIELD
                        if ( projectHandling->textFieldArea->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                            projectHandling->openKeyboard_save();
                        }
                        
                        // CONFIRM SAVE
                        if ( projectHandling->textField_save->getCharCount() != 0 ) {
                            if ( projectHandling->buttonBg[kButtons_ProjectHandling_Index_ConfirmSave]->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                                projectHandling->setButtonTouchHasBegun( true, kButtons_ProjectHandling_Index_ConfirmSave );
                            }
                        }
                        
                        // CANCEL BUTTON
                        if ( projectHandling->buttonBg[kButtons_ProjectHandling_Index_CancelSave]->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                            projectHandling->setButtonTouchHasBegun( true, kButtons_ProjectHandling_Index_CancelSave );
                        }
                       
                        
                    // BROWSE OVERLAY
                    } else if ( projectHandling->getState() == kProjectHandling_State_BrowseOverlay ) {
                        
                        for ( int i = 0; i < projectHandling->projectNamesLabel.size(); i++ ) {
                            
                            // CHOOSE PROJECT
                            if ( projectHandling->projectNamesLabel[i].label->isVisible() ) {
                                if ( projectHandling->projectNamesLabel[i].squareBg->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                                    for ( int j = 0; j < projectHandling->projectNamesLabel.size(); j++ ) {
                                        projectHandling->projectNamesLabel[j].squareBg->setColor( Color3B( kProjectHandling_Browse_FileListBgColor, kProjectHandling_Browse_FileListBgColor, kProjectHandling_Browse_FileListBgColor ) );
                                    }
                                    projectHandling->projectNamesLabel[i].squareBg->setColor( Color3B( 0, 0, 0 ) );
                                    projectHandling->setSelectedProjectName( projectHandling->projectNamesLabel[i].getFullString() );
                                    std::string currentProjectName = UserDefault::getInstance()->getStringForKey( "currentProjectName" );
                                    if ( projectHandling->getSelectedProjectName().compare( currentProjectName ) == 0 ) {
                                        projectHandling->buttonBg[kButtons_ProjectHandling_Index_Open]->setOpacity( kProjectHandling_Button_TransparantValue );
                                        projectHandling->buttonBg[kButtons_ProjectHandling_Index_Delete]->setOpacity( kProjectHandling_Button_TransparantValue );
                                        projectHandling->buttonBg[kButtons_ProjectHandling_Index_Rename]->setOpacity( kProjectHandling_Button_TransparantValue );
                                        projectHandling->setAprojectIsSelected( false );
                                    } else {
                                        projectHandling->setAprojectIsSelected( true );
                                        if ( projectHandling->aProjectIsSelected() ) {
                                            projectHandling->buttonBg[kButtons_ProjectHandling_Index_Open]->setOpacity( 255 );
                                            projectHandling->buttonBg[kButtons_ProjectHandling_Index_Delete]->setOpacity( 255 );
                                            projectHandling->buttonBg[kButtons_ProjectHandling_Index_Rename]->setOpacity( 255 );
                                        } else {
                                            projectHandling->buttonBg[kButtons_ProjectHandling_Index_Open]->setOpacity( kProjectHandling_Button_TransparantValue );
                                            projectHandling->buttonBg[kButtons_ProjectHandling_Index_Delete]->setOpacity( kProjectHandling_Button_TransparantValue );
                                            projectHandling->buttonBg[kButtons_ProjectHandling_Index_Rename]->setOpacity( kProjectHandling_Button_TransparantValue );
                                        }
                                    }
                                    
                                }
                            }
                            
                        }
                        
                        // OPEN
                        if ( projectHandling->aProjectIsSelected() ) {
                            if ( projectHandling->buttonBg[kButtons_ProjectHandling_Index_Open]->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                                projectHandling->setButtonTouchHasBegun( true, kButtons_ProjectHandling_Index_Open );
                            }
                        }
                        
                        // DELETE
                        if ( projectHandling->aProjectIsSelected() ) {
                            if ( projectHandling->buttonBg[kButtons_ProjectHandling_Index_Delete]->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                                projectHandling->setButtonTouchHasBegun( true, kButtons_ProjectHandling_Index_Delete );
                            }
                        }
                        
                        // RENAME
                        if ( projectHandling->aProjectIsSelected() ) {
                            if ( projectHandling->buttonBg[kButtons_ProjectHandling_Index_Rename]->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                                projectHandling->setButtonTouchHasBegun( true, kButtons_ProjectHandling_Index_Rename );
                            }
                        }
                        
                        // CANCEL
                        if ( projectHandling->buttonBg[kButtons_ProjectHandling_Index_CancelBrowse]->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                            projectHandling->setButtonTouchHasBegun( true, kButtons_ProjectHandling_Index_CancelBrowse );
                        }
                        
                        // ARROW LEFT
                        if ( projectHandling->arrowLeft->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                            projectHandling->arrowLeftClicked();
                        }
                        
                        
                        // ARROW RIGHT
                        if ( projectHandling->arrowRight->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                            projectHandling->arrowRightClicked();
                        }
                        
                        
                        
                    // DELETE PROMPT
                    } else if ( projectHandling->getState() == kProjectHandling_State_DeletePrompt ) {
                        
                        // CONFIRM DELETE
                        if ( projectHandling->buttonBg[kButtons_ProjectHandling_Index_ConfirmDelete]->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                            projectHandling->setButtonTouchHasBegun( true, kButtons_ProjectHandling_Index_ConfirmDelete );
                        }
                        
                        // CANCEL DELETE
                        if ( projectHandling->buttonBg[kButtons_ProjectHandling_Index_CancelDelete]->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                            projectHandling->setButtonTouchHasBegun( true, kButtons_ProjectHandling_Index_CancelDelete );
                        }
                    
                        
                    } else if ( projectHandling->getState() == kProjectHandling_State_BrowseOverlay_Rename ) {
                        
                        // OPEN RENAME TEXT FIELD
                        if ( projectHandling->renameTextFieldBg->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                            projectHandling->openKeyboard_rename();
                        }
                        
                        // CANCEL RENAME BUTTON
                        if ( projectHandling->buttonBg[kButtons_ProjectHandling_Index_CancelRename]->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                            projectHandling->setButtonTouchHasBegun( true, kButtons_ProjectHandling_Index_CancelRename );
                        }
                        
                        // CONFIRM RENAME BUTTON
                        if ( projectHandling->buttonBg[kButtons_ProjectHandling_Index_ConfirmRename]->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                            projectHandling->setButtonTouchHasBegun( true, kButtons_ProjectHandling_Index_ConfirmRename );
                        }
                        
                    }
                    
                    if ( projectHandling->getState() == kProjectHandling_State_NameExistPrompt_Save || projectHandling->getState() == kProjectHandling_State_NameExistPrompt_Rename ) {
                        
                        // CLOSE NAME EXISTS PROMPT
                        if ( projectHandling->buttonBg[kButtons_ProjectHandling_Index_CloseExistPrompt]->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                            projectHandling->setButtonTouchHasBegun( true, kButtons_ProjectHandling_Index_CloseExistPrompt );
                        }
                        
                    }
                    
                    
                    
                } else {
                    
                    if ( mainMenu->instrumentArea->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                        
                        // INSTRUMENT AREA
                        if ( ! FMODAudioEngine::isRecording() ) {
                            
                            if ( circleEmitter.size() < kNumOfChannels ) {
                                
                                playingSoundObject[touch->getID()] = mainMenu->getActiveSoundObject();
                                
                                if ( FMODAudioEngine::hasRecordWav( playingSoundObject[touch->getID()] ) ) {
                                    circleEmitter.push_back( CircleEmitter( this, touch->getID(), touch->getLocation(), playingSoundObject[touch->getID()] ) );
                                }
                                
                            }
                            
                        }
                        
                    } else {
                        
                        // MAIN MENU
                        if ( touch->getID() == 0 ) {
                            
                            if ( ! FMODAudioEngine::isRecording() ) {
                                
                                if ( ! recIsLocked ) {
                                    if ( mainMenu->buttons_image[kButtons_ArrayNum_Rec]->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                                        mainMenu->setTouchHasBegun( true, kButtons_ArrayNum_Rec );
                                        mainMenu->buttons_image[kButtons_ArrayNum_Rec]->setScale( kButtons_ScaleValue );
                                    }
                                } else {
                                    if ( mainMenu->buttons_image[kButtons_ArrayNum_Rec]->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                                        auto goYellow = TintTo::create( 0.001f, 255, 255, 0 );
                                        auto goWhite = TintTo::create( 1.0f, 255, 255, 255 );
                                        auto seq = Sequence::create( goYellow, goWhite, NULL );
                                        mainMenu->buttons_image[kButtons_ArrayNum_Lock]->runAction( seq );
                                    }
                                }
                                
                                
                                if ( ! recIsFinalizing ) {
                                    
                                    if ( mainMenu->buttons_image[kButtons_ArrayNum_Seq]->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                                        mainMenu->buttons_image[kButtons_ArrayNum_Seq]->setScale( kButtons_ScaleValue );
                                        mainMenu->setTouchHasBegun( true, kButtons_ArrayNum_Seq );
                                    }
                                    
                                    if ( mainMenu->buttons_image[kButtons_ArrayNum_Help]->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                                        mainMenu->buttons_image[kButtons_ArrayNum_Help]->setScale( kButtons_ScaleValue );
                                        mainMenu->setTouchHasBegun( true, kButtons_ArrayNum_Help );
                                    }
                                
                                    if ( mainMenu->buttons_image[kButtons_ArrayNum_Lock]->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                                        mainMenu->buttons_image[kButtons_ArrayNum_Lock]->setScale( kButtons_ScaleValue );
                                        mainMenu->setTouchHasBegun( true, kButtons_ArrayNum_Lock );
                                    }
                                    
                                    if ( mainMenu->buttons_image[kButtons_ArrayNum_Projects]->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                                        if ( CC_TARGET_PLATFORM == CC_PLATFORM_IOS ) {
                                            if (projectHandlingIsActivatedInIosPreferences()) {
                                                mainMenu->buttons_image[kButtons_ArrayNum_Projects]->setScale(
                                                        kButtons_ScaleValue);
                                                mainMenu->setTouchHasBegun(true,
                                                                           kButtons_ArrayNum_Projects);
                                            }
                                        } else {
                                            mainMenu->buttons_image[kButtons_ArrayNum_Projects]->setScale(
                                                    kButtons_ScaleValue);
                                            mainMenu->setTouchHasBegun(true,
                                                                       kButtons_ArrayNum_Projects);
                                        }
                                    }
                                    
                                }
                                
                            }
                            
                            
                            if ( FMODAudioEngine::isRecording() ) {
                                if ( mainMenu->buttons_image[kButtons_ArrayNum_Stop]->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                                    if ( recordTimer > kMinimumRecordTime ) {
                                        mainMenu->setTouchHasBegun( true, kButtons_ArrayNum_Stop );
                                        mainMenu->buttons_image[kButtons_ArrayNum_Stop]->setScale( kButtons_ScaleValue );
                                    }
                                }
                            }
                            
                            
                        }
                        
                        if ( ! FMODAudioEngine::isRecording() ) {
                            if ( ! recIsFinalizing ) {
                                for ( int i = 0; i < kNumOfSoundObjects; i++ ) {
                                    if ( mainMenu->soundSquare[i]->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                                        mainMenu->setActiveSoundObject( i );
                                    }
                                }
                            }
                        }
                        
                        
                        
                    }
                }
            
                
            }
        }
    }
    
    
    
}

void InstrumentScene::onTouchesMoved( const std::vector<Touch*> &touches, Event* event ) {
    
    for ( auto touch : touches ) {
        if ( touch != nullptr ) {
            
            if ( ! FMODAudioEngine::isRecording() ) {
                mainMenu->abortWithTouchMove( touch->getLocation() );
            }
            
            if ( ! mainMenu->helpOverlayIsVisible ) {
            
                
                // PROJECT HANDLING
                if ( projectHandling->isShowing() ) {
                    
                    projectHandling->abortWithTouchMove( touch->getLocation() );
                    
                } else {
                    
                    if ( mainMenu->instrumentArea->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                        
                        // INSTRUMENT AREA
                        
                        if ( ! FMODAudioEngine::isRecording() ) {
                            
                            if ( FMODAudioEngine::hasRecordWav( playingSoundObject[touch->getID()] ) ) {

                                for ( int i = 0; i < circleEmitter.size(); i++ ) {
                                    if ( ! circleEmitter[i].touchHasEnded() ) {
                                        if ( circleEmitter[i].getTouchID() == touch->getID() ) {
                                            circleEmitter[i].setPos( touch->getLocation() );
                                            circleEmitter[i].setPitch( touch->getLocation() );
                                        }
                                    }
                                }
                            }
                        }
                        
                    } else {
                        // MAIN MENU
                    }
                    
                }
                
                
                
                
                
                
            }
            
            
        }
    }
}

void InstrumentScene::onTouchesEnded( const std::vector<Touch*> &touches, Event* event ) {

    for ( auto touch : touches ) {
        if ( touch != nullptr ) {
            
            if ( ! mainMenu->helpOverlayIsVisible ) {
                
                // PROJECT HANDLING
                
                if ( projectHandling->isShowing() ) {
                    
                    // MAIN SCREEN
                    if ( projectHandling->getState() == kProjectHandling_State_MainScreen ) {
                        
                        // SAVE
                        if ( projectHandling->buttonTouchHasBegun( kButtons_ProjectHandling_Index_Save ) ) {
                            projectHandling->showSaveOverlay();
                        }
                        
                        // BROWSE
                        if ( projectHandling->buttonTouchHasBegun( kButtons_ProjectHandling_Index_Browse ) ) {
                            projectHandling->showBrowseOverlay();
                        }
                        
                        // NEW
                        if ( projectHandling->buttonTouchHasBegun( kButtons_ProjectHandling_Index_New ) ) {
                            projectHandling->createNewProject();
                            UserDefault::getInstance()->setStringForKey( "currentProjectName", "Uten tittel" );
                            UserDefault::getInstance()->flush();
                            mainMenu->updateCurrentProjectNameLabel( "Uten tittel" );
                            auto scene = InstrumentScene::createScene();
                            Director::getInstance()->replaceScene( scene );
                        }
                        
                    // SAVE OVERLAY
                    } else if ( projectHandling->getState() == kProjectHandling_State_SaveOverlay ) {
                        
                        // CONFIRM SAVE
                        if ( projectHandling->buttonTouchHasBegun( kButtons_ProjectHandling_Index_ConfirmSave ) ) {
                            
                            projectHandling->checkIfNameExists( projectHandling->getTextField_save_string(), kProjectHandling_State_SaveOverlay );
                            projectHandling->setState( kProjectHandling_State_NameExistPrompt_Save );
                            
                            if ( ! projectHandling->nameExists() ) {
                                projectHandling->saveNewProject();
                                UserDefault::getInstance()->setStringForKey( "currentProjectName", projectHandling->getTextField_save_string() );
                                UserDefault::getInstance()->flush();
                                mainMenu->updateCurrentProjectNameLabel( projectHandling->getTextField_save_string() );
                                projectHandling->closeSaveOverlay();
                            }
                            
                        }
                        
                        // CANCEL BUTTON
                        if ( projectHandling->buttonTouchHasBegun( kButtons_ProjectHandling_Index_CancelSave ) ) {
                            projectHandling->cancelSaveOverlay();
                        }
                        
                    // BROWSE OVERLAY
                    } else if ( projectHandling->getState() == kProjectHandling_State_BrowseOverlay ) {
                        
                        // OPEN
                        if ( projectHandling->buttonTouchHasBegun( kButtons_ProjectHandling_Index_Open ) ) {
                            projectHandling->loadSavedProject();
                            UserDefault::getInstance()->setStringForKey( "currentProjectName", projectHandling->getSelectedProjectName() );
                            UserDefault::getInstance()->flush();
                            mainMenu->updateCurrentProjectNameLabel( projectHandling->getSelectedProjectName()  );
                            auto scene = InstrumentScene::createScene();
                            Director::getInstance()->replaceScene( scene );
                        }
                        
                        // DELETE
                        if ( projectHandling->buttonTouchHasBegun( kButtons_ProjectHandling_Index_Delete ) ) {
                            projectHandling->showDeletePrompt( projectHandling->getSelectedProjectName() );
                        }
                        
                        // RENAME
                        if ( projectHandling->buttonTouchHasBegun( kButtons_ProjectHandling_Index_Rename ) ) {
                            projectHandling->showRenameTextField( projectHandling->getSelectedProjectName() );
                        }
                        
                        // CANCEL
                        if ( projectHandling->buttonTouchHasBegun( kButtons_ProjectHandling_Index_CancelBrowse ) ) {
                            projectHandling->cancelBrowseOverlay();
                        }
                        
                    } else if ( projectHandling->getState() == kProjectHandling_State_DeletePrompt ) {
                        
                        // CONFIRM DELETE
                        if ( projectHandling->buttonTouchHasBegun( kButtons_ProjectHandling_Index_ConfirmDelete ) ) {
                            std::string currentProjectName = UserDefault::getInstance()->getStringForKey( "currentProjectName" );
                            projectHandling->deleteProject( projectHandling->getSelectedProjectName(), currentProjectName );
                        }
                        
                        // CANCEL DELETE
                        if ( projectHandling->buttonTouchHasBegun( kButtons_ProjectHandling_Index_CancelDelete ) ) {
                            projectHandling->closeDeletePrompt();
                        }
                        
                    } else if ( projectHandling->getState() == kProjectHandling_State_BrowseOverlay_Rename ) {
                        
                        // CANCEL RENAME BUTTON
                        if ( projectHandling->buttonTouchHasBegun( kButtons_ProjectHandling_Index_CancelRename ) ) {
                            projectHandling->cancelRename();
                        }
                        
                        // CONFIRM RENAME BUTTON
                        if ( projectHandling->buttonTouchHasBegun( kButtons_ProjectHandling_Index_ConfirmRename ) ) {
                            
                            projectHandling->checkIfNameExists( projectHandling->getTextField_rename_string(), kProjectHandling_State_BrowseOverlay_Rename );
                            projectHandling->setState( kProjectHandling_State_NameExistPrompt_Rename );
                            
                            if ( ! projectHandling->nameExists() ) {
                                projectHandling->renameProject( projectHandling->getSelectedProjectName() );
                            }
                            
                        }
                        
                    } else if ( projectHandling->getState() == kProjectHandling_State_NameExistPrompt_Save ) {
                        
                        // CLOSE NAME EXIST PROMPT SAVE
                        if ( projectHandling->buttonTouchHasBegun( kButtons_ProjectHandling_Index_CloseExistPrompt ) ) {
                            projectHandling->closeNameExistsPrompt_save();
                        }
                        
                    } else if ( projectHandling->getState() == kProjectHandling_State_NameExistPrompt_Rename ) {
                        
                        // CLOSE NAME EXIST PROMPT RENAME
                        if ( projectHandling->buttonTouchHasBegun( kButtons_ProjectHandling_Index_CloseExistPrompt ) ) {
                            projectHandling->closeNameExistsPrompt_rename();
                        }
                        
                    }
                    
                    
                    // Touch bagan to false
                    for ( int i = 0; i < kButtons_ProjectHandling_NumOf; i++ ) {
                        projectHandling->setButtonTouchHasBegun( false, i );
                    }
                    
                } else {
                    
                    
                    // MAIN MENU
                    if ( touch->getID() == 0 ) {
                        
                        if ( mainMenu->buttons_image[kButtons_ArrayNum_Rec]->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                            if ( ! FMODAudioEngine::isRecording() ) {
                                if ( mainMenu->getTouchHasBegun( kButtons_ArrayNum_Rec ) ) {
                                    mainMenu->buttons_image[kButtons_ArrayNum_Rec]->setColor( Color3B::RED );
                                    std::string currentProjectName = UserDefault::getInstance()->getStringForKey( "currentProjectName" );
                                    FMODAudioEngine::recordStart( currentProjectName, mainMenu->getActiveSoundObject() );
                                    mainMenu->buttons_image[kButtons_ArrayNum_Seq]->setColor( Color3B( kButtons_GrayedOutValue, kButtons_GrayedOutValue, kButtons_GrayedOutValue ) );
                                    mainMenu->buttons_image[kButtons_ArrayNum_Help]->setColor( Color3B( kButtons_GrayedOutValue, kButtons_GrayedOutValue, kButtons_GrayedOutValue ) );
                                    mainMenu->buttons_image[kButtons_ArrayNum_Projects]->setColor( Color3B( kButtons_GrayedOutValue, kButtons_GrayedOutValue, kButtons_GrayedOutValue ) );
                                    mainMenu->buttons_image[kButtons_ArrayNum_Lock]->setColor( Color3B( kButtons_GrayedOutValue, kButtons_GrayedOutValue, kButtons_GrayedOutValue ) );
                                    mainMenu->buttons_image[kButtons_ArrayNum_Stop]->setVisible( true );
                                }
                            }
                        }
                        
                        
                        if ( ! FMODAudioEngine::isRecording() && !mainMenu->buttons_image[kButtons_ArrayNum_Rec]->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                            mainMenu->buttons_image[kButtons_ArrayNum_Rec]->setScale( 1.0f );
                        }
                        
                        
                        if ( FMODAudioEngine::isRecording() ) {
                            if ( mainMenu->buttons_image[kButtons_ArrayNum_Stop]->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                                if ( mainMenu->getTouchHasBegun( kButtons_ArrayNum_Stop ) ) {
                                    stopRecording();
                                }
                            }
                        }

                    
                        if ( ! FMODAudioEngine::isRecording() ) {
                            if ( mainMenu->buttons_image[kButtons_ArrayNum_Seq]->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                                if ( mainMenu->getTouchHasBegun( kButtons_ArrayNum_Seq ) ) {
                                    auto scene = SequencerScene::createScene();
                                    Director::getInstance()->replaceScene( scene );
                                }
                            }
                            
                            if ( mainMenu->buttons_image[kButtons_ArrayNum_Help]->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                                if ( mainMenu->getTouchHasBegun( kButtons_ArrayNum_Help ) ) {
                                    mainMenu->helpOverlayIsVisible = true;
                                    //mainMenu->helpOverlay->setVisible( true );
                                    mainMenu->helpOverlay->show();
                                }
                            }
                            
                            if ( mainMenu->buttons_image[kButtons_ArrayNum_Lock]->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                                if ( mainMenu->getTouchHasBegun( kButtons_ArrayNum_Lock ) ) {
                                    if ( ! recIsLocked ) {
                                        recIsLocked = true;
                                        mainMenu->buttons_image[kButtons_ArrayNum_Lock]->setTexture( "lockButton.png" );
                                        mainMenu->buttons_image[kButtons_ArrayNum_Rec]->setOpacity( kProjectHandling_Button_TransparantValue );
                                    } else {
                                        recIsLocked = false;
                                        mainMenu->buttons_image[kButtons_ArrayNum_Lock]->setTexture( "unLockButton.png" );
                                        mainMenu->buttons_image[kButtons_ArrayNum_Rec]->setOpacity( 255 );
                                    }
                                }
                            }
                            
                            if ( mainMenu->buttons_image[kButtons_ArrayNum_Projects]->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                                if ( mainMenu->getTouchHasBegun( kButtons_ArrayNum_Projects ) ) {
                                    if ( ! projectHandling->isShowing() ) {
                                        projectHandling->show();
                                    }
                                }
                            }
                            
                            
                            
                        }

                    }
                    
                    
                    mainMenu->buttons_image[kButtons_ArrayNum_Stop]->setScale( 1.0f );
                    mainMenu->buttons_image[kButtons_ArrayNum_Seq]->setScale( 1.0f );
                    mainMenu->buttons_image[kButtons_ArrayNum_Help]->setScale( 1.0f );
                    mainMenu->buttons_image[kButtons_ArrayNum_Lock]->setScale( 1.0f );
                    mainMenu->buttons_image[kButtons_ArrayNum_Projects]->setScale( 1.0f );
                    mainMenu->setTouchHasBegun( false, kButtons_ArrayNum_Rec );
                    mainMenu->setTouchHasBegun( false, kButtons_ArrayNum_Stop );
                    mainMenu->setTouchHasBegun( false, kButtons_ArrayNum_Seq );
                    mainMenu->setTouchHasBegun( false, kButtons_ArrayNum_Help );
                    mainMenu->setTouchHasBegun( false, kButtons_ArrayNum_Lock );
                    mainMenu->setTouchHasBegun( false, kButtons_ArrayNum_Projects );
                    
                    for ( int i = 0; i < circleEmitter.size(); i++ ) {
                        if ( circleEmitter[i].getTouchID() == touch->getID() ) {
                            circleEmitter[i].setTouchHasEnded( true );
                            circleEmitter[i].fadeOut();
                        }
                    }
                    
                    
            
                }
                
            } else {
                
                
                if ( mainMenu->helpOverlay->webLinkBg->getBoundingBox().containsPoint( touch->getLocation() ) ) {
                    // Web Link
                    mainMenu->helpOverlay->openWebLink();
                } else {
                    mainMenu->helpOverlayIsVisible = false;
                    //mainMenu->helpOverlay->setVisible( false );
                    mainMenu->helpOverlay->hide();
                    if ( ! UserDefault::getInstance()->getBoolForKey( "helpOverlayHasShownOnFirstBoot" ) ) {
                        UserDefault::getInstance()->setBoolForKey( "helpOverlayHasShownOnFirstBoot", true );
                        UserDefault::getInstance()->flush();
                    }
                }
                
            }
            
            
        }
    }
}

void InstrumentScene::onTouchesCancelled( const std::vector<Touch*>& touches, Event  *event ) {
    onTouchesEnded( touches, event );
}

void InstrumentScene::stopRecording() {
    
    captureWaveform();
    
    FMODAudioEngine::recordStop( mainMenu->getActiveSoundObject() );
    
    for ( int i = 0; i < kNumOfSoundObjects; i++ ) {
        std::string currentProjectName = UserDefault::getInstance()->getStringForKey( "currentProjectName" );
        FMODAudioEngine::loadSoundFromDisk( currentProjectName, i );
    }
    
    recordTimer = 0;
    
    mainMenu->buttons_image[kButtons_ArrayNum_Rec]->setColor( Color3B::WHITE );
    mainMenu->buttons_image[kButtons_ArrayNum_Seq]->setColor( Color3B::WHITE );
    mainMenu->buttons_image[kButtons_ArrayNum_Help]->setColor( Color3B::WHITE );
    mainMenu->buttons_image[kButtons_ArrayNum_Projects]->setColor( Color3B::WHITE );
    mainMenu->buttons_image[kButtons_ArrayNum_Lock]->setColor( Color3B::WHITE );
    mainMenu->buttons_image[kButtons_ArrayNum_Stop]->setVisible( false );
    mainMenu->buttons_image[kButtons_ArrayNum_Rec]->setScale( 1.0f );
    
    recIsLocked = true;
    mainMenu->buttons_image[kButtons_ArrayNum_Lock]->setTexture( "lockButton.png" );
    mainMenu->buttons_image[kButtons_ArrayNum_Rec]->setOpacity( 100 );
    
    recIsFinalizing = true;
    
}

void InstrumentScene::drawWaveForm() {
    float startX = mainMenu->getPlayHeadStartPosX();
    waveFormPosX = scaleValue( recordTimer, 0.0f, kRecordMaxLengthSeconds, startX, mainMenu->instrumentArea->getBoundingBox().getMaxX(), true );
    
    float recordWaveForm_height_multiplier = scaleValue( FMODAudioEngine::getSpectrum( FMODAudioEngine::getRecPlayChannel() ), kSpectrum_MinValue, kSpectrum_MaxValue, 0.001f, kRecordWaveForm_height_multiplier_MAX, true );
    float posY = mainMenu->buttons_gray[kButtons_ArrayNum_Rec]->getPosition().y;
    float rectWidth = safeAreaRect.width * 0.008f;
    
    Vec2 originPos = Vec2( waveFormPosX - rectWidth, posY + safeAreaRect.height * recordWaveForm_height_multiplier );
    Vec2 destinationPos = Vec2( waveFormPosX, posY - safeAreaRect.height * recordWaveForm_height_multiplier );
    
    waveFormRect->drawSolidRect( originPos, destinationPos, Color4F( Color3B(mainMenu->soundSquare[mainMenu->getActiveSoundObject()]->getColor()) ) );
}

void InstrumentScene::captureWaveform() {
    
    FileUtils *fileUtils = FileUtils::getInstance();
    std::string writablePath = fileUtils->getWritablePath();
    std::string imageFile = "waveForm" + to_string( mainMenu->getActiveSoundObject() ) + ".png";
    std::string currentProjectName = UserDefault::getInstance()->getStringForKey( "currentProjectName" );
    std::string projectFolder = currentProjectName;
    std::string imageFileInProjectFolder = projectFolder + "/" + imageFile;
    std::string imageFileFullPath = writablePath + projectFolder + "/" + imageFile;
    log( "capture wave form - image file full path: %s", imageFileFullPath.c_str() );
    if ( fileUtils->isFileExist( imageFileFullPath ) ) {
        Director::getInstance()->getTextureCache()->removeTexture( Sprite::create( imageFileFullPath )->getTexture() );
        fileUtils->removeFile( imageFileFullPath );
    }

    
    Size designSize = Director::getInstance()->getWinSize();
    Size winPixelSize = Director::getInstance()->getWinSizeInPixels();
    float startX = mainMenu->getPlayHeadStartPosX();
    float captureRectHeight = mainMenu->buttons_gray[kButtons_ArrayNum_Rec]->getBoundingBox().size.height;
    
    //RenderTexture *rend = RenderTexture::create( waveFormPosX - startX, ((safeAreaRect.height * kRecordWaveForm_height_multiplier_MAX) * 2) + safeAreaOrigin.y, backend::PixelFormat::RGBA8888 );
    RenderTexture *rend = RenderTexture::create( waveFormPosX - startX, captureRectHeight, backend::PixelFormat::RGBA8888 );
    rend->retain();
    rend->setKeepMatrix( true );

    //rend->setVirtualViewport( Vec2( startX, mainMenu->buttons_gray[kButtons_ArrayNum_Rec]->getPosition().y - (safeAreaRect.height * kRecordWaveForm_height_multiplier_MAX) ), Rect( 0, 0, designSize.width, designSize.height ), Rect( 0, 0, winPixelSize.width, winPixelSize.height ) );
    rend->setVirtualViewport( Vec2( startX, mainMenu->buttons_gray[kButtons_ArrayNum_Rec]->getPosition().y - (captureRectHeight/2) ), Rect( 0, 0, designSize.width, designSize.height ), Rect( 0, 0, winPixelSize.width, winPixelSize.height ) );
    
    rend->beginWithClear( 0.0f, 0.0f, 0.0f, 0.0f );
    auto renderer = Director::getInstance()->getRenderer();
    waveFormRect->visit( renderer, this->getNodeToParentTransform(), 0 );
    rend->end();
    
    rend->saveToFile( imageFileInProjectFolder );
    

    hasUpdatedWaveSprite = false;
    bClearWaveForm = true;
}

void InstrumentScene::clearWaveForm( float dt ) {
    
    clearWaveFormTimer += dt;
    
    if ( clearWaveFormTimer > 1.0f ) {
        
        if ( ! hasUpdatedWaveSprite ) {
            log( "updating wave sprite" );
            FileUtils *fileUtils = FileUtils::getInstance();
            std::string writablePath = fileUtils->getWritablePath();
            std::string currentProjectName = UserDefault::getInstance()->getStringForKey( "currentProjectName" );
            std::string projectFolder = currentProjectName;
            std::string imageFileFullPath = writablePath + projectFolder + "/" + "waveForm" + to_string( mainMenu->getActiveSoundObject() ) + ".png";
            log( "clear wave form - image file full path: %s", imageFileFullPath.c_str() );
            
            mainMenu->waveForm[mainMenu->getActiveSoundObject()]->setTexture( imageFileFullPath );
            mainMenu->waveForm[mainMenu->getActiveSoundObject()]->setTextureRect( Rect( 0, 0,  mainMenu->waveForm[mainMenu->getActiveSoundObject()]->getContentSize().width,  mainMenu->waveForm[mainMenu->getActiveSoundObject()]->getContentSize().height ) );
            float scaleSize = mainMenu->soundSquare[mainMenu->getActiveSoundObject()]->getBoundingBox().size.width / mainMenu->getInstrumentAreaWidth();
            mainMenu->waveForm[mainMenu->getActiveSoundObject()]->setScale( scaleSize, scaleSize * 4.0f );
            mainMenu->waveForm[mainMenu->getActiveSoundObject()]->setColor( mainMenu->soundSquare[mainMenu->getActiveSoundObject()]->getColor() );
            
            hasUpdatedWaveSprite = true;
        }

    }
    
    if ( clearWaveFormTimer > 1.5f ) {
        
        waveFormRect->clear();
        
        clearWaveFormTimer = 0.0f;
        bClearWaveForm = false;
        
        recIsFinalizing = false;
        
    }
}
