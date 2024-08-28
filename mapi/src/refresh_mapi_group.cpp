#include <iostream>
#include <string>
#include <algorithm>
#include <unistd.h>
#include "refresh_mapi_group.h"
#include "Parser/utility.h"
#include "Alfred/print.h"
#include "TCM_values.h"


RefreshMapiGroup::RefreshMapiGroup(Fred* fred){
    this->fred = fred;
    firstTime=true;
}

string RefreshMapiGroup::processInputMessage(string input){
    std::string sequence = "reset\n0x0000000000100000000,write\nread\n0x0000000000200000000,write\nread\n0x0000000000500000000,write\nread\n0x0000000000700000000,write\nread\n0x0000000000F00000000,write\nread\n0x0000000006A00000000,write\nread\n0x0000000006000000000,write\nread\n0x0000000006100000000,write\nread\n0x0000000006200000000,write\nread\n0x0000000006300000000,write\nread\n0x0000000006800000000,write\nread\n0x0000000006900000000,write\nread\n0x0000000006400000000,write\nread\n0x0000000006500000000,write\nread\n0x0000000006600000000,write\nread\n0x0000000006700000000,write\nread\n0x0000000007000000000,write\nread\n0x0000000007100000000,write\nread\n0x0000000007400000000,write\nread\n0x0000000007200000000,write\nread\n0x0000000007300000000,write\nread\n0x0000000001000000000,write\nread\n0x0000000001100000000,write\nread\n0x0000000001200000000,write\nread\n0x0000000001300000000,write\nread\n0x0000000001400000000,write\nread\n0x0000000001500000000,write\nread\n0x0000000001600000000,write\nread\n0x0000000001700000000,write\nread\n0x0000000001800000000,write\nread\n0x0000000001900000000,write\nread\n0x0000000003000000000,write\nread\n0x0000000003100000000,write\nread\n0x0000000003200000000,write\nread\n0x0000000003300000000,write\nread\n0x0000000003400000000,write\nread\n0x0000000003500000000,write\nread\n0x0000000003600000000,write\nread\n0x0000000003700000000,write\nread\n0x0000000003800000000,write\nread\n0x0000000003900000000,write\nread\n0x0000000000000000000,write\nread\n0x0000000000800000000,write\nread\n0x0000000000900000000,write\nread\n0x0000000000A00000000,write\nread\n0x0000000000B00000000,write\nread\n0x0000000000C00000000,write\nread\n0x0000000000D00000000,write\nread\n0x000000000E800000000,write\nread\n0x0000000001A00000000,write\nread\n0x0000000003A00000000,write\nread\n0x0000000005000000000,write\nread\n0x0000000007500000000,write\nread\n0x0000000007600000000,write\nread\n0x0000000007700000000,write\nread\n0x0000000007800000000,write\nread\n0x0000000007900000000,write\nread\n0x0000000007A00000000,write\nread\n0x0000000007B00000000,write\nread\n0x0000000007C00000000,write\nread\n0x0000000007D00000000,write\nread\n0x0000000007E00000000,write\nread\n0x000000000FC00000000,write\nread\n0x000000000FD00000000,write\nread\n0x000000000FE00000000,write\nread\n0x000000000D800000000,write\nread\n0x0000000001B00000000,write\nread\n0x0000000000400000000,write\nread\n0x0000000000300000000,write\nread\n0x0000000000E00000000,write\nread\n0x000000000E400000000,write\nread\n0x000000000E300000000,write\nread\n0x000000000D900000000,write\nread\n0x000000000E100000000,write\nread\n0x000000000DC00000000,write\nread\n0x000000000DD00000000,write\nread\n0x000000000E900000000,write\nread\n0x000000000EA00000000,write\nread\n0x000000000EB00000000,write\nread\n0x000000000EC00000000,write\nread\n0x000000000ED00000000,write\nread\n0x000000000EE00000000,write\nread\n0x000000000EF00000000,write\nread\n0x000000000F000000000,write\nread\n0x000000000F100000000,write\nread\n0x000000000F700000000,write\nread\n0x000000000FF00000000,write\nread\n0x000000000DA00000000,write\nread\n0x000000000DB00000000,write\nread\n0x000000000DC00000000,write\nread\n0x000000000DD00000000,write\nread\n0x000000000DE00000000,write\nread\n0x000000000DF00000000,write\nread";
    return sequence;
}

string RefreshMapiGroup::processOutputMessage(string output){
    vector< pair <string, string> > requests;
    std::string value;
    try{
        bool tgPattern1Changed = false;
        output.erase(remove(output.begin(), output.end(), '\n'), output.end());
        output = output.substr(8);
        int maxCount=400, count=0;
        bool firstIteration=true;
        if(firstTime){
            tcm.temp.triggerCounterTemp = 0;
            requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/WORK_STATUS", "0,0"));
        }
        firstTime=true;
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
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/DELAY_C", "0,0"));
            }
            break;
            case 1:
            if(tcm.temp.delayLaser!=hexValue||firstTime){
                tcm.temp.delayLaser = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/LASER_DELAY", "0,0"));
            }
            break;
            case 2:
            if(tcm.temp.boardTemp!=hexValue||firstTime){
                tcm.temp.boardTemp = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/TEMPERATURE", "0,0"));
            }
            break;
            case 3:
            if(tcm.temp.boardType!=hexValue||firstTime){
                tcm.temp.boardType = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/BOARD_TYPE", "0,0"));
            }
            break;
            case 4:
            if(tcm.temp.actualValues!=hexValue||firstTime){
                tcm.temp.actualValues = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/BOARD_STATUS", "0,0"));
            }
            break;
            case 5:
            if(tcm.temp.triggers!=hexValue||firstTime){
                Print::PrintInfo("sendCommand"+std::to_string(count));
                tcm.temp.triggers = hexValue;
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/TRIGGERS_OUTPUTS_MODE", "0,0"));
            }
            break;
            case 6:
            if(tcm.temp.trigger5sign!=hexValue||firstTime){
                tcm.temp.trigger5sign = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/TRG_ORA_SIGN", "0,0"));
            }
            break;
            case 7:
            if(tcm.temp.trigger5rand!=hexValue||firstTime){
                tcm.temp.trigger5rand = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/TRG_ORA_SIGN", "0,0"));
            }
            break;
            case 8:
            if(tcm.temp.trigger4sign!=hexValue||firstTime){
                tcm.temp.trigger4sign = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/TRG_ORC_SIGN", "0,0"));
            }
            break;
            case 9:
            if(tcm.temp.trigger4rand!=hexValue||firstTime){
                tcm.temp.trigger4rand = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/TRG_ORA_SIGN", "0,0"));
            }
            break;
            case 10:
            if(tcm.temp.trigger3sign!=hexValue||firstTime){
                tcm.temp.trigger3sign = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/TRG_V_SIGN", "0,0"));
            }
            break;
            case 11:
            if(tcm.temp.trigger3rand!=hexValue||firstTime){
                tcm.temp.trigger3rand = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/TRG_ORA_SIGN", "0,0"));
            }
            break;
            case 12:
            if(tcm.temp.trigger2sign!=hexValue||firstTime){
                tcm.temp.trigger2sign = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/TRG_SC_SIGN", "0,0"));
            }
            break;
            case 13:
            if(tcm.temp.trigger2rand!=hexValue||firstTime){
                tcm.temp.trigger2rand = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/TRG_ORA_SIGN", "0,0"));
            }
            break;
            case 14:
            if(tcm.temp.trigger1sign!=hexValue||firstTime){
                tcm.temp.trigger1sign = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/TRG_C_SIGN", "0,0"));
            }
            break;
            case 15:
            if(tcm.temp.trigger1rand!=hexValue||firstTime){
                tcm.temp.trigger1rand = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/TRG_ORA_SIGN", "0,0"));
            }
            break;
            case 16:
            if(tcm.temp.trigger5cnt!=hexValue||firstTime){
                float triggerRate = 0.0;
                if(firstTime){
                triggerRate = (hexValue - tcm.temp.trigger5cnt)/1.0;
                }
                else if(tcm.temp.countersUpdRate!=0){
                triggerRate = (hexValue - tcm.temp.trigger5cnt)/tcm.temp.countersUpdRate;
                }
                if(tcm.temp.trigger5rate!=triggerRate){
                //tcm.temp.trigger5rate = triggerRate;
                //requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/TRIGGER5_RATE", "0,0"));
                }
                //tcm.temp.trigger5cnt = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                //requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/TRIGGER5_CNT", "0,0"));
            }
            break;
            case 17:
            if(tcm.temp.trigger4cnt!=hexValue||firstTime){
                float triggerRate = 0.0;
                if(firstTime){
                //triggerRate = (hexValue - tcm.temp.trigger4cnt)/1.0;
                }
                else if(tcm.temp.countersUpdRate!=0){
                //triggerRate = (hexValue - tcm.temp.trigger4cnt)/tcm.temp.countersUpdRate;
                }
                if(tcm.temp.trigger4rate!=triggerRate){
                //tcm.temp.trigger4rate = triggerRate;
                //requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/TRIGGER4_RATE", "0,0"));
                }
                //tcm.temp.trigger4cnt = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                //requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/TRIGGER4_CNT", "0,0"));
            }
            break;
            case 18:
            if(tcm.temp.trigger3cnt!=hexValue||firstTime){
                float triggerRate;
                if(firstTime){
                triggerRate = (hexValue - tcm.temp.trigger3cnt)/1.0;
                }
                else if(tcm.temp.countersUpdRate!=0){
                triggerRate = (hexValue - tcm.temp.trigger3cnt)/tcm.temp.countersUpdRate;
                }
                if(tcm.temp.trigger3rate!=triggerRate){
                //tcm.temp.trigger3rate = triggerRate;
                //requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/TRIGGER3_RATE", "0,0"));
                }
                //tcm.temp.trigger3cnt = hexValue;
                //Print::PrintInfo("sendCommand"+std::to_string(count));
                //requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/TRIGGER3_CNT", "0,0"));
            }
            break;
            case 19:
            if(tcm.temp.trigger2cnt!=hexValue||firstTime){
                float triggerRate = 0.0;
                if(firstTime){
                triggerRate = (hexValue - tcm.temp.trigger2cnt)/1.0;
                }
                else if(tcm.temp.countersUpdRate!=0){
                triggerRate = (hexValue - tcm.temp.trigger2cnt)/tcm.temp.countersUpdRate;
                }
                if(tcm.temp.trigger2rate!=triggerRate){
                //tcm.temp.trigger2rate = triggerRate;
                //requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/TRIGGER2_RATE", "0,0"));
                }
                //tcm.temp.trigger2cnt = hexValue;
                //Print::PrintInfo("sendCommand"+std::to_string(count));
                //requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/TRIGGER2_CNT", "0,0"));
            }
            break;
            case 20:
            if(tcm.temp.trigger1cnt!=hexValue||firstTime){
                float triggerRate = 0.0;
                if(firstTime){
                triggerRate = (hexValue - tcm.temp.trigger1cnt)/1.0;
                }
                else if(tcm.temp.countersUpdRate!=0){
                //triggerRate = (hexValue - tcm.temp.trigger1cnt)/tcm.temp.countersUpdRate;
                auto now = std::chrono::high_resolution_clock::now();

                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
                
                int milliseconds_since_epoch = static_cast<int>(duration.count());

                triggerRate = (hexValue - tcm.temp.trigger1cnt)
                    /(milliseconds_since_epoch-tcm.temp.oldTimeTrigger1)*1000;

                //tcm.temp.oldTime = milliseconds_since_epoch;
                }
                if(tcm.temp.trigger1rate!=triggerRate){
                //tcm.temp.trigger1rate = triggerRate;
                //requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/TRIGGER1_RATE", "0,0"));
                }
                //tcm.temp.trigger1cnt = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                //requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/TRIGGER1_CNT", "0,0"));
            }
            break;
            case 21:
            if(tcm.temp.pmA0Status!=hexValue||firstTime){
                tcm.temp.pmA0Status = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/PM_LINK_A0", "0,0"));
            }
            break;
            case 22:
            if(tcm.temp.pmA1Status!=hexValue||firstTime){
                tcm.temp.pmA1Status = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/PM_LINK_A1", "0,0"));
            }
            break;
            case 23:
            if(tcm.temp.pmA2Status!=hexValue||firstTime){
                tcm.temp.pmA2Status = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/PM_LINK_A2", "0,0"));
            }
            break;
            case 24:
            if(tcm.temp.pmA3Status!=hexValue||firstTime){
                tcm.temp.pmA3Status = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/PM_LINK_A3", "0,0"));
            }
            break;
            case 25:
            if(tcm.temp.pmA4Status!=hexValue||firstTime){
                tcm.temp.pmA4Status = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/PM_LINK_A4", "0,0"));
            }
            break;
            case 26:
            if(tcm.temp.pmA5Status!=hexValue||firstTime){
                tcm.temp.pmA5Status = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/PM_LINK_A5", "0,0"));
            }
            break;
            case 27:
            if(tcm.temp.pmA6Status!=hexValue||firstTime){
                tcm.temp.pmA6Status = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/PM_LINK_A6", "0,0"));
            }
            break;
            case 28:
            if(tcm.temp.pmA7Status!=hexValue||firstTime){
                tcm.temp.pmA7Status = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/PM_LINK_A7", "0,0"));
            }
            break;
            case 29:
            if(tcm.temp.pmA8Status!=hexValue||firstTime){
                tcm.temp.pmA8Status = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/PM_LINK_A8", "0,0"));
            }
            break;
            case 30:
            if(tcm.temp.pmA9Status!=hexValue||firstTime){
                tcm.temp.pmA9Status = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/PM_LINK_A9", "0,0"));
            }
            break;
            case 31:
            if(tcm.temp.pmC0Status!=hexValue||firstTime){
                tcm.temp.pmC0Status = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/PM_LINK_C0", "0,0"));
            }
            break;
            case 32:
            if(tcm.temp.pmC1Status!=hexValue||firstTime){
                tcm.temp.pmC1Status = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/PM_LINK_C1", "0,0"));
            }
            break;
            case 33:
            if(tcm.temp.pmC2Status!=hexValue||firstTime){
                tcm.temp.pmC2Status = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/PM_LINK_C2", "0,0"));
            }
            break;
            case 34:
            if(tcm.temp.pmC3Status!=hexValue||firstTime){
                tcm.temp.pmC3Status = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/PM_LINK_C3", "0,0"));
            }
            break;
            case 35:
            if(tcm.temp.pmC4Status!=hexValue||firstTime){
                tcm.temp.pmC4Status = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/PM_LINK_C4", "0,0"));
            }
            break;
            case 36:
            if(tcm.temp.pmC5Status!=hexValue||firstTime){
                tcm.temp.pmC5Status = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/PM_LINK_C5", "0,0"));
            }
            break;
            case 37:
            if(tcm.temp.pmC6Status!=hexValue||firstTime){
                tcm.temp.pmC6Status = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/PM_LINK_C6", "0,0"));
            }
            break;
            case 38:
            if(tcm.temp.pmC7Status!=hexValue||firstTime){
                tcm.temp.pmC7Status = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/PM_LINK_C7", "0,0"));
            }
            break;
            case 39:
            if(tcm.temp.pmC8Status!=hexValue||firstTime){
                tcm.temp.pmC8Status = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/PM_LINK_C8", "0,0"));
            }
            break;
            case 40:
            if(tcm.temp.pmC9Status!=hexValue||firstTime){
                tcm.temp.pmC9Status = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/PM_LINK_C9", "0,0"));
            }
            break;
            case 41:
            if(tcm.temp.delayA!=hexValue||firstTime){
                tcm.temp.delayA = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/DELAY_A", "0,0"));
            }
            break;
            case 42:
            if(tcm.temp.vtimeLow!=hexValue||firstTime){
                tcm.temp.vtimeLow = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/VTIME_LOW", "0,0"));
            }
            break;
            case 43:
            if(tcm.temp.vtimeHigh!=hexValue||firstTime){
                tcm.temp.vtimeHigh = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/VTIME_HIGH", "0,0"));
            }
            break;
            case 44:
            if(tcm.temp.scLevelA!=hexValue||firstTime){
                tcm.temp.scLevelA = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/SC_LEVEL_A", "0,0"));
            }
            break;
            case 45:
            if(tcm.temp.scLevelC!=hexValue||firstTime){
                tcm.temp.scLevelC = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/SC_LEVEL_C", "0,0"));
            }
            break;
            case 46:
            if(tcm.temp.cLevelA!=hexValue||firstTime){
                tcm.temp.cLevelA = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/C_LEVEL_A", "0,0"));
            }
            break;
            case 47:
            if(tcm.temp.cLevelC!=hexValue||firstTime){
                tcm.temp.cLevelC = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/C_LEVEL_C", "0,0"));
            }
            break;
            case 48:
            if(tcm.temp.mainPanelBits!=hexValue||firstTime){
                tcm.temp.mainPanelBits = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/MODES_STATUS", "0,0"));
            }
            break;
            case 49:
            if(tcm.temp.sideAStatus!=hexValue||firstTime){
                tcm.temp.sideAStatus = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/SIDE_A_STATUS", "0,0"));
            }
            break;
            case 50:
            if(tcm.temp.sideCStatus!=hexValue||firstTime){
                tcm.temp.sideCStatus = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/SIDE_C_STATUS", "0,0"));
            }
            break;
            case 51:
            if(tcm.temp.countersUpdRate!=hexValue||firstTime){
                //tcm.temp.countersUpdRate = hexValue;
                tcm.temp.countersUpdRateValue = (hexValue==0?1.0:(hexValue==1?0.1:(hexValue==2?0.2:(hexValue==3?0.5:(hexValue==4?1:(hexValue==5?2:(hexValue==6?5:10)))))));
                if(firstTime&&tcm.temp.countersUpdRate!=0){
                    tcm.temp.trigger5rate = tcm.temp.trigger5cnt/tcm.temp.countersUpdRateValue;
                    tcm.temp.trigger4rate = tcm.temp.trigger4cnt/tcm.temp.countersUpdRateValue;
                    tcm.temp.trigger3rate = tcm.temp.trigger3cnt/tcm.temp.countersUpdRateValue;
                    tcm.temp.trigger2rate = tcm.temp.trigger2cnt/tcm.temp.countersUpdRateValue;
                    tcm.temp.trigger1rate = tcm.temp.trigger1cnt/tcm.temp.countersUpdRateValue;
                }
                else{
                    tcm.temp.trigger5rate = 0;
                    tcm.temp.trigger4rate = 0;
                    tcm.temp.trigger3rate = 0;
                    tcm.temp.trigger2rate = 0;
                    tcm.temp.trigger1rate = 0;
                }
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/COUNTERS_UPD_RATE", "0,0"));
            }
            break;
            case 52:
            if(tcm.temp.bkgrnd0Cnt!=hexValue||firstTime){
                tcm.temp.bkgrnd0Cnt = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/BKGRND0_CNT", "0,0"));
            }
            break;
            case 53:
            if(tcm.temp.bkgrnd1Cnt!=hexValue||firstTime){
                tcm.temp.bkgrnd1Cnt = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/BKGRND1_CNT", "0,0"));
            }
            break;
            case 54:
            if(tcm.temp.bkgrnd2Cnt!=hexValue||firstTime){
                tcm.temp.bkgrnd2Cnt = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/BKGRND2_CNT", "0,0"));
            }
            break;
            case 55:
            if(tcm.temp.bkgrnd3Cnt!=hexValue||firstTime){
                tcm.temp.bkgrnd3Cnt = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/BKGRND3_CNT", "0,0"));
            }
            break;
            case 56:
            if(tcm.temp.bkgrnd4Cnt!=hexValue||firstTime){
                tcm.temp.bkgrnd4Cnt = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/BKGRND4_CNT", "0,0"));
            }
            break;
            case 57:
            if(tcm.temp.bkgrnd5Cnt!=hexValue||firstTime){
                tcm.temp.bkgrnd5Cnt = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/BKGRND5_CNT", "0,0"));
            }
            break;
            case 58:
            if(tcm.temp.bkgrnd6Cnt!=hexValue||firstTime){
                tcm.temp.bkgrnd6Cnt = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/BKGRND6_CNT", "0,0"));
            }
            break;
            case 59:
            if(tcm.temp.bkgrnd7Cnt!=hexValue||firstTime){
                tcm.temp.bkgrnd7Cnt = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/BKGRND7_CNT", "0,0"));
            }
            break;
            case 60:
            if(tcm.temp.bkgrnd8Cnt!=hexValue||firstTime){
                tcm.temp.bkgrnd8Cnt = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/BKGRND8_CNT", "0,0"));
            }
            break;
            case 61:
            if(tcm.temp.bkgrnd9Cnt!=hexValue||firstTime){
                tcm.temp.bkgrnd9Cnt = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/BKGRND9_CNT", "0,0"));
            }
            break;
            case 62:
            if(tcm.temp.fpgaTemp!=hexValue||firstTime){
                tcm.temp.fpgaTemp = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/FPGA_TEMP", "0,0"));
            }
            break;
            case 63:
            if(tcm.temp.vPower1!=hexValue||firstTime){
                tcm.temp.vPower1 = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/1VPOWER", "0,0"));
            }
            break;
            case 64:
            if(tcm.temp.vPower18!=hexValue||firstTime){
                tcm.temp.vPower18 = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/18VPOWER", "0,0"));
            }
            break;
            case 65:
            if(tcm.temp.modeSettings!=hexValue||firstTime){
                tcm.temp.modeSettings = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/MODE_SETTINGS", "0,0"));
            }
            break;
            case 66:
            if(tcm.temp.laserDivider!=hexValue||firstTime){
                tcm.temp.laserDivider = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/LASER_DIVIDER", "0,0"));
            }
            break;
            case 67:
            if(tcm.temp.extSwitches!=hexValue||firstTime){
                tcm.temp.extSwitches = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/EXT_SW", "0,0"));
            }
            break;
            case 68:
            if(tcm.temp.attenuator!=hexValue||firstTime){
                tcm.temp.attenuator = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/ATTENUATOR", "0,0"));
            }
            break;
            case 69:
            if(tcm.temp.triggerModes!=hexValue||firstTime){
                tcm.temp.triggerModes = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/MODE", "0,0"));
            }
            break;
            case 70:
            if(tcm.temp.dsTriggerMask!=hexValue||firstTime){
                tcm.temp.dsTriggerMask = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/DATA_SEL_TRG_MASK", "0,0"));
            }
            break;
            case 71:
            if(tcm.temp.bcidOffset!=hexValue||firstTime){
                tcm.temp.bcidOffset = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/BCID_OFFSET", "0,0"));
            }
            break;
            case 72:
            if(tcm.temp.dgTriggerMask!=hexValue||firstTime){
                tcm.temp.dgTriggerMask = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/DG_TRG_RESPOND_MASK", "0,0"));
            }
            break;
            case 73:
            if(tcm.temp.systemId!=hexValue||firstTime){
                tcm.temp.systemId = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/RDH_FIELDS", "0,0"));
            }
            break;
            case 74:
            if(tcm.temp.trgPatternLSB!=hexValue||firstTime){
                tcm.temp.trgPatternLSB = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/TG_PATTERN_1", "0,0"));
            }
            break;
            case 75:
            if(tcm.temp.trgPatternMSB!=hexValue||firstTime){
                tcm.temp.trgPatternMSB = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/TG_PATTERN_0", "0,0"));
            }
            break;
            case 76:
            if(tcm.temp.cruOrbit!=hexValue||firstTime){
                tcm.temp.cruOrbit = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/CRU_ORBIT", "0,0"));
            }
            break;
            case 77:
            if(tcm.temp.cruBcFifos!=hexValue||firstTime){
                tcm.temp.cruBcFifos = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/CRU_BC", "0,0"));
            }
            break;
            case 78:
            if(tcm.temp.fifoCount!=hexValue||firstTime){
                tcm.temp.fifoCount = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/FIFO_COUNT", "0,0"));
            }
            break;
            case 79:
            if(tcm.temp.selFirstHitDroppedOrbit!=hexValue||firstTime){
                tcm.temp.selFirstHitDroppedOrbit = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/SEL_FIRST_HIT_DROPPED_ORBIT", "0,0"));
            }
            break;
            case 80:
            if(tcm.temp.selLastHitDroppedOrbit!=hexValue||firstTime){
                tcm.temp.selLastHitDroppedOrbit = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/SEL_LAST_HIT_DROPPED_ORBIT", "0,0"));
            }
            break;
            case 81:
            if(tcm.temp.selHitsDropped!=hexValue||firstTime){
                tcm.temp.selHitsDropped = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/SEL_HITS_DROPPED", "0,0"));
            }
            break;
            case 82:
            if(tcm.temp.readoutRate!=hexValue||firstTime){
                tcm.temp.readoutRate = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/READOUT_RATE", "0,0"));
            }
            break;
            case 83:
            if(tcm.temp.IPbusFIFOdata!=hexValue||firstTime){
                tcm.temp.IPbusFIFOdata = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/IPbus_FIFO_DATA", "0,0"));
            }
            break;
            case 84:
            if(tcm.temp.eventsCount!=hexValue||firstTime){
                tcm.temp.eventsCount = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/EVENTS_COUNT", "0,0"));
            }
            break;
            case 85:
            if(tcm.temp.mcodeTime!=hexValue||firstTime){
                tcm.temp.mcodeTime = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/MCODE_TIME", "0,0"));
            }
            break;
            case 86:
            if(tcm.temp.fwTime!=hexValue||firstTime){
                tcm.temp.fwTime = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/FW_TIME", "0,0"));
            }
            break;
            case 87:
            if(tcm.temp.dgBunchPattern!=hexValue||firstTime){
                tcm.temp.dgBunchPattern = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/DG_BUNCH_PATTERN", "0,0"));
            }
            break;
            case 88:
            if(tcm.temp.tgSingleValue!=hexValue||firstTime){
                tcm.temp.tgSingleValue = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/TG_SINGLE_VALUE", "0,0"));
            }
            break;
            case 89:
            if(tcm.temp.tgPattern1!=hexValue||firstTime){
                tgPattern1Changed=true;
                tcm.temp.tgPattern1 = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
            }
            break;
            case 90:
            if(tcm.temp.tgPattern0!=hexValue||firstTime||tgPattern1Changed){
                tcm.temp.tgPattern0 = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/TG_PATTERN_0", "0,0")); //do implementacji
            }
            break;
            case 91:
            if(tcm.temp.tgContValue!=hexValue||firstTime){
                tcm.temp.tgContValue = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/TG_CONT_VALUE", "0,0"));
            }
            break;
            case 92:
            if(tcm.temp.emulationRate!=hexValue||firstTime){
                tcm.temp.emulationRate = hexValue;
                Print::PrintInfo("sendCommand"+std::to_string(count));
                requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/EMULATION_RATE", "0,0"));
            }
            break;
        }
        count+=1;
        }
        tcm.temp.triggerCounterTemp++;
        firstTime=false;
        newMapiGroupRequest(requests);
        Print::PrintInfo("mapi group request");
        //usleep(1000000);
        //sleep(1);
        //std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        //newRequest(sequence);

    }
    catch (exception &e) {
        Print::PrintInfo(e.what());
        Print::PrintError("ERROR in connection with ALF!");
    }

    return to_string(0);
    
    //Print::PrintInfo(output);
	//return output;
}
