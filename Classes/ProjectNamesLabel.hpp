#pragma once

#include <stdio.h>
#include "cocos2d.h"
#include "Konstants.h"
#include "Functions.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;

class ProjectNamesLabel {

public:
    
    ProjectNamesLabel( cocos2d::Layer *layer, std::string projectName, int index, unsigned int myPageNr, unsigned int pageIndex, cocos2d::Size overlayBrowseSize, cocos2d::Vec2 overlayBrowsePos );
    
    void show();
    void hide();
    unsigned int getMyPageNr();
    void setPosToTop();
    std::string getFullString();
    
    cocos2d::Label *label;
    cocos2d::Sprite *squareBg;
    //cocos2d::ui::Button *button;

private:
    
    void setSquareBgPos();
    
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    unsigned int _myPageNr;
    float _padding;
    unsigned int _pageIndex;
    std::string _projectNameFullString;
    
};
