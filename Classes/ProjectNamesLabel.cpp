#include "ProjectNamesLabel.hpp"

ProjectNamesLabel::ProjectNamesLabel( cocos2d::Layer *layer, std::string projectName, int index, unsigned int myPageNr, unsigned int pageIndex ) {
    
    visibleSize = Director::getInstance()->getSafeAreaRect().size;
    origin = Director::getInstance()->getSafeAreaRect().origin;
    
    _projectName = projectName;
    _index = index;
    _isTouched = false;
    _myPageNr = myPageNr;
    _pageIndex = pageIndex;
    
    log( "project name: %s - index: %i - page nr: %i", projectName.c_str(), _index, _myPageNr );
    
    label = cocos2d::Label::createWithTTF( projectName, "fonts/arial.ttf", kProjectHandling_FontSize_ProjectNames );
    _padding = label->getBoundingBox().size.height;
    label->setAnchorPoint( Vec2( 0.0, 1.0 ) );
    label->setColor( Color3B::BLACK );
    label->setPosition( Vec2( origin.x + visibleSize.width * 0.2, origin.y + ((visibleSize.height * 0.7) - ((label->getBoundingBox().size.height + _padding) * index ) ) ) );
    layer->addChild( label, kLayer_ProjectHandling_BrowseOverlay );
    
    /*button = cocos2d::ui::Button::create( "buttonBg.png", "buttonBg.png" );
    button->setTitleText( _projectName );
    button->setTitleColor( Color3B::YELLOW );
    button->setAnchorPoint( Vec2( 0.0, 1.0 ) );
    button->setTitleAlignment( TextHAlignment::LEFT );
    button->setName( _projectName );
    //button->addTouchEventListener( CC_CALLBACK_2( ProjectNamesLabel::touchEvent, this ) );
    button->addTouchEventListener( [&]( Ref* sender, cocos2d::ui::Widget::TouchEventType type ) {
            switch ( type ) {
                    case ui::Widget::TouchEventType::BEGAN:
                {
                    auto scene = Director::getInstance()->getRunningScene();
                    auto mainLayer = scene->getChildByName( "mainLayer" );
                    cocos2d::ui::ScrollView *scrlView = (cocos2d::ui::ScrollView*)mainLayer->getChildByName( "projectsScrollView" );
                    cocos2d::ui::Button *btn = (cocos2d::ui::Button*)scrlView->getChildByName( "test" );
                    log( "%s", btn->getTitleText().c_str() );
                
                    log( "_index: %i", _index );
                    log( "projectName: %s", _projectName.c_str() );
                }
                            break;
                    case ui::Widget::TouchEventType::ENDED:
                            
                            break;
                    default:
                            break;
            }
    });
    scrollView->addChild( button, kLayer_ProjectHandling_BrowseOverlay );*/
    
    
    
    hide();
}

void ProjectNamesLabel::show() {
    label->setVisible( true );
}

void ProjectNamesLabel::hide() {
    label->setVisible( false );
}

std::string ProjectNamesLabel::getProjectName() {
    return _projectName;
}

int ProjectNamesLabel::getIndex() {
    return _index;
}

bool ProjectNamesLabel::isTouched() {
    return _isTouched;
}

unsigned int ProjectNamesLabel::getMyPageNr() {
    return _myPageNr;
}

void ProjectNamesLabel::setPosToTop() {
    label->setPosition( Vec2( origin.x + visibleSize.width * 0.2, origin.y + ((visibleSize.height * 0.7) - ((label->getBoundingBox().size.height + _padding) * _pageIndex ) ) ) );
}
