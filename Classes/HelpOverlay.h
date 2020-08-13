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
    
private:
    
    cocos2d::Vec2 origin;
    cocos2d::Size visibleSize;
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
    
    
};
