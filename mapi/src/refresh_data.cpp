#include <algorithm>
#include <iostream>
#include <numeric>
#include <bitset>
#include <string>
#include "refresh_data.h"
#include "Alfred/print.h"
#include "Parser/utility.h"
#include "TCM_values.h"
#include <thread>
#include <sstream>
#include <cmath>
#include <chrono>
#include "../../core/include/dim/dic.hxx"
#include "../../core/include/FREDServer/Fred/Mapi/iterativemapi.h"

std::string numberLetter(int number){
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

RefreshData::RefreshData() {
  /*finalValue = 0;
  firstTime=true;*/
  count = 0;
  sequence = "reset";
  firstTime=true;
  std::string serviceName="READOUTCARDS/TCM0/";
  for(int i=0; i<=5; i++){
      sequence+="\n0x0000000000"+numberLetter(i)+"00000000,write\nread";
  }
  services.push_back(serviceName+"DELAY_A");
  services.push_back(serviceName+"DELAY_C");
  services.push_back(serviceName+"LASER_DELAY");
  services.push_back(serviceName+"ATTENUATOR");
  services.push_back(serviceName+"EXT_SW");
  services.push_back(serviceName+"TEMPERATURE");
  for(int i=7; i<=15; i++){
      sequence+="\n0x0000000000"+numberLetter(i)+"00000000,write\nread";
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
      sequence+="\n0x000000000"+numberLetter(i)+"00000000,write\nread";
      services.push_back(serviceName+"PM_LINK_A"+std::to_string(i%16));
  }
  for(int i=26; i<=32; i++){
      sequence+="\n0x000000000"+numberLetter(i)+"00000000,write\nread";
  }
  services.push_back(serviceName+"SIDE_A_STATUS");
  services.push_back(serviceName+"LASER_DIVIDER");
  services.push_back(serviceName+"LASER_PATTERN_1");
  services.push_back(serviceName+"LASER_PATTERN_0");
  services.push_back(serviceName+"SPI_MASK");
  services.push_back(serviceName+"TRIGGERS_SUPPRESSION_CONTROL");
  services.push_back(serviceName+"AVERAGE_TIME");
  for(int i=48; i<=57; i++){
      sequence+="\n0x000000000"+numberLetter(i)+"00000000,write\nread";
      services.push_back(serviceName+"PM_LINK_A"+std::to_string(i%16));
  }
  sequence+="\n0x000000000"+numberLetter(58)+"00000000,write\nread";
  services.push_back(serviceName+"SIDE_C_STATUS");
  sequence+="\n0x000000000"+numberLetter(0x50)+"00000000,write\nread";
  services.push_back(serviceName+"COUNTERS_UPD_RATE");
  for(int i=0x60; i<=0x69; i++){
      sequence+="\n0x000000000"+numberLetter(i)+"00000000,write\nread";
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
  sequence+="\n0x000000000"+numberLetter(0x6A)+"00000000,write\nread";
  services.push_back(serviceName+"TRIGGERS_OUTPUTS_MODE");
  for(int i=0x70; i<=0x74; i++){
      sequence+="\n0x000000000"+numberLetter(i)+"00000000,write\nread";
  }
  services.push_back(serviceName+"TRIGGER5_CNT");
  services.push_back(serviceName+"TRIGGER4_CNT");
  services.push_back(serviceName+"TRIGGER2_CNT");
  services.push_back(serviceName+"TRIGGER1_CNT");
  services.push_back(serviceName+"TRIGGER3_CNT");
  for(int i=0x75; i<=0x7E; i++){
      sequence+="\n0x000000000"+numberLetter(i)+"00000000,write\nread";
      services.push_back(serviceName+"BKGRND"+std::to_string(i-0x75)+"_CNT");
  }
  sequence+="\n0x000000000"+numberLetter(0xD8)+"00000000,write\nread";
  services.push_back(serviceName+"MODE_SETTINGS");
  sequence+="\n0x000000000"+numberLetter(0xD9)+"00000000,write\nread";
  services.push_back(serviceName+"DG_TRG_RESPOND_MASK");
  sequence+="\n0x000000000"+numberLetter(0xDA)+"00000000,write\nread";
  services.push_back(serviceName+"DG_BUNCH_PATTERN");
  sequence+="\n0x000000000"+numberLetter(0xDB)+"00000000,write\nread";
  services.push_back(serviceName+"TG_SINGLE_VALUE");
  sequence+="\n0x000000000"+numberLetter(0xDC)+"00000000,write\nread";
  services.push_back(serviceName+"TG_PATTERN_1");
  sequence+="\n0x000000000"+numberLetter(0xDD)+"00000000,write\nread";
  services.push_back(serviceName+"TG_PATTERN_0");
  sequence+="\n0x000000000"+numberLetter(0xDE)+"00000000,write\nread";
  services.push_back(serviceName+"TG_CONT_VALUE");
  sequence+="\n0x000000000"+numberLetter(0xDF)+"00000000,write\nread";
  services.push_back(serviceName+"EMULATION_RATE");
  sequence+="\n0x000000000"+numberLetter(0xE0)+"00000000,write\nread";
  services.push_back(serviceName+"GENERATOR_FREQ_OFFSET");
  sequence+="\n0x000000000"+numberLetter(0xE1)+"00000000,write\nread";
  services.push_back(serviceName+"RDH_FIELDS");
  sequence+="\n0x000000000"+numberLetter(0xE3)+"00000000,write\nread";
  services.push_back(serviceName+"BCID_OFFSET");
  sequence+="\n0x000000000"+numberLetter(0xE4)+"00000000,write\nread";
  services.push_back(serviceName+"DATA_SEL_TRG_MASK");
  sequence+="\n0x000000000"+numberLetter(0xE8)+"00000000,write\nread";
  services.push_back(serviceName+"MODES_STATUS");
  sequence+="\n0x000000000"+numberLetter(0xE9)+"00000000,write\nread";
  services.push_back(serviceName+"CRU_ORBIT");
  sequence+="\n0x000000000"+numberLetter(0xEA)+"00000000,write\nread";
  services.push_back(serviceName+"CRU_BC");
  sequence+="\n0x000000000"+numberLetter(0xEB)+"00000000,write\nread";
  services.push_back(serviceName+"FIFO_COUNT");
  sequence+="\n0x000000000"+numberLetter(0xEC)+"00000000,write\nread";
  services.push_back(serviceName+"SEL_FIRST_HIT_DROPPED_ORBIT");
  sequence+="\n0x000000000"+numberLetter(0xED)+"00000000,write\nread";
  services.push_back(serviceName+"SEL_LAST_HIT_DROPPED_ORBIT");
  sequence+="\n0x000000000"+numberLetter(0xEE)+"00000000,write\nread";
  services.push_back(serviceName+"SEL_HITS_DROPPED");
  sequence+="\n0x000000000"+numberLetter(0xEF)+"00000000,write\nread";
  services.push_back(serviceName+"READOUT_RATE");
  sequence+="\n0x000000000"+numberLetter(0xF0)+"00000000,write\nread";
  services.push_back(serviceName+"IPbus_FIFO_DATA");
  sequence+="\n0x000000000"+numberLetter(0xF1)+"00000000,write\nread";
  services.push_back(serviceName+"EVENTS_COUNT");
  sequence+="\n0x000000000"+numberLetter(0xF7)+"00000000,write\nread";
  services.push_back(serviceName+"MCODE_TIME");
  sequence+="\n0x000000000"+numberLetter(0xFC)+"00000000,write\nread";
  services.push_back(serviceName+"FPGA_TEMP");
  sequence+="\n0x000000000"+numberLetter(0xFD)+"00000000,write\nread";
  services.push_back(serviceName+"1VPOWER");
  sequence+="\n0x000000000"+numberLetter(0xFE)+"00000000,write\nread";
  services.push_back(serviceName+"18VPOWER");
  sequence+="\n0x000000000"+numberLetter(0xFF)+"00000000,write\nread";
  services.push_back(serviceName+"FW_TIME");
}

string RefreshData::processInputMessage(string input) {
  //sequence = "reset\n0x0000000000100000000,write\nread\n0x0000000000200000000,write\nread\n0x0000000000500000000,write\nread\n0x0000000000700000000,write\nread\n0x0000000000F00000000,write\nread\n0x0000000006A00000000,write\nread\n0x0000000006000000000,write\nread\n0x0000000006100000000,write\nread\n0x0000000006200000000,write\nread\n0x0000000006300000000,write\nread\n0x0000000006800000000,write\nread\n0x0000000006900000000,write\nread\n0x0000000006400000000,write\nread\n0x0000000006500000000,write\nread\n0x0000000006600000000,write\nread\n0x0000000006700000000,write\nread\n0x0000000007000000000,write\nread\n0x0000000007100000000,write\nread\n0x0000000007400000000,write\nread\n0x0000000007200000000,write\nread\n0x0000000007300000000,write\nread\n0x0000000001000000000,write\nread\n0x0000000001100000000,write\nread\n0x0000000001200000000,write\nread\n0x0000000001300000000,write\nread\n0x0000000001400000000,write\nread\n0x0000000001500000000,write\nread\n0x0000000001600000000,write\nread\n0x0000000001700000000,write\nread\n0x0000000001800000000,write\nread\n0x0000000001900000000,write\nread\n0x0000000003000000000,write\nread\n0x0000000003100000000,write\nread\n0x0000000003200000000,write\nread\n0x0000000003300000000,write\nread\n0x0000000003400000000,write\nread\n0x0000000003500000000,write\nread\n0x0000000003600000000,write\nread\n0x0000000003700000000,write\nread\n0x0000000003800000000,write\nread\n0x0000000003900000000,write\nread\n0x0000000000000000000,write\nread\n0x0000000000800000000,write\nread\n0x0000000000900000000,write\nread\n0x0000000000A00000000,write\nread\n0x0000000000B00000000,write\nread\n0x0000000000C00000000,write\nread\n0x0000000000D00000000,write\nread\n0x000000000E800000000,write\nread\n0x0000000001A00000000,write\nread\n0x0000000003A00000000,write\nread\n0x0000000005000000000,write\nread\n0x0000000007500000000,write\nread\n0x0000000007600000000,write\nread\n0x0000000007700000000,write\nread\n0x0000000007800000000,write\nread\n0x0000000007900000000,write\nread\n0x0000000007A00000000,write\nread\n0x0000000007B00000000,write\nread\n0x0000000007C00000000,write\nread\n0x0000000007D00000000,write\nread\n0x0000000007E00000000,write\nread\n0x000000000FC00000000,write\nread\n0x000000000FD00000000,write\nread\n0x000000000FE00000000,write\nread\n0x000000000D800000000,write\nread\n0x0000000001B00000000,write\nread\n0x0000000000400000000,write\nread\n0x0000000000300000000,write\nread\n0x0000000000E00000000,write\nread\n0x000000000E400000000,write\nread\n0x000000000E300000000,write\nread\n0x000000000D900000000,write\nread\n0x000000000E100000000,write\nread\n0x000000000DC00000000,write\nread\n0x000000000DD00000000,write\nread\n0x000000000E900000000,write\nread\n0x000000000EA00000000,write\nread\n0x000000000EB00000000,write\nread\n0x000000000EC00000000,write\nread\n0x000000000ED00000000,write\nread\n0x000000000EE00000000,write\nread\n0x000000000EF00000000,write\nread\n0x000000000F000000000,write\nread\n0x000000000F100000000,write\nread\n0x000000000F700000000,write\nread\n0x000000000FF00000000,write\nread\n0x0000000001E00000000,write\nread\n0x0000000001F00000000,write\nread\n0x0000000002000000000,write\nread\n0x000000000E000000000,write\nread\n0x000000000DF00000000,write\nread\n0x000000000DE00000000,write\nread\n0x0000000001C00000000,write\nread\n0x0000000001D00000000,write\nread";
  return sequence;
}

string RefreshData::processOutputMessage(string output) {
  if(output!="failure"){
    std::string serviceName="READOUTCARDS/TCM0/";
    string value;
    output.erase(remove(output.begin(), output.end(), '\n'), output.end());
    output = output.substr(8);
    int maxCount=20000, count=0;
    bool firstIteration=true;
    if(firstTime){
        updateTopicAnswer("READOUTCARDS/TCM0/WORK_STATUS", std::to_string(1));
      }
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
  }
}
