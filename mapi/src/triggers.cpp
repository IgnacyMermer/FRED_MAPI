#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include "triggers.h"
#include "Alfred/print.h"
#include "Parser/utility.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <bitset>
#include <thread>
#include <cmath>
#include <chrono>
#include "TCM_values.h"
#include "SWT_creator.h"

Triggers::Triggers() {
    finalValue = 0; 
}

string Triggers::processInputMessage(string input) {
    //std::string address="0000006A";
    std::string address = "0000"+tcm.addresses["READOUTCARDS/TCM0/TRIGGERS_OUTPUTS_MODE"];
    vector<string> parameters = Utility::splitString(input, ",");
    if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
        sequence = "reset\n0x000"+address+"00000000,write\nread";
    }
    else if(parameters.size()>1&&parameters[1]=="1"){
        SWT_creator::sequenceOperation1(std::stoi(parameters[0]), address, sequence);
    }
    else if(parameters.size()>2&&(parameters[0]=="0"||parameters[0]=="1")){
        SWT_creator::sequenceOperationBits(std::stoi(parameters[2]), 0, 0xFFFFFFFC, address, sequence);
    }
    else if(parameters.size()>2&&(parameters[0]=="3"||parameters[0]=="4")){
        SWT_creator::sequenceOperationBits(std::stoi(parameters[2]), 3, 0xFFFFFFE7, address, sequence);
    }
    else if(parameters.size()>2&&(parameters[0]=="6"||parameters[0]=="7")){
        SWT_creator::sequenceOperationBits(std::stoi(parameters[2]), 6, 0xFFFFFF3F, address, sequence);
    }
    else if(parameters.size()>2&&(parameters[0]=="9"||parameters[0]=="10")){
        SWT_creator::sequenceOperationBits(std::stoi(parameters[2]), 9, 0xFFFFF9FF, address, sequence);
    }
    else if(parameters.size()>2&&(parameters[0]=="12"||parameters[0]=="13")){
        SWT_creator::sequenceOperationBits(std::stoi(parameters[2]), 12, 0xFFFFCFFF, address, sequence);
    }
    else if(parameters.size()>1&&parameters[1]=="3"){
        SWT_creator::sequenceOperation3(std::stoi(parameters[0]), address, sequence);
    }
    else if(parameters.size()>1&&parameters[1]=="2"){
        SWT_creator::sequenceOperation2(std::stoi(parameters[0]), address, sequence);
    }
    else{
        noRpcRequest=true;
        this->publishError("Wrong parameters");
        sequence="";
    }
    return sequence;
}

string Triggers::processOutputMessage(string output) {
  string value;

  try {
    output.erase(remove(output.begin(), output.end(), '\n'), output.end());
    value = output.substr(output.size() - 8);
    finalValue = (stoi(value, nullptr, 16));
    std::bitset<32> binary(finalValue);
    
    std::string binary_str = binary.to_string();

    return binary_str;
  }
  catch (exception &e) {
    finalValue=0;
    Print::PrintError("ERROR in connection with ALF!");
  }

  return to_string(finalValue);
}

