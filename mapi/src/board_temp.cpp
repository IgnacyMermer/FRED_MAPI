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

  if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
    sequence = "reset\n0x0000000000500000000,write\nread";
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


/*BoardTemp::BoardTemp() : IndefiniteMapi::IndefiniteMapi()
{}

BoardTemp::~BoardTemp(){}

void BoardTemp::processExecution(){
    bool running;
    string response;

    string request = this->waitForRequest(running);
    if (!running){
        return;
    }

    if (request == ""){
      int boardTemp = tcm.temp.boardTemp;
      float finalValue = boardTemp/10.;
      this->publishAnswer(std::to_string(finalValue));
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

