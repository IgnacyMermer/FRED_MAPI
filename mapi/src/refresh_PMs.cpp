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
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include "SWT_creator.h"


RefreshPMs::RefreshPMs() {

    /*
        Refresh PMs initialize vector of services names and create sequence string to be sent to ALF to get values for each service 
    */

    std::string fileName = "detector_type.cfg";
    boost::property_tree::ptree tree;


    if (!boost::filesystem::exists(fileName)) {
        fileName = "./configuration/" + fileName;
    }

    std::vector<std::string> prefixesPM = {"PMA0", "PMC0"}, addresses = {"02", "16"};

    try{
        boost::property_tree::ini_parser::read_ini(fileName, tree);

        /*const std::vector<std::string> prefixesMaxTest = {"PMA0", "PMC0","PMA1", "PMC1","PMA2", "PMC2","PMA3", "PMC3","PMA4", "PMC4","PMA5", "PMC5","PMA6", "PMC6","PMA7", "PMC7","PMA8", "PMC8","PMA9", "PMC9"};
        const std::vector<std::string> addressesMaxTest = {"02", "16","02", "16","02", "16","02", "16","02", "16","02", "16","02", "16","02", "16","02", "16","02", "16"};
        const std::vector<std::string> prefixesFT0 = {"PMA0", "PMC0","PMA1", "PMC1","PMA2", "PMC2","PMA3", "PMC3","PMA4", "PMC4","PMA5", "PMC5","PMA6", "PMC6","PMA7", "PMC7","PMA8", "PMC8"};
        const std::vector<std::string> addressesFT0 = {"02", "16", "04", "18", "06", "1A", "08", "1C", "0A", "1E", "0C", "20", "0E", "22", "10", "24", "12", "26"};
          */

        for (const auto& section : tree) {
            if(section.first=="CONFIG"){
                for (const auto& key_value : section.second) {
                    if(key_value.first=="TYPE"){
                        
                    }
                    else if(key_value.first=="PM_NAMES"){
                        vector<string> names = Utility::splitString(key_value.second.get_value<std::string>(), ",");
                        prefixesPM.clear();
                        for(auto name : names){
                            prefixesPM.push_back(name);
                        }
                    }
                    else if(key_value.first=="PM_ADDRESSES"){
                        vector<string> names = Utility::splitString(key_value.second.get_value<std::string>(), ",");
                        addresses.clear();
                        for(auto name : names){
                            addresses.push_back(name);
                        }
                    }
                }
            }
        }
    }
    catch(exception& e){
        Print::PrintInfo("error during creating sequence refresh TCM");
        Print::PrintError(e.what());
    }

    firstTime=true;
    int arraySize = prefixesPM.size();
    std::string serviceName="PM/";
    sequence="reset";

    fileName = "refresh_PMs.cfg";

    if (!boost::filesystem::exists(fileName)) {
        fileName = "./configuration/" + fileName;
    }

    try{
        boost::property_tree::ini_parser::read_ini(fileName, tree);
        
        for (const auto& section : tree) {
            if(section.first=="PMA0"||section.first=="PMC0"){
                serviceName="PM/"+section.first+"/";
                std::string addressParameter;
                if(section.first=="PMA0"){
                    addressParameter = addresses[0];
                }
                else if(section.first=="PMC0"){
                    addressParameter = addresses[1];
                }
                for (const auto& key_value : section.second) {
                    sequence+="\n0x0000000"+addressParameter+key_value.first.substr(key_value.first.length()-2)+"00000000,write\nread";
                    services.push_back(serviceName+key_value.second.get_value<std::string>());
                    //tcm.addresses[serviceName+key_value.second.get_value<std::string>()]=addressParameter+key_value.first.substr(key_value.first.length()-2);
                }
            }
        }
    }
    catch(exception& e){
        Print::PrintInfo("error during creating sequence refresh TCM");
        Print::PrintError(e.what());
    }
}

string RefreshPMs::processInputMessage(string input) {
    if(input=="go"){
        return sequence;
    }
    noRpcRequest=true;
    this->publishError("wrong input parameter");
    return "";
}

string RefreshPMs::processOutputMessage(string output) {

    /*
        Compare values with data from previous iteration saved in vector, if parameter value has changed service is update with updateTopicAnswer function,
        if not that parameter is skipped in that iteration
    */

    if(output!="failure"){
        try{
            string value;
            output.erase(remove(output.begin(), output.end(), '\n'), output.end());
            output = output.substr(8);
            int maxCount=20000, count=0;
            bool firstIteration=true;
            bool updateService = false;
            while(output.length()>0&&count<maxCount&&count<services.size()){
                if(!firstIteration){
                    output=output.substr(1);
                }
                firstIteration=false;
                
                value = output.substr(13, 8);
                output = output.substr(21);
                hexValue = stoll(value, nullptr, 16);
                if (hexValue > 50000&&(services[count].find("CHANNEL_SETTINGS",0)!= std::string::npos||services[count].find("ADC0_BASELINE")!= std::string::npos
                    ||services[count].find("ADC1_BASELINE")!= std::string::npos||services[count].find("ADC0_MEAN")!= std::string::npos||services[count].find("ADC1_MEAN")!= std::string::npos
                    ||services[count].find("ADC1_MEAN")!= std::string::npos||services[count].find("CFD_ZERO")!= std::string::npos||services[count].find("ADC_ZERO")!= std::string::npos
                    ||services[count].find("TEMPERATURE")!= std::string::npos)) {
                    int16_t x = stoi(value, nullptr, 16);
                    hexValue=-(~x+1);
                }

                updateService=false;
                
                if(firstTime){
                    oldValues.push_back(hexValue);
                    updateService=true;
                }
                else if(oldValues[count]!=hexValue){
                    oldValues[count]=hexValue;
                    updateService=true;   
                }

                if(updateService){
                    if(services[count].find("TDC_12_PHASE_TUNING") != std::string::npos){

                        //return "number_1,number_2"

                        std::string returnStr = "";
                        unsigned int x;
                        std::stringstream ss;
                        ss << std::hex << value.substr(6,2);
                        ss >> x;

                        if (x > 127) {
                            returnStr += std::to_string((static_cast<int>(static_cast<signed char>(x)))*13.*8/7);
                        }
                        else {
                            returnStr += std::to_string((static_cast<int>(x))*13.*8/7);
                        }
                        returnStr+=",";
                        ss.str(std::string());
                        ss.clear();
                        ss << std::hex << value.substr(4,2);
                        ss >> x;

                        if (x > 127) {
                            returnStr += std::to_string((static_cast<int>(static_cast<signed char>(x)))*13.*8/7);
                        }
                        else {
                            returnStr += std::to_string((static_cast<int>(x))*13.*8/7);
                        }
                        updateTopicAnswer(services[count], returnStr);
                    }
                    else if(services[count].find("TDC_3_PHASE_TUNING") != std::string::npos){
                        updateTopicAnswer(services[count], std::to_string(stoi(value.substr(6,2), nullptr, 16)*13.*8/7));
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
                    else if(services[count].find("FPGA_TEMP") != std::string::npos){
                        updateTopicAnswer(services[count], std::to_string(hexValue * 503.975 / 65536 - 273.15));
                    }
                    else if(services[count].find("1VPOWER") != std::string::npos){
                        updateTopicAnswer(services[count], std::to_string(hexValue * 3 / 65536.0));
                    }
                    else if(services[count].find("18VPOWER") != std::string::npos){
                        updateTopicAnswer(services[count], std::to_string(hexValue * 3 / 65536.0));
                    }
                    else if(services[count].find("TEMPERATURE") != std::string::npos){
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
        catch(exception& e){
            Print::PrintError("error occured");
            Print::PrintError(e.what());
            this->publishError("Error");
            return "error";
        }
    }
    else{
        this->publishError("Failure");
        return "failure";
    }
    
}