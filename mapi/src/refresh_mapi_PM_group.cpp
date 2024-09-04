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


RefreshMapiPMGroup::RefreshMapiPMGroup(Fred* fred, std::vector<std::pair<std::string, std::string>> refreshServices):refreshServices(refreshServices){
    this->fred = fred;
    firstTime=true;
    sequence="reset";
    for (const auto& pair : refreshServices) {
        sequence+="\n0x000"+pair.first+"00000000,write\nread";   
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
            while(output.length()>0&&count<maxCount&&count<refreshServices.size()){
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
                    if(refreshServices[count].second.find("TDC_12_PHASE_TUNING")!=string::npos){

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
                    else if(refreshServices[count].second.find("TDC_3_PHASE_TUNING")!=string::npos){
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
                    else if(refreshServices[count].second.find("RAW_TDC_DATA")!=string::npos){
                        std::stringstream ss;
                        ss << std::hex << hexValue;
                        returnValue = "0x"+ss.str();
                    }
                    else if(refreshServices[count].second.find("ADC0_DISPERSION")!=string::npos||refreshServices[count].second.find("ADC1_DISPERSION")!=string::npos){
                        returnValue = std::to_string(std::sqrt(hexValue));
                    }
                    else if(refreshServices[count].second.find("TEMPERATURE")!=string::npos){
                        returnValue = std::to_string(hexValue/10.0);
                    }
                    else if(refreshServices[count].second.find("FPGA_TEMP")!=string::npos){
                        returnValue = std::to_string(hexValue * 503.975 / 65536 - 273.15);
                    }
                    else if(refreshServices[count].second.find("1VPOWER")!=string::npos){
                        returnValue = std::to_string(hexValue * 3 / 65536.0);
                    }
                    else if(refreshServices[count].second.find("18VPOWER")!=string::npos){
                        returnValue = std::to_string(hexValue * 3 / 65536.0);
                    }

                    if (hexValue > 50000&&(refreshServices[count].second.find("CHANNEL_SETTINGS")!=string::npos||refreshServices[count].second.find("ADC0_BASELINE")!=string::npos
                    ||refreshServices[count].second.find("ADC1_BASELINE")!=string::npos||refreshServices[count].second.find("ADC0_MEAN")!=string::npos||refreshServices[count].second.find("ADC1_MEAN")!=string::npos
                    ||refreshServices[count].second.find("ADC1_MEAN")!=string::npos||refreshServices[count].second.find("CFD_ZERO")!=string::npos||refreshServices[count].second.find("ADC_ZERO")!=string::npos
                    ||refreshServices[count].second.find("TEMPERATURE")!=string::npos)) {
                        int16_t x = stoi(value, nullptr, 16);
                        hexValue=-(~x+1);
                        returnValue = std::to_string(hexValue);
                    }
                    requests.push_back(make_pair(refreshServices[count].second, "FRED,"+returnValue));
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