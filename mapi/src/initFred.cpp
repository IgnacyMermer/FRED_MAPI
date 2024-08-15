#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include "initFred.h"
#include "Alfred/print.h"
#include "Parser/utility.h"
#include "TCM_values.h"
#include <sstream>


InitFred::InitFred() : IndefiniteMapi::IndefiniteMapi()
{}

InitFred::~InitFred(){}

void InitFred::processExecution(){
    bool running;
    string response;

    string request = this->waitForRequest(running);

    std::string sequence = "reset\n";

    if (request == ""){
        sequence+="0x0020000000EFFFFFCFF,write\nread\n0x0030000000E00000200,write\nread\n";
        sequence+="0x0010000006600003711,write\nread\n";
        sequence+="0x0010000006400003790,write\nread\n";
        sequence+="0x001000000680000380F,write\nread\n";
        sequence+="0x001000000620000388E,write\nread\n";
        sequence+="0x001000000600000390D,write\nread";
        Print::PrintInfo(sequence);
        response = this->executeAlfSequence(sequence);
        this->publishAnswer(response);
    }
}

