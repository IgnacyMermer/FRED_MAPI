#include "Fred/Mapi/mapi.h"


class Device_default: public Mapi
{
public:
    Device_default(std::string endpoint, std::string address);

    string processInputMessage(string input);
    string processOutputMessage(string output);

    uint32_t finalValue;
    string sequence, endpoint, address;
};