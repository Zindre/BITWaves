#include "ProjectNamesLabel.hpp"

ProjectNamesLabel::ProjectNamesLabel( cocos2d::Layer *layer, std::string projectName, int index, unsigned int myPageNr, unsigned int pageIndex, cocos2d::Size overlayBrowseSize, cocos2d::Vec2 overlayBrowsePos ) {
    
    safeAreaRect = Director::getInstance()->getSafeAreaRect().size;
    safeAreaOrigin = Director::getInstance()->getSafeAreaRect().origin;
    
    _myPageNr = myPageNr;
    _pageIndex = pageIndex;
    _projectNameFullString = projectName;
    
    log( "project name: %s - index: %i - page nr: %i", projectName.c_str(), index, _myPageNr );
    
    int maxStringLength = 40;
    std::string shortString = "";
    if ( projectName.length() > maxStringLength ) {
        shortString = projectName.substr ( 0, maxStringLength ) + "...";
    } else {
        shortString = projectName;
    }
    
    label = cocos2d::Label::createWithTTF( shortString, "fonts/arial.ttf", kFontSize_ProjectNames );
    _padding = label->getBoundingBox().size.height;
    label->setAnchorPoint( Vec2( 0.0, 0.5 ) );
    label->setColor( Color3B::WHITE );
    layer->addChild( label, kLayer_ProjectHandling_NameLabel );
    
    squareBg = Sprite::create( "square1px.png" );
    squareBg->setTextureRect( Rect( 0, 0, overlayBrowseSize.width * 0.5, label->getBoundingBox().size.height * 1.5 ) );
    squareBg->setAnchorPoint( Vec2( 0.0, 0.5 ) );
    squareBg->setColor( Color3B( kProjectHandling_Browse_FileListBgColor, kProjectHandling_Browse_FileListBgColor, kProjectHandling_Browse_FileListBgColor ) );
    layer->addChild( squareBg, kLayer_ProjectHandling_NameBg );
    
    setPosToTop( overlayBrowseSize, overlayBrowsePos );
    
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
    squareBg->setVisible( true );
}

void ProjectNamesLabel::hide() {
    label->setVisible( false );
    squareBg->setVisible( false );
}

unsigned int ProjectNamesLabel::getMyPageNr() {
    return _myPageNr;
}

void ProjectNamesLabel::setPosToTop( cocos2d::Size overlayBrowseSize, cocos2d::Vec2 overlayBrowsePos ) {
    label->setPosition( Vec2( overlayBrowsePos.x - (overlayBrowseSize.width * 0.5) + (_padding * 2), (overlayBrowsePos.y + (overlayBrowseSize.height * 0.5) - (_padding * 2)) - ((label->getBoundingBox().size.height + _padding) * _pageIndex ) ) );
    squareBg->setPosition( Vec2( label->getPosition().x - safeAreaRect.width * 0.01, label->getPosition().y ) );
}

std::string ProjectNamesLabel::getFullString() {
    return _projectNameFullString;
}
