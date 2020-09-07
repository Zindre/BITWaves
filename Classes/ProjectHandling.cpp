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
    
    textField = cocos2d::TextFieldTTF::textFieldWithPlaceHolder( "Prosjektnavn", "fonts/arial.ttf", 10 );
    textField->setPosition( Vec2( visibleSize.width * 0.5 + origin.x, visibleSize.height * 0.8 + origin.y ) );
    textField->setColor( Color3B::BLACK );
    layer->addChild( textField, kLayer_ProjectHandling );
    
    label_save = Label::createWithTTF( "Lagre", "fonts/arial.ttf", 10 );
    label_save->setPosition( Vec2( visibleSize.width * 0.5 + origin.x, visibleSize.height * 0.6 + origin.y ) );
    label_save->setColor( Color3B::BLACK );
    layer->addChild( label_save, kLayer_ProjectHandling );
    
    label_load = Label::createWithTTF( "Åpne", "fonts/arial.ttf", 10 );
    label_load->setPosition( Vec2( visibleSize.width * 0.5 + origin.x, visibleSize.height * 0.4 + origin.y ) );
    label_load->setColor( Color3B::BLACK );
    layer->addChild( label_load, kLayer_ProjectHandling );
    
    _isShowing = false;
    
    hide();
    loadCurrentData();
    
}

void ProjectHandling::show() {
    blackLayer->setVisible( true );
    background->setVisible( true );
    closeCross->setVisible( true );
    textField->setVisible( true );
    label_save->setVisible( true );
    label_load->setVisible( true );
    _isShowing = true;
}

void ProjectHandling::hide() {
    blackLayer->setVisible( false );
    background->setVisible( false );
    closeCross->setVisible( false );
    textField->setVisible( false );
    label_save->setVisible( false );
    label_load->setVisible( false );
    _isShowing = false;
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

void ProjectHandling::openKeyboard() {
    textField->attachWithIME();
}

void ProjectHandling::save() {
    
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
    
}

void ProjectHandling::load() {
    
    UserDefault::getInstance()->deleteValueForKey( "current_posX" );
    UserDefault::getInstance()->deleteValueForKey( "current_posY" );
    UserDefault::getInstance()->deleteValueForKey( "current_whatSound" );
    
    currentPos.clear();
    currentWhatSound.clear();
    
    
    // ----------------------------------------------------------------------------------
    std::string projectName_X = textField->getString() + "_" + "pos_X";
    
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
    std::string projectName_Y = textField->getString() + "_" + "pos_Y";
    
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
    std::string projectName_whatSound = textField->getString() + "_" + "whatSound";
    
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
