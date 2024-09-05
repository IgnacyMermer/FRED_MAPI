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


RefreshMapiPMCNTGroup::RefreshMapiPMCNTGroup(Fred* fred, std::vector<std::pair<std::string, std::string>> refreshServices){
    this->fred = fred;
    firstTime=true;

    sequence="reset";
    for (const auto& pair : refreshServices) {
        sequence+="\n0x000"+pair.first+"00000000,write\nread";
        services.push_back(pair.second);
        if(pair.second.find("SEL_LAST_HIT_DROPPED_ORBIT", 0)!=string::npos){
            servicesRates.push_back(std::string(pair.second).replace(pair.second.find("SEL_LAST_HIT_DROPPED_ORBIT", 0), 26, "GBT_WORDS_RATE"));
        }
        else if(pair.second.find("EVENTS_COUNT", 0)!=string::npos){
            servicesRates.push_back(std::string(pair.second).replace(pair.second.find("EVENTS_COUNT", 0), 12, "EVENTS_COUNT_RATE"));
        }
        else{
            servicesRates.push_back(std::string(pair.second).replace(pair.second.find("COUNT", 0), 5, "RATE"));
        }
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
    return "0";
}
