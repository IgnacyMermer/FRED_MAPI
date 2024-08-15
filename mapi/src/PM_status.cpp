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


PM_status::PM_status(std::string endpointParam){
    pmEndpoint = endpointParam;
}

string PM_status::processInputMessage(string input) {
    vector<string> parameters = Utility::splitString(input, ",");
    if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
        std::string address = "";
        if(pmEndpoint=="A0"){
            address = "00000010";
        }
        else if(pmEndpoint=="A1"){
            address = "00000011";
        }
        else if(pmEndpoint=="A2"){
            address = "00000012";
        }
        else if(pmEndpoint=="A3"){
            address = "00000013";
        }
        else if(pmEndpoint=="A4"){
            address = "00000014";
        }
        else if(pmEndpoint=="A5"){
            address = "00000015";
        }
        else if(pmEndpoint=="A6"){
            address = "00000016";
        }
        else if(pmEndpoint=="A7"){
            address = "00000017";
        }
        else if(pmEndpoint=="A8"){
            address = "00000018";
        }
        else if(pmEndpoint=="A9"){
            address = "00000019";
        }
        else if(pmEndpoint=="C0"){
            address = "00000030";
        }
        else if(pmEndpoint=="C1"){
            address = "00000031";
        }
        else if(pmEndpoint=="C2"){
            address = "00000032";
        }
        else if(pmEndpoint=="C3"){
            address = "00000033";
        }
        else if(pmEndpoint=="C4"){
            address = "00000034";
        }
        else if(pmEndpoint=="C5"){
            address = "00000035";
        }
        else if(pmEndpoint=="C6"){
            address = "00000036";
        }
        else if(pmEndpoint=="C7"){
            address = "00000037";
        }
        else if(pmEndpoint=="C8"){
            address = "00000038";
        }
        else if(pmEndpoint=="C9"){
            address = "00000039";
        }
        sequence = "reset\n0x000"+address+"00000000,write\nread";
    }
    else{
        this->publishError("Readonly paramater");
        sequence = "";
    }
    return sequence;
}

string PM_status::processOutputMessage(string output){
    string value;

    try {
        output.erase(remove(output.begin(), output.end(), '\n'), output.end());
        value = output.substr(output.size() - 8, output.size());
        return value;
    }
    catch (exception &e) {
        Print::PrintError("ERROR in connection with ALF!");
        finalValue=0;
        return "";
    }
}
