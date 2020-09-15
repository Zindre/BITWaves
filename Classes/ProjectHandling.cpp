#include "ProjectHandling.hpp"
#include <iomanip>

ProjectHandling::ProjectHandling( Layer *layer ) {
    
    visibleSize = Director::getInstance()->getSafeAreaRect().size;
    origin = Director::getInstance()->getSafeAreaRect().origin;
    
    blackLayer = Sprite::create( "square1px.png" );
    blackLayer->setTextureRect( Rect( 0, 0, visibleSize.width, visibleSize.height ) );
    blackLayer->setPosition( Vec2( visibleSize.width * 0.5 + origin.x, visibleSize.height * 0.5 + origin.y ) );
    blackLayer->setColor( Color3B::BLACK );
    blackLayer->setOpacity( 200 );
    layer->addChild( blackLayer, kLayer_ProjectHandling );
    
    background = Sprite::create( "square1px.png" );
    background->setTextureRect( Rect( 0, 0, visibleSize.width * 0.8, visibleSize.height * 0.8 ) );
    background->setPosition( Vec2( visibleSize.width * 0.5 + origin.x, visibleSize.height * 0.5 + origin.y ) );
    layer->addChild( background, kLayer_ProjectHandling );
    
    closeCross = Sprite::create( "closeCross@2x.png" );
    closeCross->setAnchorPoint( Vec2( 0, 1 ) );
    closeCross->setPosition( Vec2( origin.x + closeCross->getBoundingBox().size.width, visibleSize.height - closeCross->getBoundingBox().size.height + origin.y ) );
    layer->addChild( closeCross, kLayer_ProjectHandling );
    
    float padding = 0.0;
    
    for ( int i = 0; i < NUM_OF_BUTTONS_PROJECTSHANDLING; i++ ) {
        buttonBack[i] = Sprite::create( "buttonBack.png" );
        padding = buttonBack[i]->getBoundingBox().size.height * 0.5;
        buttonBack[i]->setPosition( Vec2( origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.6 - ( (buttonBack[i]->getBoundingBox().size.height + padding) * i) ) );
        layer->addChild( buttonBack[i], kLayer_ProjectHandling );
        
        label_buttons[i] = Label::createWithTTF( "button", "fonts/arial.ttf", 10 );
        label_buttons[i]->setColor( Color3B::WHITE );
        label_buttons[i]->setPosition( buttonBack[i]->getPosition() );
        layer->addChild( label_buttons[i], kLayer_ProjectHandling );
    }
    label_buttons[BUTTON_PROJECTSHANDLING_INDEX_SAVE]->setString( "Lagre" );
    label_buttons[BUTTON_PROJECTSHANDLING_INDEX_LOAD]->setString( "Åpne" );
    label_buttons[BUTTON_PROJECTSHANDLING_INDEX_NEW]->setString( "Nytt prosjekt" );
    label_buttons[BUTTON_PROJECTSHANDLING_INDEX_CONFIRMSAVE]->setString( "Bekreft Lagre" );
    label_buttons[BUTTON_PROJECTSHANDLING_INDEX_CONFIRMLOAD]->setString( "Bekreft Åpne" );
    label_buttons[BUTTON_PROJECTSHANDLING_INDEX_CANCEL]->setString( "Avbryt" );
    
    
    overlaySave = Sprite::create( "square1px.png" );
    overlaySave->setTextureRect( Rect( 0, 0, background->getBoundingBox().size.width * 0.7, background->getBoundingBox().size.height * 0.7 ) );
    overlaySave->setPosition( Vec2( origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.5 ) );
    overlaySave->setColor( Color3B::RED );
    overlaySave->setVisible( false );
    layer->addChild( overlaySave, kLayer_ProjectHandling );
    
    overlayLoad = Sprite::create( "square1px.png" );
    overlayLoad->setTextureRect( Rect( 0, 0, background->getBoundingBox().size.width * 0.7, background->getBoundingBox().size.height * 0.7 ) );
    overlayLoad->setPosition( Vec2( origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.5 ) );
    overlayLoad->setColor( Color3B::GREEN );
    overlayLoad->setVisible( false );
    layer->addChild( overlayLoad, kLayer_ProjectHandling );
    
    label_instructTyping = Label::createWithTTF( "Gi prosjektet et navn:", "fonts/arial.ttf", 10 );
    label_instructTyping->setPosition( Vec2( overlaySave->getPosition().x, overlaySave->getPosition().y + (overlaySave->getBoundingBox().size.height * 0.3) ) );
    label_instructTyping->setVisible( false );
    label_instructTyping->setColor( Color3B::BLACK );
    layer->addChild( label_instructTyping, kLayer_ProjectHandling );
    
    textField = cocos2d::TextFieldTTF::textFieldWithPlaceHolder( "", "fonts/arial.ttf", 10 );
    textField->setPosition( Vec2( label_instructTyping->getPosition().x, label_instructTyping->getPosition().y - (label_instructTyping->getBoundingBox().size.height * 2) ) );
    textField->setColor( Color3B::BLACK );
    textField->setVisible( false );
    layer->addChild( textField, kLayer_ProjectHandling );
    
    buttonBack[BUTTON_PROJECTSHANDLING_INDEX_CONFIRMSAVE]->setPosition( Vec2( textField->getPosition().x, textField->getPosition().y - (padding * 2) ) );
    label_buttons[BUTTON_PROJECTSHANDLING_INDEX_CONFIRMSAVE]->setPosition( buttonBack[BUTTON_PROJECTSHANDLING_INDEX_CONFIRMSAVE]->getPosition() );
    buttonBack[BUTTON_PROJECTSHANDLING_INDEX_CONFIRMSAVE]->setLocalZOrder( kLayer_ProjectHandling_SaveOverlay );
    label_buttons[BUTTON_PROJECTSHANDLING_INDEX_CONFIRMSAVE]->setLocalZOrder( kLayer_ProjectHandling_SaveOverlay );
    
    buttonBack[BUTTON_PROJECTSHANDLING_INDEX_CONFIRMLOAD]->setPosition( Vec2( overlayLoad->getPosition().x + overlayLoad->getBoundingBox().size.width * 0.3, overlayLoad->getPosition().y ) );
    label_buttons[BUTTON_PROJECTSHANDLING_INDEX_CONFIRMLOAD]->setPosition( buttonBack[BUTTON_PROJECTSHANDLING_INDEX_CONFIRMLOAD]->getPosition() );
    buttonBack[BUTTON_PROJECTSHANDLING_INDEX_CONFIRMLOAD]->setLocalZOrder( kLayer_ProjectHandling_LoadOverlay );
    label_buttons[BUTTON_PROJECTSHANDLING_INDEX_CONFIRMLOAD]->setLocalZOrder( kLayer_ProjectHandling_LoadOverlay );
    buttonBack[BUTTON_PROJECTSHANDLING_INDEX_CONFIRMLOAD]->setOpacity( 100 );
    
    buttonBack[BUTTON_PROJECTSHANDLING_INDEX_CANCEL]->setPosition( Vec2( overlayLoad->getPosition().x + overlayLoad->getBoundingBox().size.width * 0.3, overlayLoad->getPosition().y - (overlayLoad->getBoundingBox().size.height * 0.3) ) );
    label_buttons[BUTTON_PROJECTSHANDLING_INDEX_CANCEL]->setPosition( buttonBack[BUTTON_PROJECTSHANDLING_INDEX_CANCEL]->getPosition() );
    buttonBack[BUTTON_PROJECTSHANDLING_INDEX_CANCEL]->setLocalZOrder( kLayer_ProjectHandling_LoadOverlay );
    label_buttons[BUTTON_PROJECTSHANDLING_INDEX_CANCEL]->setLocalZOrder( kLayer_ProjectHandling_LoadOverlay );
    
    
    
    
    FileUtils *fileUtils = FileUtils::getInstance();
    std::string writablePath = fileUtils->getWritablePath();
    std::vector<std::string> savedProjectNamesFullPaths;
    savedProjectNamesFullPaths = fileUtils->listFiles( writablePath );
    
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
        
        // Add last token of string path to vector array
        savedProjectNames.push_back( tempStr.back() );
        
    }
    
    // Delete . and ..
    savedProjectNames.erase( savedProjectNames.begin() );
    savedProjectNames.erase( savedProjectNames.begin() );
    
    
    
    for ( int i = 0; i < savedProjectNames.size(); i++ ) {
        log( "saved project names: %s", savedProjectNames[i].c_str() );
        if ( strcmp( savedProjectNames[i].c_str(), "Uten tittel" ) != 0 ) {
            projectNames.push_back( ProjectNames( layer, savedProjectNames[i].c_str(), i ) );
        }
    }
    
    
    


    _isShowing = false;
    _isSaveOverlayOpen = false;
    _isLoadOverlayOpen = false;
    selectedProjectNameForLoading = "";
    _aProjectIsSelectedToOpen = false;
    
    
    loadCurrentData();
    
    
    std::string currentProjectName = UserDefault::getInstance()->getStringForKey( "currentProjectName" );
    if ( currentProjectName == "Uten tittel" ) {
        _savingIsPossible = true;
    } else {
        _savingIsPossible = false;
        buttonBack[BUTTON_PROJECTSHANDLING_INDEX_SAVE]->setOpacity( 100 );
        saveCurrentToOpenProject();
    }
    
    
    hide();
    
    
}

void ProjectHandling::show() {
    blackLayer->setVisible( true );
    background->setVisible( true );
    closeCross->setVisible( true );
    for ( int i = 0; i < NUM_OF_BUTTONS_PROJECTSHANDLING; i++ ) {
        buttonBack[i]->setVisible( true );
        label_buttons[i]->setVisible( true );
    }
    buttonBack[BUTTON_PROJECTSHANDLING_INDEX_CONFIRMSAVE]->setVisible( false );
    label_buttons[BUTTON_PROJECTSHANDLING_INDEX_CONFIRMSAVE]->setVisible( false );
    buttonBack[BUTTON_PROJECTSHANDLING_INDEX_CONFIRMLOAD]->setVisible( false );
    label_buttons[BUTTON_PROJECTSHANDLING_INDEX_CONFIRMLOAD]->setVisible( false );
    buttonBack[BUTTON_PROJECTSHANDLING_INDEX_CANCEL]->setVisible( false );
    label_buttons[BUTTON_PROJECTSHANDLING_INDEX_CANCEL]->setVisible( false );
    _isShowing = true;
}

void ProjectHandling::hide() {
    blackLayer->setVisible( false );
    background->setVisible( false );
    closeCross->setVisible( false );
    for ( int i = 0; i < NUM_OF_BUTTONS_PROJECTSHANDLING; i++ ) {
        buttonBack[i]->setVisible( false );
        label_buttons[i]->setVisible( false );
    }
    for ( int i = 0; i < projectNames.size(); i++ ) {
        projectNames[i].hide();
    }
    _isShowing = false;
    overlayLoad->setVisible( false );
    overlaySave->setVisible( false );
    _isSaveOverlayOpen = false;
    _isLoadOverlayOpen = false;
    selectedProjectNameForLoading = "";
    _aProjectIsSelectedToOpen = false;
    label_instructTyping->setVisible( false );
    textField->setVisible( false );
}

bool ProjectHandling::isShowing() {
    return _isShowing;
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
    std::string projectName_X = selectedProjectNameForLoading + "_" + "pos_X";
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
    std::string projectName_Y = selectedProjectNameForLoading + "_" + "pos_Y";
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
    std::string projectName_whatSound = selectedProjectNameForLoading + "_" + "whatSound";
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
    buttonBack[BUTTON_PROJECTSHANDLING_INDEX_CONFIRMSAVE]->setVisible( true );
    label_buttons[BUTTON_PROJECTSHANDLING_INDEX_CONFIRMSAVE]->setVisible( true );
    buttonBack[BUTTON_PROJECTSHANDLING_INDEX_CANCEL]->setVisible( true );
    label_buttons[BUTTON_PROJECTSHANDLING_INDEX_CANCEL]->setVisible( true );
    _isSaveOverlayOpen = true;
}

void ProjectHandling::showLoadOverlay() {
    overlayLoad->setVisible( true );
    for ( int i = 0; i < projectNames.size(); i++ ) {
        projectNames[i].show();
    }
    buttonBack[BUTTON_PROJECTSHANDLING_INDEX_CONFIRMLOAD]->setVisible( true );
    label_buttons[BUTTON_PROJECTSHANDLING_INDEX_CONFIRMLOAD]->setVisible( true );
    buttonBack[BUTTON_PROJECTSHANDLING_INDEX_CANCEL]->setVisible( true );
    label_buttons[BUTTON_PROJECTSHANDLING_INDEX_CANCEL]->setVisible( true );
    _isLoadOverlayOpen = true;
}

bool ProjectHandling::isSaveOverlayOpen() {
    return _isSaveOverlayOpen;
}

bool ProjectHandling::isLoadOverlayOpen() {
    return _isLoadOverlayOpen;
}

void ProjectHandling::openKeyboard() {
    textField->attachWithIME();
}

void ProjectHandling::closeSaveOverlay() {
    overlaySave->setVisible( false );
    textField->setVisible( false );
    label_instructTyping->setVisible( false );
    buttonBack[BUTTON_PROJECTSHANDLING_INDEX_CONFIRMSAVE]->setVisible( false );
    label_buttons[BUTTON_PROJECTSHANDLING_INDEX_CONFIRMSAVE]->setVisible( false );
    _isSaveOverlayOpen = false;
    buttonBack[BUTTON_PROJECTSHANDLING_INDEX_SAVE]->setOpacity( 100 );
    _savingIsPossible = false;
    buttonBack[BUTTON_PROJECTSHANDLING_INDEX_CANCEL]->setVisible( false );
    label_buttons[BUTTON_PROJECTSHANDLING_INDEX_CANCEL]->setVisible( false );
}

void ProjectHandling::setSelectedProjectNameForLoading( std::string projectName ) {
    selectedProjectNameForLoading = projectName;
    log( "selected project for loading: %s", selectedProjectNameForLoading.c_str() );
}

std::string ProjectHandling::getSelectedProjectNameForLoading( ){
    return selectedProjectNameForLoading;
}

bool ProjectHandling::savingIsPossible() {
    return _savingIsPossible;
}

bool ProjectHandling::aProjectIsSelectedToOpen() {
    return _aProjectIsSelectedToOpen;
}

void ProjectHandling::setAprojectIsSelectedToOpen( bool aProjectIsSelectedToOpen ) {
    _aProjectIsSelectedToOpen = aProjectIsSelectedToOpen;
    if ( _aProjectIsSelectedToOpen ) {
        buttonBack[BUTTON_PROJECTSHANDLING_INDEX_CONFIRMLOAD]->setOpacity( 255 );
    } else {
        buttonBack[BUTTON_PROJECTSHANDLING_INDEX_CONFIRMLOAD]->setOpacity( 100 );
    }
}
