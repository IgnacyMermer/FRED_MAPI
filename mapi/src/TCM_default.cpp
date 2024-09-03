#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include "TCM_default.h"
#include "Alfred/print.h"
#include "Parser/utility.h"
#include "TCM_values.h"
#include <sstream>
#include <cmath>
#include "SWT_creator.h"
#include <bitset>

TCM_default::TCM_default(std::string endpointParam) {
    finalValue = 0;
    endpoint=endpointParam;
}

string TCM_default::processInputMessage(string input) {
    std::string address="", sequence="";
    vector<string> parameters = Utility::splitString(input, ",");
    address = "0000"+tcm.addresses[endpoint];
    if(input.length()>5&&input.substr(0,5)=="FRED,"){
        this->publishAnswer(input.substr(5));
        noRpcRequest=true;
        return "0";
    }
    if (input == ""||input == "set"||(parameters.size()>1&&parameters[1]=="0")){
        sequence = "reset\n0x000"+address+"00000000,write\nread";
        Print::PrintInfo(sequence);
        return sequence;
    }
    else if(parameters.size()>1&&parameters[1]=="1"){
        long long num = SWT_creator::parameterValue(parameters[0]);
        bool readonly=false, wrongValue=false;
        for(std::vector<long long> word : tcm.tcmWords[endpoint]){
            if(word[0]==1){
                readonly=true;
                break;
            }
            if(word[6]!=1){
                long long maskTemp = ((1<<(word[2]-word[1]+1))-1)<<word[1];
                if(!((num&maskTemp)>>word[1]>=word[4]&&(num&maskTemp)>>word[1]<=word[5])){
                    wrongValue=true;
                }
            }
        }
        if((!readonly)&&(!wrongValue)){
            SWT_creator::sequenceOperationWrite(num, address, sequence);
            return sequence;
        }
        else{
            noRpcRequest=true;
            this->publishError("Value out of correct range");
            return "";
        }
    }
    else if(parameters.size()>2&&(parameters[1]=="2"||parameters[1]=="3")){
        long long index = SWT_creator::parameterValue(parameters[0]);
        long long num = SWT_creator::parameterValue(parameters[2]);
        bool readonly=false, wrongValue=false;
        int firstBit=0, lastBit=0;
        for(std::vector<long long> word : tcm.tcmWords[endpoint]){
            if(word[1]<=index&&word[2]>=index){
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
        if((!readonly)&&(!wrongValue)){
            uint32_t zeros = ((1ULL<<(lastBit-firstBit+1))-1)<<firstBit;
            uint32_t mask = 0xFFFFFFFF&(~zeros);
            SWT_creator::sequenceOperationBits(num, firstBit, mask, address, sequence);
            return sequence;
        }
        else{
            noRpcRequest=true;
            this->publishError("Value out of correct range");
            return "";
        }
    }
    else if(parameters.size()>1&&parameters[1]=="2"){
        long long num = SWT_creator::parameterValue(parameters[0]);
        bool readonly=false, wrongValue=false;
        for(std::vector<long long> word : tcm.tcmWords[endpoint]){
            if(word[1]<=num&&word[2]>=num){
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
        if((!readonly)&&(!wrongValue)){
            SWT_creator::sequenceOperationRMWAND(num, address, sequence);
            return sequence;
        }
        else{
            noRpcRequest=true;
            this->publishError("Value out of correct range");
            return "";
        }
    }
    else if(parameters.size()>1&&parameters[1]=="3"){
        long long num = SWT_creator::parameterValue(parameters[0]);
        bool readonly=false, wrongValue=false;
        for(std::vector<long long> word : tcm.tcmWords[endpoint]){
            if(word[1]<=num&&word[2]>=num){
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
        if((!readonly)&&(!wrongValue)){
            SWT_creator::sequenceOperationRMWOR(num, address, sequence);
            return sequence;
        }
        else{
            noRpcRequest=true;
            this->publishError("Value out of correct range");
            return "";
        }
    }
}


string TCM_default::processOutputMessage(string output) {
  string value;
  try {
    output.erase(remove(output.begin(), output.end(), '\n'), output.end());
    value = output.substr(output.size() - 8, output.size());
    finalValue = stoll(value, nullptr, 16);
    if(endpoint=="FPGA_TEMP"){
        return std::to_string(finalValue * 503.975 / 65536 - 273.15);
    }
    else if(endpoint=="1VPOWER"){
        return std::to_string(finalValue * 3 / 65536.0);
    }
    else if(endpoint=="18VPOWER"){
        return std::to_string(finalValue * 3 / 65536.0);
    }
    else if(endpoint=="TRG_1_RATE"){
        return std::to_string(tcm.temp.trigger1rate);
    }
    else if(endpoint=="TRG_2_RATE"){
        return std::to_string(tcm.temp.trigger2rate);
    }
    else if(endpoint=="TRG_3_RATE"){
        return std::to_string(tcm.temp.trigger3rate);
    }
    else if(endpoint=="TRG_4_RATE"){
        return std::to_string(tcm.temp.trigger4rate);
    }
    else if(endpoint=="TRG_5_RATE"){
        return std::to_string(tcm.temp.trigger5rate);
    }
    else if(endpoint=="TEMPERATURE"){
        return std::to_string(finalValue/10.);
    }
    else if(endpoint=="AVERAGE_TIME"){
        return "";
    }
    else if(endpoint=="LASER_DELAY"){
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
    else if(endpoint=="LASER_DIVIDER"||endpoint=="LASER_FREQUENCY"){
        float systemClock_MHz = tcm.act.externalClock?40.0789658:40.;
        long long tempValue = std::stoll(value.substr(2,6), nullptr, 16);
        float laserFrequency = systemClock_MHz*std::pow(10,6)/(tempValue==0?1<<24:tempValue);
        updateTopicAnswer("READOUTCARDS/TCM0/LASER_FREQUENCY", std::to_string(laserFrequency));
    }
    else if(endpoint=="DELAY_A"||endpoint=="DELAY_C"){
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
    else if(endpoint=="TRG_ORA_SIGN"||endpoint=="TRG_ORC_SIGN"||endpoint=="TRG_SC_SIGN"||endpoint=="TRG_C_SIGN"||endpoint=="TRG_V_SIGN"){
        return std::to_string(finalValue/128);
    }
    else if(endpoint=="BOARD_STATUS"){
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
    else if(endpoint=="BOARD_TYPE"){
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
