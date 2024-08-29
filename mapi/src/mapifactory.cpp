#include "mapifactory.h"
#include "work_status.h"
#include "board_status.h"
#include "board_type.h"
#include "Parser/utility.h"
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
#include "ORGate.h"
#include "refresh_mapi_PM_cnt_group.h"



MapiFactory::MapiFactory(Fred *fred){
    this->fred = fred;

    try{
        this->generateObjects();
    }
    catch (const exception& e){
        Print::PrintError(e.what());
        exit(EXIT_FAILURE);
    }
}

MapiFactory::~MapiFactory(){
    for (size_t i = 0; i < this->mapiObjects.size(); i++){
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
    RefreshData* refreshData = new RefreshData();
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/REFRESH_DATA", refreshData);
    this->mapiObjects.push_back(refreshData);
    PM_status* pmStatus = nullptr;
    for(int i=0; i<=9; i++){
        pmStatus = new PM_status(std::string("A"+std::to_string(i)));
        this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/PM_LINK_A"+std::to_string(i), pmStatus);
        this->mapiObjects.push_back(pmStatus);
    }
    for(int i=0; i<=9; i++){
        pmStatus = new PM_status(std::string("C"+std::to_string(i)));
        this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/PM_LINK_C"+std::to_string(i), pmStatus);
        this->mapiObjects.push_back(pmStatus);
    }
    RefreshCounters* refreshCounters = new RefreshCounters();
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/REFRESH_COUNTERS", refreshCounters);
    this->mapiObjects.push_back(refreshCounters);
    InitFred* initFred = new InitFred();
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/INIT_FRED", initFred);
    this->mapiObjects.push_back(initFred);
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
                        PM_default* pmDefault = new PM_default(key_value.second.get_value<std::string>(), prefixes[j]);
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
    RefreshMapiGroup *refreshMapiGroup = new RefreshMapiGroup(this->fred);
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/REFRESH_MAPI_GROUP",refreshMapiGroup);
    this->mapiObjects.push_back(refreshMapiGroup);
    RefreshMapiPMGroup* mapiPMgroup = new RefreshMapiPMGroup(this->fred);
    this->fred->registerMapiObject(fred->Name()+"/READOUTCARDS/TCM0/REFRESH_PM_MAPI_GROUP", mapiPMgroup);
    this->mapiObjects.push_back(mapiPMgroup);
    ORGate* orGateA0 = new ORGate("A0");
    this->fred->registerMapiObject(fred->Name()+"/READOUTCARDS/TCM0/OR_GATE_A", orGateA0);
    this->mapiObjects.push_back(orGateA0);
    ORGate* orGateC0 = new ORGate("C0");
    this->fred->registerMapiObject(fred->Name()+"/READOUTCARDS/TCM0/OR_GATE_C", orGateC0);
    this->mapiObjects.push_back(orGateC0);
    RefreshMapiPMCNTGroup* mapiPMCNTGroup = new RefreshMapiPMCNTGroup(this->fred);
    this->fred->registerMapiObject(fred->Name()+"/READOUTCARDS/TCM0/REFRESH_MAPI_PM_CNT_GROUP", mapiPMCNTGroup);
    this->mapiObjects.push_back(mapiPMCNTGroup);
}


