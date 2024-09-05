#include <iostream>
#include <string>
#include <algorithm>
#include <unistd.h>
#include "RefreshMapiGroup.h"
#include "Parser/utility.h"
#include "Alfred/print.h"
#include "tcmValues.h"
#include "swtCreator.h"


RefreshMapiGroup::RefreshMapiGroup(Fred* fred, std::vector<std::pair<std::string, std::string>> refreshServices):refreshServices(refreshServices){
    this->fred = fred;
    firstTime=true;
    sequence="reset";
    for (const auto& pair : refreshServices) {
        sequence+="\n0x000"+pair.first+"00000000,write\nread";   
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
            while(output.length()>0&&count<maxCount&&count<refreshServices.size()){
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
                    if(tcm.tcmEquations[refreshServices[count].second.substr(4)].first!=""){
                        std::string equation = tcm.tcmEquations[refreshServices[count].second.substr(4)].first;
                        std::vector<std::string> paramNames = Utility::splitString(tcm.tcmEquations[refreshServices[count].second.substr(4)].second,";");
                        std::vector<double> values = std::vector<double>{hexValue};
                        returnValue = std::to_string(Utility::calculateEquation(equation,paramNames,values));
                    }
                    else if(refreshServices[count].second.find("LASER_DELAY")!=string::npos){
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
                    else if(refreshServices[count].second.find("LASER_DIVIDER")!=string::npos||refreshServices[count].second.find("LASER_FREQUENCY")!=string::npos){
                        float systemClock_MHz = tcm.act.externalClock?40.0789658:40.;
                        uint32_t tempValue = std::stoll(value.substr(2,6), nullptr, 16);
                        float laserFrequency = systemClock_MHz*std::pow(10,6)/(tempValue==0?1<<24:tempValue);


                        updateTopicAnswer("READOUTCARDS/TCM0/LASER_FREQUENCY", std::to_string(laserFrequency));
                    
                    
                    }
                    else if(refreshServices[count].second.find("DELAY_A")!=string::npos||refreshServices[count].second.find("DELAY_C")!=string::npos){
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
                    requests.push_back(make_pair(refreshServices[count].second, "FRED,"+returnValue));                    
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

