#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include "default2.h"
#include "Alfred/print.h"
#include "Parser/utility.h"
#include "TCM_values.h"
#include <sstream>
#include <cmath>


Default2::Default2(std::string endpointParam) {
    finalValue = 0;
    endpoint=endpointParam;
}

void Default2::sequenceOperation1(long long num, std::string address, std::string& sequence){
    std::stringstream ss;
    ss << std::hex << num;
    std::string hex_str = ss.str();
    std::string data="";
    for(int i=0; i<8-hex_str.length(); i++){
        data+="0";
    }
    data+=hex_str;
    sequence="reset\n0x001"+address+data+",write\nread";
}

void Default2::sequenceOperation2(int num, std::string address, std::string& sequence){
    int temp = 0xFFFFFFFF;
    temp-=(1 << num);
    std::stringstream ss;
    ss << std::hex << temp;
    std::string data = ss.str();
    sequence = "reset\n0x002"+address+data+",write\nread\n0x003"+address+"00000000,write\nread\n0x000"+address+"00000000,write\nread";
}

void Default2::sequenceOperation3(int num, std::string address, std::string& sequence){
    std::stringstream ss;
    ss << std::hex << (1 << num);
    std::string hex_str = ss.str();
    std::string data="";
    for(int i=0; i<8-hex_str.length(); i++){
        data+="0";
    }
    data+=hex_str;
    sequence = "reset\n0x002"+address+"FFFFFFFF,write\nread\n0x003"+address+data+",write\nread\n0x000"+address+"00000000,write\nread";
}

void Default2::sequenceOperationBits(int num, int power, int maskNumber, std::string address, std::string& sequence){
    std::stringstream ss;
    ss << std::hex << maskNumber;
    std::string hex_str = ss.str();
    std::string data="";
    for(int i=0; i<8-hex_str.length(); i++){
        data+="0";
    }
    data+=hex_str;
    long long temp=num*std::pow(2,power);
    std::stringstream ss2;
    ss2 << std::hex << temp;
    std::string data2 = "";
    hex_str = ss2.str();
    for(int i=0; i<8-hex_str.length(); i++){
        data2+="0";
    }
    data2+=hex_str;
    sequence = "reset\n0x002"+address+data+",write\nread\n0x003"+address+data2+",write\nread\n0x000"+address+"00000000,write\nread";
}

long long Default2::parameterValue(std::string strValue){
    if(strValue.rfind("0x", 0)==0){
        return std::stoll(strValue.substr(2), nullptr, 16);
    }
    else{
        return std::stoll(strValue);
    }
}

string Default2::processInputMessage(string input) {
    std::string address="", sequence="";
    vector<string> parameters = Utility::splitString(input, ",");

    if(endpoint=="VTIME_LOW"||endpoint=="VTIME_HIGH"){
        if(endpoint=="VTIME_HIGH"){ address="00000009";}
        else if(endpoint=="VTIME_LOW"){ address="00000008";}
        if (input == ""||input == "set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            long long num = parameterValue(parameters[0]);
            if(num<-512||num>511){
                noRpcRequest=true;
                this->publishError("Value out of correct range");
                return "";
            }
            sequenceOperation1(num, address, sequence);
        }
        else{
            noRpcRequest=true;
            this->publishError("Parameter can be only read or write");
            sequence="";
        }
        return sequence;
    }
    else if(endpoint=="SC_LEVEL_A"||endpoint=="SC_LEVEL_C"||endpoint=="C_LEVEL_A"||endpoint=="C_LEVEL_C"){
        if(endpoint=="SC_LEVEL_C"){ address="0000000B";}
        else if(endpoint=="SC_LEVEL_A"){ address="0000000A";}
        else if(endpoint=="C_LEVEL_A"){ address="0000000C";}
        else if(endpoint=="C_LEVEL_C"){ address="0000000D";}

        if (input == ""||input == "set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            long long num = parameterValue(parameters[0]);
            if(num<0||num>65535){
                noRpcRequest=true;
                this->publishError("Value out of correct range");
                return "";
            }
            sequenceOperation1(num, address, sequence);
        }
        else{
            this->publishError("Parameter can be only read or write");
            sequence="";
            noRpcRequest=true;
        }
        return sequence;
    }
    else if(endpoint=="LASER_DELAY"){
        address="00000002";
        if (input == ""||input == "set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            float num = parameterValue(parameters[0]);
            float systemClock_MHz = tcm.act.externalClock?40.0789658:40.;
            float halfBC_ns = 500. / systemClock_MHz;
            float phaseStep_ns = halfBC_ns / (
            //SERIAL_NUM ? 
            1024 
            //: 1280
            );
            num = num / phaseStep_ns;
            if(num<-1024||num>1024){
                this->publishError("Value out of correct range");
                noRpcRequest=true;
                return "";  
            }
            sequenceOperation1((long long) num, address, sequence);
        }
        else{
            this->publishError("Parameter can be only read or write");
            sequence="";
            noRpcRequest=true;
        }
        return sequence;
    }
    else if(endpoint=="ATTENUATOR"){
        address="00000003";
        int index = std::stoi(parameters[0]);
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            sequenceOperation1(parameterValue(parameters[0]), address, sequence);
        }
        else{
            this->publishError("wrong parameters");
            sequence = "";
            noRpcRequest=true;
        }
        return sequence;
    }
    else if(endpoint=="EXT_SW"||endpoint=="SIDE_A_STATUS"||endpoint=="SIDE_C_STATUS"){
        if(endpoint=="EXT_SW"){ address="00000004";}
        else if(endpoint=="SIDE_A_STATUS"){ address="0000001A";}
        else if(endpoint=="SIDE_C_STATUS"){ address="0000003A";}

        if (input == ""||input == "set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            sequenceOperation1(parameterValue(parameters[0]), address, sequence);
        }
        else if(parameters.size()>1&&parameters[1]=="3"){
            sequenceOperation3((int)parameterValue(parameters[0]), address, sequence);
        }
        else if(parameters.size()>1&&parameters[1]=="2"){
            sequenceOperation2((int)parameterValue(parameters[0]), address, sequence);
        }
        else{
            this->publishError("Parameter can be only read or write or RMW");
            sequence="";
            noRpcRequest=true;
        }
        return sequence;
    }
    else if(endpoint=="MODE"){
        std::string hex_str = "";
        address="0000000E";
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            sequenceOperation1(parameterValue(parameters[0]), address, sequence);
        }
        else if(parameters.size()>2&&(parameters[0]=="1"||parameters[0]=="2")){
            sequenceOperationBits(parameterValue(parameters[2]), 1, 0xFFFFFFF9, address, sequence);
        }
        else if(parameters.size()>2&&(parameters[0]=="4"||parameters[0]=="5"||parameters[0]=="6"||parameters[0]=="7")){
            sequenceOperationBits(parameterValue(parameters[2]), 4, 0xFFFFFF0F, address, sequence);
        }
        else if(parameters.size()>1&&parameters[1]=="3"){
            sequenceOperation3((int)parameterValue(parameters[0]), address, sequence);
        }
        else if(parameters.size()>1&&parameters[1]=="2"){
            sequenceOperation2((int)parameterValue(parameters[0]), address, sequence);
        }
        else{
            this->publishError("wrong parameters");
            noRpcRequest=true;
            sequence="";
        }
        return sequence;
    }
    else if(endpoint=="MODE_SETTINGS"){
        address="000000D8";
        std::string hex_str = "";
        int num = parameterValue(parameters[0]);
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            sequenceOperation1(parameterValue(parameters[0]), address, sequence);
        }
        else if(parameters.size()>2&&(num>=0&&num<=3)){
            sequenceOperationBits(parameterValue(parameters[2]), 0, 0xFFFFFFF0, address, sequence);
        }
        else if(parameters.size()>2&&(num>=4&&num<=7)){
            sequenceOperationBits(parameterValue(parameters[2]), 4, 0xFFFFFF0F, address, sequence);
        }
        else if(parameters.size()>2&&(num>=16&&num<=19)){
            sequenceOperationBits(parameterValue(parameters[2]), 16, 0xFFF0FFFF, address, sequence);
        }
        else if(parameters.size()>1&&parameters[1]=="3"){
            sequenceOperation3((int)parameterValue(parameters[0]), address, sequence);
        }
        else if(parameters.size()>1&&parameters[1]=="2"){
            sequenceOperation2((int)parameterValue(parameters[0]), address, sequence);
        }
        else{
            sequence="";
            noRpcRequest=true;
        }
        return sequence;
    }
    else if(endpoint=="LASER_DIVIDER"){

        //do sprawdzenia wartosc wyswietlana

        address="0000001B";
        std::string hex_str = "";
        int index = std::stoi(parameters[0]);
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            sequenceOperation1(parameterValue(parameters[0]), address, sequence);
        }
        else if(parameters.size()>2&&(index>=0&&index<=23)){
            sequenceOperationBits(parameterValue(parameters[2]), 0, 0xFF000000, address, sequence);
        }
        else if(parameters.size()>2&&(index>=25&&index<=29)){
            sequence="";
            noRpcRequest=true;
        }
        else if(parameters.size()>1&&parameters[1]=="3"){
            sequenceOperation3((int)parameterValue(parameters[0]), address, sequence);
        }
        else if(parameters.size()>1&&parameters[1]=="2"){
            sequenceOperation2((int)parameterValue(parameters[0]), address, sequence);
        }
        else{
            sequence="";
            noRpcRequest=true;
            this->publishError("wrong parameters");
        }
        return sequence;
    }
    else if(endpoint=="DATA_SEL_TRG_MASK"||endpoint=="BCID_OFFSET"||endpoint=="DG_TRG_RESPOND_MASK"||endpoint=="RDH_FIELDS"){
        if(endpoint=="BCID_OFFSET"){ address="000000E3";}
        else if(endpoint=="DATA_SEL_TRG_MASK"){ address="000000E4";}
        else if(endpoint=="DG_TRG_RESPOND_MASK"){ address="000000D9";}
        else if(endpoint=="RDH_FIELDS"){ address="000000E1";}
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            sequenceOperation1(parameterValue(parameters[0]), address, sequence);
        }
        else{
            this->publishError("wrong parameters");
            sequence="";
            noRpcRequest=true;
        }
        return sequence;
    }
    else if(endpoint=="CRU_ORBIT"||endpoint=="CRU_BC"||endpoint=="FIFO_COUNT"||endpoint=="SEL_FIRST_HIT_DROPPED_ORBIT"||endpoint=="SEL_LAST_HIT_DROPPED_ORBIT"||endpoint=="SEL_HITS_DROPPED"
    ||endpoint=="READOUT_RATE"||endpoint=="IPbus_FIFO_DATA"||endpoint=="EVENTS_COUNT"||endpoint=="MCODE_TIME"||endpoint=="FW_TIME"||endpoint=="TEMPERATURE"||endpoint=="MODES_STATUS"
    ||endpoint=="FPGA_STATUS"||endpoint=="1VPOWER"||endpoint=="18VPOWER"){
        if(endpoint=="CRU_BC"){ address="000000EA";}
        else if(endpoint=="CRU_ORBIT"){ address="000000E9";}
        else if(endpoint=="FIFO_COUNT"){ address="000000EB";}
        else if(endpoint=="SEL_FIRST_HIT_DROPPED_ORBIT"){ address="000000EC";}
        else if(endpoint=="SEL_LAST_HIT_DROPPED_ORBIT"){ address="000000ED";}
        else if(endpoint=="SEL_HITS_DROPPED"){ address="000000EE";}
        else if(endpoint=="READOUT_RATE"){ address="000000EF";}
        else if(endpoint=="IPbus_FIFO_DATA"){ address="000000F0";}
        else if(endpoint=="EVENTS_COUNT"){ address="000000F1";}
        else if(endpoint=="MCODE_TIME"){ address="000000F7";}
        else if(endpoint=="FW_TIME"){ address="000000FF";}
        else if(endpoint=="TEMPERATURE"){ address="00000005";}
        else if(endpoint=="MODES_STATUS"){ address="000000E8";}
        else if(endpoint=="FPGA_TEMP"){ address="000000FC";}
        else if(endpoint=="1VPOWER"){ address="000000FD";}
        else if(endpoint=="18VPOWER"){ address="000000FE";}
        vector<string> parameters = Utility::splitString(input, ",");
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else{
            sequence="";
            noRpcRequest=true;
            this->publishError("Readonly parameter");
        }
        return sequence;
    }
    else if(endpoint=="TRIGGERS_OUTPUTS_MODE"){
        address="0000006A";
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            sequenceOperation1(parameterValue(parameters[0]), address, sequence);
        }
        else if(parameters.size()>2&&(parameters[0]=="0"||parameters[0]=="1")){
            sequenceOperationBits(parameterValue(parameters[2]), 0, 0xFFFFFFFC, address, sequence);
        }
        else if(parameters.size()>2&&(parameters[0]=="3"||parameters[0]=="4")){
            sequenceOperationBits(parameterValue(parameters[2]), 3, 0xFFFFFFE7, address, sequence);
        }
        else if(parameters.size()>2&&(parameters[0]=="6"||parameters[0]=="7")){
            sequenceOperationBits(parameterValue(parameters[2]), 6, 0xFFFFFF3F, address, sequence);
        }
        else if(parameters.size()>2&&(parameters[0]=="9"||parameters[0]=="10")){
            sequenceOperationBits(parameterValue(parameters[2]), 9, 0xFFFFF9FF, address, sequence);
        }
        else if(parameters.size()>2&&(parameters[0]=="12"||parameters[0]=="13")){
            sequenceOperationBits(parameterValue(parameters[2]), 12, 0xFFFFCFFF, address, sequence);
        }
        else if(parameters.size()>1&&parameters[1]=="3"){
            sequenceOperation3((int)parameterValue(parameters[0]), address, sequence);
        }
        else if(parameters.size()>1&&parameters[1]=="2"){
            sequenceOperation2((int)parameterValue(parameters[0]), address, sequence);
        }
        else{
            this->publishError("Wrong parameters");
            sequence="";
            noRpcRequest=true;
        }
        return sequence;
    }
    else if(endpoint=="COUNTERS_UPD_RATE"){
        address="00000050";
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            long long num = parameterValue(parameters[0]);
            if(!(num>=0&&num<=7)){
                this->publishError("Value out of correct ranges");
                return "";
            }
            sequenceOperation1(num, address, sequence);
        }
        else{
            noRpcRequest=true;
            this->publishError("Parameter can be only read or write and range is 0-7");
            sequence="";
        }
        return sequence;
    }
    else if(endpoint=="DG_BUNCH_PATTERN"||endpoint=="TG_SINGLE_VALUE"||endpoint=="TG_PATTERN_1"||endpoint=="TG_PATTERN_1"||endpoint=="TG_CONT_VALUE"){
        if(endpoint=="TG_SINGLE_VALUE"){ address="000000DB";}
        else if(endpoint=="DG_BUNCH_PATTERN"){ address="000000DA";}
        else if(endpoint=="TG_PATTERN_1"){ address="000000DC";}
        else if(endpoint=="TG_PATTERN_0"){ address="000000DD";}
        else if(endpoint=="TG_CONT_VALUE"){ address="000000DE";}

        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            sequenceOperation1(parameterValue(parameters[0]), address, sequence);
        }
        else{
            this->publishError("wrong parameters");
            sequence="";
        }
        return sequence;
    }
    else if(endpoint=="EMULATION_RATE"){
        address="000000DF";
        int indexTemp = std::stoi(parameters[0]);
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            sequenceOperation1(parameterValue(parameters[0]), address, sequence);
        }
        else if(parameters.size()>2&&(indexTemp>=0&&indexTemp<=15)){
            sequenceOperationBits(parameterValue(parameters[2]), 0, 0xFFFF0000, address, sequence);
        }
        else if(parameters.size()>2&&(indexTemp>=16&&indexTemp<=31)){
            sequenceOperationBits(parameterValue(parameters[2]), 16, 0x0000FFFF, address, sequence);
        }
        else{
            this->publishError("wrong parameters");
            sequence="";
            noRpcRequest=true;
        }
        return sequence;
    }
    else if(endpoint=="SPI_MASK"){
        address="0000001E";

        if (input == ""||input == "set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            sequenceOperation1(parameterValue(parameters[0]), address, sequence);
        }
        else{
            this->publishError("wrong parameters");
            sequence="";
            noRpcRequest=true;
        }
        return sequence;
    }
    else if(endpoint=="TRIGGERS_SUPPRESSION_CONTROL"){
        address="0000001F";
        int index = std::stoi(parameters[0]);
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            sequenceOperation1(parameterValue(parameters[0]), address, sequence);
        }
        else if(parameters.size()>2&&(index>=0&&index<=5)){
            sequenceOperationBits(parameterValue(parameters[2]), 0, 0xFFFFFFC0, address, sequence);
        }
        else if(parameters.size()>2&&(index>=6&&index<=7)){
            sequenceOperationBits(parameterValue(parameters[2]), 6, 0xFFFFFF3F, address, sequence);
        }
        else{
            sequence="";
            noRpcRequest=true;
            this->publishError("wrong parameters");
        }
        return sequence;
    }
    else if(endpoint=="GENERATOR_FREQ_OFFSET"){
        address="000000E0";
        int index = std::stoi(parameters[0]);
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            sequenceOperation1(parameterValue(parameters[0]), address, sequence);
        }
        else if(parameters.size()>2&&(index>=0&&index<=11)){
            sequenceOperationBits(parameterValue(parameters[2]), 0, 0xFFFFF000, address, sequence);
        }
        else if(parameters.size()>2&&(index>=12&&index<=15)){
            sequenceOperationBits(parameterValue(parameters[2]), 12, 0xFFFF0FFF, address, sequence);
        }
        else if(parameters.size()>2&&(index>=16&&index<=27)){
            sequenceOperationBits(parameterValue(parameters[2]), 16, 0xF000FFFF, address, sequence);
        }
        else if(parameters.size()>2&&(index>=28&&index<=31)){
            sequenceOperationBits(parameterValue(parameters[2]), 28, 0x0FFFFFFF, address, sequence);
        }
        else{
            this->publishError("Value not to set");
            sequence="";
        }
        return sequence;
    }
    else if(endpoint=="LASER_PATTERN_1"||endpoint=="LASER_PATTERN_0"){
        if(endpoint=="LASER_PATTERN_0"){
            address="0000001D";
        }
        else if(endpoint=="LASER_PATTERN_1"){
            address="0000001C";
        }
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            sequenceOperation1(parameterValue(parameters[0]), address, sequence);
        }
        else if(parameters.size()>1&&parameters[1]=="3"){
            sequenceOperation3(parameterValue(parameters[0]), address, sequence);
        }
        else if(parameters.size()>1&&parameters[1]=="2"){
            sequenceOperation2(parameterValue(parameters[0]), address, sequence);
        }
        else{
            this->publishError("wrong parameters");
            sequence="";
            noRpcRequest=true;
        }
        return sequence;
    }
    return sequence;
}



string Default2::processOutputMessage(string output) {
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
    else if(endpoint=="TRG_C_RATE"){
        return std::to_string(tcm.temp.trigger1rate);
    }
    else if(endpoint=="TRG_SC_RATE"){
        return std::to_string(tcm.temp.trigger2rate);
    }
    else if(endpoint=="TRG_V_RATE"){
        return std::to_string(tcm.temp.trigger3rate);
    }
    else if(endpoint=="TRG_ORC_RATE"){
        return std::to_string(tcm.temp.trigger4rate);
    }
    else if(endpoint=="TRG_ORA_RATE"){
        return std::to_string(tcm.temp.trigger5rate);
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
    else if(endpoint=="LASER_DIVIDER"){
        float systemClock_MHz = tcm.act.externalClock?40.0789658:40.;
        long long tempValue = std::stoll(value.substr(2,6), nullptr, 16);
        float laserFrequency = systemClock_MHz*std::pow(10,6)/(tempValue==0?1<<24:tempValue);
        updateTopicAnswer("READOUTCARDS/TCM0/LASER_FREQUENCY", std::to_string(laserFrequency));
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
