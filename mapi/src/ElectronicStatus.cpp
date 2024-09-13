#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include "ElectronicStatus.h"
#include "Alfred/print.h"
#include "Parser/utility.h"
#include "registerData.h"
#include <sstream>


ElectronicStatus::ElectronicStatus() : IndefiniteMapi::IndefiniteMapi()
{}


ElectronicStatus::~ElectronicStatus(){}

void ElectronicStatus::processExecution(){
    bool running;
    string response;

    string request = this->waitForRequest(running);
    
    vector<string> parameters = Utility::splitString(request, ",");

    if (!running){
        return;
    }

    if (request == ""||(parameters.size()>1&&parameters[1]=="0")){
        this->publishAnswer(std::to_string(registersData.temp.electronicStatus));
    }
    else if (request == "error"){
        this->publishError("Error!");
    }
    else if(parameters.size()>1&&parameters[1]=="1"){
        registersData.temp.electronicStatus = std::stoi(parameters[0]);
        this->publishAnswer(parameters[0]);
    }
}