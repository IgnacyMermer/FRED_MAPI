#include "Fred/Mapi/mapi.h"

class RefreshPMCounters: public Mapi
{
public:
    RefreshPMCounters();

    string processInputMessage(string input);
    string processOutputMessage(string output);

    float finalValue;
    bool firstTime;
    string sequence;
    std::vector<std::string> services;
    std::vector<std::string> servicesRates;
    std::vector<long long> oldValues;
    std::vector<int> oldTimes;
    std::vector<float> oldValuesRates;
};