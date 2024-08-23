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

long long PM_default::parameterValue(std::string strValue){
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
    return numberLetterStr;
}



PM_default::PM_default(std::string endpointParam, std::string pmNumberParam) {
    finalValue = 0;
    pmNumber = pmNumberParam;
    endpoint=endpointParam;
}

string PM_default::processInputMessage(string input) {
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
    else if(endpoint.rfind("ADC0_BASELINE",0)==0){
        if(!((endpoint[endpoint.length()-1]>='1'&&endpoint[endpoint.length()-1]<='9')||(endpoint[endpoint.length()-1]>='A'&&endpoint[endpoint.length()-1]<='C'))){
            return "";
        }
        std::string lastNumber = numberLetter(0xD +(endpoint[endpoint.length()-1]-'1')*2);
        /*if(endpoint[endpoint.length()-1]=='1'){ lastNumber="0D";}
        else if(endpoint[endpoint.length()-1]=='2'){ lastNumber="0F";}
        else if(endpoint[endpoint.length()-1]=='3'){ lastNumber="11";}
        else if(endpoint[endpoint.length()-1]=='4'){ lastNumber="13";}
        else if(endpoint[endpoint.length()-1]=='5'){ lastNumber="15";}
        else if(endpoint[endpoint.length()-1]=='6'){ lastNumber="17";}
        else if(endpoint[endpoint.length()-1]=='7'){ lastNumber="19";}
        else if(endpoint[endpoint.length()-1]=='8'){ lastNumber="1B";}
        else if(endpoint[endpoint.length()-1]=='9'){ lastNumber="1D";}
        else if(endpoint[endpoint.length()-1]=='A'){ lastNumber="1F";}
        else if(endpoint[endpoint.length()-1]=='B'){ lastNumber="21";}
        else if(endpoint[endpoint.length()-1]=='C'){ lastNumber="23";}*/
        address="0000"+pmNumber+lastNumber;
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
    else if(endpoint.rfind("ADC1_BASELINE",0)==0){
        if(!((endpoint[endpoint.length()-1]>='1'&&endpoint[endpoint.length()-1]<='9')||(endpoint[endpoint.length()-1]>='A'&&endpoint[endpoint.length()-1]<='C'))){
            return "";
        }
        std::string lastNumber = numberLetter(0xE +(endpoint[endpoint.length()-1]-'1')*2);
        /*if(endpoint[endpoint.length()-1]=='1'){ lastNumber="0E";}
        else if(endpoint[endpoint.length()-1]=='2'){ lastNumber="10";}
        else if(endpoint[endpoint.length()-1]=='3'){ lastNumber="12";}
        else if(endpoint[endpoint.length()-1]=='4'){ lastNumber="14";}
        else if(endpoint[endpoint.length()-1]=='5'){ lastNumber="16";}
        else if(endpoint[endpoint.length()-1]=='6'){ lastNumber="18";}
        else if(endpoint[endpoint.length()-1]=='7'){ lastNumber="1A"; }
        else if(endpoint[endpoint.length()-1]=='8'){ lastNumber="1C";}
        else if(endpoint[endpoint.length()-1]=='9'){ lastNumber="1E";}
        else if(endpoint[endpoint.length()-1]=='A'){ lastNumber="20";}
        else if(endpoint[endpoint.length()-1]=='B'){ lastNumber="22";}
        else if(endpoint[endpoint.length()-1]=='C'){ lastNumber="24";}*/
        address="0000"+std::string(pmNumber.length()>1?"":"0")+pmNumber+lastNumber;
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
    else if(endpoint.rfind("ADC0_RANGE_CORR",0)==0){
        if(!((endpoint[endpoint.length()-1]>='1'&&endpoint[endpoint.length()-1]<='9')||(endpoint[endpoint.length()-1]>='A'&&endpoint[endpoint.length()-1]<='C'))){
            return "";
        }
        std::string lastNumber = numberLetter(0x25+(endpoint[endpoint.length()-1]-'1')*2);
        /*if(endpoint[endpoint.length()-1]=='1'){ lastNumber="25";}
        else if(endpoint[endpoint.length()-1]=='2'){ lastNumber="27";}
        else if(endpoint[endpoint.length()-1]=='3'){ lastNumber="29";}
        else if(endpoint[endpoint.length()-1]=='4'){ lastNumber="2B";}
        else if(endpoint[endpoint.length()-1]=='5'){ lastNumber="2D";}
        else if(endpoint[endpoint.length()-1]=='6'){ lastNumber="2F";}
        else if(endpoint[endpoint.length()-1]=='7'){ lastNumber="31";}
        else if(endpoint[endpoint.length()-1]=='8'){ lastNumber="33";}
        else if(endpoint[endpoint.length()-1]=='9'){ lastNumber="35";}
        else if(endpoint[endpoint.length()-1]=='A'){ lastNumber="37";}
        else if(endpoint[endpoint.length()-1]=='B'){ lastNumber="39";}
        else if(endpoint[endpoint.length()-1]=='C'){ lastNumber="3B";}*/

        address="0000"+pmNumber+lastNumber;
        int index = parameterValue(parameters[0]);
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            sequenceOperation1(index, address, sequence);
        }
        else{
            noRpcRequest=true;
            this->publishError("Readonly parameter");
            sequence="";
        }
        return sequence;
    }
    else if(endpoint.rfind("ADC1_RANGE_CORR",0)==0){
        if(!((endpoint[endpoint.length()-1]>='1'&&endpoint[endpoint.length()-1]<='9')||(endpoint[endpoint.length()-1]>='A'&&endpoint[endpoint.length()-1]<='C'))){
            return "";
        }
        std::string lastNumber = numberLetter(0x26+(endpoint[endpoint.length()-1]-'1')*2);
        /*if(endpoint[endpoint.length()-1]=='1'){ lastNumber="26";}
        else if(endpoint[endpoint.length()-1]=='2'){ lastNumber="28";}
        else if(endpoint[endpoint.length()-1]=='3'){ lastNumber="2A";}
        else if(endpoint[endpoint.length()-1]=='4'){ lastNumber="2C";}
        else if(endpoint[endpoint.length()-1]=='5'){ lastNumber="2E";}
        else if(endpoint[endpoint.length()-1]=='6'){ lastNumber="30";}
        else if(endpoint[endpoint.length()-1]=='7'){ lastNumber="32";}
        else if(endpoint[endpoint.length()-1]=='8'){ lastNumber="34";}
        else if(endpoint[endpoint.length()-1]=='9'){ lastNumber="36";}
        else if(endpoint[endpoint.length()-1]=='A'){ lastNumber="38";}
        else if(endpoint[endpoint.length()-1]=='B'){ lastNumber="3A";}
        else if(endpoint[endpoint.length()-1]=='C'){ lastNumber="3C";}*/
        address="0000"+std::string(pmNumber.length()>1?"":"0")+pmNumber+lastNumber;
        int index = parameterValue(parameters[0]);
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            sequenceOperation1(index, address, sequence);
        }
        else{
            noRpcRequest=true;
            this->publishError("Readonly parameter");
            sequence="";
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
    else if(endpoint.rfind("TDC_12_PHASE_TUNING",0)==0||endpoint.rfind("TDC_3_PHASE_TUNING",0)==0){
        if(endpoint.rfind("TDC_3_PHASE_TUNING",0)==0){
            address="0000"+std::string(pmNumber.length()>1?"":"0")+pmNumber+"3F";
        }
        else if(endpoint.rfind("TDC_12_PHASE_TUNING",0)==0){
            address="0000"+std::string(pmNumber.length()>1?"":"0")+pmNumber+"3E";
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
    else if(endpoint.rfind("RAW_TDC_DATA",0)==0){
        if(!((endpoint[endpoint.length()-1]>='1'&&endpoint[endpoint.length()-1]<='9')||(endpoint[endpoint.length()-1]>='A'&&endpoint[endpoint.length()-1]<='C'))){
            return "";
        }
        //std::string lastNumber = "";
        std::string lastNumber = numberLetter(0x40+(endpoint[endpoint.length()-1]-'1'));
        /*if(endpoint[endpoint.length()-1]=='1'){ lastNumber="40";}
        else if(endpoint[endpoint.length()-1]=='2'){ lastNumber="41";}
        else if(endpoint[endpoint.length()-1]=='3'){ lastNumber="42";}
        else if(endpoint[endpoint.length()-1]=='4'){ lastNumber="43";}
        else if(endpoint[endpoint.length()-1]=='5'){ lastNumber="44";}
        else if(endpoint[endpoint.length()-1]=='6'){ lastNumber="45";}
        else if(endpoint[endpoint.length()-1]=='7'){ lastNumber="46";}
        else if(endpoint[endpoint.length()-1]=='8'){ lastNumber="47";}
        else if(endpoint[endpoint.length()-1]=='9'){ lastNumber="48";}
        else if(endpoint[endpoint.length()-1]=='A'){ lastNumber="49";}
        else if(endpoint[endpoint.length()-1]=='B'){ lastNumber="4A";}
        else if(endpoint[endpoint.length()-1]=='C'){ lastNumber="4B";}*/
        address="0000"+pmNumber+lastNumber;
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
    else if(endpoint.rfind("ADC0_DISPERSION",0)==0){
        if(!((endpoint[endpoint.length()-1]>='1'&&endpoint[endpoint.length()-1]<='9')||(endpoint[endpoint.length()-1]>='A'&&endpoint[endpoint.length()-1]<='C'))){
            return "";
        }
        std::string lastNumber = numberLetter(0x4C+(endpoint[endpoint.length()-1]-'1')*2);
        /*if(endpoint[endpoint.length()-1]=='1'){ lastNumber="4C";}
        else if(endpoint[endpoint.length()-1]=='2'){ lastNumber="4E";}
        else if(endpoint[endpoint.length()-1]=='3'){ lastNumber="50";}
        else if(endpoint[endpoint.length()-1]=='4'){ lastNumber="52";}
        else if(endpoint[endpoint.length()-1]=='5'){ lastNumber="54";}
        else if(endpoint[endpoint.length()-1]=='6'){ lastNumber="56";}
        else if(endpoint[endpoint.length()-1]=='7'){ lastNumber="58";}
        else if(endpoint[endpoint.length()-1]=='8'){ lastNumber="5A";}
        else if(endpoint[endpoint.length()-1]=='9'){ lastNumber="5C";}
        else if(endpoint[endpoint.length()-1]=='A'){ lastNumber="5E";}
        else if(endpoint[endpoint.length()-1]=='B'){ lastNumber="60";}
        else if(endpoint[endpoint.length()-1]=='C'){ lastNumber="62";}*/
        address="0000"+pmNumber+lastNumber;
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
    else if(endpoint.rfind("ADC1_DISPERSION",0)==0){
        if(!((endpoint[endpoint.length()-1]>='1'&&endpoint[endpoint.length()-1]<='9')||(endpoint[endpoint.length()-1]>='A'&&endpoint[endpoint.length()-1]<='C'))){
            return "";
        }
        std::string lastNumber = numberLetter(0x4D+(endpoint[endpoint.length()-1]-'1')*2);
        /*if(endpoint[endpoint.length()-1]=='1'){ lastNumber="4D";}
        else if(endpoint[endpoint.length()-1]=='2'){ lastNumber="4F";}
        else if(endpoint[endpoint.length()-1]=='3'){ lastNumber="51";}
        else if(endpoint[endpoint.length()-1]=='4'){ lastNumber="53";}
        else if(endpoint[endpoint.length()-1]=='5'){ lastNumber="55";}
        else if(endpoint[endpoint.length()-1]=='6'){ lastNumber="57";}
        else if(endpoint[endpoint.length()-1]=='7'){ lastNumber="59";}
        else if(endpoint[endpoint.length()-1]=='8'){ lastNumber="5B";}
        else if(endpoint[endpoint.length()-1]=='9'){ lastNumber="5D";}
        else if(endpoint[endpoint.length()-1]=='A'){ lastNumber="5F";}
        else if(endpoint[endpoint.length()-1]=='B'){ lastNumber="61";}
        else if(endpoint[endpoint.length()-1]=='C'){ lastNumber="63";}*/
        address="0000"+pmNumber+lastNumber;
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
    else if(endpoint.rfind("ADC0_MEAN",0)==0){
        if(!((endpoint[endpoint.length()-1]>='1'&&endpoint[endpoint.length()-1]<='9')||(endpoint[endpoint.length()-1]>='A'&&endpoint[endpoint.length()-1]<='C'))){
            return "";
        }
        std::string lastNumber = numberLetter(0x64+(endpoint[endpoint.length()-1]-'1')*2);
        /*if(endpoint[endpoint.length()-1]=='1'){ lastNumber="64";}
        else if(endpoint[endpoint.length()-1]=='2'){ lastNumber="66";}
        else if(endpoint[endpoint.length()-1]=='3'){ lastNumber="68";}
        else if(endpoint[endpoint.length()-1]=='4'){ lastNumber="6A";}
        else if(endpoint[endpoint.length()-1]=='5'){ lastNumber="6C";}
        else if(endpoint[endpoint.length()-1]=='6'){ lastNumber="6E";}
        else if(endpoint[endpoint.length()-1]=='7'){ lastNumber="70";}
        else if(endpoint[endpoint.length()-1]=='8'){ lastNumber="72";}
        else if(endpoint[endpoint.length()-1]=='9'){ lastNumber="74";}
        else if(endpoint[endpoint.length()-1]=='A'){ lastNumber="76";}
        else if(endpoint[endpoint.length()-1]=='B'){ lastNumber="78";}
        else if(endpoint[endpoint.length()-1]=='C'){ lastNumber="7A";}*/
        address="0000"+std::string(pmNumber.length()>1?"":"0")+pmNumber+lastNumber;
        int index = std::stoi(parameters[0]);
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
    else if(endpoint.rfind("ADC1_MEAN",0)==0){
        if(!((endpoint[endpoint.length()-1]>='1'&&endpoint[endpoint.length()-1]<='9')||(endpoint[endpoint.length()-1]>='A'&&endpoint[endpoint.length()-1]<='C'))){
            return "";
        }
        std::string lastNumber = numberLetter(0x65+(endpoint[endpoint.length()-1]-'1')*2);
        /*if(endpoint[endpoint.length()-1]=='1'){ lastNumber="65";}
        else if(endpoint[endpoint.length()-1]=='2'){ lastNumber="67";}
        else if(endpoint[endpoint.length()-1]=='3'){ lastNumber="69";}
        else if(endpoint[endpoint.length()-1]=='4'){ lastNumber="6B";}
        else if(endpoint[endpoint.length()-1]=='5'){ lastNumber="6D";}
        else if(endpoint[endpoint.length()-1]=='6'){ lastNumber="6F";}
        else if(endpoint[endpoint.length()-1]=='7'){ lastNumber="71";}
        else if(endpoint[endpoint.length()-1]=='8'){ lastNumber="73";}
        else if(endpoint[endpoint.length()-1]=='9'){ lastNumber="75";}
        else if(endpoint[endpoint.length()-1]=='A'){ lastNumber="77";}
        else if(endpoint[endpoint.length()-1]=='B'){ lastNumber="79";}
        else if(endpoint[endpoint.length()-1]=='C'){ lastNumber="7B";}*/
        address="0000"+std::string(pmNumber.length()>1?"":"0")+pmNumber+lastNumber;
        int index = std::stoi(parameters[0]);
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
    else if(endpoint=="CHANNELS_MASK"){
        address="0000027C";
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
    else if(endpoint=="CHANNEL_ADC_BASELINE"){
        address="0000027D";
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
    else if(endpoint.rfind("CFD_THRESHOLD",0)==0){
        if(!((endpoint[endpoint.length()-1]>='1'&&endpoint[endpoint.length()-1]<='9')||(endpoint[endpoint.length()-1]>='A'&&endpoint[endpoint.length()-1]<='C'))){
            return "";
        }
        std::string lastNumber = numberLetter(0x80+(endpoint[endpoint.length()-1]-'1')*4);
        /*if(endpoint[endpoint.length()-1]=='1'){ lastNumber="80";
        }
        else if(endpoint[endpoint.length()-1]=='2'){ lastNumber="84";
        }
        else if(endpoint[endpoint.length()-1]=='3'){ lastNumber="88";
        }
        else if(endpoint[endpoint.length()-1]=='4'){ lastNumber="8C";
        }
        else if(endpoint[endpoint.length()-1]=='5'){ lastNumber="90";
        }
        else if(endpoint[endpoint.length()-1]=='6'){ lastNumber="94";
        }
        else if(endpoint[endpoint.length()-1]=='7'){ lastNumber="98";
        }
        else if(endpoint[endpoint.length()-1]=='8'){ lastNumber="9C";
        }
        else if(endpoint[endpoint.length()-1]=='9'){ lastNumber="A0";
        }
        else if(endpoint[endpoint.length()-1]=='A'){ lastNumber="A4";
        }
        else if(endpoint[endpoint.length()-1]=='B'){ lastNumber="A8";
        }
        else if(endpoint[endpoint.length()-1]=='C'){ lastNumber="AC";
        }*/
        address="0000"+std::string(pmNumber.length()>1?"":"0")+pmNumber+lastNumber;
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
            this->publishError("Readonly parameter");
        }
        return sequence;
    }
    else if(endpoint.rfind("CFD_ZERO",0)==0){
        if(!((endpoint[endpoint.length()-1]>='1'&&endpoint[endpoint.length()-1]<='9')||(endpoint[endpoint.length()-1]>='A'&&endpoint[endpoint.length()-1]<='C'))){
            return "";
        }
        std::string lastNumber = numberLetter(0x81+(endpoint[endpoint.length()-1]-'1')*4);
        /*if(endpoint[endpoint.length()-1]=='1'){ lastNumber="81";
        }
        else if(endpoint[endpoint.length()-1]=='2'){ lastNumber="85";
        }
        else if(endpoint[endpoint.length()-1]=='3'){ lastNumber="89";
        }
        else if(endpoint[endpoint.length()-1]=='4'){ lastNumber="8D";
        }
        else if(endpoint[endpoint.length()-1]=='5'){ lastNumber="91";
        }
        else if(endpoint[endpoint.length()-1]=='6'){ lastNumber="95";
        }
        else if(endpoint[endpoint.length()-1]=='7'){ lastNumber="99";
        }
        else if(endpoint[endpoint.length()-1]=='8'){ lastNumber="9D";
        }
        else if(endpoint[endpoint.length()-1]=='9'){ lastNumber="A1";
        }
        else if(endpoint[endpoint.length()-1]=='A'){ lastNumber="A5";
        }
        else if(endpoint[endpoint.length()-1]=='B'){ lastNumber="A9";
        }
        else if(endpoint[endpoint.length()-1]=='C'){ lastNumber="AD";
        }*/
        address="0000"+std::string(pmNumber.length()>1?"":"0")+pmNumber+lastNumber;
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
            this->publishError("Readonly parameter");
        }
        return sequence;
    }
    else if(endpoint.rfind("ADC_ZERO",0)==0){
        if(!((endpoint[endpoint.length()-1]>='1'&&endpoint[endpoint.length()-1]<='9')||(endpoint[endpoint.length()-1]>='A'&&endpoint[endpoint.length()-1]<='C'))){
            return "";
        }
        std::string lastNumber = numberLetter(0x82+(endpoint[endpoint.length()-1]-'1')*4);
        /*std::string lastNumber = "";
        if(endpoint[endpoint.length()-1]=='1'){ lastNumber="82";
        }
        else if(endpoint[endpoint.length()-1]=='2'){ lastNumber="86";
        }
        else if(endpoint[endpoint.length()-1]=='3'){ lastNumber="8A";
        }
        else if(endpoint[endpoint.length()-1]=='4'){ lastNumber="8E";
        }
        else if(endpoint[endpoint.length()-1]=='5'){ lastNumber="92";
        }
        else if(endpoint[endpoint.length()-1]=='6'){ lastNumber="96";
        }
        else if(endpoint[endpoint.length()-1]=='7'){ lastNumber="9A";
        }
        else if(endpoint[endpoint.length()-1]=='8'){ lastNumber="9E";
        }
        else if(endpoint[endpoint.length()-1]=='9'){ lastNumber="A2";
        }
        else if(endpoint[endpoint.length()-1]=='A'){ lastNumber="A6";
        }
        else if(endpoint[endpoint.length()-1]=='B'){ lastNumber="AA";
        }
        else if(endpoint[endpoint.length()-1]=='C'){ lastNumber="AE";
        }*/
        address="0000"+std::string(pmNumber.length()>1?"":"0")+pmNumber+lastNumber;
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
            this->publishError("Readonly parameter");
        }
        return sequence;
    }
    else if(endpoint.rfind("ADC_DELAY",0)==0){
        if(!((endpoint[endpoint.length()-1]>='1'&&endpoint[endpoint.length()-1]<='9')||(endpoint[endpoint.length()-1]>='A'&&endpoint[endpoint.length()-1]<='C'))){
            return "";
        }
        std::string lastNumber = numberLetter(0x83+(endpoint[endpoint.length()-1]-'1')*4);
        /*if(endpoint[endpoint.length()-1]=='1'){ lastNumber="83";
        }
        else if(endpoint[endpoint.length()-1]=='2'){ lastNumber="87";
        }
        else if(endpoint[endpoint.length()-1]=='3'){ lastNumber="9B";
        }
        else if(endpoint[endpoint.length()-1]=='4'){ lastNumber="8F";
        }
        else if(endpoint[endpoint.length()-1]=='5'){ lastNumber="93";
        }
        else if(endpoint[endpoint.length()-1]=='6'){ lastNumber="97";
        }
        else if(endpoint[endpoint.length()-1]=='7'){ lastNumber="9B";
        }
        else if(endpoint[endpoint.length()-1]=='8'){ lastNumber="9F";
        }
        else if(endpoint[endpoint.length()-1]=='9'){ lastNumber="A3";
        }
        else if(endpoint[endpoint.length()-1]=='A'){ lastNumber="A7";
        }
        else if(endpoint[endpoint.length()-1]=='B'){ lastNumber="AB";
        }
        else if(endpoint[endpoint.length()-1]=='C'){ lastNumber="AF";
        }*/
        address="0000"+std::string(pmNumber.length()>1?"":"0")+pmNumber+lastNumber;
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
            this->publishError("Readonly parameter");
        }
        return sequence;
    }
    else if(endpoint.rfind("THRESHOLD_CALIBRATION",0)==0){
        if(!((endpoint[endpoint.length()-1]>='1'&&endpoint[endpoint.length()-1]<='9')||(endpoint[endpoint.length()-1]>='A'&&endpoint[endpoint.length()-1]<='C'))){
            return "";
        }
        std::string lastNumber = numberLetter(0xB0+(endpoint[endpoint.length()-1]-'1'));
        /*if(endpoint[endpoint.length()-1]=='1'){ lastNumber="B0";
        }
        else if(endpoint[endpoint.length()-1]=='2'){ lastNumber="B1";
        }
        else if(endpoint[endpoint.length()-1]=='3'){ lastNumber="B2";
        }
        else if(endpoint[endpoint.length()-1]=='4'){ lastNumber="B3";
        }
        else if(endpoint[endpoint.length()-1]=='5'){ lastNumber="B4";
        }
        else if(endpoint[endpoint.length()-1]=='6'){ lastNumber="B5";
        }
        else if(endpoint[endpoint.length()-1]=='7'){ lastNumber="B6";
        }
        else if(endpoint[endpoint.length()-1]=='8'){ lastNumber="B7";
        }
        else if(endpoint[endpoint.length()-1]=='9'){ lastNumber="B8";
        }
        else if(endpoint[endpoint.length()-1]=='A'){ lastNumber="B9";
        }
        else if(endpoint[endpoint.length()-1]=='B'){ lastNumber="BA";
        }
        else if(endpoint[endpoint.length()-1]=='C'){ lastNumber="BB";
        }*/
        address="0000"+std::string(pmNumber.length()>1?"":"0")+pmNumber+lastNumber;
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
    else if(endpoint.rfind("COUNT_CFD_HITS",0)==0){
        if(!((endpoint[endpoint.length()-1]>='1'&&endpoint[endpoint.length()-1]<='9')||(endpoint[endpoint.length()-1]>='A'&&endpoint[endpoint.length()-1]<='C'))){
            return "";
        }
        std::string lastNumber = numberLetter(0xC0+(endpoint[endpoint.length()-1]-'1')*2);
        /*if(endpoint[endpoint.length()-1]=='1'){ lastNumber="C0";
        }
        else if(endpoint[endpoint.length()-1]=='2'){ lastNumber="C2";
        }
        else if(endpoint[endpoint.length()-1]=='3'){ lastNumber="C4";
        }
        else if(endpoint[endpoint.length()-1]=='4'){ lastNumber="C6";
        }
        else if(endpoint[endpoint.length()-1]=='5'){ lastNumber="C8";
        }
        else if(endpoint[endpoint.length()-1]=='6'){ lastNumber="CA";
        }
        else if(endpoint[endpoint.length()-1]=='7'){ lastNumber="CC";
        }
        else if(endpoint[endpoint.length()-1]=='8'){ lastNumber="CE";
        }
        else if(endpoint[endpoint.length()-1]=='9'){ lastNumber="D0";
        }
        else if(endpoint[endpoint.length()-1]=='A'){ lastNumber="D2";
        }
        else if(endpoint[endpoint.length()-1]=='B'){ lastNumber="D4";
        }
        else if(endpoint[endpoint.length()-1]=='C'){ lastNumber="D6";
        }*/
        address="0000"+std::string(pmNumber.length()>1?"":"0")+pmNumber+lastNumber;
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
    else if(endpoint=="TRIGGER_RESPOND_MASK"){
        address="0000"+pmNumber+"D9";
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
    else if(endpoint=="LAST_RESTART_REASON"){
        address="0000"+pmNumber+"BE";
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
            sequenceOperation1(index, address, sequence);
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
            sequenceOperationBits(index, 0, 0x0000FFFF, address, sequence);
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
        int index = -1;
        try{
            index=std::stoi(parameters[0]);
        }
        catch(exception& e){
            index=-1;    
        }
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            int num = std::stoi(parameters[0]);
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
        else if(parameters.size()>2&&(index>=0&&index<=15)){
            int numValue = std::stoi(parameters[2]);
            int temp = 0xFFFF0000;
            std::stringstream ss;
            ss << std::hex << temp;
            std::string hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            temp=numValue;
            std::stringstream ss2;
            ss2 << std::hex << temp;
            std::string data2 = "";
            hex_str = ss2.str();
            for(int i=0; i<8-hex_str.length(); i++){
                data2+="0";
            }
            data2+=hex_str;
            sequence = "reset\n0x002"+address+data+",write\nread\n0x003"+address+data2+",write\nread";
        }
        else if(parameters.size()>2&&(index>=16&&index<=31)){
            int numValue = std::stoi(parameters[2]);
            int temp = 0x0000FFFF;
            std::stringstream ss;
            ss << std::hex << temp;
            std::string hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            temp=numValue*std::pow(2,16);
            std::stringstream ss2;
            ss2 << std::hex << temp;
            std::string data2 = "";
            hex_str = ss2.str();
            for(int i=0; i<8-hex_str.length(); i++){
                data2+="0";
            }
            data2+=hex_str;
            sequence = "reset\n0x002"+address+data+",write\nread\n0x003"+address+data2+",write\nread";
        }
        else{
            sequence="";
            this->publishError("Wrong parameters");
        }
        return sequence;
    }
    else if(endpoint=="DELAYS"){
        address="0000"+pmNumber+"E3";
        int index = -1;
        try{
            index=std::stoi(parameters[0]);
        }
        catch(exception& e){
            index=-1;    
        }
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            int num = std::stoi(parameters[0]);
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
        else if(parameters.size()>2&&(index>=0&&index<=11)){
            int numValue = std::stoi(parameters[2]);
            int temp = 0xFFFFF000;
            std::stringstream ss;
            ss << std::hex << temp;
            std::string hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            temp=numValue;
            std::stringstream ss2;
            ss2 << std::hex << temp;
            std::string data2 = "";
            hex_str = ss2.str();
            for(int i=0; i<8-hex_str.length(); i++){
                data2+="0";
            }
            data2+=hex_str;
            sequence = "reset\n0x002"+address+data+",write\nread\n0x003"+address+data2+",write\nread";
        }
        else if(parameters.size()>2&&(index>=16&&index<=31)){
            int numValue = std::stoi(parameters[2]);
            int temp = 0xF000FFFF;
            std::stringstream ss;
            ss << std::hex << temp;
            std::string hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            temp=numValue*std::pow(2,16);
            std::stringstream ss2;
            ss2 << std::hex << temp;
            std::string data2 = "";
            hex_str = ss2.str();
            for(int i=0; i<8-hex_str.length(); i++){
                data2+="0";
            }
            data2+=hex_str;
            sequence = "reset\n0x002"+address+data+",write\nread\n0x003"+address+data2+",write\nread";
        }
        else{
            sequence="";
            this->publishError("Wrong parameters");
        }
        return sequence;
    }
    else if(endpoint=="DATA_SELECT_TRG_MASK"){
        address="0000"+pmNumber+"E4";
        int index = -1;
        try{
            index=std::stoi(parameters[0]);
        }
        catch(exception& e){
            index=-1;    
        }
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            int num = std::stoi(parameters[0]);
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
        else{
            sequence="";
            this->publishError("Wrong parameters");
        }
        return sequence;
    }
    else if(endpoint=="MODES_STATUS"){
        address="0000"+pmNumber+"E8";
        int index = -1;
        try{
            index=std::stoi(parameters[0]);
        }
        catch(exception& e){
            index=-1;    
        }
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            int num = std::stoi(parameters[0]);
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
        else if(parameters.size()>2&&(index>=16&&index<=19)){
            int numValue = std::stoi(parameters[2]);
            int temp = 0xFFF0FFFF;
            std::stringstream ss;
            ss << std::hex << temp;
            std::string hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            temp=numValue*std::pow(2,16);
            std::stringstream ss2;
            ss2 << std::hex << temp;
            std::string data2 = "";
            hex_str = ss2.str();
            for(int i=0; i<8-hex_str.length(); i++){
                data2+="0";
            }
            data2+=hex_str;
            sequence = "reset\n0x002"+address+data+",write\nread\n0x003"+address+data2+",write\nread";
        }
        else if(parameters.size()>2&&(index>=20&&index<=23)){
            int numValue = std::stoi(parameters[2]);
            int temp = 0xFF0FFFFF;
            std::stringstream ss;
            ss << std::hex << temp;
            std::string hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            temp=numValue*std::pow(2,20);
            std::stringstream ss2;
            ss2 << std::hex << temp;
            std::string data2 = "";
            hex_str = ss2.str();
            for(int i=0; i<8-hex_str.length(); i++){
                data2+="0";
            }
            data2+=hex_str;
            sequence = "reset\n0x002"+address+data+",write\nread\n0x003"+address+data2+",write\nread";
        }
        else if(parameters.size()>2&&(index>=24&&index<=27)){
            int numValue = std::stoi(parameters[2]);
            int temp = 0xF0FFFFFF;
            std::stringstream ss;
            ss << std::hex << temp;
            std::string hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            temp=numValue*std::pow(2,24);
            std::stringstream ss2;
            ss2 << std::hex << temp;
            std::string data2 = "";
            hex_str = ss2.str();
            for(int i=0; i<8-hex_str.length(); i++){
                data2+="0";
            }
            data2+=hex_str;
            sequence = "reset\n0x002"+address+data+",write\nread\n0x003"+address+data2+",write\nread";
        }
        else if(parameters.size()>2&&(index>=28&&index<=31)){
            int numValue = std::stoi(parameters[2]);
            int temp = 0x0FFFFFFF;
            std::stringstream ss;
            ss << std::hex << temp;
            std::string hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            temp=numValue*std::pow(2,28);
            std::stringstream ss2;
            ss2 << std::hex << temp;
            std::string data2 = "";
            hex_str = ss2.str();
            for(int i=0; i<8-hex_str.length(); i++){
                data2+="0";
            }
            data2+=hex_str;
            sequence = "reset\n0x002"+address+data+",write\nread\n0x003"+address+data2+",write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="3"){
            int num = std::stoi(parameters[0]);
            std::string data="";
            std::stringstream ss;
            ss << std::hex << (1 << num);
            std::string hex_str = ss.str();

            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            sequence = "reset\n0x002"+address+"FFFFFFFF,write\nread\n0x003"+address+data+",write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="2"){
            int num = std::stoi(parameters[0]);
            int temp = 0xFFFFFFFF;
            temp-=(1 << num);
            std::stringstream ss;
            ss << std::hex << temp;
            std::string data = ss.str();
            sequence = "reset\n0x002"+address+data+",write\nread\n0x003"+address+"00000000,write\nread";
        }
        else{
            sequence="";
            this->publishError("Wrong parameters");
        }
        return sequence;
    }
    else if(endpoint=="CRU_ORBIT"){
        address="0000"+pmNumber+"E9";
        int index = -1;
        try{
            index=std::stoi(parameters[0]);
        }
        catch(exception& e){
            index=-1;    
        }
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            int num = std::stoi(parameters[0]);
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
        else{
            sequence="";
            this->publishError("Wrong parameters");
        }
        return sequence;
    }
    else if(endpoint=="CRU_BC"){
        address="0000"+pmNumber+"EA";
        int index = -1;
        try{
            index=std::stoi(parameters[0]);
        }
        catch(exception& e){
            index=-1;    
        }
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            int num = std::stoi(parameters[0]);
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
        else{
            sequence="";
            this->publishError("Wrong parameters");
        }
        return sequence;
    }
    else if(endpoint=="FIFO_COUNT"){
        address="0000"+pmNumber+"EB";
        int index = -1;
        try{
            index=std::stoi(parameters[0]);
        }
        catch(exception& e){
            index=-1;    
        }
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            int num = std::stoi(parameters[0]);
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
        else if(parameters.size()>2&&(index>=0&&index<=15)){
            
            int numValue = std::stoi(parameters[2]);
            int temp = 0xFFFF0000;
            std::stringstream ss;
            ss << std::hex << temp;
            std::string hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            temp=numValue;
            std::stringstream ss2;
            ss2 << std::hex << temp;
            std::string data2 = "";
            hex_str = ss2.str();
            for(int i=0; i<8-hex_str.length(); i++){
                data2+="0";
            }
            data2+=hex_str;
            sequence = "reset\n0x002"+address+data+",write\nread\n0x003"+address+data2+",write\nread";
        }
        else if(parameters.size()>2&&(index>=16&&index<=31)){
            int numValue = std::stoi(parameters[2]);
            int temp = 0x0000FFFF;
            std::stringstream ss;
            ss << std::hex << temp;
            std::string hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            temp=numValue*std::pow(2,16);
            std::stringstream ss2;
            ss2 << std::hex << temp;
            std::string data2 = "";
            hex_str = ss2.str();
            for(int i=0; i<8-hex_str.length(); i++){
                data2+="0";
            }
            data2+=hex_str;
            sequence = "reset\n0x002"+address+data+",write\nread\n0x003"+address+data2+",write\nread";
        }
        else{
            sequence="";
            this->publishError("Wrong parameters");
        }
        return sequence;
    }
    else if(endpoint=="SEL_FIRST_HIT_DROPPED_ORBIT"){
        address="0000"+pmNumber+"EC";
        int index = -1;
        try{
            index=std::stoi(parameters[0]);
        }
        catch(exception& e){
            index=-1;    
        }
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            int num = std::stoi(parameters[0]);
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
        else{
            sequence="";
            this->publishError("Wrong parameters");
        }
        return sequence;
    }
    else if(endpoint=="SEL_LAST_HIT_DROPPED_ORBIT"){
        address="0000"+pmNumber+"ED";
        int index = -1;
        try{
            index=std::stoi(parameters[0]);
        }
        catch(exception& e){
            index=-1;    
        }
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            int num = std::stoi(parameters[0]);
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
        else{
            sequence="";
            this->publishError("Wrong parameters");
        }
        return sequence;
    }
    else if(endpoint=="SEL_HITS_DROPPED"){
        address="0000"+pmNumber+"EE";
        int index = -1;
        try{
            index=std::stoi(parameters[0]);
        }
        catch(exception& e){
            index=-1;    
        }
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            int num = std::stoi(parameters[0]);
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
        else{
            sequence="";
            this->publishError("Wrong parameters");
        }
        return sequence;
    }
    else if(endpoint=="READOUT_RATE"){
        address="0000"+pmNumber+"EF";
        int index = -1;
        try{
            index=std::stoi(parameters[0]);
        }
        catch(exception& e){
            index=-1;    
        }
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            int num = std::stoi(parameters[0]);
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
        else{
            sequence="";
            this->publishError("Wrong parameters");
        }
        return sequence;
    }
    else if(endpoint=="CURRENT_ADDRESS"){
        address="0000"+pmNumber+"F5";
        int index = -1;
        try{
            index=std::stoi(parameters[0]);
        }
        catch(exception& e){
            index=-1;    
        }
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            int num = std::stoi(parameters[0]);
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
        else{
            sequence="";
            this->publishError("Wrong parameters");
        }
        return sequence;
    }
    else if(endpoint=="HISTOGRAM_DATA_READOUT"){
        address="0000"+pmNumber+"F5";
        int index = -1;
        try{
            index=std::stoi(parameters[0]);
        }
        catch(exception& e){
            index=-1;    
        }
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            int num = std::stoi(parameters[0]);
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
        else{
            sequence="";
            this->publishError("Wrong parameters");
        }
        return sequence;
    }
    else if(endpoint=="FPGA_TEMPERATURE"){
        address="0000"+pmNumber+"FC";
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else{
            sequence="";
            this->publishError("Readonly parameter");
        }
        return sequence;
    }
    else if(endpoint=="1VPOWER"){
        address="0000"+pmNumber+"FD";
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else{
            sequence="";
            this->publishError("Readonly parameter");
        }
        return sequence;
    }
    else if(endpoint=="18VPOWER"){
        address="0000"+pmNumber+"FE";
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else{
            sequence="";
            this->publishError("Readonly parameter");
        }
        return sequence;
    }
    else if(endpoint=="FPGA_TIMESTAMP"){
        address="0000"+pmNumber+"FF";
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
    string value;
    Print::PrintInfo(output);
    output.erase(remove(output.begin(), output.end(), '\n'), output.end());
    value = output.substr(output.size() - 8, output.size());
    finalValue = stoll(value, nullptr, 16);
    if(endpoint.rfind("TDC_12_PHASE_TUNING",0)==0){

        //return "number_1,number_2"

        std::string returnStr = "";
        finalValue = 0;
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
        finalValue = 0;
        finalValue += stoll(value.substr(6,2), nullptr, 16)*13*8/7;
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


    return to_string(finalValue);

}

