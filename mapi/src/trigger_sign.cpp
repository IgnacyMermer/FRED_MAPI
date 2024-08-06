#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include "trigger_sign.h"
#include "Alfred/print.h"
#include "Parser/utility.h"
#include <thread>
#include <chrono>
#include "TCM_values.h"


/*TriggerSign::TriggerSign(std::string endpoint) {
  triggerEndpoint=endpoint;
  finalValue = 0;
}

string TriggerSign::processInputMessage(string input) {
  Print::PrintInfo(input);
  if(input.length()>10){
    if(input[input.length()-1]=='r'){
      std::this_thread::sleep_for(std::chrono::seconds(1));
      input=input.substr(0, input.length()-1);
    }
    sequence=input;
  }
  else{
    if(triggerEndpoint=="TRIGGER5_SIGN"){
      sequence = "reset\n0x0000000006000000000,write\nread";
    }
    else if(triggerEndpoint=="TRIGGER4_SIGN"){
      sequence = "reset\n0x0000000006200000000,write\nread";
    }
    else if(triggerEndpoint=="TRIGGER3_SIGN"){
      sequence = "reset\n0x0000000006800000000,write\nread";
    }
    else if(triggerEndpoint=="TRIGGER2_SIGN"){
      sequence = "reset\n0x0000000006400000000,write\nread";
    }
    else if(triggerEndpoint=="TRIGGER1_SIGN"){
      sequence = "reset\n0x0000000006600000000,write\nread";
    }
  }
  return sequence;
}

string TriggerSign::processOutputMessage(string output) {
  string value;

  try {
    std::cout<<std::to_string(tcm.temp.trigger1sign);
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
}*/

TriggerSign::TriggerSign(std::string endpoint) : IndefiniteMapi::IndefiniteMapi(){
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
}

