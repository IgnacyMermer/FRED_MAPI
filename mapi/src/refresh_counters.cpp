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
#include <stdlib.h>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>



RefreshCounters::RefreshCounters() {
  finalValue = 0;
  firstTime=true;
  std::string serviceName="READOUTCARDS/TCM0/";

  std::string fileName = "refresh_TCM_counters.cfg";
  boost::property_tree::ptree tree;

  if (!boost::filesystem::exists(fileName)) {
    fileName = "./configuration/" + fileName;
  }

  try{
    boost::property_tree::ini_parser::read_ini(fileName, tree);
    sequence="reset";
    for (const auto& section : tree) {
      if(section.first=="TCM"){
        for (const auto& key_value : section.second) {
          sequence+="\n0x000000000"+key_value.first.substr(key_value.first.length()-2)+"00000000,write\nread";
          servicesCnt.push_back(serviceName+Utility::splitString(key_value.second.get_value<std::string>(),",")[0]);
          servicesRate.push_back(serviceName+Utility::splitString(key_value.second.get_value<std::string>(),",")[1]);
          //tcm.addresses[serviceName+Utility::splitString(key_value.second.get_value<std::string>(),",")[0]]="00"+key_value.first.substr(key_value.first.length()-2);
          //tcm.addresses[serviceName+Utility::splitString(key_value.second.get_value<std::string>(),",")[1]]="00"+key_value.first.substr(key_value.first.length()-2);
        }
      }
    }
  }
  catch(exception& e){
    Print::PrintInfo("error during creating sequence refresh TCM");
    Print::PrintError(e.what());
  }
}

string RefreshCounters::processInputMessage(string input) {
  //sequence = "reset\n0x0000000007000000000,write\nread\n0x0000000007100000000,write\nread\n0x0000000007200000000,write\nread\n0x0000000007300000000,write\nread\n0x0000000007400000000,write\nread\n0x0000000007500000000,write\nread\n0x0000000007600000000,write\nread\n0x0000000007700000000,write\nread\n0x0000000007800000000,write\nread\n0x0000000007900000000,write\nread\n0x0000000007A00000000,write\nread\n0x0000000007B00000000,write\nread\n0x0000000007C00000000,write\nread\n0x0000000007D00000000,write\nread\n0x0000000007E00000000,write\nread";
  return sequence;
}

string RefreshCounters::processOutputMessage(string output) {
  string value;
  if(output!="failure"){
    try {
      output.erase(remove(output.begin(), output.end(), '\n'), output.end());
      output = output.substr(8);
      int maxCount=20000, count=0;
      bool firstIteration=true;
      float triggerRate = 0.0;
      while(output.length()>0&&count<maxCount){
        if(!firstIteration){
          output=output.substr(1);
        }
        firstIteration=false;
        
        value = output.substr(13, 8);
        output = output.substr(21);
        
        long long hexValue = stoll(value, nullptr, 16);
        bool updateData=false;
        if(firstTime){
          updateData=true;
          oldValues.push_back(hexValue);
        }
        else if(oldValues[count]!=hexValue){
          updateData=true;
        }
        if(updateData){
          triggerRate = 0.0;
          auto now = std::chrono::high_resolution_clock::now();

          auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
          int milliseconds_since_epoch = static_cast<int>(duration.count());

          if(firstTime){
            triggerRate = hexValue/1.0;
            oldTimes.push_back(milliseconds_since_epoch);
            
          }
          else {
            //triggerRate = (hexValue - tcm.temp.trigger1cnt)/tcm.temp.countersUpdRate;
            int difference = 0;

            if(firstTime){
              difference = milliseconds_since_epoch;
            }
            else{
              difference = milliseconds_since_epoch - oldTimes[count];
            }
            
            if(difference==0){
              difference=1000;
            }

            triggerRate = abs(hexValue - oldValues[count])/difference*1000.0;

            oldTimes[count] = milliseconds_since_epoch;
          }

          if(hexValue==0){
            triggerRate=0;
          }
          
          if(firstTime){
            oldRates.push_back(0);
            updateTopicAnswer(servicesRate[count], std::to_string(triggerRate));
          }
          else if(oldRates[count]!=triggerRate&&!firstTime){
            oldRates[count] = triggerRate;
            updateTopicAnswer(servicesRate[count], std::to_string(triggerRate));
          }
          
          tcm.temp.trigger5cnt = hexValue;
          updateTopicAnswer(servicesCnt[count], std::to_string(hexValue));
        }
        count++;
        
      }
      firstTime=false;
    }
    catch(exception &e){
      Print::PrintInfo(e.what());
      Print::PrintError("ERROR in connection with ALF!");
    }

    return to_string(0);
  }
  else{
    this->publishError("Failure");
    return "failure";
  }
}

