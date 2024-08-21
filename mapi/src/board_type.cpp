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
  
  vector<string> parameters = Utility::splitString(input, ",");

  if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
    sequence = "reset\n0x0000000000700000000,write\nread";
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

/*

BoardType::BoardType() : IndefiniteMapi::IndefiniteMapi()
{}

BoardType::~BoardType(){}

void BoardType::processExecution(){
    bool running;
    string response;

    string request = this->waitForRequest(running);
    if (!running){
        return;
    }

    if (request == ""){
      int finalValue = tcm.temp.boardType;
      int detectorSubType = finalValue & 3;
      int boardSerialNr = ((finalValue>>8)&0xFF);
      tcm.act.boardType=detectorSubType;
      tcm.act.SERIAL_NUM=boardSerialNr;
      std::stringstream stream;
      stream << std::hex << tcm.temp.boardType;
      std::string hexValue = "0x"+stream.str();
      this->publishAnswer(hexValue);
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

*/