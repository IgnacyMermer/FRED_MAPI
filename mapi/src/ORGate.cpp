

#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include "ORGate.h"
#include "Alfred/print.h"
#include "Parser/utility.h"
#include "tcmValues.h"
#include <sstream>
#include <iomanip>
#include "swtCreator.h"
#include <cmath>

ORGate::ORGate(string endpointParam) {
  finalValue = 0;
  endpoint=endpointParam;
}

string ORGate::processInputMessage(string input) {
    vector<string> parameters = Utility::splitString(input, ",");
    if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
        if(endpoint=="A0"){
            sequence="reset\n0x0000000020000000000,write\nread";
        }
        else if(endpoint=="C0"){
            sequence="reset\n0x0000000160000000000,write\nread";
        }
    }
    else if(parameters.size()>1&&parameters[1]=="1"){
        std::stringstream ss;
        ss << std::hex << SwtCreator::parameterValue(parameters[0]);
        std::string hex_str = ss.str();
        if(hex_str.length()>8){
            hex_str=hex_str.substr(hex_str.length()-4);
        }
        std::string data="";
        for(int i=0; i<8-hex_str.length(); i++){
            data+="0";
        }
        data+=hex_str;
        if(endpoint=="A0"){
            sequence="reset\n0x000000002,"+data+"write\nread";
        }
        else if(endpoint=="C0"){
            sequence="reset\n0x000000016"+data+",write\nread";
        }
    }
    else{
        sequence="";
        noRpcRequest=true;
        this->publishError("Wrong parameters");   
    }
    return sequence;
}


string ORGate::processOutputMessage(string output) {
  string value;

  if(output!="failure"){
    try {
        string value;
        output.erase(remove(output.begin(), output.end(), '\n'), output.end());
        value = output.substr(output.size() - 8, output.size());
        finalValue = stoll(value.substr(0,8), nullptr, 16);
        return std::to_string(finalValue);
    }
    catch (exception &e) {
        Print::PrintError("ERROR in connection with ALF!");
        finalValue=0;
        this->publishError("ERROR");
    }
  }
  else{
    this->publishError("Failure response");
  }

  return "0";
}
