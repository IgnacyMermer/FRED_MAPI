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


void PM_default::sequenceOperation1(long long num, std::string address, std::string& sequence){
    std::stringstream ss;
    ss << std::hex << num;
    std::string hex_str = ss.str();
    if(hex_str.length()>8){
        hex_str=hex_str.substr(hex_str.length()-4);
    }
    std::string data="";
    for(int i=0; i<8-hex_str.length(); i++){
        data+="0";
    }
    data+=hex_str;
    sequence="reset\n0x001"+address+data+",write\nread";
}

void PM_default::sequenceOperation2(int num, std::string address, std::string& sequence){
    int temp = 0xFFFFFFFF;
    temp-=(1 << num);
    std::stringstream ss;
    ss << std::hex << temp;
    std::string data = ss.str();
    sequence = "reset\n0x002"+address+data+",write\nread\n0x003"+address+"00000000,write\nread\n0x000"+address+"00000000,write\nread";
}

void PM_default::sequenceOperation3(int num, std::string address, std::string& sequence){
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

void PM_default::sequenceOperationBits(int num, int power, int maskNumber, std::string address, std::string& sequence){
    std::stringstream ss;
    ss << std::hex << maskNumber;
    std::string hex_str = ss.str();
    if(hex_str.length()>8){
        hex_str=hex_str.substr(hex_str.length()-4);
    }
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
    if(hex_str.length()>8){
        hex_str=hex_str.substr(hex_str.length()-4);
    }
    for(int i=0; i<8-hex_str.length(); i++){
        data2+="0";
    }
    data2+=hex_str;
    sequence = "reset\n0x002"+address+data+",write\nread\n0x003"+address+data2+",write\nread\n0x000"+address+"00000000,write\nread";
}

long long PM_default::parameterValue(std::string strValue){
    long long inputValue;
    if(strValue.rfind("0x", 0)==0){
        return std::stoll(strValue.substr(2), nullptr, 16);
    }
    else{
        return std::stoll(strValue);
    }
}

std::string PM_default::numberLetter(int number){
    std::string numberLetterStr = "";
    if((number/16)<10&&(number/16)!=0){
        numberLetterStr+=std::to_string(number/16);
    }
    else if((number/16)!=0){
        char tempChar = 'A'+((number/16)-10);
        std::string temp(1, tempChar);
        numberLetterStr+=temp;
    }
    number=number%16;
    if(number<10){
        numberLetterStr+=std::to_string(number);
    }
    else{
        char tempChar = 'A'+(number-10);
        std::string temp(1, tempChar);
        numberLetterStr+=temp;
    }
    if(numberLetterStr.length()==1){
        numberLetterStr="0"+numberLetterStr;
    }
    return numberLetterStr;
}



PM_default::PM_default(std::string endpointParam, std::string pmNumberParam) {
    finalValue = 0;
    pmNumber = pmNumberParam;
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
    if(endpoint=="TRG_SETTINGS"){
        address="0000"+pmNumber+"00";
        int index = parameterValue(parameters[0]);
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            sequenceOperation1(index, address, sequence);
        }
        else if(parameters.size()>2&&(index>=0&&index<=7)){
            sequenceOperationBits(parameterValue(parameters[2]), 0, 0xFFFFFF00, address, sequence);
        }
        else if(parameters.size()>1&&parameters[1]=="3"){
            sequenceOperation3(index, address, sequence);
        }
        else if(parameters.size()>1&&parameters[1]=="2"){
            sequenceOperation2(index, address, sequence);
        }
        else{
            this->publishError("Wrong parameters");
            noRpcRequest=true;
            sequence="";
        }
        return sequence;
    }
    else if(endpoint.rfind("CHANNEL_SETTINGS",0)==0){
        if(!((endpoint[endpoint.length()-1]>='1'&&endpoint[endpoint.length()-1]<='9')||(endpoint[endpoint.length()-1]>='A'&&endpoint[endpoint.length()-1]<='C'))){
            noRpcRequest=true;
            return "";
        }
        address="0000"+pmNumber+"0"+endpoint[endpoint.length()-1];
        int index = parameterValue(parameters[0]);
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            sequenceOperation1(index, address, sequence);
        }
        else if(parameters.size()>2&&(index>=0&&index<=11)){
            sequenceOperationBits(parameterValue(parameters[2]), 0, 0xFFFFF000, address, sequence);
        }
        else if(parameters.size()>1&&parameters[1]=="3"){
            sequenceOperation3(index, address, sequence);
        }
        else if(parameters.size()>1&&parameters[1]=="2"){
            sequenceOperation2(index, address, sequence);
        }
        else{
            this->publishError("Wrong parameters");
            noRpcRequest=true;
            sequence="";
        }
        return sequence;
    }
    else if(endpoint.rfind("ADC0_BASELINE",0)==0||endpoint.rfind("ADC1_BASELINE",0)==0||endpoint.rfind("ADC0_RANGE_CORR",0)==0||endpoint.rfind("ADC1_RANGE_CORR",0)==0){
        if(!((endpoint[endpoint.length()-1]>='1'&&endpoint[endpoint.length()-1]<='9')||(endpoint[endpoint.length()-1]>='A'&&endpoint[endpoint.length()-1]<='C'))){
            return "";
        }
        if(endpoint.rfind("ADC1_BASELINE",0)==0){
            std::string lastNumber = numberLetter(0xE +(endpoint[endpoint.length()-1]-'1')*2);
            address="0000"+pmNumber+lastNumber;
        }
        else if(endpoint.rfind("ADC0_BASELINE",0)==0){
            std::string lastNumber = numberLetter(0xD +(endpoint[endpoint.length()-1]-'1')*2);
            address="0000"+pmNumber+lastNumber;
        }
        else if(endpoint.rfind("ADC0_RANGE_CORR",0)==0){
            std::string lastNumber = numberLetter(0x25+(endpoint[endpoint.length()-1]-'1')*2);
            address="0000"+pmNumber+lastNumber;
        }
        else if(endpoint.rfind("ADC1_RANGE_CORR",0)==0){
            std::string lastNumber = numberLetter(0x26+(endpoint[endpoint.length()-1]-'1')*2);
            address="0000"+std::string(pmNumber.length()>1?"":"0")+pmNumber+lastNumber;
        }
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else{
            this->publishError("Readonly parameter");
            noRpcRequest=true;
            sequence="";
        }
        return sequence;
    }
    else if(endpoint.rfind("TDC_12_PHASE_TUNING",0)==0||endpoint.rfind("TDC_3_PHASE_TUNING",0)==0||endpoint.rfind("RAW_TDC_DATA",0)==0
    ||endpoint.rfind("ADC0_DISPERSION",0)==0||endpoint.rfind("ADC1_DISPERSION",0)==0||endpoint.rfind("ADC0_MEAN",0)==0||endpoint.rfind("ADC1_MEAN",0)==0
    ||endpoint.rfind("CFD_ZERO",0)==0||endpoint.rfind("ADC_ZERO",0)==0||endpoint.rfind("COUNT_CFD_HITS",0)==0){

        if(endpoint.rfind("TDC_3_PHASE_TUNING",0)==0){
            address="0000"+std::string(pmNumber.length()>1?"":"0")+pmNumber+"3F";
        }
        else if(endpoint.rfind("TDC_12_PHASE_TUNING",0)==0){
            address="0000"+std::string(pmNumber.length()>1?"":"0")+pmNumber+"3E";
        }
        else{
            if(!((endpoint[endpoint.length()-1]>='1'&&endpoint[endpoint.length()-1]<='9')||(endpoint[endpoint.length()-1]>='A'&&endpoint[endpoint.length()-1]<='C'))){
                noRpcRequest=true;
                this->publishError("wrong parameter endpoint pm number");
                return "";
            }
            if(endpoint.rfind("RAW_TDC_DATA",0)==0){
                std::string lastNumber = numberLetter(0x40+(endpoint[endpoint.length()-1]-'1'));
                address="0000"+pmNumber+lastNumber;
            }
            else if(endpoint.rfind("ADC0_DISPERSION",0)==0){
                std::string lastNumber = numberLetter(0x4C+(endpoint[endpoint.length()-1]-'1')*2);
                address="0000"+pmNumber+lastNumber;
            }
            else if(endpoint.rfind("ADC1_DISPERSION",0)==0){
                std::string lastNumber = numberLetter(0x4D+(endpoint[endpoint.length()-1]-'1')*2);
                address="0000"+pmNumber+lastNumber;
            }
            else if(endpoint.rfind("ADC0_MEAN",0)==0){
                std::string lastNumber = numberLetter(0x64+(endpoint[endpoint.length()-1]-'1')*2);
                address="0000"+std::string(pmNumber.length()>1?"":"0")+pmNumber+lastNumber;
            }
            else if(endpoint.rfind("ADC1_MEAN",0)==0){
                std::string lastNumber = numberLetter(0x65+(endpoint[endpoint.length()-1]-'1')*2);
                address="0000"+std::string(pmNumber.length()>1?"":"0")+pmNumber+lastNumber;
            }
            else if(endpoint.rfind("CFD_ZERO",0)==0){
                std::string lastNumber = numberLetter(0x81+(endpoint[endpoint.length()-1]-'1')*4);
                address="0000"+std::string(pmNumber.length()>1?"":"0")+pmNumber+lastNumber;
            }
            else if(endpoint.rfind("ADC_ZERO",0)==0){
                std::string lastNumber = numberLetter(0x82+(endpoint[endpoint.length()-1]-'1')*4);
                address="0000"+std::string(pmNumber.length()>1?"":"0")+pmNumber+lastNumber;
            }
            else if(endpoint.rfind("COUNT_CFD_HITS",0)==0){
                std::string lastNumber = numberLetter(0xC0+(endpoint[endpoint.length()-1]-'1')*2);
                address="0000"+std::string(pmNumber.length()>1?"":"0")+pmNumber+lastNumber;
            }
        }

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
    else if(endpoint.rfind("TRG_CHARGE_LEVELS",0)==0){
        address="0000"+pmNumber+"3D";
        int index = parameterValue(parameters[0]);
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            sequenceOperation1(index, address, sequence);
        }
        else if(parameters.size()>2&&(index>=0&&index<=11)){
            sequenceOperationBits(parameterValue(parameters[2]), 0, 0xFFFFF000, address, sequence);
        }
        else if(parameters.size()>2&&(index>=12&&index<=15)){
            sequenceOperationBits(parameterValue(parameters[2]), 12, 0xFFFF0FFF, address, sequence);
        }
        else{
            noRpcRequest=true;
            sequence="";
            this->publishError("Wrong parameters");
        }
        return sequence;
    }
    else if(endpoint=="CHANNELS_MASK"||endpoint=="CHANNEL_ADC_BASELINE"||endpoint.rfind("CFD_THRESHOLD",0)==0||endpoint.rfind("THRESHOLD_CALIBRATION",0)==0||endpoint.rfind("ADC_DELAY",0)==0){
        if(endpoint=="CHANNEL_ADC_BASELINE"){
            address="0000027D";
        }
        else if(endpoint=="CHANNELS_MASK"){
            address="0000027C";
        }
        else{
            if(!((endpoint[endpoint.length()-1]>='1'&&endpoint[endpoint.length()-1]<='9')||(endpoint[endpoint.length()-1]>='A'&&endpoint[endpoint.length()-1]<='C'))){
                noRpcRequest=true;
                return "";
            }
            if(endpoint.rfind("CFD_THRESHOLD",0)==0){
                std::string lastNumber = numberLetter(0x80+(endpoint[endpoint.length()-1]-'1')*4);
                address="0000"+std::string(pmNumber.length()>1?"":"0")+pmNumber+lastNumber;
            }
            else if(endpoint.rfind("ADC_DELAY",0)==0){
                std::string lastNumber = numberLetter(0x83+(endpoint[endpoint.length()-1]-'1')*4);
                address="0000"+std::string(pmNumber.length()>1?"":"0")+pmNumber+lastNumber;
            }
            else if(endpoint.rfind("THRESHOLD_CALIBRATION",0)==0){
                std::string lastNumber = numberLetter(0xB0+(endpoint[endpoint.length()-1]-'1'));
                address="0000"+std::string(pmNumber.length()>1?"":"0")+pmNumber+lastNumber;
            }
        }
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            sequenceOperation1(parameterValue(parameters[0]), address, sequence);
        }
        else{
            sequence="";
            noRpcRequest=true;
            this->publishError("Wrong parameters");
        }
        return sequence;
    }
    else if(endpoint=="HISTOGRAMMING_CONTROL"){
        address="0000027E";
        int index = parameterValue(parameters[0]);
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            sequenceOperation1(parameterValue(parameters[0]), address, sequence);
        }
        else if(parameters.size()>2&&(index>=0&&index<=11)){
            sequenceOperationBits(parameterValue(parameters[2]), 0, 0xFFFFF000, address, sequence);
        }
        else if(parameters.size()>1&&parameters[1]=="3"){
            sequenceOperation3(index, address, sequence);
        }
        else if(parameters.size()>1&&parameters[1]=="2"){
            sequenceOperation2(index, address, sequence);
        }
        else{
            sequence="";
            noRpcRequest=true;
            this->publishError("Wrong parameters");
        }
        return sequence;
    }
    else if(endpoint=="STATUS_BITS"){
        address="0000027F";
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
            noRpcRequest=true;
            sequence="";
            this->publishError("Wrong parameters");
        }
        return sequence;
    }
    else if(endpoint=="BOARD_TEMPERATURE"||endpoint=="BOARD_ID"||endpoint=="LAST_RESTART_REASON"){
        if(endpoint=="BOARD_ID"){
            address="0000"+pmNumber+"BD";
        }
        else if(endpoint=="BOARD_TEMPERATURE"){
            address="0000"+pmNumber+"BC";
        }
        else if(endpoint=="LAST_RESTART_REASON"){
            address="0000"+pmNumber+"BE";
        }
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
    else if(endpoint=="MODE_SETTINGS"){
        address="0000"+pmNumber+"D8";
        int index = parameterValue(parameters[0]);
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            sequenceOperation1(index, address, sequence);
        }
        else if(parameters.size()>2&&(index>=0&&index<=3)){
            sequenceOperationBits(index, 0, 0xFFFFFFF0, address, sequence);
        }
        else if(parameters.size()>2&&(index>=4&&index<=7)){
            sequenceOperationBits(index, 4, 0xFFFFFF0F, address, sequence);
        }
        else if(parameters.size()>2&&(index>=16&&index<=19)){
            sequenceOperationBits(index, 16, 0xFFF0FFFF, address, sequence);
        }
        else if(parameters.size()>1&&parameters[1]=="3"){
            sequenceOperation3(index, address, sequence);
        }
        else if(parameters.size()>1&&parameters[1]=="2"){
            sequenceOperation2(index, address, sequence);
        }
        else{
            noRpcRequest=true;
            sequence="";
            this->publishError("Wrong parameters");
        }
        return sequence;
    }
    else if(endpoint=="TRIGGER_RESPOND_MASK"||endpoint=="LAST_RESTART_REASON"){
        if(endpoint=="TRIGGER_RESPOND_MASK"){
            address="0000"+pmNumber+"D9";
        }
        else if(endpoint=="LAST_RESTART_REASON"){
            address="0000"+pmNumber+"BE";
        }
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            sequenceOperation1(parameterValue(parameters[0]), address, sequence);
        }
         else{
            noRpcRequest=true;
            sequence="";
            this->publishError("Wrong parameters");
        }
        return sequence;
    }
    else if(endpoint=="DATA_BUNCH_PATTERN"||endpoint=="TRIGGER_SINGLE_VALUE"||endpoint=="TRIGGER_CONT_PATTERN_MSB"||endpoint=="TRIGGER_CONT_PATTERN_LSB"||endpoint=="TRIGGER_CONT_VALUE"
    ||endpoint=="GENERATORS_BUNCH_FREQ"||endpoint=="GENERATORS_FREQ_OFFSET"){
        if(endpoint=="TRIGGER_SINGLE_VALUE"){
            address="0000"+pmNumber+"DB";
        }
        else if(endpoint=="DATA_BUNCH_PATTERN"){
            address="0000"+pmNumber+"DA";
        }
        else if(endpoint=="TRIGGER_CONT_PATTERN_MSB"){
            address="0000"+pmNumber+"DC";
        }
        else if(endpoint=="TRIGGER_CONT_PATTERN_LSB"){
            address="0000"+pmNumber+"DD";
        }
        else if(endpoint=="TRIGGER_CONT_VALUE"){
            address="0000"+pmNumber+"DE";
        }
        else if(endpoint=="GENERATORS_BUNCH_FREQ"){
            address="0000"+pmNumber+"DF";
        }
        else if(endpoint=="GENERATORS_FREQ_OFFSET"){
            address="0000"+pmNumber+"E0";
        }
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            sequenceOperation1(parameterValue(parameters[0]), address, sequence);
        }
        else{
            noRpcRequest=true;
            sequence="";
            this->publishError("Wrong parameters");
        }
        return sequence;
    }
    else if(endpoint=="RDH_FIELDS1"){
        address="0000"+pmNumber+"E1";
        int index = parameterValue(parameters[0]);
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            sequenceOperation1(index, address, sequence);
        }
        else if(parameters.size()>2&&(index>=0&&index<=15)){
            sequenceOperationBits(index, 0, 0xFFFF0000, address, sequence);
        }
        else if(parameters.size()>2&&(index>=16&&index<=31)){
            sequenceOperationBits(index, 16, 0x0000FFFF, address, sequence);
        }
        else{
            noRpcRequest=true;
            sequence="";
            this->publishError("Wrong parameters");
        }
        return sequence;
    }
    else if(endpoint=="RDH_FIELDS2"){
        address="0000"+pmNumber+"E2";
        int index = parameterValue(parameters[0]);
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            sequenceOperation1(index, address, sequence);
        }
        else if(parameters.size()>2&&(index>=0&&index<=15)){
            sequenceOperationBits(parameterValue(parameters[2]), 0, 0xFFFF0000, address, sequence);
        }
        else if(parameters.size()>2&&(index>=16&&index<=31)){
            sequenceOperationBits(parameterValue(parameters[2]), 16, 0x0000FFFF, address, sequence);
        }
        else{
            noRpcRequest=true;
            sequence="";
            this->publishError("Wrong parameters");
        }
        return sequence;
    }
    else if(endpoint=="DELAYS"){
        address="0000"+pmNumber+"E3";
        int index = parameterValue(parameters[0]);
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            sequenceOperation1(index, address, sequence);
        }
        else if(parameters.size()>2&&(index>=0&&index<=11)){
            sequenceOperationBits(parameterValue(parameters[2]), 0, 0xFFFFF000, address, sequence);
        }
        else if(parameters.size()>2&&(index>=16&&index<=27)){
            sequenceOperationBits(parameterValue(parameters[2]), 16, 0xF000FFFF, address, sequence);
        }
        else{
            noRpcRequest=true;
            sequence="";
            this->publishError("Wrong parameters");
        }
        return sequence;
    }
    else if(endpoint=="MODES_STATUS"){
        address="0000"+pmNumber+"E8";
        int index = parameterValue(parameters[0]);
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            sequenceOperation1(index, address, sequence);
        }
        else if(parameters.size()>2&&(index>=16&&index<=19)){
            sequenceOperationBits(index, 16, 0xFFF0FFFF, address, sequence);
        }
        else if(parameters.size()>2&&(index>=20&&index<=23)){
            sequenceOperationBits(index, 20, 0xFF0FFFFF, address, sequence);
        }
        else if(parameters.size()>2&&(index>=24&&index<=27)){
            sequenceOperationBits(index, 24, 0xF0FFFFFF, address, sequence);
        }
        else if(parameters.size()>2&&(index>=28&&index<=31)){
            sequenceOperationBits(index, 28, 0x0FFFFFFF, address, sequence);
        }
        else if(parameters.size()>1&&parameters[1]=="3"){
            sequenceOperation3(index, address, sequence);
        }
        else if(parameters.size()>1&&parameters[1]=="2"){
            sequenceOperation2(index, address, sequence);
        }
        else{
            noRpcRequest=true;
            sequence="";
            this->publishError("Wrong parameters");
        }
        return sequence;
    }
    else if(endpoint=="FIFO_COUNT"){
        address="0000"+pmNumber+"EB";
        int index = parameterValue(parameters[0]);
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            sequenceOperation1(index, address, sequence);
        }
        else if(parameters.size()>2&&(index>=0&&index<=15)){
            sequenceOperationBits(parameterValue(parameters[2]), 0, 0xFFFF0000, address, sequence);
        }
        else if(parameters.size()>2&&(index>=16&&index<=31)){
            sequenceOperationBits(parameterValue(parameters[2]), 16, 0x0000FFFF, address, sequence);
        }
        else{
            noRpcRequest=true;
            sequence="";
            this->publishError("Wrong parameters");
        }
        return sequence;
    }
    else if(endpoint=="CURRENT_ADDRESS"||endpoint=="HISTOGRAM_DATA_READOUT"||endpoint=="READOUT_RATE"||endpoint=="SEL_HITS_DROPPED"||endpoint=="SEL_LAST_HIT_DROPPED_ORBIT"
    ||endpoint=="SEL_FIRST_HIT_DROPPED_ORBIT"||endpoint=="CRU_BC"||endpoint=="CRU_ORBIT"||endpoint=="DATA_SELECT_TRG_MASK"){
        if(endpoint=="HISTOGRAM_DATA_READOUT"){
            address="0000"+pmNumber+"F5";
        }
        else if(endpoint=="CURRENT_ADDRESS"){
            address="0000"+pmNumber+"F5";
        }
        else if(endpoint=="READOUT_RATE"){
            address="0000"+pmNumber+"EF";
        }
        else if(endpoint=="SEL_HITS_DROPPED"){
            address="0000"+pmNumber+"EE";
        }
        else if(endpoint=="SEL_LAST_HIT_DROPPED_ORBIT"){
            address="0000"+pmNumber+"ED";
        }
        else if(endpoint=="SEL_FIRST_HIT_DROPPED_ORBIT"){
            address="0000"+pmNumber+"EC";
        }
        else if(endpoint=="CRU_BC"){
            address="0000"+pmNumber+"EA";
        }
        else if(endpoint=="CRU_ORBIT"){
            address="0000"+pmNumber+"E9";
        }
        else if(endpoint=="DATA_SELECT_TRG_MASK"){
            address="0000"+pmNumber+"E4";
        }
        int index = parameterValue(parameters[0]);
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            sequenceOperation1(index, address, sequence);
        }
        else{
            noRpcRequest=true;
            sequence="";
            this->publishError("Wrong parameters");
        }
        return sequence;
    }
    else if(endpoint=="FPGA_TEMPERATURE"||endpoint=="1VPOWER"||endpoint=="18VPOWER"||endpoint=="FPGA_TIMESTAMP"){
        if(endpoint=="1VPOWER"){
            address="0000"+pmNumber+"FD";
        }
        else if(endpoint=="FPGA_TEMPERATURE"){
            address="0000"+pmNumber+"FC";
        }
        else if(endpoint=="18VPOWER"){
            address="0000"+pmNumber+"FE";
        }
        else if(endpoint=="FPGA_TIMESTAMP"){
            address="0000"+pmNumber+"FF";
        }
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else{
            sequence="";
            this->publishError("Readonly parameter");
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
    }
    else if(endpoint.rfind("RAW_TDC_DATA",0)==0){
        std::stringstream ss;
        ss << std::hex << finalValue;
        return "0x"+ss.str();
    }
    else if(endpoint.rfind("ADC0_DISPERSION",0)==0||endpoint.rfind("ADC1_DISPERSION",0)==0){
        return std::to_string(std::sqrt(finalValue));
    }
    else if(endpoint == "BOARD_TEMPERATURE"){
        return std::to_string(finalValue/10.0);
    }
    else if(endpoint == "FPGA_TEMPERATURE"){
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
    ||endpoint.rfind("BOARD_TEMPERATURE",0)==0)) {
      int16_t x = stoi(value, nullptr, 16);
      finalValue=-(~x+1);
    }

    return to_string(finalValue);

}

