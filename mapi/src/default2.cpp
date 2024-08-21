#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include "default2.h"
#include "Alfred/print.h"
#include "Parser/utility.h"
#include "TCM_values.h"
#include <sstream>
#include <cmath>


Default2::Default2(std::string endpointParam) {
    finalValue = 0;
    endpoint=endpointParam;
}

string Default2::processInputMessage(string input) {
    std::string address="", sequence="";
    vector<string> parameters = Utility::splitString(input, ",");

    if(endpoint=="VTIME_LOW"){
        address="00000008";
        if (input == ""||input == "set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            int num = std::stoi(parameters[0].substr(2));
            if(num<-512||num>511){
                noRpcRequest=true;
                this->publishError("Value out of correct range");
                return "";
            }
            std::stringstream ss;
            ss << std::hex << num;
            std::string hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            sequence="reset\n0x001"+address+data+",write\nread";
        }
        else{
            noRpcRequest=true;
            this->publishError("Parameter can be only read or write");
            sequence="";
        }
        return sequence;
    }
    else if(endpoint=="VTIME_HIGH"){
        address="00000009";
        if (input == ""||input == "set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            int num = std::stoi(parameters[0].substr(2));
            if(num<-512||num>511){
                noRpcRequest=true;
                this->publishError("Value out of correct range");
                return "";
            }
            std::stringstream ss;
            ss << std::hex << num;
            std::string hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            sequence="reset\n0x001"+address+data+",write\nread";
        }
        else{
            this->publishError("Parameter can be only read or write");
            sequence="";
            noRpcRequest=true;
        }
        return sequence;
        
    }
    else if(endpoint=="SC_LEVEL_A"){
        address="0000000A";
        if (input == ""||input == "set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            int num = std::stoi(parameters[0].substr(2));
            if(num<0||num>65535){
                noRpcRequest=true;
                this->publishError("Value out of correct range");
                return "";
            }
            std::stringstream ss;
            ss << std::hex << num;
            std::string hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            sequence="reset\n0x001"+address+data+",write\nread";
        }
        else{
            this->publishError("Parameter can be only read or write");
            sequence="";
            noRpcRequest=true;
        }
        return sequence;
    }
    else if(endpoint=="SC_LEVEL_C"){
        address="0000000B";
        if (input == ""||input == "set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            int num = std::stoi(parameters[0].substr(2));
            if(num<0||num>65535){
                this->publishError("Value out of correct range");
                noRpcRequest=true;
                return "";
            }
            std::stringstream ss;
            ss << std::hex << num;
            std::string hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            sequence="reset\n0x001"+address+data+",write\nread";
        }
        else{
            this->publishError("Parameter can be only read or write");
            sequence="";
            noRpcRequest=true;
        }
        return sequence;
    }
    else if(endpoint=="C_LEVEL_A"){
        address="0000000C";
        if (input == ""||input == "set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            int num = std::stoi(parameters[0].substr(2));
            if(num<0||num>65535){
                this->publishError("Value out of correct range");
                noRpcRequest=true;
                return "";
            }
            std::stringstream ss;
            ss << std::hex << num;
            std::string hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            sequence="reset\n0x001"+address+data+",write\nread";
        }
        else{
            this->publishError("Parameter can be only read or write");
            sequence="";
            noRpcRequest=true;
        }
        return sequence;
    }
    else if(endpoint=="C_LEVEL_C"){
        address="0000000D";
        if (input == ""||input == "set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            int num = std::stoi(parameters[0].substr(2));
            if(num<0||num>65535){
                this->publishError("Value out of correct range");
                noRpcRequest=true;
                return "";
            }
            std::stringstream ss;
            ss << std::hex << num;
            std::string hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            sequence="reset\n0x001"+address+data+",write\nread";
        }
        else{
            this->publishError("Parameter can be only read or write");
            sequence="";
            noRpcRequest=true;
        }
        return sequence;
    }
    else if(endpoint=="LASER_DELAY"){
        address="00000002";
        if (input == ""||input == "set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            float num = std::stof(parameters[0]);
            float systemClock_MHz = tcm.act.externalClock?40.0789658:40.;
            float halfBC_ns = 500. / systemClock_MHz;
            float phaseStep_ns = halfBC_ns / (
            //SERIAL_NUM ? 
            1024 
            //: 1280
            );
            num = num / phaseStep_ns;
            if(num<-1024||num>1024){
                this->publishError("Value out of correct range");
                noRpcRequest=true;
                return "";
            }
            std::stringstream ss;
            int numInt = num;
            ss << std::hex << numInt;
            std::string hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            sequence="reset\n0x001"+address+data+",write\nread";
        }
        else{
            this->publishError("Parameter can be only read or write");
            sequence="";
            noRpcRequest=true;
        }
        return sequence;
    }
    else if(endpoint=="ATTENUATOR"){
        address="00000003";
        std::string hex_str = "";
        int index = std::stoi(parameters[0]);
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            int num = std::stoi(parameters[0]);
            std::stringstream ss;
            ss << std::hex << num;
            std::string hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            sequence="reset\n0x001"+address+data+",write\nread";
        }
        else{
            this->publishError("wrong parameters");
            sequence = "";
            noRpcRequest=true;
        }
        return sequence;
    }
    else if(endpoint=="EXT_SW"){
        address="00000004";
        if (input == ""||input == "set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            int num = std::stoi(parameters[0]);
            std::stringstream ss;
            ss << std::hex << num;
            std::string hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            sequence="reset\n0x001"+address+data+",write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="3"){
            int num = std::stoi(parameters[0]);
            std::stringstream ss;
            ss << std::hex << (1 << num);
            std::string hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            sequence = "reset\n0x002"+address+"FFFFFFFF,write\nread\n0x003"+address+data+",write\nread\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="2"){
            int num = std::stoi(parameters[0]);
            int temp = 0xFFFFFFFF;
            temp-=(1 << num);
            std::stringstream ss;
            ss << std::hex << temp;
            std::string data = ss.str();
            sequence = "reset\n0x002"+address+data+",write\nread\n0x003"+address+"00000000,write\nread\n0x000"+address+"00000000,write\nread";
        }
        else{
            this->publishError("Parameter can be only read or write or RMW");
            sequence="";
            noRpcRequest=true;
        }
        return sequence;
    }
    else if(endpoint=="TEMPERATURE"){
        address="00000005";
        vector<string> parameters = Utility::splitString(input, ",");

        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else{
            noRpcRequest=true;
            sequence="";
            this->publishError("Readonly parameter");
        }
    }
    else if(endpoint=="TRIGGER5_CNT"){
        address="00000070";
    }
    else if(endpoint=="TRIGGER4_CNT"){
        address="00000071";
    }
    else if(endpoint=="TRIGGER3_CNT"){
        address="00000074";
    }
    else if(endpoint=="TRIGGER2_CNT"){
        address="00000072";
    }
    else if(endpoint=="TRIGGER1_CNT"){
        address="00000073";
    }
    else if(endpoint=="MODE"){
        std::string hex_str = "";
        address="0000000E";
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            int num = std::stoi(parameters[0]);
            std::stringstream ss;
            ss << std::hex << num;
            std::string hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            sequence="reset\n0x001"+address+data+",write\nread";
        }
        else if(parameters.size()>2&&(parameters[0]=="1"||parameters[0]=="2")){
            int numValue = std::stoi(parameters[2]);
            int temp = 0xFFFFFFFF;
            temp-=6;
            std::stringstream ss;
            ss << std::hex << temp;
            hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            temp=numValue*2;
            std::stringstream ss2;
            ss2 << std::hex << temp;
            std::string data2 = "";
            hex_str = ss2.str();
            for(int i=0; i<8-hex_str.length(); i++){
                data2+="0";
            }
            data2+=hex_str;
            sequence = "reset\n0x002"+address+data+",write\nread\n0x003"+address+data2+",write\nread\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>2&&(parameters[0]=="4"||parameters[0]=="5"||parameters[0]=="6"||parameters[0]=="7")){

            int numValue = std::stoi(parameters[2]);
            int temp = 0xFFFFFFFF;
            temp-=240;
            std::stringstream ss;
            ss << std::hex << temp;
            hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            temp=numValue*16;
            std::stringstream ss2;
            ss2 << std::hex << temp;
            std::string data2 = "";
            hex_str = ss2.str();
            for(int i=0; i<8-hex_str.length(); i++){
                data2+="0";
            }
            data2+=hex_str;
            sequence = "reset\n0x002"+address+data+",write\nread\n0x003"+address+data2+",write\nread\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="3"){
            int num = std::stoi(parameters[0]);
            std::string data="";
            std::stringstream ss;
            ss << std::hex << (1 << num);
            hex_str = ss.str();

            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            sequence = "reset\n0x002"+address+"FFFFFFFF,write\nread\n0x003"+address+data+",write\nread\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="2"){
            int num = std::stoi(parameters[0]);
            int temp = 0xFFFFFFFF;
            temp-=(1 << num);
            std::stringstream ss;
            ss << std::hex << temp;
            std::string data = ss.str();
            sequence = "reset\n0x002"+address+data+",write\nread\n0x003"+address+"00000000,write\nread\n0x000"+address+"00000000,write\nread";
        }
        else{
            this->publishError("wrong parameters");
            noRpcRequest=true;
            sequence="";
        }
        return sequence;
    }
    else if(endpoint=="MODES_STATUS"){
        address="000000E8";
        vector<string> parameters = Utility::splitString(input, ",");

        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else{
            sequence="";
            noRpcRequest=true;
            this->publishError("Readonly parameter");
        }
        return sequence;
    }
    else if(endpoint=="SIDE_A_STATUS"){
        address="0000001A";
        if (input == ""||input == "set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            int num = std::stoi(parameters[0]);
            std::stringstream ss;
            ss << std::hex << num;
            std::string hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            sequence="reset\n0x001"+address+data+",write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="3"){
            int num = std::stoi(parameters[0]);
            std::string data="";
            std::stringstream ss;
            ss << std::hex << (1 << num);
            std::string hex_str = ss.str();

            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            sequence = "reset\n0x002"+address+"FFFFFFFF,write\nread\n0x003"+address+data+",write\nread\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="2"){
            int num = std::stoi(parameters[0]);
            int temp = 0xFFFFFFFF;
            temp-=(1 << num);
            std::stringstream ss;
            ss << std::hex << temp;
            std::string data = ss.str();
            sequence = "reset\n0x002"+address+data+",write\nread\n0x003"+address+"00000000,write\nread\n0x000"+address+"00000000,write\nread";
        }
        else{
            this->publishError("Parameter can be only read or RMW");
            sequence="";
            noRpcRequest=true;
        }
        return sequence;
    }
    else if(endpoint=="SIDE_C_STATUS"){
        address="0000003A";
        if (input == ""||input == "set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="3"){
            int num = std::stoi(parameters[0]);
            std::string data="";
            std::stringstream ss;
            ss << std::hex << (1 << num);
            std::string hex_str = ss.str();

            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            sequence = "reset\n0x002"+address+"FFFFFFFF,write\nread\n0x003"+address+data+",write\nread\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="2"){
            int num = std::stoi(parameters[0]);
            int temp = 0xFFFFFFFF;
            temp-=(1 << num);
            std::stringstream ss;
            ss << std::hex << temp;
            std::string data = ss.str();
            sequence = "reset\n0x002"+address+data+",write\nread\n0x003"+address+"00000000,write\nread\n0x000"+address+"00000000,write\nread";
        }
        else{
            this->publishError("Parameter can be only read or RMW");
            sequence="";
            noRpcRequest=true;
        }
        return sequence;
    }
    else if(endpoint=="BKGRND0_CNT"){
        address="00000075";
    }
    else if(endpoint=="BKGRND1_CNT"){
        address="00000076";
    }
    else if(endpoint=="BKGRND2_CNT"){
        address="00000077";
    }
    else if(endpoint=="BKGRND3_CNT"){
        address="00000078";
    }
    else if(endpoint=="BKGRND4_CNT"){
        address="00000079";
    }
    else if(endpoint=="BKGRND5_CNT"){
        address="0000007A";
    }
    else if(endpoint=="BKGRND6_CNT"){
        address="0000007B";
    }
    else if(endpoint=="BKGRND7_CNT"){
        address="0000007C";
    }
    else if(endpoint=="BKGRND8_CNT"){
        address="0000007D";
    }
    else if(endpoint=="BKGRND9_CNT"){
        address="0000007E";
    }
    else if(endpoint=="FPGA_TEMP"){
        address="000000FC";
        vector<string> parameters = Utility::splitString(input, ",");

        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else{
            noRpcRequest=true;
            sequence="";
            this->publishError("Readonly parameter");
        }
        return sequence;
    }
    else if(endpoint=="1VPOWER"){
        address="000000FD";
        vector<string> parameters = Utility::splitString(input, ",");
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else{
            sequence="";
            this->publishError("Readonly parameter");
            noRpcRequest=true;
        }
        return sequence;
    }
    else if(endpoint=="18VPOWER"){
        address="000000FE";
        vector<string> parameters = Utility::splitString(input, ",");
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else{
            sequence="";
            noRpcRequest=true;
            this->publishError("Readonly parameter");
        }
        return sequence;
    }
    else if(endpoint=="MODE_SETTINGS"){
        address="000000D8";
        std::string hex_str = "";
        int num = 0;
        if(parameters[0].rfind("0x", 0)==0){
            num = std::stoi(parameters[0].substr(2), nullptr, 16);
        }
        else{
            num = std::stoi(parameters[0]);
        }
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            //int num = std::stoi(parameters[0]);
            Print::PrintInfo(std::to_string(num));
            std::stringstream ss;
            ss << std::hex << num;
            std::string hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            sequence="reset\n0x001"+address+data+",write\nread";
        }
        else if(parameters.size()>2&&(num>=0&&num<=3)){
            int numValue = std::stoi(parameters[2]);
            if(numValue<0||numValue>2){
                this->publishError("dg mode out of range");
                return "";
            }
            int temp = 0xFFFFFFF0;
            std::stringstream ss;
            ss << std::hex << temp;
            hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            temp=numValue;
            std::stringstream ss2;
            ss2 << std::hex << temp;
            std::string data2 = "";
            hex_str = ss2.str();
            for(int i=0; i<8-hex_str.length(); i++){
                data2+="0";
            }
            data2+=hex_str;
            sequence = "reset\n0x002"+address+data+",write\nread\n0x003"+address+data2+",write\nread\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>2&&(num>=4&&num<=7)){
            int numValue = std::stoi(parameters[2]);
            if(numValue<0||numValue>2){
                this->publishError("tg mode out of range");
                return "";
            }
            int temp = 0xFFFFFF0F;
            std::stringstream ss;
            ss << std::hex << temp;
            hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            temp=numValue*std::pow(2,4);
            std::stringstream ss2;
            ss2 << std::hex << temp;
            std::string data2 = "";
            hex_str = ss2.str();
            for(int i=0; i<8-hex_str.length(); i++){
                data2+="0";
            }
            data2+=hex_str;
            sequence = "reset\n0x002"+address+data+",write\nread\n0x003"+address+data2+",write\nread\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>2&&(num>=16&&num<=19)){
            int numValue = std::stoi(parameters[2]);
            if(numValue<0||numValue>2){
                this->publishError("CTP emulation run type out of range");
                return "";
            }
            int temp = 0xFFF0FFFF;
            std::stringstream ss;
            ss << std::hex << temp;
            hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            temp=numValue*std::pow(2,16);
            std::stringstream ss2;
            ss2 << std::hex << temp;
            std::string data2 = "";
            hex_str = ss2.str();
            for(int i=0; i<8-hex_str.length(); i++){
                data2+="0";
            }
            data2+=hex_str;
            sequence = "reset\n0x002"+address+data+",write\nread\n0x003"+address+data2+",write\nread\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="3"){
            int num = std::stoi(parameters[0]);
            std::string data="";
            std::stringstream ss;
            ss << std::hex << (1 << num);
            hex_str = ss.str();

            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            sequence = "reset\n0x002"+address+"FFFFFFFF,write\nread\n0x003"+address+data+",write\nread\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="2"){
            int num = std::stoi(parameters[0]);
            int temp = 0xFFFFFFFF;
            temp-=(1 << num);
            std::stringstream ss;
            ss << std::hex << temp;
            std::string data = ss.str();
            sequence = "reset\n0x002"+address+data+",write\nread\n0x003"+address+"00000000,write\nread\n0x000"+address+"00000000,write\nread";
        }
        else{
            sequence="";
            noRpcRequest=true;
        }
        return sequence;
    }
    else if(endpoint=="LASER_DIVIDER"){

        //do sprawdzenia wartosc wyswietlana

        address="0000001B";
        std::string hex_str = "";
        int index = std::stoi(parameters[0]);
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            long long num = std::stoll(parameters[0]);
            std::stringstream ss;
            ss << std::hex << num;
            std::string hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            sequence="reset\n0x001"+address+data+",write\nread";
        }
        else if(parameters.size()>2&&(index>=0&&index<=23)){
            int numValue = std::stoi(parameters[2]);
            int temp = 0xFF000000;
            std::stringstream ss;
            ss << std::hex << temp;
            hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            temp=numValue;
            std::stringstream ss2;
            ss2 << std::hex << temp;
            std::string data2 = "";
            hex_str = ss2.str();
            for(int i=0; i<8-hex_str.length(); i++){
                data2+="0";
            }
            data2+=hex_str;
            sequence = "reset\n0x002"+address+data+",write\nread\n0x003"+address+data2+",write\nread\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>2&&(index>=25&&index<=29)){
            sequence="";
            noRpcRequest=true;
        }
        else if(parameters.size()>1&&parameters[1]=="3"){
            int num = std::stoi(parameters[0]);
            std::string data="";
            std::stringstream ss;
            ss << std::hex << (1 << num);
            hex_str = ss.str();

            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            sequence = "reset\n0x002"+address+"FFFFFFFF,write\nread\n0x003"+address+data+",write\nread\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="2"){
            int num = std::stoi(parameters[0]);
            long long temp = 0xFFFFFFFF;
            temp-=(1 << num);
            std::stringstream ss;
            ss << std::hex << temp;
            std::string data = ss.str();
            sequence = "reset\n0x002"+address+data+",write\nread\n0x003"+address+"00000000,write\nread\n0x000"+address+"00000000,write\nread";
        }
        else{
            sequence="";
            noRpcRequest=true;
            this->publishError("wrong parameters");
        }
        return sequence;
    }
    else if(endpoint=="DATA_SEL_TRG_MASK"){
        address="000000E4";
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            int num = 0;
            if(parameters[0].rfind("0x", 0)==0){
                num = std::stoi(parameters[0].substr(2), nullptr, 16);
            }
            else{
                num = std::stoi(parameters[0]);
            }
            std::stringstream ss;
            ss << std::hex << num;
            std::string hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            sequence="reset\n0x001"+address+data+",write\nread";
        }
        else{
            this->publishError("wrong parameters");
            sequence="";
            noRpcRequest=true;
        }
        return sequence;
    }
    else if(endpoint=="BCID_OFFSET"){
        address="000000E3";
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            int num = 0;
            if(parameters[0].rfind("0x", 0)==0){
                num = std::stoi(parameters[0].substr(2), nullptr, 16);
            }
            else{
                num = std::stoi(parameters[0]);
            }
            std::stringstream ss;
            ss << std::hex << num;
            std::string hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            sequence="reset\n0x001"+address+data+",write\nread";
        }
        else{
            this->publishError("wrong parameters");
            sequence="";
            noRpcRequest=true;
        }
        return sequence;
    }
    else if(endpoint=="DG_TRG_RESPOND_MASK"){
        address="000000D9";
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            int num = 0;
            if(parameters[0].rfind("0x", 0)==0){
                num = std::stoi(parameters[0].substr(2), nullptr, 16);
            }
            else{
                num = std::stoi(parameters[0]);
            }
            std::stringstream ss;
            ss << std::hex << num;
            std::string hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            sequence="reset\n0x001"+address+data+",write\nread";
        }
        else{
            this->publishError("wrong parameters");
            sequence="";
            noRpcRequest=true;
        }
        return sequence;
    }
    else if(endpoint=="RDH_FIELDS"){
        address="000000E1";
    }
    else if(endpoint=="CRU_ORBIT"){
        address="000000E9";
        vector<string> parameters = Utility::splitString(input, ",");
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else{
            sequence="";
            noRpcRequest=true;
            this->publishError("Readonly parameter");
        }
        return sequence;
    }
    else if(endpoint=="CRU_BC"){
        address="000000EA";
        vector<string> parameters = Utility::splitString(input, ",");

        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else{
            sequence="";
            noRpcRequest=true;
            this->publishError("Readonly parameter");
        }
        return sequence;
    }
    else if(endpoint=="FIFO_COUNT"){
        address="000000EB";
        vector<string> parameters = Utility::splitString(input, ",");
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else{
            sequence="";
            this->publishError("Readonly parameter");
            noRpcRequest=true;
        }
        return sequence;
    }
    else if(endpoint=="SEL_FIRST_HIT_DROPPED_ORBIT"){
        address="000000EC";
        vector<string> parameters = Utility::splitString(input, ",");
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else{
            sequence="";
            this->publishError("Readonly parameter");
            noRpcRequest=true;
        }
        return sequence;
    }
    else if(endpoint=="SEL_LAST_HIT_DROPPED_ORBIT"){
        address="000000ED";
        vector<string> parameters = Utility::splitString(input, ",");
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else{
            sequence="";
            noRpcRequest=true;
            this->publishError("Readonly parameter");
        }
        return sequence;
    }
    else if(endpoint=="SEL_HITS_DROPPED"){
        address="000000EE";
        vector<string> parameters = Utility::splitString(input, ",");
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else{
            sequence="";
            noRpcRequest=true;
            this->publishError("Readonly parameter");
        }
        return sequence;
    }
    else if(endpoint=="READOUT_RATE"){
        address="000000EF";
        vector<string> parameters = Utility::splitString(input, ",");
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else{
            sequence="";
            noRpcRequest=true;
            this->publishError("Readonly parameter");
        }
        return sequence;
    }
    else if(endpoint=="IPbus_FIFO_DATA"){
        address="000000F0";
        vector<string> parameters = Utility::splitString(input, ",");
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else{
            sequence="";
            noRpcRequest=true;
            this->publishError("Readonly parameter");
        }
        return sequence;
    }
    else if(endpoint=="EVENTS_COUNT"){
        address="000000F1";
        vector<string> parameters = Utility::splitString(input, ",");
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else{
            sequence="";
            noRpcRequest=true;
            this->publishError("Readonly parameter");
        }
        return sequence;
    }
    else if(endpoint=="MCODE_TIME"){
        address="000000F7";
        vector<string> parameters = Utility::splitString(input, ",");

        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else{
            sequence="";
            noRpcRequest=true;
            this->publishError("Readonly parameter");
        }
        return sequence;
    }
    else if(endpoint=="FW_TIME"){
        address="000000FF";
        vector<string> parameters = Utility::splitString(input, ",");
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else{
            sequence="";
            noRpcRequest=true;
            this->publishError("Readonly parameter");
        }
        return sequence;
    }
    else if(endpoint=="TRIGGERS_OUTPUTS_MODE"){
        address="0000006A";
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            int num = std::stoi(parameters[0]);
            std::stringstream ss;
            ss << std::hex << num;
            std::string hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            sequence="reset\n0x001"+address+data+",write\nread";
        }
        else if(parameters.size()>2&&(parameters[0]=="0"||parameters[0]=="1")){
            int numValue = std::stoi(parameters[2]);
            int temp = 0xFFFFFFFF;
            temp-=3;
            std::stringstream ss;
            ss << std::hex << temp;
            std::string hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            temp=numValue;
            std::stringstream ss2;
            ss2 << std::hex << temp;
            std::string data2 = "";
            hex_str = ss2.str();
            for(int i=0; i<8-hex_str.length(); i++){
                data2+="0";
            }
            data2+=hex_str;
            sequence = "reset\n0x002"+address+data+",write\nread\n0x003"+address+data2+",write\nread";
        }
        else if(parameters.size()>2&&(parameters[0]=="3"||parameters[0]=="4")){
            int numValue = std::stoi(parameters[2]);
            int temp = 0xFFFFFFFF;
            temp-=24;
            std::stringstream ss;
            ss << std::hex << temp;
            std::string hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            temp=numValue*8;
            std::stringstream ss2;
            ss2 << std::hex << temp;
            std::string data2 = "";
            hex_str = ss2.str();
            for(int i=0; i<8-hex_str.length(); i++){
                data2+="0";
            }
            data2+=hex_str;
            sequence = "reset\n0x002"+address+data+",write\nread\n0x003"+address+data2+",write\nread";
        }
        else if(parameters.size()>2&&(parameters[0]=="6"||parameters[0]=="7")){
            int numValue = std::stoi(parameters[2]);
            int temp = 0xFFFFFFFF;
            temp-=192;
            std::stringstream ss;
            ss << std::hex << temp;
            std::string hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            temp=numValue*64;
            std::stringstream ss2;
            ss2 << std::hex << temp;
            std::string data2 = "";
            hex_str = ss2.str();
            for(int i=0; i<8-hex_str.length(); i++){
                data2+="0";
            }
            data2+=hex_str;
            sequence = "reset\n0x002"+address+data+",write\nread\n0x003"+address+data2+",write\nread";
        }
        else if(parameters.size()>2&&(parameters[0]=="6"||parameters[0]=="7")){
            int numValue = std::stoi(parameters[2]);
            int temp = 0xFFFFFFFF;
            temp-=1536;
            std::stringstream ss;
            ss << std::hex << temp;
            std::string hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            temp=numValue*512;
            std::stringstream ss2;
            ss2 << std::hex << temp;
            std::string data2 = "";
            hex_str = ss2.str();
            for(int i=0; i<8-hex_str.length(); i++){
                data2+="0";
            }
            data2+=hex_str;
            sequence = "reset\n0x002"+address+data+",write\nread\n0x003"+address+data2+",write\nread";
        }
        else if(parameters.size()>2&&(parameters[0]=="6"||parameters[0]=="7")){
            int numValue = std::stoi(parameters[2]);
            int temp = 0xFFFFFFFF;
            temp-=12288;
            std::stringstream ss;
            ss << std::hex << temp;
            std::string hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            temp=numValue*4096;
            std::stringstream ss2;
            ss2 << std::hex << temp;
            std::string data2 = "";
            hex_str = ss2.str();
            for(int i=0; i<8-hex_str.length(); i++){
                data2+="0";
            }
            data2+=hex_str;
            sequence = "reset\n0x002"+address+data+",write\nread\n0x003"+address+data2+",write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="3"){
            int num = std::stoi(parameters[0]);
            std::string data="";
            std::stringstream ss;
            ss << std::hex << (1 << num);
            std::string hex_str = ss.str();

            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            sequence = "reset\n0x002"+address+"FFFFFFFF,write\nread\n0x003"+address+data+",write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="2"){
            int num = std::stoi(parameters[0]);
            int temp = 0xFFFFFFFF;
            temp-=(1 << num);
            std::stringstream ss;
            ss << std::hex << temp;
            std::string data = ss.str();
            sequence = "reset\n0x002"+address+data+",write\nread\n0x003"+address+"00000000,write\nread";
        }
        else{
            this->publishError("Wrong parameters");
            sequence="";
            noRpcRequest=true;
        }
        return sequence;
    }
    else if(endpoint=="COUNTERS_UPD_RATE"){
        address="00000050";
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            int num = std::stoi(parameters[0]);
            if(!(num>=0&&num<=7)){
                this->publishError("Value out of correct ranges");
                return "";
            }
            std::stringstream ss;
            ss << std::hex << num;
            std::string hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            sequence="reset\n0x001"+address+data+",write\nread";
        }
        else{
            noRpcRequest=true;
            this->publishError("Parameter can be only read or write and range is 0-7");
            sequence="";
        }
        return sequence;
    }
    else if(endpoint=="TRG_C_RATE"){
        address="00000050";
    }
    else if(endpoint=="TRG_SC_RATE"){
        address="00000050";
    }
    else if(endpoint=="TRG_V_RATE"){
        address="00000050";
    }
    else if(endpoint=="TRG_ORC_RATE"){
        address="00000050";
    }
    else if(endpoint=="TRG_ORA_RATE"){
        address="00000050";
    }
    else if(endpoint=="DG_BUNCH_PATTERN"){
        address="000000DA";
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            long long num = 0;
            if(parameters[0].rfind("0x", 0)==0){
                num = std::stoll(parameters[0].substr(2), nullptr, 16);
            }
            else{
                num = std::stoll(parameters[0]);
            }
            std::stringstream ss;
            ss << std::hex << num;
            std::string hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            sequence="reset\n0x001"+address+data+",write\nread";
        }
        else{
            this->publishError("wrong parameters");
            sequence="";
        }
        return sequence;
    }
    else if(endpoint=="TG_SINGLE_VALUE"){
        address="000000DB";
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            long long num = 0;
            if(parameters[0].rfind("0x", 0)==0){
                num = std::stoll(parameters[0].substr(2), nullptr, 16);
            }
            else{
                num = std::stoll(parameters[0]);
            }
            std::stringstream ss;
            ss << std::hex << num;
            std::string hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            sequence="reset\n0x001"+address+data+",write\nread";
        }
        else{
            noRpcRequest=true;
            this->publishError("wrong parameters");
            sequence="";
        }
        return sequence;
    }
    else if(endpoint=="TG_PATTERN_1"){
        address="000000DC";
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            long long num = 0;
            if(parameters[0].rfind("0x", 0)==0){
                num = std::stoll(parameters[0].substr(2), nullptr, 16);
            }
            else{
                num = std::stoll(parameters[0]);
            }
            std::stringstream ss;
            ss << std::hex << num;
            std::string hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            sequence="reset\n0x001"+address+data+",write\nread";
        }
        else{
            noRpcRequest=true;
            this->publishError("wrong parameters");
            sequence="";
        }
        return sequence;
    }
    else if(endpoint=="TG_PATTERN_0"){
        address="000000DD";
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            long long num = 0;
            if(parameters[0].rfind("0x", 0)==0){
                num = std::stoll(parameters[0].substr(2), nullptr, 16);
            }
            else{
                num = std::stoll(parameters[0]);
            }
            std::stringstream ss;
            ss << std::hex << num;
            std::string hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            sequence="reset\n0x001"+address+data+",write\nread";
        }
        else{
            noRpcRequest=true;
            this->publishError("wrong parameters");
            sequence="";
        }
        return sequence;
    }
    else if(endpoint=="TG_CONT_VALUE"){
        address="000000DE";
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            long long num = 0;
            if(parameters[0].rfind("0x", 0)==0){
                num = std::stoll(parameters[0].substr(2), nullptr, 16);
            }
            else{
                num = std::stoll(parameters[0]);
            }
            std::stringstream ss;
            ss << std::hex << num;
            std::string hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            sequence="reset\n0x001"+address+data+",write\nread";
        }
        else{
            this->publishError("wrong parameters");
            sequence="";
            noRpcRequest=true;
        }
        return sequence;
    }
    else if(endpoint=="EMULATION_RATE"){
        address="000000DF";
        int indexTemp = std::stoi(parameters[0]);
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            long long num = 0;
            if(parameters[0].rfind("0x", 0)==0){
                num = std::stoll(parameters[0].substr(2), nullptr, 16);
            }
            else{
                num = std::stoll(parameters[0]);
            }
            std::stringstream ss;
            ss << std::hex << num;
            std::string hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            sequence="reset\n0x001"+address+data+",write\nread";
        }
        else if(parameters.size()>2&&(indexTemp>=0&&indexTemp<=15)){
            int numValue = 0;
            if(parameters[2].rfind("0x", 0)==0){
                numValue = std::stoi(parameters[2].substr(2), nullptr, 16);
            }
            else{
                numValue = std::stoi(parameters[2]);
            }
            int temp = 0xFFFF0000;
            std::stringstream ss;
            ss << std::hex << temp;
            std::string hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            temp=numValue;
            std::stringstream ss2;
            ss2 << std::hex << temp;
            std::string data2 = "";
            hex_str = ss2.str();
            for(int i=0; i<8-hex_str.length(); i++){
                data2+="0";
            }
            data2+=hex_str;
            sequence = "reset\n0x002"+address+data+",write\nread\n0x003"+address+data2+",write\nread";
        }
        else if(parameters.size()>2&&(indexTemp>=16&&indexTemp<=31)){
            int numValue = 0;
            if(parameters[2].rfind("0x", 0)==0){
                numValue = std::stoi(parameters[2].substr(2), nullptr, 16);
            }
            else{
                numValue = std::stoi(parameters[2]);
            }
            
            int temp = 0x0000FFFF;
            std::stringstream ss;
            ss << std::hex << temp;
            std::string hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            temp=numValue*std::pow(2,16);
            std::stringstream ss2;
            ss2 << std::hex << temp;
            std::string data2 = "";
            hex_str = ss2.str();
            for(int i=0; i<8-hex_str.length(); i++){
                data2+="0";
            }
            data2+=hex_str;
            sequence = "reset\n0x002"+address+data+",write\nread\n0x003"+address+data2+",write\nread";
        }
        else{
            this->publishError("wrong parameters");
            sequence="";
            noRpcRequest=true;
        }
        return sequence;
    }
    else if(endpoint=="SPI_MASK"){
        address="0000001E";

        if (input == ""||input == "set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            int num = std::stoi(parameters[0]);
            std::stringstream ss;
            ss << std::hex << num;
            std::string hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            sequence="reset\n0x001"+address+data+",write\nread";
        }
        else{
            this->publishError("wrong parameters");
            sequence="";
            noRpcRequest=true;
        }
        return sequence;
    }
    else if(endpoint=="TRIGGERS_SUPPRESSION_CONTROL"){
        address="0000001F";
        int index = std::stoi(parameters[0]);
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>2&&(index>=0&&index<=5)){
            int numValue = std::stoi(parameters[2]);
            int temp = 0xFFFFFFFF;
            temp-=63;
            std::stringstream ss;
            ss << std::hex << temp;
            std::string hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            temp=numValue;
            std::stringstream ss2;
            ss2 << std::hex << temp;
            std::string data2 = "";
            hex_str = ss2.str();
            for(int i=0; i<8-hex_str.length(); i++){
                data2+="0";
            }
            data2+=hex_str;
            sequence = "reset\n0x002"+address+data+",write\nread\n0x003"+address+data2+",write\nread";
        }
        else if(parameters.size()>2&&(index>=6&&index<=7)){
            int numValue = std::stoi(parameters[2]);
            int temp = 0xFFFFFFFF;
            temp-=192;
            std::stringstream ss;
            ss << std::hex << temp;
            std::string hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            temp=numValue*64;
            std::stringstream ss2;
            ss2 << std::hex << temp;
            std::string data2 = "";
            hex_str = ss2.str();
            for(int i=0; i<8-hex_str.length(); i++){
                data2+="0";
            }
            data2+=hex_str;
            sequence = "reset\n0x002"+address+data+",write\nread\n0x003"+address+data2+",write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            int num = std::stoi(parameters[0]);
            std::stringstream ss;
            ss << std::hex << num;
            std::string hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            sequence="reset\n0x001"+address+data+",write\nread";
        }
        else{
            sequence="";
            noRpcRequest=true;
            this->publishError("wrong parameters");
        }
        return sequence;
    }
    else if(endpoint=="GENERATOR_FREQ_OFFSET"){
        address="000000E0";
        int index = std::stoi(parameters[0]);
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>2&&(index>=0&&index<=11)){
            int numValue = 0;
            if(parameters[2].rfind("0x", 0)==0){
                numValue = std::stoi(parameters[2].substr(2), nullptr, 16);
            }
            else{
                numValue = std::stoi(parameters[2]);
            }
            int temp = 0xFFFFF000;
            std::stringstream ss;
            ss << std::hex << temp;
            std::string hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            temp=numValue;
            std::stringstream ss2;
            ss2 << std::hex << temp;
            std::string data2 = "";
            hex_str = ss2.str();
            for(int i=0; i<8-hex_str.length(); i++){
                data2+="0";
            }
            data2+=hex_str;
            sequence = "reset\n0x002"+address+data+",write\nread\n0x003"+address+data2+",write\nread\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>2&&(index>=12&&index<=15)){
            int numValue = 0;
            if(parameters[2].rfind("0x", 0)==0){
                numValue = std::stoi(parameters[2].substr(2), nullptr, 16);
            }
            else{
                numValue = std::stoi(parameters[2]);
            }
            int temp = 0xFFFF0FFF;
            std::stringstream ss;
            ss << std::hex << temp;
            std::string hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            temp=numValue*std::pow(2,12);
            std::stringstream ss2;
            ss2 << std::hex << temp;
            std::string data2 = "";
            hex_str = ss2.str();
            for(int i=0; i<8-hex_str.length(); i++){
                data2+="0";
            }
            data2+=hex_str;
            sequence = "reset\n0x002"+address+data+",write\nread\n0x003"+address+data2+",write\nread\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>2&&(index>=16&&index<=27)){
            int numValue = 0;
            if(parameters[2].rfind("0x", 0)==0){
                numValue = std::stoi(parameters[2].substr(2), nullptr, 16);
            }
            else{
                numValue = std::stoi(parameters[2]);
            }
            int temp = 0xF000FFFF;
            std::stringstream ss;
            ss << std::hex << temp;
            std::string hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            temp=numValue*std::pow(2,16);
            std::stringstream ss2;
            ss2 << std::hex << temp;
            std::string data2 = "";
            hex_str = ss2.str();
            for(int i=0; i<8-hex_str.length(); i++){
                data2+="0";
            }
            data2+=hex_str;
            sequence = "reset\n0x002"+address+data+",write\nread\n0x003"+address+data2+",write\nread\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>2&&(index>=28&&index<=31)){
            long long numValue = 0;
            if(parameters[2].rfind("0x", 0)==0){
                numValue = std::stoll(parameters[2].substr(2), nullptr, 16);
            }
            else{
                numValue = std::stoll(parameters[2]);
            }
            long long temp = 0x0FFFFFFF;
            std::stringstream ss;
            ss << std::hex << temp;
            std::string hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            temp=numValue*std::pow(2,28);
            std::stringstream ss2;
            ss2 << std::hex << temp;
            std::string data2 = "";
            hex_str = ss2.str();
            for(int i=0; i<8-hex_str.length(); i++){
                data2+="0";
            }
            data2+=hex_str;
            sequence = "reset\n0x002"+address+data+",write\nread\n0x003"+address+data2+",write\nread\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            int num = 0;
            if(parameters[0].rfind("0x", 0)==0){
                num = std::stoi(parameters[0].substr(2), nullptr, 16);
            }
            else{
                num = std::stoi(parameters[0]);
            }
            std::stringstream ss;
            ss << std::hex << num;
            std::string hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            sequence="reset\n0x001"+address+data+",write\nread";
        }
        else{
            this->publishError("Value not to set");
            sequence="";
        }
        return sequence;
    }
    else if(endpoint=="LASER_PATTERN_1"){
        address="0000001C";
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            long long num = 0;
            if(parameters[0].rfind("0x", 0)==0){
                num = std::stoll(parameters[0].substr(2), nullptr, 16);
            }
            else{
                num = std::stoll(parameters[0]);
            }
            std::stringstream ss;
            ss << std::hex << num;
            std::string hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            sequence="reset\n0x001"+address+data+",write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="3"){
            int num = std::stoi(parameters[0]);
            std::string data="";
            std::stringstream ss;
            ss << std::hex << (1 << num);
            std::string hex_str = ss.str();

            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            sequence = "reset\n0x002"+address+"FFFFFFFF,write\nread\n0x003"+address+data+",write\nread\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="2"){
            int num = std::stoi(parameters[0]);
            int temp = 0xFFFFFFFF;
            temp-=(1 << num);
            std::stringstream ss;
            ss << std::hex << temp;
            std::string data = ss.str();
            sequence = "reset\n0x002"+address+data+",write\nread\n0x003"+address+"00000000,write\nread\n0x000"+address+"00000000,write\nread";
        }
        else{
            this->publishError("wrong parameters");
            sequence="";
            noRpcRequest=true;
        }
        return sequence;
    }
    else if(endpoint=="LASER_PATTERN_0"){
        address="0000001D";
        if(input==""||input=="set"||(parameters.size()>1&&parameters[1]=="0")){
            sequence = "reset\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            long long num = 0;
            if(parameters[0].rfind("0x", 0)==0){
                num = std::stoll(parameters[0].substr(2), nullptr, 16);
            }
            else{
                num = std::stoll(parameters[0]);
            }
            std::stringstream ss;
            ss << std::hex << num;
            std::string hex_str = ss.str();
            std::string data="";
            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            sequence="reset\n0x001"+address+data+",write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="3"){
            int num = std::stoi(parameters[0]);
            std::string data="";
            std::stringstream ss;
            ss << std::hex << (1 << num);
            std::string hex_str = ss.str();

            for(int i=0; i<8-hex_str.length(); i++){
                data+="0";
            }
            data+=hex_str;
            sequence = "reset\n0x002"+address+"FFFFFFFF,write\nread\n0x003"+address+data+",write\nread\n0x000"+address+"00000000,write\nread";
        }
        else if(parameters.size()>1&&parameters[1]=="2"){
            int num = std::stoi(parameters[0]);
            int temp = 0xFFFFFFFF;
            temp-=(1 << num);
            std::stringstream ss;
            ss << std::hex << temp;
            std::string data = ss.str();
            sequence = "reset\n0x002"+address+data+",write\nread\n0x003"+address+"00000000,write\nread\n0x000"+address+"00000000,write\nread";
        }
        else{
            this->publishError("wrong parameters");
            sequence="";
            noRpcRequest=true;
        }
        return sequence;
    }
    if (input == ""||input == "set"||(parameters.size()>1&&parameters[1]=="0")){
        if(address==""){
            Print::PrintError(endpoint);
            address="00000000";
        }
        sequence = "reset\n0x000"+address+"00000000,write\nread";
    }
    else if(input=="error"){
        noRpcRequest=true;
        Print::PrintInfo("error");
        this->publishError("Error!");
    }
    else if(parameters[1]=="1"||parameters[1]=="2"||parameters[1]=="3"){
        string data="";
        int num = std::stoi(parameters[0]);
        if(endpoint=="SIDE_A_STATUS"){
            if(parameters.size()>1&&parameters[1]=="3"){
                std::stringstream ss;
                ss << std::hex << (1 << num);
                std::string hex_str = ss.str();
                data="";
                for(int i=0; i<8-hex_str.length(); i++){
                    data+="0";
                }
                data+=hex_str;
                sequence = "reset\n0x0020000001AFFFFFFFF,write\nread\n0x0030000001A"+data+",write\nread";
            }
            else if(parameters[0].length()>1&&parameters[1]=="2"){
                int temp = 0xFFFFFFFF;
                temp-=(1 << num);
                std::stringstream ss;
                ss << std::hex << temp;
                data = ss.str();
                sequence = "reset\n0x0020000001A"+data+",write\nread\n0x0030000001A00000000,write\nread";
            }
        }
        else if(endpoint=="SIDE_C_STATUS"){
            if(parameters.size()>1&&parameters[1]=="3"){
                std::stringstream ss;
                ss << std::hex << (1 << num);
                std::string hex_str = ss.str();
                data="";
                for(int i=0; i<8-hex_str.length(); i++){
                    data+="0";
                }
                data+=hex_str;
                sequence = "reset\n0x0020000003AFFFFFFFF,write\nread\n0x0030000003A"+data+",write\nread";
            }
            else if(parameters[0].length()>1&&parameters[1]=="2"){
                int temp = 0xFFFFFFFF;
                temp-=(1 << num);
                std::stringstream ss;
                ss << std::hex << temp;
                data = ss.str();
                sequence = "reset\n0x0020000003A"+data+",write\nread\n0x0030000003A00000000,write\nread";
            }
        }
        else if(parameters.size()>1&&parameters[1]=="1"){
            if(parameters[0].length()>2&&parameters[0].substr(0,2)=="0x"){
                parameters[0]=parameters[0].substr(2);
                data+=parameters[0];
                
                int num = std::stoi(data);

                std::stringstream ss;
                ss << std::hex << num;
                std::string hex_str = ss.str();
                for(int i=0; i<8-hex_str.length(); i++){
                    data+="0";
                }
                data+=hex_str;
                this->publishAnswer(data);
            }
            else{
                int num = std::stoi(parameters[0]);
                std::stringstream ss;
                ss << std::hex << num;
                std::string hex_str = ss.str();
                for(int i=0; i<8-hex_str.length(); i++){
                    data+="0";
                }
                data+=hex_str;
                this->publishAnswer(data);
            }
            data="00000000";
            sequence = "reset\n0x001"+address+data+",write\nread";
        }
        //this->publishAnswer(data);
        /*if(endpoint!="SIDE_A_STATUS"&&endpoint!="SIDE_C_STATUS"&&endpoint!="MODE"){
            this->publishAnswer(data);
            sequence = "reset\n0x001"+address+data+",write\nread"; 
            Print::PrintInfo("write sequence:");
            Print::PrintInfo(sequence);
        }*/
    }
    return sequence;
}

string Default2::processOutputMessage(string output) {
  string value;

  try {
    output.erase(remove(output.begin(), output.end(), '\n'), output.end());
    value = output.substr(output.size() - 8, output.size());
    finalValue = stoll(value, nullptr, 16);
    if(endpoint=="FPGA_TEMP"){
        return std::to_string(finalValue * 503.975 / 65536 - 273.15);
    }
    else if(endpoint=="1VPOWER"){
        return std::to_string(finalValue * 3 / 65536.0);
    }
    else if(endpoint=="18VPOWER"){
        return std::to_string(finalValue * 3 / 65536.0);
    }
    else if(endpoint=="TRG_C_RATE"){
        return std::to_string(tcm.temp.trigger1rate);
    }
    else if(endpoint=="TRG_SC_RATE"){
        return std::to_string(tcm.temp.trigger2rate);
    }
    else if(endpoint=="TRG_V_RATE"){
        return std::to_string(tcm.temp.trigger3rate);
    }
    else if(endpoint=="TRG_ORC_RATE"){
        return std::to_string(tcm.temp.trigger4rate);
    }
    else if(endpoint=="TRG_ORA_RATE"){
        return std::to_string(tcm.temp.trigger5rate);
    }
    else if(endpoint=="LASER_DELAY"){
        Print::PrintInfo(value);
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
        return std::to_string(tempValue);
    }
    else if(endpoint=="LASER_DIVIDER"){
        float systemClock_MHz = tcm.act.externalClock?40.0789658:40.;
        Print::PrintInfo(value.substr(0,6));
        long long tempValue = std::stoll(value.substr(2,6), nullptr, 16);
        Print::PrintInfo(std::to_string(tempValue));
        Print::PrintInfo(std::to_string(systemClock_MHz*std::pow(10,6)));
        Print::PrintInfo(std::to_string(tempValue==0?1<<24:tempValue));
        Print::PrintInfo(std::to_string(systemClock_MHz*std::pow(10,6)/(tempValue==0?1<<24:tempValue)));
        float laserFrequency = systemClock_MHz*std::pow(10,6)/(tempValue==0?1<<24:tempValue);
        updateTopicAnswer("READOUTCARDS/TCM0/LASER_FREQUENCY", std::to_string(laserFrequency));
    }
    noReturn=false;
  }
  catch (exception &e) {
    Print::PrintInfo(e.what());
    Print::PrintError("ERROR in connection with ALF!");
    finalValue=0;
  }

  return to_string(finalValue);
}