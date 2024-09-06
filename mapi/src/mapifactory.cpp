#include "mapifactory.h"
#include "WorkStatus.h"
#include "Parser/utility.h"
#include "Register.h"
#include "InitFred.h"
#include "RefreshMapiGroup.h"
#include "RefreshMapiCntGroup.h"
#include "ElectronicStatus.h"
#include "ResetErrors.h"
#include "InitGBT.h"
#include "tcmValues.h"
#include "HistogramReader.h"
#include "RefreshMapiPMGroup.h"
#include "ORGate.h"
#include "RefreshMapiPMCntGroup.h"
#include <fstream>
#include <utility>
#include "Database/databaseinterface.h"


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

    std::vector<std::pair<std::string, std::string>> refreshServicesTCM;
    std::vector<std::pair<std::string, std::string>> refreshServicesTCMCnt;
    std::map<std::string, std::vector<std::pair<std::string, std::string>>> refreshServicesPM;
    std::map<std::string, std::vector<std::pair<std::string, std::string>>> refreshServicesPMCnt;

    DatabaseInterface* db;
    bool dbError = false;
    //check database connection
    if(db->isConnected()){
        Print::PrintInfo("DB connected");
        bool status=false;
        try{

            Register* registerMapi = nullptr;
            //get from db register addresses with services names 
            vector<vector<MultiBase*> > registerResult = db->executeQuery("SELECT addresses.*, devices.NAME, devices.ADDRESS FROM DEVICE_ADDRESSES addresses INNER JOIN DEVICES devices ON addresses.DEVICE_ID=devices.ID", status);
            
            if(status){
                std::string previousDeviceName="";
                for(vector<MultiBase*> row : registerResult){
                    if(previousDeviceName!=row[6]->getString()&&row[6]->getString()!="TCM"){
                        previousDeviceName=row[6]->getString();
                        refreshServicesPM[row[6]->getString()]=std::vector<std::pair<std::string, std::string>>();
                        refreshServicesPMCnt[row[6]->getString()]=std::vector<std::pair<std::string, std::string>>();
                    }                      
                    int wordCount=0;

                    serviceName = (row[6]->getString()=="TCM"?"READOUTCARDS/TCM0/":("PM/"+row[6]->getString()+"/"))+row[3]->getString();

                    //get from db words for single registers
                    vector<vector<MultiBase*> > addressResult = db->executeQuery("SELECT addresses.*, devices.NAME as DEVICE_NAME, devices.ADDRESS AS DEVICE_ADDRESS FROM DEVICE_ADDRESSES_WORDS addresses INNER JOIN DEVICES devices ON addresses.device_id=devices.id WHERE addresses.DEVICE_ID="
                    +std::to_string(row[1]->getInt())+" AND addresses.ADDRESS=\'"+row[2]->getString()+"\'", status);

                    bool firstIterationAddress = true;
                    for(vector<MultiBase*> rowAddress : addressResult){
                        //save locally register words for comparing values and allow WinCC for specific operations on registers
                        if(firstIterationAddress){
                            firstIterationAddress=false;
                            tcm.tcmWords[serviceName]=std::vector<std::vector<int64_t>>();
                        }
                        
                        tcm.tcmWords[serviceName].push_back(std::vector<int64_t>());
                        
                        for(int i=1; i<8; i++){
                            tcm.tcmWords[serviceName][wordCount].push_back((rowAddress[i]->getInt()));
                        }

                        //if register have specific equations with passed values from WinCC, save locally equation with variables names
                        if(rowAddress[9]->getString()!="-"){
                            tcm.tcmEquations[serviceName]=std::make_pair(rowAddress[8]->getString(), rowAddress[9]->getString());
                        }   
                        
                        wordCount++;
                    }

                    // add service to MAPI
                    registerMapi = new Register(serviceName, "0000"+row[7]->getString()+row[2]->getString());
                    this->fred->registerMapiObject(fred->Name()+"/"+serviceName, registerMapi);
                    this->mapiObjects.push_back(registerMapi);
                    // save locally address of DIM service 
                    tcm.addresses[serviceName]=row[7]->getString()+row[2]->getString();
                    // if service must be refresh we save it to specific local refresh array

                    switch(row[4]->getInt()){
                        case 1:
                            refreshServicesTCM.emplace_back("0000"+row[7]->getString()+row[2]->getString(), fred->Name()+"/"+serviceName);
                            break;
                        case 2:
                            refreshServicesTCMCnt.emplace_back("0000"+row[7]->getString()+row[2]->getString(), fred->Name()+"/"+serviceName);
                            break;
                        case 3:
                            refreshServicesPM[row[6]->getString()].emplace_back("0000"+row[7]->getString()+row[2]->getString(), fred->Name()+"/"+serviceName);
                            break;
                        case 4:
                            refreshServicesPMCnt[row[6]->getString()].emplace_back("0000"+row[7]->getString()+row[2]->getString(), fred->Name()+"/"+serviceName);
                            break;
                    }
                }
            }
        }
        catch(exception& e){
            Print::PrintInfo("error during get all default TCM");
            Print::PrintError(e.what());
        }
    }
    else{
        dbError=true;
        Print::PrintError("Database not connected");
    }

    if(dbError){
        Print::PrintInfo("DB erorrorr");
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
            std::string line, lineWords, device="", previousDeviceName="";
            Register* registerMapi = nullptr;
            
            while(std::getline(file, line)) {
                
                std::vector<std::string> parameters = Utility::splitString(line, ",");
                int wordCount=0;
                std::string previousAddress="", fileNameWords = "Device_addresses_words.txt";
                std::ifstream fileWords(fileNameWords);
                
                if (!fileWords.is_open()) {
                    fileNameWords = "./configuration/" + fileNameWords;
                    fileWords.open(fileNameWords);
                    if(!fileWords.is_open()){
                        Print::PrintError("error while opening words configuration");
                    }
                }

                 if(previousDeviceName!=parameters[1]&&parameters[1]!="TCM"){
                    previousDeviceName=parameters[1];
                    refreshServicesPM[parameters[1]]=std::vector<std::pair<std::string, std::string>>();
                    refreshServicesPMCnt[parameters[1]]=std::vector<std::pair<std::string, std::string>>();
                }

                serviceName = (parameters[1]=="TCM"?"READOUTCARDS/TCM0/":("PM/"+parameters[1]+"/"))+parameters[3];
                
                while(std::getline(fileWords, lineWords)){
                    std::vector<std::string> parametersWord = Utility::splitString(lineWords, ",");

                    if(parametersWord[1]==parameters[1]&&parametersWord[0]==parameters[0]){
                        if(previousAddress!=parameters[0]){
                            previousAddress=parameters[0];
                            tcm.tcmWords[serviceName]=std::vector<std::vector<int64_t>>();
                        }
                        
                        tcm.tcmWords[serviceName].push_back(std::vector<int64_t>());
                        
                        for(int i=2; i<8; i++){
                            tcm.tcmWords[serviceName][wordCount].push_back(std::stoll(parametersWord[i]));
                        }
                        
                        if(parametersWord.size()>10&&parametersWord[10]!="-"){
                            tcm.tcmEquations[serviceName]=std::make_pair(parametersWord[9], parametersWord[10]);
                        }                
                        
                        wordCount++;
                    }
                }
                registerMapi = new Register(serviceName, "0000"+devicesAddresses[parameters[1]]+parameters[0]);
                this->fred->registerMapiObject(fred->Name()+"/"+serviceName, registerMapi);
                this->mapiObjects.push_back(registerMapi);

                tcm.addresses[serviceName]=devicesAddresses[parameters[1]]+parameters[0];
                int refreshServicesId = std::stoi(parameters[2]);
                switch(refreshServicesId){
                    case 1:
                        refreshServicesTCM.emplace_back("0000"+devicesAddresses[parameters[1]]+parameters[0], fred->Name()+"/"+serviceName);
                        break;
                    case 2:
                        refreshServicesTCMCnt.emplace_back("0000"+devicesAddresses[parameters[1]]+parameters[0], fred->Name()+"/"+serviceName);
                        break;
                    case 3:
                        refreshServicesPM[parameters[1]].emplace_back("0000"+devicesAddresses[parameters[1]]+parameters[0], fred->Name()+"/"+serviceName);
                        break;
                    case 4:
                        refreshServicesPMCnt[parameters[1]].emplace_back("0000"+devicesAddresses[parameters[1]]+parameters[0], fred->Name()+"/"+serviceName);
                        break;
                }
            }
        }
        catch(exception& e){
            Print::PrintInfo("error during get all default TCM");
            Print::PrintError(e.what());
        }
    }

    RefreshMapiPMGroup* mapiPMgroup = nullptr;
    for(auto& pair : refreshServicesPM){
        mapiPMgroup = new RefreshMapiPMGroup(this->fred, pair.second);
        this->fred->registerMapiObject(fred->Name()+"/PM/"+pair.first+"/REFRESH_PM_MAPI_GROUP", mapiPMgroup);
        this->mapiObjects.push_back(mapiPMgroup);
    }

    RefreshMapiPMCNTGroup* mapiPMCNTGroup = nullptr;
    for(auto& pair : refreshServicesPMCnt){
        mapiPMCNTGroup = new RefreshMapiPMCNTGroup(this->fred, pair.second);
        this->fred->registerMapiObject(fred->Name()+"/PM/"+pair.first+"/REFRESH_MAPI_PM_CNT_GROUP", mapiPMCNTGroup);
        this->mapiObjects.push_back(mapiPMCNTGroup);
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
    RefreshMapiGroup *refreshMapiGroup = new RefreshMapiGroup(this->fred, refreshServicesTCM);
    this->fred->registerMapiObject(fred->Name() + "/READOUTCARDS/TCM0/REFRESH_MAPI_GROUP",refreshMapiGroup);
    this->mapiObjects.push_back(refreshMapiGroup);
    ORGate* orGateA0 = new ORGate("A0");
    this->fred->registerMapiObject(fred->Name()+"/READOUTCARDS/TCM0/OR_GATE_A", orGateA0);
    this->mapiObjects.push_back(orGateA0);
    ORGate* orGateC0 = new ORGate("C0");
    this->fred->registerMapiObject(fred->Name()+"/READOUTCARDS/TCM0/OR_GATE_C", orGateC0);
    this->mapiObjects.push_back(orGateC0);
    RefreshMapiCNTGroup* mapiCNTGroup = new RefreshMapiCNTGroup(this->fred, refreshServicesTCMCnt);
    this->fred->registerMapiObject(fred->Name()+"/READOUTCARDS/TCM0/REFRESH_MAPI_CNT_GROUP", mapiCNTGroup);
    this->mapiObjects.push_back(mapiCNTGroup);
}


