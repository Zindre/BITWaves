#pragma once

#include <stdio.h>
#include "cocos2d.h"
#include "Konstants.h"
#include "Functions.h"
#include "ui/CocosGUI.h"
#include "ProjectNamesLabel.hpp"

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
    void openKeyboard();
    void closeSaveOverlay();
    void closeLoadOverlay();
    void setSelectedProjectNameForLoading( std::string projectName );
    std::string getSelectedProjectNameForLoading();
    bool savingIsPossible();
    bool aProjectIsSelectedToOpen();
    void setAprojectIsSelectedToOpen( bool aProjectIsSelectedToOpen );
    unsigned int getState();
    void cancelSaveOverlay();
    
    cocos2d::Sprite *closeCross;
    cocos2d::TextFieldTTF *textField;
    cocos2d::Label *label_buttons[kButtons_ProjectHandling_NumOf];
    cocos2d::Sprite *buttonBack[kButtons_ProjectHandling_NumOf];
    std::vector<ProjectNamesLabel> projectNamesLabel;
    cocos2d::Sprite *textFieldArea;
    //cocos2d::ui::ScrollView *scrollView;
    
private:
    
    void touchEvent( Ref *pSender, cocos2d::ui::Widget::TouchEventType type );
    
    void loadCurrentData();
    void saveCurrentToOpenProject();
    void updateProjectList();
    
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
    std::vector<std::string> savedProjectNames;
    std::string selectedProjectNameForLoading;
    bool _savingIsPossible;
    bool _aProjectIsSelectedToOpen;
    cocos2d::Layer *_layer;
    unsigned int _whatState;
    cocos2d::Size scrollViewSize;
    float padding;
    
    


    

};
