#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include "board_type.h"
#include "Alfred/print.h"
#include "Parser/utility.h"
#include "TCM_values.h"


BoardType::BoardType() {
  finalValue = 0;
}

string BoardType::processInputMessage(string input) {

  sequence = "0x0001118880000000000,write\nread";
  return sequence;
}

string BoardType::processOutputMessage(string output) {
  string value;

  try {
    Print::PrintInfo(output);
    output.erase(remove(output.begin(), output.end(), '\n'), output.end());
    value = output.substr(output.size() - 4, output.size());
    int boardBits = stoi(value, nullptr, 16);
    finalValue = boardBits;
    int detectorSubType = boardBits & 2;
    int boardSerialNr = ((boardBits>>8)&0xF);
    tcm.act.boardType=detectorSubType;
    tcm.act.SERIAL_NUM=boardSerialNr;
    Print::PrintInfo(std::to_string(detectorSubType));
    Print::PrintInfo(std::to_string(boardSerialNr));
  }
  catch (exception &e) {
    Print::PrintError("ERROR in connection with ALF!");
    finalValue=0;
  }

  return output;
}
