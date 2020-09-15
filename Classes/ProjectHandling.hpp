#pragma once

#include <stdio.h>
#include "cocos2d.h"
#include "Konstants.h"
#include "Functions.h"
#include "ui/CocosGUI.h"
#include "ProjectNames.hpp"

using namespace cocos2d;

class ProjectHandling {

public:

    ProjectHandling( cocos2d::Layer *layer );
    
    void show();
    void hide();
    bool isShowing();
    void saveNewProject();
    void loadSavedProject();
    std::string getTextFieldString();
    void createNewProject();
    void showSaveOverlay();
    void showLoadOverlay();
    bool isSaveOverlayOpen();
    bool isLoadOverlayOpen();
    void openKeyboard();
    void closeSaveOverlay();
    void setSelectedProjectNameForLoading( std::string projectName );
    std::string getSelectedProjectNameForLoading();
    bool savingIsPossible();
    bool aProjectIsSelectedToOpen();
    void setAprojectIsSelectedToOpen( bool aProjectIsSelectedToOpen );
    
    cocos2d::Sprite *closeCross;
    cocos2d::TextFieldTTF *textField;
    cocos2d::Label *label_buttons[NUM_OF_BUTTONS_PROJECTSHANDLING];
    cocos2d::Sprite *buttonBack[NUM_OF_BUTTONS_PROJECTSHANDLING];
    std::vector<ProjectNames> projectNames;
    
private:
    
    
    void loadCurrentData();
    void saveCurrentToOpenProject();
    
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    cocos2d::Sprite *blackLayer;
    cocos2d::Sprite *background;
    bool _isShowing;
    std::vector<Vec2> currentPos;
    std::vector<int> currentWhatSound;
    cocos2d::Sprite *overlaySave;
    cocos2d::Sprite *overlayLoad;
    cocos2d::Label *label_instructTyping;
    bool _isSaveOverlayOpen;
    bool _isLoadOverlayOpen;
    std::vector<std::string> savedProjectNames;
    std::string selectedProjectNameForLoading;
    bool _savingIsPossible;
    bool _aProjectIsSelectedToOpen;


    

};
