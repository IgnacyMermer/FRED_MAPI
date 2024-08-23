

#include <algorithm>
#include <iostream>
#include <numeric>
#include <bitset>
#include <sstream>
#include <string>
#include "refresh_TCM.h"
#include "Alfred/print.h"
#include "Parser/utility.h"
#include "TCM_values.h"
#include <thread>
#include <chrono>
#include <cmath>
#include "../../core/include/dim/dic.hxx"
#include "../../core/include/FREDServer/Fred/Mapi/iterativemapi.h"

std::string numberLetter2(int number){
    std::string numberLetterStr = "";
    if((number/16)<10&&(number/16)!=0){
        numberLetterStr+=std::to_string(number/16);
    }
    else if((number/16)!=0){
        char tempChar = 'A'+((number/16)-10);
        std::string temp(1, tempChar);
        numberLetterStr+=temp;
    }
    number=number%16;
    if(number<10){
        numberLetterStr+=std::to_string(number);
    }
    else{
        char tempChar = 'A'+(number-10);
        std::string temp(1, tempChar);
        numberLetterStr+=temp;
    }
    return numberLetterStr;
}

RefreshTCM::RefreshTCM() {
    count = 0;
    sequence = "reset";
    firstTime=true;
    std::string serviceName="READOUTCARDS/TCM0/";
    for(int i=0; i<=5; i++){
        sequence+="\n0x0000000000"+numberLetter2(i)+"00000000,write\nread";
    }
    services.push_back(serviceName+"DELAY_A");
    services.push_back(serviceName+"DELAY_C");
    services.push_back(serviceName+"LASER_DELAY");
    services.push_back(serviceName+"EXT_SW");
    services.push_back(serviceName+"ATTENUATOR");
    services.push_back(serviceName+"TEMPERATURE");
    for(int i=7; i<=15; i++){
        sequence+="\n0x0000000000"+numberLetter2(i)+"00000000,write\nread";
    }
    services.push_back(serviceName+"BOARD_TYPE");
    services.push_back(serviceName+"VTIME_LOW");
    services.push_back(serviceName+"VTIME_HIGH");
    services.push_back(serviceName+"SC_LEVEL_A");
    services.push_back(serviceName+"SC_LEVEL_C");
    services.push_back(serviceName+"C_LEVEL_A");
    services.push_back(serviceName+"C_LEVEL_C");
    services.push_back(serviceName+"MODE");
    services.push_back(serviceName+"BOARD_STATUS");
    for(int i=16; i<=25; i++){
        sequence+="\n0x000000000"+numberLetter2(i)+"00000000,write\nread";
        services.push_back(serviceName+"PM_LINK_A"+std::to_string(i%16));
    }
    for(int i=26; i<=32; i++){
        sequence+="\n0x000000000"+numberLetter2(i)+"00000000,write\nread";
    }
    services.push_back(serviceName+"SIDE_A_STATUS");
    services.push_back(serviceName+"LASER_DIVIDER");
    services.push_back(serviceName+"LASER_PATTERN_1");
    services.push_back(serviceName+"LASER_PATTERN_0");
    services.push_back(serviceName+"SPI_MASK");
    services.push_back(serviceName+"TRIGGERS_SUPPRESSION_CONTROL");
    services.push_back(serviceName+"AVERAGE_TIME");
    for(int i=48; i<=57; i++){
        sequence+="\n0x000000000"+numberLetter2(i)+"00000000,write\nread";
        services.push_back(serviceName+"PM_LINK_A"+std::to_string(i%16));
    }
    sequence+="\n0x000000000"+numberLetter2(58)+"00000000,write\nread";
    services.push_back(serviceName+"SIDE_C_STATUS");
    sequence+="\n0x000000000"+numberLetter2(0x50)+"00000000,write\nread";
    services.push_back(serviceName+"COUNTERS_UPD_RATE");
    for(int i=0x60; i<=0x69; i++){
        sequence+="\n0x000000000"+numberLetter2(i)+"00000000,write\nread";
    }
    services.push_back(serviceName+"TRG_ORA_SIGN");
    services.push_back(serviceName+"TRIGGER5_RAND");
    services.push_back(serviceName+"TRG_ORC_SIGN");
    services.push_back(serviceName+"TRIGGER4_RAND");
    services.push_back(serviceName+"TRG_SC_SIGN");
    services.push_back(serviceName+"TRIGGER2_RAND");
    services.push_back(serviceName+"TRG_C_SIGN");
    services.push_back(serviceName+"TRIGGER1_RAND");
    services.push_back(serviceName+"TRG_V_SIGN");
    services.push_back(serviceName+"TRIGGER3_RAND");
    sequence+="\n0x000000000"+numberLetter2(0x6A)+"00000000,write\nread";
    services.push_back(serviceName+"TRIGGERS_OUTPUTS_MODE");
    for(int i=0x70; i<=0x74; i++){
        sequence+="\n0x000000000"+numberLetter2(i)+"00000000,write\nread";
    }
    services.push_back(serviceName+"TRIGGER5_CNT");
    services.push_back(serviceName+"TRIGGER4_CNT");
    services.push_back(serviceName+"TRIGGER2_CNT");
    services.push_back(serviceName+"TRIGGER1_CNT");
    services.push_back(serviceName+"TRIGGER3_CNT");
    for(int i=0x75; i<=0x7E; i++){
        sequence+="\n0x000000000"+numberLetter2(i)+"00000000,write\nread";
        services.push_back(serviceName+"BKGRND"+std::to_string(i-0x75)+"_CNT");
    }
    sequence+="\n0x000000000"+numberLetter2(0xD8)+"00000000,write\nread";
    services.push_back(serviceName+"MODE_SETTINGS");
    sequence+="\n0x000000000"+numberLetter2(0xD9)+"00000000,write\nread";
    services.push_back(serviceName+"DG_TRG_RESPOND_MASK");
    sequence+="\n0x000000000"+numberLetter2(0xDA)+"00000000,write\nread";
    services.push_back(serviceName+"DG_BUNCH_PATTERN");
    sequence+="\n0x000000000"+numberLetter2(0xDB)+"00000000,write\nread";
    services.push_back(serviceName+"TG_SINGLE_VALUE");
    sequence+="\n0x000000000"+numberLetter2(0xDC)+"00000000,write\nread";
    services.push_back(serviceName+"TG_PATTERN_1");
    sequence+="\n0x000000000"+numberLetter2(0xDD)+"00000000,write\nread";
    services.push_back(serviceName+"TG_PATTERN_0");
    sequence+="\n0x000000000"+numberLetter2(0xDE)+"00000000,write\nread";
    services.push_back(serviceName+"TG_CONT_VALUE");
    sequence+="\n0x000000000"+numberLetter2(0xDF)+"00000000,write\nread";
    services.push_back(serviceName+"EMULATION_RATE");
    sequence+="\n0x000000000"+numberLetter2(0xE0)+"00000000,write\nread";
    services.push_back(serviceName+"GENERATOR_FREQ_OFFSET");
    sequence+="\n0x000000000"+numberLetter2(0xE1)+"00000000,write\nread";
    services.push_back(serviceName+"RDH_FIELDS");
    sequence+="\n0x000000000"+numberLetter2(0xE3)+"00000000,write\nread";
    services.push_back(serviceName+"BCID_OFFSET");
    sequence+="\n0x000000000"+numberLetter2(0xE4)+"00000000,write\nread";
    services.push_back(serviceName+"DATA_SEL_TRG_MASK");
    sequence+="\n0x000000000"+numberLetter2(0xE8)+"00000000,write\nread";
    services.push_back(serviceName+"MODES_STATUS");
    sequence+="\n0x000000000"+numberLetter2(0xE9)+"00000000,write\nread";
    services.push_back(serviceName+"CRU_ORBIT");
    sequence+="\n0x000000000"+numberLetter2(0xEA)+"00000000,write\nread";
    services.push_back(serviceName+"CRU_BC");
    sequence+="\n0x000000000"+numberLetter2(0xEB)+"00000000,write\nread";
    services.push_back(serviceName+"FIFO_COUNT");
    sequence+="\n0x000000000"+numberLetter2(0xEC)+"00000000,write\nread";
    services.push_back(serviceName+"SEL_FIRST_HIT_DROPPED_ORBIT");
    sequence+="\n0x000000000"+numberLetter2(0xED)+"00000000,write\nread";
    services.push_back(serviceName+"SEL_LAST_HIT_DROPPED_ORBIT");
    sequence+="\n0x000000000"+numberLetter2(0xEE)+"00000000,write\nread";
    services.push_back(serviceName+"SEL_HITS_DROPPED");
    sequence+="\n0x000000000"+numberLetter2(0xEF)+"00000000,write\nread";
    services.push_back(serviceName+"READOUT_RATE");
    sequence+="\n0x000000000"+numberLetter2(0xF0)+"00000000,write\nread";
    services.push_back(serviceName+"IPbus_FIFO_DATA");
    sequence+="\n0x000000000"+numberLetter2(0xF1)+"00000000,write\nread";
    services.push_back(serviceName+"EVENTS_COUNT");
    sequence+="\n0x000000000"+numberLetter2(0xF7)+"00000000,write\nread";
    services.push_back(serviceName+"MCODE_TIME");
    sequence+="\n0x000000000"+numberLetter2(0xFC)+"00000000,write\nread";
    services.push_back(serviceName+"FPGA_TEMP");
    sequence+="\n0x000000000"+numberLetter2(0xFD)+"00000000,write\nread";
    services.push_back(serviceName+"1VPOWER");
    sequence+="\n0x000000000"+numberLetter2(0xFE)+"00000000,write\nread";
    services.push_back(serviceName+"18VPOWER");
    sequence+="\n0x000000000"+numberLetter2(0xFF)+"00000000,write\nread";
    services.push_back(serviceName+"FW_TIME");
}

string RefreshTCM::processInputMessage(string input) { 
    //if(input=="go"){
    return sequence;
    //}
    return "";
}

//string RefreshData::processInputMessage(string input) {

  //sequence = "reset\n0x0000000000100000000,write\nread\n0x0000000000200000000,write\nread\n0x0000000000500000000,write\nread\n0x0000000000700000000,write\nread\n0x0000000000F00000000,write\nread\n0x0000000006A00000000,write\nread\n0x0000000006000000000,write\nread\n0x0000000006100000000,write\nread\n0x0000000006200000000,write\nread\n0x0000000006300000000,write\nread\n0x0000000006800000000,write\nread\n0x0000000006900000000,write\nread\n0x0000000006400000000,write\nread\n0x0000000006500000000,write\nread\n0x0000000006600000000,write\nread\n0x0000000006700000000,write\nread\n0x0000000007000000000,write\nread\n0x0000000007100000000,write\nread\n0x0000000007400000000,write\nread\n0x0000000007200000000,write\nread\n0x0000000007300000000,write\nread\n0x0000000001000000000,write\nread\n0x0000000001100000000,write\nread\n0x0000000001200000000,write\nread\n0x0000000001300000000,write\nread\n0x0000000001400000000,write\nread\n0x0000000001500000000,write\nread\n0x0000000001600000000,write\nread\n0x0000000001700000000,write\nread\n0x0000000001800000000,write\nread\n0x0000000001900000000,write\nread\n0x0000000003000000000,write\nread\n0x0000000003100000000,write\nread\n0x0000000003200000000,write\nread\n0x0000000003300000000,write\nread\n0x0000000003400000000,write\nread\n0x0000000003500000000,write\nread\n0x0000000003600000000,write\nread\n0x0000000003700000000,write\nread\n0x0000000003800000000,write\nread\n0x0000000003900000000,write\nread\n0x0000000000000000000,write\nread\n0x0000000000800000000,write\nread\n0x0000000000900000000,write\nread\n0x0000000000A00000000,write\nread\n0x0000000000B00000000,write\nread\n0x0000000000C00000000,write\nread\n0x0000000000D00000000,write\nread\n0x000000000E800000000,write\nread\n0x0000000001A00000000,write\nread\n0x0000000003A00000000,write\nread\n0x0000000005000000000,write\nread\n0x0000000007500000000,write\nread\n0x0000000007600000000,write\nread\n0x0000000007700000000,write\nread\n0x0000000007800000000,write\nread\n0x0000000007900000000,write\nread\n0x0000000007A00000000,write\nread\n0x0000000007B00000000,write\nread\n0x0000000007C00000000,write\nread\n0x0000000007D00000000,write\nread\n0x0000000007E00000000,write\nread\n0x000000000FC00000000,write\nread\n0x000000000FD00000000,write\nread\n0x000000000FE00000000,write\nread\n0x000000000D800000000,write\nread\n0x0000000001B00000000,write\nread\n0x0000000000400000000,write\nread\n0x0000000000300000000,write\nread\n0x0000000000E00000000,write\nread\n0x000000000E400000000,write\nread\n0x000000000E300000000,write\nread\n0x000000000D900000000,write\nread\n0x000000000E100000000,write\nread\n0x000000000DC00000000,write\nread\n0x000000000DD00000000,write\nread\n0x000000000E900000000,write\nread\n0x000000000EA00000000,write\nread\n0x000000000EB00000000,write\nread\n0x000000000EC00000000,write\nread\n0x000000000ED00000000,write\nread\n0x000000000EE00000000,write\nread\n0x000000000EF00000000,write\nread\n0x000000000F000000000,write\nread\n0x000000000F100000000,write\nread\n0x000000000F700000000,write\nread\n0x000000000FF00000000,write\nread\n0x0000000001E00000000,write\nread\n0x0000000001F00000000,write\nread\n0x0000000002000000000,write\nread\n0x000000000E000000000,write\nread\n0x000000000DF00000000,write\nread\n0x000000000DE00000000,write\nread\n0x0000000001C00000000,write\nread\n0x0000000001D00000000,write\nread";
  //return sequence;
//}

string RefreshTCM::processOutputMessage(string output) {
  /*string value;
  if(output!="failure"){
    try {
      output.erase(remove(output.begin(), output.end(), '\n'), output.end());
      output = output.substr(8);
      int maxCount=20000, count=0;
      bool firstIteration=true;
      if(firstTime){
        tcm.temp.triggerCounterTemp = 0;
        updateTopicAnswer("READOUTCARDS/TCM0/WORK_STATUS", std::to_string(1));
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
              if (hexValue > 10000) {
                int16_t x = stoi(value, nullptr, 16);
                hexValue=-(~x+1);
              }
              Print::PrintInfo("sendCommand"+std::to_string(count));
              float systemClock_MHz = tcm.act.externalClock?40.0789658:40.;
              float halfBC_ns = 500. / systemClock_MHz;
              float phaseStep_ns = halfBC_ns / 
              //(SERIAL_NUM ? 
              1024;
              // : 1280);
              float returnValue = hexValue*phaseStep_ns;
              updateTopicAnswer("READOUTCARDS/TCM0/DELAY_C", std::to_string(returnValue));
            }
            break;
          case 1:
            if(tcm.temp.delayLaser!=hexValue||firstTime){
              tcm.temp.delayLaser = hexValue;
              if (hexValue > 10000) {
                int16_t x = stoi(value, nullptr, 16);
                hexValue=-(~x+1);
              }
              float tempValue = hexValue;
              float systemClock_MHz = tcm.act.externalClock?40.0789658:40.;
              float halfBC_ns = 500. / systemClock_MHz;
              float phaseStep_ns = halfBC_ns / 
              //(SERIAL_NUM ? 
              1024 ; 
              //: 1280);
              tempValue = tempValue*phaseStep_ns;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/LASER_DELAY", std::to_string(tempValue));
            }
            break;
          case 2:
            if(tcm.temp.boardTemp!=hexValue||firstTime){
              tcm.temp.boardTemp = hexValue;
              float returnValue = hexValue/10.0;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/TEMPERATURE", std::to_string(hexValue/10.));
            }
            break;
          case 3:
            if(tcm.temp.boardType!=hexValue||firstTime){
              tcm.temp.boardType = hexValue;
              Print::PrintInfo("changed");
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/BOARD_TYPE", std::to_string(hexValue));
            }
            break;
          case 4:
            if(tcm.temp.actualValues!=hexValue||firstTime){
              tcm.temp.actualValues = hexValue;
              int pllLockC = (hexValue)&1;
              int pllLockA = (hexValue>>1)&1;
              int systemRestarted = (hexValue>>2)&1;
              int clockSrc = (hexValue>>3)&1;
              int RxReady = (hexValue>>4)&1;
              int forceLocalClock = (hexValue>>10)&1;
              tcm.act.PLLlockA=pllLockA;
              tcm.act.PLLlockC=pllLockC;
              tcm.act.systemRestarted=systemRestarted;
              tcm.act.externalClock=clockSrc;
              tcm.act.GBTRxReady=RxReady;
              tcm.act.forceLocalClock=forceLocalClock;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/BOARD_STATUS", std::to_string(hexValue));
            }
            break;
          case 5:
            if(tcm.temp.triggers!=hexValue||firstTime){
              Print::PrintInfo("sendCommand"+std::to_string(count));
              std::bitset<32> binary(hexValue);
              std::string binary_str = binary.to_string();
              updateTopicAnswer("READOUTCARDS/TCM0/TRIGGERS_OUTPUTS_MODE", binary_str);
            }
            break;
          case 6:
            if(tcm.temp.trigger5sign!=hexValue||firstTime){
              tcm.temp.trigger5sign = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/TRG_ORA_SIGN", std::to_string(hexValue/128));
            }
            break;
          case 7:
            if(tcm.temp.trigger5rand!=hexValue||firstTime){
              tcm.temp.trigger5rand = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/TRIGGER5_RAND", std::to_string(hexValue));
            }
            break;
          case 8:
            if(tcm.temp.trigger4sign!=hexValue||firstTime){
              tcm.temp.trigger4sign = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/TRG_ORC_SIGN", std::to_string(hexValue/128));
            }
            break;
          case 9:
            if(tcm.temp.trigger4rand!=hexValue||firstTime){
              tcm.temp.trigger4rand = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/TRIGGER4_RAND", std::to_string(hexValue));
            }
            break;
          case 10:
            if(tcm.temp.trigger3sign!=hexValue||firstTime){
              tcm.temp.trigger3sign = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/TRG_V_SIGN", std::to_string(hexValue/128));
            }
            break;
          case 11:
            if(tcm.temp.trigger3rand!=hexValue||firstTime){
              tcm.temp.trigger3rand = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/TRIGGER3_RAND", std::to_string(hexValue));
            }
            break;
          case 12:
            if(tcm.temp.trigger2sign!=hexValue||firstTime){
              tcm.temp.trigger2sign = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/TRG_SC_SIGN", std::to_string(hexValue/128));
            }
            break;
          case 13:
            if(tcm.temp.trigger2rand!=hexValue||firstTime){
              tcm.temp.trigger2rand = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/TRIGGER2_RAND", std::to_string(hexValue));
            }
            break;
          case 14:
            if(tcm.temp.trigger1sign!=hexValue||firstTime){
              tcm.temp.trigger1sign = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/TRG_C_SIGN", std::to_string(hexValue/128));
            }
            break;
          case 15:
            if(tcm.temp.trigger1rand!=hexValue||firstTime){
              tcm.temp.trigger1rand = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/TRIGGER1_RAND", std::to_string(hexValue));
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
                //updateTopicAnswer("READOUTCARDS/TCM0/TRIGGER5_RATE", std::to_string(hexValue));
              }
              //tcm.temp.trigger5cnt = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              //updateTopicAnswer("READOUTCARDS/TCM0/TRIGGER5_CNT", std::to_string(hexValue));
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
                //updateTopicAnswer("READOUTCARDS/TCM0/TRIGGER4_RATE", std::to_string(hexValue));
              }
              //tcm.temp.trigger4cnt = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              //updateTopicAnswer("READOUTCARDS/TCM0/TRIGGER4_CNT", std::to_string(hexValue));
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
                //updateTopicAnswer("READOUTCARDS/TCM0/TRIGGER3_RATE", std::to_string(hexValue));
              }
              //tcm.temp.trigger3cnt = hexValue;
              //Print::PrintInfo("sendCommand"+std::to_string(count));
              //updateTopicAnswer("READOUTCARDS/TCM0/TRIGGER3_CNT", std::to_string(hexValue));
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
                //updateTopicAnswer("READOUTCARDS/TCM0/TRIGGER2_RATE", std::to_string(hexValue));
              }
              //tcm.temp.trigger2cnt = hexValue;
              //Print::PrintInfo("sendCommand"+std::to_string(count));
              //updateTopicAnswer("READOUTCARDS/TCM0/TRIGGER2_CNT", std::to_string(hexValue));
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
                //updateTopicAnswer("READOUTCARDS/TCM0/TRIGGER1_RATE", std::to_string(hexValue));
              }
              //tcm.temp.trigger1cnt = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              //updateTopicAnswer("READOUTCARDS/TCM0/TRIGGER1_CNT", std::to_string(hexValue));
            }
            break;
          case 21:
            if(tcm.temp.pmA0Status!=hexValue||firstTime){
              tcm.temp.pmA0Status = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/PM_LINK_A0", std::to_string(hexValue));
            }
            break;
          case 22:
            if(tcm.temp.pmA1Status!=hexValue||firstTime){
              tcm.temp.pmA1Status = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/PM_LINK_A1", std::to_string(hexValue));
            }
            break;
          case 23:
            if(tcm.temp.pmA2Status!=hexValue||firstTime){
              tcm.temp.pmA2Status = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/PM_LINK_A2", std::to_string(hexValue));
            }
            break;
          case 24:
            if(tcm.temp.pmA3Status!=hexValue||firstTime){
              tcm.temp.pmA3Status = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/PM_LINK_A3", std::to_string(hexValue));
            }
            break;
          case 25:
            if(tcm.temp.pmA4Status!=hexValue||firstTime){
              tcm.temp.pmA4Status = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/PM_LINK_A4", std::to_string(hexValue));
            }
            break;
          case 26:
            if(tcm.temp.pmA5Status!=hexValue||firstTime){
              tcm.temp.pmA5Status = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/PM_LINK_A5", std::to_string(hexValue));
            }
            break;
          case 27:
            if(tcm.temp.pmA6Status!=hexValue||firstTime){
              tcm.temp.pmA6Status = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/PM_LINK_A6", std::to_string(hexValue));
            }
            break;
          case 28:
            if(tcm.temp.pmA7Status!=hexValue||firstTime){
              tcm.temp.pmA7Status = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/PM_LINK_A7", std::to_string(hexValue));
            }
            break;
          case 29:
            if(tcm.temp.pmA8Status!=hexValue||firstTime){
              tcm.temp.pmA8Status = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/PM_LINK_A8", std::to_string(hexValue));
            }
            break;
          case 30:
            if(tcm.temp.pmA9Status!=hexValue||firstTime){
              tcm.temp.pmA9Status = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/PM_LINK_A9", std::to_string(hexValue));
            }
            break;
          case 31:
            if(tcm.temp.pmC0Status!=hexValue||firstTime){
              tcm.temp.pmC0Status = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/PM_LINK_C0", std::to_string(hexValue));
            }
            break;
          case 32:
            if(tcm.temp.pmC1Status!=hexValue||firstTime){
              tcm.temp.pmC1Status = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/PM_LINK_C1", std::to_string(hexValue));
            }
            break;
          case 33:
            if(tcm.temp.pmC2Status!=hexValue||firstTime){
              tcm.temp.pmC2Status = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/PM_LINK_C2", std::to_string(hexValue));
            }
            break;
          case 34:
            if(tcm.temp.pmC3Status!=hexValue||firstTime){
              tcm.temp.pmC3Status = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/PM_LINK_C3", std::to_string(hexValue));
            }
            break;
          case 35:
            if(tcm.temp.pmC4Status!=hexValue||firstTime){
              tcm.temp.pmC4Status = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/PM_LINK_C4", std::to_string(hexValue));
            }
            break;
          case 36:
            if(tcm.temp.pmC5Status!=hexValue||firstTime){
              tcm.temp.pmC5Status = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/PM_LINK_C5", std::to_string(hexValue));
            }
            break;
          case 37:
            if(tcm.temp.pmC6Status!=hexValue||firstTime){
              tcm.temp.pmC6Status = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/PM_LINK_C6", std::to_string(hexValue));
            }
            break;
          case 38:
            if(tcm.temp.pmC7Status!=hexValue||firstTime){
              tcm.temp.pmC7Status = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/PM_LINK_C7", std::to_string(hexValue));
            }
            break;
          case 39:
            if(tcm.temp.pmC8Status!=hexValue||firstTime){
              tcm.temp.pmC8Status = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/PM_LINK_C8", std::to_string(hexValue));
            }
            break;
          case 40:
            if(tcm.temp.pmC9Status!=hexValue||firstTime){
              tcm.temp.pmC9Status = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/PM_LINK_C9", std::to_string(hexValue));
            }
            break;
          case 41:
            if(tcm.temp.delayA!=hexValue||firstTime){
              tcm.temp.delayA = hexValue;
              if (hexValue > 10000) {
                int16_t x = stoi(value, nullptr, 16);
                hexValue=-(~x+1);
              }
              Print::PrintInfo("sendCommand"+std::to_string(count));
              float systemClock_MHz = tcm.act.externalClock?40.0789658:40.;
              float halfBC_ns = 500. / systemClock_MHz;
              float phaseStep_ns = halfBC_ns / 
              //(SERIAL_NUM ? 
              1024 ; 
              //: 1280);
              float returnValue = hexValue*phaseStep_ns;
              updateTopicAnswer("READOUTCARDS/TCM0/DELAY_A", std::to_string(returnValue));
            }
            break;
          case 42:
            if(tcm.temp.vtimeLow!=hexValue||firstTime){
              //signed
              tcm.temp.vtimeLow = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/VTIME_LOW", std::to_string(hexValue));
            }
            break;
          case 43:
            if(tcm.temp.vtimeHigh!=hexValue||firstTime){
              //signed
              tcm.temp.vtimeHigh = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/VTIME_HIGH", std::to_string(hexValue));
            }
            break;
          case 44:
            if(tcm.temp.scLevelA!=hexValue||firstTime){
              tcm.temp.scLevelA = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/SC_LEVEL_A", std::to_string(hexValue));
            }
            break;
          case 45:
            if(tcm.temp.scLevelC!=hexValue||firstTime){
              tcm.temp.scLevelC = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/SC_LEVEL_C", std::to_string(hexValue));
            }
            break;
          case 46:
            if(tcm.temp.cLevelA!=hexValue||firstTime){
              tcm.temp.cLevelA = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/C_LEVEL_A", std::to_string(hexValue));
            }
            break;
          case 47:
            if(tcm.temp.cLevelC!=hexValue||firstTime){
              tcm.temp.cLevelC = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/C_LEVEL_C", std::to_string(hexValue));
            }
            break;
          case 48:
            if(tcm.temp.mainPanelBits!=hexValue||firstTime){
              tcm.temp.mainPanelBits = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/MODES_STATUS", std::to_string(hexValue));
            }
            break;
          case 49:
            if(tcm.temp.sideAStatus!=hexValue||firstTime){
              tcm.temp.sideAStatus = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/SIDE_A_STATUS", std::to_string(hexValue));
            }
            break;
          case 50:
            if(tcm.temp.sideCStatus!=hexValue||firstTime){
              tcm.temp.sideCStatus = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/SIDE_C_STATUS", std::to_string(hexValue));
            }
            break;
          case 51:
            if(tcm.temp.countersUpdRate!=(hexValue==0?1.0:(hexValue==1?0.1:(hexValue==2?0.2:(hexValue==3?0.5:(hexValue==4?1:(hexValue==5?2:(hexValue==6?5:10)))))))
            ||firstTime){
              tcm.temp.countersUpdRate = (hexValue==0?1.0:(hexValue==1?0.1:(hexValue==2?0.2:(hexValue==3?0.5:(hexValue==4?1:(hexValue==5?2:(hexValue==6?5:10)))))));
              if(firstTime&&tcm.temp.countersUpdRate!=0){
                tcm.temp.trigger5rate = tcm.temp.trigger5cnt/tcm.temp.countersUpdRate;
                tcm.temp.trigger4rate = tcm.temp.trigger4cnt/tcm.temp.countersUpdRate;
                tcm.temp.trigger3rate = tcm.temp.trigger3cnt/tcm.temp.countersUpdRate;
                tcm.temp.trigger2rate = tcm.temp.trigger2cnt/tcm.temp.countersUpdRate;
                tcm.temp.trigger1rate = tcm.temp.trigger1cnt/tcm.temp.countersUpdRate;
              }
              else{
                tcm.temp.trigger5rate = 0;
                tcm.temp.trigger4rate = 0;
                tcm.temp.trigger3rate = 0;
                tcm.temp.trigger2rate = 0;
                tcm.temp.trigger1rate = 0;
              }
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/COUNTERS_UPD_RATE", std::to_string(hexValue));
            }
            break;
          case 52:
            if(tcm.temp.bkgrnd0Cnt!=hexValue||firstTime){
              tcm.temp.bkgrnd0Cnt = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/BKGRND0_CNT", std::to_string(hexValue));
            }
            break;
          case 53:
            if(tcm.temp.bkgrnd1Cnt!=hexValue||firstTime){
              tcm.temp.bkgrnd1Cnt = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/BKGRND1_CNT", std::to_string(hexValue));
            }
            break;
          case 54:
            if(tcm.temp.bkgrnd2Cnt!=hexValue||firstTime){
              tcm.temp.bkgrnd2Cnt = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/BKGRND2_CNT", std::to_string(hexValue));
            }
            break;
          case 55:
            if(tcm.temp.bkgrnd3Cnt!=hexValue||firstTime){
              tcm.temp.bkgrnd3Cnt = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/BKGRND3_CNT", std::to_string(hexValue));
            }
            break;
          case 56:
            if(tcm.temp.bkgrnd4Cnt!=hexValue||firstTime){
              tcm.temp.bkgrnd4Cnt = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/BKGRND4_CNT", std::to_string(hexValue));
            }
            break;
          case 57:
            if(tcm.temp.bkgrnd5Cnt!=hexValue||firstTime){
              tcm.temp.bkgrnd5Cnt = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/BKGRND5_CNT", std::to_string(hexValue));
            }
            break;
          case 58:
            if(tcm.temp.bkgrnd6Cnt!=hexValue||firstTime){
              tcm.temp.bkgrnd6Cnt = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/BKGRND6_CNT", std::to_string(hexValue));
            }
            break;
          case 59:
            if(tcm.temp.bkgrnd7Cnt!=hexValue||firstTime){
              tcm.temp.bkgrnd7Cnt = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/BKGRND7_CNT", std::to_string(hexValue));
            }
            break;
          case 60:
            if(tcm.temp.bkgrnd8Cnt!=hexValue||firstTime){
              tcm.temp.bkgrnd8Cnt = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/BKGRND8_CNT", std::to_string(hexValue));
            }
            break;
          case 61:
            if(tcm.temp.bkgrnd9Cnt!=hexValue||firstTime){
              tcm.temp.bkgrnd9Cnt = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/BKGRND9_CNT", std::to_string(hexValue));
            }
            break;
          case 62:
            if(tcm.temp.fpgaTemp!=hexValue||firstTime){
              tcm.temp.fpgaTemp = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/FPGA_TEMP", std::to_string(hexValue * 503.975 / 65536 - 273.15));
            }
            break;
          case 63:
            if(tcm.temp.vPower1!=hexValue||firstTime){
              tcm.temp.vPower1 = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/1VPOWER", std::to_string(hexValue * 3 / 65536.0));
            }
            break;
          case 64:
            if(tcm.temp.vPower18!=hexValue||firstTime){
              tcm.temp.vPower18 = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/18VPOWER", std::to_string(hexValue * 3 / 65536.0));
            }
            break;
          case 65:
            if(tcm.temp.modeSettings!=hexValue||firstTime){
              tcm.temp.modeSettings = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/MODE_SETTINGS", std::to_string(hexValue));
            }
            break;
          case 66:
            if(tcm.temp.laserDivider!=hexValue||firstTime){
              tcm.temp.laserDivider = hexValue;
              float systemClock_MHz = tcm.act.externalClock?40.0789658:40.;
              long long tempValue = std::stoi(value.substr(2,6), nullptr, 16);
              float laserFrequency = systemClock_MHz*std::pow(10,6)/(tempValue==0?1<<24:tempValue);
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/LASER_DIVIDER", std::to_string(hexValue));
              updateTopicAnswer("READOUTCARDS/TCM0/LASER_FREQUENCY", std::to_string(laserFrequency));
            }
            break;
          case 67:
            if(tcm.temp.extSwitches!=hexValue||firstTime){
              tcm.temp.extSwitches = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/EXT_SW", std::to_string(hexValue));
            }
            break;
          case 68:
            if(tcm.temp.attenuator!=hexValue||firstTime){
              tcm.temp.attenuator = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/ATTENUATOR", std::to_string(hexValue));
            }
            break;
          case 69:
            if(tcm.temp.triggerModes!=hexValue||firstTime){
              tcm.temp.triggerModes = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/MODE", std::to_string(hexValue));
            }
            break;
          case 70:
            if(tcm.temp.dsTriggerMask!=hexValue||firstTime){
              tcm.temp.dsTriggerMask = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/DATA_SEL_TRG_MASK", std::to_string(hexValue));
            }
            break;
          case 71:
            if(tcm.temp.bcidOffset!=hexValue||firstTime){
              tcm.temp.bcidOffset = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/BCID_OFFSET", std::to_string(hexValue));
            }
            break;
          case 72:
            if(tcm.temp.dgTriggerMask!=hexValue||firstTime){
              tcm.temp.dgTriggerMask = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/DG_TRG_RESPOND_MASK", std::to_string(hexValue));
            }
            break;
          case 73:
            if(tcm.temp.systemId!=hexValue||firstTime){
              tcm.temp.systemId = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/RDH_FIELDS", std::to_string(hexValue));
            }
            break;
          case 74:
            if(tcm.temp.trgPatternLSB!=hexValue||firstTime){
              tcm.temp.trgPatternLSB = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/TG_PATTERN_1", std::to_string(hexValue));
            }
            break;
          case 75:
            if(tcm.temp.trgPatternMSB!=hexValue||firstTime){
              tcm.temp.trgPatternMSB = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/TG_PATTERN_0", std::to_string(hexValue));
            }
            break;
          case 76:
            if(tcm.temp.cruOrbit!=hexValue||firstTime){
              tcm.temp.cruOrbit = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/CRU_ORBIT", std::to_string(hexValue));
            }
            break;
          case 77:
            if(tcm.temp.cruBcFifos!=hexValue||firstTime){
              tcm.temp.cruBcFifos = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/CRU_BC", std::to_string(hexValue));
            }
            break;
          case 78:
            if(tcm.temp.fifoCount!=hexValue||firstTime){
              tcm.temp.fifoCount = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/FIFO_COUNT", std::to_string(hexValue));
            }
            break;
          case 79:
            if(tcm.temp.selFirstHitDroppedOrbit!=hexValue||firstTime){
              tcm.temp.selFirstHitDroppedOrbit = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/SEL_FIRST_HIT_DROPPED_ORBIT", std::to_string(hexValue));
            }
            break;
          case 80:
            if(tcm.temp.selLastHitDroppedOrbit!=hexValue||firstTime){
              tcm.temp.selLastHitDroppedOrbit = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/SEL_LAST_HIT_DROPPED_ORBIT", std::to_string(hexValue));
            }
            break;
          case 81:
            if(tcm.temp.selHitsDropped!=hexValue||firstTime){
              tcm.temp.selHitsDropped = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/SEL_HITS_DROPPED", std::to_string(hexValue));
            }
            break;
          case 82:
            if(tcm.temp.readoutRate!=hexValue||firstTime){
              tcm.temp.readoutRate = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/READOUT_RATE", std::to_string(hexValue));
            }
            break;
          case 83:
            if(tcm.temp.IPbusFIFOdata!=hexValue||firstTime){
              tcm.temp.IPbusFIFOdata = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/IPbus_FIFO_DATA", std::to_string(hexValue));
            }
            break;
          case 84:
            if(tcm.temp.eventsCount!=hexValue||firstTime){
              tcm.temp.eventsCount = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/EVENTS_COUNT", std::to_string(hexValue));
            }
            break;
          case 85:
            if(tcm.temp.mcodeTime!=hexValue||firstTime){
              tcm.temp.mcodeTime = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/MCODE_TIME", std::to_string(hexValue));
            }
            break;
          case 86:
            if(tcm.temp.fwTime!=hexValue||firstTime){
              tcm.temp.fwTime = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/FW_TIME", std::to_string(hexValue));
            }
            break;
          case 87:
            if(tcm.temp.spiMask!=hexValue||firstTime){
              tcm.temp.spiMask = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/SPI_MASK", std::to_string(hexValue));
            }
            break;
          case 88:
            if(tcm.temp.triggersSuppressionControl!=hexValue||firstTime){
              tcm.temp.triggersSuppressionControl = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/TRIGGERS_SUPPRESSION_CONTROL", std::to_string(hexValue));
            }
            break;
          case 89:
            if(tcm.temp.averageTime!=hexValue||firstTime){
              tcm.temp.averageTime = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/AVERAGE_TIME", std::to_string(hexValue));
            }
            break;
          case 90:
            if(tcm.temp.generatorFreqOffset!=hexValue||firstTime){
              tcm.temp.generatorFreqOffset = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/GENERATOR_FREQ_OFFSET", std::to_string(hexValue));
            }
            break;
          case 91:
            if(tcm.temp.emulationRate!=hexValue||firstTime){
              tcm.temp.emulationRate = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/EMULATION_RATE", std::to_string(hexValue));
            }
            break;
          case 92:
            if(tcm.temp.tgContValue!=hexValue||firstTime){
              tcm.temp.tgContValue = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/TG_CONT_VALUE", std::to_string(hexValue));
            }
            break;
          case 93:
            if(tcm.temp.laserPattern1!=hexValue||firstTime){
              tcm.temp.laserPattern1 = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/LASER_PATTERN_1", std::to_string(hexValue));
            }
            break;
          case 94:
            if(tcm.temp.laserPattern0!=hexValue||firstTime){
              tcm.temp.laserPattern0 = hexValue;
              Print::PrintInfo("sendCommand"+std::to_string(count));
              updateTopicAnswer("READOUTCARDS/TCM0/LASER_PATTERN_0", std::to_string(hexValue));
            }
            break;

        }
        count+=1;
      }
      tcm.temp.triggerCounterTemp++;
      firstTime=false;
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
  }
  else{
    this->publishError("Failure");
    return "failure";
  }
  return "0";*/
  /*if(output!="failure"){

    std::string serviceName="READOUTCARDS/TCM0/";
      string value;
      output.erase(remove(output.begin(), output.end(), '\n'), output.end());
      output = output.substr(8);
      int maxCount=20000, count=0;
      bool firstIteration=true;
      while(output.length()>0&&count<maxCount&&count<services.size()){
        if(!firstIteration){
            output=output.substr(1);
        }
        firstIteration=false;
        
        value = output.substr(13, 8);
        output = output.substr(21);
        long long hexValue = stoll(value, nullptr, 16);
        bool updateParameter = false;
        if(firstTime){
          oldValues.push_back(hexValue);
          updateParameter=true;
        }
        else if(oldValues[count]!=hexValue){
          oldValues[count]=hexValue;
          updateParameter=true;
        }

        if(updateParameter){
          if(services[count]==serviceName+"LASER_DIVIDER"){
            float systemClock_MHz = tcm.act.externalClock?40.0789658:40.;
            long long tempValue = std::stoi(value.substr(2,6), nullptr, 16);
            float laserFrequency = systemClock_MHz*std::pow(10,6)/(tempValue==0?1<<24:tempValue);
            updateTopicAnswer(services[count], std::to_string(hexValue));
            updateTopicAnswer("READOUTCARDS/TCM0/LASER_FREQUENCY", std::to_string(laserFrequency));
          }
          else if(services[count]==serviceName+"DELAY_C"||services[count]==serviceName+"DELAY_A"||services[count]==serviceName+"LASER_DELAY"){
            if (hexValue > 10000) {
              int16_t x = stoi(value, nullptr, 16);
              hexValue=-(~x+1);
            }
            Print::PrintInfo("sendCommand"+std::to_string(count));
            float systemClock_MHz = tcm.act.externalClock?40.0789658:40.;
            float halfBC_ns = 500. / systemClock_MHz;
            float phaseStep_ns = halfBC_ns / 
            //(SERIAL_NUM ? 
            1024;
            // : 1280);
            float returnValue = hexValue*phaseStep_ns;
            updateTopicAnswer(services[count], std::to_string(returnValue));
          }
          else if(services[count]==serviceName+"TEMPERATURE"){
            updateTopicAnswer(services[count], std::to_string(hexValue/10.));
          }
          else if(services[count]==serviceName+"BOARD_STATUS"){
            int pllLockC = (hexValue)&1;
            int pllLockA = (hexValue>>1)&1;
            int systemRestarted = (hexValue>>2)&1;
            int clockSrc = (hexValue>>3)&1;
            int RxReady = (hexValue>>4)&1;
            int forceLocalClock = (hexValue>>10)&1;
            tcm.act.PLLlockA=pllLockA;
            tcm.act.PLLlockC=pllLockC;
            tcm.act.systemRestarted=systemRestarted;
            tcm.act.externalClock=clockSrc;
            tcm.act.GBTRxReady=RxReady;
            tcm.act.forceLocalClock=forceLocalClock;
            updateTopicAnswer(services[count], std::to_string(hexValue));
          }
          else if(services[count]==serviceName+"TRIGGERS_OUTPUTS_MODE"){
            std::bitset<32> binary(hexValue);
            std::string binary_str = binary.to_string();
            updateTopicAnswer(services[count], binary_str);
          }
          else if(services[count]==serviceName+"TRG_ORA_SIGN"||services[count]==serviceName+"TRG_ORC_SIGN"||services[count]==serviceName+"TRG_SC_SIGN"||services[count]==serviceName+"TRG_C_SIGN"||services[count]=="TRG_V_SIGN"){
            updateTopicAnswer(services[count], std::to_string(hexValue/10.));
          }
          else if(services[count]==serviceName+"1VPOWER"||services[count]==serviceName+"18VPOWER"){
            updateTopicAnswer(services[count], std::to_string(hexValue*3/65536.));
          }
          else if(services[count]==serviceName+"FPGA_TEMP"){
            updateTopicAnswer(services[count], std::to_string(hexValue*503.975/65536. - 273.15));
          }
          else{
            updateTopicAnswer(services[count], std::to_string(hexValue));
          }
        }
        count++;
      }
      return "0";
    }
    else{
        this->publishError("Failure");
        return "failure";
    }*/
}
