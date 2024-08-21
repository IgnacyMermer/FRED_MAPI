#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include "refresh_PM_counters.h"
#include "Alfred/print.h"
#include "Parser/utility.h"
#include "TCM_values.h"
#include <thread>
#include <chrono>
#include "../../core/include/dim/dic.hxx"
#include "../../core/include/FREDServer/Fred/Mapi/iterativemapi.h"
#include <stdlib.h>


RefreshPMCounters::RefreshPMCounters() {
    finalValue = 0;
    firstTime=true;
    sequence = "reset";
    const std::string prefixesPM[2] = {"PMA0", "PMC0"};
    const std::string addresses[2] = {"02", "16"};
    //const std::string prefixesPM[20] = {"PMA0", "PMC0","PMA1", "PMC1","PMA2", "PMC2","PMA3", "PMC3","PMA4", "PMC4","PMA5", "PMC5","PMA6", "PMC6","PMA7", "PMC7","PMA8", "PMC8","PMA9", "PMC9"};
    //const std::string addresses[20] = {"02", "16","02", "16","02", "16","02", "16","02", "16","02", "16","02", "16","02", "16","02", "16","02", "16"};
    int arraySize = sizeof(prefixesPM)/sizeof(string);
    for(int z=0; z<arraySize; z++){
        for(int i=0; i<=15; i++){
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
            sequence+="\n0x0000000"+addresses[z]+"C"+iNumber+"00000000,write\nread";
        }
        for(int i=0; i<=7; i++){
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
            sequence+="\n0x0000000"+addresses[z]+"D"+iNumber+"00000000,write\nread";
        }
    }
    for(int z=0; z<arraySize; z++){
        std::string serviceName="PM/"+prefixesPM[z]+"/";
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
            servicesRates.push_back(serviceName+"RATE_CFD_HITS_"+iNumber);
            services.push_back(serviceName+"COUNT_TRG_HITS_"+iNumber);
            servicesRates.push_back(serviceName+"RATE_TRG_HITS_"+iNumber);
        }
    }

}

string RefreshPMCounters::processInputMessage(string input) {
    if(input=="go"){
        return sequence;
    }
    return "";
}

string RefreshPMCounters::processOutputMessage(string output) {
    string value;
    if(output!="failure"){
        try {
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
                
                long long hexValue = stoll(value, nullptr, 16);
                float triggerRate = 0.0;

                if(firstTime){
                    auto now = std::chrono::high_resolution_clock::now();

                    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
                    
                    int milliseconds_since_epoch = static_cast<int>(duration.count());

                    int difference = milliseconds_since_epoch - tcm.temp.oldTimeTrigger5;
                    if(difference==0){
                        difference=1000;
                    }

                    triggerRate = abs(hexValue - tcm.temp.trigger5cnt)/difference*1000.0;

                    oldTimes.push_back(milliseconds_since_epoch);
                    
                    if(hexValue==0){
                        triggerRate=0;
                    }
                    
                    oldValuesRates.push_back(triggerRate);
                    updateTopicAnswer(servicesRates[count], std::to_string(triggerRate));

                    oldValues.push_back(hexValue);
                    updateTopicAnswer(services[count], std::to_string(hexValue));
                    
                }
                else if(oldValues[count]!=hexValue||firstTime){
                    auto now = std::chrono::high_resolution_clock::now();

                    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
                    
                    int milliseconds_since_epoch = static_cast<int>(duration.count());

                    int difference = milliseconds_since_epoch - oldTimes[count];
                    if(difference==0){
                        difference=1000;
                    }

                    triggerRate = abs(hexValue - oldValues[count])/difference*1000.0;

                    oldTimes[count] = milliseconds_since_epoch;
                    

                    if(hexValue==0){
                        triggerRate=0;
                    }
                    
                    if(oldValuesRates[count]!=triggerRate){
                        oldValuesRates[count] = triggerRate;
                        updateTopicAnswer(servicesRates[count], std::to_string(triggerRate));
                    }
                    oldValues[count] = hexValue;
                    updateTopicAnswer(services[count], std::to_string(hexValue));
                    
                }
                count++;
            }
            firstTime=false;
        }
        catch(exception& e){
            Print::PrintInfo(e.what());
            Print::PrintError("ERROR in connection with ALF!");
        }
        return "OK";
    }
    else{
        this->publishError("Failure");
        return "failure";
    }
}

