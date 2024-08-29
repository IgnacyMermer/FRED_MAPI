#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include "board_type.h"
#include "Alfred/print.h"
#include "Parser/utility.h"
#include "TCM_values.h"
#include <sstream>
#include <iomanip>


BoardType::BoardType() {
  finalValue = 0;
}

string BoardType::processInputMessage(string input) {
  if(input.length()>5&&input.substr(0,5)=="FRED,"){
    this->publishAnswer(input.substr(5));
    noRpcRequest=true;
    return "0";
  }
  vector<string> parameters = Utility::splitString(input, ",");
  std::string address = "0000"+tcm.addresses["READOUTCARDS/TCM0/BOARD_STATUS"];
  if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
    sequence = "reset\n0x000"+address+"00000000,write\nread";
  }
  else{
    sequence="";
    this->publishError("Readonly parameter");
  }
  return sequence;
}

string BoardType::processOutputMessage(string output) {
  string value;

  try {
    output.erase(remove(output.begin(), output.end(), '\n'), output.end());
    value = output.substr(output.size() - 4, output.size());
    int boardBits = stoi(value, nullptr, 16);
    finalValue = boardBits;
    int detectorSubType = boardBits & 3;
    int boardSerialNr = ((boardBits>>8)&0xFF);
    tcm.act.boardType=detectorSubType;
    tcm.act.SERIAL_NUM=boardSerialNr;
    return std::to_string(boardBits);
  }
  catch (exception &e) {
    Print::PrintError("ERROR in connection with ALF!");
    finalValue=0;
  }

  return std::to_string(0);
}
