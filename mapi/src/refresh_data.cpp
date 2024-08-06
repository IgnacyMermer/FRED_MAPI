#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include "refresh_data.h"
#include "Alfred/print.h"
#include "Parser/utility.h"
#include "TCM_values.h"
#include <thread>
#include <chrono>
#include "../../core/include/dim/dic.hxx"
#include "../../core/include/FREDServer/Fred/Mapi/iterativemapi.h"


RefreshData::RefreshData() {
  finalValue = 0;
  firstTime=true;
}

string RefreshData::processInputMessage(string input) {

  sequence = "reset\n0x0000000000100000000,write\nread\n0x0000000000200000000,write\nread\n0x0000000000500000000,write\nread\n0x0000000000700000000,write\nread\n0x0000000000F00000000,write\nread\n0x0000000006A00000000,write\nread\n0x0000000006000000000,write\nread\n0x0000000006100000000,write\nread\n0x0000000006200000000,write\nread\n0x0000000006300000000,write\nread\n0x0000000006800000000,write\nread\n0x0000000006900000000,write\nread\n0x0000000006400000000,write\nread\n0x0000000006500000000,write\nread\n0x0000000006600000000,write\nread\n0x0000000006700000000,write\nread\n0x0000000007000000000,write\nread\n0x0000000007100000000,write\nread\n0x0000000007400000000,write\nread\n0x0000000007200000000,write\nread\n0x0000000007300000000,write\nread\n0x0000000001000000000,write\nread\n0x0000000001100000000,write\nread\n0x0000000001200000000,write\nread\n0x0000000001300000000,write\nread\n0x0000000001400000000,write\nread\n0x0000000001500000000,write\nread\n0x0000000001600000000,write\nread\n0x0000000001700000000,write\nread\n0x0000000001800000000,write\nread\n0x0000000001900000000,write\nread\n0x0000000003000000000,write\nread\n0x0000000003100000000,write\nread\n0x0000000003200000000,write\nread\n0x0000000003300000000,write\nread\n0x0000000003400000000,write\nread\n0x0000000003500000000,write\nread\n0x0000000003600000000,write\nread\n0x0000000003700000000,write\nread\n0x0000000003800000000,write\nread\n0x0000000003900000000,write\nread\n0x0000000000000000000,write\nread\n0x0000000000800000000,write\nread\n0x0000000000900000000,write\nread\n0x0000000000A00000000,write\nread\n0x0000000000B00000000,write\nread\n0x0000000000C00000000,write\nread\n0x0000000000D00000000,write\nread\n0x000000000E800000000,write\nread";
  return sequence;
}

string RefreshData::processOutputMessage(string output) {
  string value;

  try {
    output.erase(remove(output.begin(), output.end(), '\n'), output.end());
    output = output.substr(8);
    int maxCount=400, count=0;
    bool firstIteration=true;
    if(firstTime){
      DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/WORK_STATUS_REQ").c_str(), std::string("").c_str());
    }
    while(output.length()>0&&count<maxCount){
        if(!firstIteration){
          output=output.substr(1);
        }
        firstIteration=false;
        
        value = output.substr(13, 8);
        output = output.substr(21);
        long long hexValue = stol(value, nullptr, 16);
        switch(count){
          case 0:
            if(tcm.temp.delayC!=hexValue||firstTime){
              tcm.temp.delayC = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/DELAY_C_REQ").c_str(), std::string("").c_str());
            }
            break;
          case 1:
            if(tcm.temp.delayLaser!=hexValue||firstTime){
              tcm.temp.delayLaser = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/DELAY_LASER_REQ").c_str(), std::string(std::to_string(hexValue)+",1").c_str());
            }
            break;
          case 2:
            if(tcm.temp.boardTemp!=hexValue||firstTime){
              tcm.temp.boardTemp = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/BOARD_TEMP_REQ").c_str(), std::string("").c_str());
            }
            break;
          case 3:
            if(tcm.temp.boardType!=hexValue||firstTime){
              tcm.temp.boardType = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/BOARD_TYPE_REQ").c_str(), std::string("").c_str());
            }
            break;
          case 4:
            if(tcm.temp.actualValues!=hexValue||firstTime){
              tcm.temp.actualValues = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/ACTUAL_VALUES_REQ").c_str(), std::string("").c_str());
            }
            break;
          case 5:
            if(tcm.temp.triggers!=hexValue||firstTime){
              Print::PrintInfo("sendCommand"+std::to_string(count));
              tcm.temp.triggers = hexValue;
              DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/TRIGGERS_REQ").c_str(), std::string("").c_str());
            }
            break;
          case 6:
            if(tcm.temp.trigger5sign!=hexValue||firstTime){
              tcm.temp.trigger5sign = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/TRIGGER5_SIGN_REQ").c_str(), std::string("").c_str());
            }
            break;
          case 7:
            if(tcm.temp.trigger5rand!=hexValue||firstTime){
              tcm.temp.trigger5rand = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/TRIGGER5_RAND_REQ").c_str(), "");
            }
            break;
          case 8:
            if(tcm.temp.trigger4sign!=hexValue||firstTime){
              tcm.temp.trigger4sign = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/TRIGGER4_SIGN_REQ").c_str(), std::string("").c_str());
            }
            break;
          case 9:
            if(tcm.temp.trigger4rand!=hexValue||firstTime){
              tcm.temp.trigger4rand = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/TRIGGER4_RAND_REQ").c_str(), "");
            }
            break;
          case 10:
            if(tcm.temp.trigger3sign!=hexValue||firstTime){
              tcm.temp.trigger3sign = hexValue;
              Print::PrintInfo("sign");
              Print::PrintInfo(output);
              Print::PrintInfo(std::to_string(hexValue));
              Print::PrintInfo("sendCommand"+std::to_string(count));
              DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/TRIGGER3_SIGN_REQ").c_str(), std::string("").c_str());
            }
            break;
          case 11:
            if(tcm.temp.trigger3rand!=hexValue||firstTime){
              tcm.temp.trigger3rand = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/TRIGGER3_RAND_REQ").c_str(), "");
            }
            break;
          case 12:
            if(tcm.temp.trigger2sign!=hexValue||firstTime){
              tcm.temp.trigger2sign = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/TRIGGER2_SIGN_REQ").c_str(), std::string("").c_str());
            }
            break;
          case 13:
            if(tcm.temp.trigger2rand!=hexValue||firstTime){
              tcm.temp.trigger2rand = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/TRIGGER2_RAND_REQ").c_str(), "");
            }
            break;
          case 14:
            if(tcm.temp.trigger1sign!=hexValue||firstTime){
              tcm.temp.trigger1sign = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/TRIGGER1_SIGN_REQ").c_str(), std::string("").c_str());
            }
            break;
          case 15:
            if(tcm.temp.trigger1rand!=hexValue||firstTime){
              tcm.temp.trigger1rand = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/TRIGGER1_RAND_REQ").c_str(), "");
            }
            break;
          case 16:
            if(tcm.temp.trigger5cnt!=hexValue||firstTime){
              tcm.temp.trigger5cnt = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/TRIGGER5_CNT_REQ").c_str(), "");
            }
            break;
          case 17:
            if(tcm.temp.trigger4cnt!=hexValue||firstTime){
              tcm.temp.trigger4cnt = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/TRIGGER4_CNT_REQ").c_str(), "");
            }
            break;
          case 18:
            if(tcm.temp.trigger3cnt!=hexValue||firstTime){
              tcm.temp.trigger3cnt = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/TRIGGER3_CNT_REQ").c_str(), "");
            }
            break;
          case 19:
            if(tcm.temp.trigger2cnt!=hexValue||firstTime){
              tcm.temp.trigger2cnt = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/TRIGGER2_CNT_REQ").c_str(), "");
            }
            break;
          case 20:
            if(tcm.temp.trigger1cnt!=hexValue||firstTime){
              tcm.temp.trigger1cnt = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/TRIGGER1_CNT_REQ").c_str(), "");
            }
            break;
          case 21:
            if(tcm.temp.pmA0Status!=hexValue||firstTime){
              tcm.temp.pmA0Status = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/PM0A_REQ").c_str(), "");
            }
            break;
          case 22:
            if(tcm.temp.pmA1Status!=hexValue||firstTime){
              tcm.temp.pmA1Status = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/PM1A_REQ").c_str(), "");
            }
            break;
          case 23:
            if(tcm.temp.pmA2Status!=hexValue||firstTime){
              tcm.temp.pmA2Status = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/PM2A_REQ").c_str(), "");
            }
            break;
          case 24:
            if(tcm.temp.pmA3Status!=hexValue||firstTime){
              tcm.temp.pmA3Status = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/PM3A_REQ").c_str(), "");
            }
            break;
          case 25:
            if(tcm.temp.pmA4Status!=hexValue||firstTime){
              tcm.temp.pmA4Status = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/PM4A_REQ").c_str(), "");
            }
            break;
          case 26:
            if(tcm.temp.pmA5Status!=hexValue||firstTime){
              tcm.temp.pmA5Status = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/PM5A_REQ").c_str(), "");
            }
            break;
          case 27:
            if(tcm.temp.pmA6Status!=hexValue||firstTime){
              tcm.temp.pmA6Status = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/PM6A_REQ").c_str(), "");
            }
            break;
          case 28:
            if(tcm.temp.pmA7Status!=hexValue||firstTime){
              tcm.temp.pmA7Status = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/PM7A_REQ").c_str(), "");
            }
            break;
          case 29:
            if(tcm.temp.pmA8Status!=hexValue||firstTime){
              tcm.temp.pmA8Status = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/PM8A_REQ").c_str(), "");
            }
            break;
          case 30:
            if(tcm.temp.pmA9Status!=hexValue||firstTime){
              tcm.temp.pmA9Status = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/PM9A_REQ").c_str(), "");
            }
            break;
          case 31:
            if(tcm.temp.pmC0Status!=hexValue||firstTime){
              tcm.temp.pmC0Status = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/PM0C_REQ").c_str(), "");
            }
            break;
          case 32:
            if(tcm.temp.pmC1Status!=hexValue||firstTime){
              tcm.temp.pmC1Status = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/PM1C_REQ").c_str(), "");
            }
            break;
          case 33:
            if(tcm.temp.pmC2Status!=hexValue||firstTime){
              tcm.temp.pmC2Status = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/PM2C_REQ").c_str(), "");
            }
            break;
          case 34:
            if(tcm.temp.pmC3Status!=hexValue||firstTime){
              tcm.temp.pmC3Status = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/PM3C_REQ").c_str(), "");
            }
            break;
          case 35:
            if(tcm.temp.pmC4Status!=hexValue||firstTime){
              tcm.temp.pmC4Status = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/PM4C_REQ").c_str(), "");
            }
            break;
          case 36:
            if(tcm.temp.pmC5Status!=hexValue||firstTime){
              tcm.temp.pmC5Status = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/PM5C_REQ").c_str(), "");
            }
            break;
          case 37:
            if(tcm.temp.pmC6Status!=hexValue||firstTime){
              tcm.temp.pmC6Status = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/PM6C_REQ").c_str(), "");
            }
            break;
          case 38:
            if(tcm.temp.pmC7Status!=hexValue||firstTime){
              tcm.temp.pmC7Status = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/PM7C_REQ").c_str(), "");
            }
            break;
          case 39:
            if(tcm.temp.pmC8Status!=hexValue||firstTime){
              tcm.temp.pmC8Status = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/PM8C_REQ").c_str(), "");
            }
            break;
          case 40:
            if(tcm.temp.pmC9Status!=hexValue||firstTime){
              tcm.temp.pmC9Status = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/PM9C_REQ").c_str(), "");
            }
            break;
          case 41:
            if(tcm.temp.delayA!=hexValue||firstTime){
              tcm.temp.delayA = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/DELAY_A_REQ").c_str(), "");
            }
            break;
          case 42:
            if(tcm.temp.vtimeLow!=hexValue||firstTime){
              tcm.temp.vtimeLow = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/VTIME_LOW_REQ").c_str(), "");
            }
            break;
          case 43:
            if(tcm.temp.vtimeHigh!=hexValue||firstTime){
              tcm.temp.vtimeHigh = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/VTIME_HIGH_REQ").c_str(), "");
            }
            break;
          case 44:
            if(tcm.temp.scLevelA!=hexValue||firstTime){
              tcm.temp.scLevelA = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/SC_LEVEL_A_REQ").c_str(), "");
            }
            break;
          case 45:
            if(tcm.temp.scLevelC!=hexValue||firstTime){
              tcm.temp.scLevelC = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/SC_LEVEL_C_REQ").c_str(), "");
            }
            break;
          case 46:
            if(tcm.temp.cLevelA!=hexValue||firstTime){
              tcm.temp.cLevelA = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/C_LEVEL_A_REQ").c_str(), "");
            }
            break;
          case 47:
            if(tcm.temp.cLevelC!=hexValue||firstTime){
              tcm.temp.cLevelC = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/C_LEVEL_C_REQ").c_str(), "");
            }
            break;
          case 48:
            if(tcm.temp.mainPanelBits!=hexValue||firstTime){
              tcm.temp.mainPanelBits = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              DimClient::sendCommand(std::string("FIT_FRED/READOUTCARDS/FIT0/MAIN_PANEL_REQ").c_str(), "");
            }
            break;
        }
        count+=1;
    } 
    firstTime=false;
    sleep(1);
    newRequest(sequence);

  }
  catch (exception &e) {
    Print::PrintInfo(e.what());
    Print::PrintError("ERROR in connection with ALF!");
  }

  return to_string(0);
}
