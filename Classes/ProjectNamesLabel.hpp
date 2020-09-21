#pragma once

#include <stdio.h>
#include "cocos2d.h"
#include "Konstants.h"
#include "Functions.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;

class ProjectNamesLabel {

public:
    
    ProjectNamesLabel( cocos2d::Layer *layer, std::string projectName, int index );
    
    void show();
    void hide();
    std::string getProjectName();
    int getIndex();
    bool isTouched();
    
    cocos2d::Label *label;
    //cocos2d::ui::Button *button;

private:
    
    
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    std::string _projectName;
    int _index;
    bool _isTouched;

    
};
