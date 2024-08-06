#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include "work_status.h"
#include "Alfred/print.h"
#include "Parser/utility.h"

WorkStatus::WorkStatus() {
  status = 0;
}

string WorkStatus::processInputMessage(string input) {

  sequence = "0x0000000000000000000,write\nread";
  return sequence;
}

string WorkStatus::processOutputMessage(string output) {
  string value;

  try {
    Print::PrintInfo(output);
    output.erase(remove(output.begin(), output.end(), '\n'), output.end());
    //value = output.substr(output.size() - 3, output.size());
    status = 1;
  }
  catch (exception &e) {
    Print::PrintError("ERROR in connection with ALF!");
    status=0;
  }

  return to_string(status);
}
