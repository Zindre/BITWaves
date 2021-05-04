#pragma once

#include <stdio.h>
#include "cocos2d.h"
#include "Functions.h"
#include "Konstants.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;

class HelpOverlay {
    
public:
    
    HelpOverlay( cocos2d::Layer *layer, unsigned int whatScene );
    void show();
    void hide();
    void openWebLink();
    
    cocos2d::Sprite *webLinkBg;
    
private:
    
    cocos2d::Vec2 safeAreaOrigin;
    cocos2d::Size safeAreaRect;
    cocos2d::ui::ScrollView *scrollView;
    
    cocos2d::Label *helpHeading;
    cocos2d::Label *helpText;
    cocos2d::Label *aboutHeading;
    cocos2d::Label *aboutText;
    
    std::string helpHeading_string;
    std::string helpText_instrument_string;
    std::string helpText_sequencer_string;
    std::string aboutHeading_string;
    std::string about_string;
    
    float padding;
    
    cocos2d::Sprite *helpLabelContainer[kNumOfButtons];
    cocos2d::Sprite *helpLabel[kNumOfButtons];
    
    cocos2d::Sprite *logoBox;
    cocos2d::Sprite *logoBoxBgBox;
    
    unsigned int whatScene;
    
    cocos2d::Sprite *border;
    cocos2d::Label *label_webLink;
    
    cocos2d::Sprite *privacyBgWhite;
    cocos2d::Sprite *privacyBgBlack;
    cocos2d::Label *privacyHeader;
    cocos2d::Label *privacyText;
    cocos2d::Sprite *privacyButtonBg;
    cocos2d::Label *privacyButtonText;
    
};
