#include "mapifactory.h"
#include "work_status.h"
#include "actual_values.h"
#include "delayA.h"
#include "delayC.h"
#include "board_temp.h"
#include "board_type.h"
#include "trigger_sign.h"
#include "trigger_rand.h"
#include "refresh_data.h"
#include "triggers.h"
#include "PM_status.h"
#include "default.h"



MapiFactory::MapiFactory(Fred *fred)
{
    this->fred = fred;

    try
    {
        this->generateObjects();
    }
    catch (const exception& e)
    {
        Print::PrintError(e.what());
        exit(EXIT_FAILURE);
    }
}

MapiFactory::~MapiFactory()
{
    for (size_t i = 0; i < this->mapiObjects.size(); i++)
    {
        delete this->mapiObjects[i];
    }
    
}

/**
 * 
 * Registering mapi objects into FRED
 * 
**/
void MapiFactory::generateObjects(){
    WorkStatus* workStatus = new WorkStatus();
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/WORK_STATUS", workStatus);
    this->mapiObjects.push_back(workStatus);
    ActualValues* actualValues = new ActualValues();
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/ACTUAL_VALUES", actualValues);
    this->mapiObjects.push_back(actualValues);
    DelayA* delayA = new DelayA();
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/DELAY_A", delayA);
    this->mapiObjects.push_back(delayA);
    DelayC* delayC = new DelayC();
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/DELAY_C", delayC);
    this->mapiObjects.push_back(delayC);
    BoardTemp* boardTemp = new BoardTemp();
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/BOARD_TEMP", boardTemp);
    this->mapiObjects.push_back(boardTemp);
    BoardType* boardType = new BoardType();
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/BOARD_TYPE", boardType);
    this->mapiObjects.push_back(boardType);
    TriggerSign* triggerSign5 = new TriggerSign("TRIGGER5_SIGN");
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/TRIGGER5_SIGN", triggerSign5);
    this->mapiObjects.push_back(triggerSign5);
    TriggerSign* triggerSign4 = new TriggerSign("TRIGGER4_SIGN");
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/TRIGGER4_SIGN", triggerSign4);
    this->mapiObjects.push_back(triggerSign4);
    TriggerSign* triggerSign3 = new TriggerSign("TRIGGER3_SIGN");
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/TRIGGER3_SIGN", triggerSign3);
    this->mapiObjects.push_back(triggerSign3);
    TriggerSign* triggerSign2 = new TriggerSign("TRIGGER2_SIGN");
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/TRIGGER2_SIGN", triggerSign2);
    this->mapiObjects.push_back(triggerSign2);
    TriggerSign* triggerSign1 = new TriggerSign("TRIGGER1_SIGN");
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/TRIGGER1_SIGN", triggerSign1);
    this->mapiObjects.push_back(triggerSign1);
    RefreshData* refreshData = new RefreshData();
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/FIT0/REFRESH_DATA", refreshData);
    this->mapiObjects.push_back(refreshData);
    Triggers* triggers = new Triggers();
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/FIT0/TRIGGERS", triggers);
    this->mapiObjects.push_back(triggers);
    TriggerRand* triggerRand5 = new TriggerRand(std::string("TRIGGER5_RAND"));
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/TRIGGER5_RAND", triggerRand5);
    this->mapiObjects.push_back(triggerRand5);
    TriggerRand* triggerRand4 = new TriggerRand("TRIGGER4_RAND");
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/TRIGGER4_RAND", triggerRand4);
    this->mapiObjects.push_back(triggerRand4);
    TriggerRand* triggerRand3 = new TriggerRand("TRIGGER3_RAND");
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/TRIGGER3_RAND", triggerRand3);
    this->mapiObjects.push_back(triggerRand3);
    TriggerRand* triggerRand2 = new TriggerRand("TRIGGER2_RAND");
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/TRIGGER2_RAND", triggerRand2);
    this->mapiObjects.push_back(triggerRand2);
    TriggerRand* triggerRand1 = new TriggerRand(std::string("TRIGGER1_RAND"));
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/TRIGGER1_RAND", triggerRand1);
    this->mapiObjects.push_back(triggerRand1);
    PM_status* pmA0Status = new PM_status(std::string("A0"));
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/PM0A", pmA0Status);
    this->mapiObjects.push_back(pmA0Status);
    PM_status* pmA1Status = new PM_status(std::string("A1"));
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/PM1A", pmA1Status);
    this->mapiObjects.push_back(pmA1Status);
    PM_status* pmA2Status = new PM_status(std::string("A2"));
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/PM2A", pmA2Status);
    this->mapiObjects.push_back(pmA2Status);
    PM_status* pmA3Status = new PM_status(std::string("A3"));
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/PM3A", pmA3Status);
    this->mapiObjects.push_back(pmA3Status);
    PM_status* pmA4Status = new PM_status(std::string("A4"));
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/PM4A", pmA4Status);
    this->mapiObjects.push_back(pmA4Status);
    PM_status* pmA5Status = new PM_status(std::string("A5"));
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/PM5A", pmA5Status);
    this->mapiObjects.push_back(pmA5Status);
    PM_status* pmA6Status = new PM_status(std::string("A6"));
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/PM6A", pmA6Status);
    this->mapiObjects.push_back(pmA6Status);
    PM_status* pmA7Status = new PM_status(std::string("A7"));
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/PM7A", pmA7Status);
    this->mapiObjects.push_back(pmA7Status);
    PM_status* pmA8Status = new PM_status(std::string("A8"));
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/PM8A", pmA8Status);
    this->mapiObjects.push_back(pmA8Status);
    PM_status* pmA9Status = new PM_status(std::string("A9"));
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/PM9A", pmA9Status);
    this->mapiObjects.push_back(pmA9Status);
    PM_status* pmC0Status = new PM_status(std::string("C0"));
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/PM0C", pmC0Status);
    this->mapiObjects.push_back(pmC0Status);
    PM_status* pmC1Status = new PM_status(std::string("C1"));
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/PM1C", pmC1Status);
    this->mapiObjects.push_back(pmC1Status);
    PM_status* pmC2Status = new PM_status(std::string("C2"));
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/PM2C", pmC2Status);
    this->mapiObjects.push_back(pmC2Status);
    PM_status* pmC3Status = new PM_status(std::string("C3"));
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/PM3C", pmC3Status);
    this->mapiObjects.push_back(pmC3Status);
    PM_status* pmC4Status = new PM_status(std::string("C4"));
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/PM4C", pmC4Status);
    this->mapiObjects.push_back(pmC4Status);
    PM_status* pmC5Status = new PM_status(std::string("C5"));
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/PM5C", pmC5Status);
    this->mapiObjects.push_back(pmC5Status);
    PM_status* pmC6Status = new PM_status(std::string("C6"));
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/PM6C", pmC6Status);
    this->mapiObjects.push_back(pmC6Status);
    PM_status* pmC7Status = new PM_status(std::string("C7"));
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/PM7C", pmC7Status);
    this->mapiObjects.push_back(pmC7Status);
    PM_status* pmC8Status = new PM_status(std::string("C8"));
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/PM8C", pmC8Status);
    this->mapiObjects.push_back(pmC8Status);
    PM_status* pmC9Status = new PM_status(std::string("C9"));
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/PM9C", pmC9Status);
    this->mapiObjects.push_back(pmC9Status);
    Default* vTimeLow = new Default("VTIME_LOW");
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/VTIME_LOW", vTimeLow);
    this->mapiObjects.push_back(vTimeLow);
    Default* vTimeHigh = new Default("VTIME_HIGH");
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/VTIME_HIGH", vTimeHigh);
    this->mapiObjects.push_back(vTimeHigh);
    Default* scLevelA = new Default("SC_LEVEL_A");
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/SC_LEVEL_A", scLevelA);
    this->mapiObjects.push_back(scLevelA);
    Default* scLevelC = new Default("SC_LEVEL_C");
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/SC_LEVEL_C", scLevelC);
    this->mapiObjects.push_back(scLevelC);
    Default* cLevelA = new Default("C_LEVEL_A");
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/C_LEVEL_A", cLevelA);
    this->mapiObjects.push_back(cLevelA);
    Default* cLevelC = new Default("C_LEVEL_C");
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/C_LEVEL_C", cLevelC);
    this->mapiObjects.push_back(cLevelC);
    Default* trigger1Cnt = new Default("TRIGGER1_CNT");
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/TRIGGER1_CNT", trigger1Cnt);
    this->mapiObjects.push_back(trigger1Cnt);
    Default* trigger2Cnt = new Default("TRIGGER2_CNT");
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/TRIGGER2_CNT", trigger2Cnt);
    this->mapiObjects.push_back(trigger2Cnt);
    Default* trigger3Cnt = new Default("TRIGGER3_CNT");
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/TRIGGER3_CNT", trigger3Cnt);
    this->mapiObjects.push_back(trigger3Cnt);
    Default* trigger4Cnt = new Default("TRIGGER4_CNT");
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/TRIGGER4_CNT", trigger4Cnt);
    this->mapiObjects.push_back(trigger4Cnt);
    Default* trigger5Cnt = new Default("TRIGGER5_CNT");
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/TRIGGER5_CNT", trigger5Cnt);
    this->mapiObjects.push_back(trigger5Cnt);
    Default* mainPanel = new Default("MAIN_PANEL");
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/MAIN_PANEL", mainPanel);
    this->mapiObjects.push_back(mainPanel);
    Default* delayLaser = new Default("DELAY_LASER");
    this->fred->registerMapiObject("FIT_FRED/READOUTCARDS/FIT0/DELAY_LASER", delayLaser);
    this->mapiObjects.push_back(delayLaser);
}
