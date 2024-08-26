#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include "trigger_rand.h"
#include "Alfred/print.h"
#include "Parser/utility.h"
#include <thread>
#include <chrono>
#include "TCM_values.h"
#include <sstream>
#include <iomanip>
#include <bitset>


TriggerRand::TriggerRand(std::string endpoint) : IndefiniteMapi::IndefiniteMapi(){
    triggerEndpoint = endpoint;
}

TriggerRand::~TriggerRand(){}

void TriggerRand::processExecution(){
    bool running;
    string response;

    string request = this->waitForRequest(running); // Wait for incoming request from WinCC
    vector<string> parameters = Utility::splitString(request, ",");

    if (!running){
        return;
    }

    if (request == ""||(parameters.size()>1&&parameters[1]=="0")){
        int finalValue;
        if(triggerEndpoint=="TRIGGER5_RAND"){
            finalValue = tcm.temp.trigger5rand;
        }
        else if(triggerEndpoint=="TRIGGER4_RAND"){
            finalValue = tcm.temp.trigger4rand;
        }
        else if(triggerEndpoint=="TRIGGER3_RAND"){
            finalValue = tcm.temp.trigger3rand;
        }
        else if(triggerEndpoint=="TRIGGER2_RAND"){
            finalValue = tcm.temp.trigger2rand;
        }
        else if(triggerEndpoint=="TRIGGER1_RAND"){
            finalValue = tcm.temp.trigger1rand;
        }

        string data="";
        std::stringstream ss;
        ss << std::hex << finalValue;
        std::string hex_str = ss.str();

        for(int i=0; i<8-hex_str.length(); i++){
            data+="0";
        }
        data+=hex_str;
        this->publishAnswer(data);
    }
    else if (request == "error"){
        this->publishError("Error!");
    }
    else{
        std::string address = "0000"+tcm.addresses["READOUTCARDS/TCM0/"+triggerEndpoint];
        std::string sequence = "";
        /*if(triggerEndpoint=="TRIGGER5_RAND"){
            address="00000061";
        }
        else if(triggerEndpoint=="TRIGGER4_RAND"){
            address="00000063";
        }
        else if(triggerEndpoint=="TRIGGER3_RAND"){
            address="00000069";
        }
        else if(triggerEndpoint=="TRIGGER2_RAND"){
            address="00000065";
        }
        else if(triggerEndpoint=="TRIGGER1_RAND"){
            address="00000067";
        }*/
        
        if(parameters[1]=="1"){
            string data="";
            if(parameters[0].length()>2&&parameters[0].substr(0,2)=="0x"){
                parameters[0]=parameters[0].substr(2);
                for(int i=0; i<8-parameters[0].length(); i++){
                    data+="0";
                }
                data+=parameters[0];
                
                /*int num = std::stoi(data);

                std::stringstream ss;
                ss << std::hex << num;
                std::string hex_str = ss.str();
                for(int i=0; i<8-hex_str.length(); i++){
                    data+="0";
                }
                data+=hex_str;*/
                this->publishAnswer(data);
            }
            else{
                int num = std::stoi(parameters[0]);
                std::stringstream ss;
                ss << std::hex << num;
                std::string hex_str = ss.str();
                for(int i=0; i<8-hex_str.length(); i++){
                    data+="0";
                }
                data+=hex_str;
                this->publishAnswer(data);
            }
            sequence = "reset\n0x001"+address+data+",write\nread";
        }
        else{
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }

        std::string response = this->executeAlfSequence(sequence);

    }
}
