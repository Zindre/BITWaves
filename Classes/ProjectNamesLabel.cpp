#include "ProjectNamesLabel.hpp"

ProjectNamesLabel::ProjectNamesLabel( cocos2d::Layer *layer, std::string projectName, unsigned int index ) {
    
    visibleSize = Director::getInstance()->getSafeAreaRect().size;
    origin = Director::getInstance()->getSafeAreaRect().origin;
    
    /*label = cocos2d::Label::createWithTTF( projectName, "fonts/arial.ttf", 10 );
    float padding = label->getBoundingBox().size.height;
    label->setAnchorPoint( Vec2( 0.0, 1.0 ) );
    label->setColor( Color3B::BLACK );*/
    
    button = cocos2d::ui::Button::create( "buttonBack.png", "buttonBack.png" );
    button->setTitleText( projectName );
    button->setTitleColor( Color3B::YELLOW );
    button->setAnchorPoint( Vec2( 0.0, 1.0 ) );
    button->setTitleAlignment( TextHAlignment::LEFT );
    button->addTouchEventListener( CC_CALLBACK_2( ProjectNamesLabel::touchEvent, this ) );
    
    _projectName = projectName;
    _index = index;
    _isTouched = false;
    
    hide();
}

void ProjectNamesLabel::show() {
    button->setVisible( true );
}

void ProjectNamesLabel::hide() {
    button->setVisible( false );
}

std::string ProjectNamesLabel::getProjectName() {
    return _projectName;
}

unsigned int ProjectNamesLabel::getIndex() {
    return _index;
}

void ProjectNamesLabel::touchEvent( Ref *pSender, cocos2d::ui::Widget::TouchEventType type ) {
    
    if ( type == cocos2d::ui::Widget::TouchEventType::BEGAN ) {
        _isTouched = true;
        //setSelectedProjectNameForLoading( projectNamesLabel[].getProjectName() );
        //setAprojectIsSelectedToOpen( true );
    }

}

bool ProjectNamesLabel::isTouched() {
    return _isTouched;
}
