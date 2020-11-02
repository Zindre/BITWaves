
#define kLayer_Circles 1
#define kLayer_Midline 2
#define kLayer_SeqSoundRect_Under 3
#define kLayer_RecordWaveForm 4
#define kLayer_MainMenu 5
#define kLayer_SeqSoundRect_Over 6
#define kLayer_PlayHead 7
#define kLayer_BombAnim 8
#define kLayer_InstrumentArea 9
#define kLayer_ConfirmRec 10
#define kLayer_ProjectHandling 11
#define kLayer_ProjectHandling_SaveOverlay 12
#define kLayer_ProjectHandling_BrowseOverlay 13
#define kLayer_ProjectHandling_NameBg 14
#define kLayer_ProjectHandling_NameLabel 15
#define kLayer_ProjectHandling_RenameOverlay 16
#define kLayer_ProjectHandling_Prompt 17
#define kLayer_BounceAndShare 18
#define kLayer_BounceAndShare_Prompt 19
#define kLayer_HelpOverlayBorder 20
#define kLayer_HelpOverlay 21
#define kLayer_CurrentProjectName 22

#define kTouch_NumOfTouchesAllowed 11
#define kPitchMin 0.1f
#define kPitchMax 1.0f

#define kScene_Instrument 1
#define kScene_Sequencer 2

#define kNumOfSoundObjects 7

#define kNumOfButtons 7
#define kButtons_ScaleValue 1.25f
#define kButtons_GrayedOutValue 70

#define kButtons_ArrayNum_Rec 0
#define kButtons_ArrayNum_Stop 1
#define kButtons_ArrayNum_Seq 6
#define kButtons_ArrayNum_Play 0
#define kButtons_ArrayNum_Loop 2
#define kButtons_ArrayNum_Bounce 3
#define kButtons_ArrayNum_Bomb 4
#define kButtons_ArrayNum_Mic 6
#define kButtons_ArrayNum_Help 5
#define kButtons_ArrayNum_Lock 2
#define kButtons_ArrayNum_Projects 4

#define kSequenceLengthInMS 60000

#define kTag_MoveRight 1

#define kMidLine_Height_Multiplier 0.8

#define kRecordMaxLengthSeconds 10

#define kRecordWaveForm_height_multiplier_MAX 0.05f

#define kSequencer_MinYpos_height_multiplier 0.1f

#define kSeqSoundRect_Opacity_Low 200
#define kSeqSoundRect_Opacity_High 255

//#define kSpectrum_MaxValue 6232
//#define kSpectrum_MinValue 1000
#define kSpectrum_MinValue 0
#define kSpectrum_MaxValue 5000

#define kNumOfChannels 32

#define kButtons_ProjectHandling_NumOf 14
#define kButtons_ProjectHandling_Index_Save 0
#define kButtons_ProjectHandling_Index_Browse 1
#define kButtons_ProjectHandling_Index_New 2
#define kButtons_ProjectHandling_Index_ConfirmSave 3
#define kButtons_ProjectHandling_Index_Open 4
#define kButtons_ProjectHandling_Index_CancelSave 5
#define kButtons_ProjectHandling_Index_Delete 6
#define kButtons_ProjectHandling_Index_ConfirmDelete 7
#define kButtons_ProjectHandling_Index_CancelDelete 8
#define kButtons_ProjectHandling_Index_CloseExistPrompt 9
#define kButtons_ProjectHandling_Index_CancelBrowse 10
#define kButtons_ProjectHandling_Index_Rename 11
#define kButtons_ProjectHandling_Index_CancelRename 12
#define kButtons_ProjectHandling_Index_ConfirmRename 13



#define kProjectHandling_Button_TransparantValue 100

#define kProjectHandling_State_Closed 1
#define kProjectHandling_State_MainScreen 2
#define kProjectHandling_State_BrowseOverlay 3
#define kProjectHandling_State_BrowseOverlay_Rename 4
#define kProjectHandling_State_SaveOverlay 5
#define kProjectHandling_State_SaveOverlay_KeyboardOpen 6
#define kProjectHandling_State_DeletePrompt 7
#define kProjectHandling_State_NameExistPrompt_Save 8
#define kProjectHandling_State_NameExistPrompt_Rename 9

#define kFontSize_Buttons 9
#define kFontSize_ProjectNames 7
#define kFontSize_BigText 11
#define kFontSize_SmallText 7

#define kProjectHandling_Browse_FileListBgColor 74

#define kSequencerScene_State_Normal 1
#define kSequencerScene_State_HelpOverlay 2
#define kSequencerScene_State_PlayHeadIsMoving 3
#define kSequencerScene_State_BounceAndShare 4
#define kSequencerScene_State_BounceAndShare_Prompt 5
#define kSequencerScene_State_IsBouncing 6
#define kSequencerScene_State_BombIsPressed 7

#define kBounceAndShare_Buttons_NumOf 2
#define kBounceAndShare_Buttons_Index_Share 0
#define kBounceAndShare_Buttons_Index_PromptConfirm 1
