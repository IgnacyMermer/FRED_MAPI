#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include "PM_default.h"
#include "Alfred/print.h"
#include "Parser/utility.h"
#include "TCM_values.h"
#include <sstream>
#include <cmath>
#include "SWT_creator.h"


PM_default::PM_default(std::string endpointParam, std::string pmNumberParam, std::string pmNameParam) {
    finalValue = 0;
    pmNumber = pmNumberParam;
    pmName = pmNameParam;
    endpoint=endpointParam;
}

string PM_default::processInputMessage(string input) {

    /*input must consists one of following sentence:
        - (empty request) - read
        - "set" - read
        - "(sth dont matter),0" - read
        - "(value),1" - write value on address of parameter
        - "(index),2" - set bit to zero
        - "(index),3" - set bit to one
        - "(index),(2/3),(value)" - set value on bits, user must provide index in the range of word bits in parameter address, then value is put on these bits. There is no matter if user provide 2 or 3 as the second parameter
    
    After going inside of if else function there is creation of correct sequence string and then it is return to ALF, if user provides not valid data, 
    the error is returning and there is no request to ALF.
    */

    std::string address="", sequence="";
    vector<string> parameters = Utility::splitString(input, ",");
    address = "0000"+tcm.addresses["PM/"+pmName+"/"+endpoint];
    if(input.length()>5&&input.substr(0,5)=="FRED,"){
        this->publishAnswer(input.substr(5));
        noRpcRequest=true;
        return "0";
    }
    if(endpoint.rfind("TDC_12_PHASE_TUNING",0)==0||endpoint.rfind("TDC_3_PHASE_TUNING",0)==0||endpoint.rfind("RAW_TDC_DATA",0)==0
    ||endpoint.rfind("ADC0_DISPERSION",0)==0||endpoint.rfind("ADC1_DISPERSION",0)==0||endpoint.rfind("ADC0_MEAN",0)==0||endpoint.rfind("ADC1_MEAN",0)==0
    ||endpoint.rfind("COUNT_CFD_HITS",0)==0||endpoint.rfind("COUNT_TRG_HITS",0)==0||endpoint.rfind("ADC0_BASELINE",0)==0||endpoint.rfind("ADC1_BASELINE",0)==0||endpoint=="TEMPERATURE"
    ||endpoint=="BOARD_TYPE"||endpoint=="LAST_RESTART_REASON"||endpoint=="FPGA_TEMP"||endpoint=="1VPOWER"||endpoint=="18VPOWER"||endpoint=="FW_TIME"||endpoint=="MCODE_TIME"){

        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else{
            noRpcRequest=true;
            sequence="";
            this->publishError("Readonly parameter");
        }
        return sequence;
    }
    else if(endpoint=="CHANNEL_ADC_BASELINE"||endpoint.rfind("CFD_THRESHOLD",0)==0||endpoint.rfind("THRESHOLD_CALIBRATION",0)==0||endpoint.rfind("ADC_DELAY",0)==0
    ||endpoint.rfind("CFD_ZERO",0)==0||endpoint.rfind("ADC_ZERO",0)==0||endpoint=="CURRENT_ADDRESS"||endpoint=="HISTOGRAM_DATA_READOUT"||endpoint=="READOUT_RATE"
    ||endpoint=="SEL_HITS_DROPPED"||endpoint=="SEL_LAST_HIT_DROPPED_ORBIT"||endpoint=="SEL_FIRST_HIT_DROPPED_ORBIT"||endpoint=="CRU_BC"||endpoint=="CRU_ORBIT"
    ||endpoint=="DATA_SELECT_TRG_MASK"||endpoint=="DATA_BUNCH_PATTERN"||endpoint=="TRIGGER_SINGLE_VALUE"||endpoint=="TRIGGER_CONT_PATTERN_MSB"||endpoint=="TRIGGER_CONT_PATTERN_LSB"||endpoint=="TRIGGER_CONT_VALUE"
    ||endpoint=="GENERATORS_BUNCH_FREQ"||endpoint=="GENERATORS_FREQ_OFFSET"||endpoint.rfind("ADC0_RANGE_CORR",0)==0||endpoint.rfind("ADC1_RANGE_CORR",0)==0||endpoint=="TRIGGER_RESPOND_MASK"
    ||endpoint=="LAST_RESTART_REASON"||endpoint=="FW_UPGRADE_COMM"||endpoint=="FW_UPGRADE_DATA"||endpoint=="FW_UPGRADE_END"||endpoint=="FW_UPGRADE_STATUS"){
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            SWT_creator::sequenceOperationWrite(SWT_creator::parameterValue(parameters[0]), address, sequence);
        }
        else{
            sequence="";
            noRpcRequest=true;
            this->publishError("Wrong parameters");
        }
        return sequence;
    }
    else if(endpoint=="TRG_SETTINGS"){
        SWT_creator::readWriteRWMSequences(input, parameters, address, sequence);
        if(sequence==""){
            this->publishError("Wrong parameters");
            noRpcRequest=true;
        }
        else if(sequence=="check"){
            int index = SWT_creator::parameterValue(parameters[0]);
            if((index>=0&&index<=7)){
                SWT_creator::sequenceOperationBits(SWT_creator::parameterValue(parameters[2]), 0, 0xFFFFFF00, address, sequence);
            }
            else{
                this->publishError("Wrong parameters");
                noRpcRequest=true;
            }
        }
        return sequence;
    }
    else if(endpoint.rfind("CHANNEL_SETTINGS",0)==0){
        SWT_creator::readWriteRWMSequences(input, parameters, address, sequence);
        if(sequence==""){
            this->publishError("Wrong parameters");
            noRpcRequest=true;
        }
        else if(sequence=="check"){
            int index = SWT_creator::parameterValue(parameters[0]);
            if(parameters.size()>2&&(index>=0&&index<=11)){
                SWT_creator::sequenceOperationBits(SWT_creator::parameterValue(parameters[2]), 0, 0xFFFFF000, address, sequence);
            }
            else{
                this->publishError("Wrong parameters");
                noRpcRequest=true;
            }
        }
        return sequence;
    }
    else if(endpoint.rfind("TRG_CHARGE_LEVELS",0)==0){
        int index = SWT_creator::parameterValue(parameters[0]);
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            SWT_creator::sequenceOperationWrite(index, address, sequence);
        }
        else if(parameters.size()>2&&(index>=0&&index<=11)){
            SWT_creator::sequenceOperationBits(SWT_creator::parameterValue(parameters[2]), 0, 0xFFFFF000, address, sequence);
        }
        else if(parameters.size()>2&&(index>=12&&index<=15)){
            SWT_creator::sequenceOperationBits(SWT_creator::parameterValue(parameters[2]), 12, 0xFFFF0FFF, address, sequence);
        }
        else{
            noRpcRequest=true;
            sequence="";
            this->publishError("Wrong parameters");
        }
        return sequence;
    }
    else if(endpoint=="CHANNELS_MASK"||endpoint=="STATUS_BITS"){
        SWT_creator::readWriteRWMSequences(input, parameters, address, sequence);
        if(sequence==""||sequence=="check"){
            this->publishError("Wrong parameters");
            noRpcRequest=true;
        }
        return sequence;
    }
    else if(endpoint=="HISTOGRAMMING_CONTROL"){
        SWT_creator::readWriteRWMSequences(input, parameters, address, sequence);
        if(sequence==""){
            this->publishError("Wrong parameters");
            noRpcRequest=true;
        }
        else if(sequence=="check"){
            int index = SWT_creator::parameterValue(parameters[0]);
            if(parameters.size()>2&&(index>=0&&index<=11)){
                SWT_creator::sequenceOperationBits(SWT_creator::parameterValue(parameters[2]), 0, 0xFFFFF000, address, sequence);
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
                SWT_creator::sequenceOperationBits(index, 0, 0xFFFFFFF0, address, sequence);
            }
            else if(parameters.size()>2&&(index>=4&&index<=7)){
                SWT_creator::sequenceOperationBits(index, 4, 0xFFFFFF0F, address, sequence);
            }
            else if(parameters.size()>2&&(index>=16&&index<=19)){
                SWT_creator::sequenceOperationBits(index, 16, 0xFFF0FFFF, address, sequence);
            }
            else{
                this->publishError("Wrong parameters");
                noRpcRequest=true;
            }
        }
        return sequence;
    }
    else if(endpoint=="RDH_FIELDS"){

        int index = SWT_creator::parameterValue(parameters[0]);
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            SWT_creator::sequenceOperationWrite(index, address, sequence);
        }
        else if(parameters.size()>2&&(index>=0&&index<=15)){
            SWT_creator::sequenceOperationBits(index, 0, 0xFFFF0000, address, sequence);
        }
        else if(parameters.size()>2&&(index>=16&&index<=31)){
            SWT_creator::sequenceOperationBits(index, 16, 0x0000FFFF, address, sequence);
        }
        else{
            noRpcRequest=true;
            sequence="";
            this->publishError("Wrong parameters");
        }
        return sequence;
    }
    else if(endpoint=="CRU_SIZE"){

        int index = SWT_creator::parameterValue(parameters[0]);
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            SWT_creator::sequenceOperationWrite(index, address, sequence);
        }
        else if(parameters.size()>2&&(index>=0&&index<=15)){
            SWT_creator::sequenceOperationBits(SWT_creator::parameterValue(parameters[2]), 0, 0xFFFF0000, address, sequence);
        }
        else if(parameters.size()>2&&(index>=16&&index<=31)){
            SWT_creator::sequenceOperationBits(SWT_creator::parameterValue(parameters[2]), 16, 0x0000FFFF, address, sequence);
        }
        else{
            noRpcRequest=true;
            sequence="";
            this->publishError("Wrong parameters");
        }
        return sequence;
    }
    else if(endpoint=="DELAYS"){

        int index = SWT_creator::parameterValue(parameters[0]);
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            SWT_creator::sequenceOperationWrite(index, address, sequence);
        }
        else if(parameters.size()>2&&(index>=0&&index<=11)){
            SWT_creator::sequenceOperationBits(SWT_creator::parameterValue(parameters[2]), 0, 0xFFFFF000, address, sequence);
        }
        else if(parameters.size()>2&&(index>=16&&index<=27)){
            SWT_creator::sequenceOperationBits(SWT_creator::parameterValue(parameters[2]), 16, 0xF000FFFF, address, sequence);
        }
        else{
            noRpcRequest=true;
            sequence="";
            this->publishError("Wrong parameters");
        }
        return sequence;
    }
    else if(endpoint=="MODES_STATUS"){
        SWT_creator::readWriteRWMSequences(input, parameters, address, sequence);
        if(sequence==""){
            this->publishError("Wrong parameters");
            noRpcRequest=true;
        }
        else if(sequence=="check"){
            int index = SWT_creator::parameterValue(parameters[0]);
            if(parameters.size()>2&&(index>=16&&index<=19)){
                SWT_creator::sequenceOperationBits(index, 16, 0xFFF0FFFF, address, sequence);
            }
            else if(parameters.size()>2&&(index>=20&&index<=23)){
                SWT_creator::sequenceOperationBits(index, 20, 0xFF0FFFFF, address, sequence);
            }
            else if(parameters.size()>2&&(index>=24&&index<=27)){
                SWT_creator::sequenceOperationBits(index, 24, 0xF0FFFFFF, address, sequence);
            }
            else if(parameters.size()>2&&(index>=28&&index<=31)){
                SWT_creator::sequenceOperationBits(index, 28, 0x0FFFFFFF, address, sequence);
            }
            else{
                this->publishError("Wrong parameters");
                noRpcRequest=true;
            }
        }
        return sequence;
    }
    else if(endpoint=="FIFO_COUNT"){
        int index = SWT_creator::parameterValue(parameters[0]);
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            SWT_creator::sequenceOperationWrite(index, address, sequence);
        }
        else if(parameters.size()>2&&(index>=0&&index<=15)){
            SWT_creator::sequenceOperationBits(SWT_creator::parameterValue(parameters[2]), 0, 0xFFFF0000, address, sequence);
        }
        else if(parameters.size()>2&&(index>=16&&index<=31)){
            SWT_creator::sequenceOperationBits(SWT_creator::parameterValue(parameters[2]), 16, 0x0000FFFF, address, sequence);
        }
        else{
            noRpcRequest=true;
            sequence="";
            this->publishError("Wrong parameters");
        }
        return sequence;
    }
}

string PM_default::processOutputMessage(string output) {

    /*
        When ALF return output, program takes only value saved on last 8bits of read word. On some parameters in the following lines there are made some 
        calculations, in other case value is returned with no additional operations.
    */

    string value;
    output.erase(remove(output.begin(), output.end(), '\n'), output.end());
    value = output.substr(output.size() - 8, output.size());
    finalValue = stoll(value, nullptr, 16);
    if(endpoint.rfind("TDC_12_PHASE_TUNING",0)==0){

        //program split two words from one parameter for two values to transfer into signed values.
        //return "number_1,number_2"

        std::string returnStr = "";
        unsigned int x;
        std::stringstream ss;
        ss << std::hex << value.substr(6,2);
        ss >> x;

        if (x > 127) {
            returnStr += std::to_string((static_cast<int>(static_cast<signed char>(x)))*13*8/7);
        }
        else {
            returnStr += std::to_string((static_cast<int>(x))*13*8/7);
        }
        returnStr+=",";
        ss.str(std::string());
        ss.clear();
        ss << std::hex << value.substr(4,2);
        ss >> x;

        if (x > 127) {
            returnStr += std::to_string((static_cast<int>(static_cast<signed char>(x)))*13*8/7);
        }
        else {
            returnStr += std::to_string((static_cast<int>(x))*13*8/7);
        }
        return returnStr;
    }
    else if(endpoint.rfind("TDC_3_PHASE_TUNING",0)==0){
        std::string returnStr = "";
        unsigned int x;
        std::stringstream ss;
        ss << std::hex << value.substr(6,2);
        ss >> x;

        if (x > 127) {
            returnStr += std::to_string((static_cast<int>(static_cast<signed char>(x)))*13*8/7);
        }
        else {
            returnStr += std::to_string((static_cast<int>(x))*13*8/7);
        }
        return returnStr;
    }
    else if(endpoint.rfind("RAW_TDC_DATA",0)==0){
        std::stringstream ss;
        ss << std::hex << finalValue;
        return "0x"+ss.str();
    }
    else if(endpoint.rfind("ADC0_DISPERSION",0)==0||endpoint.rfind("ADC1_DISPERSION",0)==0){
        return std::to_string(std::sqrt(finalValue));
    }
    else if(endpoint == "TEMPERATURE"){
        return std::to_string(finalValue/10.0);
    }
    else if(endpoint == "FPGA_TEMP"){
        return std::to_string(finalValue * 503.975 / 65536 - 273.15);
    }
    else if(endpoint == "1VPOWER"){
        return std::to_string(finalValue * 3 / 65536.0);
    }
    else if(endpoint == "18VPOWER"){
        return std::to_string(finalValue * 3 / 65536.0);
    }

    if (finalValue > 50000&&(endpoint.rfind("CHANNEL_SETTINGS",0)==0||endpoint.rfind("ADC0_BASELINE",0)==0
    ||endpoint.rfind("ADC1_BASELINE",0)==0||endpoint.rfind("ADC0_MEAN",0)==0||endpoint.rfind("ADC1_MEAN",0)==0
    ||endpoint.rfind("ADC1_MEAN",0)==0||endpoint.rfind("CFD_ZERO",0)==0||endpoint.rfind("ADC_ZERO",0)==0
    ||endpoint.rfind("TEMPERATURE",0)==0)) {
      int16_t x = stoi(value, nullptr, 16);
      finalValue=-(~x+1);
    }

    return to_string(finalValue);

}

