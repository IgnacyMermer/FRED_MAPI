#include <iostream>
#include <string>
#include <algorithm>
#include <unistd.h>
#include "refresh_mapi_group.h"
#include "Parser/utility.h"
#include "Alfred/print.h"
#include "TCM_values.h"
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include "SWT_creator.h"


RefreshMapiGroup::RefreshMapiGroup(Fred* fred){
    this->fred = fred;
    firstTime=true;;
    boost::property_tree::ptree tree;

    std::string serviceName="LAB/READOUTCARDS/TCM0/";
    sequence="reset";

    std::string fileName = "refresh_data.cfg";

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
                    //tcm.addresses[serviceName+key_value.second.get_value<std::string>()]="00"+key_value.first.substr(key_value.first.length()-2);
                }
            }
        }
    }
    catch(exception& e){
        Print::PrintInfo("error during creating sequence refresh TCM");
        Print::PrintError(e.what());
    }
}

string RefreshMapiGroup::processInputMessage(string input){
    if(input=="go"){
        return sequence;
    }
    else{
        noRpcRequest=true;
        this->publishError("Wrong input parameter");
        return "";
    }
}

string RefreshMapiGroup::processOutputMessage(string output){
    vector< pair <string, string> > requests;
    std::string value;

    if(output!="failure"){
        try{
            string value;
            output.erase(remove(output.begin(), output.end(), '\n'), output.end());
            output = output.substr(8);
            int maxCount=20000, count=0;
            bool firstIteration = true;
            bool updateService = false;
            while(output.length()>0&&count<maxCount&&count<services.size()){
                if(!firstIteration){
                    output=output.substr(1);
                }
                firstIteration=false;
                
                value = output.substr(13, 8);
                output = output.substr(21);
                hexValue = stoll(value, nullptr, 16);

                updateService=false;
                
                if(firstTime){
                    oldValues.push_back(hexValue);
                    updateService=true;
                    requests.push_back(make_pair("LAB/READOUTCARDS/TCM0/WORK_STATUS", "set"));
                }
                else if(oldValues[count]!=hexValue){
                    oldValues[count]=hexValue;
                    updateService=true;   
                }

                if(updateService){
                    std::string returnValue = std::to_string(hexValue);
                    if(services[count].find("FPGA_TEMP")!=string::npos){
                        returnValue = std::to_string(hexValue * 503.975 / 65536 - 273.15);
                    }
                    else if(services[count].find("1VPOWER")!=string::npos){
                        returnValue = std::to_string(hexValue * 3 / 65536.0);
                    }
                    else if(services[count].find("18VPOWER")!=string::npos){
                        returnValue = std::to_string(hexValue * 3 / 65536.0);
                    }
                    else if(services[count].find("TEMPERATURE")!=string::npos){
                        returnValue = std::to_string(hexValue/10.);
                    }
                    else if(services[count].find("LASER_DELAY")!=string::npos){
                        float tempValue = stoi(value, nullptr, 16);
                        if (tempValue > 10000) {
                            int16_t x = stoi(value, nullptr, 16);
                            tempValue=-(~x+1);
                        }
                        float systemClock_MHz = tcm.act.externalClock?40.0789658:40.;
                        float halfBC_ns = 500. / systemClock_MHz;
                        float phaseStep_ns = halfBC_ns / 
                        //(SERIAL_NUM ? 
                        1024 ; 
                        //: 1280);
                        tempValue = tempValue*phaseStep_ns;
                        returnValue = std::to_string(tempValue);
                    }
                    else if(services[count].find("LASER_DIVIDER")!=string::npos||services[count].find("LASER_FREQUENCY")!=string::npos){
                        float systemClock_MHz = tcm.act.externalClock?40.0789658:40.;
                        long long tempValue = std::stoll(value.substr(2,6), nullptr, 16);
                        float laserFrequency = systemClock_MHz*std::pow(10,6)/(tempValue==0?1<<24:tempValue);


                        updateTopicAnswer("READOUTCARDS/TCM0/LASER_FREQUENCY", std::to_string(laserFrequency));
                    
                    
                    }
                    else if(services[count].find("DELAY_A")!=string::npos||services[count].find("DELAY_C")!=string::npos){
                        if (hexValue > 10000) {
                            int16_t x = stoi(value, nullptr, 16);
                            hexValue=-(~x+1);
                        }
                        float systemClock_MHz = tcm.act.externalClock?40.0789658:40.;
                        float halfBC_ns = 500. / systemClock_MHz;
                        float phaseStep_ns = halfBC_ns / 
                        //(SERIAL_NUM ? 
                        1024 ; 
                        //: 1280);
                        returnValue = std::to_string(hexValue*phaseStep_ns);
                    }
                    else if(services[count].find("TRG_ORA_SIGN")!=string::npos||services[count].find("TRG_ORC_SIGN")!=string::npos||services[count].find("TRG_SC_SIGN")!=string::npos
                    ||services[count].find("TRG_C_SIGN")!=string::npos||services[count].find("TRG_V_SIGN")!=string::npos){
                        returnValue = std::to_string(hexValue/128);
                    }
                    requests.push_back(make_pair(services[count], "FRED,"+returnValue));
                    //requests.push_back(make_pair(services[count], "0,0"));
                    
                }
                count++;
            }
            firstTime=false;
            newMapiGroupRequest(requests);
            return "OK";
        }
        catch(exception& e){
            Print::PrintInfo("Error");
            Print::PrintError(e.what());
            this->publishError("error");
            return "Error";
        }
    }
    else{
        Print::PrintError("Failure response from ALF");
        this->publishError("Failure response from ALF");
        return "Failure";
    }
}

