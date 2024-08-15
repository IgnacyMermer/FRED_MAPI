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

    if (request == ""||request == "set"||(parameters.size()>1&&parameters[1]=="0")){
      Print::PrintInfo(endpoint);
      long long value = 0;
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
      else if(endpoint=="MODES_STATUS"){
        value = tcm.temp.mainPanelBits;
      }
      else if(endpoint=="LASER_DELAY"){
        value = tcm.temp.delayLaser;
      }
      else if(endpoint=="SIDE_A_STATUS"){
        value = tcm.temp.sideAStatus;
      }
      else if(endpoint=="SIDE_C_STATUS"){
        value = tcm.temp.sideCStatus;
      }
      else if(endpoint=="TRG_C_RATE"){
        value = tcm.temp.trigger1rate;
      }
      else if(endpoint=="TRG_SC_RATE"){
        value = tcm.temp.trigger2rate;
      }
      else if(endpoint=="TRG_V_RATE"){
        value = tcm.temp.trigger3rate;
      }
      else if(endpoint=="TRG_ORC_RATE"){
        value = tcm.temp.trigger4rate;
      }
      else if(endpoint=="TRG_ORA_RATE"){
        value = tcm.temp.trigger5rate;
      }
      else if(endpoint=="BKGRND0_CNT"){
        value = tcm.temp.bkgrnd0Cnt;
      }
      else if(endpoint=="BKGRND1_CNT"){
        value = tcm.temp.bkgrnd1Cnt;
      }
      else if(endpoint=="BKGRND2_CNT"){
        value = tcm.temp.bkgrnd2Cnt;
      }
      else if(endpoint=="BKGRND3_CNT"){
        value = tcm.temp.bkgrnd3Cnt;
      }
      else if(endpoint=="BKGRND4_CNT"){
        value = tcm.temp.bkgrnd4Cnt;
      }
      else if(endpoint=="BKGRND5_CNT"){
        value = tcm.temp.bkgrnd5Cnt;
      }
      else if(endpoint=="BKGRND6_CNT"){
        value = tcm.temp.bkgrnd6Cnt;
      }
      else if(endpoint=="BKGRND7_CNT"){
        value = tcm.temp.bkgrnd7Cnt;
      }
      else if(endpoint=="BKGRND8_CNT"){
        value = tcm.temp.bkgrnd8Cnt;
      }
      else if(endpoint=="BKGRND9_CNT"){
        value = tcm.temp.bkgrnd9Cnt;
      }
      else if(endpoint=="MODE_SETTINGS"){
        value = tcm.temp.modeSettings;
      }
      else if(endpoint=="LASER_DIVIDER"){
        value = tcm.temp.laserDivider;
      }
      else if(endpoint=="EXT_SW"){
        value = tcm.temp.extSwitches;
      }
      else if(endpoint=="ATTENUATOR"){
        value = tcm.temp.attenuator;
      }
      else if(endpoint=="MODE"){
        value = tcm.temp.triggerModes;
      }
      else if(endpoint=="DATA_SEL_TRG_MASK"){
        value = tcm.temp.dsTriggerMask;
      }
      else if(endpoint=="BCID_OFFSET"){
        value = tcm.temp.bcidOffset;
      }
      else if(endpoint=="DG_TRG_RESPOND_MASK"){
        value = tcm.temp.dgTriggerMask;
      }
      else if(endpoint=="RDH_FIELDS"){
        value = tcm.temp.systemId;
      }
      else if(endpoint=="TRG_CONT_PATTERN_LSB"){
        value = tcm.temp.trgPatternLSB;
      }
      else if(endpoint=="TRG_CONT_PATTERN_MSB"){
        value = tcm.temp.trgPatternMSB;
      }
      else if(endpoint=="CRU_ORBIT"){
        value = tcm.temp.cruOrbit;
      }
      else if(endpoint=="CRU_BC"){
        value = tcm.temp.cruBcFifos;
      }
      else if(endpoint=="FIFO_COUNT"){
        value = tcm.temp.fifoCount;
      }
      else if(endpoint=="SEL_FIRST_HIT_DROPPED_ORBIT"){
        value = tcm.temp.selFirstHitDroppedOrbit;
      }
      else if(endpoint=="SEL_LAST_HIT_DROPPED_ORBIT"){
        value = tcm.temp.selLastHitDroppedOrbit;
      }
      else if(endpoint=="SEL_HITS_DROPPED"){
        value = tcm.temp.selHitsDropped;
      }
      else if(endpoint=="READOUT_RATE"){
        value = tcm.temp.readoutRate;
      }
      else if(endpoint=="IPbus_FIFO_DATA"){
        value = tcm.temp.IPbusFIFOdata;
      }
      else if(endpoint=="EVENTS_COUNT"){
        value = tcm.temp.eventsCount;
      }
      else if(endpoint=="MCODE_TIME"){
        value = tcm.temp.mcodeTime;
      }
      else if(endpoint=="FW_TIME"){
        value = tcm.temp.fwTime;
      }
      else if(endpoint=="COUNTERS_UPD_RATE"){
        value = tcm.temp.countersUpdRate; 
      }
      else if(endpoint=="FPGA_TEMP"){
        this->publishAnswer(std::to_string(tcm.temp.fpgaTemp* 503.975 / 65536 - 273.15));
      }
      else if(endpoint=="1VPOWER"){
        this->publishAnswer(std::to_string(tcm.temp.vPower1*3/65536.0));
      }
      else if(endpoint=="18VPOWER"){
        this->publishAnswer(std::to_string(tcm.temp.vPower18*3/65536.0));
      }
      if(endpoint!="FPGA_TEMP"&&endpoint!="1VPOWER"&&endpoint!="18VPOWER"){
        this->publishAnswer(std::to_string(value));
      }
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
            int num = std::stoi(parameters[0]);
            if(endpoint=="SIDE_A_STATUS"){
              if(parameters.size()>2&&parameters[2]=="set"){
                std::stringstream ss;
                ss << std::hex << (1 << num);
                std::string hex_str = ss.str();
                data="";
                for(int i=0; i<8-hex_str.length(); i++){
                  data+="0";
                }
                data+=hex_str;
                sequence = "reset\n0x0020000001AFFFFFFFF,write\nread\n0x0030000001A"+data+",write\nread";
              }
              else if(parameters[2]=="clear"){
                int temp = 0xFFFFFFFF;
                temp-=(1 << num);
                std::stringstream ss;
                ss << std::hex << temp;
                data = ss.str();
                sequence = "reset\n0x0020000001A"+data+",write\nread\n0x0030000001A00000000,write\nread";
              }
            }
            else if(endpoint=="SIDE_C_STATUS"){
              if(parameters.size()>2&&parameters[2]=="set"){
                std::stringstream ss;
                ss << std::hex << (1 << num);
                std::string hex_str = ss.str();
                data="";
                for(int i=0; i<8-hex_str.length(); i++){
                  data+="0";
                }
                data+=hex_str;
                sequence = "reset\n0x0020000003AFFFFFFFF,write\nread\n0x0030000003A"+data+",write\nread";
              }
              else if(parameters[2]=="clear"){
                int temp = 0xFFFFFFFF;
                temp-=(1 << num);
                std::stringstream ss;
                ss << std::hex << temp;
                data = ss.str();
                sequence = "reset\n0x0020000003A"+data+",write\nread\n0x0030000003A00000000,write\nread";
              }
            }
            else if(endpoint=="MODE"){
              if(parameters.size()>2&&parameters[2]=="set"){
                std::stringstream ss;
                ss << std::hex << (1 << num);
                std::string hex_str = ss.str();
                data="";
                for(int i=0; i<8-hex_str.length(); i++){
                  data+="0";
                }
                data+=hex_str;
                sequence = "reset\n0x0020000000EFFFFFFFF,write\nread\n0x0030000000E"+data+",write\nread";
              }
              else if(parameters[2]=="clear"){
                int temp = 0xFFFFFFFF;
                temp-=(1 << num);
                std::stringstream ss;
                ss << std::hex << temp;
                data = ss.str();
                sequence = "reset\n0x0020000000E"+data+",write\nread\n0x0030000000E00000000,write\nread";
              }
              Print::PrintInfo(sequence);
            }
            else{
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
                  for(int i=0; i<8-hex_str.length(); i++){
                      data+="0";
                  }
                  data+=hex_str;
                  this->publishAnswer(data);
              }
              sequence = "reset\n0x001"+address+data+",write\nread";
          }
          //this->publishAnswer(data);
          if(endpoint!="SIDE_A_STATUS"&&endpoint!="SIDE_C_STATUS"&&endpoint!="MODE"){
            this->publishAnswer(data);
            sequence = "reset\n0x001"+address+data+",write\nread"; 
          }
        }
        else{
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }

        Print::PrintInfo("sequence:");
        Print::PrintInfo(sequence);

        std::string response = this->executeAlfSequence(sequence);

        //response = this->executeAlfSequence("read\n0x00000170,0x80000000"); // execute desired sequence to alf, waits for response from ALF
        //this->publishAnswer(response);

        //response = this->executeAlfSequence("read\n0x00000180,0x80000000"); // It is possible to execute multiple sequences to ALF with one command from WinCC
        //this->publishAnswer(response);
    }
}

