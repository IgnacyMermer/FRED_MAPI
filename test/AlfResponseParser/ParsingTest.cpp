#include"AlfResponseParser.h"
#include<iostream>
#include<iomanip>
int main(int argc, const char** argv)
{
    const char* data = "success\n0x001A0B0C0D0AABBCCDD\n0x002F0E0A0C011223344\n0";
    AlfResponseParser parser(data);
    for(auto line: parser)
    {
        if(line.type == AlfResponseParser::Line::Type::ResponseToWrite)
        std::cout << "0" <<std::endl;
        else
        std::cout << std::hex << line.frame.prefix <<  ' ' << std::hex  << line.frame.address << ' ' << std::hex << line.frame.data << std::endl;
    }
    return 0;
}