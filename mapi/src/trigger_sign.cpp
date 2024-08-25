#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include "trigger_sign.h"
#include "Alfred/print.h"
#include "Parser/utility.h"
#include <thread>
#include <chrono>
#include <sstream>
#include "TCM_values.h"


TriggerSign::TriggerSign(std::string endpoint) {
  triggerEndpoint=endpoint;
  finalValue = 0;
}

string TriggerSign::processInputMessage(string input) {
  std::string address = "";
  vector<string> parameters = Utility::splitString(input, ",");
  if(triggerEndpoint=="TRG_ORA_SIGN"){
    address="00000060";
  }
  else if(triggerEndpoint=="TRG_ORC_SIGN"){
    address="00000062";
  }
  else if(triggerEndpoint=="TRG_V_SIGN"){
    address="00000068";
  }
  else if(triggerEndpoint=="TRG_SC_SIGN"){
    address="00000064";
  }
  else if(triggerEndpoint=="TRG_C_SIGN"){
    address="00000066";
  }

  if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
    sequence = "reset\n0x000"+address+"00000000,write\nread";
  }
  else if(parameters.size()>1&&parameters[1]=="1"){
    int num = std::stoi(parameters[0]);
    num*=128;
    std::stringstream ss;
    ss << std::hex << num;
    std::string hex_str = ss.str();
    std::string data="";
    for(int i=0; i<8-hex_str.length(); i++){
      data+="0";
    }
    data+=hex_str;
    sequence="reset\n0x001"+address+data+",write\nread";
  }
  else{
    this->publishError("Parameter can be only read or write and range is 0-7");
    sequence="";
  }
  return sequence;
}

string TriggerSign::processOutputMessage(string output) {
  string value;

  try {
    output.erase(remove(output.begin(), output.end(), '\n'), output.end());
    value = output.substr(output.size() - 4, output.size());
    //Print::PrintInfo(value);
    //newRequest(sequence+"r");
    finalValue = (stoi(value, nullptr, 16))/128;
  }
  catch (exception &e) {
    Print::PrintError("ERROR in connection with ALF!");
  }

  return to_string(finalValue);
}

