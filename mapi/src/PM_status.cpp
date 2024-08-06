#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include "PM_status.h"
#include "Alfred/print.h"
#include "Parser/utility.h"
#include <thread>
#include <chrono>
#include "TCM_values.h"
#include <sstream>
#include <iomanip>
#include <bitset>


PM_status::PM_status(std::string endpoint) : IndefiniteMapi::IndefiniteMapi(){
    pmEndpoint = endpoint;
}

PM_status::~PM_status(){}

void PM_status::processExecution(){
    bool running;
    string response;

    string request = this->waitForRequest(running);
    vector<string> parameters = Utility::splitString(request, ",");

    if (!running){
        return;
    }

    if (request != "error"){
        int finalValue;
        if(pmEndpoint=="A0"){
            finalValue = tcm.temp.pmA0Status;
        }
        else if(pmEndpoint=="A1"){
            finalValue = tcm.temp.pmA1Status;
        }
        else if(pmEndpoint=="A2"){
            finalValue = tcm.temp.pmA2Status;
        }
        else if(pmEndpoint=="A3"){
            finalValue = tcm.temp.pmA3Status;
        }
        else if(pmEndpoint=="A4"){
            finalValue = tcm.temp.pmA4Status;
        }
        else if(pmEndpoint=="A5"){
            finalValue = tcm.temp.pmA5Status;
        }
        else if(pmEndpoint=="A6"){
            finalValue = tcm.temp.pmA6Status;
        }
        else if(pmEndpoint=="A7"){
            finalValue = tcm.temp.pmA7Status;
        }
        else if(pmEndpoint=="A8"){
            finalValue = tcm.temp.pmA8Status;
        }
        else if(pmEndpoint=="A9"){
            finalValue = tcm.temp.pmA9Status;
        }
        else if(pmEndpoint=="C0"){
            finalValue = tcm.temp.pmC0Status;
        }
        else if(pmEndpoint=="C1"){
            finalValue = tcm.temp.pmC1Status;
        }
        else if(pmEndpoint=="C2"){
            finalValue = tcm.temp.pmC2Status;
        }
        else if(pmEndpoint=="C3"){
            finalValue = tcm.temp.pmC3Status;
        }
        else if(pmEndpoint=="C4"){
            finalValue = tcm.temp.pmC4Status;
        }
        else if(pmEndpoint=="C5"){
            finalValue = tcm.temp.pmC5Status;
        }
        else if(pmEndpoint=="C6"){
            finalValue = tcm.temp.pmC6Status;
        }
        else if(pmEndpoint=="C7"){
            finalValue = tcm.temp.pmC7Status;
        }
        else if(pmEndpoint=="C8"){
            finalValue = tcm.temp.pmC8Status;
        }
        else if(pmEndpoint=="C9"){
            finalValue = tcm.temp.pmC9Status;
        }

        string data="0x";
        std::stringstream ss;
        ss << std::hex << finalValue;
        std::string hex_str = ss.str();

        for(int i=0; i<8-hex_str.length(); i++){
            data+="0";
        }
        data+=hex_str;
        this->publishAnswer(data);
    }
    else if (request == "error"){
        this->publishError("Error!");
    }
}
