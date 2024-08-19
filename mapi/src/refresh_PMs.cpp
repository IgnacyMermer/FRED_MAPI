#include <algorithm>
#include <iostream>
#include <numeric>
#include <bitset>
#include <sstream>
#include <string>
#include "refresh_PMs.h"
#include "Alfred/print.h"
#include "Parser/utility.h"
#include "TCM_values.h"
#include <thread>
#include <chrono>
#include <cmath>
#include "../../core/include/dim/dic.hxx"
#include "../../core/include/FREDServer/Fred/Mapi/iterativemapi.h"

RefreshPMs::RefreshPMs() {
    finalValue = 0;
    firstTime=true;
    const std::string prefixesPM[20] = {"PMA0", "PMC0","PMA1", "PMC1","PMA2", "PMC2","PMA3", "PMC3","PMA4", "PMC4","PMA5", "PMC5","PMA6", "PMC6","PMA7", "PMC7","PMA8", "PMC8","PMA9", "PMC9"};
    const std::string addresses[20] = {"02", "16","02", "16","02", "16","02", "16","02", "16","02", "16","02", "16","02", "16","02", "16","02", "16"};
    int arraySize = sizeof(prefixesPM)/sizeof(string);
    for(int z=0; z<arraySize; z++){
        std::string serviceName="PM/"+prefixesPM[z]+"/";
        services.push_back(serviceName+"TRG_SETTINGS");
        const std::string prefixes[] = {"CHANNEL_SETTINGS", "ADC?_BASELINE", "ADC?_RANGE_CORR", "RAW_TDC_DATA", "ADC?_DISPERSION", "ADC?_MEAN", "CHANNELS_MASK", "CHANNEL_ADC_BASELINE", "HISTOGRAMMING_CONTROL", "STATUS_BITS"};
        for(int i=0;i<3;i++){
            for(int j=1; j<=12; j++){
                if(j<10){
                    if(prefixes[i][3]=='?'){
                        services.push_back(serviceName+prefixes[i].substr(0,3)+"0"+prefixes[i].substr(4)+"_"+std::to_string(j));
                        services.push_back(serviceName+prefixes[i].substr(0,3)+"1"+prefixes[i].substr(4)+"_"+std::to_string(j));
                    }
                    else{
                        services.push_back(serviceName+prefixes[i]+"_"+std::to_string(j));
                    }
                }
                else if(j==10){
                    if(prefixes[i][3]=='?'){
                        services.push_back(serviceName+prefixes[i].substr(0,3)+"0"+prefixes[i].substr(4)+"_A");
                        services.push_back(serviceName+prefixes[i].substr(0,3)+"1"+prefixes[i].substr(4)+"_A");
                    }
                    else{
                        services.push_back(serviceName+prefixes[i]+"_A");
                    }
                }
                else if(j==11){
                    if(prefixes[i][3]=='?'){
                        services.push_back(serviceName+prefixes[i].substr(0,3)+"0"+prefixes[i].substr(4)+"_B");
                        services.push_back(serviceName+prefixes[i].substr(0,3)+"1"+prefixes[i].substr(4)+"_B");
                    }
                    else{
                        services.push_back(serviceName+prefixes[i]+"_B");
                    }
                }
                else if(j==12){
                    if(prefixes[i][3]=='?'){
                        services.push_back(serviceName+prefixes[i].substr(0,3)+"0"+prefixes[i].substr(4)+"_C");
                        services.push_back(serviceName+prefixes[i].substr(0,3)+"1"+prefixes[i].substr(4)+"_C");
                    }
                    else{
                        services.push_back(serviceName+prefixes[i]+"_C");
                    }
                }
            }
        }
        services.push_back(serviceName+"TRG_CHARGE_LEVELS");
        services.push_back(serviceName+"TDC_12_PHASE_TUNING");
        services.push_back(serviceName+"TDC_3_PHASE_TUNING");
        for(int i=3;i<6;i++){
            for(int j=1; j<=12; j++){
                if(j<10){
                    if(prefixes[i][3]=='?'){
                        services.push_back(serviceName+prefixes[i].substr(0,3)+"0"+prefixes[i].substr(4)+"_"+std::to_string(j));
                        services.push_back(serviceName+prefixes[i].substr(0,3)+"1"+prefixes[i].substr(4)+"_"+std::to_string(j));
                    }
                    else{
                        services.push_back(serviceName+prefixes[i]+"_"+std::to_string(j));
                    }
                }
                else if(j==10){
                    if(prefixes[i][3]=='?'){
                        services.push_back(serviceName+prefixes[i].substr(0,3)+"0"+prefixes[i].substr(4)+"_A");
                        services.push_back(serviceName+prefixes[i].substr(0,3)+"1"+prefixes[i].substr(4)+"_A");
                    }
                    else{
                        services.push_back(serviceName+prefixes[i]+"_A");
                    }
                }
                else if(j==11){
                    if(prefixes[i][3]=='?'){
                        services.push_back(serviceName+prefixes[i].substr(0,3)+"0"+prefixes[i].substr(4)+"_B");
                        services.push_back(serviceName+prefixes[i].substr(0,3)+"1"+prefixes[i].substr(4)+"_B");
                    }
                    else{
                        services.push_back(serviceName+prefixes[i]+"_B");
                    }
                }
                else if(j==12){
                    if(prefixes[i][3]=='?'){
                        services.push_back(serviceName+prefixes[i].substr(0,3)+"0"+prefixes[i].substr(4)+"_C");
                        services.push_back(serviceName+prefixes[i].substr(0,3)+"1"+prefixes[i].substr(4)+"_C");
                    }
                    else{
                        services.push_back(serviceName+prefixes[i]+"_C");
                    }
                }
            }
        }
        services.push_back(serviceName+"CHANNELS_MASK");
        services.push_back(serviceName+"CHANNEL_ADC_BASELINE");
        services.push_back(serviceName+"HISTOGRAMMING_CONTROL");
        services.push_back(serviceName+"STATUS_BITS");
        for(int i=1;i<=12;i++){
            std::string iNumber = std::to_string(i);
            if(i==10){
                iNumber="A";
            }
            else if(i==11){
                iNumber="B";
            }
            else if(i==12){
                iNumber="C";
            }
            services.push_back(serviceName+"CFD_THRESHOLD_"+iNumber);
            services.push_back(serviceName+"CFD_ZERO_"+iNumber);
            services.push_back(serviceName+"ADC_ZERO_"+iNumber);
            services.push_back(serviceName+"ADC_DELAY_"+iNumber);
        }
        for(int i=1;i<=12;i++){
            std::string iNumber = std::to_string(i);
            if(i==10){
                iNumber="A";
            }
            else if(i==11){
                iNumber="B";
            }
            else if(i==12){
                iNumber="C";
            }
            services.push_back(serviceName+"THRESHOLD_CALIBRATION_"+iNumber);
        }
        services.push_back(serviceName+"BOARD_TEMPERATURE");
        services.push_back(serviceName+"BOARD_ID");
        services.push_back(serviceName+"LAST_RESTART_REASON");
        for(int i=1;i<=12;i++){
            std::string iNumber = std::to_string(i);
            if(i==10){
                iNumber="A";
            }
            else if(i==11){
                iNumber="B";
            }
            else if(i==12){
                iNumber="C";
            }
            services.push_back(serviceName+"COUNT_CFD_HITS_"+iNumber);
            services.push_back(serviceName+"COUNT_TRG_HITS_"+iNumber);
        }
        services.push_back(serviceName+"MODE_SETTINGS");
        services.push_back(serviceName+"TRIGGER_RESPOND_MASK");
        services.push_back(serviceName+"DATA_BUNCH_PATTERN");
        services.push_back(serviceName+"TRIGGER_SINGLE_VALUE");
        services.push_back(serviceName+"TRIGGER_CONT_PATTERN_MSB");
        services.push_back(serviceName+"TRIGGER_CONT_PATTERN_LSB");
        services.push_back(serviceName+"TRIGGER_CONT_VALUE");
        services.push_back(serviceName+"GENERATORS_BUNCH_FREQ");
        services.push_back(serviceName+"GENERATORS_FREQ_OFFSET");
        services.push_back(serviceName+"RDH_FIELDS1");
        services.push_back(serviceName+"RDH_FIELDS2");
        services.push_back(serviceName+"DELAYS");
        services.push_back(serviceName+"DATA_SELECT_TRG_MASK");
        services.push_back(serviceName+"MODES_STATUS");
        services.push_back(serviceName+"CRU_BC");
        services.push_back(serviceName+"CRU_ORBIT");
        services.push_back(serviceName+"FIFO_COUNT");
        services.push_back(serviceName+"SEL_FIRST_HIT_DROPPED_ORBIT");
        services.push_back(serviceName+"SEL_LAST_HIT_DROPPED_ORBIT");
        services.push_back(serviceName+"SEL_HITS_DROPPED");
        services.push_back(serviceName+"READOUT_RATE");
        services.push_back(serviceName+"CURRENT_ADDRESS");
        services.push_back(serviceName+"HISTOGRAM_DATA_READOUT");
        services.push_back(serviceName+"ATX_TIMESTAMP");
        services.push_back(serviceName+"FW_UPGRADE_COMM");
        services.push_back(serviceName+"FW_UPGRADE_DATA");
        services.push_back(serviceName+"FW_UPGRADE_END");
        services.push_back(serviceName+"FW_UPGRADE_STATUS");
        services.push_back(serviceName+"FPGA_TEMPERATURE");
        services.push_back(serviceName+"1VPOWER");
        services.push_back(serviceName+"18VPOWER");
        services.push_back(serviceName+"FPGA_TIMESTAMP");
    }
    sequence="reset";
    int tempCount = 0;
    for(int z=0; z<arraySize; z++){
        for(int i=0;i<15;i++){
            std::string iNumber = std::to_string(i);
            if(i==10){
                iNumber="A";
            }
            else if(i==11){
                iNumber="B";
            }
            else if(i==12){
                iNumber="C";
            }
            else if(i==13){
                iNumber="D";
            }
            else if(i==14){
                iNumber="E";
            }
            else if(i==15){
                iNumber="F";
            }
            for(int j=0; j<16; j++){
                if(!((i==11&&j==15)||(i==14&&j>=5&&j<=7))){
                    tempCount++;
                    sequence+="\n0x000";
                    std::string address="0000"+addresses[z];
                    if(j<10){
                        address+=iNumber+std::to_string(j);
                    }
                    else if(j==10){
                        address+=iNumber+"A";
                    }
                    else if(j==11){
                        address+=iNumber+"B";
                    }
                    else if(j==12){
                        address+=iNumber+"C";
                    }
                    else if(j==13){
                        address+=iNumber+"D";
                    }
                    else if(j==14){
                        address+=iNumber+"E";
                    }
                    else if(j==15){
                        address+=iNumber+"F";
                    }
                    sequence+=address;
                    sequence+="00000000,write\nread";
                }
            }
        }
        std::string iNumber = "F";
        for(int j=5; j<16; j++){
            tempCount++;
            sequence+="\n0x000";
            std::string address="0000"+addresses[z];
            if(j<10){
                address+=iNumber+std::to_string(j);
            }
            else if(j==10){
                address+=iNumber+"A";
            }
            else if(j==11){
                address+=iNumber+"B";
            }
            else if(j==12){
                address+=iNumber+"C";
            }
            else if(j==13){
                address+=iNumber+"D";
            }
            else if(j==14){
                address+=iNumber+"E";
            }
            else if(j==15){
                address+=iNumber+"F";
            }
            sequence+=address;
            sequence+="00000000,write\nread";
        }
    }
    Print::PrintInfo("PM services counts:");
    Print::PrintInfo(std::to_string(services.size()));
}

string RefreshPMs::processInputMessage(string input) {
    //if(input=="go"){
    return sequence;
    //}
    return "";
}

string RefreshPMs::processOutputMessage(string output) {
    if(output!="failure"){
        string value;
        output.erase(remove(output.begin(), output.end(), '\n'), output.end());
        output = output.substr(8);
        int maxCount=20000, count=0;
        bool firstIteration=true;
        while(output.length()>0&&count<maxCount&&count<services.size()){
            if(!firstIteration){
                output=output.substr(1);
            }
            firstIteration=false;
            
            value = output.substr(13, 8);
            output = output.substr(21);
            long long hexValue = stoll(value, nullptr, 16);
            if(firstTime){
                oldValues.push_back(hexValue);
                if(services[count].find("TDC_12_PHASE_TUNING") != std::string::npos){

                    //return "number_1,number_2"

                    std::string returnStr = "";
                    long long finalValue = 0;
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
                    updateTopicAnswer(services[count], returnStr);
                }
                else if(services[count].find("TDC_3_PHASE_TUNING") != std::string::npos){
                    long long finalValue = 0;
                    finalValue += stoll(value.substr(6,2), nullptr, 16)*13*8/7;
                    updateTopicAnswer(services[count], std::to_string(finalValue));
                }
                else if(services[count].find("RAW_TDC_DATA") != std::string::npos){
                    std::stringstream ss;
                    ss << std::hex << hexValue;
                    updateTopicAnswer(services[count], "0x"+ss.str());
                }
                else if(services[count].find("ADC0_DISPERSION") != std::string::npos||services[count].find("ADC1_DISPERSION") != std::string::npos){
                    updateTopicAnswer(services[count], std::to_string(std::sqrt(hexValue)));
                }
                else if(services[count].find("CFD_THRESHOLD") != std::string::npos || services[count].find("CFD_ZERO") != std::string::npos || services[count].find("ADC_ZERO") != std::string::npos || services[count].find("ADC_DELAY") != std::string::npos){
                    //do sprawdzenia
                    std::string returnStr = "";
                    unsigned int x = hexValue;
                    if (hexValue > 1000) {
                        returnStr += std::to_string((static_cast<int>(static_cast<signed char>(x))));
                    }
                    else {
                        returnStr += std::to_string((static_cast<int>(x)));
                    }
                    updateTopicAnswer(services[count], returnStr);
                }
                else if(services[count].find("FPGA_TEMPERATURE") != std::string::npos){
                    updateTopicAnswer(services[count], std::to_string(hexValue * 503.975 / 65536 - 273.15));
                }
                else if(services[count].find("1VPOWER") != std::string::npos){
                    updateTopicAnswer(services[count], std::to_string(hexValue * 3 / 65536.0));
                }
                else if(services[count].find("18VPOWER") != std::string::npos){
                    updateTopicAnswer(services[count], std::to_string(hexValue * 3 / 65536.0));
                }
                else if(services[count].find("BOARD_TEMPERATURE") != std::string::npos){
                    updateTopicAnswer(services[count], std::to_string(hexValue / 10.0));
                }
                else{
                    updateTopicAnswer(services[count], std::to_string(hexValue));
                }
            }
            else if(oldValues[count]!=hexValue){
                oldValues[count]=hexValue;
                if(services[count].find("TDC_12_PHASE_TUNING") != std::string::npos){

                    //return "number_1,number_2"

                    std::string returnStr = "";
                    long long finalValue = 0;
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
                    updateTopicAnswer(services[count], returnStr);
                }
                else if(services[count].find("TDC_3_PHASE_TUNING") != std::string::npos){
                    long long finalValue = 0;
                    finalValue += stoll(value.substr(6,2), nullptr, 16)*13*8/7;
                    updateTopicAnswer(services[count], std::to_string(finalValue));
                }
                else if(services[count].find("RAW_TDC_DATA") != std::string::npos){
                    std::stringstream ss;
                    ss << std::hex << hexValue;
                    updateTopicAnswer(services[count], "0x"+ss.str());
                }
                else if(services[count].find("ADC0_DISPERSION") != std::string::npos||services[count].find("ADC1_DISPERSION") != std::string::npos){
                    updateTopicAnswer(services[count], std::to_string(std::sqrt(hexValue)));
                }
                else if(services[count].find("CFD_THRESHOLD") != std::string::npos || services[count].find("CFD_ZERO") != std::string::npos || services[count].find("ADC_ZERO") != std::string::npos || services[count].find("ADC_DELAY") != std::string::npos){
                    //do sprawdzenia
                    std::string returnStr = "";
                    unsigned int x = hexValue;
                    if (hexValue > 1000) {
                        returnStr += std::to_string((static_cast<int>(static_cast<signed char>(x))));
                    }
                    else {
                        returnStr += std::to_string((static_cast<int>(x)));
                    }
                    updateTopicAnswer(services[count], returnStr);
                }
                else if(services[count].find("FPGA_TEMPERATURE") != std::string::npos){
                    updateTopicAnswer(services[count], std::to_string(hexValue * 503.975 / 65536 - 273.15));
                }
                else if(services[count].find("1VPOWER") != std::string::npos){
                    updateTopicAnswer(services[count], std::to_string(hexValue * 3 / 65536.0));
                }
                else if(services[count].find("18VPOWER") != std::string::npos){
                    updateTopicAnswer(services[count], std::to_string(hexValue * 3 / 65536.0));
                }
                else if(services[count].find("BOARD_TEMPERATURE") != std::string::npos){
                    updateTopicAnswer(services[count], std::to_string(hexValue / 10.0));
                }
                else{
                    updateTopicAnswer(services[count], std::to_string(hexValue));
                }
            }
            count++;
        }
        firstTime=false;
        return "OK";
    }
    else{
        this->publishError("Failure");
        return "failure";
    }
    
}