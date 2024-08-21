#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include "initFred.h"
#include "Alfred/print.h"
#include "Parser/utility.h"
#include "TCM_values.h"
#include <sstream>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <sstream>


namespace fs = boost::filesystem;
namespace po = boost::program_options;
namespace pt = boost::property_tree;


InitFred::InitFred() : IndefiniteMapi::IndefiniteMapi()
{}

InitFred::~InitFred(){}

void InitFred::processExecution(){
    bool running;
    string response;

    string request = this->waitForRequest(running);
    std::string sequence = "reset\n0x0000000000000000000,write\nread\n0x0000000000100000000,write\nread";

    response = this->executeAlfSequence(sequence);

    long long delayA = 0, delayC = 0;
    try{
        response.erase(remove(response.begin(), response.end(), '\n'), response.end());
        response = response.substr(8);
        Print::PrintInfo("response");
        Print::PrintInfo(response.substr(13, 8));
        delayA = stoll(response.substr(13, 8), nullptr, 16);
        response = response.substr(22);
        Print::PrintInfo("response2");
        Print::PrintInfo(response.substr(13, 8));
        delayC = stoll(response.substr(13, 8), nullptr, 16);
    }
    catch(exception& e){
        Print::PrintError(e.what());
    }

    sequence = "reset\n";

    if (request == "go"){

        std::string fileName = "default.cfg";
        pt::ptree tree;

        if (!fs::exists(fileName)) {
            fileName = "./configuration/" + fileName;
            if (!fs::exists(fileName)) {
                fileName = "./configuration/default.cfg";
            }
        }

        try{
            pt::ini_parser::read_ini(fileName, tree);
            
            for (const auto& section : tree) {
                Print::PrintInfo(section.first);
                std::map<int, long long> confParameters;
                if(section.first=="TCM"){
                    for (const auto& key_value : section.second) {
                        int address = std::stoi(key_value.first.substr(key_value.first.length()-2), nullptr, 16);
                        Print::PrintInfo(key_value.second.get_value<std::string>());
                        long long value = std::stoll(key_value.second.get_value<std::string>(), nullptr, 16);
                        if(address<256){
                            confParameters[address] = value;
                        }
                    }

                    if(confParameters.find(0)!=confParameters.end()&&confParameters.find(1)!=confParameters.end()){
                        long long delay = std::max(std::max((int)(confParameters[0]-delayA), (int)(confParameters[1]-delayC)), 0);
                        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
                        std::stringstream ss;
                        ss << std::hex << confParameters[0];
                        std::string hex_str = ss.str();
                        std::string data="";
                        for(int i=0; i<8-hex_str.length(); i++){
                            data+="0";
                        }
                        data+=hex_str;
                        std::string sequence = "reset\n0x00100000000"+data+",write\nread";
                        ss.str("");
                        ss << std::hex << confParameters[1];
                        hex_str = ss.str();
                        data="";
                        for(int i=0; i<8-hex_str.length(); i++){
                            data+="0";
                        }
                        data+=hex_str;
                        sequence += "\n0x00100000001"+data+",write\nread";
                        Print::PrintInfo("set parameters sequence 0:");
                        Print::PrintInfo(sequence);
                        this->executeAlfSequence(sequence);
                    }
                    else if(confParameters.find(0)!=confParameters.end()){
                        long long delay = std::max((int)(confParameters[0]-delayA), 0);
                        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
                        std::stringstream ss;
                        ss << std::hex << confParameters[0];
                        std::string hex_str = ss.str();
                        std::string data="";
                        for(int i=0; i<8-hex_str.length(); i++){
                            data+="0";
                        }
                        data+=hex_str;
                        Print::PrintInfo("set parameters sequence 1:");
                        Print::PrintInfo("reset\n0x00100000000"+data+",write\nread");
                        std::string sequence = "reset\n0x00100000000"+data+",write\nread";
                        this->executeAlfSequence(sequence);
                        
                    }
                    else if(confParameters.find(1)!=confParameters.end()){
                        long long delay = std::max(0, (int)(confParameters[1]-delayC));
                        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
                        std::stringstream ss;
                        ss << std::hex << confParameters[1];
                        std::string hex_str = ss.str();
                        std::string data="";
                        for(int i=0; i<8-hex_str.length(); i++){
                            data+="0";
                        }
                        data+=hex_str;
                        Print::PrintInfo("set parameters sequence 2:");
                        Print::PrintInfo("reset\n0x00100000001"+data+",write\nread");
                        std::string sequence = "reset\n0x00100000001"+data+",write\nread";
                        this->executeAlfSequence(sequence);
                    }
                    confParameters.erase(0x50);
                    std::string sequence = "";
                    if(confParameters.find(0xE)!=confParameters.end()){
                        int temp = confParameters[0xE]&0x0000030F;
                        std::stringstream ss;
                        ss << std::hex << temp;
                        std::string hex_str = ss.str();
                        std::string data="";
                        for(int i=0; i<8-hex_str.length(); i++){
                            data+="0";
                        }
                        data+=hex_str;
                        sequence = "reset\n0x0020000000EFFFFFCF0,write\nread\n0x0030000000E"+data+",write\nread";
                        confParameters.erase(0xE);
                    }
                    for (const auto& pair : confParameters) {
                        std::stringstream ss;
                        ss << std::hex << pair.first;
                        std::string hex_str = ss.str();
                        std::string data="";
                        for(int i=0; i<8-hex_str.length(); i++){
                            data+="0";
                        }
                        data+=hex_str;
                        ss.str("");
                        ss << std::hex << pair.second;
                        hex_str = ss.str();
                        std::string data2="";
                        for(int i=0; i<8-hex_str.length(); i++){
                            data+="0";
                        }
                        data2+=hex_str;
                        sequence+="\n0x001"+data+data2+",write\nread";
                    }

                    Print::PrintInfo("set parameters sequence:");
                    Print::PrintInfo(sequence);

                    this->executeAlfSequence(sequence);
                    
                }
                
            }
        }
        catch(exception& e){
            Print::PrintError(e.what());
        }

        sequence+="0x0020000000EFFFFFCFF,write\nread\n0x0030000000E00000200,write\nread\n";
        sequence+="0x0010000006600003711,write\nread\n";
        sequence+="0x0010000006400003790,write\nread\n";
        sequence+="0x001000000680000380F,write\nread\n";
        sequence+="0x001000000620000388E,write\nread\n";
        sequence+="0x001000000600000390D,write\nread\n";
        sequence+="0x0010000001E000FFFFF,write\nread";
        //response = this->executeAlfSequence(sequence);
        this->publishAnswer("response");
    }
}

