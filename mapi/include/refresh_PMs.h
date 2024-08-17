#include "Fred/Mapi/mapi.h"
#include "Fred/Mapi/iterativemapi.h"

class RefreshPMs: public Mapi
{
public:
    RefreshPMs();

    string processInputMessage(string input);
    string processOutputMessage(string output);

    float finalValue;
    bool firstTime;
    string sequence;
    std::vector<std::string> services;
    std::vector<long long> oldValues;
};