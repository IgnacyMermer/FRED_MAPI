#include <iostream>
#include <string>
#include <algorithm>
#include <unistd.h>
#include "RefreshMapiCntGroup.h"
#include "Parser/utility.h"
#include "Alfred/print.h"
#include "tcmValues.h"
#include <cmath>
#include "swtCreator.h"


RefreshMapiCNTGroup::RefreshMapiCNTGroup(Fred* fred, std::vector<std::pair<std::string, std::string>> refreshServices){
    this->fred = fred;
    firstTime=true;

    sequence="reset";
    
    for (const auto& pair : refreshServices) {
        sequence+="\n0x000"+pair.first+"00000000,write\nread";
        services.push_back(pair.second);
        servicesRates.push_back(std::string(pair.second).replace(pair.second.find("COUNT", 0), 5, "RATE"));
    }
}

string RefreshMapiCNTGroup::processInputMessage(string input){
    if(input=="go"){
        return sequence;
    }
    noRpcRequest=true;
    this->publishError("wrong input parameter");
    return "";
}

string RefreshMapiCNTGroup::processOutputMessage(string output){
    vector< pair <string, string> > requests;
    std::string value;
    if(output!="failure"){
        try{
            output.erase(remove(output.begin(), output.end(), '\n'), output.end());
            output = output.substr(8);
            uint16_t count=0;
            uint32_t hexValue, difference, millisecondsSinceEpoch;
            float triggerRate;
            bool firstIteration=true;
            while(output.length()>0){
                if(!firstIteration){
                    output=output.substr(1);
                }
                firstIteration=false;
                
                value = output.substr(13, 8);
                output = output.substr(21);
                
                hexValue = stoll(value, nullptr, 16);
                triggerRate = 0.0;

                if(firstTime||oldValues[count]!=hexValue){
                    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>((std::chrono::high_resolution_clock::now()).time_since_epoch());
                    
                    millisecondsSinceEpoch = static_cast<int>(duration.count());

                    if(firstTime){
                        triggerRate = hexValue;

                        oldTimes.push_back(millisecondsSinceEpoch);

                        if(hexValue==0){
                            triggerRate=0;
                        }
                        oldValuesRates.push_back(triggerRate);
                        requests.push_back(make_pair(servicesRates[count], "FRED,"+std::to_string(triggerRate)));

                        oldValues.push_back(hexValue);
                    }
                    else{
                        
                        difference = millisecondsSinceEpoch - oldTimes[count];
                        if(difference==0){
                            difference=1000;
                        }
                        triggerRate = abs((long long)(hexValue - oldValues[count]))*1000.0/difference;

                        oldTimes[count] = millisecondsSinceEpoch;

                        if(hexValue==0){
                            triggerRate=0;
                        }

                        if(oldValuesRates[count]!=triggerRate){
                            oldValuesRates[count] = triggerRate;
                            requests.push_back(make_pair(servicesRates[count], "FRED,"+std::to_string(triggerRate)));
                        }
                        
                        oldValues[count] = hexValue;
                    
                    }
                    requests.push_back(make_pair(services[count], "FRED,"+std::to_string(hexValue)));
                }
                else if(oldValuesRates[count]!=0){
                    requests.push_back(make_pair(servicesRates[count], "FRED,0"));
                    oldValuesRates[count]=0;
                }
                count++;
            }
            firstTime=false;
            newMapiGroupRequest(requests);
        }
        catch(exception& e){
            Print::PrintError("error in refresh counters TCM");
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
