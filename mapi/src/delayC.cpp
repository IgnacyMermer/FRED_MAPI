#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include "delayC.h"
#include "Alfred/print.h"
#include "Parser/utility.h"
#include "TCM_values.h"
#include <sstream>

DelayC::DelayC() {
  finalValue = 0;
}

string DelayC::processInputMessage(string input) {
  vector<string> parameters = Utility::splitString(input, ",");
  std::string address = "0000"+tcm.addresses["READOUTCARDS/TCM0/DELAY_C"];
  if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
    sequence = "reset\n0x000"+address+"00000000,write\nread";
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
    sequence="reset\n0x001"+address+data+",write\nread";
  }
  else{
    sequence="";
  }

  return sequence;
}

string DelayC::processOutputMessage(string output) {
  string value;

  try {
    Print::PrintInfo(output);
    output.erase(remove(output.begin(), output.end(), '\n'), output.end());
    value = output.substr(output.size() - 8, output.size());
    finalValue = stoi(value, nullptr, 16);
    if (finalValue > 10000) {
      int16_t x = stoi(value, nullptr, 16);
      finalValue=-(~x+1);
    }
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

