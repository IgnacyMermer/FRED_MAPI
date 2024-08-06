#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include "default.h"
#include "Alfred/print.h"
#include "Parser/utility.h"
#include "TCM_values.h"
#include <sstream>
#include <iomanip>



Default::Default(std::string endpointParam) : IndefiniteMapi::IndefiniteMapi(){
    endpoint=endpointParam;
}

Default::~Default(){}

void Default::processExecution()
{
    bool running;
    string response;

    string request = this->waitForRequest(running);
    vector<string> parameters = Utility::splitString(request, ",");

    if (!running){
        return;
    }

    if (request == ""||(parameters.size()>1&&parameters[1]=="0")){
      int value = 0;
      if(endpoint=="VTIME_LOW"){
        value = tcm.temp.vtimeLow;
      }
      else if(endpoint=="VTIME_HIGH"){
        value = tcm.temp.vtimeHigh;
      }
      else if(endpoint=="SC_LEVEL_A"){
        value = tcm.temp.scLevelA;
      }
      else if(endpoint=="SC_LEVEL_C"){
        value = tcm.temp.scLevelC;
      }
      else if(endpoint=="C_LEVEL_A"){
        value = tcm.temp.cLevelA;
      }
      else if(endpoint=="C_LEVEL_C"){
        value = tcm.temp.cLevelC;
      }
      else if(endpoint=="TRIGGER1_CNT"){
        value = tcm.temp.trigger1cnt;
      }
      else if(endpoint=="TRIGGER2_CNT"){
        value = tcm.temp.trigger2cnt;
      }
      else if(endpoint=="TRIGGER3_CNT"){
        value = tcm.temp.trigger3cnt;
      }
      else if(endpoint=="TRIGGER4_CNT"){
        value = tcm.temp.trigger4cnt;
      }
      else if(endpoint=="TRIGGER5_CNT"){
        value = tcm.temp.trigger5cnt;
      }
      else if(endpoint=="MAIN_PANEL"){
        value = tcm.temp.mainPanelBits;
      }
      else if(endpoint=="DELAY_LASER"){
        value = tcm.temp.delayLaser;
      }
      this->publishAnswer(std::to_string(value));
    }

    else if (request == "error"){
        this->publishError("Error!");
    }
    
    else{

        std::string sequence = "", address="";

        if(endpoint=="VTIME_LOW"){
            address="00000008";
        }
        else if(endpoint=="VTIME_HIGH"){
            address="00000009";
        }
        else if(endpoint=="SC_LEVEL_A"){
            address="0000000A";
        }
        else if(endpoint=="SC_LEVEL_C"){
            address="0000000B";
        }
        else if(endpoint=="C_LEVEL_A"){
            address="0000000C";
        }
        else if(endpoint=="C_LEVEL_C"){
            address="0000000D";
        }
        
        if(parameters[1]=="1"){
            string data="";
            if(parameters[0].length()>2&&parameters[0].substr(0,2)=="0x"){
                parameters[0]=parameters[0].substr(2);
                data+=parameters[0];
                
                /*int num = std::stoi(data);

                std::stringstream ss;
                ss << std::hex << num;
                std::string hex_str = ss.str();
                for(int i=0; i<8-hex_str.length(); i++){
                    data+="0";
                }
                data+=hex_str;*/
                this->publishAnswer(data);
            }
            else{
                int num = std::stoi(parameters[0]);
                std::stringstream ss;
                ss << std::hex << num;
                std::string hex_str = ss.str();
                data+=hex_str;
                this->publishAnswer(data);
            }
            sequence = "reset\n0x001"+address+data+",write\nread";
        }
        else{
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }

        std::string response = this->executeAlfSequence(sequence);

        //response = this->executeAlfSequence("read\n0x00000170,0x80000000"); // execute desired sequence to alf, waits for response from ALF
        //this->publishAnswer(response);

        //response = this->executeAlfSequence("read\n0x00000180,0x80000000"); // It is possible to execute multiple sequences to ALF with one command from WinCC
        //this->publishAnswer(response);
    }
}

