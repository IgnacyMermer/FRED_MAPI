#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include "WorkStatus.h"
#include "Alfred/print.h"
#include "Parser/utility.h"

WorkStatus::WorkStatus() {
  status = 0;
}

string WorkStatus::processInputMessage(string input) {
  this->publishAnswer("1");
  noRpcRequest=true;
  //sequence = "0x0000000000000000000,write\nread";
  return "";
}

string WorkStatus::processOutputMessage(string output) {
  string value;

  try {
    output.erase(remove(output.begin(), output.end(), '\n'), output.end());
    status = 1;
  }
  catch (exception &e) {
    Print::PrintError("ERROR in connection with ALF!");
    status=0;
  }

  return to_string(status);
}
