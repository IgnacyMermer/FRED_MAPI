#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include "triggers.h"
#include "Alfred/print.h"
#include "Parser/utility.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <bitset>
#include <thread>
#include <chrono>
#include "TCM_values.h"


/*Triggers::Triggers() {
    sequence = "reset\n0x0000000006A00000000,write\nread";
    finalValue = 0;
}

string Triggers::processInputMessage(string input) {
    if(input[input.length()-1]=='r'){
        std::this_thread::sleep_for(std::chrono::seconds(1));
        input=input.substr(0, input.length()-1);
    }
    Print::PrintInfo(input);
    vector<string> parameters = Utility::splitString(input, ",");
    if(parameters[1]=="1"){
        string data="";
        if(parameters[0].length()>2&&parameters[0].substr(0,2)=="0x"){
            parameters[0]=parameters[0].substr(2);
            for(int i=0; i<8-parameters[0].length(); i++){
                data+="0";
            }
            data+=parameters[0];
        }
        else{
            int num = std::stoi(parameters[0]);
            std::stringstream ss;
            ss << std::hex << num;
            std::string hex_str = ss.str();
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
        }
        sequence = "reset\n0x0010000006A"+data+",write\nread";
    }
    else{
        sequence = "reset\n0x0000000006A00000000,write\nread";
    }
    return sequence;
}

string Triggers::processOutputMessage(string output) {
  string value;

  try {
    Print::PrintInfo("output");
    output.erase(remove(output.begin(), output.end(), '\n'), output.end());
    value = output.substr(output.size() - 8);
    finalValue = (stoi(value, nullptr, 16));
    std::bitset<32> binary(finalValue);
    
    std::string binary_str = binary.to_string();

    //newRequest("reset\n0x0000000006A00000000,write\nreadr");

    return binary_str;
  }
  catch (exception &e) {
    finalValue=0;
    Print::PrintError("ERROR in connection with ALF!");
  }

  return to_string(finalValue);
}*/


Triggers::Triggers() : IndefiniteMapi::IndefiniteMapi()
{}

Triggers::~Triggers(){}

void Triggers::processExecution(){
    bool running;
    string response;

    string request = this->waitForRequest(running);
    vector<string> parameters = Utility::splitString(request, ",");

    if (!running){
        return;
    }

    if (request == ""||(parameters.size()>1&&parameters[1]=="0")){
        int triggers = tcm.temp.triggers;
        std::bitset<32> binary(triggers);
        
        std::string binary_str = binary.to_string();
        this->publishAnswer(binary_str);
    }
    else if (request == "error"){
        this->publishError("Error!");
    }
    else{
        std::string sequence = "";
        if(parameters[1]=="1"){
            string data="";
            if(parameters[0].length()>2&&parameters[0].substr(0,2)=="0x"){
                parameters[0]=parameters[0].substr(2);
                for(int i=0; i<8-parameters[0].length(); i++){
                    data+="0";
                }
                data+=parameters[0];
            }
            else{
                int num = std::stoi(parameters[0]);
                std::stringstream ss;
                ss << std::hex << num;
                std::string hex_str = ss.str();
                for(int i=0; i<8-hex_str.length(); i++){
                    data+="0";
                }
                data+=hex_str;
            }
            sequence = "reset\n0x0010000006A"+data+",write\nread";
        }
        else{
            sequence = "reset\n0x0000000006A00000000,write\nread";
        }

        response = this->executeAlfSequence(sequence);

    }
}
