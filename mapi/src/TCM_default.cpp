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
    address = "0000"+tcm.addresses["READOUTCARDS/TCM0/"+endpoint];
    if(input.length()>5&&input.substr(0,5)=="FRED,"){
        this->publishAnswer(input.substr(5));
        noRpcRequest=true;
        return "0";
    }
    if(endpoint=="VTIME_LOW"||endpoint=="VTIME_HIGH"){
        if (input == ""||input == "set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            long long num = SWT_creator::parameterValue(parameters[0]);
            if(num<-512||num>511){
                noRpcRequest=true;
                this->publishError("Value out of correct range");
                return "";
            }
            SWT_creator::sequenceOperationWrite(num, address, sequence);
        }
        else{
            noRpcRequest=true;
            this->publishError("Parameter can be only read or write");
            sequence="";
        }
        return sequence;
    }
    else if(endpoint=="AVERAGE_TIME"){
        if (input == ""||input == "set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
    }
    else if(endpoint=="SC_LEVEL_A"||endpoint=="SC_LEVEL_C"||endpoint=="C_LEVEL_A"||endpoint=="C_LEVEL_C"){
        if (input == ""||input == "set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            long long num = SWT_creator::parameterValue(parameters[0]);
            if(num<0||num>65535){
                noRpcRequest=true;
                this->publishError("Value out of correct range");
                return "";
            }
            SWT_creator::sequenceOperationWrite(num, address, sequence);
        }
        else{
            this->publishError("Parameter can be only read or write");
            sequence="";
            noRpcRequest=true;
        }
        return sequence;
    }
    else if(endpoint=="LASER_DELAY"){
        if (input == ""||input == "set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            float num = SWT_creator::SWT_creator::parameterValue(parameters[0]);
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
            SWT_creator::sequenceOperationWrite((long long) num, address, sequence);
        }
        else{
            this->publishError("Parameter can be only read or write");
            sequence="";
            noRpcRequest=true;
        }
        return sequence;
    }
    else if(endpoint=="ATTENUATOR"||endpoint=="TRG_ORA_RATE"||endpoint=="TRG_ORC_RATE"||endpoint=="TRG_SC_RATE"||endpoint=="TRG_C_RATE"||endpoint=="TRG_V_RATE"
    ||endpoint=="TRG_ORA_SIGN"||endpoint=="TRG_ORC_SIGN"||endpoint=="TRG_SC_SIGN"||endpoint=="TRG_C_SIGN"||endpoint=="TRG_V_SIGN"){
        int index = std::stoi(parameters[0]);
        Print::PrintInfo(input);
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            SWT_creator::sequenceOperationWrite(SWT_creator::SWT_creator::parameterValue(parameters[0]), address, sequence);
        }
        else{
            this->publishError("wrong parameters");
            sequence = "";
            noRpcRequest=true;
        }
        return sequence;
    }
    else if(endpoint=="EXT_SW"||endpoint=="SIDE_A_STATUS"||endpoint=="SIDE_C_STATUS"){
        if (input == ""||input == "set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            SWT_creator::sequenceOperationWrite(SWT_creator::SWT_creator::parameterValue(parameters[0]), address, sequence);
        }
        else if(parameters.size()>1&&parameters[1]=="3"){
            SWT_creator::sequenceOperationRMWOR((int)SWT_creator::SWT_creator::parameterValue(parameters[0]), address, sequence);
        }
        else if(parameters.size()>1&&parameters[1]=="2"){
            SWT_creator::sequenceOperationRMWAND((int)SWT_creator::SWT_creator::parameterValue(parameters[0]), address, sequence);
        }
        else{
            this->publishError("Parameter can be only read or write or RMW");
            sequence="";
            noRpcRequest=true;
        }
        return sequence;
    }
    else if(endpoint=="MODE"){
        SWT_creator::readWriteRWMSequences(input, parameters, address, sequence);
        if(sequence==""){
            this->publishError("Wrong parameters");
            noRpcRequest=true;
        }
        else if(sequence=="check"){
            int index = SWT_creator::parameterValue(parameters[0]);
            if(parameters.size()>2&&(index==1||index==2)){
                SWT_creator::sequenceOperationBits(SWT_creator::SWT_creator::parameterValue(parameters[2]), 1, 0xFFFFFFF9, address, sequence);
            }
            else if(parameters.size()>2&&(index>=4&&index<=7)){
                SWT_creator::sequenceOperationBits(SWT_creator::SWT_creator::parameterValue(parameters[2]), 4, 0xFFFFFF0F, address, sequence);
            }
            else{
                this->publishError("Wrong parameters");
                noRpcRequest=true;
            }
        }
        return sequence;
    }
    else if(endpoint=="MODE_SETTINGS"){
        SWT_creator::readWriteRWMSequences(input, parameters, address, sequence);
        if(sequence==""){
            this->publishError("Wrong parameters");
            noRpcRequest=true;
        }
        else if(sequence=="check"){
            int index = SWT_creator::parameterValue(parameters[0]);
            if(parameters.size()>2&&(index>=0&&index<=3)){
                SWT_creator::sequenceOperationBits(SWT_creator::SWT_creator::parameterValue(parameters[2]), 0, 0xFFFFFFF0, address, sequence);
            }
            else if(parameters.size()>2&&(index>=4&&index<=7)){
                SWT_creator::sequenceOperationBits(SWT_creator::SWT_creator::parameterValue(parameters[2]), 4, 0xFFFFFF0F, address, sequence);
            }
            else if(parameters.size()>2&&(index>=16&&index<=19)){
                SWT_creator::sequenceOperationBits(SWT_creator::SWT_creator::parameterValue(parameters[2]), 16, 0xFFF0FFFF, address, sequence);
            }
            else{
                this->publishError("Wrong parameters");
                noRpcRequest=true;
            }
        }
        return sequence;
    }
    else if(endpoint=="LASER_DIVIDER"){

        //do sprawdzenia wartosc wyswietlana

        SWT_creator::readWriteRWMSequences(input, parameters, address, sequence);
        if(sequence==""){
            this->publishError("Wrong parameters");
            noRpcRequest=true;
        }
        else if(sequence=="check"){
            int index = SWT_creator::parameterValue(parameters[0]);
            if(parameters.size()>2&&(index>=0&&index<=23)){
                SWT_creator::sequenceOperationBits(SWT_creator::SWT_creator::parameterValue(parameters[2]), 0, 0xFF000000, address, sequence);
            }
            else{
                this->publishError("Wrong parameters");
                noRpcRequest=true;
            }
        }
        return sequence;
    }
    else if(endpoint=="DATA_SEL_TRG_MASK"||endpoint=="BCID_OFFSET"||endpoint=="DG_TRG_RESPOND_MASK"||endpoint=="RDH_FIELDS"){
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            SWT_creator::sequenceOperationWrite(SWT_creator::SWT_creator::parameterValue(parameters[0]), address, sequence);
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
    ||endpoint=="FPGA_STATUS"||endpoint=="1VPOWER"||endpoint=="18VPOWER"||endpoint=="FPGA_TEMP"||endpoint.rfind("BKGRND",0)==0||endpoint=="TRIGGER5_CNT"||endpoint=="TRIGGER4_CNT"
    ||endpoint=="TRIGGER3_CNT"||endpoint=="TRIGGER2_CNT"||endpoint=="TRIGGER1_CNT"){
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
        SWT_creator::readWriteRWMSequences(input, parameters, address, sequence);
        if(sequence==""){
            this->publishError("Wrong parameters");
            noRpcRequest=true;
        }
        else if(sequence=="check"){
            int index = SWT_creator::parameterValue(parameters[0]);
            if(parameters.size()>2&&(index==0||index==1)){
                SWT_creator::sequenceOperationBits(SWT_creator::SWT_creator::parameterValue(parameters[2]), 0, 0xFFFFFFFC, address, sequence);
            }
            else if(parameters.size()>2&&(index==3||index==4)){
                SWT_creator::sequenceOperationBits(SWT_creator::SWT_creator::parameterValue(parameters[2]), 3, 0xFFFFFFE7, address, sequence);
            }
            else if(parameters.size()>2&&(index==6||index==7)){
                SWT_creator::sequenceOperationBits(SWT_creator::SWT_creator::parameterValue(parameters[2]), 6, 0xFFFFFF3F, address, sequence);
            }
            else if(parameters.size()>2&&(index==9||index==10)){
                SWT_creator::sequenceOperationBits(SWT_creator::SWT_creator::parameterValue(parameters[2]), 9, 0xFFFFF9FF, address, sequence);
            }
            else if(parameters.size()>2&&(index==12||index==13)){
                SWT_creator::sequenceOperationBits(SWT_creator::SWT_creator::parameterValue(parameters[2]), 12, 0xFFFFCFFF, address, sequence);
            }
            else{
                this->publishError("Wrong parameters");
                noRpcRequest=true;
            }
        }
        Print::PrintInfo(sequence);
        return sequence;
    }
    else if(endpoint=="COUNTERS_UPD_RATE"){
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            long long num = SWT_creator::SWT_creator::parameterValue(parameters[0]);
            if(!(num>=0&&num<=7)){
                this->publishError("Value out of correct ranges");
                return "";
            }
            SWT_creator::sequenceOperationWrite(num, address, sequence);
        }
        else{
            noRpcRequest=true;
            this->publishError("Parameter can be only read or write and range is 0-7");
            sequence="";
        }
        return sequence;
    }
    else if(endpoint=="DG_BUNCH_PATTERN"||endpoint=="TG_SINGLE_VALUE"||endpoint=="TG_PATTERN_1"||endpoint=="TG_PATTERN_0"||endpoint=="TG_CONT_VALUE"){
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            SWT_creator::sequenceOperationWrite(SWT_creator::SWT_creator::parameterValue(parameters[0]), address, sequence);
        }
        else{
            this->publishError("wrong parameters");
            sequence="";
        }
        return sequence;
    }
    else if(endpoint=="EMULATION_RATE"){
        int indexTemp = std::stoi(parameters[0]);
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            SWT_creator::sequenceOperationWrite(SWT_creator::SWT_creator::parameterValue(parameters[0]), address, sequence);
        }
        else if(parameters.size()>2&&(indexTemp>=0&&indexTemp<=15)){
            SWT_creator::sequenceOperationBits(SWT_creator::SWT_creator::parameterValue(parameters[2]), 0, 0xFFFF0000, address, sequence);
        }
        else if(parameters.size()>2&&(indexTemp>=16&&indexTemp<=31)){
            SWT_creator::sequenceOperationBits(SWT_creator::SWT_creator::parameterValue(parameters[2]), 16, 0x0000FFFF, address, sequence);
        }
        else{
            this->publishError("wrong parameters");
            sequence="";
            noRpcRequest=true;
        }
        return sequence;
    }
    else if(endpoint=="SPI_MASK"){
        if (input == ""||input == "set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            SWT_creator::sequenceOperationWrite(SWT_creator::SWT_creator::parameterValue(parameters[0]), address, sequence);
        }
        else{
            this->publishError("wrong parameters");
            sequence="";
            noRpcRequest=true;
        }
        return sequence;
    }
    else if(endpoint=="TRIGGERS_SUPPRESSION_CONTROL"){
        int index = std::stoi(parameters[0]);
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            SWT_creator::sequenceOperationWrite(SWT_creator::SWT_creator::parameterValue(parameters[0]), address, sequence);
        }
        else if(parameters.size()>2&&(index>=0&&index<=5)){
            SWT_creator::sequenceOperationBits(SWT_creator::SWT_creator::parameterValue(parameters[2]), 0, 0xFFFFFFC0, address, sequence);
        }
        else if(parameters.size()>2&&(index>=6&&index<=7)){
            SWT_creator::sequenceOperationBits(SWT_creator::SWT_creator::parameterValue(parameters[2]), 6, 0xFFFFFF3F, address, sequence);
        }
        else{
            sequence="";
            noRpcRequest=true;
            this->publishError("wrong parameters");
        }
        return sequence;
    }
    else if(endpoint=="GENERATOR_FREQ_OFFSET"){
        int index = std::stoi(parameters[0]);
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            SWT_creator::sequenceOperationWrite(SWT_creator::SWT_creator::parameterValue(parameters[0]), address, sequence);
        }
        else if(parameters.size()>2&&(index>=0&&index<=11)){
            SWT_creator::sequenceOperationBits(SWT_creator::SWT_creator::parameterValue(parameters[2]), 0, 0xFFFFF000, address, sequence);
        }
        else if(parameters.size()>2&&(index>=12&&index<=15)){
            SWT_creator::sequenceOperationBits(SWT_creator::SWT_creator::parameterValue(parameters[2]), 12, 0xFFFF0FFF, address, sequence);
        }
        else if(parameters.size()>2&&(index>=16&&index<=27)){
            SWT_creator::sequenceOperationBits(SWT_creator::SWT_creator::parameterValue(parameters[2]), 16, 0xF000FFFF, address, sequence);
        }
        else if(parameters.size()>2&&(index>=28&&index<=31)){
            SWT_creator::sequenceOperationBits(SWT_creator::SWT_creator::parameterValue(parameters[2]), 28, 0x0FFFFFFF, address, sequence);
        }
        else{
            this->publishError("Value not to set");
            sequence="";
        }
        return sequence;
    }
    else if(endpoint=="LASER_PATTERN_1"||endpoint=="LASER_PATTERN_0"){
        SWT_creator::readWriteRWMSequences(input, parameters, address, sequence);
        if(sequence==""||sequence=="check"){
            this->publishError("Wrong parameters");
            noRpcRequest=true;
        }
        return sequence;
    }
    else if(endpoint=="LASER_FREQUENCY"){
        //float systemClock_MHz = tcm.act.externalClock?40.0789658:40.;
        //long long tempValue = std::stoll(value.substr(2,6), nullptr, 16);
        //float laserFrequency = systemClock_MHz*std::pow(10,6)/(tempValue==0?1<<24:tempValue);
        //float laserFrequency = std::stof(parameters[0]);
        //laserFrequency = 
    }
    else if(endpoint=="DELAY_A"||endpoint=="DELAY_C"){
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            float num = std::stof(parameters[0]);
            float systemClock_MHz = tcm.act.externalClock?40.0789658:40.;
            float halfBC_ns = 500. / systemClock_MHz;
            float phaseStep_ns = halfBC_ns / (
            //SERIAL_NUM ? 
            1024 
            //: 1280
            );
            num = num / phaseStep_ns;
            SWT_creator::sequenceOperationWrite((int)num, address, sequence);
        }
        else{
            this->publishError("wrong parameters");
            sequence="";
            noRpcRequest=true;
        }
        return sequence;
    }
    else if(endpoint=="TRG_1_RATE"){
        noRpcRequest=true;
        this->publishAnswer(std::to_string(tcm.temp.trigger1rate));
        sequence="";
    }
    else if(endpoint=="TRG_2_RATE"){
        noRpcRequest=true;
        this->publishAnswer(std::to_string(tcm.temp.trigger2rate));
        sequence="";
    }
    else if(endpoint=="TRG_3_RATE"){
        noRpcRequest=true;
        this->publishAnswer(std::to_string(tcm.temp.trigger3rate));
        sequence="";
    }
    else if(endpoint=="TRG_4_RATE"){
        noRpcRequest=true;
        this->publishAnswer(std::to_string(tcm.temp.trigger4rate));
        sequence="";
    }
    else if(endpoint=="TRG_5_RATE"){
        noRpcRequest=true;
        this->publishAnswer(std::to_string(tcm.temp.trigger5rate));
        sequence="";
    }
    return sequence;
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
    /*else if(endpoint=="TRIGGERS_OUTPUTS_MODE"){
        std::bitset<32> binary(finalValue);
        std::string binary_str = binary.to_string();
        return binary_str;
    }*/
    noReturn=false;
  }
  catch (exception &e) {
    Print::PrintInfo(e.what());
    Print::PrintError("ERROR in connection with ALF!");
    finalValue=0;
  }
  return to_string(finalValue);
}
