
#include "HelpOverlay.h"

HelpOverlay::HelpOverlay( cocos2d::Layer *layer, unsigned int whatScene ) {
    
    this->whatScene = whatScene;
    
    //visibleSize = Director::getInstance()->getVisibleSize();
    visibleSize = Director::getInstance()->getSafeAreaRect().size;
    //origin = Director::getInstance()->getVisibleOrigin();
    origin = Director::getInstance()->getSafeAreaRect().origin;
    
    // SCROLL BOX
    scrollView = ui::ScrollView::create();
    cocos2d::Size boxSize = cocos2d::Size( visibleSize.width * 0.5f, visibleSize.height * 0.7f );
    scrollView->setContentSize( boxSize );
    scrollView->setDirection( ui::ScrollView::Direction::VERTICAL );
    scrollView->setAnchorPoint( Vec2( 0.5f, 0.5f ) );
    scrollView->setPosition( Vec2( visibleSize.width * 0.7f + origin.x, visibleSize.height * 0.57f + origin.y ) );
    scrollView->setBounceEnabled( true );
    scrollView->setScrollBarAutoHideEnabled( false );
    scrollView->setScrollBarPositionFromCornerForVertical( Vec2( scrollView->getScrollBarWidth() * 2, scrollView->getScrollBarWidth() * 2 ) );
    scrollView->setScrollBarColor( Color3B( 255, 255, 255 ) );
    scrollView->setScrollBarOpacity( 255 );
    scrollView->setBackGroundColorType( cocos2d::ui::HBox::BackGroundColorType::SOLID );
    scrollView->setBackGroundColor( Color3B( 50, 50, 50 ) );
    //scrollView->setBackGroundColorOpacity( 0 );
    layer->addChild( scrollView, kLayer_HelpOverlay );
    
    unsigned int fontSize_heading = visibleSize.width * 0.03f;
    unsigned int fontSize_Text = visibleSize.width * 0.019f;
    float lineHeightMultiplier = visibleSize.height/45.7f;
    
    helpHeading_string = std::string( "Hjelp" );
    helpHeading = Label::createWithTTF( helpHeading_string, "fonts/arial.ttf", fontSize_heading );
    helpHeading->setAnchorPoint( Vec2( 0.0f, 1.0f ) );
    helpHeading->setColor( Color3B::WHITE );
    scrollView->addChild( helpHeading, kLayer_HelpOverlay );
    
    padding = visibleSize.height * 0.03f;
    float maxLineWidth = boxSize.width - (padding * 2.5f);

    helpText_instrument_string = std::string( "- Start med å låse opp for opptak ved å trykke på hengelåsen.\n\n- Trykk på opptaksknappen for å gjøre opptak.\n\n- Spill av lydene ved å trykke på det sorte området på skjermen. Du kan bruke flere fingre samtidig, og dra fingrene over skjermen.\n\n- På toppen av skjermen over linjen, spilles lyden av i originalt tempo og tonehøyde. Nedover på skjermen blir lyden langsommere og mørkere.\n\n- Lag et nytt opptak ved å velge en annen farge til høyre på skjermen.\n\n- Trykk på komposisjonsknappen for å bytte til komposisjonsmodus." );

    helpText_sequencer_string = std::string( "- Hvis du har laget innspillinger i opptak- og instumentmodus, kan du dra lyder inn på skjermen fra de fargede boksene til høyre.\n\n- Avspillingen av lydene vil endre tempo og tone i forhold til hvor på skjermen de er plassert. Lydene spilles av i originalt tempo og tonehøyde øverst på skjermen. Lydene blir mørkere og langsommere jo lengre ned på skjermen de er.\n\n- For å starte en ny komposisjon, trykk og hold på bombeknappen. Alt vil slettes og det kan ikke angres.\n\n- Trykk på mikrofonknappen for å gå tilbake til opptak- og instrumentmodus." );

    helpText = Label::createWithTTF( helpText_instrument_string, "fonts/arial.ttf", fontSize_Text );
    helpText->setAnchorPoint( Vec2( 0.0f, 1.0f ) );
    helpText->setMaxLineWidth( maxLineWidth );
    helpText->setLineHeight( fontSize_Text * lineHeightMultiplier );
    helpText->setColor( Color3B::WHITE );
    scrollView->addChild( helpText, kLayer_HelpOverlay );
    if ( whatScene == kScene_Instrument ) {
        helpText->setString( helpText_instrument_string );
    } else {
        helpText->setString( helpText_sequencer_string );
    }
    
    aboutHeading_string = std::string( "Om BITWaves" );
    aboutHeading = Label::createWithTTF( aboutHeading_string, "fonts/arial.ttf", fontSize_heading );
    aboutHeading->setAnchorPoint( Vec2( 0.0f, 1.0f ) );
    aboutHeading->setColor( Color3B::WHITE );
    scrollView->addChild( aboutHeading, kLayer_HelpOverlay );

    about_string = std::string( "BIT20 Ensemble ble stiftet i 1989 og er et samtidsmusikkorkester. Orkesteret gjør innspillinger av norske og utenlandske komponister, har konserter over hele verden og står bak flere spennende deltakerprosjekt med barn, ungdom og voksne i alle aldre. BITWaves ble utviklet av Stian Remvik ved BEK (Bergen Senter For Elektronisk Kunst) i 2015 som en musikkapp for BIT20 Ensemble. App-en kan brukes i komposisjonsprosjekter og i musikkproduksjon. BITWaves er finansiert av BIT20 Ensemble med støtte fra Norsk Kulturråd og Bergen Kommune. App-en brukes primært i undervisning og formidling rettet mot barn og unge.\n\nIdé: Ole André Farstad\nDesign og programmering: Stian Remvik\nProgrammering: Sindre Sørensen\n\n v. 2.3\n\n" );

    aboutText = Label::createWithTTF( about_string, "fonts/arial.ttf", fontSize_Text );
    aboutText->setAnchorPoint( Vec2( 0.0f, 1.0f ) );
    aboutText->setMaxLineWidth( maxLineWidth );
    aboutText->setLineHeight( fontSize_Text * lineHeightMultiplier );
    aboutText->setColor( Color3B::WHITE );
    scrollView->addChild( aboutText, kLayer_HelpOverlay );
    
    scrollView->setInnerContainerSize( Size( maxLineWidth, helpHeading->getBoundingBox().size.height + helpText->getBoundingBox().size.height + aboutHeading->getBoundingBox().size.height + aboutText->getBoundingBox().size.height + (padding * 5) ) );
    
    helpHeading->setPosition( Vec2( scrollView->getInnerContainerPosition().x + padding, scrollView->getInnerContainerSize().height - padding ) );
    helpText->setPosition( Vec2( helpHeading->getPosition().x, helpHeading->getPosition().y - helpHeading->getBoundingBox().size.height - padding ) );
    aboutHeading->setPosition( Vec2( helpText->getPosition().x, helpText->getPosition().y - helpText->getBoundingBox().size.height - padding ) );
    aboutText->setPosition( Vec2( aboutHeading->getPosition().x, aboutHeading->getPosition().y - aboutHeading->getBoundingBox().size.height - padding ) );
    
    // LOGO BOX
    logoBoxBgBox = Sprite::create( "square1px.png" );
    logoBoxBgBox->setTextureRect( Rect( 0, 0, scrollView->getBoundingBox().size.width, visibleSize.height * 0.15f ) );
    logoBoxBgBox->setAnchorPoint( Vec2( 0.5f, 1.0f ) );
    logoBoxBgBox->setPosition( Vec2( scrollView->getPosition().x, scrollView->getPosition().y - (scrollView->getBoundingBox().size.height/2.0f) ) );
    logoBoxBgBox->setColor( Color3B::BLACK );
    layer->addChild( logoBoxBgBox, kLayer_HelpOverlay );
    
    logoBox = Sprite::create( "LogoBox.png" );
    logoBox->setAnchorPoint( Vec2( 0.5f, 0.5f ) );
    logoBox->setPosition( Vec2( logoBoxBgBox->getPosition().x, logoBoxBgBox->getPosition().y - (logoBoxBgBox->getBoundingBox().size.height/2.0f) ) );
    layer->addChild( logoBox, kLayer_HelpOverlay );
    
    
    // BORDER
    border = Sprite::create( "square1px.png" );
    border->setTextureRect( Rect( 0, 0, scrollView->getBoundingBox().size.width + (padding/8.0f), scrollView->getBoundingBox().size.height + logoBoxBgBox->getBoundingBox().size.height + (padding/8.0f) ) );
    border->setPosition( Vec2( scrollView->getPosition().x, scrollView->getPosition().y - (logoBoxBgBox->getBoundingBox().size.height/2.0f) ) );
    //border->setOpacity( 50 );
    border->setColor( Color3B( 170, 170, 170 ) );
    layer->addChild( border, kLayer_HelpOverlayBorder );
    
    
    // HELP LABELS
    float soundSquareHeight = visibleSize.height / kNumOfSoundObjects;
    
    // Using container to spread out the buttons evenly on Y-axis
    for ( int i = 0; i < kNumOfButtons; i++ ) {
        helpLabelContainer[i] = Sprite::create( "square1px.png" );
        helpLabelContainer[i]->setAnchorPoint( Vec2( 0.0f, 1.0f ) );
        helpLabelContainer[i]->setTextureRect( Rect( 0, 0, soundSquareHeight, soundSquareHeight ) );
        helpLabelContainer[i]->setPosition( Vec2( soundSquareHeight + origin.x, visibleSize.height - (helpLabelContainer[i]->getBoundingBox().size.height * i) + origin.y ) );
        helpLabelContainer[i]->setColor( Color3B( 255, 0, 0 ) );
        helpLabelContainer[i]->setVisible( false );
        layer->addChild( helpLabelContainer[i], kLayer_HelpOverlay );
    }
    
    for ( int i = 0; i < kNumOfButtons; i++ ) {
        helpLabel[i] = Sprite::create( "square1px.png" );
        helpLabel[i]->setAnchorPoint( Vec2( 0.0f, 0.5f ) );
        helpLabel[i]->setTextureRect( Rect( 0, 0, soundSquareHeight, soundSquareHeight ) );
        helpLabel[i]->setPosition( Vec2( helpLabelContainer[i]->getPosition().x, helpLabelContainer[i]->getPosition().y - (helpLabelContainer[i]->getBoundingBox().size.height/2.0f) ) );
        layer->addChild( helpLabel[i], kLayer_HelpOverlay );
    }
    if ( whatScene == kScene_Sequencer ) {
        helpLabel[0]->setTexture( "help_play.png" );
        helpLabel[1]->setTexture( "help_back.png" );
        helpLabel[2]->setTexture( "help_loop.png" );
        helpLabel[3]->setTexture( "help_share.png" );
        helpLabel[4]->setTexture( "help_delete.png" );
        helpLabel[5]->setTexture( "help_instrument.png" );
        helpLabel[6]->setTexture( "help_help.png" );
    } else {
        helpLabel[0]->setTexture( "help_rec.png" );
        helpLabel[2]->setTexture( "help_lock.png" );
        helpLabel[5]->setTexture( "help_comp.png" );
        helpLabel[6]->setTexture( "help_help.png" );
    }
    
}

void HelpOverlay::hide() {
    scrollView->setVisible( false );
    aboutHeading->setVisible( false );
    aboutText->setVisible( false );
    
    logoBox->setVisible( false );
    logoBoxBgBox->setVisible( false );
    
    for ( int i = 0; i < kNumOfButtons; i++ ) {
        helpLabel[i]->setVisible( false );
    }
    border->setVisible( false );
    
}

void HelpOverlay::show() {
    scrollView->setVisible( true );
    aboutHeading->setVisible( true );
    aboutText->setVisible( true );
    
    logoBox->setVisible( true );
    logoBoxBgBox->setVisible( true );
    
    for ( int i = 0; i < kNumOfButtons; i++ ) {
        helpLabel[i]->setVisible( true );
    }

    // TEMP REMOVE BOUNCE
    helpLabel[3]->setVisible( false );

    if ( whatScene == kScene_Instrument ) {
        helpLabel[1]->setVisible( false );
        helpLabel[3]->setVisible( false );
        helpLabel[4]->setVisible( false );
    }
    
    border->setVisible( true );
    
}
