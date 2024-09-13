#include <iostream>
#include <string>
#include <algorithm>
#include <unistd.h>
#include "RefreshMapiGroup.h"
#include "Parser/utility.h"
#include "Alfred/print.h"
#include "registerData.h"
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
                    if(registersData.equations[refreshServices[count].second.substr(4)].first!=""){
                        if(registersData.registerFields[refreshServices[count].second.substr(4)].size()==1&&registersData.registerFields[refreshServices[count].second.substr(4)][0][3]==1){
                            if (hexValue > 50000) {
                                int16_t x = stoi(value, nullptr, 16);
                                hexValue=-(~x+1);
                            }
                        }
                        std::string equation = registersData.equations[refreshServices[count].second.substr(4)].first;
                        std::vector<std::string> paramNames = Utility::splitString(registersData.equations[refreshServices[count].second.substr(4)].second,";");
                        std::vector<double> values = std::vector<double>{(double)hexValue};
                        if(paramNames.size()>1&&paramNames[1]=="systemClock"){
                            values.push_back(registersData.act.externalClock?40.0789658:40.);
                        }
                        returnValue = std::to_string(Utility::calculateEquation(equation,paramNames,values));
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

