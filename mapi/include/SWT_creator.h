

class SWT_creator{
public:
    SWT_creator();

    static void sequenceOperation1(long long num, std::string address, std::string& sequence){
    std::stringstream ss;
    ss << std::hex << num;
    std::string hex_str = ss.str();
    if(hex_str.length()>8){
        hex_str=hex_str.substr(hex_str.length()-4);
    }
    std::string data="";
    for(int i=0; i<8-hex_str.length(); i++){
        data+="0";
    }
    data+=hex_str;
    sequence="reset\n0x001"+address+data+",write\nread";
}

static void sequenceOperation2(int num, std::string address, std::string& sequence){
    int temp = 0xFFFFFFFF;
    temp-=(1 << num);
    std::stringstream ss;
    ss << std::hex << temp;
    std::string data = ss.str();
    sequence = "reset\n0x002"+address+data+",write\nread\n0x003"+address+"00000000,write\nread\n0x000"+address+"00000000,write\nread";
}

static void sequenceOperation3(int num, std::string address, std::string& sequence){
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

static void sequenceOperationBits(int num, int power, int maskNumber, std::string address, std::string& sequence){
    std::stringstream ss;
    ss << std::hex << maskNumber;
    std::string hex_str = ss.str();
    if(hex_str.length()>8){
        hex_str=hex_str.substr(hex_str.length()-4);
    }
    std::string data="";
    for(int i=0; i<8-hex_str.length(); i++){
        data+="0";
    }
    data+=hex_str;
    long long temp=num*std::pow(2,power);
    std::stringstream ss2;
    ss2 << std::hex << temp;
    std::string data2 = "";
    hex_str = ss2.str();
    if(hex_str.length()>8){
        hex_str=hex_str.substr(hex_str.length()-4);
    }
    for(int i=0; i<8-hex_str.length(); i++){
        data2+="0";
    }
    data2+=hex_str;
    sequence = "reset\n0x002"+address+data+",write\nread\n0x003"+address+data2+",write\nread\n0x000"+address+"00000000,write\nread";
}

static long long parameterValue(std::string strValue){
    long long inputValue;
    if(strValue.rfind("0x", 0)==0){
        return std::stoll(strValue.substr(2), nullptr, 16);
    }
    else{
        return std::stoll(strValue);
    }
}

static std::string numberLetter(int number){
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
    if(numberLetterStr.length()==1){
        numberLetterStr="0"+numberLetterStr;
    }
    return numberLetterStr;
}

};