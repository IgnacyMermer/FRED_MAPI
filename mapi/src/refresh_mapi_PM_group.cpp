#include <iostream>
#include <string>
#include <algorithm>
#include <unistd.h>
#include "refresh_mapi_PM_group.h"
#include "Parser/utility.h"
#include "Alfred/print.h"
#include "TCM_values.h"
#include <cmath>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include "SWT_creator.h"


RefreshMapiPMGroup::RefreshMapiPMGroup(Fred* fred){
    this->fred = fred;
    firstTime=true;
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
    std::string serviceName="LAB/PM/";
    sequence="reset";

    fileName = "refresh_PMs.cfg";

    if (!boost::filesystem::exists(fileName)) {
        fileName = "./configuration/" + fileName;
    }

    try{
        boost::property_tree::ini_parser::read_ini(fileName, tree);
        
        for (const auto& section : tree) {
            if(section.first=="PMA0"||section.first=="PMC0"){
                serviceName="LAB/PM/"+section.first+"/";
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

string RefreshMapiPMGroup::processInputMessage(string input){
    if(input=="go"){
        return sequence;
    }
    noRpcRequest=true;
    this->publishError("wrong input parameter");
    return "";
}

string RefreshMapiPMGroup::processOutputMessage(string output){
    vector< pair <string, string> > requests;
    std::string value;
    if(output!="failure"){
        try{
            string value;
            output.erase(remove(output.begin(), output.end(), '\n'), output.end());
            output = output.substr(8);
            int maxCount=20000, count=0;
            bool firstIteration = true;
            bool updateService = false;
            while(output.length()>0&&count<maxCount&&count<services.size()){
                if(!firstIteration){
                    output=output.substr(1);
                }
                firstIteration=false;
                
                value = output.substr(13, 8);
                output = output.substr(21);
                hexValue = stoll(value, nullptr, 16);

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
                    std::string returnValue = std::to_string(hexValue);
                    if(services[count].find("TDC_12_PHASE_TUNING")!=string::npos){

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
                        returnValue = returnStr;
                    }
                    else if(services[count].find("TDC_3_PHASE_TUNING")!=string::npos){
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
                        returnValue = returnStr;
                    }
                    else if(services[count].find("RAW_TDC_DATA")!=string::npos){
                        std::stringstream ss;
                        ss << std::hex << hexValue;
                        returnValue = "0x"+ss.str();
                    }
                    else if(services[count].find("ADC0_DISPERSION")!=string::npos||services[count].find("ADC1_DISPERSION")!=string::npos){
                        returnValue = std::to_string(std::sqrt(hexValue));
                    }
                    else if(services[count].find("TEMPERATURE")!=string::npos){
                        returnValue = std::to_string(hexValue/10.0);
                    }
                    else if(services[count].find("FPGA_TEMP")!=string::npos){
                        returnValue = std::to_string(hexValue * 503.975 / 65536 - 273.15);
                    }
                    else if(services[count].find("1VPOWER")!=string::npos){
                        returnValue = std::to_string(hexValue * 3 / 65536.0);
                    }
                    else if(services[count].find("18VPOWER")!=string::npos){
                        returnValue = std::to_string(hexValue * 3 / 65536.0);
                    }

                    if (hexValue > 50000&&(services[count].find("CHANNEL_SETTINGS")!=string::npos||services[count].find("ADC0_BASELINE")!=string::npos
                    ||services[count].find("ADC1_BASELINE")!=string::npos||services[count].find("ADC0_MEAN")!=string::npos||services[count].find("ADC1_MEAN")!=string::npos
                    ||services[count].find("ADC1_MEAN")!=string::npos||services[count].find("CFD_ZERO")!=string::npos||services[count].find("ADC_ZERO")!=string::npos
                    ||services[count].find("TEMPERATURE")!=string::npos)) {
                        int16_t x = stoi(value, nullptr, 16);
                        hexValue=-(~x+1);
                        returnValue = std::to_string(hexValue);
                    }
                    requests.push_back(make_pair(services[count], "FRED,"+returnValue));
                }

                count++;
            }
            firstTime=false;
            newMapiGroupRequest(requests);
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