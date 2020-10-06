#include "ProjectHandling.hpp"
#include <iomanip>

ProjectHandling::ProjectHandling( Layer *layer ) {
    
    this->_layer = layer;
    
    visibleSize = Director::getInstance()->getSafeAreaRect().size;
    origin = Director::getInstance()->getSafeAreaRect().origin;
    
    _whatState = kProjectHandling_State_Closed;
    
    blackLayer = Sprite::create( "square1px.png" );
    blackLayer->setTextureRect( Rect( 0, 0, visibleSize.width, visibleSize.height ) );
    blackLayer->setPosition( Vec2( visibleSize.width * 0.5 + origin.x, visibleSize.height * 0.5 + origin.y ) );
    blackLayer->setColor( Color3B::BLACK );
    blackLayer->setOpacity( 200 );
    layer->addChild( blackLayer, kLayer_ProjectHandling );
    
    background = Sprite::create( "square1px.png" );
    background->setTextureRect( Rect( 0, 0, visibleSize.width, visibleSize.height ) );
    background->setPosition( Vec2( visibleSize.width * 0.5 + origin.x, visibleSize.height * 0.5 + origin.y ) );
    layer->addChild( background, kLayer_ProjectHandling );
    
    closeCross = Sprite::create( "closeCross.png" );
    closeCross->setAnchorPoint( Vec2( 0, 1 ) );
    closeCross->setPosition( Vec2( origin.x + (closeCross->getBoundingBox().size.width * 0.5), visibleSize.height - (closeCross->getBoundingBox().size.height * 0.5) + origin.y ) );
    layer->addChild( closeCross, kLayer_ProjectHandling );
    
    for ( int i = 0; i < kButtons_ProjectHandling_NumOf; i++ ) {
        buttonBg[i] = Sprite::create( "buttonBg.png" );
        buttonBg[i]->setColor( Color3B::BLACK );
        padding = buttonBg[i]->getBoundingBox().size.height * 0.5;
        layer->addChild( buttonBg[i], kLayer_ProjectHandling );
        
        label_buttons[i] = Label::createWithTTF( "button", "fonts/arial.ttf", kProjectHandling_FontSize_Buttons );
        label_buttons[i]->setColor( Color3B::WHITE );
        label_buttons[i]->setPosition( buttonBg[i]->getPosition() );
        layer->addChild( label_buttons[i], kLayer_ProjectHandling );
    }
    label_buttons[kButtons_ProjectHandling_Index_Save]->setString( "Lagre" );
    label_buttons[kButtons_ProjectHandling_Index_Browse]->setString( "Bla gjennom" );
    label_buttons[kButtons_ProjectHandling_Index_New]->setString( "Nytt prosjekt" );
    label_buttons[kButtons_ProjectHandling_Index_ConfirmSave]->setString( "Bekreft Lagre" );
    label_buttons[kButtons_ProjectHandling_Index_Open]->setString( "Åpne" );
    label_buttons[kButtons_ProjectHandling_Index_Cancel]->setString( "Avbryt" );
    label_buttons[kButtons_ProjectHandling_Index_Delete]->setString( "Slett" );
    label_buttons[kButtons_ProjectHandling_Index_ConfirmDelete]->setString( "Ja" );
    label_buttons[kButtons_ProjectHandling_Index_CancelDelete]->setString( "Avbryt" );
    
    
    overlaySave = Sprite::create( "square1px.png" );
    overlaySave->setTextureRect( Rect( 0, 0, background->getBoundingBox().size.width * 0.8, background->getBoundingBox().size.height * 0.7 ) );
    overlaySave->setPosition( Vec2( origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.5 ) );
    overlaySave->setColor( Color3B::GRAY );
    overlaySave->setVisible( false );
    layer->addChild( overlaySave, kLayer_ProjectHandling_SaveOverlay );
    
    overlayBrowse = Sprite::create( "square1px.png" );
    overlayBrowse->setTextureRect( Rect( 0, 0, background->getBoundingBox().size.width * 0.8, background->getBoundingBox().size.height * 0.8 ) );
    overlayBrowse->setPosition( Vec2( origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.5 ) );
    overlayBrowse->setColor( Color3B::GRAY );
    overlayBrowse->setVisible( false );
    layer->addChild( overlayBrowse, kLayer_ProjectHandling_BrowseOverlay );
    
    label_instructTyping = Label::createWithTTF( "Gi prosjektet et navn:", "fonts/arial.ttf", kProjectHandling_FontSize_Text );
    label_instructTyping->setPosition( Vec2( overlaySave->getPosition().x, overlaySave->getPosition().y + (overlaySave->getBoundingBox().size.height * 0.3) ) );
    label_instructTyping->setVisible( false );
    label_instructTyping->setColor( Color3B::BLACK );
    layer->addChild( label_instructTyping, kLayer_ProjectHandling_SaveOverlay );
    
    textFieldArea = Sprite::create( "square1px.png" );
    textFieldArea->setTextureRect( Rect( 0, 0, overlayBrowse->getBoundingBox().size.width * 0.8, label_instructTyping->getBoundingBox().size.height * 1.2 ) );
    textFieldArea->setPosition( Vec2( label_instructTyping->getPosition().x, label_instructTyping->getPosition().y - (label_instructTyping->getBoundingBox().size.height * 2) ) );
    textFieldArea->setVisible( false );
    layer->addChild( textFieldArea, kLayer_ProjectHandling_SaveOverlay );
    
    textField = cocos2d::TextFieldTTF::textFieldWithPlaceHolder( "", "fonts/arial.ttf", kProjectHandling_FontSize_Text );
    textField->setPosition( Vec2( label_instructTyping->getPosition().x, label_instructTyping->getPosition().y - (label_instructTyping->getBoundingBox().size.height * 2) ) );
    textField->setVisible( false );
    textField->setCursorEnabled( true );
    textField->setTextColor( Color4B( 0, 0, 0, 255 ) );
    layer->addChild( textField, kLayer_ProjectHandling_SaveOverlay );
    
    cocos2d::Size buttonBgSize = buttonBg[0]->getBoundingBox().size;
    
    buttonBg[kButtons_ProjectHandling_Index_Browse]->setPosition( Vec2( overlayBrowse->getPosition().x, overlayBrowse->getPosition().y ) );
    label_buttons[kButtons_ProjectHandling_Index_Browse]->setPosition( buttonBg[kButtons_ProjectHandling_Index_Browse]->getPosition() );
    
    buttonBg[kButtons_ProjectHandling_Index_Save]->setPosition( Vec2( buttonBg[kButtons_ProjectHandling_Index_Browse]->getPosition().x, buttonBg[kButtons_ProjectHandling_Index_Browse]->getPosition().y + (padding * 3) ) );
    label_buttons[kButtons_ProjectHandling_Index_Save]->setPosition( buttonBg[kButtons_ProjectHandling_Index_Save]->getPosition() );
    
    buttonBg[kButtons_ProjectHandling_Index_New]->setPosition( Vec2( buttonBg[kButtons_ProjectHandling_Index_Browse]->getPosition().x, buttonBg[kButtons_ProjectHandling_Index_Browse]->getPosition().y - (padding * 3) ) );
    label_buttons[kButtons_ProjectHandling_Index_New]->setPosition( buttonBg[kButtons_ProjectHandling_Index_New]->getPosition() );
    
    buttonBg[kButtons_ProjectHandling_Index_ConfirmSave]->setPosition( Vec2( overlaySave->getPosition().x - (buttonBgSize.width * 0.5) - padding, overlaySave->getPosition().y - buttonBgSize.height ) );
    label_buttons[kButtons_ProjectHandling_Index_ConfirmSave]->setPosition( buttonBg[kButtons_ProjectHandling_Index_ConfirmSave]->getPosition() );
    buttonBg[kButtons_ProjectHandling_Index_ConfirmSave]->setLocalZOrder( kLayer_ProjectHandling_SaveOverlay );
    label_buttons[kButtons_ProjectHandling_Index_ConfirmSave]->setLocalZOrder( kLayer_ProjectHandling_SaveOverlay );
    
    buttonBg[kButtons_ProjectHandling_Index_Open]->setPosition( Vec2( overlayBrowse->getPosition().x + buttonBgSize.width, overlayBrowse->getPosition().y + (buttonBgSize.height * 2) ) );
    label_buttons[kButtons_ProjectHandling_Index_Open]->setPosition( buttonBg[kButtons_ProjectHandling_Index_Open]->getPosition() );
    buttonBg[kButtons_ProjectHandling_Index_Open]->setLocalZOrder( kLayer_ProjectHandling_BrowseOverlay );
    label_buttons[kButtons_ProjectHandling_Index_Open]->setLocalZOrder( kLayer_ProjectHandling_BrowseOverlay );
    buttonBg[kButtons_ProjectHandling_Index_Open]->setOpacity( kProjectHandling_Button_TransparantValue );
    
    buttonBg[kButtons_ProjectHandling_Index_Delete]->setPosition( Vec2( overlayBrowse->getPosition().x + buttonBgSize.width, overlayBrowse->getPosition().y ) );
    label_buttons[kButtons_ProjectHandling_Index_Delete]->setPosition( buttonBg[kButtons_ProjectHandling_Index_Delete]->getPosition() );
    buttonBg[kButtons_ProjectHandling_Index_Delete]->setLocalZOrder( kLayer_ProjectHandling_BrowseOverlay );
    label_buttons[kButtons_ProjectHandling_Index_Delete]->setLocalZOrder( kLayer_ProjectHandling_BrowseOverlay );
    buttonBg[kButtons_ProjectHandling_Index_Delete]->setOpacity( kProjectHandling_Button_TransparantValue );
    
    buttonBg[kButtons_ProjectHandling_Index_Cancel]->setPosition( Vec2( overlayBrowse->getPosition().x + buttonBgSize.width, overlayBrowse->getPosition().y - (buttonBgSize.height * 2) ) );
    label_buttons[kButtons_ProjectHandling_Index_Cancel]->setPosition( buttonBg[kButtons_ProjectHandling_Index_Cancel]->getPosition() );
    buttonBg[kButtons_ProjectHandling_Index_Cancel]->setLocalZOrder( kLayer_ProjectHandling_BrowseOverlay );
    label_buttons[kButtons_ProjectHandling_Index_Cancel]->setLocalZOrder( kLayer_ProjectHandling_BrowseOverlay );
    
    
    cocos2d::Size overlayBrowseSize = overlayBrowse->getBoundingBox().size;
    
    arrowLeft = Sprite::create( "arrow.png" );
    arrowLeft->setFlippedX( true );
    arrowLeft->setAnchorPoint( Vec2( 0.0, 0.0 ) );
    arrowLeft->setPosition( Vec2( overlayBrowse->getPosition().x - (overlayBrowseSize.width * 0.5), overlayBrowse->getPosition().y - (overlayBrowseSize.height * 0.5) ) );
    arrowLeft->setVisible( false );
    layer->addChild( arrowLeft, kLayer_ProjectHandling_BrowseOverlay );
    
    arrowRight = Sprite::create( "arrow.png" );
    arrowRight->setAnchorPoint( Vec2( 0.5, 0.0 ) );
    arrowRight->setPosition( Vec2( overlayBrowse->getPosition().x, overlayBrowse->getPosition().y - (overlayBrowseSize.height * 0.5) ) );
    arrowRight->setVisible( false );
    layer->addChild( arrowRight, kLayer_ProjectHandling_BrowseOverlay );
    
    label_projectNamesPageNr = Label::createWithTTF( "1/1", "fonts/arial.ttf", kProjectHandling_FontSize_Text );
    label_projectNamesPageNr->setAnchorPoint( Vec2( 0.5, 0.5 ) );
    label_projectNamesPageNr->setPosition( Vec2( arrowLeft->getPosition().x + overlayBrowseSize.width * 0.25, arrowLeft->getPosition().y + (arrowLeft->getBoundingBox().size.height * 0.5) ) );
    label_projectNamesPageNr->setVisible( false );
    label_projectNamesPageNr->setColor( Color3B::BLACK );
    layer->addChild( label_projectNamesPageNr, kLayer_ProjectHandling_BrowseOverlay );
    
    
    overlayDeletePrompt = Sprite::create( "square1px.png" );
    overlayDeletePrompt->setTextureRect( Rect( 0, 0, overlayBrowse->getBoundingBox().size.width * 0.8, overlayBrowse->getBoundingBox().size.height * 0.8 ) );
    overlayDeletePrompt->setPosition( Vec2( origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.5 ) );
    overlayDeletePrompt->setColor( Color3B::WHITE );
    overlayDeletePrompt->setVisible( false );
    layer->addChild( overlayDeletePrompt, kLayer_ProjectHandling_DeletePrompt );
    
    label_deletePrompt_areYouSure = Label::createWithTTF( "Er du sikker på du vil slette?", "fonts/arial.ttf", kProjectHandling_FontSize_Text );
    label_deletePrompt_areYouSure->setPosition( Vec2( overlayDeletePrompt->getPosition().x, overlayDeletePrompt->getPosition().y + (overlayDeletePrompt->getBoundingBox().size.height * 0.25 ) ) );
    label_deletePrompt_areYouSure->setVisible( false );
    label_deletePrompt_areYouSure->setColor( Color3B::BLACK );
    label_deletePrompt_areYouSure->setAlignment( TextHAlignment::CENTER );
    layer->addChild( label_deletePrompt_areYouSure, kLayer_ProjectHandling_DeletePrompt );
    
    buttonBg[kButtons_ProjectHandling_Index_ConfirmDelete]->setPosition( Vec2( overlayDeletePrompt->getPosition().x - (buttonBgSize.width * 0.6), overlayDeletePrompt->getPosition().y - (buttonBgSize.height * 0.5) ) );
    label_buttons[kButtons_ProjectHandling_Index_ConfirmDelete]->setPosition( buttonBg[kButtons_ProjectHandling_Index_ConfirmDelete]->getPosition() );
    buttonBg[kButtons_ProjectHandling_Index_ConfirmDelete]->setLocalZOrder( kLayer_ProjectHandling_DeletePrompt );
    label_buttons[kButtons_ProjectHandling_Index_ConfirmDelete]->setLocalZOrder( kLayer_ProjectHandling_DeletePrompt );
    
    buttonBg[kButtons_ProjectHandling_Index_CancelDelete]->setPosition( Vec2( overlayDeletePrompt->getPosition().x + (buttonBgSize.width * 0.6), overlayDeletePrompt->getPosition().y - (buttonBgSize.height * 0.5) ) );
    label_buttons[kButtons_ProjectHandling_Index_CancelDelete]->setPosition( buttonBg[kButtons_ProjectHandling_Index_CancelDelete]->getPosition() );
    buttonBg[kButtons_ProjectHandling_Index_CancelDelete]->setLocalZOrder( kLayer_ProjectHandling_DeletePrompt );
    label_buttons[kButtons_ProjectHandling_Index_CancelDelete]->setLocalZOrder( kLayer_ProjectHandling_DeletePrompt );
    
    
    // SCROLL BOX
    /*scrollViewSize = cocos2d::Size( visibleSize.width * 0.5f, visibleSize.height * 0.7f );
    
    scrollView = ui::ScrollView::create();
    scrollView->setContentSize( scrollViewSize );
    scrollView->setDirection( ui::ScrollView::Direction::VERTICAL );
    scrollView->setAnchorPoint( Vec2( 0.5f, 0.5f ) );
    scrollView->setPosition( Vec2( visibleSize.width * 0.3 + origin.x, visibleSize.height * 0.5 + origin.y ) );
    scrollView->setBounceEnabled( true );
    scrollView->setScrollBarAutoHideEnabled( false );
    scrollView->setScrollBarPositionFromCornerForVertical( Vec2( scrollView->getScrollBarWidth() * 2, scrollView->getScrollBarWidth() * 2 ) );
    scrollView->setScrollBarColor( Color3B( 255, 255, 255 ) );
    scrollView->setScrollBarOpacity( 255 );
    scrollView->setBackGroundColorType( cocos2d::ui::HBox::BackGroundColorType::SOLID );
    scrollView->setBackGroundColor( Color3B( 50, 50, 50 ) );
    //scrollView->setBackGroundColorOpacity( 0 );
    scrollView->setVisible( false );
    scrollView->setName( "projectsScrollView" );
    layer->addChild( scrollView, kLayer_ProjectHandling_BrowseOverlay );*/
    
    
    
    
    
    
    
    updateProjectList();
    
    
    
    selectedProjectName = "";
    _aProjectIsSelected = false;
    _isShowing = false;
    _currentPageNr = 1;
    _totalNrOfPages = 1;
    for ( int i = 0; i < kButtons_ProjectHandling_NumOf; i++ ) {
        _buttonTouchHasBegun[i] = false;
    }
    _touchStartPos = Vec2( 0.0, 0.0 );
    
    
    loadCurrentData();
    
    
    std::string currentProjectName = UserDefault::getInstance()->getStringForKey( "currentProjectName" );
    if ( currentProjectName == "Uten tittel" ) {
        _savingIsPossible = true;
    } else {
        _savingIsPossible = false;
        buttonBg[kButtons_ProjectHandling_Index_Save]->setOpacity( kProjectHandling_Button_TransparantValue );
        saveCurrentToOpenProject();
    }
    
    
    hide();
    
    
}


void ProjectHandling::show() {
    blackLayer->setVisible( true );
    background->setVisible( true );
    closeCross->setVisible( true );
    for ( int i = 0; i < kButtons_ProjectHandling_NumOf; i++ ) {
        buttonBg[i]->setVisible( true );
        label_buttons[i]->setVisible( true );
    }
    buttonBg[kButtons_ProjectHandling_Index_ConfirmSave]->setVisible( false );
    label_buttons[kButtons_ProjectHandling_Index_ConfirmSave]->setVisible( false );
    buttonBg[kButtons_ProjectHandling_Index_Open]->setVisible( false );
    label_buttons[kButtons_ProjectHandling_Index_Open]->setVisible( false );
    buttonBg[kButtons_ProjectHandling_Index_Cancel]->setVisible( false );
    label_buttons[kButtons_ProjectHandling_Index_Cancel]->setVisible( false );
    buttonBg[kButtons_ProjectHandling_Index_Delete]->setVisible( false );
    label_buttons[kButtons_ProjectHandling_Index_Delete]->setVisible( false );
    buttonBg[kButtons_ProjectHandling_Index_ConfirmDelete]->setVisible( false );
    label_buttons[kButtons_ProjectHandling_Index_ConfirmDelete]->setVisible( false );
    buttonBg[kButtons_ProjectHandling_Index_CancelDelete]->setVisible( false );
    label_buttons[kButtons_ProjectHandling_Index_CancelDelete]->setVisible( false );
    _whatState = kProjectHandling_State_MainScreen;
    _isShowing = true;
}

void ProjectHandling::hide() {
    blackLayer->setVisible( false );
    background->setVisible( false );
    closeCross->setVisible( false );
    for ( int i = 0; i < kButtons_ProjectHandling_NumOf; i++ ) {
        buttonBg[i]->setVisible( false );
        label_buttons[i]->setVisible( false );
    }
    for ( int i = 0; i < projectNamesLabel.size(); i++ ) {
        projectNamesLabel[i].hide();
    }
    overlayBrowse->setVisible( false );
    overlaySave->setVisible( false );
    selectedProjectName = "";
    _aProjectIsSelected = false;
    label_instructTyping->setVisible( false );
    textField->setVisible( false );
    textFieldArea->setVisible( false );
    _whatState = kProjectHandling_State_Closed;
    _isShowing = false;
    arrowLeft->setVisible( false );
    arrowRight->setVisible( false );
    label_projectNamesPageNr->setVisible( false );
}

void ProjectHandling::loadCurrentData() {
    
    // ----------------------------------------------------------------------------------
    Data data_pos_X = UserDefault::getInstance()->getDataForKey( "current_posX" );
    float*  buffer_X = (float*) data_pos_X.getBytes();
    ssize_t length_X = data_pos_X.getSize() / sizeof(float);
    
    std::ostringstream ss_X;
    ss_X << std::setprecision(2) << std::fixed;
    for (int i = 0; i < length_X; i++) {
        ss_X << buffer_X[i] << " ";
    }
    CCLOG("PH init load %s is %s", "current_posX", ss_X.str().c_str());
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
    CCLOG("PH init load %s is %s", "current_posY", ss_Y.str().c_str());
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
    log( "PH init load %s is %s", "current_whatSound", ss_whatSound.str().c_str() );
    // ----------------------------------------------------------------------------------
    
    
    for ( int i = 0; i < length_X; i++ ) {
        currentPos.push_back( Vec2( buffer_X[i], buffer_Y[i] ) );
    }
    
    for ( int i = 0; i < length_whatSound; i++ ) {
        currentWhatSound.push_back( buffer_whatSound[i] );
    }

}

void ProjectHandling::saveNewProject() {
    
    /*std::string newFileName = textField->getString();
    
    for ( int i = 0; i < savedProjectNames.size(); i++ ) {
        if ( savedProjectNames[i].compare( textField->getString() ) == 0 ) {
            log( "filename exist!" );
            newFileName = textField->getString() + " 1";
        }
    }*/
    
    if ( currentPos.size() != 0 ) {
        
        // ----------------------------------------------------------------------------------
        Data data_pos_X;
        std::vector<float> pos_X;
        
        for ( int i = 0; i < currentPos.size(); i++ ) {
            pos_X.push_back( currentPos[i].x );
        }
        
        for ( int i = 0; i < pos_X.size(); i++ ) {
            log( "PH save posX[%i]: %f", i, pos_X[i] );
        }
        
        data_pos_X.copy((unsigned char*) pos_X.data(), pos_X.size() * sizeof(float));
        std::string projectName_X = textField->getString() + "_" + "pos_X";
        log( "PH save project name X: %s", projectName_X.c_str() );
        UserDefault::getInstance()->deleteValueForKey( projectName_X.c_str() );
        UserDefault::getInstance()->setDataForKey( projectName_X.c_str(), data_pos_X );
        // ----------------------------------------------------------------------------------
        
        
        // ----------------------------------------------------------------------------------
        Data data_pos_Y;
        std::vector<float> pos_Y;
        
        for ( int i = 0; i < currentPos.size(); i++ ) {
            pos_Y.push_back( currentPos[i].y );
        }
        
        for ( int i = 0; i < pos_Y.size(); i++ ) {
            log( "PH save posY[%i]: %f", i, pos_Y[i] );
        }
        
        data_pos_Y.copy((unsigned char*) pos_Y.data(), pos_Y.size() * sizeof(float));
        std::string projectName_Y = textField->getString() + "_" + "pos_Y";
        log( "PH save project name Y: %s", projectName_Y.c_str() );
        UserDefault::getInstance()->deleteValueForKey( projectName_Y.c_str() );
        UserDefault::getInstance()->setDataForKey( projectName_Y.c_str(), data_pos_Y );
        // ----------------------------------------------------------------------------------
        
        
        // ----------------------------------------------------------------------------------
        Data data_whatSound;
        std::vector<int> whatSound;
        
        for ( int i = 0; i < currentWhatSound.size(); i++ ) {
            whatSound.push_back( currentWhatSound[i] );
        }
        
        for ( int i = 0; i < currentWhatSound.size(); i++ ) {
            log( "PH save what sound: %d", currentWhatSound[i] );
        }
        
        data_whatSound.copy((unsigned char*) whatSound.data(), whatSound.size() * sizeof(int));
        std::string projectName_whatSound = textField->getString() + "_" + "whatSound";
        log( "PH save project name whatSound: %s", projectName_whatSound.c_str() );
        UserDefault::getInstance()->deleteValueForKey( projectName_whatSound.c_str() );
        UserDefault::getInstance()->setDataForKey( projectName_whatSound.c_str(), data_whatSound );
        // ----------------------------------------------------------------------------------
        
    }
    
    FileUtils *fileUtils = FileUtils::getInstance();
    std::string writablePath = fileUtils->getWritablePath();
    if ( fileUtils->isDirectoryExist( writablePath + "Uten tittel" ) ) {
        fileUtils->renameFile( writablePath, "Uten tittel", textField->getString() );
    }
    
    
}

void ProjectHandling::saveCurrentToOpenProject() {
    
    if ( currentPos.size() != 0 ) {
        
        std::string currentProjectName = UserDefault::getInstance()->getStringForKey( "currentProjectName" );
        
        // ----------------------------------------------------------------------------------
        Data data_pos_X;
        std::vector<float> pos_X;
        
        for ( int i = 0; i < currentPos.size(); i++ ) {
            pos_X.push_back( currentPos[i].x );
        }
        
        for ( int i = 0; i < pos_X.size(); i++ ) {
            log( "PH save posX[%i]: %f", i, pos_X[i] );
        }
        
        data_pos_X.copy((unsigned char*) pos_X.data(), pos_X.size() * sizeof(float));
        std::string projectName_X = currentProjectName + "_" + "pos_X";
        log( "PH save current to open project X: %s", projectName_X.c_str() );
        UserDefault::getInstance()->deleteValueForKey( projectName_X.c_str() );
        UserDefault::getInstance()->setDataForKey( projectName_X.c_str(), data_pos_X );
        // ----------------------------------------------------------------------------------
        
        
        // ----------------------------------------------------------------------------------
        Data data_pos_Y;
        std::vector<float> pos_Y;
        
        for ( int i = 0; i < currentPos.size(); i++ ) {
            pos_Y.push_back( currentPos[i].y );
        }
        
        for ( int i = 0; i < pos_Y.size(); i++ ) {
            log( "PH save posY[%i]: %f", i, pos_Y[i] );
        }
        
        data_pos_Y.copy((unsigned char*) pos_Y.data(), pos_Y.size() * sizeof(float));
        std::string projectName_Y = currentProjectName + "_" + "pos_Y";
        log( "PH save current to project Y: %s", projectName_Y.c_str() );
        UserDefault::getInstance()->deleteValueForKey( projectName_Y.c_str() );
        UserDefault::getInstance()->setDataForKey( projectName_Y.c_str(), data_pos_Y );
        // ----------------------------------------------------------------------------------
        
        
        // ----------------------------------------------------------------------------------
        Data data_whatSound;
        std::vector<int> whatSound;
        
        for ( int i = 0; i < currentWhatSound.size(); i++ ) {
            whatSound.push_back( currentWhatSound[i] );
        }
        
        for ( int i = 0; i < currentWhatSound.size(); i++ ) {
            log( "PH save what sound: %d", currentWhatSound[i] );
        }
        
        data_whatSound.copy((unsigned char*) whatSound.data(), whatSound.size() * sizeof(int));
        std::string projectName_whatSound = currentProjectName + "_" + "whatSound";
        log( "PH save current to project whatSound: %s", projectName_whatSound.c_str() );
        UserDefault::getInstance()->deleteValueForKey( projectName_whatSound.c_str() );
        UserDefault::getInstance()->setDataForKey( projectName_whatSound.c_str(), data_whatSound );
        // ----------------------------------------------------------------------------------
        
    }
    
}

void ProjectHandling::loadSavedProject() {
    
    UserDefault::getInstance()->deleteValueForKey( "current_posX" );
    UserDefault::getInstance()->deleteValueForKey( "current_posY" );
    UserDefault::getInstance()->deleteValueForKey( "current_whatSound" );
    
    currentPos.clear();
    currentWhatSound.clear();
    
    
    // ----------------------------------------------------------------------------------
    std::string projectName_X = selectedProjectName + "_" + "pos_X";
    log( "load saved project name (posX): %s", projectName_X.c_str() );
    Data data_pos_X = UserDefault::getInstance()->getDataForKey( projectName_X.c_str() );
    float*  buffer_X = (float*) data_pos_X.getBytes();
    ssize_t length_X = data_pos_X.getSize() / sizeof(float);
    
    std::ostringstream ss_X;
    ss_X << std::setprecision(2) << std::fixed;
    for (int i = 0; i < length_X; i++) {
        ss_X << buffer_X[i] << " ";
    }
    CCLOG("PH load %s is %s", "pos_X", ss_X.str().c_str());
    // ----------------------------------------------------------------------------------
    
    
    // ----------------------------------------------------------------------------------
    std::string projectName_Y = selectedProjectName + "_" + "pos_Y";
    log( "load saved project name (posY): %s", projectName_Y.c_str() );
    Data data_pos_Y = UserDefault::getInstance()->getDataForKey( projectName_Y.c_str() );
    float*  buffer_Y = (float*) data_pos_Y.getBytes();
    ssize_t length_Y = data_pos_Y.getSize() / sizeof(float);
    
    std::ostringstream ss_Y;
    ss_Y << std::setprecision(2) << std::fixed;
    for (int i = 0; i < length_Y; i++) {
        ss_Y << buffer_Y[i] << " ";
    }
    CCLOG("PH load %s is %s", "pos_Y", ss_Y.str().c_str());
    // ----------------------------------------------------------------------------------
    
    
    // ----------------------------------------------------------------------------------
    std::string projectName_whatSound = selectedProjectName + "_" + "whatSound";
    log( "load saved project name (whatSound): %s", projectName_whatSound.c_str() );
    Data data_whatSound = UserDefault::getInstance()->getDataForKey( projectName_whatSound.c_str() );
    int* buffer_whatSound = (int*) data_whatSound.getBytes();
    ssize_t length_whatSound = data_whatSound.getSize() / sizeof(int);
    
    std::ostringstream ss_whatSound;
    ss_whatSound << std::setprecision(2) << std::fixed;
    for ( int i = 0; i < length_whatSound; i++ ) {
        ss_whatSound << buffer_whatSound[i] << " ";
    }
    log( "PH load %s is %s", "current_whatSound", ss_whatSound.str().c_str() );
    // ----------------------------------------------------------------------------------
    

    
    for ( int i = 0; i < length_X; i++ ) {
        currentPos.push_back( Vec2( buffer_X[i], buffer_Y[i] ) );
    }
    
    for ( int i = 0; i < length_whatSound; i++ ) {
        currentWhatSound.push_back( buffer_whatSound[i] );
    }
    
    for ( int i = 0; i < currentPos.size(); i++ ) {
        log( "PH current pos X: %f", currentPos[i].x );
        log( "PH current pos Y: %f", currentPos[i].y );
    }
    
    for ( int i = 0; i < currentWhatSound.size(); i++ ) {
        log( "PH current what sound: %i", currentWhatSound[i] );
    }
    
    if ( currentPos.size() != 0 ) {
        Data data_current_pos_X;
        std::vector<float> pos_X;
        for ( int i = 0; i < currentPos.size(); i++ ) {
            pos_X.push_back( currentPos[i].x );
        }
        data_current_pos_X.copy((unsigned char*) pos_X.data(), pos_X.size() * sizeof(float));
        UserDefault::getInstance()->setDataForKey( "current_posX", data_current_pos_X );
        
        
        Data data_current_pos_Y;
        std::vector<float> pos_Y;
        for ( int i = 0; i < currentPos.size(); i++ ) {
            pos_Y.push_back( currentPos[i].y );
        }
        data_current_pos_Y.copy((unsigned char*) pos_Y.data(), pos_Y.size() * sizeof(float));
        UserDefault::getInstance()->setDataForKey( "current_posY", data_current_pos_Y );
        
        
        Data data_current_whatSound;
        std::vector<int> whatSound;
        for ( int i = 0; i < currentWhatSound.size(); i++ ) {
            whatSound.push_back( currentWhatSound[i] );
        }
        data_current_whatSound.copy((unsigned char*) whatSound.data(), whatSound.size() * sizeof(int));
        UserDefault::getInstance()->setDataForKey( "current_whatSound", data_current_whatSound );
    }
    
}

std::string ProjectHandling::getTextFieldString() {
    return textField->getString();
}

void ProjectHandling::createNewProject() {
    
    FileUtils *fileUtils = FileUtils::getInstance();
    std::string writablePath = fileUtils->getWritablePath();
    std::string standardFolder = writablePath + "Uten tittel";
    
    if ( fileUtils->isDirectoryExist( standardFolder ) ) {
        fileUtils->removeDirectory( standardFolder );
        fileUtils->createDirectory( standardFolder );
    }
    
    UserDefault::getInstance()->deleteValueForKey( "current_posX" );
    UserDefault::getInstance()->deleteValueForKey( "current_posY" );
    UserDefault::getInstance()->deleteValueForKey( "current_whatSound" );
    
}

void ProjectHandling::showSaveOverlay() {
    overlaySave->setVisible( true );
    textField->setVisible( true );
    label_instructTyping->setVisible( true );
    textField->attachWithIME();
    buttonBg[kButtons_ProjectHandling_Index_ConfirmSave]->setVisible( true );
    label_buttons[kButtons_ProjectHandling_Index_ConfirmSave]->setVisible( true );
    buttonBg[kButtons_ProjectHandling_Index_Cancel]->setVisible( true );
    label_buttons[kButtons_ProjectHandling_Index_Cancel]->setVisible( true );
    buttonBg[kButtons_ProjectHandling_Index_Cancel]->setPosition( Vec2( overlaySave->getPosition().x + (buttonBg[0]->getBoundingBox().size.width * 0.5) + padding, overlaySave->getPosition().y - buttonBg[0]->getBoundingBox().size.height  ) );
    label_buttons[kButtons_ProjectHandling_Index_Cancel]->setPosition( buttonBg[kButtons_ProjectHandling_Index_Cancel]->getPosition() );
    _whatState = kProjectHandling_State_SaveOverlay;
    textFieldArea->setVisible( true );
}

void ProjectHandling::showBrowseOverlay() {
    updateProjectList();
    overlayBrowse->setVisible( true );
    buttonBg[kButtons_ProjectHandling_Index_Open]->setVisible( true );
    label_buttons[kButtons_ProjectHandling_Index_Open]->setVisible( true );
    buttonBg[kButtons_ProjectHandling_Index_Cancel]->setVisible( true );
    label_buttons[kButtons_ProjectHandling_Index_Cancel]->setVisible( true );
    buttonBg[kButtons_ProjectHandling_Index_Delete]->setVisible( true );
    label_buttons[kButtons_ProjectHandling_Index_Delete]->setVisible( true );
    buttonBg[kButtons_ProjectHandling_Index_Cancel]->setPosition( Vec2( overlayBrowse->getPosition().x + buttonBg[kButtons_ProjectHandling_Index_Cancel]->getBoundingBox().size.width, overlayBrowse->getPosition().y - (buttonBg[kButtons_ProjectHandling_Index_Cancel]->getBoundingBox().size.height * 2) ) );
    label_buttons[kButtons_ProjectHandling_Index_Cancel]->setPosition( buttonBg[kButtons_ProjectHandling_Index_Cancel]->getPosition() );
    _whatState = kProjectHandling_State_BrowseOverlay;
    arrowLeft->setVisible( true );
    arrowRight->setVisible( true );
    label_projectNamesPageNr->setVisible( true );
    for ( int i = 0; i < projectNamesLabel.size(); i++ ) {
       projectNamesLabel[i].show();
    }
    decideWhichProjectNamesToShow();
}

void ProjectHandling::openKeyboard() {
    textField->attachWithIME();
}

void ProjectHandling::closeSaveOverlay() {
    overlaySave->setVisible( false );
    textField->setVisible( false );
    label_instructTyping->setVisible( false );
    buttonBg[kButtons_ProjectHandling_Index_ConfirmSave]->setVisible( false );
    label_buttons[kButtons_ProjectHandling_Index_ConfirmSave]->setVisible( false );
    buttonBg[kButtons_ProjectHandling_Index_Save]->setOpacity( kProjectHandling_Button_TransparantValue );
    _savingIsPossible = false;
    buttonBg[kButtons_ProjectHandling_Index_Cancel]->setVisible( false );
    label_buttons[kButtons_ProjectHandling_Index_Cancel]->setVisible( false );
    updateProjectList();
    _whatState = kProjectHandling_State_MainScreen;
    textFieldArea->setVisible( false );
}

void ProjectHandling::closeBrowseOverlay() {
    overlayBrowse->setVisible( false );
    for ( int i = 0; i < projectNamesLabel.size(); i++ ) {
       projectNamesLabel[i].hide();
    }
    buttonBg[kButtons_ProjectHandling_Index_Open]->setVisible( false );
    label_buttons[kButtons_ProjectHandling_Index_Open]->setVisible( false );
    buttonBg[kButtons_ProjectHandling_Index_Cancel]->setVisible( false );
    label_buttons[kButtons_ProjectHandling_Index_Cancel]->setVisible( false );
    buttonBg[kButtons_ProjectHandling_Index_Delete]->setVisible( false );
    label_buttons[kButtons_ProjectHandling_Index_Delete]->setVisible( false );
    _whatState = kProjectHandling_State_MainScreen;
    arrowLeft->setVisible( false );
    arrowRight->setVisible( false );
    label_projectNamesPageNr->setVisible( false );
}

void ProjectHandling::setSelectedProjectName( std::string projectName ) {
    selectedProjectName = projectName;
    log( "selected project: %s", selectedProjectName.c_str() );
}

std::string ProjectHandling::getSelectedProjectName( ){
    return selectedProjectName;
}

bool ProjectHandling::savingIsPossible() {
    return _savingIsPossible;
}

bool ProjectHandling::aProjectIsSelected() {
    return _aProjectIsSelected;
}

void ProjectHandling::setAprojectIsSelected( bool aProjectIsSelected ) {
    _aProjectIsSelected = aProjectIsSelected;
}

void ProjectHandling::updateProjectList() {
    
    savedProjectNames.clear();
    projectNamesLabel.clear();
    
    FileUtils *fileUtils = FileUtils::getInstance();
    std::string writablePath = fileUtils->getWritablePath();
    std::vector<std::string> savedProjectNamesFullPaths;
    savedProjectNamesFullPaths = fileUtils->listFiles( writablePath );
    
    
    // Look for existing files from previous version of app
    for ( int i = 0; i < savedProjectNamesFullPaths.size(); i++ ) {
        std::size_t foundWaveForm = savedProjectNamesFullPaths[i].find( "waveForm" );
        if ( foundWaveForm != std::string::npos ) {
            log( "found old waveFormX.png files" );
            std::string fromLastVerString = "Fra forrige versjon";
            std::string fromLastVerFullPath = writablePath + fromLastVerString;
            if ( ! fileUtils->isDirectoryExist( fromLastVerFullPath ) ) {
                fileUtils->createDirectory( fromLastVerFullPath );
            }
            //fileUtils->renameFile( savedProjectNamesFullPaths[i], writablePath + fromLastVerString + "/" + "waveForm" + to_string( i ) + ".png" );
        }
        
        std::size_t foundRecord = savedProjectNamesFullPaths[i].find( "record" );
        if ( foundRecord != std::string::npos ) {
            log( "found old recordX.wav files" );
        }
    }
    
    
    // Extract folder string from full path
    std::vector<std::string> tempStr;
    
    for ( int i = 0; i < savedProjectNamesFullPaths.size(); i++ ) {
        
        log( "saved projects full paths: %s", savedProjectNamesFullPaths[i].c_str() );
        
        char str[1024];
        strcpy( str, savedProjectNamesFullPaths[i].c_str() );
        
        // Returns first token
        char *token = strtok( str, "/" );

        // Keep printing tokens while one of the
        // delimiters present in str[].
        while ( token != NULL ) {
            log( "token: %s", token );
            tempStr.push_back( token );
            token = strtok( NULL, "/" );
        }
        
        // Add last token (the directory we want) of string path to vector array
        savedProjectNames.push_back( tempStr.back() );
        
    }
    
    log( "savedProjectNames.size() before erase: %lu", savedProjectNames.size() );
    for ( int i = 0; i < savedProjectNames.size(); i++ ) {
        log( "saved project names before erase: %s", savedProjectNames[i].c_str() );
    }
    
    // Remove unwanted dirs to show on list
    for ( int i = 0; i < savedProjectNames.size(); i++ ) {
        if ( savedProjectNames[i].compare( "Uten tittel" ) == 0 ) {
            log( "Uten tittel savedProjectNames gets ereased" );
            savedProjectNames.erase( savedProjectNames.begin() + i );
        }
        
        if ( savedProjectNames[i].compare( "." ) == 0 ) {
            savedProjectNames.erase( savedProjectNames.begin() + i );
        }
        
        if ( savedProjectNames[i].compare( ".." ) == 0 ) {
            savedProjectNames.erase( savedProjectNames.begin() + i );
        }
        
        if ( savedProjectNames[i].compare( ".Trash" ) == 0 ) {
            savedProjectNames.erase( savedProjectNames.begin() + i );
        }
    }
    
    log( "savedProjectNames.size() after: %lu", savedProjectNames.size() );
    for ( int i = 0; i < savedProjectNames.size(); i++ ) {
        log( "saved project names after: %s", savedProjectNames[i].c_str() );
    }
    
    
    // Sort alphabetically
    std::sort( savedProjectNames.begin(), savedProjectNames.end() );
    for ( int i = 0; i < savedProjectNames.size(); i++ ) {
        log( "saved project names alphabetically: %s", savedProjectNames[i].c_str() );
    }
    
    
    unsigned long amount = savedProjectNames.size();
    const int membersPrGroup = 8;
    int groupsFloored = floor( amount / membersPrGroup );
    int leftover = amount % membersPrGroup;
    int groups = 1;
    if ( leftover > 0 ) {
        groups = groupsFloored + 1;
    } else {
        groups = groupsFloored;
    }
    
    
    // Spread labels into pages
    log( "amount: %lu", amount );
    log( "members pr group: %i", membersPrGroup );
    log( "groups floored: %i", groupsFloored );
    log( "leftover: %i", leftover );
    log( "groups: %i", groups );
    
    _totalNrOfPages = groups;
    log( "total nr of pages: %i", _totalNrOfPages );
    label_projectNamesPageNr->setString( to_string( _currentPageNr ) + " / " + to_string( _totalNrOfPages ) );

    unsigned int myPageNr = 1;
    int valCounter = 0;
    
    for ( int i = 0; i < savedProjectNames.size(); i++ ) {
        
        if ( myPageNr <= groups ) {
            valCounter++;
            if ( valCounter > membersPrGroup ) {
                myPageNr++;
                valCounter = 1; // Reset to 1 to get the correct num of members on first page
            }
        }
        
        log( "val counter: %i", valCounter );
        log( "my page nr: %i", myPageNr );
        unsigned int pageIndex = valCounter - 1;
        projectNamesLabel.push_back( ProjectNamesLabel( _layer, savedProjectNames[i].c_str(), i, myPageNr, pageIndex, overlayBrowse->getBoundingBox().size, overlayBrowse->getPosition() ) );
    }
    


    
}

unsigned int ProjectHandling::getState() {
    return _whatState;
}

bool ProjectHandling::isShowing() {
    return _isShowing;
}

void ProjectHandling::cancelSaveOverlay() {
    
    overlaySave->setVisible( false );
    textField->setVisible( false );
    label_instructTyping->setVisible( false );
    buttonBg[kButtons_ProjectHandling_Index_ConfirmSave]->setVisible( false );
    label_buttons[kButtons_ProjectHandling_Index_ConfirmSave]->setVisible( false );
    buttonBg[kButtons_ProjectHandling_Index_Cancel]->setVisible( false );
    label_buttons[kButtons_ProjectHandling_Index_Cancel]->setVisible( false );
    _whatState = kProjectHandling_State_MainScreen;
    textFieldArea->setVisible( false );
}

void ProjectHandling::cancelBrowseOverlay() {
    setSelectedProjectName( "" );
    setAprojectIsSelected( false );
    closeBrowseOverlay();
    buttonBg[kButtons_ProjectHandling_Index_Open]->setOpacity( kProjectHandling_Button_TransparantValue );
    buttonBg[kButtons_ProjectHandling_Index_Delete]->setOpacity( kProjectHandling_Button_TransparantValue );
}

void ProjectHandling::arrowRightClicked() {
    if ( _currentPageNr < _totalNrOfPages ) {
        _currentPageNr++;
        label_projectNamesPageNr->setString( to_string( _currentPageNr ) + " / " + to_string( _totalNrOfPages ) );
        decideWhichProjectNamesToShow();
        setSelectedProjectName( "" );
        setAprojectIsSelected( false );
        buttonBg[kButtons_ProjectHandling_Index_Open]->setOpacity( kProjectHandling_Button_TransparantValue );
        buttonBg[kButtons_ProjectHandling_Index_Delete]->setOpacity( kProjectHandling_Button_TransparantValue );
        for ( int i = 0; i < projectNamesLabel.size(); i++ ) {
            projectNamesLabel[i].squareBg->setColor( Color3B( kProjectHandling_Browse_FileListBgColor, kProjectHandling_Browse_FileListBgColor, kProjectHandling_Browse_FileListBgColor ) );
        }
    }
}

void ProjectHandling::arrowLeftClicked() {
    if ( _currentPageNr > 1 ) {
        _currentPageNr--;
        label_projectNamesPageNr->setString( to_string( _currentPageNr ) + " / " + to_string( _totalNrOfPages ) );
        decideWhichProjectNamesToShow();
        setSelectedProjectName( "" );
        setAprojectIsSelected( false );
        buttonBg[kButtons_ProjectHandling_Index_Open]->setOpacity( kProjectHandling_Button_TransparantValue );
        buttonBg[kButtons_ProjectHandling_Index_Delete]->setOpacity( kProjectHandling_Button_TransparantValue );
        for ( int i = 0; i < projectNamesLabel.size(); i++ ) {
            projectNamesLabel[i].squareBg->setColor( Color3B( kProjectHandling_Browse_FileListBgColor, kProjectHandling_Browse_FileListBgColor, kProjectHandling_Browse_FileListBgColor ) );
        }
    }
}

void ProjectHandling::decideWhichProjectNamesToShow() {
    
    for ( int i = 0; i < projectNamesLabel.size(); i++ ) {
        if ( projectNamesLabel[i].getMyPageNr() == _currentPageNr ) {
            projectNamesLabel[i].show();
        } else {
            projectNamesLabel[i].hide();
        }
        
        if ( projectNamesLabel[i].label->isVisible() ) {
            projectNamesLabel[i].setPosToTop( overlayBrowse->getBoundingBox().size, overlayBrowse->getPosition() );
        }
    }
    
}

void ProjectHandling::showDeletePrompt( std::string projectName ) {
    _whatState = kProjectHandling_State_DeletePrompt;
    overlayDeletePrompt->setVisible( true );
    label_deletePrompt_areYouSure->setString( "Er du sikker på du vil slette\n" + projectName + "?" );
    label_deletePrompt_areYouSure->setVisible( true );
    buttonBg[kButtons_ProjectHandling_Index_ConfirmDelete]->setVisible( true );
    label_buttons[kButtons_ProjectHandling_Index_ConfirmDelete]->setVisible( true );
    buttonBg[kButtons_ProjectHandling_Index_CancelDelete]->setVisible( true );
    label_buttons[kButtons_ProjectHandling_Index_CancelDelete]->setVisible( true );
}

void ProjectHandling::deleteProject( std::string projectName, std::string currentProjectName ) {
    
    log( "delete project: %s", projectName.c_str() );
    
    if ( projectName.compare( currentProjectName ) != 0 ) {
        
        FileUtils *fileUtils = FileUtils::getInstance();
        std::string writablePath = fileUtils->getWritablePath();
        std::string projectFullPath = writablePath + projectName;
        log( "full path project to delete: %s", projectFullPath.c_str() );
        if ( fileUtils->isDirectoryExist( projectFullPath ) ) {
            fileUtils->removeDirectory( projectFullPath );
        }
        
        for ( int i = 0; i < projectNamesLabel.size(); i++ ) {
            _layer->removeChild( projectNamesLabel[i].label );
            _layer->removeChild( projectNamesLabel[i].squareBg );
        }
        projectNamesLabel.clear();
        
        updateProjectList();
        decideWhichProjectNamesToShow();
        setSelectedProjectName( "" );
        setAprojectIsSelected( false );
        buttonBg[kButtons_ProjectHandling_Index_Open]->setOpacity( kProjectHandling_Button_TransparantValue );
        buttonBg[kButtons_ProjectHandling_Index_Delete]->setOpacity( kProjectHandling_Button_TransparantValue );
        
        if ( _currentPageNr > _totalNrOfPages ) {
            _currentPageNr = _totalNrOfPages;
            label_projectNamesPageNr->setString( to_string( _currentPageNr ) + " / " + to_string( _totalNrOfPages ) );
            decideWhichProjectNamesToShow();
        }
        
        closeDeletePrompt();
        
    } else {
        log( "can not delete a project that is currently open" );
    }
    
}

bool ProjectHandling::buttonTouchHasBegun( int whatButton ) {
    return _buttonTouchHasBegun[whatButton];
}

void ProjectHandling::setButtonTouchHasBegun( bool touchHasBegun, int whatButton ) {
    _buttonTouchHasBegun[whatButton] = touchHasBegun;
    if ( touchHasBegun ) {
        buttonBg[whatButton]->setScale( 1.1 );
        label_buttons[whatButton]->setScale( 1.1 );
    } else {
        buttonBg[whatButton]->setScale( 1.0 );
        label_buttons[whatButton]->setScale( 1.0 );
    }
}

void ProjectHandling::abortWithTouchMove( Vec2 touchPos ) {
    
    Vec2 stopPos = touchPos;
    float distX = _touchStartPos.x - stopPos.x;
    float distY = _touchStartPos.y - stopPos.y;
    float touchMoveTolerance = visibleSize.width * 0.05f;
    
    float offsetAbs = abs( distX ) + abs( distY );
    
    if ( offsetAbs > touchMoveTolerance ) {
        for ( int i = 0; i < kButtons_ProjectHandling_NumOf; i++ ) {
            _buttonTouchHasBegun[i] = false;
            buttonBg[i]->setScale( 1.0 );
            label_buttons[i]->setScale( 1.0 );
        }
    }
        
}

void ProjectHandling::setTouchStartPos( Vec2 touchPos ) {
    _touchStartPos = touchPos;
}

void ProjectHandling::closeDeletePrompt() {
    _whatState = kProjectHandling_State_BrowseOverlay;
    overlayDeletePrompt->setVisible( false );
    label_deletePrompt_areYouSure->setVisible( false );
    buttonBg[kButtons_ProjectHandling_Index_CancelDelete]->setVisible( false );
    label_buttons[kButtons_ProjectHandling_Index_CancelDelete]->setVisible( false );
    buttonBg[kButtons_ProjectHandling_Index_ConfirmDelete]->setVisible( false );
    label_buttons[kButtons_ProjectHandling_Index_ConfirmDelete]->setVisible( false );
}
