#include <iostream>
#include <string>
#include <algorithm>
#include <unistd.h>
#include "refresh_mapi_PM_cnt_group.h"
#include "Parser/utility.h"
#include "Alfred/print.h"
#include "TCM_values.h"
#include <cmath>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include "SWT_creator.h"


RefreshMapiPMCNTGroup::RefreshMapiPMCNTGroup(Fred* fred){
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
        Print::PrintInfo("error during creating sequence refresh counters PMs");
        Print::PrintError(e.what());
    }

    firstTime=true;
    int arraySize = prefixesPM.size();
    std::string serviceName="LAB/PM/";
    sequence="reset";

    fileName = "refresh_PMs_cnt.cfg";

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
                    if(key_value.second.get_value<std::string>()=="SEL_LAST_HIT_DROPPED_ORBIT"){
                        servicesRates.push_back(serviceName+"GBT_WORDS_RATE");
                    }
                    else if(key_value.second.get_value<std::string>()=="EVENTS_COUNT"){
                        servicesRates.push_back(serviceName+"EVENTS_COUNT_RATE");
                    }
                    else{
                        servicesRates.push_back(serviceName+"RATE"+key_value.second.get_value<std::string>().substr(5));
                    }
                    tcm.addresses[serviceName+key_value.second.get_value<std::string>()]=addressParameter+key_value.first.substr(key_value.first.length()-2);
                }
            }
        }
    }
    catch(exception& e){
        Print::PrintInfo("error during creating sequence refresh TCM");
        Print::PrintError(e.what());
    }
}

string RefreshMapiPMCNTGroup::processInputMessage(string input){
    if(input=="go"){
        return sequence;
    }
    noRpcRequest=true;
    this->publishError("wrong input parameter");
    return "";
}

string RefreshMapiPMCNTGroup::processOutputMessage(string output){
    vector< pair <string, string> > requests;
    std::string value;
    if(output!="failure"){
        try{
            output.erase(remove(output.begin(), output.end(), '\n'), output.end());
            output = output.substr(8);
            int maxCount=10000, count=0;
            bool firstIteration=true;
            while(output.length()>0&&count<maxCount){
                if(!firstIteration){
                    output=output.substr(1);
                }
                firstIteration=false;
                
                value = output.substr(13, 8);
                output = output.substr(21);
                
                uint32_t hexValue = stoll(value, nullptr, 16);
                float triggerRate = 0.0;

                if(firstTime){
                    auto now = std::chrono::high_resolution_clock::now();

                    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
                    
                    int milliseconds_since_epoch = static_cast<int>(duration.count());

                    int difference = milliseconds_since_epoch - tcm.temp.oldTimeTrigger5;
                    if(difference==0){
                        difference=1000;
                    }

                    triggerRate = abs((long long)(hexValue - tcm.temp.trigger5cnt))/difference*1000.0;

                    oldTimes.push_back(milliseconds_since_epoch);
                    
                    if(hexValue==0){
                        triggerRate=0;
                    }
                    
                    oldValuesRates.push_back(triggerRate);
                    requests.push_back(make_pair(servicesRates[count], "FRED,"+std::to_string(triggerRate)));

                    oldValues.push_back(hexValue);
                    requests.push_back(make_pair(services[count], "FRED,"+std::to_string(hexValue)));
                    
                }
                else if(oldValues[count]!=hexValue){
                    auto now = std::chrono::high_resolution_clock::now();

                    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
                    
                    int milliseconds_since_epoch = static_cast<int>(duration.count());

                    int difference = milliseconds_since_epoch - oldTimes[count];
                    if(difference==0){
                        difference=1000;
                    }

                    triggerRate = abs((long long)(hexValue - oldValues[count]))/difference*1000.0;

                    oldTimes[count] = milliseconds_since_epoch;

                    if(hexValue==0){
                        triggerRate=0;
                    }
                    
                    if(oldValuesRates[count]!=triggerRate){
                        oldValuesRates[count] = triggerRate;
                        requests.push_back(make_pair(servicesRates[count], "FRED,"+std::to_string(triggerRate)));
                    }
                    oldValues[count] = hexValue;
                    requests.push_back(make_pair(services[count], "FRED,"+std::to_string(hexValue)));
                    
                }
                count++;
            }
            firstTime=false;
            newMapiGroupRequest(requests);
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
