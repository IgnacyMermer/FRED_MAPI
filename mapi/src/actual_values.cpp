#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include "actual_values.h"
#include "Alfred/print.h"
#include "Parser/utility.h"
#include "TCM_values.h"
#include <sstream>
#include <iomanip>


ActualValues::ActualValues() {
  finalValue = 0;
}

string ActualValues::processInputMessage(string input) {
  sequence = "0x0001110000000000000,write\nread";
  return sequence;
}

string ActualValues::processOutputMessage(string output) {
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
    tcm.act.PLLlockA=pllLockA;
    tcm.act.PLLlockC=pllLockC;
    tcm.act.systemRestarted=systemRestarted;
    tcm.act.externalClock=clockSrc;
    tcm.act.GBTRxReady=RxReady;
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
