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
    std::string getTextField_save_string();
    std::string getTextField_rename_string();
    void createNewProject();
    void showSaveOverlay();
    void showBrowseOverlay();
    void openKeyboard_save();
    void closeSaveOverlay();
    void closeBrowseOverlay();
    void setSelectedProjectName( std::string projectName );
    std::string getSelectedProjectName();
    bool savingIsPossible();
    bool aProjectIsSelected();
    void setAprojectIsSelected( bool aProjectIsSelected );
    unsigned int getState();
    void cancelSaveOverlay();
    void cancelBrowseOverlay();
    void arrowRightClicked();
    void arrowLeftClicked();
    void deleteProject( std::string projectName, std::string currentProjectName );
    bool buttonTouchHasBegun( int whatButton );
    void setButtonTouchHasBegun( bool touchHasBegun, int whatButton );
    void setTouchStartPos( cocos2d::Vec2 touchPos );
    void abortWithTouchMove( Vec2 touchPos );
    void showDeletePrompt( std::string projectName );
    void closeDeletePrompt();
    void showNameExistPrompt( std::string textFieldString );
    bool nameExists();
    void checkIfNameExists( std::string projectName, int whatState );
    void closeNameExistsPrompt_save();
    void closeNameExistsPrompt_rename();
    void showRenameTextField( std::string projectName );
    void openKeyboard_rename();
    void cancelRename();
    void renameProject( std::string selectedProjectName );
    void setState( int whatState );
    
    cocos2d::Sprite *closeCross;
    cocos2d::TextFieldTTF *textField_save;
    cocos2d::Label *label_buttons[kButtons_ProjectHandling_NumOf];
    cocos2d::Sprite *buttonBg[kButtons_ProjectHandling_NumOf];
    std::vector<ProjectNamesLabel> projectNamesLabel;
    cocos2d::Sprite *textFieldArea;
    //cocos2d::ui::ScrollView *scrollView;
    cocos2d::Sprite *arrowRight;
    cocos2d::Sprite *arrowLeft;
    cocos2d::Sprite *renameTextFieldBg;
    
private:
    
    void touchEvent( Ref *pSender, cocos2d::ui::Widget::TouchEventType type );
    void loadCurrentData();
    void saveCurrentToOpenProject();
    void updateProjectList();
    void decideWhichProjectNamesToShow();
    
    
    
    cocos2d::Vec2 safeAreaOrigin;
    cocos2d::Size safeAreaRect;
    cocos2d::Sprite *blackLayer;
    cocos2d::Sprite *background;
    bool _isShowing;
    std::vector<Vec2> currentPos;
    std::vector<int> currentWhatSound;
    cocos2d::Sprite *overlaySave;
    cocos2d::Sprite *overlayBrowse;
    cocos2d::Label *label_instructTyping;
    std::vector<std::string> savedProjectNames;
    std::string selectedProjectName;
    bool _savingIsPossible;
    bool _aProjectIsSelected;
    cocos2d::Layer *_layer;
    unsigned int _whatState;
    cocos2d::Size scrollViewSize;
    float padding;
    cocos2d::Label *label_projectNamesPageNr;
    unsigned int _currentPageNr;
    unsigned int _totalNrOfPages;
    bool _buttonTouchHasBegun[kButtons_ProjectHandling_NumOf];
    cocos2d::Vec2 _touchStartPos;
    cocos2d::Sprite *overlayPrompt;
    cocos2d::Label *label_prompt;
    bool _nameExists;
    cocos2d::TextFieldTTF *textField_rename;
    cocos2d::Sprite *overlayRename;
    
    


    

};
