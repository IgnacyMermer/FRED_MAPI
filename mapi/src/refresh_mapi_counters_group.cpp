#include <iostream>
#include <string>
#include <algorithm>
#include <unistd.h>
#include "refresh_mapi_counters_group.h"
#include "Parser/utility.h"
#include "Alfred/print.h"
#include "TCM_values.h"


RefreshMapiCountersGroup::RefreshMapiCountersGroup(Fred* fred){
    this->fred = fred;
    firstTime=true;
}

string RefreshMapiCountersGroup::processInputMessage(string input){

    std::string sequence = "reset\n0x0000000007000000000,write\nread\n0x0000000007100000000,write\nread\n0x0000000007400000000,write\nread\n0x0000000007200000000,write\nread\n0x0000000007300000000,write\nread";
    return sequence;
}

string RefreshMapiCountersGroup::processOutputMessage(string output){
    vector< pair <string, string> > requests;
    std::string value;
    try{
        output.erase(remove(output.begin(), output.end(), '\n'), output.end());
        output = output.substr(8);
        int maxCount=400, count=0;
        bool firstIteration=true;
        if(firstTime){
        tcm.temp.triggerCounterTemp = 0;
        requests.push_back(make_pair("FIT_FRED/READOUTCARDS/TCM0/WORK_STATUS", "set"));
        }
        while(output.length()>0&&count<maxCount){
            if(!firstIteration){
                output=output.substr(1);
            }
            firstIteration=false;
            
            value = output.substr(13, 8);
            output = output.substr(21);
            long long hexValue = stol(value, nullptr, 16);
            switch(count){
                case 0:
                if(tcm.temp.trigger5cnt!=hexValue||firstTime){
                    float triggerRate = 0.0;
                    if(firstTime){
                        triggerRate = (hexValue - tcm.temp.trigger5cnt)/1.0;
                    }
                    else if(tcm.temp.countersUpdRate!=0){
                        triggerRate = (hexValue - tcm.temp.trigger5cnt)/tcm.temp.countersUpdRateValue;
                    }
                    if(tcm.temp.trigger5rate!=triggerRate){
                        tcm.temp.trigger5rate = triggerRate;
                        requests.push_back(make_pair("FIT_FRED/READOUTCARDS/TCM0/TRIGGER5_RATE", "set"));
                    }
                    tcm.temp.trigger5cnt = hexValue;
                    Print::PrintInfo("sendCommand"+std::to_string(count));
                    requests.push_back(make_pair("FIT_FRED/READOUTCARDS/TCM0/TRIGGER5_CNT", "set"));
                }
                break;
                case 1:
                if(tcm.temp.trigger4cnt!=hexValue||firstTime){
                    float triggerRate = 0.0;
                    if(firstTime){
                        triggerRate = (hexValue - tcm.temp.trigger4cnt)/1.0;
                    }
                    else if(tcm.temp.countersUpdRate!=0){
                        triggerRate = (hexValue - tcm.temp.trigger4cnt)/tcm.temp.countersUpdRateValue;
                    }
                    if(tcm.temp.trigger4rate!=triggerRate){
                        tcm.temp.trigger4rate = triggerRate;
                        requests.push_back(make_pair("FIT_FRED/READOUTCARDS/TCM0/TRIGGER4_RATE", "set"));
                    }
                    tcm.temp.trigger4cnt = hexValue;
                    Print::PrintInfo("sendCommand"+std::to_string(count));
                    requests.push_back(make_pair("FIT_FRED/READOUTCARDS/TCM0/TRIGGER4_CNT", "set"));
                }
                break;
                case 2:
                if(tcm.temp.trigger3cnt!=hexValue||firstTime){
                    float triggerRate;
                    if(firstTime){
                        triggerRate = (hexValue - tcm.temp.trigger3cnt)/1.0;
                    }
                    else if(tcm.temp.countersUpdRate!=0){
                        triggerRate = (hexValue - tcm.temp.trigger3cnt)/tcm.temp.countersUpdRateValue;
                    }
                    if(tcm.temp.trigger3rate!=triggerRate){
                        //tcm.temp.trigger3rate = triggerRate;
                        requests.push_back(make_pair("FIT_FRED/READOUTCARDS/TCM0/TRIGGER3_RATE", "set"));
                    }
                    //tcm.temp.trigger3cnt = hexValue;
                    Print::PrintInfo("sendCommand"+std::to_string(count));
                    requests.push_back(make_pair("FIT_FRED/READOUTCARDS/TCM0/TRIGGER3_CNT", "set"));
                }
                break;
                case 3:
                if(tcm.temp.trigger2cnt!=hexValue||firstTime){
                    float triggerRate = 0.0;
                    if(firstTime){
                        triggerRate = (hexValue - tcm.temp.trigger2cnt)/1.0;
                    }
                    else if(tcm.temp.countersUpdRate!=0){
                        triggerRate = (hexValue - tcm.temp.trigger2cnt)/tcm.temp.countersUpdRate;
                    }
                    if(tcm.temp.trigger2rate!=triggerRate){
                        //tcm.temp.trigger2rate = triggerRate;
                        requests.push_back(make_pair("FIT_FRED/READOUTCARDS/TCM0/TRIGGER2_RATE", "set"));
                    }
                    //tcm.temp.trigger2cnt = hexValue;
                    Print::PrintInfo("sendCommand"+std::to_string(count));
                    requests.push_back(make_pair("FIT_FRED/READOUTCARDS/TCM0/TRIGGER2_CNT", "set"));
                }
                break;
                case 4:
                if(tcm.temp.trigger1cnt!=hexValue||firstTime){
                    float triggerRate = 0.0;
                    if(firstTime){
                        triggerRate = (hexValue - tcm.temp.trigger1cnt)/1.0;
                    }
                    else if(tcm.temp.countersUpdRate!=0){
                        Print::PrintInfo(std::to_string(hexValue - tcm.temp.trigger1cnt));
                        triggerRate = (hexValue - tcm.temp.trigger1cnt)/tcm.temp.countersUpdRate;
                        auto now = std::chrono::high_resolution_clock::now();

                        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
                        
                        int milliseconds_since_epoch = static_cast<int>(duration.count());

                        
                        Print::PrintInfo(std::to_string(tcm.temp.trigger1cnt));
                        triggerRate = (hexValue - tcm.temp.trigger1cnt)
                            /(milliseconds_since_epoch-tcm.temp.oldTimeTrigger1)*1000;

                        //tcm.temp.oldTimeTrigger1 = milliseconds_since_epoch;
                    }
                    if(tcm.temp.trigger1rate!=triggerRate){
                        tcm.temp.trigger1rate = triggerRate;
                        requests.push_back(make_pair("FIT_FRED/READOUTCARDS/TCM0/TRIGGER1_RATE", "set"));
                    }
                    //tcm.temp.trigger1cnt = hexValue;
                    Print::PrintInfo("sendCommand"+std::to_string(count));
                    requests.push_back(make_pair("FIT_FRED/READOUTCARDS/TCM0/TRIGGER1_CNT", "set"));
                }
                break;
            }
            count+=1;
        }
        tcm.temp.triggerCounterTemp++;
        firstTime=false;
        //newMapiGroupRequest(requests);

    }
    catch (exception &e) {
        Print::PrintInfo(e.what());
        Print::PrintError("ERROR in connection with ALF!");
    }

    return to_string(0);
    
    //Print::PrintInfo(output);
	//return output;
}
