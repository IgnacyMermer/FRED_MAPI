

#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include "ORGate.h"
#include "Alfred/print.h"
#include "Parser/utility.h"
#include "tcmValues.h"
#include <sstream>
#include <iomanip>
#include "swtCreator.h"
#include <cmath>
#include "WinccMessage.h"
#include "wordsUtility.h"


ORGate::ORGate(string endpoint, string address):endpoint(endpoint), address(address), finalValue(0) { }

string ORGate::processInputMessage(string input) {

    if(WordsUtility::mapiMessage(input)){
        this->publishAnswer(input.substr(5));
        noRpcRequest=true;
        return "0";
    }

    WinccMessage messageFromWinCC(input);

    //read input message    
    if (messageFromWinCC.readMessage()){
        SwtCreator::sequenceOperationRead(address, sequence);
        return sequence;
    }
    //write value on register input message
    else if(messageFromWinCC.writeMessage()){
        int64_t value = messageFromWinCC.getValueWriteMessage();
        bool isSigned=false;
        if(value>=0&&value<=255){
            SwtCreator::sequenceOperationWrite(value, address, sequence);
            return sequence;
        }
        else{
            noRpcRequest=true;
            this->publishError("Value out of correct range");
            return "";
        }
    }
}


string ORGate::processOutputMessage(string output) {
  string value;

  if(output!="failure"){
    try {
        string value;
        output.erase(remove(output.begin(), output.end(), '\n'), output.end());
        value = output.substr(output.size() - 8, output.size());
        finalValue = stoll(value.substr(0,8), nullptr, 16);
        return std::to_string(finalValue);
    }
    catch (exception &e) {
        Print::PrintError("ERROR in connection with ALF!");
        finalValue=0;
        this->publishError("ERROR");
    }
  }
  else{
    this->publishError("Failure response");
  }

  return "0";
}
