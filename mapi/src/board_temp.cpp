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

  sequence = "0x0001117770000000000,write\nread";
  return sequence;
}

string BoardTemp::processOutputMessage(string output) {
  string value;

  try {
    output.erase(remove(output.begin(), output.end(), '\n'), output.end());
    value = output.substr(output.size() - 4, output.size());
    finalValue = stoi(value, nullptr, 16);
    finalValue = finalValue/10.;
  }
  catch (exception &e) {
    Print::PrintError("ERROR in connection with ALF!");
    finalValue=0;
  }

  return to_string(finalValue);
}
