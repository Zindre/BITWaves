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
    void save();
    void load();
    std::string getTextFieldString();
    void createNewProject();
    void showSaveOverlay();
    bool isSaveOverlayOpen();
    void openKeyboard();
    void closeSaveOverlay();
    
    cocos2d::Sprite *closeCross;
    cocos2d::TextFieldTTF *textField;
    cocos2d::Label *label_buttons[NUM_OF_BUTTONS_PROJECTSHANDLING];
    cocos2d::Sprite *buttonBack[NUM_OF_BUTTONS_PROJECTSHANDLING];
    
private:
    
    
    void loadCurrentData();
    
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    cocos2d::Sprite *blackLayer;
    cocos2d::Sprite *background;
    bool _isShowing;
    std::vector<Vec2> currentPos;
    std::vector<int> currentWhatSound;
    cocos2d::Sprite *overlaySave;
    cocos2d::Label *label_instructTyping;
    bool _isSaveOverlayOpen;
    std::vector<std::string> savedProjectNames;
    std::vector<cocos2d::Label> *label_savedProjectNames;
    

    
    

};
