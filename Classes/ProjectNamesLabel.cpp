#include "ProjectNamesLabel.hpp"

ProjectNamesLabel::ProjectNamesLabel( cocos2d::Layer *layer, std::string projectName, int index ) {
    
    visibleSize = Director::getInstance()->getSafeAreaRect().size;
    origin = Director::getInstance()->getSafeAreaRect().origin;
    
    label = cocos2d::Label::createWithTTF( projectName, "fonts/arial.ttf", 10 );
    float padding = label->getBoundingBox().size.height;
    label->setPosition( Vec2( origin.x + visibleSize.width * 0.3, origin.y + visibleSize.height * 0.7 - ((label->getBoundingBox().size.height + padding) * index) ) );
    label->setColor( Color3B::BLACK );
    layer->addChild( label, kLayer_ProjectHandling );
    
    hide();
}

void ProjectNamesLabel::show() {
    label->setVisible( true );
}

void ProjectNamesLabel::hide() {
    label->setVisible( false );
}
