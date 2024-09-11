#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include "Register.h"
#include "Alfred/print.h"
#include "Parser/utility.h"
#include <sstream>
#include <cmath>
#include "swtCreator.h"
#include "wordsUtility.h"
#include "tcmValues.h"
#include <bitset>
#include "WinccMessage.h"

Register::Register(std::string endpoint, std::string address, std::vector<std::vector<int64_t>> words, std::pair<std::string, std::string> equation)
:endpoint(endpoint), finalValue(0), address(address), words(words), equation(equation) {}

string Register::processInputMessage(string input) {

    //message from refresh mapi group
    if(WordsUtility::mapiMessage(input)){
        this->publishAnswer(input.substr(5));
        noRpcRequest=true;
        return "0";
    }

    WinccMessage messageFromWinCC(input);

    //read input message    
    if (messageFromWinCC.readMessage()){
        SwtCreator::sequenceOperationRead(address, sequence, false);
        return sequence;
    }
    //write value on register input message
    else if(messageFromWinCC.writeMessage()){
        int64_t value = messageFromWinCC.getValueWriteMessage();
        bool isSigned=false;
        if(WordsUtility::checkValueWords(endpoint, value, isSigned, words)){
            if(isSigned&&value<0){
                value=~((value*(-1))-1);
            }
            SwtCreator::sequenceOperationWrite(value, address, sequence, false);
            return sequence;
        }
        else{
            noRpcRequest=true;
            this->publishError("Value out of correct range");
            return "";
        }
    }
    //write value on specific bits in register
    else if(messageFromWinCC.writeWordMessage()){
        int64_t index = messageFromWinCC.getIndexAndOrWriteMessage();
        int64_t value = messageFromWinCC.getValueWriteWordMessage();
        bool isSigned=false;
        int firstBit=0, lastBit=0;
        if(WordsUtility::findCheckWord(endpoint, value, index, isSigned, firstBit, lastBit, words)){
            uint32_t mask = ~(((1ULL<<(lastBit-firstBit+1))-1)<<firstBit);
            if(isSigned&&value<0){
                value=~((value*(-1))-1);
                Print::PrintInfo(std::to_string(value));
            }
            SwtCreator::sequenceOperationBits(value, firstBit, mask, address, sequence, false);
            return sequence;
        }
        else{
            noRpcRequest=true;
            this->publishError("Value out of correct range");
            return "";
        }
    }
    //set or clear one bit
    else if(messageFromWinCC.orAndMessage()){
        int64_t index = messageFromWinCC.getIndexAndOrWriteMessage();
        if(WordsUtility::checkWord(endpoint, index, words)){
            if(messageFromWinCC.orMessage()){
                SwtCreator::sequenceOperationRMWAND(index, address, sequence, false);
            }
            else{
                SwtCreator::sequenceOperationRMWOR(index, address, sequence, false);
            }
            return sequence;
        }
        else{
            noRpcRequest=true;
            this->publishError("Value out of correct range");
            return "";
        }
    }
}


string Register::processOutputMessage(string output) {
  string value;
  try {
    output.erase(remove(output.begin(), output.end(), '\n'), output.end());
    value = output.substr(output.size() - 8, output.size());
    finalValue = stoll(value, nullptr, 16);

    if(equation.first!=""){
        //check if there is only one word and if it is signed
        if(words.size()==1&&words[0][3]==1){
            if (finalValue > 10000) {
                int16_t x = stoi(value, nullptr, 16);
                finalValue=-(~x+1);
            }
        }
        
        std::vector<std::string> paramNames = Utility::splitString(equation.second,";");
        std::vector<double> values = std::vector<double>{(double)finalValue};
        
        if(paramNames.size()>1&&paramNames[1]=="systemClock"){
            values.push_back(tcm.act.externalClock?40.0789658:40.);
        }
        return std::to_string(Utility::calculateEquation(equation.first,paramNames,values));
    }

    else if(endpoint.find("AVERAGE_TIME")!=string::npos){
        return "";
    }
    
    else if(endpoint.find("BOARD_TYPE")!=string::npos){
        value = output.substr(output.size() - 4, output.size());
        int boardBits = stoi(value, nullptr, 16);
        int boardSerialNr = ((boardBits>>8)&0xFF);
        tcm.act.SERIAL_NUM=boardSerialNr;
    }
    noReturn=false;
  }
  catch (exception &e) {
    Print::PrintInfo(e.what());
    Print::PrintError("ERROR in connection with ALF!");
    finalValue=0;
  }
  return to_string(finalValue);
}
