#pragma once

#include <stdio.h>
#include "cocos2d.h"
#include "Konstants.h"
#include "Functions.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;

class ProjectHandling {

public:

    ProjectHandling( cocos2d::Layer *layer );
    
    void show();
    void hide();
    bool isShowing();
    void openKeyboard();
    void save();
    void load();
    
    cocos2d::Sprite *closeCross;
    cocos2d::TextFieldTTF *textField;
    cocos2d::Label *label_save;
    cocos2d::Label *label_load;
    
private:
    
    
    void loadCurrentData();
    
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    cocos2d::Sprite *blackLayer;
    cocos2d::Sprite *background;
    bool _isShowing;
    std::vector<Vec2> currentPos;
    std::vector<int> currentWhatSound;

    
    

};
