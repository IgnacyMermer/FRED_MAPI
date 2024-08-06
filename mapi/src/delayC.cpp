#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include "delayC.h"
#include "Alfred/print.h"
#include "Parser/utility.h"
#include "TCM_values.h"

/*DelayC::DelayC() {
  finalValue = 0;
}

string DelayC::processInputMessage(string input) {

  sequence = "reset\n0x0000000000100000000,write\nread";
  return sequence;
}

string DelayC::processOutputMessage(string output) {
  string value;

  try {
    output.erase(remove(output.begin(), output.end(), '\n'), output.end());
    value = output.substr(output.size() - 8, output.size());
    finalValue = stoi(value, nullptr, 16);
    float systemClock_MHz = tcm.act.externalClock?40.0789658:40.;
    float halfBC_ns = 500. / systemClock_MHz;
    float phaseStep_ns = halfBC_ns / 
    //(SERIAL_NUM ? 
    1024
    // : 1280);
    finalValue = finalValue*phaseStep_ns;
  }
  catch (exception &e) {
    Print::PrintError("ERROR in connection with ALF!");
    finalValue=0;
  }

  return to_string(finalValue);
}*/

DelayC::DelayC() : IndefiniteMapi::IndefiniteMapi()
{}

DelayC::~DelayC(){}

void DelayC::processExecution()
{
    bool running;
    string response;

    string request = this->waitForRequest(running); // Wait for incoming request from WinCC
    if (!running){
        return;
    }

    if (request == ""){
      int delayATemp = tcm.temp.delayC;
      float systemClock_MHz = tcm.act.externalClock?40.0789658:40.;
      float halfBC_ns = 500. / systemClock_MHz;
      float phaseStep_ns = halfBC_ns / /*(SERIAL_NUM ? */1024 /*: 1280)*/;
      float finalValue = delayATemp*phaseStep_ns;
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
}
