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

/*TriggerSign::TriggerSign(std::string endpoint) : IndefiniteMapi::IndefiniteMapi(){
  triggerEndpoint = endpoint;
}

TriggerSign::~TriggerSign(){}

void TriggerSign::processExecution(){
    bool running;
    string response;

    string request = this->waitForRequest(running);
    if (!running){
        return;
    }

    if (request == ""){
      int value = 0;
      if(triggerEndpoint=="TRIGGER5_SIGN"){
        Print::PrintInfo("trigger5 sign");
        Print::PrintInfo(std::to_string(tcm.temp.trigger5sign));
        value = tcm.temp.trigger5sign/128;
      }
      else if(triggerEndpoint=="TRIGGER4_SIGN"){
        value = tcm.temp.trigger4sign/128;
      }
      else if(triggerEndpoint=="TRIGGER3_SIGN"){
        value = tcm.temp.trigger3sign/128;
      }
      else if(triggerEndpoint=="TRIGGER2_SIGN"){
        value = tcm.temp.trigger2sign/128;
      }
      else if(triggerEndpoint=="TRIGGER1_SIGN"){
        value = tcm.temp.trigger1sign/128;
      }
      this->publishAnswer(std::to_string(value));
    }
    else if (request == "error"){
        this->publishError("Error!");
    }
    else{

        //response = this->executeAlfSequence("read\n0x00000170,0x80000000"); // execute desired sequence to alf, waits for response from ALF
        //this->publishAnswer(response);

        //response = this->executeAlfSequence("read\n0x00000180,0x80000000"); // It is possible to execute multiple sequences to ALF with one command from WinCC
        //this->publishAnswer(response);
    }
}*/

