#include "Fred/Mapi/mapi.h"

class RefreshTCM: public Mapi{
public:
    RefreshTCM();

    string processInputMessage(string input);
    string processOutputMessage(string output);

    float finalValue;
    bool firstTime;
    string sequence;
    int count;
    std::vector<string> services;
    std::vector<long long> oldValues;
};