#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include "histogramReader.h"
#include "Alfred/print.h"
#include "Parser/utility.h"
#include "TCM_values.h"
#include <sstream>
#include <cmath>
#include "SWT_creator.h"


HistogramReader::HistogramReader() {
    finalValue = 0;
}


string HistogramReader::processInputMessage(string input) {
    vector<string> parameters = Utility::splitString(input, ",");
    sequence="reset";
    int number = 0x3000;
    std::stringstream ss;
    ss << std::hex << number;
    std::string hex_str = ss.str();
    for(int i=0;i<500;i++){
        ss.str("");
        ss << std::hex << number;
        std::string hex_str = ss.str();
        sequence+="\n0x0000000"+hex_str+"00000000,write\nread";
        number++;
    }
    return sequence;
    
}

string HistogramReader::processOutputMessage(string output) {
    string value;
    int count=0;
    try {
        std::vector<std::string> strings = Utility::splitString(output,"\n");
        for(int i=0;i<strings.size(); i++){
            if(strings[i].length()>15&&strings[i].substr(13,8)!="0"){
                count++;
                Print::PrintInfo(strings[i].substr(13,8)+" - "+SWT_creator::numberLetter(i));
            }
        }
        Print::PrintInfo("count: "+std::to_string(count));
        return "OK";
    }
    catch (exception &e) {
        Print::PrintInfo(e.what());
        Print::PrintError("ERROR in connection with ALF!");
        finalValue=0;
    }
    return to_string(finalValue);
}
