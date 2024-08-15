#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include "refresh_counters.h"
#include "Alfred/print.h"
#include "Parser/utility.h"
#include "TCM_values.h"
#include <thread>
#include <chrono>
#include "../../core/include/dim/dic.hxx"
#include "../../core/include/FREDServer/Fred/Mapi/iterativemapi.h"
#include <stdlib.h>


RefreshCounters::RefreshCounters() {
  finalValue = 0;
  firstTime=true;
}

string RefreshCounters::processInputMessage(string input) {

  sequence = "reset\n0x0000000007000000000,write\nread\n0x0000000007100000000,write\nread\n0x0000000007200000000,write\nread\n0x0000000007300000000,write\nread\n0x0000000007400000000,write\nread\n0x0000000007500000000,write\nread\n0x0000000007600000000,write\nread\n0x0000000007700000000,write\nread\n0x0000000007800000000,write\nread\n0x0000000007900000000,write\nread\n0x0000000007A00000000,write\nread\n0x0000000007B00000000,write\nread\n0x0000000007C00000000,write\nread\n0x0000000007D00000000,write\nread\n0x0000000007E00000000,write\nread";
  return sequence;
}

string RefreshCounters::processOutputMessage(string output) {
  string value;

  try {
    Print::PrintInfo("refresh_counters");
    output.erase(remove(output.begin(), output.end(), '\n'), output.end());
    output = output.substr(8);
    int maxCount=5, count=0;
    bool firstIteration=true;
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
            if(tcm.temp.trigger5cnt!=hexValue||firstTime){
              float triggerRate = 0.0;
              auto now = std::chrono::high_resolution_clock::now();

              auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
              
              int milliseconds_since_epoch = static_cast<int>(duration.count());

              if(firstTime){
                triggerRate = abs(hexValue - tcm.temp.trigger5cnt)/1.0;
                tcm.temp.oldTimeTrigger5 = milliseconds_since_epoch;
              }
              else if(tcm.temp.countersUpdRate!=0){
                //triggerRate = (hexValue - tcm.temp.trigger1cnt)/tcm.temp.countersUpdRate;
                auto now = std::chrono::high_resolution_clock::now();

                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
                
                int milliseconds_since_epoch = static_cast<int>(duration.count());

                int difference = milliseconds_since_epoch - tcm.temp.oldTimeTrigger5;
                if(difference==0){
                  difference=1000;
                }

                triggerRate = abs(hexValue - tcm.temp.trigger5cnt)/difference*1000.0;

                tcm.temp.oldTimeTrigger5 = milliseconds_since_epoch;
              }

              if(hexValue==0){
                triggerRate=0;
              }
              
              if(tcm.temp.trigger5rate!=triggerRate&&!firstTime){
                tcm.temp.trigger5rate = triggerRate;
                updateTopicAnswer("READOUTCARDS/TCM0/TRG_ORA_RATE", std::to_string(triggerRate));
              }
              if(firstTime){
                tcm.temp.trigger5rate = 0;
                updateTopicAnswer("READOUTCARDS/TCM0/TRG_ORA_RATE", std::to_string(triggerRate));
              }
              tcm.temp.trigger5cnt = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/TRIGGER5_CNT", std::to_string(hexValue));
            }
            break;
          case 1:
            if(tcm.temp.trigger4cnt!=hexValue||firstTime){
              float triggerRate = 0.0;
              auto now = std::chrono::high_resolution_clock::now();

              auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
              
              int milliseconds_since_epoch = static_cast<int>(duration.count());

              if(firstTime){
                triggerRate = abs(hexValue - tcm.temp.trigger4cnt)/1.0;
                tcm.temp.oldTimeTrigger4 = milliseconds_since_epoch;
              }
              else if(tcm.temp.countersUpdRate!=0){
                //triggerRate = (hexValue - tcm.temp.trigger1cnt)/tcm.temp.countersUpdRate;

                int difference = milliseconds_since_epoch - tcm.temp.oldTimeTrigger4;
                if(difference==0){
                  difference=1000;
                }

                triggerRate = abs(hexValue - tcm.temp.trigger4cnt)/difference*1000.0;

                tcm.temp.oldTimeTrigger4 = milliseconds_since_epoch;
              }

              if(hexValue==0){
                triggerRate=0;
              }

              if(tcm.temp.trigger4rate!=triggerRate&&!firstTime){
                tcm.temp.trigger4rate = triggerRate;
                updateTopicAnswer("READOUTCARDS/TCM0/TRG_ORC_RATE", std::to_string(triggerRate));
              }
              if(firstTime){
                tcm.temp.trigger4rate = 0;
                updateTopicAnswer("READOUTCARDS/TCM0/TRG_ORC_RATE", std::to_string(triggerRate));
              }
              tcm.temp.trigger4cnt = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/TRIGGER4_CNT", std::to_string(hexValue));
            }
            break;
          case 2:
            if(tcm.temp.trigger2cnt!=hexValue||firstTime){                
              float triggerRate = 0.0;
              auto now = std::chrono::high_resolution_clock::now();

              auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
              
              int milliseconds_since_epoch = static_cast<int>(duration.count());
              if(firstTime){
                Print::PrintInfo("lalallaa1");
                triggerRate = abs(hexValue - tcm.temp.trigger2cnt)/1.0;
                tcm.temp.oldTimeTrigger2 = milliseconds_since_epoch;
              }
              else/* if(tcm.temp.countersUpdRate!=0)*/{
                //triggerRate = (hexValue - tcm.temp.trigger1cnt)/tcm.temp.countersUpdRate;
                Print::PrintInfo("info");
                int difference = milliseconds_since_epoch - tcm.temp.oldTimeTrigger2;
                if(difference==0){
                  difference=1000;
                }  

                triggerRate = abs(hexValue - tcm.temp.trigger2cnt)/difference*1000.0;

                tcm.temp.oldTimeTrigger2 = milliseconds_since_epoch;
              }

              if(hexValue==0){
                triggerRate=0;
              }

              if(tcm.temp.trigger2rate!=triggerRate&&!firstTime){
                tcm.temp.trigger2rate = triggerRate;
                updateTopicAnswer("READOUTCARDS/TCM0/TRG_SC_RATE", std::to_string(triggerRate));
              }
              if(firstTime){
                tcm.temp.trigger2rate = 0;
                updateTopicAnswer("READOUTCARDS/TCM0/TRG_SC_RATE", std::to_string(triggerRate));
              }
              tcm.temp.trigger2cnt = hexValue;
              updateTopicAnswer("READOUTCARDS/TCM0/TRIGGER2_CNT", std::to_string(hexValue));
            }
            break;
          case 3:
            if(tcm.temp.trigger1cnt!=hexValue||firstTime){
              
              float triggerRate = 0.0;
              auto now = std::chrono::high_resolution_clock::now();

              auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
              
              int milliseconds_since_epoch = static_cast<int>(duration.count());
              if(firstTime){
                triggerRate = abs(hexValue - tcm.temp.trigger1cnt)/1.0;
                tcm.temp.oldTimeTrigger1 = milliseconds_since_epoch;
              }
              else/* if(tcm.temp.countersUpdRate!=0)*/{

                int difference = milliseconds_since_epoch - tcm.temp.oldTimeTrigger1;
                if(difference==0){
                  difference=1000;
                }

                triggerRate = abs(hexValue - tcm.temp.trigger1cnt)/difference*1000.0;

                tcm.temp.oldTimeTrigger1 = milliseconds_since_epoch;
              }

              if(hexValue==0){
                triggerRate=0;
              }

              if(tcm.temp.trigger1rate!=triggerRate&&!firstTime){
                tcm.temp.trigger1rate = triggerRate;
                updateTopicAnswer("READOUTCARDS/TCM0/TRG_C_RATE", std::to_string(triggerRate));
              }
              if(firstTime){
                tcm.temp.trigger1rate = 0;
                updateTopicAnswer("READOUTCARDS/TCM0/TRG_C_RATE", std::to_string(triggerRate));
              }
              tcm.temp.trigger1cnt = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/TRIGGER1_CNT", std::to_string(hexValue));
            }
            break;
          case 4:
            if(tcm.temp.trigger3cnt!=hexValue||firstTime){
              float triggerRate = 0.0;
              tcm.temp.countersUpdRate = 1.0;
              
              auto now = std::chrono::high_resolution_clock::now();

              auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
                
              int milliseconds_since_epoch = static_cast<int>(duration.count());
              
              if(firstTime){
                triggerRate = abs(hexValue - tcm.temp.trigger3cnt)/1.0;
                tcm.temp.oldTimeTrigger3 = milliseconds_since_epoch;
              }
              else if(tcm.temp.countersUpdRate!=0){
                //triggerRate = (hexValue - tcm.temp.trigger1cnt)/tcm.temp.countersUpdRate;

                int difference = milliseconds_since_epoch - tcm.temp.oldTimeTrigger3;
                if(difference==0){
                  difference=1000;
                }

                triggerRate = abs(hexValue - tcm.temp.trigger3cnt)/difference*1000.0;

              }

              if(hexValue==0){
                triggerRate=0;
              }

              tcm.temp.oldTimeTrigger3 = milliseconds_since_epoch;
              if(tcm.temp.trigger3rate!=triggerRate&&!firstTime){
                tcm.temp.trigger3rate = triggerRate;
                updateTopicAnswer("READOUTCARDS/TCM0/TRG_V_RATE", std::to_string(triggerRate));
              }
              if(firstTime){
                tcm.temp.trigger3rate = 0;
                updateTopicAnswer("READOUTCARDS/TCM0/TRG_V_RATE", std::to_string(triggerRate));
              }
              tcm.temp.trigger3cnt = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/TRIGGER3_CNT", std::to_string(hexValue));
            }
            break;
          case 5:
            if(tcm.temp.bkgrnd0Cnt!=hexValue||firstTime){
              float bkgrndRate = 0.0;
              tcm.temp.countersUpdRate = 1.0;
              
              auto now = std::chrono::high_resolution_clock::now();

              auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
                
              int milliseconds_since_epoch = static_cast<int>(duration.count());
              
              if(firstTime){
                bkgrndRate = abs(hexValue - tcm.temp.bkgrnd0Cnt)/1.0;
                tcm.temp.bkgrnd0Cnt = milliseconds_since_epoch;
              }
              else if(tcm.temp.countersUpdRate!=0){
                //bkgrndRate = (hexValue - tcm.temp.trigger1cnt)/tcm.temp.countersUpdRate;

                int difference = milliseconds_since_epoch - tcm.temp.oldTimeBkgrnd0;
                if(difference==0){
                  difference=1000;
                }

                bkgrndRate = abs(hexValue - tcm.temp.bkgrnd0Cnt)/difference*1000.0;

              }

              if(hexValue==0){
                bkgrndRate=0;
              }

              tcm.temp.oldTimeBkgrnd0 = milliseconds_since_epoch;
              if(tcm.temp.bkgrnd0Rate!=bkgrndRate&&!firstTime){
                tcm.temp.bkgrnd0Rate = bkgrndRate;
                updateTopicAnswer("READOUTCARDS/TCM0/BKGRND0_RATE", std::to_string(bkgrndRate));
              }
              if(firstTime){
                tcm.temp.bkgrnd0Rate = 0;
                updateTopicAnswer("READOUTCARDS/TCM0/BKGRND0_RATE", std::to_string(bkgrndRate));
              }
              tcm.temp.trigger3cnt = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/BKGRND0_CNT", std::to_string(hexValue));
            }
            break;
          case 6:
            if(tcm.temp.bkgrnd1Cnt!=hexValue||firstTime){
              float bkgrndRate = 0.0;
              tcm.temp.countersUpdRate = 1.0;
              
              auto now = std::chrono::high_resolution_clock::now();

              auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
                
              int milliseconds_since_epoch = static_cast<int>(duration.count());
              
              if(firstTime){
                bkgrndRate = abs(hexValue - tcm.temp.oldTimeBkgrnd1)/1.0;
                tcm.temp.bkgrnd1Cnt = milliseconds_since_epoch;
              }
              else if(tcm.temp.countersUpdRate!=0){
                //bkgrndRate = (hexValue - tcm.temp.trigger1cnt)/tcm.temp.countersUpdRate;

                int difference = milliseconds_since_epoch - tcm.temp.bkgrnd1Cnt;
                if(difference==0){
                  difference=1000;
                }

                bkgrndRate = abs(hexValue - tcm.temp.bkgrnd1Cnt)/difference*1000.0;

              }

              if(hexValue==0){
                bkgrndRate=0;
              }

              tcm.temp.oldTimeBkgrnd1 = milliseconds_since_epoch;
              if(tcm.temp.bkgrnd1Rate!=bkgrndRate&&!firstTime){
                tcm.temp.bkgrnd1Rate = bkgrndRate;
                updateTopicAnswer("READOUTCARDS/TCM0/BKGRND1_RATE", std::to_string(bkgrndRate));
              }
              if(firstTime){
                tcm.temp.bkgrnd1Rate = 0;
                updateTopicAnswer("READOUTCARDS/TCM0/BKGRND1_RATE", std::to_string(bkgrndRate));
              }
              tcm.temp.bkgrnd1Cnt = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/BKGRND1_CNT", std::to_string(hexValue));
            }
            break;
          case 7:
            if(tcm.temp.bkgrnd2Cnt!=hexValue||firstTime){
              float bkgrndRate = 0.0;
              tcm.temp.countersUpdRate = 1.0;
              
              auto now = std::chrono::high_resolution_clock::now();

              auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
                
              int milliseconds_since_epoch = static_cast<int>(duration.count());
              
              if(firstTime){
                bkgrndRate = abs(hexValue - tcm.temp.oldTimeBkgrnd2)/1.0;
                tcm.temp.bkgrnd2Cnt = milliseconds_since_epoch;
              }
              else if(tcm.temp.countersUpdRate!=0){
                //bkgrndRate = (hexValue - tcm.temp.trigger1cnt)/tcm.temp.countersUpdRate;

                int difference = milliseconds_since_epoch - tcm.temp.bkgrnd2Cnt;
                if(difference==0){
                  difference=1000;
                }

                bkgrndRate = abs(hexValue - tcm.temp.bkgrnd2Cnt)/difference*1000.0;

              }

              if(hexValue==0){
                bkgrndRate=0;
              }

              tcm.temp.oldTimeBkgrnd2 = milliseconds_since_epoch;
              if(tcm.temp.bkgrnd2Rate!=bkgrndRate&&!firstTime){
                tcm.temp.bkgrnd2Rate = bkgrndRate;
                updateTopicAnswer("READOUTCARDS/TCM0/BKGRND2_RATE", std::to_string(bkgrndRate));
              }
              if(firstTime){
                tcm.temp.bkgrnd2Rate = 0;
                updateTopicAnswer("READOUTCARDS/TCM0/BKGRND2_RATE", std::to_string(bkgrndRate));
              }
              tcm.temp.bkgrnd2Cnt = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/BKGRND2_CNT", std::to_string(hexValue));
            }
            break;
          case 8:
            if(tcm.temp.bkgrnd3Cnt!=hexValue||firstTime){
              float bkgrndRate = 0.0;
              tcm.temp.countersUpdRate = 1.0;
              
              auto now = std::chrono::high_resolution_clock::now();

              auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
                
              int milliseconds_since_epoch = static_cast<int>(duration.count());
              
              if(firstTime){
                bkgrndRate = abs(hexValue - tcm.temp.oldTimeBkgrnd3)/1.0;
                tcm.temp.bkgrnd3Cnt = milliseconds_since_epoch;
              }
              else if(tcm.temp.countersUpdRate!=0){
                //bkgrndRate = (hexValue - tcm.temp.trigger1cnt)/tcm.temp.countersUpdRate;

                int difference = milliseconds_since_epoch - tcm.temp.bkgrnd3Cnt;
                if(difference==0){
                  difference=1000;
                }

                bkgrndRate = abs(hexValue - tcm.temp.bkgrnd3Cnt)/difference*1000.0;

              }

              if(hexValue==0){
                bkgrndRate=0;
              }

              tcm.temp.oldTimeBkgrnd3 = milliseconds_since_epoch;
              if(tcm.temp.bkgrnd3Rate!=bkgrndRate&&!firstTime){
                tcm.temp.bkgrnd3Rate = bkgrndRate;
                updateTopicAnswer("READOUTCARDS/TCM0/BKGRND3_RATE", std::to_string(bkgrndRate));
              }
              if(firstTime){
                tcm.temp.bkgrnd3Rate = 0;
                updateTopicAnswer("READOUTCARDS/TCM0/BKGRND3_RATE", std::to_string(bkgrndRate));
              }
              tcm.temp.bkgrnd3Cnt = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/BKGRND3_CNT", std::to_string(hexValue));
            }
            break;
          case 9:
            if(tcm.temp.bkgrnd4Cnt!=hexValue||firstTime){
              float bkgrndRate = 0.0;
              tcm.temp.countersUpdRate = 1.0;
              
              auto now = std::chrono::high_resolution_clock::now();

              auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
                
              int milliseconds_since_epoch = static_cast<int>(duration.count());
              
              if(firstTime){
                bkgrndRate = abs(hexValue - tcm.temp.oldTimeBkgrnd4)/1.0;
                tcm.temp.bkgrnd4Cnt = milliseconds_since_epoch;
              }
              else if(tcm.temp.countersUpdRate!=0){
                //bkgrndRate = (hexValue - tcm.temp.trigger1cnt)/tcm.temp.countersUpdRate;

                int difference = milliseconds_since_epoch - tcm.temp.bkgrnd4Cnt;
                if(difference==0){
                  difference=1000;
                }

                bkgrndRate = abs(hexValue - tcm.temp.bkgrnd4Cnt)/difference*1000.0;

              }

              if(hexValue==0){
                bkgrndRate=0;
              }

              tcm.temp.oldTimeBkgrnd4 = milliseconds_since_epoch;
              if(tcm.temp.bkgrnd4Rate!=bkgrndRate&&!firstTime){
                tcm.temp.bkgrnd4Rate = bkgrndRate;
                updateTopicAnswer("READOUTCARDS/TCM0/BKGRND4_RATE", std::to_string(bkgrndRate));
              }
              if(firstTime){
                tcm.temp.bkgrnd4Rate = 0;
                updateTopicAnswer("READOUTCARDS/TCM0/BKGRND4_RATE", std::to_string(bkgrndRate));
              }
              tcm.temp.bkgrnd4Cnt = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/BKGRND4_CNT", std::to_string(hexValue));
            }
            break;
          case 10:
            if(tcm.temp.bkgrnd5Cnt!=hexValue||firstTime){
              float bkgrndRate = 0.0;
              tcm.temp.countersUpdRate = 1.0;
              
              auto now = std::chrono::high_resolution_clock::now();

              auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
                
              int milliseconds_since_epoch = static_cast<int>(duration.count());
              
              if(firstTime){
                bkgrndRate = abs(hexValue - tcm.temp.oldTimeBkgrnd5)/1.0;
                tcm.temp.bkgrnd5Cnt = milliseconds_since_epoch;
              }
              else if(tcm.temp.countersUpdRate!=0){
                //bkgrndRate = (hexValue - tcm.temp.trigger1cnt)/tcm.temp.countersUpdRate;

                int difference = milliseconds_since_epoch - tcm.temp.bkgrnd5Cnt;
                if(difference==0){
                  difference=1000;
                }

                bkgrndRate = abs(hexValue - tcm.temp.bkgrnd5Cnt)/difference*1000.0;

              }

              if(hexValue==0){
                bkgrndRate=0;
              }

              tcm.temp.oldTimeBkgrnd5 = milliseconds_since_epoch;
              if(tcm.temp.bkgrnd5Rate!=bkgrndRate&&!firstTime){
                tcm.temp.bkgrnd5Rate = bkgrndRate;
                updateTopicAnswer("READOUTCARDS/TCM0/BKGRND5_RATE", std::to_string(bkgrndRate));
              }
              if(firstTime){
                tcm.temp.bkgrnd5Rate = 0;
                updateTopicAnswer("READOUTCARDS/TCM0/BKGRND5_RATE", std::to_string(bkgrndRate));
              }
              tcm.temp.bkgrnd5Cnt = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/BKGRND5_CNT", std::to_string(hexValue));
            }
            break;
          case 11:
            if(tcm.temp.bkgrnd6Cnt!=hexValue||firstTime){
              float bkgrndRate = 0.0;
              tcm.temp.countersUpdRate = 1.0;
              
              auto now = std::chrono::high_resolution_clock::now();

              auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
                
              int milliseconds_since_epoch = static_cast<int>(duration.count());
              
              if(firstTime){
                bkgrndRate = abs(hexValue - tcm.temp.oldTimeBkgrnd6)/1.0;
                tcm.temp.bkgrnd6Cnt = milliseconds_since_epoch;
              }
              else if(tcm.temp.countersUpdRate!=0){
                //bkgrndRate = (hexValue - tcm.temp.trigger1cnt)/tcm.temp.countersUpdRate;

                int difference = milliseconds_since_epoch - tcm.temp.bkgrnd6Cnt;
                if(difference==0){
                  difference=1000;
                }

                bkgrndRate = abs(hexValue - tcm.temp.bkgrnd6Cnt)/difference*1000.0;

              }

              if(hexValue==0){
                bkgrndRate=0;
              }

              tcm.temp.oldTimeBkgrnd6 = milliseconds_since_epoch;
              if(tcm.temp.bkgrnd6Rate!=bkgrndRate&&!firstTime){
                tcm.temp.bkgrnd6Rate = bkgrndRate;
                updateTopicAnswer("READOUTCARDS/TCM0/BKGRND6_RATE", std::to_string(bkgrndRate));
              }
              if(firstTime){
                tcm.temp.bkgrnd6Rate = 0;
                updateTopicAnswer("READOUTCARDS/TCM0/BKGRND6_RATE", std::to_string(bkgrndRate));
              }
              tcm.temp.bkgrnd6Cnt = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/BKGRND6_CNT", std::to_string(hexValue));
            }
            break;
          case 12:
            if(tcm.temp.bkgrnd7Cnt!=hexValue||firstTime){
              float bkgrndRate = 0.0;
              tcm.temp.countersUpdRate = 1.0;
              
              auto now = std::chrono::high_resolution_clock::now();

              auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
                
              int milliseconds_since_epoch = static_cast<int>(duration.count());
              
              if(firstTime){
                bkgrndRate = abs(hexValue - tcm.temp.oldTimeBkgrnd7)/1.0;
                tcm.temp.bkgrnd7Cnt = milliseconds_since_epoch;
              }
              else if(tcm.temp.countersUpdRate!=0){
                //bkgrndRate = (hexValue - tcm.temp.trigger1cnt)/tcm.temp.countersUpdRate;

                int difference = milliseconds_since_epoch - tcm.temp.bkgrnd7Cnt;
                if(difference==0){
                  difference=1000;
                }

                bkgrndRate = abs(hexValue - tcm.temp.bkgrnd7Cnt)/difference*1000.0;

              }

              if(hexValue==0){
                bkgrndRate=0;
              }

              tcm.temp.oldTimeBkgrnd7 = milliseconds_since_epoch;
              if(tcm.temp.bkgrnd7Rate!=bkgrndRate&&!firstTime){
                tcm.temp.bkgrnd7Rate = bkgrndRate;
                updateTopicAnswer("READOUTCARDS/TCM0/BKGRND7_RATE", std::to_string(bkgrndRate));
              }
              if(firstTime){
                tcm.temp.bkgrnd7Rate = 0;
                updateTopicAnswer("READOUTCARDS/TCM0/BKGRND7_RATE", std::to_string(bkgrndRate));
              }
              tcm.temp.bkgrnd7Cnt = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/BKGRND7_CNT", std::to_string(hexValue));
            }
            break;
          case 13:
            if(tcm.temp.bkgrnd8Cnt!=hexValue||firstTime){
              float bkgrndRate = 0.0;
              tcm.temp.countersUpdRate = 1.0;
              
              auto now = std::chrono::high_resolution_clock::now();

              auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
                
              int milliseconds_since_epoch = static_cast<int>(duration.count());
              
              if(firstTime){
                bkgrndRate = abs(hexValue - tcm.temp.oldTimeBkgrnd8)/1.0;
                tcm.temp.bkgrnd8Cnt = milliseconds_since_epoch;
              }
              else if(tcm.temp.countersUpdRate!=0){
                //bkgrndRate = (hexValue - tcm.temp.trigger1cnt)/tcm.temp.countersUpdRate;

                int difference = milliseconds_since_epoch - tcm.temp.bkgrnd8Cnt;
                if(difference==0){
                  difference=1000;
                }

                bkgrndRate = abs(hexValue - tcm.temp.bkgrnd8Cnt)/difference*1000.0;

              }

              if(hexValue==0){
                bkgrndRate=0;
              }

              tcm.temp.oldTimeBkgrnd8 = milliseconds_since_epoch;
              if(tcm.temp.bkgrnd8Rate!=bkgrndRate&&!firstTime){
                tcm.temp.bkgrnd8Rate = bkgrndRate;
                updateTopicAnswer("READOUTCARDS/TCM0/BKGRND8_RATE", std::to_string(bkgrndRate));
              }
              if(firstTime){
                tcm.temp.bkgrnd8Rate = 0;
                updateTopicAnswer("READOUTCARDS/TCM0/BKGRND8_RATE", std::to_string(bkgrndRate));
              }
              tcm.temp.bkgrnd8Cnt = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/BKGRND8_CNT", std::to_string(hexValue));
            }
            break;
          case 14:
            if(tcm.temp.bkgrnd9Cnt!=hexValue||firstTime){
              float bkgrndRate = 0.0;
              tcm.temp.countersUpdRate = 1.0;
              
              auto now = std::chrono::high_resolution_clock::now();

              auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
                
              int milliseconds_since_epoch = static_cast<int>(duration.count());
              
              if(firstTime){
                bkgrndRate = abs(hexValue - tcm.temp.oldTimeBkgrnd9)/1.0;
                tcm.temp.bkgrnd9Cnt = milliseconds_since_epoch;
              }
              else if(tcm.temp.countersUpdRate!=0){
                //bkgrndRate = (hexValue - tcm.temp.trigger1cnt)/tcm.temp.countersUpdRate;

                int difference = milliseconds_since_epoch - tcm.temp.bkgrnd9Cnt;
                if(difference==0){
                  difference=1000;
                }

                bkgrndRate = abs(hexValue - tcm.temp.bkgrnd9Cnt)/difference*1000.0;

              }

              if(hexValue==0){
                bkgrndRate=0;
              }

              tcm.temp.oldTimeBkgrnd9 = milliseconds_since_epoch;
              if(tcm.temp.bkgrnd9Rate!=bkgrndRate&&!firstTime){
                tcm.temp.bkgrnd9Rate = bkgrndRate;
                updateTopicAnswer("READOUTCARDS/TCM0/BKGRND9_RATE", std::to_string(bkgrndRate));
              }
              if(firstTime){
                tcm.temp.bkgrnd9Rate = 0;
                updateTopicAnswer("READOUTCARDS/TCM0/BKGRND9_RATE", std::to_string(bkgrndRate));
              }
              tcm.temp.bkgrnd9Cnt = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/BKGRND9_CNT", std::to_string(hexValue));
            }
            break;
        }
        count++;
    }
    firstTime=false;
    //usleep(5000000);
    //sleep(1);
    //std::this_thread::sleep_for(std::chrono::milliseconds(5000));
  }
  catch(exception &e){
    Print::PrintInfo(e.what());
    Print::PrintError("ERROR in connection with ALF!");
  }

  return to_string(0);
}

