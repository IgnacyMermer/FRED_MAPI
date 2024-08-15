#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include "delayA.h"
#include "Alfred/print.h"
#include "Parser/utility.h"
#include "TCM_values.h"
#include <sstream>


DelayA::DelayA() {
  finalValue = 0;
}

string DelayA::processInputMessage(string input) {

  vector<string> parameters = Utility::splitString(input, ",");

  if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
    sequence = "reset\n0x0000000000000000000,write\nread";
  }
  else if(parameters.size()>1&&parameters[1]=="1"){
    float num = std::stof(parameters[0]);
    float systemClock_MHz = tcm.act.externalClock?40.0789658:40.;
    float halfBC_ns = 500. / systemClock_MHz;
    float phaseStep_ns = halfBC_ns / (
      //SERIAL_NUM ? 
      1024 
      //: 1280
      );
    num = num / phaseStep_ns;
    int value = (int)num;
    std::stringstream ss;
    ss << std::hex << value;
    std::string hex_str = ss.str();
    std::string data="";
    for(int i=0; i<8-hex_str.length(); i++){
      data+="0";
    }
    data+=hex_str;
    sequence="reset\n0x00100000000"+data+",write\nread";
  }
  else{
    sequence="";
  }

  return sequence;
}

string DelayA::processOutputMessage(string output) {
  string value;

  try {
    Print::PrintInfo(output);
    output.erase(remove(output.begin(), output.end(), '\n'), output.end());
    value = output.substr(output.size() - 8, output.size());
    finalValue = stoi(value, nullptr, 16);
    float systemClock_MHz = tcm.act.externalClock?40.0789658:40.;
    float halfBC_ns = 500. / systemClock_MHz;
    float phaseStep_ns = halfBC_ns / 
    //(SERIAL_NUM ? 
    1024 ; 
    //: 1280);
    finalValue = finalValue*phaseStep_ns;
  }
  catch (exception &e) {
    Print::PrintError("ERROR in connection with ALF!");
    finalValue=0;
  }

  return to_string(finalValue);
}


/*DelayA::DelayA() : IndefiniteMapi::IndefiniteMapi()
{}


DelayA::~DelayA(){}

void DelayA::processExecution(){
    bool running;
    string response;

    string request = this->waitForRequest(running);

    Print::PrintInfo(request);
    
    vector<string> parameters = Utility::splitString(request, ",");

    if (!running){
        return;
    }

    if (request == ""||(parameters.size()>1&&parameters[1]=="0")){
      int delayATemp = tcm.temp.delayA;
      float systemClock_MHz = tcm.act.externalClock?40.0789658:40.;
      float halfBC_ns = 500. / systemClock_MHz;
      float phaseStep_ns = halfBC_ns / (SERIAL_NUM ? 1024 : 1280);
      float finalValue = delayATemp*phaseStep_ns;
      this->publishAnswer(std::to_string(finalValue));
    }
    else if (request == "error"){
        this->publishError("Error!");
    }
    else if(parameters.size()>1&&parameters[1]=="1"){
      std::string data="";
      float num = std::stof(parameters[0]);response;
      float systemClock_MHz = tcm.act.externalClock?40.0789658:40.;
      float halfBC_ns = 500. / systemClock_MHz;
      float phaseStep_ns = halfBC_ns / (SERIAL_NUM ? 1024 : 1280);
      num = num / phaseStep_ns;
      int value = (int)num;
      std::stringstream ss;
      ss << std::hex << value;
      std::string hex_str = ss.str();
      data="";
      for(int i=0; i<8-hex_str.length(); i++){
        data+="0";
      }
      data+=hex_str;
      std::string sequence = "reset\n0x00100000000"+data+",write\nread";
      Print::PrintInfo("save delay A");
      Print::PrintInfo(sequence);
      response = this->executeAlfSequence(sequence);
      Print::PrintInfo(response);
      this->publishAnswer(parameters[0]);
    }
}*/

