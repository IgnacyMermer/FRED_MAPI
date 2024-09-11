#include "Fred/Mapi/mapi.h"


class Register: public Mapi
{
public:
    Register(std::string endpoint, std::string address, std::vector<std::vector<int64_t>> words, std::pair<std::string, std::string> equation);

    string processInputMessage(string input);
    string processOutputMessage(string output);

    int64_t finalValue;
    string sequence, endpoint, address;
    std::vector<std::vector<int64_t>> words;
    std::pair<std::string, std::string> equation;
};