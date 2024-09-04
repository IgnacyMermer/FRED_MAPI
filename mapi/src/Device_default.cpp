#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include "Device_default.h"
#include "Alfred/print.h"
#include "Parser/utility.h"
#include "TCM_values.h"
#include <sstream>
#include <cmath>
#include "SWT_creator.h"
#include <bitset>

Device_default::Device_default(std::string endpoint, std::string address):endpoint(endpoint), finalValue(0), address(address) {}


string Device_default::processInputMessage(string input) {
    
    vector<string> parameters = Utility::splitString(input, ",");

    if(input.length()>5&&input.substr(0,5)=="FRED,"){
        this->publishAnswer(input.substr(5));
        noRpcRequest=true;
        return "0";
    }

    if (input == ""||input == "set"||(parameters.size()>1&&parameters[1]=="0")){
        SWT_creator::sequenceOperationRead(address, sequence);
        Print::PrintInfo(sequence);
        return sequence;
    }
    else if(parameters.size()>1&&parameters[1]=="1"){
        uint32_t num = SWT_creator::parameterValue(parameters[0]);
        bool readonly=false, wrongValue=false;
        for(std::vector<uint32_t> word : tcm.tcmWords[endpoint]){
            if(word[0]==1){
                readonly=true;
                break;
            }
            if(word[6]!=1){
                uint32_t maskTemp = ((1<<(word[2]-word[1]+1))-1)<<word[1];
                uint32_t maskNum = (num&maskTemp)>>word[1];
                if(maskNum<word[4]||maskNum>word[5]){
                    wrongValue=true;
                }
            }
        }
        if((!readonly)&&(!wrongValue)){
            SWT_creator::sequenceOperationWrite(num, address, sequence);
            Print::PrintInfo(sequence);
            return sequence;
        }
        else{
            noRpcRequest=true;
            this->publishError("Value out of correct range");
            return "";
        }
    }
    else if(parameters.size()>2&&(parameters[1]=="2"||parameters[1]=="3")){
        uint32_t index = SWT_creator::parameterValue(parameters[0]);
        uint32_t num = SWT_creator::parameterValue(parameters[2]);
        bool readonly=false, wrongValue=false, wordFound=false;
        int firstBit=0, lastBit=0;
        for(std::vector<uint32_t> word : tcm.tcmWords[endpoint]){
            if(word[1]<=index&&word[2]>=index){
                wordFound=true;
                if(word[0]==1){
                    readonly=true;
                    break;
                }
                if((num<word[4]||num>word[5])){
                    wrongValue=true;
                    break;
                }
                firstBit=word[1];
                lastBit=word[2];
                break;
            }
        }
        if((!readonly)&&(!wrongValue)&&wordFound){
            uint32_t mask = ~(((1ULL<<(lastBit-firstBit+1))-1)<<firstBit);
            SWT_creator::sequenceOperationBits(num, firstBit, mask, address, sequence);
            Print::PrintInfo(sequence);
            return sequence;
        }
        else{
            noRpcRequest=true;
            this->publishError("Value out of correct range");
            return "";
        }
    }
    else if(parameters.size()>1&&(parameters[1]=="2"||parameters[1]=="3")){
        uint32_t num = SWT_creator::parameterValue(parameters[0]);
        bool readonly=false, wrongValue=false, wordFound=false;
        for(std::vector<uint32_t> word : tcm.tcmWords[endpoint]){
            if(word[1]<=num&&word[2]>=num){
                wordFound=true;
                if(word[0]==1){
                    readonly=true;
                    break;
                }
                if(word[1]!=word[2]&&word[6]==0){
                    wrongValue=true;
                    break;
                }
                break;
            }
        }
        if((!readonly)&&(!wrongValue)&&wordFound){
            if(parameters[1]=="2"){
                SWT_creator::sequenceOperationRMWAND(num, address, sequence);
            }
            else{
                SWT_creator::sequenceOperationRMWOR(num, address, sequence);
            }
            
            Print::PrintInfo(sequence);
            return sequence;
        }
        else{
            noRpcRequest=true;
            this->publishError("Value out of correct range");
            return "";
        }
    }
}


string Device_default::processOutputMessage(string output) {
  string value;
  try {
    output.erase(remove(output.begin(), output.end(), '\n'), output.end());
    value = output.substr(output.size() - 8, output.size());
    finalValue = stoll(value, nullptr, 16);
    if(tcm.tcmEquations[endpoint].first!=""){
        std::string equation = tcm.tcmEquations[refreshServices[count].second].first;
        std::vector<std::string> paramNames = Utility::splitString(tcm.tcmEquations[refreshServices[count].second].second,";");
        std::vector<double> values = std::vector<double>{hexValue};
        returnValue = std::to_string(Utility::calculateEquation(equation,paramNames,values));
    }
    else if(endpoint.find("TRG_1_RATE")!=string::npos){
        return std::to_string(tcm.temp.trigger1rate);
    }
    else if(endpoint.find("TRG_2_RATE")!=string::npos){
        return std::to_string(tcm.temp.trigger2rate);
    }
    else if(endpoint.find("TRG_3_RATE")!=string::npos){
        return std::to_string(tcm.temp.trigger3rate);
    }
    else if(endpoint.find("TRG_4_RATE")!=string::npos){
        return std::to_string(tcm.temp.trigger4rate);
    }
    else if(endpoint.find("TRG_5_RATE")!=string::npos){
        return std::to_string(tcm.temp.trigger5rate);
    }
    else if(endpoint.find("AVERAGE_TIME")!=string::npos){
        return "";
    }
    else if(endpoint.find("LASER_DELAY")!=string::npos){
        float tempValue = stoi(value, nullptr, 16);
        if (tempValue > 10000) {
            int16_t x = stoi(value, nullptr, 16);
            tempValue=-(~x+1);
        }
        float systemClock_MHz = tcm.act.externalClock?40.0789658:40.;
        float halfBC_ns = 500. / systemClock_MHz;
        float phaseStep_ns = halfBC_ns / 
        //(SERIAL_NUM ? 
        1024 ; 
        //: 1280);
        tempValue = tempValue*phaseStep_ns;
        return std::to_string(tempValue);
    }
    else if(endpoint.find("LASER_DIVIDER")!=string::npos){
        float systemClock_MHz = tcm.act.externalClock?40.0789658:40.;
        uint32_t tempValue = std::stoll(value.substr(2,6), nullptr, 16);
        float laserFrequency = systemClock_MHz*std::pow(10,6)/(tempValue==0?1<<24:tempValue);
        updateTopicAnswer("READOUTCARDS/TCM0/LASER_FREQUENCY", std::to_string(laserFrequency));
    }
    else if(endpoint.find("DELAY_A")!=string::npos||endpoint.find("DELAY_C")!=string::npos){
        finalValue = stoi(value, nullptr, 16);
        if (finalValue > 10000) {
            int16_t x = stoi(value, nullptr, 16);
            finalValue=-(~x+1);
        }
        float systemClock_MHz = tcm.act.externalClock?40.0789658:40.;
        float halfBC_ns = 500. / systemClock_MHz;
        float phaseStep_ns = halfBC_ns / 
        //(SERIAL_NUM ? 
        1024 ; 
        //: 1280);
        return std::to_string(finalValue*phaseStep_ns);
    }
    else if(endpoint.find("BOARD_STATUS")!=string::npos){
        int pllLockC = (finalValue)&1;
        int pllLockA = (finalValue>>1)&1;
        int systemRestarted = (finalValue>>2)&1;
        int clockSrc = (finalValue>>3)&1;
        int RxReady = (finalValue>>4)&1;
        int forceLocalClock = (finalValue>>10)&1;
        tcm.act.PLLlockA=pllLockA;
        tcm.act.PLLlockC=pllLockC;
        tcm.act.systemRestarted=systemRestarted;
        tcm.act.externalClock=clockSrc;
        tcm.act.GBTRxReady=RxReady;
        tcm.act.forceLocalClock=forceLocalClock;
    }
    else if(endpoint.find("BOARD_TYPE")!=string::npos){
        value = output.substr(output.size() - 4, output.size());
        int boardBits = stoi(value, nullptr, 16);
        int detectorSubType = boardBits & 3;
        int boardSerialNr = ((boardBits>>8)&0xFF);
        tcm.act.boardType=detectorSubType;
        tcm.act.SERIAL_NUM=boardSerialNr;
    }
    noReturn=false;
  }
  catch (exception &e) {
    Print::PrintInfo(e.what());
    Print::PrintError("ERROR in connection with ALF!");
    finalValue=0;
  }
  return to_string(finalValue);
}
