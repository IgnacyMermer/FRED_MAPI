#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include "ResetErrors.h"
#include "Alfred/print.h"
#include "Parser/utility.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <bitset>
#include <thread>
#include <chrono>
#include "tcmValues.h"
#include "swtCreator.h"


ResetErrors::ResetErrors() {
    
}

string ResetErrors::processInputMessage(string input) {
    vector<string> parameters = Utility::splitString(input, ",");
    if(input=="RESTART"&&parameters.size()>1){
        if(parameters[1]=="1"){
            sequence = "reset\n0x0010000000F00000C00,write\nread";
        }
        else{
            sequence = "reset\n0x0010000000F00000800,write\nread";
        }
        Print::PrintInfo(sequence);
        return sequence;
    }
    else{
        std::string pmAddress = SwtCreator::numberLetter(SwtCreator::parameterValue(parameters[1])*2);
        if(input=="GBT"){
            sequence="reset\n0x0020000"+pmAddress+"D8FFFF00FF,write\n0x0030000"+pmAddress+"D800000000,write\n0x0020000"+pmAddress+"D8FFFFFFFF,write\n0x0030000"+pmAddress+"D800001000,write\n0x0020000"+pmAddress+"D8FFFF00FF,write\n0x0030000"+pmAddress+"D800000000,write\n";
            return sequence;
        }
        else if(input=="DG_RESET"){
            sequence="reset\n0x0020000"+pmAddress+"D8FFFF00FF,write\n0x0030000"+pmAddress+"D800000000,write\n0x0020000"+pmAddress+"D8FFFFFFFF,write\n0x0030000"+pmAddress+"D800000400,write\n0x0020000"+pmAddress+"D8FFFF00FF,write\n0x0030000"+pmAddress+"D800000000,write\n";
            return sequence;
        }
        else if(input=="BITS_RESET"){
            sequence="reset\n0x0020000"+pmAddress+"D8FFFF00FF,write\n0x0030000"+pmAddress+"D800000000,write\n0x0020000"+pmAddress+"D8FFFFFFFF,write\n0x0030000"+pmAddress+"D800000800,write\n0x0020000"+pmAddress+"D8FFFF00FF,write\n0x0030000"+pmAddress+"D800000000,write\n";
            return sequence;
        }
        else if(input=="RX_RESET"){
            sequence="reset\n0x0020000"+pmAddress+"D8FFFF00FF,write\n0x0030000"+pmAddress+"D800000000,write\n0x0020000"+pmAddress+"D8FFFFFFFF,write\n0x0030000"+pmAddress+"D800002000,write\n0x0020000"+pmAddress+"D8FFFF00FF,write\n0x0030000"+pmAddress+"D800000000,write\n";
            return sequence;
        }
        else if(input=="RESYNC"){
            sequence="reset\n0x0020000"+pmAddress+"D8FFFF00FF,write\n0x0030000"+pmAddress+"D800000000,write\n0x0020000"+pmAddress+"D8FFFFFFFF,write\n0x0030000"+pmAddress+"D800000100,write\n0x0020000"+pmAddress+"D8FFFF00FF,write\n0x0030000"+pmAddress+"D800000000,write\n";
            return sequence;
        }
        else if(input=="CLEAR"){
            sequence = "reset\n0x002000000D8FFFF00FF,write\nread\n0x003000000D800000000,write\nread\n0x002000000D8FFFFFFFF,write\nread\n0x003000000D80000C800,write\nread\n0x002000000D8FFBF00FF,write\nread\n0x003000000D800000000,write\nread";
            const std::string prefixesPM[2] = {"PMA0", "PMC0"};
            const std::string addresses[2] = {"02", "16"};
            int arraySize = sizeof(prefixesPM)/sizeof(string);
            for(int i=0; i<arraySize; i++){
                sequence+="\n0x0020000"+addresses[i]+"D8FFFF00FF,write\nread\n0x0030000"+addresses[i]+"D800000000,write\nread\n0x0020000"+addresses[i]+"D8FFFFFFFF,write\nread\n0x0030000"+addresses[i]+"D80000C800,write\nread\n0x0020000"+addresses[i]+"D8FFBF00FF,write\nread\n0x0030000"+addresses[i]+"D800000000,write\nread";
            }
            sequence+="\n0x0010000000F00000004,write\nread";
            return sequence;
        }
        else if(input=="STATS_RESET"){
            noRpcRequest=true;
            return "";
        }
        else{
            this->publishError("Wrong input");
            noRpcRequest=true;
            return "";
        }
    }
}

string ResetErrors::processOutputMessage(string output) {
    if(output=="failure"){
        this->publishError("Error during clear errors");
        return "Error";
    }
    return "set";
}

