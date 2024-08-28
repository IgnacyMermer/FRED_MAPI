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


    std::string serviceName=fred->Name()+"/READOUTCARDS/TCM0/";

    fileName = "TCM_defaults.cfg";

    if (!boost::filesystem::exists(fileName)) {
        fileName = "./configuration/" + fileName;
    }

    try{
        boost::property_tree::ini_parser::read_ini(fileName, tree);
        
        for (const auto& section : tree) {
            if(section.first=="TCM"){
                for (const auto& key_value : section.second) {
                    TCM_default* tcmDefault = new TCM_default(key_value.second.get_value<std::string>());
                    this->fred->registerMapiObject(serviceName+key_value.second.get_value<std::string>(), tcmDefault);
                    this->mapiObjects.push_back(tcmDefault);
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
    RefreshCounters* refreshCounters = new RefreshCounters();
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/REFRESH_COUNTERS", refreshCounters);
    this->mapiObjects.push_back(refreshCounters);
    /*TCM_default* trgPatternLSB = new TCM_default("TRG_CONT_PATTERN_LSB");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/TRG_CONT_PATTERN_LSB", trgPatternLSB);
    this->mapiObjects.push_back(trgPatternLSB);
    TCM_default* trgPatternMSB = new TCM_default("TRG_CONT_PATTERN_MSB");
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/TRG_CONT_PATTERN_MSB", trgPatternMSB);
    this->mapiObjects.push_back(trgPatternMSB);*/
    InitFred* initFred = new InitFred();
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/INIT_FRED", initFred);
    this->mapiObjects.push_back(initFred);
    RefreshMapiGroup *refreshMapiGroup = new RefreshMapiGroup(this->fred);
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/REFRESH_MAPI_GROUP",refreshMapiGroup);
    this->mapiObjects.push_back(refreshMapiGroup);
    ElectronicStatus* electronicStatus = new ElectronicStatus();
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/ELECTRONIC_STATUS", electronicStatus);
    this->mapiObjects.push_back(electronicStatus);
    RefreshPMs* refreshPMs = new RefreshPMs();
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/REFRESH_PMS", refreshPMs);
    this->mapiObjects.push_back(refreshPMs);
    for(int j=0; j<arraySize; j++){
        std::string serviceName=fred->Name()+"/PM/"+prefixes[j]+"/";

        fileName = "PM_defaults.cfg";

        if (!boost::filesystem::exists(fileName)) {
            fileName = "./configuration/" + fileName;
        }

        try{
            boost::property_tree::ini_parser::read_ini(fileName, tree);
            
            for (const auto& section : tree) {
                if(section.first.substr(0,2)=="PM"){
                    for (const auto& key_value : section.second) {
                        PM_default* pmDefault = new PM_default(key_value.second.get_value<std::string>(), addresses[j], prefixes[j]);
                        this->fred->registerMapiObject(serviceName+key_value.second.get_value<std::string>(), pmDefault);
                        this->mapiObjects.push_back(pmDefault);
                    }
                }
            }
        }
        catch(exception& e){
            Print::PrintInfo("error during creating sequence refresh TCM");
            Print::PrintError(e.what());
        }
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


