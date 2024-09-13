#include "WinccMessage.h"
#include "Parser/utility.h"
#include "swtCreator.h"


WinccMessage::WinccMessage(std::string input) : parameters(Utility::splitString(input, ",")) {}

//read - READ
//write - WRITE,,(value)
//write word on bits - WRITE,(field_name),(value)


bool WinccMessage::readMessage() const {
    return (parameters.size()>1&&parameters[1]=="0")||parameters[0]=="read";
}

bool WinccMessage::writeMessage() const {
    return parameters.size()>1&&parameters[1]=="1";
}

bool WinccMessage::orAndMessage() const {
    return parameters.size()>1&&(parameters[1]=="2"||parameters[1]=="3");
}

bool WinccMessage::orMessage() const {
    return parameters.size()>1&&parameters[1]=="2";
}

bool WinccMessage::writeWordMessage() const {
    return parameters.size()>2&&(parameters[1]=="2"||parameters[1]=="3");
}

int64_t WinccMessage::getValueWriteMessage() const {
    return SwtCreator::parameterValue(parameters[0]);
}

int64_t WinccMessage::getIndexAndOrWriteMessage() const {
    return SwtCreator::parameterValue(parameters[0]);
}

int64_t WinccMessage::getValueWriteWordMessage() const {
    return SwtCreator::parameterValue(parameters[2]);
}
