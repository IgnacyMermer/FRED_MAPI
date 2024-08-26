#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include "board_temp.h"
#include "Alfred/print.h"
#include "Parser/utility.h"
#include "TCM_values.h"


BoardTemp::BoardTemp() {
  finalValue = 0;
}

string BoardTemp::processInputMessage(string input) {
  vector<string> parameters = Utility::splitString(input, ",");
  std::string address = "0000"+tcm.addresses["READOUTCARDS/TCM0/TEMPERATURE"];
  if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
    sequence = "reset\n0x000"+address+"00000000,write\nread";
  }
  else{
    sequence="";
    this->publishError("Readonly parameter");
  }
  return sequence;
}

string BoardTemp::processOutputMessage(string output) {
  string value;

  try {
    output.erase(remove(output.begin(), output.end(), '\n'), output.end());
    value = output.substr(output.size() - 4, output.size());
    finalValue = stoi(value, nullptr, 16);
    Print::PrintInfo("temperatire"+std::to_string(finalValue));
    finalValue = finalValue/10.;
  }
  catch (exception &e) {
    Print::PrintError("ERROR in connection with ALF!");
    finalValue=0;
  }

  return to_string(finalValue);
}

