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
    if(input.length()>5&&input.substr(0,5)=="FRED,"){
        this->publishAnswer(input.substr(5));
        noRpcRequest=true;
        return "0";
    }
    vector<string> parameters = Utility::splitString(input, ",");
    std::string address = "0000"+tcm.addresses["READOUTCARDS/TCM0/PM_LINK_"+pmEndpoint];
    if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
        sequence = "reset\n0x000"+address+"00000000,write\nread";
    }
    else{
        noRpcRequest=true;
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
