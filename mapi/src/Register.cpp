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
#include <bitset>

Register::Register(std::string endpoint, std::string address):endpoint(endpoint), finalValue(0), address(address) {}

string Register::processInputMessage(string input) {

    //message from refresh mapi group
    if(WordsUtility::mapiMessage(input)){
        this->publishAnswer(input.substr(5));
        noRpcRequest=true;
        return "0";
    }

    vector<string> parameters = Utility::splitString(input, ",");

    //read input message
    if (WordsUtility::readMessage(parameters)){
        SwtCreator::sequenceOperationRead(address, sequence);
        return sequence;
    }
    //write value on register input message
    else if(WordsUtility::writeMessage(parameters)){
        int64_t num = SwtCreator::parameterValue(parameters[0]);
        bool isSigned=false;
        if(WordsUtility::checkValueWords(endpoint, num, isSigned)){
            if(isSigned&&num<0){
                num=~((num*(-1))-1);
            }
            SwtCreator::sequenceOperationWrite(num, address, sequence);
            return sequence;
        }
        else{
            noRpcRequest=true;
            this->publishError("Value out of correct range");
            return "";
        }
    }
    //write value on specific bits in register
    else if(WordsUtility::writeWordMessage(parameters)){
        int64_t index = SwtCreator::parameterValue(parameters[0]);
        int64_t num = SwtCreator::parameterValue(parameters[2]);
        bool isSigned=false;
        int firstBit=0, lastBit=0;
        if(WordsUtility::findCheckWord(endpoint, num, index, isSigned, firstBit, lastBit)){
            uint32_t mask = ~(((1ULL<<(lastBit-firstBit+1))-1)<<firstBit);
            if(isSigned&&num<0){
                num=~((num*(-1))-1);
                Print::PrintInfo(std::to_string(num));
            }
            SwtCreator::sequenceOperationBits(num, firstBit, mask, address, sequence);
            Print::PrintInfo(sequence);
            return sequence;
        }
        else{
            noRpcRequest=true;
            this->publishError("Value out of correct range");
            return "";
        }
    }
    //set or clear one bit
    else if(WordsUtility::orAndMessage(parameters)){
        int64_t num = SwtCreator::parameterValue(parameters[0]);
        if(WordsUtility::checkWord(endpoint, num)){
            if(parameters[1]=="2"){
                SwtCreator::sequenceOperationRMWAND(num, address, sequence);
            }
            else{
                SwtCreator::sequenceOperationRMWOR(num, address, sequence);
            }
            Print::PrintInfo(sequence);
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
    if(tcm.tcmEquations[endpoint].first!=""){
        std::string equation = tcm.tcmEquations[endpoint].first;
        std::vector<std::string> paramNames = Utility::splitString(tcm.tcmEquations[endpoint].second,";");
        std::vector<double> values = std::vector<double>{finalValue};
        return std::to_string(Utility::calculateEquation(equation,paramNames,values));
    }
    else if(endpoint.find("AVERAGE_TIME")!=string::npos){
        return "";
    }
    else if(endpoint.find("LASER_DELAY")!=string::npos){
        float tempValue = stoi(value, nullptr, 16);
        if (tempValue > 10000) {
            int16_t x = stoi(value, nullptr, 16);
            tempValue=-(~x+1);
        }
        float systemClock_MHz = tcm.act.externalClock?40.0789658:40.;
        float halfBC_ns = 500. / systemClock_MHz;
        float phaseStep_ns = halfBC_ns / 
        //(SERIAL_NUM ? 
        1024 ; 
        //: 1280);
        tempValue = tempValue*phaseStep_ns;
        return std::to_string(tempValue);
    }
    else if(endpoint.find("LASER_DIVIDER")!=string::npos){
        float systemClock_MHz = tcm.act.externalClock?40.0789658:40.;
        uint32_t tempValue = std::stoll(value.substr(2,6), nullptr, 16);
        float laserFrequency = systemClock_MHz*std::pow(10,6)/(tempValue==0?1<<24:tempValue);
        updateTopicAnswer("READOUTCARDS/TCM0/LASER_FREQUENCY", std::to_string(laserFrequency));
    }
    else if(endpoint.find("DELAY_A")!=string::npos||endpoint.find("DELAY_C")!=string::npos){
        finalValue = stoi(value, nullptr, 16);
        if (finalValue > 10000) {
            int16_t x = stoi(value, nullptr, 16);
            finalValue=-(~x+1);
        }
        float systemClock_MHz = tcm.act.externalClock?40.0789658:40.;
        float halfBC_ns = 500. / systemClock_MHz;
        float phaseStep_ns = halfBC_ns / 
        //(SERIAL_NUM ? 
        1024 ; 
        //: 1280);
        return std::to_string(finalValue*phaseStep_ns);
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
