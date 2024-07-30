#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include "delayA.h"
#include "Alfred/print.h"
#include "Parser/utility.h"
#include "TCM_values.h"


DelayA::DelayA() {
  finalValue = 0;
}

string DelayA::processInputMessage(string input) {

  sequence = "0x0001114440000000000,write\nread";
  return sequence;
}

string DelayA::processOutputMessage(string output) {
  string value;

  try {
    output.erase(remove(output.begin(), output.end(), '\n'), output.end());
    value = output.substr(output.size() - 8, output.size());
    finalValue = stoi(value, nullptr, 16);
    float systemClock_MHz = tcm.act.externalClock?40.0789658:40.;
    float halfBC_ns = 500. / systemClock_MHz;
    float phaseStep_ns = halfBC_ns / /*(SERIAL_NUM ? */1024/* : 1280)*/;
    finalValue = finalValue*phaseStep_ns;
  }
  catch (exception &e) {
    Print::PrintError("ERROR in connection with ALF!");
    finalValue=0;
  }

  return to_string(finalValue);
}
