#include "mapifactory.h"
#include "work_status.h"
#include "Parser/utility.h"
#include "Device_default.h"
#include "initFred.h"
#include "refresh_mapi_group.h"
#include "electronic_status.h"
#include "ResetErrors.h"
#include "initGBT.h"
#include "TCM_values.h"
#include "histogramReader.h"
#include "refresh_mapi_PM_group.h"
#include "ORGate.h"
#include "refresh_mapi_PM_cnt_group.h"
#include <fstream>
#include <utility>


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

    std::string serviceName="";

    //index 0-TCM, index 1-TCM counters, index 2-PM, index 3 - PM counters
    std::vector<std::vector<std::pair<std::string, std::string>>> refreshServices;
    refreshServices.push_back(std::vector<std::pair<std::string, std::string>>());
    refreshServices.push_back(std::vector<std::pair<std::string, std::string>>());
    refreshServices.push_back(std::vector<std::pair<std::string, std::string>>());
    refreshServices.push_back(std::vector<std::pair<std::string, std::string>>());

    std::string fileName = "Devices_addresses.txt";

    std::ifstream file(fileName);
    if (!file.is_open()) {
        fileName = "./configuration/" + fileName;
        file.open(fileName);
        if(!file.is_open()){
            Print::PrintError("error while opening devices addresses configuration");
        }
    }

    std::string fileNameDevices = "Devices.txt";

    std::ifstream fileDevices(fileNameDevices);
    if (!fileDevices.is_open()) {
        fileNameDevices = "./configuration/" + fileNameDevices;
        fileDevices.open(fileNameDevices);
        if(!fileDevices.is_open()){
            Print::PrintError("error while opening devices configuration");
        }
    }

    std::map<std::string, std::string> devicesAddresses;
    std::string lineDevice;

    while(std::getline(fileDevices, lineDevice)) {
        devicesAddresses[Utility::splitString(lineDevice, ",")[0]]=Utility::splitString(lineDevice, ",")[1];
    }

    try{
        std::string line, lineWords, device="";
        while(std::getline(file, line)) {
            std::vector<std::string> parameters = Utility::splitString(line, ",");
            int wordCount=0;
            std::string previousAddress="";
            std::string fileNameWords = "Device_addresses_words.txt";
            std::ifstream fileWords(fileNameWords);
            if (!fileWords.is_open()) {
                fileNameWords = "./configuration/" + fileNameWords;
                fileWords.open(fileNameWords);
                if(!fileWords.is_open()){
                    Print::PrintError("error while opening words configuration");
                }
            }
            serviceName = (parameters[1]=="TCM"?"READOUTCARDS/TCM0/":("PM/"+parameters[1]+"/"))+parameters[3];
            while(std::getline(fileWords, lineWords)){
                std::vector<std::string> parametersWord = Utility::splitString(lineWords, ",");

                if(parametersWord[1]==parameters[1]&&parametersWord[0]==parameters[0]){
                    if(previousAddress!=parameters[0]){
                        previousAddress=parameters[0];
                        tcm.tcmWords[serviceName]=std::vector<std::vector<uint32_t>>();
                    }
                    tcm.tcmWords[serviceName].push_back(std::vector<uint32_t>());
                    for(int i=2; i<8; i++){
                        tcm.tcmWords[serviceName][wordCount].push_back(std::stoll(parametersWord[i]));
                    }
                    if(parametersWord.size()>10&&parametersWord[10]!="-"){
                        tcm.tcmEquations[serviceName]=std::make_pair(parametersWord[9], parametersWord[10]);
                    }                
                    wordCount++;
                }
            }
            Device_default* tcmDefault = new Device_default(serviceName, "0000"+devicesAddresses[parameters[1]]+parameters[0].substr(3,2));
            this->fred->registerMapiObject(fred->Name()+"/"+serviceName, tcmDefault);
            this->mapiObjects.push_back(tcmDefault);
            tcm.addresses[serviceName]=devicesAddresses[parameters[1]]+parameters[0].substr(3,2);
            int refreshServicesId = std::stoi(parameters[2]);
            if(refreshServicesId>=1&&refreshServicesId<=4){
                refreshServices[refreshServicesId-1].emplace_back("0000"+devicesAddresses[parameters[1]]+parameters[0].substr(3,2), fred->Name()+"/"+serviceName);
            }
        }
    }
    catch(exception& e){
        Print::PrintInfo("error during get all default TCM");
        Print::PrintError(e.what());
    }

    WorkStatus* workStatus = new WorkStatus();
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/WORK_STATUS", workStatus);
    this->mapiObjects.push_back(workStatus);
    InitFred* initFred = new InitFred();
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/INIT_FRED", initFred);
    this->mapiObjects.push_back(initFred);
    ElectronicStatus* electronicStatus = new ElectronicStatus();
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/ELECTRONIC_STATUS", electronicStatus);
    this->mapiObjects.push_back(electronicStatus);
    ResetErrors* resetErrors = new ResetErrors();
    this->fred->registerMapiObject(fred->Name()+"/READOUTCARDS/TCM0/RESET_ERRORS", resetErrors);
    this->mapiObjects.push_back(resetErrors);
    InitGBT* initGBT = new InitGBT();
    this->fred->registerMapiObject(fred->Name()+"/READOUTCARDS/TCM0/INIT_GBT", initGBT);
    this->mapiObjects.push_back(initGBT);
    HistogramReader* histogramReader = new HistogramReader();
    this->fred->registerMapiObject(fred->Name()+"/READOUTCARDS/TCM0/HISTOGRAM_READER", histogramReader);
    this->mapiObjects.push_back(histogramReader);
    RefreshMapiGroup *refreshMapiGroup = new RefreshMapiGroup(this->fred, refreshServices[0]);
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/REFRESH_MAPI_GROUP",refreshMapiGroup);
    this->mapiObjects.push_back(refreshMapiGroup);
    RefreshMapiPMGroup* mapiPMgroup = new RefreshMapiPMGroup(this->fred, refreshServices[2]);
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


