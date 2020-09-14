#pragma once

#include <stdio.h>
#include "cocos2d.h"
#include "Konstants.h"
#include "Functions.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;

class ProjectNames {

public:
    
    ProjectNames( cocos2d::Layer *layer, std::string projectName, int index );
    
    void show();
    void hide();
    
    cocos2d::Label *label;

private:
    
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    std::vector<std::string> savedProjectNames;
    
};
