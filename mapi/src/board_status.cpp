#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include "board_status.h"
#include "Alfred/print.h"
#include "Parser/utility.h"
#include "TCM_values.h"
#include <sstream>
#include <iomanip>

BoardStatus::BoardStatus() {
  finalValue = 0;
}

string BoardStatus::processInputMessage(string input) {
  if(input.length()>5&&input.substr(0,5)=="FRED,"){
    this->publishAnswer(input.substr(5));
    noRpcRequest=true;
    return "0";
  }
  vector<string> parameters = Utility::splitString(input, ",");
  std::string address = "0000"+tcm.addresses["READOUTCARDS/TCM0/BOARD_STATUS"];
  if(parameters.size()>1&&parameters[1]=="1"){
    if(parameters[0]=="0x200"){
      sequence = "reset\n0x001"+address+"00000200,write\nread";
    }
    else if(parameters[0]=="0x800"){
      if(tcm.act.forceLocalClock==1){
        sequence = "reset\n0x001"+address+"00000C00,write\nread";
      }
      else{
        sequence = "reset\n0x001"+address+"00000800,write\nread";
      }
    }
    else{
      sequence="";
    }
  }
  else if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
    sequence = "reset\n0x000"+address+"00000000,write\nread";
  }
  else{
    sequence="";
    this->publishError("Readonly parameter");
  }
  return sequence;
}

string BoardStatus::processOutputMessage(string output) {
  string value;

  try {
    output.erase(remove(output.begin(), output.end(), '\n'), output.end());
    value = output.substr(output.size() - 8, output.size());
    finalValue = stoi(value, nullptr, 16);
    int pllLockC = (finalValue)&1;
    int pllLockA = (finalValue>>1)&1;
    int systemRestarted = (finalValue>>2)&1;
    int clockSrc = (finalValue>>3)&1;
    int RxReady = (finalValue>>4)&1;
    int forceLocalClock = (finalValue>>10)&1;
    tcm.act.PLLlockA=pllLockA;
    tcm.act.PLLlockC=pllLockC;
    tcm.act.systemRestarted=systemRestarted;
    tcm.act.externalClock=clockSrc;
    tcm.act.GBTRxReady=RxReady;
    tcm.act.forceLocalClock=forceLocalClock;
    std::stringstream stream;
    stream << std::hex << finalValue;
    value = "0x"+stream.str();
  }
  catch (exception &e) {
    Print::PrintError("ERROR in connection with ALF!");
    finalValue=0;
  }

  return value;
}
