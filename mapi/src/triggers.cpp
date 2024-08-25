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


Triggers::Triggers() {
    finalValue = 0; 
}

string Triggers::processInputMessage(string input) {
    std::string address="0000006A";
    vector<string> parameters = Utility::splitString(input, ",");
    if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
        sequence = "reset\n0x000"+address+"00000000,write\nread";
    }
    else if(parameters.size()>1&&parameters[1]=="1"){
        int num = std::stoi(parameters[0]);
        std::stringstream ss;
        ss << std::hex << num;
        std::string hex_str = ss.str();
        std::string data="";
        for(int i=0; i<8-hex_str.length(); i++){
            data+="0";
        }
        data+=hex_str;
        sequence="reset\n0x001"+address+data+",write\nread";
    }
    else if(parameters.size()>2&&(parameters[0]=="0"||parameters[0]=="1")){
        int numValue = std::stoi(parameters[2]);
        int temp = 0xFFFFFFFF;
        temp-=3;
        std::stringstream ss;
        ss << std::hex << temp;
        std::string hex_str = ss.str();
        std::string data="";
        for(int i=0; i<8-hex_str.length(); i++){
            data+="0";
        }
        data+=hex_str;
        temp=numValue;
        std::stringstream ss2;
        ss2 << std::hex << temp;
        std::string data2 = "";
        hex_str = ss2.str();
        for(int i=0; i<8-hex_str.length(); i++){
            data2+="0";
        }
        data2+=hex_str;
        sequence = "reset\n0x002"+address+data+",write\nread\n0x003"+address+data2+",write\nread\n0x000"+address+"00000000,write\nread";
    }
    else if(parameters.size()>2&&(parameters[0]=="3"||parameters[0]=="4")){
        int numValue = std::stoi(parameters[2]);
        int temp = 0xFFFFFFFF;
        temp-=24;
        std::stringstream ss;
        ss << std::hex << temp;
        std::string hex_str = ss.str();
        std::string data="";
        for(int i=0; i<8-hex_str.length(); i++){
            data+="0";
        }
        data+=hex_str;
        temp=numValue*8;
        std::stringstream ss2;
        ss2 << std::hex << temp;
        std::string data2 = "";
        hex_str = ss2.str();
        for(int i=0; i<8-hex_str.length(); i++){
            data2+="0";
        }
        data2+=hex_str;
        sequence = "reset\n0x002"+address+data+",write\nread\n0x003"+address+data2+",write\nread\n0x000"+address+"00000000,write\nread";
    }
    else if(parameters.size()>2&&(parameters[0]=="6"||parameters[0]=="7")){
        int numValue = std::stoi(parameters[2]);
        int temp = 0xFFFFFFFF;
        temp-=192;
        std::stringstream ss;
        ss << std::hex << temp;
        std::string hex_str = ss.str();
        std::string data="";
        for(int i=0; i<8-hex_str.length(); i++){
            data+="0";
        }
        data+=hex_str;
        temp=numValue*64;
        std::stringstream ss2;
        ss2 << std::hex << temp;
        std::string data2 = "";
        hex_str = ss2.str();
        for(int i=0; i<8-hex_str.length(); i++){
            data2+="0";
        }
        data2+=hex_str;
        sequence = "reset\n0x002"+address+data+",write\nread\n0x003"+address+data2+",write\nread\n0x000"+address+"00000000,write\nread";
    }
    else if(parameters.size()>2&&(parameters[0]=="9"||parameters[0]=="10")){
        int numValue = std::stoi(parameters[2]);
        int temp = 0xFFFFFFFF;
        temp-=1536;
        std::stringstream ss;
        ss << std::hex << temp;
        std::string hex_str = ss.str();
        std::string data="";
        for(int i=0; i<8-hex_str.length(); i++){
            data+="0";
        }
        data+=hex_str;
        temp=numValue*512;
        std::stringstream ss2;
        ss2 << std::hex << temp;
        std::string data2 = "";
        hex_str = ss2.str();
        for(int i=0; i<8-hex_str.length(); i++){
            data2+="0";
        }
        data2+=hex_str;
        sequence = "reset\n0x002"+address+data+",write\nread\n0x003"+address+data2+",write\nread\n0x000"+address+"00000000,write\nread";
    }
    else if(parameters.size()>2&&(parameters[0]=="12"||parameters[0]=="13")){
        int numValue = std::stoi(parameters[2]);
        int temp = 0xFFFFFFFF;
        temp-=12288;
        std::stringstream ss;
        ss << std::hex << temp;
        std::string hex_str = ss.str();
        std::string data="";
        for(int i=0; i<8-hex_str.length(); i++){
            data+="0";
        }
        data+=hex_str;
        temp=numValue*4096;
        std::stringstream ss2;
        ss2 << std::hex << temp;
        std::string data2 = "";
        hex_str = ss2.str();
        for(int i=0; i<8-hex_str.length(); i++){
            data2+="0";
        }
        data2+=hex_str;
        sequence = "reset\n0x002"+address+data+",write\nread\n0x003"+address+data2+",write\nread\n0x000"+address+"00000000,write\nread";
    }
    else if(parameters.size()>1&&parameters[1]=="3"){
        int num = std::stoi(parameters[0]);
        std::string data="";
        std::stringstream ss;
        ss << std::hex << (1 << num);
        std::string hex_str = ss.str();

        for(int i=0; i<8-hex_str.length(); i++){
            data+="0";
        }
        data+=hex_str;
        sequence = "reset\n0x002"+address+"FFFFFFFF,write\nread\n0x003"+address+data+",write\nread\n0x000"+address+"00000000,write\nread";
    }
    else if(parameters.size()>1&&parameters[1]=="2"){
        int num = std::stoi(parameters[0]);
        int temp = 0xFFFFFFFF;
        temp-=(1 << num);
        std::stringstream ss;
        ss << std::hex << temp;
        std::string data = ss.str();
        sequence = "reset\n0x002"+address+data+",write\nread\n0x003"+address+"00000000,write\nread\n0x000"+address+"00000000,write\nread";
    }
    else{
        noRpcRequest=true;
        this->publishError("Wrong parameters");
        sequence="";
    }
    return sequence;
}

string Triggers::processOutputMessage(string output) {
  string value;

  try {
    output.erase(remove(output.begin(), output.end(), '\n'), output.end());
    value = output.substr(output.size() - 8);
    finalValue = (stoi(value, nullptr, 16));
    std::bitset<32> binary(finalValue);
    
    std::string binary_str = binary.to_string();

    return binary_str;
  }
  catch (exception &e) {
    finalValue=0;
    Print::PrintError("ERROR in connection with ALF!");
  }

  return to_string(finalValue);
}

