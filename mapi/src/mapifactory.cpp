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
#include "default2.h"
#include "refresh_counters.h"
#include "initFred.h"
#include "refresh_mapi_group.h"
#include "electronic_status.h"
#include "refresh_mapi_counters_group.h"
#include "PM_default.h"
#include "refresh_PMs.h"


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
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/WORK_STATUS", workStatus);
    this->mapiObjects.push_back(workStatus);
    ActualValues* actualValues = new ActualValues();
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/BOARD_STATUS", actualValues);
    this->mapiObjects.push_back(actualValues);
    DelayA* delayA = new DelayA();
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/DELAY_A", delayA);
    this->mapiObjects.push_back(delayA);
    DelayC* delayC = new DelayC();
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/DELAY_C", delayC);
    this->mapiObjects.push_back(delayC);
    BoardType* boardType = new BoardType();
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/BOARD_TYPE", boardType);
    this->mapiObjects.push_back(boardType);
    TriggerSign* triggerSign5 = new TriggerSign("TRG_ORA_SIGN");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/TRG_ORA_SIGN", triggerSign5);
    this->mapiObjects.push_back(triggerSign5);
    TriggerSign* triggerSign4 = new TriggerSign("TRG_ORC_SIGN");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/TRG_ORC_SIGN", triggerSign4);
    this->mapiObjects.push_back(triggerSign4);
    TriggerSign* triggerSign3 = new TriggerSign("TRG_V_SIGN");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/TRG_V_SIGN", triggerSign3);
    this->mapiObjects.push_back(triggerSign3);
    TriggerSign* triggerSign2 = new TriggerSign("TRG_SC_SIGN");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/TRG_SC_SIGN", triggerSign2);
    this->mapiObjects.push_back(triggerSign2);
    TriggerSign* triggerSign1 = new TriggerSign("TRG_C_SIGN");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/TRG_C_SIGN", triggerSign1);
    this->mapiObjects.push_back(triggerSign1);
    RefreshData* refreshData = new RefreshData();
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/REFRESH_DATA", refreshData);
    this->mapiObjects.push_back(refreshData);
    Triggers* triggers = new Triggers();
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/TRIGGERS_OUTPUTS_MODE", triggers);
    this->mapiObjects.push_back(triggers);
    TriggerRand* triggerRand5 = new TriggerRand(std::string("TRIGGER5_RAND"));
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/TRIGGER5_RAND", triggerRand5);
    this->mapiObjects.push_back(triggerRand5);
    TriggerRand* triggerRand4 = new TriggerRand("TRIGGER4_RAND");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/TRIGGER4_RAND", triggerRand4);
    this->mapiObjects.push_back(triggerRand4);
    TriggerRand* triggerRand3 = new TriggerRand("TRIGGER3_RAND");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/TRIGGER3_RAND", triggerRand3);
    this->mapiObjects.push_back(triggerRand3);
    TriggerRand* triggerRand2 = new TriggerRand("TRIGGER2_RAND");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/TRIGGER2_RAND", triggerRand2);
    this->mapiObjects.push_back(triggerRand2);
    TriggerRand* triggerRand1 = new TriggerRand(std::string("TRIGGER1_RAND"));
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/TRIGGER1_RAND", triggerRand1);
    this->mapiObjects.push_back(triggerRand1);
    PM_status* pmA0Status = new PM_status(std::string("A0"));
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/PM_LINK_A0", pmA0Status);
    this->mapiObjects.push_back(pmA0Status);
    PM_status* pmA1Status = new PM_status(std::string("A1"));
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/PM_LINK_A1", pmA1Status);
    this->mapiObjects.push_back(pmA1Status);
    PM_status* pmA2Status = new PM_status(std::string("A2"));
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/PM_LINK_A2", pmA2Status);
    this->mapiObjects.push_back(pmA2Status);
    PM_status* pmA3Status = new PM_status(std::string("A3"));
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/PM_LINK_A3", pmA3Status);
    this->mapiObjects.push_back(pmA3Status);
    PM_status* pmA4Status = new PM_status(std::string("A4"));
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/PM_LINK_A4", pmA4Status);
    this->mapiObjects.push_back(pmA4Status);
    PM_status* pmA5Status = new PM_status(std::string("A5"));
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/PM_LINK_A5", pmA5Status);
    this->mapiObjects.push_back(pmA5Status);
    PM_status* pmA6Status = new PM_status(std::string("A6"));
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/PM_LINK_A6", pmA6Status);
    this->mapiObjects.push_back(pmA6Status);
    PM_status* pmA7Status = new PM_status(std::string("A7"));
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/PM_LINK_A7", pmA7Status);
    this->mapiObjects.push_back(pmA7Status);
    PM_status* pmA8Status = new PM_status(std::string("A8"));
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/PM_LINK_A8", pmA8Status);
    this->mapiObjects.push_back(pmA8Status);
    PM_status* pmA9Status = new PM_status(std::string("A9"));
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/PM_LINK_A9", pmA9Status);
    this->mapiObjects.push_back(pmA9Status);
    PM_status* pmC0Status = new PM_status(std::string("C0"));
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/PM_LINK_C0", pmC0Status);
    this->mapiObjects.push_back(pmC0Status);
    PM_status* pmC1Status = new PM_status(std::string("C1"));
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/PM_LINK_C1", pmC1Status);
    this->mapiObjects.push_back(pmC1Status);
    PM_status* pmC2Status = new PM_status(std::string("C2"));
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/PM_LINK_C2", pmC2Status);
    this->mapiObjects.push_back(pmC2Status);
    PM_status* pmC3Status = new PM_status(std::string("C3"));
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/PM_LINK_C3", pmC3Status);
    this->mapiObjects.push_back(pmC3Status);
    PM_status* pmC4Status = new PM_status(std::string("C4"));
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/PM_LINK_C4", pmC4Status);
    this->mapiObjects.push_back(pmC4Status);
    PM_status* pmC5Status = new PM_status(std::string("C5"));
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/PM_LINK_C5", pmC5Status);
    this->mapiObjects.push_back(pmC5Status);
    PM_status* pmC6Status = new PM_status(std::string("C6"));
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/PM_LINK_C6", pmC6Status);
    this->mapiObjects.push_back(pmC6Status);
    PM_status* pmC7Status = new PM_status(std::string("C7"));
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/PM_LINK_C7", pmC7Status);
    this->mapiObjects.push_back(pmC7Status);
    PM_status* pmC8Status = new PM_status(std::string("C8"));
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/PM_LINK_C8", pmC8Status);
    this->mapiObjects.push_back(pmC8Status);
    PM_status* pmC9Status = new PM_status(std::string("C9"));
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/PM_LINK_C9", pmC9Status);
    this->mapiObjects.push_back(pmC9Status);
    Default2* vTimeLow = new Default2("VTIME_LOW");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/VTIME_LOW", vTimeLow);
    this->mapiObjects.push_back(vTimeLow);
    Default2* vTimeHigh = new Default2("VTIME_HIGH");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/VTIME_HIGH", vTimeHigh);
    this->mapiObjects.push_back(vTimeHigh);
    Default2* scLevelA = new Default2("SC_LEVEL_A");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/SC_LEVEL_A", scLevelA);
    this->mapiObjects.push_back(scLevelA);
    Default2* scLevelC = new Default2("SC_LEVEL_C");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/SC_LEVEL_C", scLevelC);
    this->mapiObjects.push_back(scLevelC);
    Default2* cLevelA = new Default2("C_LEVEL_A");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/C_LEVEL_A", cLevelA);
    this->mapiObjects.push_back(cLevelA);
    Default2* cLevelC = new Default2("C_LEVEL_C");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/C_LEVEL_C", cLevelC);
    this->mapiObjects.push_back(cLevelC);
    Default2* trigger1Cnt = new Default2("TRIGGER1_CNT");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/TRIGGER1_CNT", trigger1Cnt);
    this->mapiObjects.push_back(trigger1Cnt);
    Default2* trigger2Cnt = new Default2("TRIGGER2_CNT");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/TRIGGER2_CNT", trigger2Cnt);
    this->mapiObjects.push_back(trigger2Cnt);
    Default2* trigger3Cnt = new Default2("TRIGGER3_CNT");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/TRIGGER3_CNT", trigger3Cnt);
    this->mapiObjects.push_back(trigger3Cnt);
    Default2* trigger4Cnt = new Default2("TRIGGER4_CNT");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/TRIGGER4_CNT", trigger4Cnt);
    this->mapiObjects.push_back(trigger4Cnt);
    Default2* trigger5Cnt = new Default2("TRIGGER5_CNT");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/TRIGGER5_CNT", trigger5Cnt);
    this->mapiObjects.push_back(trigger5Cnt);
    Default2* mainPanel = new Default2("MODES_STATUS");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/MODES_STATUS", mainPanel);
    this->mapiObjects.push_back(mainPanel);
    Default2* delayLaser = new Default2("LASER_DELAY");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/LASER_DELAY", delayLaser);
    this->mapiObjects.push_back(delayLaser);
    Default2* sideAStatus = new Default2("SIDE_A_STATUS");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/SIDE_A_STATUS", sideAStatus);
    this->mapiObjects.push_back(sideAStatus);
    Default2* sideCStatus = new Default2("SIDE_C_STATUS");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/SIDE_C_STATUS", sideCStatus);
    this->mapiObjects.push_back(sideCStatus);
    Default2* countUpdRates = new Default2("COUNTERS_UPD_RATE");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/COUNTERS_UPD_RATE", countUpdRates);
    this->mapiObjects.push_back(countUpdRates);
    Default2* trigger1Rate = new Default2("TRG_C_RATE");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/TRG_C_RATE", trigger1Rate);
    this->mapiObjects.push_back(trigger1Rate);
    Default2* trigger2Rate = new Default2("TRG_SC_RATE");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/TRG_SC_RATE", trigger2Rate);
    this->mapiObjects.push_back(trigger2Rate);
    Default2* trigger3Rate = new Default2("TRG_V_RATE");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/TRG_V_RATE", trigger3Rate);
    this->mapiObjects.push_back(trigger3Rate);
    Default2* trigger4Rate = new Default2("TRG_ORC_RATE");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/TRG_ORC_RATE", trigger4Rate);
    this->mapiObjects.push_back(trigger4Rate);
    Default2* trigger5Rate = new Default2("TRG_ORA_RATE");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/TRG_ORA_RATE", trigger5Rate);
    this->mapiObjects.push_back(trigger5Rate);
    Default2* bkgrnd0Cnt = new Default2("BKGRND0_CNT");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/BKGRND0_CNT", bkgrnd0Cnt);
    this->mapiObjects.push_back(bkgrnd0Cnt);
    Default2* bkgrnd1Cnt = new Default2("BKGRND1_CNT");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/BKGRND1_CNT", bkgrnd1Cnt);
    this->mapiObjects.push_back(bkgrnd1Cnt);
    Default2* bkgrnd2Cnt = new Default2("BKGRND2_CNT");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/BKGRND2_CNT", bkgrnd2Cnt);
    this->mapiObjects.push_back(bkgrnd2Cnt);
    Default2* bkgrnd3Cnt = new Default2("BKGRND3_CNT");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/BKGRND3_CNT", bkgrnd3Cnt);
    this->mapiObjects.push_back(bkgrnd3Cnt);
    Default2* bkgrnd4Cnt = new Default2("BKGRND4_CNT");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/BKGRND4_CNT", bkgrnd4Cnt);
    this->mapiObjects.push_back(bkgrnd4Cnt);
    Default2* bkgrnd5Cnt = new Default2("BKGRND5_CNT");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/BKGRND5_CNT", bkgrnd5Cnt);
    this->mapiObjects.push_back(bkgrnd5Cnt);
    Default2* bkgrnd6Cnt = new Default2("BKGRND6_CNT");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/BKGRND6_CNT", bkgrnd6Cnt);
    this->mapiObjects.push_back(bkgrnd6Cnt);
    Default2* bkgrnd7Cnt = new Default2("BKGRND7_CNT");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/BKGRND7_CNT", bkgrnd7Cnt);
    this->mapiObjects.push_back(bkgrnd7Cnt);
    Default2* bkgrnd8Cnt = new Default2("BKGRND8_CNT");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/BKGRND8_CNT", bkgrnd8Cnt);
    this->mapiObjects.push_back(bkgrnd8Cnt);
    Default2* bkgrnd9Cnt = new Default2("BKGRND9_CNT");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/BKGRND9_CNT", bkgrnd9Cnt);
    this->mapiObjects.push_back(bkgrnd9Cnt);
    RefreshCounters* refreshCounters = new RefreshCounters();
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/REFRESH_COUNTERS", refreshCounters);
    this->mapiObjects.push_back(refreshCounters);
    Default2* fpgaTemp = new Default2("FPGA_TEMP");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/FPGA_TEMP", fpgaTemp);
    this->mapiObjects.push_back(fpgaTemp);
    Default2* vPower1 = new Default2("1VPOWER");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/1VPOWER", vPower1);
    this->mapiObjects.push_back(vPower1);
    Default2* vPower18 = new Default2("18VPOWER");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/18VPOWER", vPower18);
    this->mapiObjects.push_back(vPower18);
    Default2* modeSettings = new Default2("MODE_SETTINGS");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/MODE_SETTINGS", modeSettings);
    this->mapiObjects.push_back(modeSettings);
    Default2* laserDivider = new Default2("LASER_DIVIDER");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/LASER_DIVIDER", laserDivider);
    this->mapiObjects.push_back(laserDivider);
    Default2* extSwitches = new Default2("EXT_SW");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/EXT_SW", extSwitches);
    this->mapiObjects.push_back(extSwitches);
    Default2* attenuator = new Default2("ATTENUATOR");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/ATTENUATOR", attenuator);
    this->mapiObjects.push_back(attenuator);
    Default2* triggerModes = new Default2("MODE");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/MODE", triggerModes);
    this->mapiObjects.push_back(triggerModes);
    Default2* dsTriggerMask = new Default2("DATA_SEL_TRG_MASK");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/DATA_SEL_TRG_MASK", dsTriggerMask);
    this->mapiObjects.push_back(dsTriggerMask);
    Default2* bcidOffset = new Default2("BCID_OFFSET");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/BCID_OFFSET", bcidOffset);
    this->mapiObjects.push_back(bcidOffset);
    Default2* dgTriggerMask = new Default2("DG_TRG_RESPOND_MASK");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/DG_TRG_RESPOND_MASK", dgTriggerMask);
    this->mapiObjects.push_back(dgTriggerMask);
    Default2* systemId = new Default2("RDH_FIELDS");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/RDH_FIELDS", systemId);
    this->mapiObjects.push_back(systemId);
    Default2* trgPatternLSB = new Default2("TRG_CONT_PATTERN_LSB");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/TRG_CONT_PATTERN_LSB", trgPatternLSB);
    this->mapiObjects.push_back(trgPatternLSB);
    Default2* trgPatternMSB = new Default2("TRG_CONT_PATTERN_MSB");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/TRG_CONT_PATTERN_MSB", trgPatternMSB);
    this->mapiObjects.push_back(trgPatternMSB);
    InitFred* initFred = new InitFred();
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/INIT_FRED", initFred);
    this->mapiObjects.push_back(initFred);
    Default2* cruOrbit = new Default2("CRU_ORBIT");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/CRU_ORBIT", cruOrbit);
    this->mapiObjects.push_back(cruOrbit);
    Default2* cruBcFifos = new Default2("CRU_BC");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/CRU_BC", cruBcFifos);
    this->mapiObjects.push_back(cruBcFifos);
    Default2* fifoCount = new Default2("FIFO_COUNT");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/FIFO_COUNT", fifoCount);
    this->mapiObjects.push_back(fifoCount);
    Default2* selFirstHitDroppedOrbit = new Default2("SEL_FIRST_HIT_DROPPED_ORBIT");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/SEL_FIRST_HIT_DROPPED_ORBIT", selFirstHitDroppedOrbit);
    this->mapiObjects.push_back(selFirstHitDroppedOrbit);
    Default2* selLastHitDroppedOrbit = new Default2("SEL_LAST_HIT_DROPPED_ORBIT");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/SEL_LAST_HIT_DROPPED_ORBIT", selLastHitDroppedOrbit);
    this->mapiObjects.push_back(selLastHitDroppedOrbit);
    Default2* selHitsDropped = new Default2("SEL_HITS_DROPPED");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/SEL_HITS_DROPPED", selHitsDropped);
    this->mapiObjects.push_back(selHitsDropped);
    Default2* readoutRate = new Default2("READOUT_RATE");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/READOUT_RATE", readoutRate);
    this->mapiObjects.push_back(readoutRate);
    Default2* IPbusFIFOdata = new Default2("IPbus_FIFO_DATA");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/IPbus_FIFO_DATA", IPbusFIFOdata);
    this->mapiObjects.push_back(IPbusFIFOdata);
    Default2* eventsCount = new Default2("EVENTS_COUNT");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/EVENTS_COUNT", eventsCount);
    this->mapiObjects.push_back(eventsCount);
    Default2* mcodeTime = new Default2("MCODE_TIME");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/MCODE_TIME", mcodeTime);
    this->mapiObjects.push_back(mcodeTime);
    Default2* fwTime = new Default2("FW_TIME");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/FW_TIME", fwTime);
    this->mapiObjects.push_back(fwTime);
    //RefreshMapiGroup *refreshMapiGroup = new RefreshMapiGroup(this->fred);
    //this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/REFRESH_MAPI_GROUP",refreshMapiGroup);
    //this->mapiObjects.push_back(refreshMapiGroup);
    //RefreshMapiCountersGroup *refreshMapiCountersGroup = new RefreshMapiCountersGroup(this->fred);
    //this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/REFRESH_MAPI_COUNTERS_GROUP",refreshMapiCountersGroup);
    //this->mapiObjects.push_back(refreshMapiCountersGroup);
    ElectronicStatus* electronicStatus = new ElectronicStatus();
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/ELECTRONIC_STATUS", electronicStatus);
    this->mapiObjects.push_back(electronicStatus);
    Default2* dgBunchPattern = new Default2("DG_BUNCH_PATTERN");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/DG_BUNCH_PATTERN", dgBunchPattern);
    this->mapiObjects.push_back(dgBunchPattern);
    Default2* tgSingleValue = new Default2("TG_SINGLE_VALUE");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/TG_SINGLE_VALUE", tgSingleValue);
    this->mapiObjects.push_back(tgSingleValue);
    Default2* tgPattern = new Default2("TG_PATTERN");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/TG_PATTERN", tgPattern);
    this->mapiObjects.push_back(tgPattern);
    Default2* tgContValue = new Default2("TG_CONT_VALUE");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/TG_CONT_VALUE", tgContValue);
    this->mapiObjects.push_back(tgContValue);
    Default2* emulationRate = new Default2("EMULATION_RATE");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/EMULATION_RATE", emulationRate);
    this->mapiObjects.push_back(emulationRate);
    Default2* spiMask = new Default2("SPI_MASK");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/SPI_MASK", spiMask);
    this->mapiObjects.push_back(spiMask);
    Default2* triggersSuppressionControl = new Default2("TRIGGERS_SUPPRESSION_CONTROL");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/TRIGGERS_SUPPRESSION_CONTROL", triggersSuppressionControl);
    this->mapiObjects.push_back(triggersSuppressionControl);
    Default2* averageTime = new Default2("AVERAGE_TIME");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/AVERAGE_TIME", averageTime);
    this->mapiObjects.push_back(averageTime);
    Default2* generatorFreqOffset = new Default2("GENERATOR_FREQ_OFFSET");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/GENERATOR_FREQ_OFFSET", generatorFreqOffset);
    this->mapiObjects.push_back(generatorFreqOffset);
    Default2* tcmCountersValues = new Default2("TCM_COUNTERS_VALUES");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/TCM_COUNTERS_VALUES", tcmCountersValues);
    this->mapiObjects.push_back(tcmCountersValues);
    RefreshPMs* refreshPMs = new RefreshPMs();
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/REFRESH_PMS", refreshPMs);
    this->mapiObjects.push_back(refreshPMs);
    PM_default* trgSettings = new PM_default("TRG_SETTINGS", "0");
    this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/TRG_SETTINGS", trgSettings);
    this->mapiObjects.push_back(trgSettings);
    for(int i=1; i<=12; i++){
        std::string number = std::to_string(i);
        if(i==10){
            number="A";
        }
        else if(i==11){
            number="B";
        }
        else if(i==12){
            number="C";
        }
        PM_default* channelSettings = new PM_default("CHANNEL_SETTINGS_"+number, "02");
        this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/CHANNEL_SETTINGS_"+number, channelSettings);
        this->mapiObjects.push_back(channelSettings);
    }
    for(int i=1; i<=12; i++){
        std::string number = std::to_string(i);
        if(i==10){
            number="A";
        }
        else if(i==11){
            number="B";
        }
        else if(i==12){
            number="C";
        }
        PM_default* adcBaseline = new PM_default("ADC0_BASELINE_"+number, "02");
        this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/ADC0_BASELINE_"+number, adcBaseline);
        this->mapiObjects.push_back(adcBaseline);
    }
    for(int i=1; i<=12; i++){
        std::string number = std::to_string(i);
        if(i==10){
            number="A";
        }
        else if(i==11){
            number="B";
        }
        else if(i==12){
            number="C";
        }
        PM_default* adcBaseline = new PM_default("ADC1_BASELINE_"+number, "02");
        this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/ADC1_BASELINE_"+number, adcBaseline);
        this->mapiObjects.push_back(adcBaseline);
    }
    for(int i=1; i<=12; i++){
        std::string number = std::to_string(i);
        if(i==10){
            number="A";
        }
        else if(i==11){
            number="B";
        }
        else if(i==12){
            number="C";
        }
        PM_default* adcBaseline = new PM_default("ADC0_RANGE_CORR_"+number, "02");
        this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/ADC0_RANGE_CORR_"+number, adcBaseline);
        this->mapiObjects.push_back(adcBaseline);
    }
    for(int i=1; i<=12; i++){
        std::string number = std::to_string(i);
        if(i==10){
            number="A";
        }
        else if(i==11){
            number="B";
        }
        else if(i==12){
            number="C";
        }
        PM_default* adcBaseline = new PM_default("ADC1_RANGE_CORR_"+number, "02");
        this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/ADC1_RANGE_CORR_"+number, adcBaseline);
        this->mapiObjects.push_back(adcBaseline);
    }
    PM_default* adcBaseline = new PM_default("TRG_CHARGE_LEVELS", "02");
    this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/TRG_CHARGE_LEVELS", adcBaseline);
    this->mapiObjects.push_back(adcBaseline);
    PM_default* tdc12PhaseTuning = new PM_default("TDC_12_PHASE_TUNING", "02");
    this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/TDC_12_PHASE_TUNING", tdc12PhaseTuning);
    this->mapiObjects.push_back(tdc12PhaseTuning);
    PM_default* tdc3PhaseTuning = new PM_default("TDC_3_PHASE_TUNING", "02");
    this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/TDC_3_PHASE_TUNING", tdc3PhaseTuning);
    this->mapiObjects.push_back(tdc3PhaseTuning);
    for(int i=1; i<=12; i++){
        std::string number = std::to_string(i);
        if(i==10){
            number="A";
        }
        else if(i==11){
            number="B";
        }
        else if(i==12){
            number="C";
        }
        PM_default* rawTdcData = new PM_default("RAW_TDC_DATA_"+number, "02");
        this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/RAW_TDC_DATA_"+number, rawTdcData);
        this->mapiObjects.push_back(rawTdcData);
    }
    for(int i=1; i<=12; i++){
        std::string number = std::to_string(i);
        if(i==10){
            number="A";
        }
        else if(i==11){
            number="B";
        }
        else if(i==12){
            number="C";
        }
        PM_default* adcDisp = new PM_default("ADC0_DISPERSION_"+number, "02");
        this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/ADC0_DISPERSION_"+number, adcDisp);
        this->mapiObjects.push_back(adcDisp);
    }
    for(int i=1; i<=12; i++){
        std::string number = std::to_string(i);
        if(i==10){
            number="A";
        }
        else if(i==11){
            number="B";
        }
        else if(i==12){
            number="C";
        }
        PM_default* adcDisp = new PM_default("ADC1_DISPERSION_"+number, "02");
        this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/ADC1_DISPERSION_"+number, adcDisp);
        this->mapiObjects.push_back(adcDisp);
    }
    for(int i=1; i<=12; i++){
        std::string number = std::to_string(i);
        if(i==10){
            number="A";
        }
        else if(i==11){
            number="B";
        }
        else if(i==12){
            number="C";
        }
        PM_default* adcMean = new PM_default("ADC0_MEAN_"+number, "02");
        this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/ADC0_MEAN_"+number, adcMean);
        this->mapiObjects.push_back(adcMean);
    }
    for(int i=1; i<=12; i++){
        std::string number = std::to_string(i);
        if(i==10){
            number="A";
        }
        else if(i==11){
            number="B";
        }
        else if(i==12){
            number="C";
        }
        PM_default* adcMean = new PM_default("ADC1_MEAN_"+number, "02");
        this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/ADC1_MEAN_"+number, adcMean);
        this->mapiObjects.push_back(adcMean);
    }
    PM_default* channelsMask = new PM_default("CHANNELS_MASK", "02");
    this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/CHANNELS_MASK", channelsMask);
    this->mapiObjects.push_back(channelsMask);
    PM_default* channelAdcBaseline = new PM_default("CHANNEL_ADC_BASELINE", "02");
    this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/CHANNEL_ADC_BASELINE", channelAdcBaseline);
    this->mapiObjects.push_back(channelAdcBaseline);
    PM_default* histogrammingControl = new PM_default("HISTOGRAMMING_CONTROL", "02");
    this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/HISTOGRAMMING_CONTROL", histogrammingControl);
    this->mapiObjects.push_back(histogrammingControl);
    PM_default* statusBits = new PM_default("STATUS_BITS", "02");
    this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/STATUS_BITS", statusBits);
    this->mapiObjects.push_back(statusBits);
    for(int i=1; i<=12; i++){
        std::string number = std::to_string(i);
        if(i==10){
            number="A";
        }
        else if(i==11){
            number="B";
        }
        else if(i==12){
            number="C";
        }
        PM_default* tempMapi = new PM_default("CFD_THRESHOLD_"+number, "02");
        this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/CFD_THRESHOLD_"+number, tempMapi);
        this->mapiObjects.push_back(tempMapi);
        tempMapi = new PM_default("CFD_ZERO_"+number, "02");
        this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/CFD_ZERO_"+number, tempMapi);
        this->mapiObjects.push_back(tempMapi);
        tempMapi = new PM_default("ADC_ZERO_"+number, "02");
        this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/ADC_ZERO_"+number, tempMapi);
        this->mapiObjects.push_back(tempMapi);
        tempMapi = new PM_default("ADC_DELAY_"+number, "02");
        this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/ADC_DELAY_"+number, tempMapi);
        this->mapiObjects.push_back(tempMapi);
        tempMapi = new PM_default("THRESHOLD_CALIBRATION_"+number, "02");
        this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/THRESHOLD_CALIBRATION_"+number, tempMapi);
        this->mapiObjects.push_back(tempMapi);
        
    }
    PM_default* boardTemp = new PM_default("BOARD_TEMPERATURE", "02");
    this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/BOARD_TEMPERATURE", boardTemp);
    this->mapiObjects.push_back(boardTemp);
    PM_default* boardId = new PM_default("BOARD_ID", "02");
    this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/BOARD_ID", boardId);
    this->mapiObjects.push_back(boardId);
    PM_default* lastRestartReason = new PM_default("LAST_RESTART_REASON", "02");
    this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/LAST_RESTART_REASON", lastRestartReason);
    this->mapiObjects.push_back(lastRestartReason);
    for(int i=1; i<=12; i++){
        std::string number = std::to_string(i);
        if(i==10){
            number="A";
        }
        else if(i==11){
            number="B";
        }
        else if(i==12){
            number="C";
        }
        PM_default* tempMapi = new PM_default("COUNT_CFD_HITS_"+number, "02");
        this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/COUNT_CFD_HITS_"+number, tempMapi);
        this->mapiObjects.push_back(tempMapi);
        tempMapi = new PM_default("COUNT_TRG_HITS_"+number, "02");
        this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/COUNT_TRG_HITS_"+number, tempMapi);
        this->mapiObjects.push_back(tempMapi);
    }
    lastRestartReason = new PM_default("MODE_SETTINGS", "02");
    this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/MODE_SETTINGS", lastRestartReason);
    this->mapiObjects.push_back(lastRestartReason);
    lastRestartReason = new PM_default("TRIGGER_RESPOND_MASK", "02");
    this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/TRIGGER_RESPOND_MASK", lastRestartReason);
    this->mapiObjects.push_back(lastRestartReason);
    lastRestartReason = new PM_default("DATA_BUNCH_PATTERN", "02");
    this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/DATA_BUNCH_PATTERN", lastRestartReason);
    this->mapiObjects.push_back(lastRestartReason);
    lastRestartReason = new PM_default("TRIGGER_SINGLE_VALUE", "02");
    this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/TRIGGER_SINGLE_VALUE", lastRestartReason);
    this->mapiObjects.push_back(lastRestartReason);
    lastRestartReason = new PM_default("TRIGGER_CONT_PATTERN_MSB", "02");
    this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/TRIGGER_CONT_PATTERN_MSB", lastRestartReason);
    this->mapiObjects.push_back(lastRestartReason);
    lastRestartReason = new PM_default("TRIGGER_CONT_PATTERN_LSB", "02");
    this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/TRIGGER_CONT_PATTERN_LSB", lastRestartReason);
    this->mapiObjects.push_back(lastRestartReason);
    lastRestartReason = new PM_default("TRIGGER_CONT_VALUE", "02");
    this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/TRIGGER_CONT_VALUE", lastRestartReason);
    this->mapiObjects.push_back(lastRestartReason);
    lastRestartReason = new PM_default("GENERATORS_BUNCH_FREQ", "02");
    this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/GENERATORS_BUNCH_FREQ", lastRestartReason);
    this->mapiObjects.push_back(lastRestartReason);
    lastRestartReason = new PM_default("GENERATORS_FREQ_OFFSET", "02");
    this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/GENERATORS_FREQ_OFFSET", lastRestartReason);
    this->mapiObjects.push_back(lastRestartReason);
    lastRestartReason = new PM_default("RDH_FIELDS1", "02");
    this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/RDH_FIELDS1", lastRestartReason);
    this->mapiObjects.push_back(lastRestartReason);
    lastRestartReason = new PM_default("RDH_FIELDS2", "02");
    this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/RDH_FIELDS2", lastRestartReason);
    this->mapiObjects.push_back(lastRestartReason);
    lastRestartReason = new PM_default("DELAYS", "02");
    this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/DELAYS", lastRestartReason);
    this->mapiObjects.push_back(lastRestartReason);
    lastRestartReason = new PM_default("DATA_SELECT_TRG_MASK", "02");
    this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/DATA_SELECT_TRG_MASK", lastRestartReason);
    this->mapiObjects.push_back(lastRestartReason);
    lastRestartReason = new PM_default("MODES_STATUS", "02");
    this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/MODES_STATUS", lastRestartReason);
    this->mapiObjects.push_back(lastRestartReason);
    lastRestartReason = new PM_default("CRU_BC", "02");
    this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/CRU_BC", lastRestartReason);
    this->mapiObjects.push_back(lastRestartReason);
    lastRestartReason = new PM_default("CRU_ORBIT", "02");
    this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/CRU_ORBIT", lastRestartReason);
    this->mapiObjects.push_back(lastRestartReason);
    lastRestartReason = new PM_default("FIFO_COUNT", "02");
    this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/FIFO_COUNT", lastRestartReason);
    this->mapiObjects.push_back(lastRestartReason);
    lastRestartReason = new PM_default("SEL_FIRST_HIT_DROPPED_ORBIT", "02");
    this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/SEL_FIRST_HIT_DROPPED_ORBIT", lastRestartReason);
    this->mapiObjects.push_back(lastRestartReason);
    lastRestartReason = new PM_default("SEL_LAST_HIT_DROPPED_ORBIT", "02");
    this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/SEL_LAST_HIT_DROPPED_ORBIT", lastRestartReason);
    this->mapiObjects.push_back(lastRestartReason);
    lastRestartReason = new PM_default("SEL_HITS_DROPPED", "02");
    this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/SEL_HITS_DROPPED", lastRestartReason);
    this->mapiObjects.push_back(lastRestartReason);
    lastRestartReason = new PM_default("READOUT_RATE", "02");
    this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/READOUT_RATE", lastRestartReason);
    this->mapiObjects.push_back(lastRestartReason);
    lastRestartReason = new PM_default("CURRENT_ADDRESS", "02");
    this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/CURRENT_ADDRESS", lastRestartReason);
    this->mapiObjects.push_back(lastRestartReason);
    lastRestartReason = new PM_default("HISTOGRAM_DATA_READOUT", "02");
    this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/HISTOGRAM_DATA_READOUT", lastRestartReason);
    this->mapiObjects.push_back(lastRestartReason);
    lastRestartReason = new PM_default("ATX_TIMESTAMP", "02");
    this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/ATX_TIMESTAMP", lastRestartReason);
    this->mapiObjects.push_back(lastRestartReason);
    lastRestartReason = new PM_default("FW_UPGRADE_COMM", "02");
    this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/FW_UPGRADE_COMM", lastRestartReason);
    this->mapiObjects.push_back(lastRestartReason);
    lastRestartReason = new PM_default("FW_UPGRADE_DATA", "02");
    this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/FW_UPGRADE_DATA", lastRestartReason);
    this->mapiObjects.push_back(lastRestartReason);
    lastRestartReason = new PM_default("FW_UPGRADE_END", "02");
    this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/FW_UPGRADE_END", lastRestartReason);
    this->mapiObjects.push_back(lastRestartReason);
    lastRestartReason = new PM_default("FW_UPGRADE_STATUS", "02");
    this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/FW_UPGRADE_STATUS", lastRestartReason);
    this->mapiObjects.push_back(lastRestartReason);
    lastRestartReason = new PM_default("FPGA_TEMPERATURE", "02");
    this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/FPGA_TEMPERATURE", lastRestartReason);
    this->mapiObjects.push_back(lastRestartReason);
    lastRestartReason = new PM_default("1VPOWER", "02");
    this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/1VPOWER", lastRestartReason);
    this->mapiObjects.push_back(lastRestartReason);
    lastRestartReason = new PM_default("18VPOWER", "02");
    this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/18VPOWER", lastRestartReason);
    this->mapiObjects.push_back(lastRestartReason);
    lastRestartReason = new PM_default("FPGA_TIMESTAMP", "02");
    this->fred->registerMapiObject(fred->Name() + "/PM/PMA0/FPGA_TIMESTAMP", lastRestartReason);
    this->mapiObjects.push_back(lastRestartReason);
    Default2* lastPattern1 = new Default2("LASER_PATTERN_1");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/LASER_PATTERN_1", lastPattern1);
    this->mapiObjects.push_back(lastPattern1);
    Default2* lastPattern0 = new Default2("LASER_PATTERN_0");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/LASER_PATTERN_0", lastPattern1);
    this->mapiObjects.push_back(lastPattern1);
    
}


