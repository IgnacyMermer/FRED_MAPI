#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include "InitElectronics.h"
#include "Alfred/print.h"
#include "Parser/utility.h"
#include "tcmValues.h"
#include <sstream>
#include <cmath>
#include "swtCreator.h"


InitElectronics::InitElectronics(): IndefiniteMapi::IndefiniteMapi() {}

InitElectronics::~InitElectronics(){}


void InitElectronics::processExecution() {
    bool running;
    string response, sequence;

    string request = this->waitForRequest(running);

    if(!running){
        return;
    }

    if(request!="go"){
        this->publishError("Wrong input parameter");
    }
    else{
        sequence="reset\n";
        sequence+="0x0020000000EFFFFFCFF,write\nread\n0x0030000000E00000200,write\nread\n";
        sequence+="0x0010000006600003711,write\nread\n";
        sequence+="0x0010000006400003790,write\nread\n";
        sequence+="0x001000000680000380F,write\nread\n";
        sequence+="0x001000000620000388E,write\nread\n";
        sequence+="0x001000000600000390D,write\nread\n";
        sequence+="0x0010000001E000FFFFF,write\nread";

        this->executeAlfSequence(sequence);

        uint32_t pmMaskSpi=0, chMaskA=0, chMaskC=0;

        sequence = "reset\n0x0000000001E00000000,write\nread\n0x0000000001A00000000,write\nread\n0x0000000003A00000000,write\nread";
        response = this->executeAlfSequence(sequence);
        if(response=="failure"){
            this->publishError("Failure response in read");
            return;
        }
        response.erase(remove(response.begin(), response.end(), '\n'), response.end());
        pmMaskSpi = stoi(response.substr(13, 8), nullptr, 16);
        response = response.substr(22);
        chMaskA = stoi(response.substr(13, 8), nullptr, 16);
        response = response.substr(22);
        chMaskC = stoi(response.substr(13, 8), nullptr, 16);
        std::string tempStr;
        sequence="reset";
        for(int i=0; i<20; ++i){
            if(!(pmMaskSpi>>i & 1)){
                SwtCreator::sequenceOperationRMWOR(i, "0000001E", tempStr);
                sequence+=tempStr.substr(5);
            }
        }
        response = this->executeAlfSequence(sequence);
        sequence="reset";
        int pmNumber=0;
        for(int i=0; i<20; ++i){
            pmNumber+=0x2;
            try{
                response = this->executeAlfSequence("reset\n0x0000000"+SwtCreator::numberLetter(pmNumber)+"FE00000000,write\nread");
                if(response[response.length()-1]=='\n'){
                    response=response.substr(0, response.length()-1);
                }
                if(response=="failure"||(response.length()>10&&response.substr(response.length()-8)=="FFFFFFFF")){
                    SwtCreator::sequenceOperationRMWAND(i, "0000001E", tempStr);
                    sequence+=tempStr.substr(5);
                }
            }
            catch(exception& e){
                Print::PrintError("error index "+std::to_string(i));
            }
        }
        
        this->executeAlfSequence(sequence);

        sequence = "reset\n0x001000000D800100000,write\n0x001000000D900000040,write\n0x001000000DA00000000,write\n0x001000000DB00000000,write\n0x001000000DC00000000,write\n0x001000000DD00000000,write\n0x001000000DE00000000,write\n0x001000000DF00000000,write\n0x001000000E000000000,write\n0x001000000E100000000,write";
        sequence += "\n0x001000002D800100000,write\n0x001000002D900000040,write\n0x001000002DA00000000,write\n0x001000002DB00000000,write\n0x001000002DC00000000,write\n0x001000002DD00000000,write\n0x001000002DE00000000,write\n0x001000002DF00000000,write\n0x001000002E000000000,write\n0x001000002E100000000,write";
        sequence += "\n0x001000016D800100000,write\n0x001000016D900000040,write\n0x001000016DA00000000,write\n0x001000016DB00000000,write\n0x001000016DC00000000,write\n0x001000016DD00000000,write\n0x001000016DE00000000,write\n0x001000016DF00000000,write\n0x001000016E000000000,write\n0x001000016E100000000,write";
        response = this->executeAlfSequence(sequence);
        this->publishAnswer("updated");
    }
}