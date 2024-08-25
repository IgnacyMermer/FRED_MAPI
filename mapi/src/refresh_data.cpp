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
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <cmath>
#include <chrono>
#include "SWT_creator.h"


RefreshData::RefreshData() {
  count = 0;
  sequence = "reset";
  firstTime=true;
  std::string serviceName="READOUTCARDS/TCM0/";

  std::string fileName = "refresh_data.cfg";
  boost::property_tree::ptree tree;

  if (!boost::filesystem::exists(fileName)) {
    fileName = "./configuration/" + fileName;
  }

  try{
    boost::property_tree::ini_parser::read_ini(fileName, tree);
    
    for (const auto& section : tree) {
      if(section.first=="TCM"){
        for (const auto& key_value : section.second) {
          sequence+="\n0x000000000"+key_value.first.substr(key_value.first.length()-2)+"00000000,write\nread";
          services.push_back(serviceName+key_value.second.get_value<std::string>());
        }
      }
    }
  }
  catch(exception& e){
    Print::PrintInfo("error during creating sequence refresh TCM");
    Print::PrintError(e.what());
  }

}

string RefreshData::processInputMessage(string input) {
  return sequence;
}

string RefreshData::processOutputMessage(string output) {
  if(output!="failure"){
    try{
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
            if (hexValue > 50000) {
              int16_t x = stoi(value, nullptr, 16);
              hexValue=-(~x+1);
            }
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
            updateTopicAnswer(services[count], std::to_string(hexValue/128));
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
    }
    catch(exception& e){
      Print::PrintError("Error in refresh TCM data");
      Print::PrintError(e.what());
      this->publishError("Error");
      return "Error";
    }
    return "0";
  }
  else{
    this->publishError("Failure");
    return "failure";
  }
}
