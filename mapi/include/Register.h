#include "Fred/Mapi/mapi.h"


class Register: public Mapi
{
public:
    Register(std::string endpoint, std::string address);

    string processInputMessage(string input);
    string processOutputMessage(string output);

    uint32_t finalValue;
    string sequence, endpoint, address;
};