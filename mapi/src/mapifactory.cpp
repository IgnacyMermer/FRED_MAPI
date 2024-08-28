#include "mapifactory.h"
#include "work_status.h"
#include "board_status.h"
#include "board_type.h"
#include "Parser/utility.h"
#include "trigger_sign.h"
#include "trigger_rand.h"
#include "refresh_data.h"
#include "PM_status.h"
#include "TCM_default.h"
#include "refresh_counters.h"
#include "initFred.h"
#include "refresh_mapi_group.h"
#include "electronic_status.h"
#include "PM_default.h"
#include "refresh_PMs.h"
#include "refresh_PM_counters.h"
#include "ResetErrors.h"
#include "initGBT.h"
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include "histogramReader.h"
#include "refresh_mapi_PM_group.h"



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

    std::string fileName = "detector_type.cfg";
    boost::property_tree::ptree tree;


    if (!boost::filesystem::exists(fileName)) {
        fileName = "./configuration/" + fileName;
    }

    std::vector<std::string> prefixes = {"PMA0", "PMC0"}, addresses = {"02", "16"};

    try{
        boost::property_tree::ini_parser::read_ini(fileName, tree);

        /*const std::vector<std::string> prefixesMaxTest = {"PMA0", "PMC0","PMA1", "PMC1","PMA2", "PMC2","PMA3", "PMC3","PMA4", "PMC4","PMA5", "PMC5","PMA6", "PMC6","PMA7", "PMC7","PMA8", "PMC8","PMA9", "PMC9"};
        const std::vector<std::string> addressesMaxTest = {"02", "16","02", "16","02", "16","02", "16","02", "16","02", "16","02", "16","02", "16","02", "16","02", "16"};
        const std::vector<std::string> prefixesFT0 = {"PMA0", "PMC0","PMA1", "PMC1","PMA2", "PMC2","PMA3", "PMC3","PMA4", "PMC4","PMA5", "PMC5","PMA6", "PMC6","PMA7", "PMC7","PMA8", "PMC8"};
        const std::vector<std::string> addressesFT0 = {"02", "16", "04", "18", "06", "1A", "08", "1C", "0A", "1E", "0C", "20", "0E", "22", "10", "24", "12", "26"};
          */

        for (const auto& section : tree) {
            if(section.first=="CONFIG"){
                for (const auto& key_value : section.second) {
                    if(key_value.first=="TYPE"){
                        
                    }
                    else if(key_value.first=="PM_NAMES"){
                        vector<string> names = Utility::splitString(key_value.second.get_value<std::string>(), ",");
                        prefixes.clear();
                        for(auto name : names){
                            prefixes.push_back(name);
                        }
                    }
                    else if(key_value.first=="PM_ADDRESSES"){
                        vector<string> names = Utility::splitString(key_value.second.get_value<std::string>(), ",");
                        addresses.clear();
                        for(auto name : names){
                            addresses.push_back(name);
                        }
                    }
                }
            }
        }
    }
    catch(exception& e){
        Print::PrintInfo("error during creating sequence refresh TCM");
        Print::PrintError(e.what());
    }

    int arraySize = prefixes.size();

    WorkStatus* workStatus = new WorkStatus();
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/WORK_STATUS", workStatus);
    this->mapiObjects.push_back(workStatus);
    BoardStatus* boardStatus = new BoardStatus();
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/BOARD_STATUS", boardStatus);
    this->mapiObjects.push_back(boardStatus);
    TCM_default* delayA = new TCM_default("DELAY_A");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/DELAY_A", delayA);
    this->mapiObjects.push_back(delayA);
    TCM_default* delayC = new TCM_default("DELAY_C");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/DELAY_C", delayC);
    this->mapiObjects.push_back(delayC);
    TCM_default* temperatureBoard = new TCM_default("TEMPERATURE");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/TEMPERATURE", temperatureBoard);
    this->mapiObjects.push_back(temperatureBoard);;
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
    TCM_default* triggers = new TCM_default("TRIGGERS_OUTPUTS_MODE");
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
    TCM_default* vTimeLow = new TCM_default("VTIME_LOW");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/VTIME_LOW", vTimeLow);
    this->mapiObjects.push_back(vTimeLow);
    TCM_default* vTimeHigh = new TCM_default("VTIME_HIGH");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/VTIME_HIGH", vTimeHigh);
    this->mapiObjects.push_back(vTimeHigh);
    TCM_default* scLevelA = new TCM_default("SC_LEVEL_A");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/SC_LEVEL_A", scLevelA);
    this->mapiObjects.push_back(scLevelA);
    TCM_default* scLevelC = new TCM_default("SC_LEVEL_C");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/SC_LEVEL_C", scLevelC);
    this->mapiObjects.push_back(scLevelC);
    TCM_default* cLevelA = new TCM_default("C_LEVEL_A");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/C_LEVEL_A", cLevelA);
    this->mapiObjects.push_back(cLevelA);
    TCM_default* cLevelC = new TCM_default("C_LEVEL_C");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/C_LEVEL_C", cLevelC);
    this->mapiObjects.push_back(cLevelC);
    TCM_default* trigger1Cnt = new TCM_default("TRIGGER1_CNT");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/TRIGGER1_CNT", trigger1Cnt);
    this->mapiObjects.push_back(trigger1Cnt);
    TCM_default* trigger2Cnt = new TCM_default("TRIGGER2_CNT");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/TRIGGER2_CNT", trigger2Cnt);
    this->mapiObjects.push_back(trigger2Cnt);
    TCM_default* trigger3Cnt = new TCM_default("TRIGGER3_CNT");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/TRIGGER3_CNT", trigger3Cnt);
    this->mapiObjects.push_back(trigger3Cnt);
    TCM_default* trigger4Cnt = new TCM_default("TRIGGER4_CNT");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/TRIGGER4_CNT", trigger4Cnt);
    this->mapiObjects.push_back(trigger4Cnt);
    TCM_default* trigger5Cnt = new TCM_default("TRIGGER5_CNT");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/TRIGGER5_CNT", trigger5Cnt);
    this->mapiObjects.push_back(trigger5Cnt);
    TCM_default* mainPanel = new TCM_default("MODES_STATUS");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/MODES_STATUS", mainPanel);
    this->mapiObjects.push_back(mainPanel);
    TCM_default* delayLaser = new TCM_default("LASER_DELAY");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/LASER_DELAY", delayLaser);
    this->mapiObjects.push_back(delayLaser);
    TCM_default* sideAStatus = new TCM_default("SIDE_A_STATUS");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/SIDE_A_STATUS", sideAStatus);
    this->mapiObjects.push_back(sideAStatus);
    TCM_default* sideCStatus = new TCM_default("SIDE_C_STATUS");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/SIDE_C_STATUS", sideCStatus);
    this->mapiObjects.push_back(sideCStatus);
    TCM_default* countUpdRates = new TCM_default("COUNTERS_UPD_RATE");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/COUNTERS_UPD_RATE", countUpdRates);
    this->mapiObjects.push_back(countUpdRates);
    TCM_default* trigger1Rate = new TCM_default("TRG_C_RATE");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/TRG_C_RATE", trigger1Rate);
    this->mapiObjects.push_back(trigger1Rate);
    TCM_default* trigger2Rate = new TCM_default("TRG_SC_RATE");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/TRG_SC_RATE", trigger2Rate);
    this->mapiObjects.push_back(trigger2Rate);
    TCM_default* trigger3Rate = new TCM_default("TRG_V_RATE");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/TRG_V_RATE", trigger3Rate);
    this->mapiObjects.push_back(trigger3Rate);
    TCM_default* trigger4Rate = new TCM_default("TRG_ORC_RATE");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/TRG_ORC_RATE", trigger4Rate);
    this->mapiObjects.push_back(trigger4Rate);
    TCM_default* trigger5Rate = new TCM_default("TRG_ORA_RATE");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/TRG_ORA_RATE", trigger5Rate);
    this->mapiObjects.push_back(trigger5Rate);
    TCM_default* bkgrnd0Cnt = new TCM_default("BKGRND0_CNT");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/BKGRND0_CNT", bkgrnd0Cnt);
    this->mapiObjects.push_back(bkgrnd0Cnt);
    TCM_default* bkgrnd1Cnt = new TCM_default("BKGRND1_CNT");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/BKGRND1_CNT", bkgrnd1Cnt);
    this->mapiObjects.push_back(bkgrnd1Cnt);
    TCM_default* bkgrnd2Cnt = new TCM_default("BKGRND2_CNT");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/BKGRND2_CNT", bkgrnd2Cnt);
    this->mapiObjects.push_back(bkgrnd2Cnt);
    TCM_default* bkgrnd3Cnt = new TCM_default("BKGRND3_CNT");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/BKGRND3_CNT", bkgrnd3Cnt);
    this->mapiObjects.push_back(bkgrnd3Cnt);
    TCM_default* bkgrnd4Cnt = new TCM_default("BKGRND4_CNT");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/BKGRND4_CNT", bkgrnd4Cnt);
    this->mapiObjects.push_back(bkgrnd4Cnt);
    TCM_default* bkgrnd5Cnt = new TCM_default("BKGRND5_CNT");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/BKGRND5_CNT", bkgrnd5Cnt);
    this->mapiObjects.push_back(bkgrnd5Cnt);
    TCM_default* bkgrnd6Cnt = new TCM_default("BKGRND6_CNT");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/BKGRND6_CNT", bkgrnd6Cnt);
    this->mapiObjects.push_back(bkgrnd6Cnt);
    TCM_default* bkgrnd7Cnt = new TCM_default("BKGRND7_CNT");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/BKGRND7_CNT", bkgrnd7Cnt);
    this->mapiObjects.push_back(bkgrnd7Cnt);
    TCM_default* bkgrnd8Cnt = new TCM_default("BKGRND8_CNT");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/BKGRND8_CNT", bkgrnd8Cnt);
    this->mapiObjects.push_back(bkgrnd8Cnt);
    TCM_default* bkgrnd9Cnt = new TCM_default("BKGRND9_CNT");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/BKGRND9_CNT", bkgrnd9Cnt);
    this->mapiObjects.push_back(bkgrnd9Cnt);
    RefreshCounters* refreshCounters = new RefreshCounters();
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/REFRESH_COUNTERS", refreshCounters);
    this->mapiObjects.push_back(refreshCounters);
    TCM_default* fpgaTemp = new TCM_default("FPGA_TEMP");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/FPGA_TEMP", fpgaTemp);
    this->mapiObjects.push_back(fpgaTemp);
    TCM_default* vPower1 = new TCM_default("1VPOWER");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/1VPOWER", vPower1);
    this->mapiObjects.push_back(vPower1);
    TCM_default* vPower18 = new TCM_default("18VPOWER");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/18VPOWER", vPower18);
    this->mapiObjects.push_back(vPower18);
    TCM_default* modeSettings = new TCM_default("MODE_SETTINGS");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/MODE_SETTINGS", modeSettings);
    this->mapiObjects.push_back(modeSettings);
    TCM_default* laserDivider = new TCM_default("LASER_DIVIDER");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/LASER_DIVIDER", laserDivider);
    this->mapiObjects.push_back(laserDivider);
    TCM_default* extSwitches = new TCM_default("EXT_SW");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/EXT_SW", extSwitches);
    this->mapiObjects.push_back(extSwitches);
    TCM_default* attenuator = new TCM_default("ATTENUATOR");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/ATTENUATOR", attenuator);
    this->mapiObjects.push_back(attenuator);
    TCM_default* triggerModes = new TCM_default("MODE");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/MODE", triggerModes);
    this->mapiObjects.push_back(triggerModes);
    TCM_default* dsTriggerMask = new TCM_default("DATA_SEL_TRG_MASK");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/DATA_SEL_TRG_MASK", dsTriggerMask);
    this->mapiObjects.push_back(dsTriggerMask);
    TCM_default* bcidOffset = new TCM_default("BCID_OFFSET");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/BCID_OFFSET", bcidOffset);
    this->mapiObjects.push_back(bcidOffset);
    TCM_default* dgTriggerMask = new TCM_default("DG_TRG_RESPOND_MASK");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/DG_TRG_RESPOND_MASK", dgTriggerMask);
    this->mapiObjects.push_back(dgTriggerMask);
    TCM_default* systemId = new TCM_default("RDH_FIELDS");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/RDH_FIELDS", systemId);
    this->mapiObjects.push_back(systemId);
    /*TCM_default* trgPatternLSB = new TCM_default("TRG_CONT_PATTERN_LSB");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/TRG_CONT_PATTERN_LSB", trgPatternLSB);
    this->mapiObjects.push_back(trgPatternLSB);
    TCM_default* trgPatternMSB = new TCM_default("TRG_CONT_PATTERN_MSB");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/TRG_CONT_PATTERN_MSB", trgPatternMSB);
    this->mapiObjects.push_back(trgPatternMSB);*/
    InitFred* initFred = new InitFred();
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/INIT_FRED", initFred);
    this->mapiObjects.push_back(initFred);
    TCM_default* cruOrbit = new TCM_default("CRU_ORBIT");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/CRU_ORBIT", cruOrbit);
    this->mapiObjects.push_back(cruOrbit);
    TCM_default* cruBcFifos = new TCM_default("CRU_BC");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/CRU_BC", cruBcFifos);
    this->mapiObjects.push_back(cruBcFifos);
    TCM_default* fifoCount = new TCM_default("FIFO_COUNT");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/FIFO_COUNT", fifoCount);
    this->mapiObjects.push_back(fifoCount);
    TCM_default* selFirstHitDroppedOrbit = new TCM_default("SEL_FIRST_HIT_DROPPED_ORBIT");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/SEL_FIRST_HIT_DROPPED_ORBIT", selFirstHitDroppedOrbit);
    this->mapiObjects.push_back(selFirstHitDroppedOrbit);
    TCM_default* selLastHitDroppedOrbit = new TCM_default("SEL_LAST_HIT_DROPPED_ORBIT");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/SEL_LAST_HIT_DROPPED_ORBIT", selLastHitDroppedOrbit);
    this->mapiObjects.push_back(selLastHitDroppedOrbit);
    TCM_default* selHitsDropped = new TCM_default("SEL_HITS_DROPPED");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/SEL_HITS_DROPPED", selHitsDropped);
    this->mapiObjects.push_back(selHitsDropped);
    TCM_default* readoutRate = new TCM_default("READOUT_RATE");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/READOUT_RATE", readoutRate);
    this->mapiObjects.push_back(readoutRate);
    TCM_default* IPbusFIFOdata = new TCM_default("IPbus_FIFO_DATA");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/IPbus_FIFO_DATA", IPbusFIFOdata);
    this->mapiObjects.push_back(IPbusFIFOdata);
    TCM_default* eventsCount = new TCM_default("EVENTS_COUNT");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/EVENTS_COUNT", eventsCount);
    this->mapiObjects.push_back(eventsCount);
    TCM_default* mcodeTime = new TCM_default("MCODE_TIME");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/MCODE_TIME", mcodeTime);
    this->mapiObjects.push_back(mcodeTime);
    TCM_default* fwTime = new TCM_default("FW_TIME");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/FW_TIME", fwTime);
    this->mapiObjects.push_back(fwTime);
    RefreshMapiGroup *refreshMapiGroup = new RefreshMapiGroup(this->fred);
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/REFRESH_MAPI_GROUP",refreshMapiGroup);
    this->mapiObjects.push_back(refreshMapiGroup);
    ElectronicStatus* electronicStatus = new ElectronicStatus();
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/ELECTRONIC_STATUS", electronicStatus);
    this->mapiObjects.push_back(electronicStatus);
    TCM_default* dgBunchPattern = new TCM_default("DG_BUNCH_PATTERN");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/DG_BUNCH_PATTERN", dgBunchPattern);
    this->mapiObjects.push_back(dgBunchPattern);
    TCM_default* tgSingleValue = new TCM_default("TG_SINGLE_VALUE");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/TG_SINGLE_VALUE", tgSingleValue);
    this->mapiObjects.push_back(tgSingleValue);
    TCM_default* tgPattern1 = new TCM_default("TG_PATTERN_1");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/TG_PATTERN_1", tgPattern1);
    this->mapiObjects.push_back(tgPattern1);
    TCM_default* tgPattern0 = new TCM_default("TG_PATTERN_0");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/TG_PATTERN_0", tgPattern0);
    this->mapiObjects.push_back(tgPattern0);
    TCM_default* tgContValue = new TCM_default("TG_CONT_VALUE");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/TG_CONT_VALUE", tgContValue);
    this->mapiObjects.push_back(tgContValue);
    TCM_default* emulationRate = new TCM_default("EMULATION_RATE");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/EMULATION_RATE", emulationRate);
    this->mapiObjects.push_back(emulationRate);
    TCM_default* spiMask = new TCM_default("SPI_MASK");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/SPI_MASK", spiMask);
    this->mapiObjects.push_back(spiMask);
    TCM_default* triggersSuppressionControl = new TCM_default("TRIGGERS_SUPPRESSION_CONTROL");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/TRIGGERS_SUPPRESSION_CONTROL", triggersSuppressionControl);
    this->mapiObjects.push_back(triggersSuppressionControl);
    TCM_default* averageTime = new TCM_default("AVERAGE_TIME");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/AVERAGE_TIME", averageTime);
    this->mapiObjects.push_back(averageTime);
    TCM_default* generatorFreqOffset = new TCM_default("GENERATOR_FREQ_OFFSET");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/GENERATOR_FREQ_OFFSET", generatorFreqOffset);
    this->mapiObjects.push_back(generatorFreqOffset);
    TCM_default* tcmCountersValues = new TCM_default("TCM_COUNTERS_VALUES");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/TCM_COUNTERS_VALUES", tcmCountersValues);
    this->mapiObjects.push_back(tcmCountersValues);
    RefreshPMs* refreshPMs = new RefreshPMs();
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/REFRESH_PMS", refreshPMs);
    this->mapiObjects.push_back(refreshPMs);
    for(int j=0; j<arraySize; j++){
        PM_default*  trgSettings = new PM_default("TRG_SETTINGS", addresses[j], prefixes[j]);
        this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/TRG_SETTINGS", trgSettings);
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
            PM_default*  channelSettings = new PM_default("CHANNEL_SETTINGS_"+number, addresses[j], prefixes[j]);
            this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/CHANNEL_SETTINGS_"+number, channelSettings);
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
            PM_default*  adcBaseline = new PM_default("ADC0_BASELINE_"+number, addresses[j], prefixes[j]);
            this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/ADC0_BASELINE_"+number, adcBaseline);
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
            PM_default*  adcBaseline = new PM_default("ADC1_BASELINE_"+number, addresses[j], prefixes[j]);
            this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/ADC1_BASELINE_"+number, adcBaseline);
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
            PM_default*  adcBaseline = new PM_default("ADC0_RANGE_CORR_"+number, addresses[j], prefixes[j]);
            this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/ADC0_RANGE_CORR_"+number, adcBaseline);
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
            PM_default*  adcBaseline = new PM_default("ADC1_RANGE_CORR_"+number, addresses[j], prefixes[j]);
            this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/ADC1_RANGE_CORR_"+number, adcBaseline);
            this->mapiObjects.push_back(adcBaseline);
            
        }
        PM_default*  adcBaseline = new PM_default("TRG_CHARGE_LEVELS", addresses[j], prefixes[j]);
        this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/TRG_CHARGE_LEVELS", adcBaseline);
        this->mapiObjects.push_back(adcBaseline);
        PM_default*  tdc12PhaseTuning = new PM_default("TDC_12_PHASE_TUNING", addresses[j], prefixes[j]);
        this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/TDC_12_PHASE_TUNING", tdc12PhaseTuning);
        this->mapiObjects.push_back(tdc12PhaseTuning);
        PM_default*  tdc3PhaseTuning = new PM_default("TDC_3_PHASE_TUNING", addresses[j], prefixes[j]);
        this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/TDC_3_PHASE_TUNING", tdc3PhaseTuning);
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
            PM_default*  rawTdcData = new PM_default("RAW_TDC_DATA_"+number, addresses[j], prefixes[j]);
            this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/RAW_TDC_DATA_"+number, rawTdcData);
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
            PM_default*  adcDisp = new PM_default("ADC0_DISPERSION_"+number, addresses[j], prefixes[j]);
            this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/ADC0_DISPERSION_"+number, adcDisp);
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
            PM_default*  adcDisp = new PM_default("ADC1_DISPERSION_"+number, addresses[j], prefixes[j]);
            this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/ADC1_DISPERSION_"+number, adcDisp);
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
            PM_default*  adcMean = new PM_default("ADC0_MEAN_"+number, addresses[j], prefixes[j]);
            this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/ADC0_MEAN_"+number, adcMean);
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
            PM_default*  adcMean = new PM_default("ADC1_MEAN_"+number, addresses[j], prefixes[j]);
            this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/ADC1_MEAN_"+number, adcMean);
            this->mapiObjects.push_back(adcMean);
        }
        PM_default*  channelsMask = new PM_default("CHANNELS_MASK", addresses[j], prefixes[j]);
        this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/CHANNELS_MASK", channelsMask);
        this->mapiObjects.push_back(channelsMask);
        PM_default*  channelAdcBaseline = new PM_default("CHANNEL_ADC_BASELINE", addresses[j], prefixes[j]);
        this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/CHANNEL_ADC_BASELINE", channelAdcBaseline);
        this->mapiObjects.push_back(channelAdcBaseline);
        PM_default*  histogrammingControl = new PM_default("HISTOGRAMMING_CONTROL", addresses[j], prefixes[j]);
        this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/HISTOGRAMMING_CONTROL", histogrammingControl);
        this->mapiObjects.push_back(histogrammingControl);
        PM_default*  statusBits = new PM_default("STATUS_BITS", addresses[j], prefixes[j]);
        this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/STATUS_BITS", statusBits);
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
            PM_default*  tempMapi = new PM_default("CFD_THRESHOLD_"+number, addresses[j], prefixes[j]);
            this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/CFD_THRESHOLD_"+number, tempMapi);
            this->mapiObjects.push_back(tempMapi);
            tempMapi = new PM_default("CFD_ZERO_"+number, addresses[j], prefixes[j]);
            this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/CFD_ZERO_"+number, tempMapi);
            this->mapiObjects.push_back(tempMapi);
            tempMapi = new PM_default("ADC_ZERO_"+number, addresses[j], prefixes[j]);
            this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/ADC_ZERO_"+number, tempMapi);
            this->mapiObjects.push_back(tempMapi);
            tempMapi = new PM_default("ADC_DELAY_"+number, addresses[j], prefixes[j]);
            this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/ADC_DELAY_"+number, tempMapi);
            this->mapiObjects.push_back(tempMapi);
            tempMapi = new PM_default("THRESHOLD_CALIBRATION_"+number, addresses[j], prefixes[j]);
            this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/THRESHOLD_CALIBRATION_"+number, tempMapi);
            this->mapiObjects.push_back(tempMapi);
            
        }
        PM_default*  boardTemp = new PM_default("TEMPERATURE", addresses[j], prefixes[j]);
        this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/TEMPERATURE", boardTemp);
        this->mapiObjects.push_back(boardTemp);
        PM_default*  boardId = new PM_default("BOARD_TYPE", addresses[j], prefixes[j]);
        this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/BOARD_TYPE", boardId);
        this->mapiObjects.push_back(boardId);
        PM_default*  lastRestartReason = new PM_default("LAST_RESTART_REASON", addresses[j], prefixes[j]);
        this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/LAST_RESTART_REASON", lastRestartReason);
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
            PM_default*  tempMapi = new PM_default("COUNT_CFD_HITS_"+number, addresses[j], prefixes[j]);
            this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/COUNT_CFD_HITS_"+number, tempMapi);
            this->mapiObjects.push_back(tempMapi);
            tempMapi = new PM_default("COUNT_TRG_HITS_"+number, addresses[j], prefixes[j]);
            this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/COUNT_TRG_HITS_"+number, tempMapi);
            this->mapiObjects.push_back(tempMapi);
            tempMapi = new PM_default("RATE_CFD_HITS_"+number, addresses[j], prefixes[j]);
            this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/RATE_CFD_HITS_"+number, tempMapi);
            this->mapiObjects.push_back(tempMapi);
            tempMapi = new PM_default("RATE_TRG_HITS_"+number, addresses[j], prefixes[j]);
            this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/RATE_TRG_HITS_"+number, tempMapi);
            this->mapiObjects.push_back(tempMapi);
        }
        lastRestartReason = new PM_default("MODE_SETTINGS", addresses[j], prefixes[j]);
        this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/MODE_SETTINGS", lastRestartReason);
        this->mapiObjects.push_back(lastRestartReason);
        lastRestartReason = new PM_default("TRIGGER_RESPOND_MASK", addresses[j], prefixes[j]);
        this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/TRIGGER_RESPOND_MASK", lastRestartReason);
        this->mapiObjects.push_back(lastRestartReason);
        lastRestartReason = new PM_default("DATA_BUNCH_PATTERN", addresses[j], prefixes[j]);
        this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/DATA_BUNCH_PATTERN", lastRestartReason);
        this->mapiObjects.push_back(lastRestartReason);
        lastRestartReason = new PM_default("TRIGGER_SINGLE_VALUE", addresses[j], prefixes[j]);
        this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/TRIGGER_SINGLE_VALUE", lastRestartReason);
        this->mapiObjects.push_back(lastRestartReason);
        lastRestartReason = new PM_default("TRIGGER_CONT_PATTERN_MSB", addresses[j], prefixes[j]);
        this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/TRIGGER_CONT_PATTERN_MSB", lastRestartReason);
        this->mapiObjects.push_back(lastRestartReason);
        lastRestartReason = new PM_default("TRIGGER_CONT_PATTERN_LSB", addresses[j], prefixes[j]);
        this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/TRIGGER_CONT_PATTERN_LSB", lastRestartReason);
        this->mapiObjects.push_back(lastRestartReason);
        lastRestartReason = new PM_default("TRIGGER_CONT_VALUE", addresses[j], prefixes[j]);
        this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/TRIGGER_CONT_VALUE", lastRestartReason);
        this->mapiObjects.push_back(lastRestartReason);
        lastRestartReason = new PM_default("GENERATORS_BUNCH_FREQ", addresses[j], prefixes[j]);
        this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/GENERATORS_BUNCH_FREQ", lastRestartReason);
        this->mapiObjects.push_back(lastRestartReason);
        lastRestartReason = new PM_default("GENERATORS_FREQ_OFFSET", addresses[j], prefixes[j]);
        this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/GENERATORS_FREQ_OFFSET", lastRestartReason);
        this->mapiObjects.push_back(lastRestartReason);
        lastRestartReason = new PM_default("RDH_FIELDS1", addresses[j], prefixes[j]);
        this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/RDH_FIELDS1", lastRestartReason);
        this->mapiObjects.push_back(lastRestartReason);
        lastRestartReason = new PM_default("RDH_FIELDS2", addresses[j], prefixes[j]);
        this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/RDH_FIELDS2", lastRestartReason);
        this->mapiObjects.push_back(lastRestartReason);
        lastRestartReason = new PM_default("DELAYS", addresses[j], prefixes[j]);
        this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/DELAYS", lastRestartReason);
        this->mapiObjects.push_back(lastRestartReason);
        lastRestartReason = new PM_default("DATA_SELECT_TRG_MASK", addresses[j], prefixes[j]);
        this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/DATA_SELECT_TRG_MASK", lastRestartReason);
        this->mapiObjects.push_back(lastRestartReason);
        lastRestartReason = new PM_default("MODES_STATUS", addresses[j], prefixes[j]);
        this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/MODES_STATUS", lastRestartReason);
        this->mapiObjects.push_back(lastRestartReason);
        lastRestartReason = new PM_default("CRU_BC", addresses[j], prefixes[j]);
        this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/CRU_BC", lastRestartReason);
        this->mapiObjects.push_back(lastRestartReason);
        lastRestartReason = new PM_default("CRU_ORBIT", addresses[j], prefixes[j]);
        this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/CRU_ORBIT", lastRestartReason);
        this->mapiObjects.push_back(lastRestartReason);
        lastRestartReason = new PM_default("FIFO_COUNT", addresses[j], prefixes[j]);
        this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/FIFO_COUNT", lastRestartReason);
        this->mapiObjects.push_back(lastRestartReason);
        lastRestartReason = new PM_default("SEL_FIRST_HIT_DROPPED_ORBIT", addresses[j], prefixes[j]);
        this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/SEL_FIRST_HIT_DROPPED_ORBIT", lastRestartReason);
        this->mapiObjects.push_back(lastRestartReason);
        lastRestartReason = new PM_default("SEL_LAST_HIT_DROPPED_ORBIT", addresses[j], prefixes[j]);
        this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/SEL_LAST_HIT_DROPPED_ORBIT", lastRestartReason);
        this->mapiObjects.push_back(lastRestartReason);
        lastRestartReason = new PM_default("SEL_HITS_DROPPED", addresses[j], prefixes[j]);
        this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/SEL_HITS_DROPPED", lastRestartReason);
        this->mapiObjects.push_back(lastRestartReason);
        lastRestartReason = new PM_default("READOUT_RATE", addresses[j], prefixes[j]);
        this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/READOUT_RATE", lastRestartReason);
        this->mapiObjects.push_back(lastRestartReason);
        lastRestartReason = new PM_default("CURRENT_ADDRESS", addresses[j], prefixes[j]);
        this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/CURRENT_ADDRESS", lastRestartReason);
        this->mapiObjects.push_back(lastRestartReason);
        lastRestartReason = new PM_default("HISTOGRAM_DATA_READOUT", addresses[j], prefixes[j]);
        this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/HISTOGRAM_DATA_READOUT", lastRestartReason);
        this->mapiObjects.push_back(lastRestartReason);
        lastRestartReason = new PM_default("ATX_TIMESTAMP", addresses[j], prefixes[j]);
        this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/ATX_TIMESTAMP", lastRestartReason);
        this->mapiObjects.push_back(lastRestartReason);
        lastRestartReason = new PM_default("FW_UPGRADE_COMM", addresses[j], prefixes[j]);
        this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/FW_UPGRADE_COMM", lastRestartReason);
        this->mapiObjects.push_back(lastRestartReason);
        lastRestartReason = new PM_default("FW_UPGRADE_DATA", addresses[j], prefixes[j]);
        this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/FW_UPGRADE_DATA", lastRestartReason);
        this->mapiObjects.push_back(lastRestartReason);
        lastRestartReason = new PM_default("FW_UPGRADE_END", addresses[j], prefixes[j]);
        this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/FW_UPGRADE_END", lastRestartReason);
        this->mapiObjects.push_back(lastRestartReason);
        lastRestartReason = new PM_default("FW_UPGRADE_STATUS", addresses[j], prefixes[j]);
        this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/FW_UPGRADE_STATUS", lastRestartReason);
        this->mapiObjects.push_back(lastRestartReason);
        lastRestartReason = new PM_default("FPGA_TEMP", addresses[j], prefixes[j]);
        this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/FPGA_TEMP", lastRestartReason);
        this->mapiObjects.push_back(lastRestartReason);
        lastRestartReason = new PM_default("1VPOWER", addresses[j], prefixes[j]);
        this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/1VPOWER", lastRestartReason);
        this->mapiObjects.push_back(lastRestartReason);
        lastRestartReason = new PM_default("18VPOWER", addresses[j], prefixes[j]);
        this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/18VPOWER", lastRestartReason);
        this->mapiObjects.push_back(lastRestartReason);
        lastRestartReason = new PM_default("FPGA_TIMESTAMP", addresses[j], prefixes[j]);
        this->fred->registerMapiObject(fred->Name() + "/PM/"+prefixes[j]+"/FPGA_TIMESTAMP", lastRestartReason);
        this->mapiObjects.push_back(lastRestartReason);
    }
    TCM_default* lastPattern1 = new TCM_default("LASER_PATTERN_1");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/LASER_PATTERN_1", lastPattern1);
    this->mapiObjects.push_back(lastPattern1);
    TCM_default* lastPattern0 = new TCM_default("LASER_PATTERN_0");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/LASER_PATTERN_0", lastPattern0);
    this->mapiObjects.push_back(lastPattern1);
    RefreshPMCounters* refreshPMCounters = new RefreshPMCounters();
    this->fred->registerMapiObject(fred->Name()+"/READOUTCARDS/TCM0/REFRESH_PM_COUNTERS", refreshPMCounters);
    this->mapiObjects.push_back(refreshPMCounters);
    ResetErrors* resetErrors = new ResetErrors();
    this->fred->registerMapiObject(fred->Name()+"/READOUTCARDS/TCM0/RESET_ERRORS", resetErrors);
    this->mapiObjects.push_back(resetErrors);
    InitGBT* initGBT = new InitGBT();
    this->fred->registerMapiObject(fred->Name()+"/READOUTCARDS/TCM0/INIT_GBT", initGBT);
    this->mapiObjects.push_back(initGBT);
    HistogramReader* histogramReader = new HistogramReader();
    this->fred->registerMapiObject(fred->Name()+"/READOUTCARDS/TCM0/HISTOGRAM_READER", histogramReader);
    this->mapiObjects.push_back(histogramReader);
    RefreshMapiPMGroup* mapiPMgroup = new RefreshMapiPMGroup(this->fred);
    this->fred->registerMapiObject(fred->Name()+"/READOUTCARDS/TCM0/REFRESH_PM_MAPI_GROUP", mapiPMgroup);
    this->mapiObjects.push_back(mapiPMgroup);
}


