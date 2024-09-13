#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include "ConfigurationApply.h"
#include "Alfred/print.h"
#include "Parser/utility.h"
#include "registerData.h"
#include <sstream>
#include "swtCreator.h"
#include "WinccMessage.h"
#include "Database/databaseinterface.h"


ConfigurationApply::ConfigurationApply() : IndefiniteMapi::IndefiniteMapi()
{}

ConfigurationApply::~ConfigurationApply(){}
    
void ConfigurationApply::processExecution(){
    bool running;
    string response;

    string request = this->waitForRequest(running);
    WinccMessage messageFromWinCC(request);
    std::string sequence = "reset\n0x0000000000000000000,write\nread\n0x0000000000100000000,write\nread";

    response = this->executeAlfSequence(sequence);

    uint32_t delayA = 0, delayC = 0;
    try{
        response.erase(remove(response.begin(), response.end(), '\n'), response.end());
        response = response.substr(8);
        delayA = stoll(response.substr(13, 8), nullptr, 16);
        response = response.substr(22);
        delayC = stoll(response.substr(13, 8), nullptr, 16);
    }
    catch(exception& e){
        Print::PrintError(e.what());
    }

    if(messageFromWinCC.writeMessage()){
        int64_t value = messageFromWinCC.getValueWriteMessage();
        DatabaseInterface* db;
        if(db->isConnected()){

            vector<vector<MultiBase*>> configurationDevices = db->executeQuery(std::string("SELECT * FROM CONFIGURATION_DEVICES conf_devices ") + 
                std::string("INNER JOIN DEVICES devices ON devices.NAME=conf_devices.DEVICE_NAME ") +
                std::string("WHERE conf_devices.CONFIGURATION_ID=")+std::to_string(value));
            
            for(const auto& rowDevice : configurationDevices){    
                std::map<int, uint32_t> confParameters;
                int countersUpdRate=-1;
                //isTCM()
                if(rowDevice[5]->getInt()==1){
                    vector<vector<MultiBase*>> configurationValues = db->executeQuery(std::string("SELECT conf_values.*, devices.ADDRESS AS DEVICE_ADDRESS FROM CONFIGURATION_VALUES conf_values ")+
                        std::string("INNER JOIN CONFIGURATION_DEVICES conf_devices ON conf_values.CONFIGURATION_ID=conf_devices.ID ") + 
                        std::string("INNER JOIN DEVICES devices ON devices.NAME=conf_devices.DEVICE_NAME ") +
                        std::string("WHERE devices.IS_TCM=1 AND conf_devices.CONFIGURATION_ID=")+std::to_string(value));

                    for(const auto& rowValue : configurationValues){
                        int address = std::stoi(rowValue[1]->getString(), nullptr, 16);
                        uint32_t value = std::stoll(rowValue[2]->getString(), nullptr, 16);
                        confParameters[address] = value;
                    }
                    sequence="reset";
                    if(confParameters.find(0)!=confParameters.end()&&confParameters.find(1)!=confParameters.end()){
                        uint32_t delay = std::max(std::max((int)(confParameters[0]-delayA), (int)(confParameters[1]-delayC)), 0);
                        Print::PrintInfo("delay: "+std::to_string(delay));
                        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
                        SwtCreator::sequenceOperationWrite(confParameters[0], "00000000", sequence);
                        SwtCreator::sequenceOperationWrite(confParameters[1], "00000001", sequence);
                        Print::PrintInfo(sequence);
                        //this->executeAlfSequence(sequence);
                    }
                    else if(confParameters.find(0)!=confParameters.end()){
                        uint32_t delay = std::max((int)(confParameters[0]-delayA), 0);
                        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
                        SwtCreator::sequenceOperationWrite(confParameters[0], "00000000", sequence);
                        
                    }
                    else if(confParameters.find(1)!=confParameters.end()){
                        uint32_t delay = std::max(0, (int)(confParameters[1]-delayC));
                        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
                        SwtCreator::sequenceOperationWrite(confParameters[0], "00000001", sequence);
                    }

                    if(confParameters.find(0x50)!=confParameters.end()){
                        countersUpdRate=confParameters[0x50];
                        confParameters.erase(0x50);
                    }

                    if(confParameters.find(0xE)!=confParameters.end()){
                        int temp = confParameters[0xE]&0x0000030F;
                        sequence = "reset\n0x0020000000EFFFFFCF0,write\nread\n0x0030000000E"+SwtCreator::create8CharHex(temp)+",write\nread";
                        confParameters.erase(0xE);
                    }

                    for (const auto& pair : confParameters) {
                        SwtCreator::sequenceOperationWrite(pair.second, SwtCreator::create8CharHex(pair.first), sequence);
                    }
                    Print::PrintInfo(sequence);
                    //this->executeAlfSequence(sequence);
                    
                    //check if contains CH_MASK_A or CH_MASK_C
                    if(confParameters.find(0x1A)!=confParameters.end()||confParameters.find(0x3A)!=confParameters.end()){
                        std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    }

                    //reset errors
                    sequence="reset\n0x002000000D8FFFF00FF,write\nread\n0x003000000D800000000,write\nread";
                    sequence+="\n0x002000000D8FFFFFFFF,write\nread\n0x003000000D80000C800,write\nread";
                    sequence+="\n0x002000000D8FFBF00FF,write\nread\n0x003000000D800000000,write\nread";
                    
                    //add system restart operation

                    Print::PrintInfo(sequence);
                    //this->executeAlfSequence(sequence);

                    //add reset counters, rates, times
                    if(countersUpdRate!=-1){
                        sequence="reset";
                        SwtCreator::sequenceOperationWrite(countersUpdRate, "00000050", sequence);
                        Print::PrintInfo(sequence);
                        //this->executeAlfSequence(sequence);
                    }
                                        
                    Print::PrintInfo(sequence);
                }
                else{
                    Print::PrintInfo("else");
                    uint16_t pmAddress = std::stoi(rowDevice[4]->getString(), nullptr, 16)<<8;
                    vector<vector<MultiBase*>> configurationValues = db->executeQuery(std::string("SELECT conf_values.*, devices.ADDRESS AS DEVICE_ADDRESS FROM CONFIGURATION_VALUES conf_values ")+
                        std::string("INNER JOIN CONFIGURATION_DEVICES conf_devices ON conf_values.CONFIGURATION_ID=conf_devices.ID ") + 
                        std::string("INNER JOIN DEVICES devices ON devices.NAME=conf_devices.DEVICE_NAME ") +
                        std::string("WHERE devices.Name=\'"+rowDevice[2]->getString()+"\' AND conf_devices.CONFIGURATION_ID=")+std::to_string(value));

                    for(const auto& rowValue : configurationValues){
                        int address = std::stoi(rowValue[1]->getString(), nullptr, 16);
                        uint32_t value = std::stoll(rowValue[2]->getString(), nullptr, 16);
                        confParameters[address] = value;
                    }

                    sequence="reset\n0x0000000001E00000000,write\nread";
                    response = this->executeAlfSequence(sequence);
                    Print::PrintInfo(response);
                    if(response=="failure"){
                        this->publishError("Wrong during get spi mask");
                        return;
                    }
                    response.erase(remove(response.begin(), response.end(), '\n'), response.end());
                    response = response.substr(8);
                    uint32_t pmsStatus = stoll(response.substr(13, 8), nullptr, 16);
                    sequence="reset";
                    if((pmsStatus>>((std::stoi(rowDevice[4]->getString(), nullptr, 16)/2)-1))&1==1){
                        Print::PrintInfo(rowDevice[4]->getString());
                        for (const auto& pair : confParameters) {
                            SwtCreator::sequenceOperationWrite(pair.second, SwtCreator::create8CharHex(pair.first+pmAddress), sequence);
                        }
                    }
                    Print::PrintInfo("PM");
                    Print::PrintInfo(sequence);
                }
            }
        }
        else{
            this->publishError("Database not connected");
        }
    }
}

